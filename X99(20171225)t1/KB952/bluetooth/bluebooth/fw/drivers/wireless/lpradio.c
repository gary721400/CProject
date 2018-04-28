//! @file lpradio.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: lpradio.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _lpradio_c_
#define _lpradio_c_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "lib_mcu\compiler.h"         // Compiler definitions
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "sio\com.h"

#pragma DATA = LPRADIO_DATA
#pragma CODE = LPRADIO_CODE

XACT_CONFIG RadioXactConfig;
RADIO_STATE RadioState;
U8          RadioRetryCount;
U8          RadioPacketLength;

//_____ M A C R O S ________________________________________________________

U8 const RadioPnCodeTable[ 20 ][ 8 ] =
{
   { 0x3C, 0x37, 0xCC, 0x91, 0xE2, 0xF8, 0xCC, 0x91 }    //PN Code 0
,  { 0x9B, 0xC5, 0xA1, 0x0F, 0xAD, 0x39, 0xA2, 0x0F }    //PN Code 1
,  { 0xEF, 0x64, 0xB0, 0x2A, 0xD2, 0x8F, 0xB1, 0x2A }    //PN Code 2
,  { 0x66, 0xCD, 0x7C, 0x50, 0xDD, 0x26, 0x7C, 0x50 }    //PN Code 3
,  { 0x5C, 0xE1, 0xF6, 0x44, 0xAD, 0x16, 0xF6, 0x44 }    //PN Code 4
,  { 0x5A, 0xCC, 0xAE, 0x46, 0xB6, 0x31, 0xAE, 0x46 }    //PN Code 5
,  { 0xA1, 0x78, 0xDC, 0x3C, 0x9E, 0x82, 0xDC, 0x3C }    //PN Code 6
,  { 0xB9, 0x8E, 0x19, 0x74, 0x6F, 0x65, 0x18, 0x74 }    //PN Code 7
,  { 0xDF, 0xB1, 0xC0, 0x49, 0x62, 0xDF, 0xC1, 0x49 }    //PN Code 8
,  { 0x97, 0xE5, 0x14, 0x72, 0x7F, 0x1A, 0x14, 0x72 }    //PN Code 9

,  { 0x82, 0xC7, 0x90, 0x36, 0x21, 0x03, 0xFF, 0x17 }    //PN Code 10
,  { 0xE2, 0xF8, 0xCC, 0x91, 0x3C, 0x37, 0xCC, 0x91 }    //PN Code 11
,  { 0xAD, 0x39, 0xA2, 0x0F, 0x9B, 0xC5, 0xA1, 0x0F }    //PN Code 12
,  { 0xD2, 0x8F, 0xB1, 0x2A, 0xEF, 0x64, 0xB0, 0x2A }    //PN Code 13
,  { 0xDD, 0x26, 0x7C, 0x50, 0x66, 0xCD, 0x7C, 0x50 }    //PN Code 14
,  { 0xAD, 0x16, 0xF6, 0x44, 0x5C, 0xE1, 0xF6, 0x44 }    //PN Code 15
,  { 0xB6, 0x31, 0xAE, 0x46, 0x5A, 0xCC, 0xAE, 0x46 }    //PN Code 16
,  { 0x9E, 0x82, 0xDC, 0x3C, 0xA1, 0x78, 0xDC, 0x3C }    //PN Code 17
,  { 0x6F, 0x65, 0x18, 0x74, 0xB9, 0x8E, 0x19, 0x74 }    //PN Code 18
,  { 0x62, 0xDF, 0xC1, 0x49, 0xDF, 0xB1, 0xC0, 0x49 }    //PN Code 19

};


//_____ D E F I N I T I O N ________________________________________________

void                RadioSetFrequency       ( U8 frequency );
U8                  RadioGetChannel         ( void );
U8                  RadioGetFrequency       ( void );
TX_CONFIG           RadioGetTxConfig        ( void );
RADIO_FRAME_CONFIG  RadioGetFrameConfig     ( void );
U8                  RadioGetThreshold32     ( void );
void                RadioSetThreshold64     ( U8 threshold );
U8                  RadioGetThreshold64     ( void );
U8                  RadioGetPreambleCount   ( void );
void                RadioSetPreamblePattern ( U16 preamblepattern );
U16                 RadioGetPreamblePattern ( void );
U16                 RadioGetCrcSeed         ( void );
void                RadioSetConstSopPnCode  ( const U8 *patternAddr );
void                RadioSetConstDataPnCode ( const U8 *patternAddr );


//_____ D E C L A R A T I O N ______________________________________________
//! RadioInit
//!
//! Initialize the Radio
//!
//! @return
void RadioInit( XACT_CONFIG xactConfig, TX_CONFIG txConfig )
{
  U8 temp;
 
  RadioXactConfig = XACT_CFG_RST;
  RadioReset();

  //We're going to want to force the RXF clock on in the streaming driver.
  //Not necessary for nonstreaming driver but harmless
  RadioWrite( CLK_EN_ADR, MSK_RXF );                            //RXF

  //Set up to use auto-cal for VCO. Register 0x34 is left at its default value to
  //allow auto-cal to be used
  RadioWrite( AUTO_CAL_TIME_ADR, AUTO_CAL_TIME_MAX );           //Extend autoCal time to max.
   
  RadioWrite( AUTO_CAL_OFFSET_ADR, AUTO_CAL_OFFSET_MINUS_4 );   //AutoCal offset to -4.

  temp = RadioRead( IO_CFG_ADR );     //Read IO_CFG_ADR, user may have changed it in RadioReset
  temp |= MSK_IRQ_POL;                //Set IRQ polarity to positive
  RadioWrite( IO_CFG_ADR, temp );     //Some have asked that this be left at the default negative

  //Set the XTAL Startup delay to 150uSec to handle warm restarts of the XTAL
  RadioWrite( XTAL_CFG_ADR, 0x08 );

  //Enable HiLo for quick-turn-around. Use low side injection for receive - this should result in
  //non-inverted data, so no need to hit the invert bit. Turn off AGC and force the LNA on
  RadioWrite( RX_CFG_ADR, ( RX_CFG_RST | MSK_FASTTURN_EN | MSK_LNA_EN )& ~( MSK_HI | MSK_RXOW_EN | MSK_AUTO_AGC_EN ));

  //Set the TX Offset to +1MHz.

  //THIS MEANS THE ACTUAL TRANSMIT CARRIER WILL BE 1MHz ABOVE THE PLL
  //FREQUENCY PROGRAMMED IN THE CHANNEL OR A & N REGISTERS.
  //RadioSetFrequency COMPENSATES FOR THIS OFFSET
  RadioWrite( TX_OFFSET_LSB_ADR, 0x55 );
  RadioWrite( TX_OFFSET_MSB_ADR, 0x05 );
  //RadioWrite( RX_OVERRIDE_ADR, ( MSK_FRC_RXDR | MSK_DIS_RXCRC ));

  //Set the radio transaction and TX configuration
  RadioSetXactConfig( xactConfig );
  RadioSetTxConfig( txConfig );

  RadioState = RADIO_IDLE;
}


//! RadioSetChannel
//!
//! Set the channel (frequency + 2MHz)
//!
//! @return
void RadioSetChannel( U8 channel )
{
  channel += 2;
  RadioSetFrequency( channel );
}


//! RadioSetFrequency
//!
//! Set the frequency on which future communications will occur
//!
//! @return
void RadioSetFrequency( U8 frequency )
{
  if( frequency )
    frequency--;
  RadioWrite( CHANNEL_ADR, frequency );
}

#if 0
//! RadioGetChannel
//!
//! Get the channel
//!
//! @return
U8 RadioGetChannel( void )
{
  U8 channel;
  channel = RadioGetFrequency();
  channel -= 2 ;
  return channel;
}


//! RadioGetFrequency
//!
//! Returns the frequency in MHz above 2.4GHz
//! Example: 0 means 2.400GHz, 83 means 2.483GHz
//! @return
U8 RadioGetFrequency( void )
{
  U8 frequency;
  frequency = RadioRead( CHANNEL_ADR );
  frequency++;
  return frequency;
}
#endif


//! RadioGetChannel
//!
//! Set the transmitter configuration
//!
//! @return
void RadioSetTxConfig( TX_CONFIG config )
{
  RadioWrite( TX_CFG_ADR, config );
}


#if 0
//! RadioGetTxConfig
//!
//! Return the transmitter configuration
//!
//! @return
TX_CONFIG RadioGetTxConfig( void )
{
  TX_CONFIG config;
  config = RadioRead( TX_CFG_ADR );
  return config;
}
#endif


//! RadioSetXactConfig
//!
//! Set the transaction configuration
//!
//! @return 
//!
void RadioSetXactConfig( XACT_CONFIG config )
{
  RadioXactConfig = config;
  RadioXactConfig &= ~MSK_FRC_END_STATE;
  RadioWrite( XACT_CFG_ADR, config );
}

#if 0
//! RadioGetXactConfig
//!
//! Return the transaction configuration
//!
//! @return 
//!
XACT_CONFIG RadioGetXactConfig( void )
{
  XACT_CONFIG config;
  config = RadioRead( XACT_CFG_ADR );
  return config;
}
#endif

//! RadioSetFrameConfig
//!
//! Set the framing configuration
//!
//! @return
void RadioSetFrameConfig( RADIO_FRAME_CONFIG config )
{
  RadioWrite( FRAMING_CFG_ADR, config );
}


#if 0
//! RadioGetFrameConfig
//!
//! Set the framing configuration
//!
//! @return
RADIO_FRAME_CONFIG RadioGetFrameConfig( void )
{
  RADIO_FRAME_CONFIG config;
  config = RadioRead( FRAMING_CFG_ADR );
  return config;
}
#endif


#if 0
//! RadioSetThreshold32
//!
//! Set the threshold for the 32 chip data modes
//!
//! @return
void RadioSetThreshold32( U8 threshold )
{
  RadioWrite( DATA32_THOLD_ADR, threshold );
}


//! RadioGetThreshold32
//!
//! Return the threshold for the 32 chip data modes
//!
//! @return
U8 RadioGetThreshold32( void )
{
  U8 threshold;
  threshold = RadioRead( DATA32_THOLD_ADR );
  return threshold;
}


//! RadioSetThreshold64
//!
//! Set the threshold for the 64 chip data modes
//!
//! @return
void RadioSetThreshold64( U8 threshold )
{
  RadioWrite( DATA64_THOLD_ADR, threshold );
}


//! RadioGetThreshold64
//!
//! Return the threshold for the 32 chip data modes
//!
//! @return
U8 RadioGetThreshold64( void )
{
  U8 threshold;
  threshold = RadioRead( DATA64_THOLD_ADR );
  return threshold;
}
#endif

//! RadioSetPreambleCount
//!
//! Start the transmission of a packet
//!
//! @return
void RadioSetPreambleCount( U8 preamblecount )
{
  U8 temp[ 2 ];
  RadioWrite( PREAMBLE_ADR, preamblecount );
  //RadioRead( PREAMBLE_ADR );
  //RadioRead( PREAMBLE_ADR );
  RadioWipPtr = temp;
  RadioFileReadWip( PREAMBLE_ADR, 2 );
}

#if 0
//! RadioGetPreambleCount
//!
//! Get the preamble repetition count
//!
//! @return
U8 RadioGetPreambleCount( void )
{
  /*U8 preamblecount;
  preamblecount = RadioRead( PREAMBLE_ADR );
  RadioRead( PREAMBLE_ADR );
  RadioRead( PREAMBLE_ADR );
  return preamblecount;*/
  U8 temp[ 3 ];
  RadioWipPtr = temp;
  RadioFileReadWip( PREAMBLE_ADR, 3 );
  return temp[ 0 ];
}


//! RadioSetPreamblePattern
//!
//! Set the preamble pattern
//!
//! @return
void RadioSetPreamblePattern( U16 preamblepattern )
{
  RadioRead( PREAMBLE_ADR );
  //RadioWrite( PREAMBLE_ADR, LSB( preamblepattern ));
  //RadioWrite( PREAMBLE_ADR, MSB( preamblepattern ));
  RadioWipPtr = ( U8 *)&preamblepattern;
  RadioFileWriteWip( PREAMBLE_ADR, 2 );
}


//! RadioGetPreamblePattern
//!
//! Get the preamble pattern
//!
//! @return
U16 RadioGetPreamblePattern( void )
{
  U16 preamblepattern;
  RadioRead( PREAMBLE_ADR );
  LSB( preamblepattern ) = RadioRead( PREAMBLE_ADR );
  MSB( preamblepattern ) = RadioRead( PREAMBLE_ADR );
  return preamblepattern;
}
#endif


//! RadioSetCrcSeed
//!
//! Sets the value used as the CRC seed value for both transmit and receive
//!
//! @return
void RadioSetCrcSeed( U16 crcSeed )
{
  RadioWrite( CRC_SEED_LSB_ADR, LSB( crcSeed ));
  RadioWrite( CRC_SEED_MSB_ADR, MSB( crcSeed ));
  //RadioWipPtr = ( U8 *)&crcSeed;
  //RadioBurstWriteWip( CRC_SEED_LSB_ADR, 2 );
}


#if 0
//! RadioGetCrcSeed
//!
//! Returns the value used as the CRC seed value for both transmit and receive
//!
//! @return
U16 RadioGetCrcSeed( void )
{
  U16 crcSeed;
  //LSB( crcSeed ) = RadioRead( CRC_SEED_LSB_ADR );
  //MSB( crcSeed ) = RadioRead( CRC_SEED_MSB_ADR );
  RadioWipPtr = ( U8 *)&crcSeed;
  RadioBurstReadWip( CRC_SEED_LSB_ADR, 2 );
  return crcSeed;
}
#endif


#if 1
//! RadioGetFuses
//!
//! Get the fuse values from the radio
//!
//! @return
void RadioGetFuses( void )
{
  U8 length = 6;
  U8 *p = RadioPtr;
  RadioWrite( MFG_ID_ADR, 0xFF );  //Turn on fuse read bias

  RadioFileRead( MFG_ID_ADR, 6 );
  do
    *p++ ^= 0xFF;
  while( --length );

  RadioWrite( MFG_ID_ADR, 0x00 );  //Turn off fuse read bias
}
#endif


//! RadioSetConstSopPnCode
//!
//! Set the Start Of Packet PN Code
//!
//! @return
void RadioSetConstSopPnCode( const U8 *patternAddr )
{
  U8 RadioCount = 8;
  do
    RadioWrite( SOP_CODE_ADR, *patternAddr++ );
  while( --RadioCount );
}


#if 0
//! RadioSetConstDataPnCode
//!
//! Set the Start Of Packet PN Code
//!
//! @return
void RadioSetConstDataPnCode( const U8 *patternAddr )
{
  U8 RadioCount = 16;
  do
    RadioWrite( DATA_CODE_ADR, *patternAddr++ );
  while( --RadioCount );
}
#endif


//! RadioSetSopPnCode
//!
//! Set the Start Of Packet PN Code
//!
//! @return
void RadioSetSopPnCode( U8 patternNum )
{
  RadioSetConstSopPnCode( &RadioPnCodeTable[ patternNum ][ 0 ]);
}


#endif  // _lpradio_c_
