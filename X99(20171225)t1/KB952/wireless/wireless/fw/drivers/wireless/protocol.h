//! @file protocol.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: protocol.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//_____ I N C L U D E S ____________________________________________________
#include "config.h"
#include "conf\conf_oid.h"
#include "modules\power\power.h"             // Power module definition

//_____ M A C R O S ________________________________________________________

//--------------------------------------------------------------------------
// Protocol Specific Configuration
//--------------------------------------------------------------------------
// Data payload length for mouse tx data packets
#define APP_TX_PACKET_SIZE                      0x0F//0x03//

// Data payload lenght for mouse rx data packets
// This is not used in this application, but needs to be at least one in size
#define APP_RX_PACKET_SIZE                      0x01


//_____ D E F I N I T I O N ________________________________________________
#undef _GLOBEXT_
#if (defined _protocol_c_)
# define _GLOBEXT_ extern
#else
# define _GLOBEXT_ extern
#endif

//_____ D E C L A R A T I O N ______________________________________________

//--------------------------------------
// API Definitions and Types
//--------------------------------------
#ifndef NUM_PN_CODES
//! Number of PN codes in the PN code table   
//! 10 PN codes are used 
#define NUM_PN_CODES        20//10//
#endif

// Set the default LVI Threshold to 1.8V  
#define PWR_CTRL_SETTING    ( PWR_CTRL_RST | LV_IRQ_TH_1P8_V | PMU_OUTV_2P7 )

// Sys Params is structured to improve memory utilization.  It is used
// to read the mid out of flash and then is converted to system parameters
// that are used during operation.
typedef struct _SYS_PARAMETERS
{
  // Structure used for flash reads
  struct 
  {
    U8 signature;     // Parameters initialized and valid signature
    U8 mid_1;         // Manufacturing id of bound bridge
    U8 mid_2;
    U8 mid_3;
    U8 mid_4;

  }bridge_mid;

  // Structure used for system operation
  struct 
  {
    U8 channel;       // Currently selected channel
    U8 pn_code;       // Currently selected pncode
    U8 seed;          // CRC seed used for transmissions
    U8 pin;           // Pin used for transmissions

  }network_id;

}SYS_PARAMETERS;

// Protocol return status codes
typedef enum _PROTOCOL_STATUS
{
  PROTOCOL_SUCCESS,
  PROTOCOL_FAILED,
  PROTOCOL_TX_TIMEOUT

}PROTOCOL_STATUS;


// Connected state of the protocol layer
typedef enum _PROTOCOL_CONNECT_STATE
{
  PROTOCOL_DISCONNECTED,
  PROTOCOL_CONNECTED

}PROTOCOL_CONNECT_STATE;

// Deivce Type
typedef enum _DEVICE_TYPE
{
  PRESENTER_DEVICE_TYPE   = 0x00,
  RESERVED_DEVICE_TYPE    = 0x01,
  KEYBOARD_DEVICE_TYPE    = 0x02,   // Keyboard, for backward compatible
  MOUSE_DEVICE_TYPE       = 0x03    // Mouse, for backward compatible            
}RF_DEVICE_TYPE;


// Packet Type
typedef enum _PACKET_TYPE
{
  NULL_PACKET_TYPE      = 0x00,
  INDEX_PACKET_TYPE     = 0x01,
  KEYBOARD_PACKET_TYPE  = 0x02,
  I2C_PACKET_TYPE1      = 0x03,
  I2C_PACKET_TYPE2      = 0x04
}PACKET_TYPE;

//! To set up a data packet
typedef union _LP_PACKET
{
  //! To hold all data of bind request packet
  struct 
  {  
    //! To hold the header data of bind request packet
    struct
    {
      //! Type of bind request packet
      unsigned    type        : 0x04;

      //! Reserved
      unsigned    reserved1   : 0x01;

      //! Type of device part of bind request packet
      unsigned    dev_type    : 0x02;
      
      //! Reserved
      unsigned    reserved    : 0x01;
    }hdr;
  }bind_request;

  //! To hold the data of bind response packet
  struct 
  {  
    //! To hold the header data of bind response packet
    struct
    {
      //! Type of bind response packet
      unsigned    type        : 0x04;

      //! Reserved
      unsigned    reserved1   : 0x01;

      //! Type of device part of bind request packet
      unsigned    dev_type    : 0x02;
      
      //! Reserved
      unsigned    reserved    : 0x01;
    }hdr;

    //! 1st byte of radio manufacturing ID
    U8    mid_1;

    //! 2nd byte of radio manufacturing ID
    U8    mid_2;

    //! 3rd byte of radio manufacturing ID
    U8    mid_3;

    //! 4th byte of radio manufacturing ID
    U8    mid_4;
        
  }bind_response;

  //! To hold the data of connect request packet
  struct
  {  
    //! To hold the header data of connect request packet
    struct
    {
      //! Type of connect request packet
      unsigned    type        : 0x04;

      //! Reserved
      unsigned    reserved1   : 0x01;

      //! Type of device part of bind request packet
      unsigned    dev_type    : 0x02;
      
      //! Reserved
      unsigned    reserved    : 0x01;
    }hdr;

    //! 1st byte of radio manufacturing ID
    U8    mid_1;

    //! 2nd byte of radio manufacturing ID
    U8    mid_2;

    //! 3rd byte of radio manufacturing ID
    U8    mid_3;

    //! 4th byte of radio manufacturing ID
    U8    mid_4;

  }connect_request;

  //! To hold the data of connect response packet
  struct 
  {  
    //! To hold the header data of connect response packet
    struct
    {
      //! Type of connect response packet
      unsigned    type        : 0x04;

      //! Data packet flag to tell whether it was a good or bad data
      unsigned    flag        : 0x01;
      
      //! Reserved
      unsigned    reserved    : 0x03;
    }hdr;

  }connect_response;

  //! To hold the data of the ping packet
  struct
  {
    //! To hold the header data of the ping packet
    struct
    {
      //! Type of ping packet
      unsigned    type        : 0x04;

      //! Reserved
      unsigned    reserved    : 0x03;
      
      //! Specify a Ping (0) or Ping Response (1)
      unsigned    flag        : 0x01;
    }hdr;
  }ping;

  //! To hold the data of an data packet
  struct
  {
    //! To hold the header of a data packet
    struct
    {
      //! Packet type
      unsigned    type            : 0x04;

      // Back channel request
      unsigned    back_ch_req     : 0x01;

      //! Data toggle information of data part of packet
      unsigned    dt_data         : 0x01;
      
      //! Type of device part of data packet
      unsigned    data_dev_type   : 0x02;   // bit0 and bit1 swapped for backward compatible
    }hdr;

    //! Buffer to hold data packet
    U8    app_packet[ APP_TX_PACKET_SIZE ];

  }data_packet;

    //! To hold the transmit data of null packet
  struct
  {
    //! To hold the transmit header data of null packet
    struct
    {
      //! Type of data packet
      unsigned    type        : 0x04;

      //! Reserved
      unsigned    reserved    : 0x02;
      
      unsigned    dev_type    : 0x02;   // bit0 and bit1 swapped for backward compatible
    }hdr;
  }null;

  //! To hold the first byte of the packet
  struct
  {
    //! First byte of the packet
    U8 byte;
  }first;
   
}LP_PACKET;

// Protocol connected state
_GLOBEXT_ PROTOCOL_CONNECT_STATE  connected_state;
_GLOBEXT_ LP_PACKET               rx_packet;
_GLOBEXT_ U8                      rf_rx_packet[ APP_TX_PACKET_SIZE + 1 ];

void                    protocol_init         ( void );
void                    protocol_button_bind  ( U8 dev_type );
void                    *protocol_get_tx_pkt  ( void );
PROTOCOL_STATUS         protocol_send_packet  ( U8 dev_type, U8 data_dev_type, U8 data_length );
U8                      radio_receive_packet  ( void );

void  wireless_task_init         ( void );
void  wireless_task              ( void );

#endif  // _PROTOCOL_H_

