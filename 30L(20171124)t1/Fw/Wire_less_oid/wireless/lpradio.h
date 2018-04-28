//! @file lpradio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: lpradio.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _LPRADIO_H_
#define _LPRADIO_H_


//_____ I N C L U D E S ____________________________________________________
#undef _GLOBEXT_
#if (defined _lpradio_c_)
#  define _GLOBEXT_
#elif (!defined _GLOBEXT_)
#  define _GLOBEXT_ extern
#endif

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

//_____ D E C L A R A T I O N ______________________________________________

typedef unsigned char RADIO_FRAME_CONFIG;
 #define RADIO_SOP_EN           0x80
 #define RADIO_SOP_LEN          0x40
 #define RADIO_LEN_EN           0x20
 #define RADIO_SOP_THRESH_MSK   0x1F

typedef unsigned char RADIO_TX_STATUS;
 #define RADIO_XS               0x80
 #define RADIO_LV               0x40
 #define RADIO_TXE              0x01
 
//----------------------------------------------------------------------------
//
// LP SPI Interface definitions:
//
#define mSPI_ADDRESS            0x3F
#define bSPI_WRITE              0x80
#define bSPI_AUTO_INC           0x40

//----------------------------------------------------------------------------
//
// RADIO_STATE uses many of the same bits as the RX_IRQ_STATUS_ADR register. We
//  add in a couple of our own. If something changes and we have a collision
//  between the symbolics and the constants that should generate a compile error.
//
typedef unsigned char RADIO_STATE;
#define RADIO_IDLE        0x00
#define RADIO_RX          0x80
#define RADIO_TX          0x20
#define RADIO_SOP         MSK_SOFDET_IRQ
#define RADIO_DATA        MSK_RXB1_IRQ
#define RADIO_COMPLETE    MSK_RXC_IRQ
#define RADIO_ERROR       MSK_RXE_IRQ

//----------------------------------------------------------------------------
//
// XACT_CONFIG defines the states the radio hardware can automatically
//  got to after performing an operation and whether the radio acks
//  automatically.
//
typedef unsigned char XACT_CONFIG;

//----------------------------------------------------------------------------
//
// TX_CONFIG defines the data and SOP length and encoding for transmit data
//  and also the TX power.
//
typedef unsigned char TX_CONFIG;

//----------------------------------------------------------------------------
//
// RADIO_RX_STATUS 
//
typedef unsigned char RADIO_RX_STATUS;
#define RADIO_RX_ACK      0x80
#define RADIO_RX_MISS     0x40
#define RADIO_RX_OF       0x20
#define RADIO_RX_CRC0     0x10
#define RADIO_BAD_CRC     0x08
#define RADIO_DATCODE_LEN 0x04
#define RADIO_SDR         0x03
#define RADIO_DDR         0x02
#define RADIO_8DR         0x01
#define RADIO_GFSK        0x00

//--------------------------------------
// Type Declarations
//--------------------------------------
typedef unsigned char RADIO_LENGTH;
typedef unsigned char RADIO_REG_ADDR;
typedef void *RADIO_BUFFER_PTR;
typedef const char *RADIO_CONST_PTR;
typedef unsigned char RADIO_RSSI;

//
// Return value for a RadioAbort completed successfully. Otherwise the return value
//  is the length of the packet received (without error).
//
#define RADIO_ABORT_SUCCESS 0xFF


extern XACT_CONFIG RadioXactConfig;
extern RADIO_STATE RadioState;
extern U8          RadioRetryCount;
extern U8          RadioPacketLength;


void        RadioInit               ( XACT_CONFIG xactConfig, TX_CONFIG txConfig );
void        RadioSetChannel         ( U8 channel );
void        RadioSetTxConfig        ( TX_CONFIG config );
void        RadioSetXactConfig      ( XACT_CONFIG config );
XACT_CONFIG RadioGetXactConfig      ( void );
void        RadioSetFrameConfig     ( RADIO_FRAME_CONFIG config );
void        RadioSetThreshold32     ( U8 threshold );
void        RadioSetPreambleCount   ( U8 preamblecount );
void        RadioSetCrcSeed         ( U16 crcSeed );
void        RadioGetFuses           ( void );
void        RadioSetSopPnCode       ( U8 patternNum );


#endif  // _LPRADIO_H_
