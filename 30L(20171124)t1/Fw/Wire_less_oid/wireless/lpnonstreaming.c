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
#include <KBDM9_DDK.H>
#include "compiler.h"
#include "lpnonstreaming.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "protocol.h"
#include "oid.h"
#include "com.h"
//#include "Int_drv.h"
#include "system.h"
#include "power.h"
#include "oid.h"

//_____ M A C R O S ________________________________________________________

#pragma DATA = LPNONSTREAMING_DATA
#pragma CODE = LPNONSTREAMING_CODE

//==============================================================================
extern U8 rf_recive_data[LEN_RF_RECIVE_DATA];
//----------------------------------------------------
//extern void  delay1ms    ( int count );
//extern void  delay10us   ( void );
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
//  __disable_interrupt();
   Int_disable();
  RadioWrite( TX_LENGTH_ADR, RadioPacketLength );
  RadioWrite( TX_CTRL_ADR, MSK_TX_GO | MSK_TX_CLR | MSK_TXC_IRQ | MSK_TXE_IRQ );
  /*RadioTemp2 = MSK_TX_GO | MSK_TX_CLR | MSK_TXC_IRQ | MSK_TXE_IRQ;
  RadioWipPtr = &RadioTemp1;
  RadioWipLen = 2;
  RadioBurstWriteWip( TX_LENGTH_ADR, 2 );*/
  RadioFileWrite( TX_BUFFER_ADR, RadioPacketLength );
//  __enable_interrupt();
  Int_enable();
}


//! RadioGetTransmitState
//!
//! Returns the state of the current transmit operation
//!
//! @return
//!
RADIO_STATE RadioGetTransmitState( void )
{
//	_bitSET(RF_IRQ_DIR); // Set RF_IRQ with Input.
//	_bitSET(RF_IRQ_PH);  // Set FR_IRQ with Pull-high.
  if( IRQ )
  {
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
#ifdef  TX_DEBUG
//	SendString(( char *)"\r\nstatus=");
//	send_hex_u8(status);
#endif   
  status &= MSK_TXC_IRQ | MSK_TXE_IRQ;
  RadioState |= status;
  if( status == MSK_TXC_IRQ )
    return;

  //Reading the RX_IRQ_STATUS register will clear up the IRQ. We don't need to do the read if there is just a
  //simple error, but no harm
  (void)RadioRead( RX_IRQ_STATUS_ADR );
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
#ifdef  TX_DEBUG
//	SendString(( char *)"\r\nRadioTemp2=");
//	send_hex_u8(RadioTemp2);
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
#if 1
  RadioWipPtr = &rf_recive_data;
  RadioWipLen = RadioLen;
  RadioState = RADIO_RX;
  //RadioBytesRead = 0;
  wakeupRadio();
  RadioWrite( RX_CTRL_ADR, MSK_RX_GO | MSK_RXC_IRQ | MSK_RXE_IRQ );
//  RadioRssiShadow = 0;
//  RadioRead( RSSI_ADR );
  
  (void)RadioRead( RX_IRQ_STATUS_ADR );
#else
  RadioWipPtr = RadioPtr;
  RadioWipLen = RadioLen;
  RadioState = RADIO_RX;
  //RadioBytesRead = 0;
  wakeupRadio();
  RadioWrite( RX_CTRL_ADR, MSK_RX_GO | MSK_RXC_IRQ | MSK_RXE_IRQ );
//  RadioRssiShadow = 0;
//  RadioRead( RSSI_ADR );
  
  RadioRead( RX_IRQ_STATUS_ADR );
#endif
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
    Delay_ms(3);
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
  U8 length;
  RADIO_LENGTH RadioTemp3;

  RadioTemp3 = RadioRead( RX_CFG_ADR );
  length = RadioRead( RX_COUNT_ADR );
  if( RadioTemp3 & MSK_VLD_EN )
    length <<= 1;
  RadioTemp3 = length;
  if( RadioState & MSK_RXE_IRQ )
  {
    length = 0x10;
  }
  RadioFileReadWip( RX_BUFFER_ADR, length );
//  if( RadioXactConfig & MSK_END_STATE )
//    RadioState = RADIO_IDLE;
//  else
//    RadioRxCleanup();
  if(!( RadioXactConfig & MSK_END_STATE ))
    RadioRxCleanup();
  RadioState = RADIO_IDLE;
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
	
//    delay10us();
//    delay10us();
//    delay10us();
//    delay10us();
//    delay10us();
	  Delay_ms(1);
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
