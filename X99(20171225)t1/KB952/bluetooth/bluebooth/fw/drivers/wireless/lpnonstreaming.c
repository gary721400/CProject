//! @file lpnonstreaming.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: lpnonstreaming.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _lpnonstreaming_c_
#define _lpnonstreaming_c_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "lib_mcu\compiler.h"         // Compiler definitions
#include "lpnonstreaming.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "protocol.h"
#include "lib_mcu\int\int_drv.h"                // Interrupt routines
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
#include "drivers\oid\oid.h"
#include "sio\com.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\power\power.h"             // Power module definition

#pragma DATA = LPNONSTREAMING_DATA
#pragma CODE = LPNONSTREAMING_CODE

//_____ M A C R O S ________________________________________________________
#define  LP_FIFO_SIZE         16

//_____ D E F I N I T I O N ________________________________________________


void              RadioStartTransmit      ( U8 retryCount, RADIO_LENGTH length );
void              RadioRestartTransmit    ( void );
RADIO_STATE       RadioGetTransmitState   ( void );
void              RadioEndTransmit        ( void );
void              RadioRxCleanup          ( void );
void              RadioForceState         ( XACT_CONFIG endStateBitsOnly );
void              forceState              ( XACT_CONFIG endState );
void              frcState                ( XACT_CONFIG endState );
void              wakeupRadio             ( void );
RADIO_RX_STATUS   RadioGetReceiveStatus   ( void );

//_____ D E C L A R A T I O N ______________________________________________
//! RadioStartTransmit
//!
//! Start the transmission of a packet
//!
//! @return none
void RadioStartTransmit( U8 retryCount, RADIO_LENGTH length )
{
  RadioRetryCount = retryCount;
  RadioPacketLength = length;
  RadioRestartTransmit();
}


//! RadioRestartTransmit
//!
//! ReStart the transmission of a packet
//!
//! @return none
void RadioRestartTransmit( void )
{
  //wakeupRadio();
  RadioState = RADIO_TX;
  RadioWrite( TX_LENGTH_ADR, RadioPacketLength );
  RadioWrite( TX_CTRL_ADR, MSK_TX_GO | MSK_TX_CLR | MSK_TXC_IRQ | MSK_TXE_IRQ );
  /*RadioTemp2 = MSK_TX_GO | MSK_TX_CLR | MSK_TXC_IRQ | MSK_TXE_IRQ;
  RadioWipPtr = &RadioTemp1;
  RadioWipLen = 2;
  RadioBurstWriteWip( TX_LENGTH_ADR, 2 );*/
  RadioFileWrite( TX_BUFFER_ADR, RadioPacketLength );

}


//! RadioGetTransmitState
//!
//! Returns the state of the current transmit operation
//!
//! @return
//!
RADIO_STATE RadioGetTransmitState( void )
{
  if( IRQ )
  {
#if 0//def TX_DEBUG
	SendString((U8 *)"\r\n IRQ!");
#else
	delay1ms(1);
#endif
    RadioGetTransmitStateIsr();
    
  }
  return RadioState;  
}


//! RadioGetTransmitStateIsr
//!
//! Returns the state of the current transmit operation
//!
//! @return 
//!
void RadioGetTransmitStateIsr( void )
{
  RADIO_STATE status;
  //Something interesting is happening at the IRQ, find out what
  status = RadioReadStatusDebounced( TX_IRQ_STATUS_ADR );
  status &= MSK_TXC_IRQ | MSK_TXE_IRQ;
  RadioState |= status;
  if( status == MSK_TXC_IRQ )
    return;

  //Reading the RX_IRQ_STATUS register will clear up the IRQ. We don't need to do the read if there is just a
  //simple error, but no harm
  ( void )RadioRead( RX_IRQ_STATUS_ADR );
  RadioState |= MSK_TXC_IRQ | MSK_TXE_IRQ;
  if( RadioRetryCount-- )
  {
//    RadioRetryCount--;
    RadioRestartTransmit();
  }
}


//! RadioEndTransmit
//!
//! Completes a transmit operation
//!
//! @return none
void RadioEndTransmit( void )
{
  RadioState = RADIO_IDLE;
}


//! RadioBlockingTransmit
//!
//! Transmit a packet
//!
//! @return none
RADIO_STATE RadioBlockingTransmit( U8 retryCount, RADIO_LENGTH length )
{
  RADIO_STATE RadioTemp2;
  RadioStartTransmit( retryCount, length );
  do{
    RadioTemp2 = RadioGetTransmitState();
#if 0//def  TX_DEBUG
	SendString(( U8 *)"\r\nRadioTemp2=");
	send_hex_u8(RadioTemp2);
#endif	
  	}while( !( RadioTemp2 &( MSK_TXC_IRQ | MSK_TXE_IRQ )));
  RadioEndTransmit();
  return RadioTemp2;
}


//! RadioStartReceive
//!
//! Start the reception of a packet
//!
//! @return
//! 
void RadioStartReceive( void )
{
  RadioWipPtr = RadioPtr;
  RadioWipLen = RadioLen;
  RadioState = RADIO_RX;
  //RadioBytesRead = 0;
  wakeupRadio();
  RadioWrite( RX_CTRL_ADR, MSK_RX_GO | MSK_RXC_IRQ | MSK_RXE_IRQ );
//  RadioRssiShadow = 0;
//  RadioRead( RSSI_ADR );
  
  ( void )RadioRead( RX_IRQ_STATUS_ADR );
}


//! RadioGetReceiveState
//!
//! Returns the state of the current receive operation
//!
//! @return
//!
RADIO_STATE RadioGetReceiveState( void )
{
  if( IRQ )
  {
    RadioGetReceiveStateIsr();
    delay1ms(5);
  }
  return RadioState;
}


//! RadioGetReceiveState
//!
//! Returns the state of the current receive operation
//!
//! @return 
//!
void RadioGetReceiveStateIsr( void )
{
  RADIO_STATE status;
  status = RadioReadStatusDebounced( RX_IRQ_STATUS_ADR );
  status &= MSK_RXBERR_IRQ | MSK_RXE_IRQ | MSK_RXC_IRQ;
  RadioState |= status;
  if(!( RadioState &( MSK_RXBERR_IRQ | MSK_RXE_IRQ )))
    return;
  RadioState |= MSK_RXC_IRQ | MSK_RXE_IRQ;
}


//! RadioEndReceive
//!
//! Completes a transmit operation
//!
//! @return
//!
RADIO_LENGTH RadioEndReceive( void )
{
  U8 length, i;
  RADIO_LENGTH RadioTemp3;

  RadioTemp3 = RadioRead( RX_CFG_ADR );
  length = RadioRead( RX_COUNT_ADR );
  if( RadioTemp3 & MSK_VLD_EN )
    length <<= 1;
  RadioTemp3 = length;
  if( RadioState & MSK_RXE_IRQ )
  {
    length = LP_FIFO_SIZE;
  }
  RadioFileReadWip( RX_BUFFER_ADR, length );
//  if( RadioXactConfig & MSK_END_STATE )
//    RadioState = RADIO_IDLE;
//  else
//    RadioRxCleanup();
  if(!( RadioXactConfig & MSK_END_STATE ))
    RadioRxCleanup();
  RadioState = RADIO_IDLE;

  //SendString((U8 *)"rf rx=");
  rx_packet.first.byte = rf_rx_packet[ 0 ];
  //send_hex_u8(rf_rx_packet[ 0 ]);
  //SendString((U8 *)" ");
  for( i = 0; i < length; i++ )
  {
    rx_packet.data_packet.app_packet[ i ] = rf_rx_packet[ i + 1 ];
    //send_hex_u8(rf_rx_packet[ i + 1 ]);
    //SendString((U8 *)" ");
  }
  //SendString((U8 *)"\r\n");

  return RadioTemp3;
}


//! RadioRxCleanup
//!
//! 
//!
//! @return
//!
void RadioRxCleanup( void )
{
  RadioWrite( XACT_CFG_ADR, RadioXactConfig | MSK_FRC_END_STATE );
  while( RadioRead( XACT_CFG_ADR )& MSK_FRC_END_STATE );
  RadioState = RADIO_IDLE;
}

#if 1
//! Force radio to desired state NOW.
//! 
//! Updates global RadioXacConfig shadow variable.
//! Sometimes used to wake/sleep radio to measure voltage.
//! @return
//!
void RadioForceState( XACT_CONFIG endStateBitsOnly )
{
  if(!( RadioXactConfig & MSK_END_STATE ))
  {
    forceState( END_STATE_IDLE );
  }
  forceState( endStateBitsOnly );
}


void forceState( XACT_CONFIG endState )
{
  U32 timer;
  while( 1 )
  {
    frcState( endState );
    
    timer = Timer_set_timer( TIMER_MS( 5 ));
    while( !Timer_is_timeout( timer ))
    {
      if(!( RadioRead( XACT_CFG_ADR )& MSK_FRC_END_STATE ))
        return;
    }
    frcState( END_STATE_SLEEP );
    endState = END_STATE_IDLE;
  }
}


void frcState( XACT_CONFIG endState )
{
  RadioXactConfig &= ~MSK_END_STATE;
  endState |= RadioXactConfig;
  endState |= MSK_FRC_END_STATE;
  RadioSetXactConfig( endState );
}


//! wakeupRadio 
//! 
//! If radio is in SLEEP, wake it up.
//! Leave "unforced" END STATE in (RadioXactConfig) unchanged.
//! @return
//!
void wakeupRadio( void )
{
  if( RadioXactConfig & MSK_END_STATE )
    return;
  forceState( END_STATE_IDLE );
  RadioSetXactConfig( RadioXactConfig & ~MSK_END_STATE );
}
#endif

//! RadioGetRssi
//! 
//! Returns the receiver signal strength indicator
//!
//! @return
//!
RADIO_RSSI RadioGetRssi( void )
{
//  RADIO_RSSI Rssi;
//  Rssi = RadioRead( RSSI_ADR );
////  RadioRssiShadow |= Rssi;
//  return Rssi;
  return RadioRead( RSSI_ADR );
}


//! RadioAbort
//!
//! Aborts a transmit or receive operation
//!
//! @return
//!
RADIO_LENGTH RadioAbort( void )
{
  RADIO_LENGTH length = RADIO_ABORT_SUCCESS;
  RADIO_STATE status;
  if( RadioState & RADIO_RX )
  {
    RadioWrite( RX_ABORT_ADR, MSK_ABORT_EN );
    //delay10us( 5 );
    delay10us();delay10us();delay10us();delay10us();delay10us();
//    RadioGetRssi();
    status = RadioReadStatusDebounced( RX_IRQ_STATUS_ADR );
    RadioState |= status;
    if(!( RadioState & MSK_SOFDET_IRQ ))
//    if(!( RadioRssiShadow & 0x80 ))
    {
      RadioRxCleanup();
    }
    else
    {
      while( !( RadioGetReceiveState()&( MSK_RXE_IRQ | MSK_RXC_IRQ )));
      length = RadioEndReceive();
      if( RadioState & MSK_RXE_IRQ )
        length = RADIO_ABORT_SUCCESS;
    }
  }
  RadioWrite( RX_ABORT_ADR, 0x00 );
  return length;
}


#if 0
//! RadioGetReceiveStatus
//!
//! Returns the receiver status register
//!
//! @return
//!
RADIO_RX_STATUS RadioGetReceiveStatus( void )
{
//  RADIO_RX_STATUS status;
//  status = RadioRead( RX_STATUS_ADR );
//  return status;
  return RadioRead( RX_STATUS_ADR );
}
#endif


#endif  // _lpnonstreaming_c_
