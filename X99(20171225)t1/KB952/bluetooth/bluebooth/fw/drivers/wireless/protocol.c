//! @file protocol.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: protocol.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _protocol_c_
#define _protocol_c_

//_____ I N C L U D E S ____________________________________________________
#include "config.h"
#include "lib_mcu\compiler.h"         // Compiler definitions
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
#include "protocol.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "lpnonstreaming.h"
#include "stdio.h"
#include "mmi\kbd\keypad.h"
#include "drivers\oid\oid.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\power\power.h"             // Power module definition
#include "spi.h"
#include "string.h"
#include "crc16.h"
#include "wireless.h"

#pragma DATA = PROTOCOL_DATA
#pragma CODE = PROTOCOL_CODE

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________
//#define DYNAMIC_DATA_RATE_PA

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
// Flash block location of system parameters
#define SYS_PARAM_LOC               0x7F

// Flash signature byte for system parameters
#define SIGNATURE_BYTE              0x90

// Number of channels in subset for each pncode
#define NUM_CHANNELS_PER_SUBSET     13

// Number of reconnect attemps on original channel before moving on
#define ORIG_CHAN_RECONNECT_COUNT   5 

// Number of loops through the channels during bind (max = 0xffff)
#define BIND_RETRY_COUNT            ( NUM_CHANNELS_PER_SUBSET * 1000 )

// Number of loops through the channels during reconnect (max = 255)
#define CONNECT_RETRY_COUNT         ( NUM_CHANNELS_PER_SUBSET * 2 + ORIG_CHAN_RECONNECT_COUNT + 1 )

// PN code for button bind
#define BUTTON_BIND_PN_CODE         0x00
#define BIND_BASE_CHANNEL           0x00

// Number of channels in the 2.4 GHz spread spectrum
#define NUM_CHANNELS                78

// Number of retransmit attempts for data packets
#define NUM_TRANSMITS               10

// Mask used in the pin distribution algorithm
#define PIN_MASK                    0x03

// To allow certain pins to be used in the pin distribution algorithm
#define MIN_PIN                     0x02

// Timer delay when waiting for bridge to ping
#define BRIDGE_PING_TIME_MS         200

// Timeout for packet receving
#define RX_PACKET_TIMEOUT           1000  //250

// Ping Reason
typedef enum _PING_REASON
{
  PING_REASON_NORMAL,               // When pinging for normal reasons
  PING_REASON_RSSI,                 // When pinging for RSSI reasons
  PING_REASON_CORRUPT               // When pinging for corrupt packet reasons
}PING_REASON;

// Packet types
typedef enum _PACKET_TYPES
{
  BIND_REQ                  = 0x00, // Bind Request Packet Type
  BIND_RESP                 = 0x00, // Bind Response Packet Type
  CONNECT_REQ               = 0x01, // Connect Request Packet Type
  CONNECT_RESP              = 0x02, // Connect Response Packet Type
  PING_PACKET               = 0x03, // Ping Packet Type
  DATA_PACKET               = 0x04, // Data Packet Type
  BACK_CHANNEL_PACKET       = 0x05, // Back channel packet Type
  NULL_PACKET               = 0x07, // Null Packet Type
  ENCRYPT_KEY_REQ           = 0x08, // Key Packet Type for encryption
  ENCRYPT_KEY_RESP          = 0x08, // Key Packet Type for encryption
  KISS_BIND_REQ             = 0x0D, // Kiss Bind Request Packet Type
  KISS_BIND_RESP            = 0x0D  // Kiss Bind Response Packet Type

} PACKET_TYPES;

typedef enum _PACKET_LENGTHS
{
  BIND_REQ_LEN              = 1,
  BIND_RESP_LEN             = 5,
  KISS_BIND_REQ_LEN         = 1,
  CONNECT_REQ_LEN           = 5,
  CONNECT_RESP_LEN          = 1,
  PING_LEN                  = 1,
  ENCRYPT_KEY_REQUEST_LEN   = 1,
  ENCRYPT_KEY_RESPONSE_LEN  = 9
} PACKET_LENGTHS;

// Transmit retry times
typedef enum _TX_RETRY_TIMES
{
  BIND_REQ_RETRIES            = 0,  // Bind request retry times
  BIND_RESP_RETRIES           = 5,  // Bind response retry times
  KISS_BIND_REQ_RETRIES       = 0,  // KissBind request retry times
  KISS_BIND_RESP_RETRIES      = 1,  // KissBind response retry times
  PING_REQ_RETRIES            = 1,  // Ping request retry times
  PING_RESP_RETRIES           = 1,  // Ping response retry times
  CONNECT_REQ_RETRIES         = 0,  // Connect request retry times
  CONNECT_RESP_RETRIES        = 1,  // Connect response retry times
  ENCRYPT_KEY_REQ_RETRIES     = 2,  // Encrypt key request retry times
  ENCRYPT_KEY_RESP_RETRIES    = 1,  // Encrypt key response retry times
  DATA_PACKET_RETRIES         = 40, // Data packet retry times
  BACK_CHANNEL_PACKET_RETRIES = 5   // Back channel packet retry times

}TX_RETRY_TIMES;

//RSSI Threshold
typedef enum _RSSI_THRESHOLD
{
  RSSI_NOISE_THRESHOLD      = 28, 
  PING_RSSI_NOISE_THRESHOLD = 20, 
  KISS_BIND_RSSI_THRESHOLD  = 10, // Kiss Bind RSSI threshold     
  PA_RSSI_RX_THRESHOLD	    = 24  // PA threshold, around 2 meters in open space.	

}RSSI_THRESHOLD;

// PA Settings
typedef enum _PA_SETTING
{
//  DATA_MODE_PA      = 6,  // PA setting for data mode
  DATA_MODE_PA_MAX  = 7,  // MAX PA setting for data mode
  DATA_MODE_PA_MIN  = 4,  // MIN PA setting for data mode
  ENCRYPT_KEY_PA    = 1,  // PA setting for encrypt key request
  BUTTON_BIND_PA    = 1,  // PA setting for button bind 
  KISS_BIND_PA      = 1   // PA setting for button kiss bind

}PA_SETTING;

#define DATA_DEV_TYPE_MASK          0x03

// Number of packet to evaluate the performance	
#define PKT_NUM                     32

// The low end threshold for the total retries
#define TOTAL_RETRY_THRESHOLD_LOW   ( PKT_NUM >> 2 )

// The number allowded when the total retries is above the current threshold but below the last threshold 
#define OVER_RETRY_NUM              2
    
#define	PA_RETRY_THRESHOLD          2

//_____ D E C L A R A T I O N ______________________________________________
void  protocol_reconnect                ( U8 dev_type );
void  retrieve_bridge_mid               ( void );
void  radio_toggle_data_mode            ( void );
Bool  radio_send_packet_and_adjust_DRPA ( U8 retry, U8 length );
void  calculate_network_id              ( void );
void  get_next_channel                  ( void );
void  radio_set_nonzero_crc_seed        ( void );
U8    radio_get_rx_rssi                 ( void );
void  radio_set_pa                      ( U8 pa );
Bool  radio_send_packet                 ( U8 retry, U8 length );
void  radio_start_receive               ( void );
void  NVRAM_read_sys_params             ( void );
void  NVRAM_write_sys_params            ( void );
void  read_machine_serial               ( void );



//--------------------------------------------
//-------------------------------------------------------
extern void wireless_index_display(U16 temp_data);
extern void wireless_lcd_bind(U8 temp_flag);
//-------------------------------------------------------
//--------------------------------------
// Local Variable Definitions
//--------------------------------------
// The protocol module owns the memory for the tx/rx packets
// The APIs are set up for zero-copy
//! Transmit packet
LP_PACKET tx_packet;
U8        rf_tx_packet[ APP_TX_PACKET_SIZE + 1 ];

// System parameters for radio communication
SYS_PARAMETERS sys_params;
U16 sys_params_crc16;
//***********************************************
U32 machine_id = 0;//0x21FC;
//***********************************************

// Transmit packet data toggle
U8 tx_data_toggle = 1;

U8 RadioTxConfig;

#ifdef DYNAMIC_DATA_RATE_PA
  U8 pkt_count = 0;
  U8 total_retry = 0;
  U8 total_retry_threshold = TOTAL_RETRY_THRESHOLD_LOW;
  U8 total_retry_threshold_last = TOTAL_RETRY_THRESHOLD_LOW;
  U8 over_retry_count = 0;
#endif  //DYNAMIC_DATA_RATE_PA
  
#ifdef BACK_CHANNEL_SUPPORT
  U8 rx_data_length;
  U8 rx_data_toggle = 1;
#endif //BACK_CHANNEL_SUPPORT

extern __X   U32   Start_Sector, Length;
extern __X   U8    Buf[ 512 ];

// Protocol connected state
PROTOCOL_CONNECT_STATE  connected_state;
LP_PACKET               rx_packet;
U8                      rf_rx_packet[ APP_TX_PACKET_SIZE + 1 ];

extern __X   unsigned long  IndexDataAddress;

//--------------------------------------------------------------------------
// protocol_init
//--------------------------------------------------------------------------
void protocol_init( void )
{
  // Configure the Radio
  // The ACK Timeout (Bit[1:0], Register XACT_CFG_ADR) sets the timeout period
  // after transmission of a packet during which an ACK must be correctly
  // received in order to prevent a transmit error condition from being detected.
  // This design uses the 11b setting for this field which is equivalent to 
  // 15 times the SOP_CODE_ADR code length. This gives the radio amble time to
  // safely detect Timeout condition. Theoretically, the "8x" setting (01b) 
  // should be sufficient, and it will slightly improve the device power 
  // consumption. Complete testing is recommended if this field is changed.

//  RadioInit( MSK_ACK_EN | END_STATE_SLEEP | ACK_TO_15X, PA_4_DBM | DATCODE_LEN_32 | DATMODE_8DR );
  // Configure the Radio
  RadioTxConfig = DATA_MODE_PA_MAX | DATCODE_LEN_32 | DATMODE_1MBPS;
  //RadioTxConfig = DATA_MODE_PA_MAX | DATCODE_LEN_32 | DATMODE_8DR;
  RadioInit( MSK_ACK_EN | END_STATE_SLEEP | ACK_TO_8X, RadioTxConfig );

  // Configure the frame
  RadioSetFrameConfig( MSK_SOP_EN | MSK_LEN_EN | 4 );

  //RadioSetThreshold32( 0x05 );
  // Load the Preamble with 0x01
  // This design defaults to using 16-chip Preamble. The 16 chip Preamble sequence 
  // may be repeated multiple times (see Register PREAMBLE_ADR). The longer 
  // Preamble allows the receiver amble time to "lock" to the coming signal. Our 
  // testing shows that 16-chip sequence is sufficient for keyboard and mouse 
  // applications.
  RadioSetPreambleCount( 0x01 );

  // Set LVI Threshold to 1.8V
  RadioWrite( PWR_CTRL_ADR, 0/*PWR_CTRL_SETTING*/ );

  // Output radio data stream
  RadioWrite( XTAL_CTRL_ADR, XOUT_FNC_RAD_STREAM );

  // The design uses the default value 0x04 for the 32 Chip Data PN Code Correlator
  // Threshold (Bit[3;0], DATA32_THOLD_ADR0). However, characterization data shows 
  // that a value of 0x05 will give a slightly improvement over the default value. 
  // This setting will be implemented in the next version of RDK. Complete testing 
  // is recommended if this field is changed
// /*
   SendString((U8 *)"protocol init \r\n");
 {
   U8 _MEM_TYPE_SLOW_ temp[ 6 ]; // Only need 6 bytes, but leave it for now.
   // Retrieve the bridge MID
   // Set receive length
   RadioSetLength( sizeof( temp ));
   // Set buffer pointer
   RadioSetPtr( temp );
   // Get MID
   RadioGetFuses();

   SendString((U8 *)"mid1=");
   send_hex_u8(temp[ 0 ]);
   SendString((U8 *)"\r\n");
   SendString((U8 *)"mid2=");
   send_hex_u8(temp[ 1 ]);
   SendString((U8 *)"\r\n");
   SendString((U8 *)"mid3=");
   send_hex_u8(temp[ 2 ]);
   SendString((U8 *)"\r\n");
   SendString((U8 *)"mid4=");
   send_hex_u8(temp[ 3 ]);
   SendString((U8 *)"\r\n");
  }
//*/

  // Retrieve the bridge MID
  retrieve_bridge_mid();
  
  // Set CRC
  radio_set_nonzero_crc_seed();

  connected_state = PROTOCOL_DISCONNECTED;
}


//--------------------------------------------------------------------------
// protocol_bind
//--------------------------------------------------------------------------
void protocol_button_bind( U8 dev_type )
{
  U32 timer;
  U16 bind_retry = 0;
  U8 packet_length;
  PROTOCOL_STATUS status = PROTOCOL_FAILED;
  
  red_led_on();
  green_led_off();
  
  timer = Timer_set_timer( TIMER_MS( 250 ));
  
  // Enter disconnected state
  connected_state = PROTOCOL_DISCONNECTED;

  // Initialize tx packet header
  tx_packet.first.byte = ( BIND_REQ << 4 )|( dev_type << 1 );
#ifdef  TX_DEBUG 
  SendString((U8 *)"\r\nss1");
#endif
  // Set the bind pn code
  RadioSetSopPnCode( BUTTON_BIND_PN_CODE );

  // seed the channel algorithm with bind parameters 0,0
  sys_params.network_id.channel = BIND_BASE_CHANNEL;

  // Set Zero CRC
#ifdef  TX_DEBUG  
  SendString((U8 *)"\r\nss2");
#endif  
  RadioSetCrcSeed( 0 );
/*  
#ifdef DYNAMIC_DATA_RATE_PA
  // Change back to 8DR mode
  if((RadioTxConfig & MSK_TX_DATMODE )== DATMODE_1MBPS )
    radio_toggle_data_mode();
#endif //DYNAMIC_DATA_RATE_PA
*/    
  // Set PA setting
  radio_set_pa( BUTTON_BIND_PA );
//  radio_set_pa( PA_0_DBM );
#ifdef  TX_DEBUG
  SendString((U8 *)"\r\nss3");
#endif
  // Force endstate to idle
  RadioForceState( END_STATE_IDLE );
#ifdef  TX_DEBUG
  SendString((U8 *)"\r\nss4");
#endif  
  // send a bind request and wait for response, if no response change
  // the channel and try again
  while( bind_retry < BIND_RETRY_COUNT )
  {
    bind_retry++;
     
    RadioSetChannel( sys_params.network_id.channel );
#if 0//def  TX_DEBUG
    SendString((U8 *)"\r\nss5");
#endif
    if( radio_send_packet( BIND_REQ_RETRIES, BIND_REQ_LEN ))
    {
#ifdef  TX_DEBUG    
	  SendString((U8 *)"\r\nss6");
#endif    
      // If a ACK is received, it will take 50ms to wait for a packet.
      // It roughly equals to 32 cycles (1.56ms per cycle). 
      bind_retry += 32;
      
      packet_length = radio_receive_packet();
      
      if(( packet_length == BIND_RESP_LEN )
         &&( rx_packet.bind_response.hdr.type == BIND_RESP ))
      {
        // extract Bridge MID from bind response packet 
        sys_params.bridge_mid.mid_1 = rx_packet.bind_response.mid_1;
        sys_params.bridge_mid.mid_2 = rx_packet.bind_response.mid_2;
        sys_params.bridge_mid.mid_3 = rx_packet.bind_response.mid_3;
        sys_params.bridge_mid.mid_4 = rx_packet.bind_response.mid_4;

        sys_params.bridge_mid.signature = SIGNATURE_BYTE;
        
        // exit loop with success
        status = PROTOCOL_SUCCESS;
#ifdef  TX_DEBUG
	SendString(( U8 *)"\r\nbind ok!");
#else
	delay10us();
#endif			
        break;
      }
    }
#if 0//def  TX_DEBUG	
  SendString((U8 *)"\r\nss7");
#endif
    get_next_channel();
#if 0//def  TX_DEBUG
     SendString((U8 *)"\r\nss8");
#endif    
    if( Timer_is_timeout( timer ))
    {
      timer = Timer_set_timer( TIMER_MS( 250 ));
      red_led_flash();
    }
#ifdef  TX_DEBUG	
	SendString((U8 *)"\r\nss9");
#else
	delay10us();
#endif
/*
    if( !power_is_vbus_present())
    {
      kbd_process();
      if( g_kbd_key == (( U16 )KEY_POWER << 8 |( U16 )KBD_KEY_LONGPRESSED ))
      {
        green_led_off();
        red_led_off();
        EXT_PWR_CTL_OFF();
        EXT_DC_PWR_OFF();
        PWR_OFF();
        while( power_key_is_press());
        CommonLoadCode( C_STARTUP );
      }
    }
	*/
  }

  // Force endstate to sleep
  RadioForceState( END_STATE_SLEEP );

  red_led_off();
  green_led_off();
  green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );

  // Restore PA setting to default state
//  radio_set_pa( PA_4_DBM );
  radio_set_pa( DATA_MODE_PA_MAX );
    
  if( status == PROTOCOL_SUCCESS )
  {
#ifdef  TX_DEBUG
	SendString(( U8 *)"\r\nPROTOCOL_SUCCESS!");

#else
	delay10us();
#endif	  
    wireless_lcd_bind(1);
    // store parameters in NVRAM
    NVRAM_write_sys_params();
/*	
    GetDataIdInfo( D_SONIX, &Start_Sector, &Length );
#ifdef SDBOOT_KBDM9
    sdtl_resv_read( Start_Sector, ( __X void *)Buf );
#endif
#ifdef SPIBOOT_KBDM9
    spif_resv_read( Start_Sector, ( __X void *)Buf );
#endif
    IndexDataAddress = 0;
 */   
    calculate_network_id();
    // set CRC
    radio_set_nonzero_crc_seed();
//    protocol_init();

    // give the dongle time to ping before trying to reconnect
    timer = Timer_set_timer( TIMER_MS( BRIDGE_PING_TIME_MS ));
    while( !Timer_is_timeout( timer ));

    protocol_reconnect( dev_type );

  }
  else
  {
#ifdef  TX_DEBUG
	SendString(( U8 *)"\r\nPROTOCOL_FAIL!");
#endif	  
     
    // can't Bind, reset to original bind parameters
    retrieve_bridge_mid();
    
    // Set CRC
    radio_set_nonzero_crc_seed();

	wireless_lcd_bind(2);
  }
}


//--------------------------------------------------------------------------
// protocol_get_tx_pkt
//--------------------------------------------------------------------------
void *protocol_get_tx_pkt( void )
{
   return tx_packet.data_packet.app_packet;
}


//--------------------------------------------------------------------------
// protocol_send_packet
//--------------------------------------------------------------------------
PROTOCOL_STATUS protocol_send_packet(  U8 dev_type
                                      , U8 data_dev_type
                                      , U8 data_length
#ifdef BACK_CHANNEL_SUPPORT
                                      , U8 back_channel
#endif //BACK_CHANNEL_SUPPORT
                                    )
{
  if( connected_state == PROTOCOL_DISCONNECTED )
  {
    protocol_reconnect( dev_type );

    return PROTOCOL_TX_TIMEOUT;
  }

  if( data_length < APP_TX_PACKET_SIZE + 1 )
  {
#ifdef BACK_CHANNEL_SUPPORT
    // Setup the packet header 
    tx_packet.first.byte = ( DATA_PACKET << 4 )|( back_channel << 3 )
                            |( tx_data_toggle << 2 )
                              |(( data_dev_type << 1 | data_dev_type >> 1 ) & DATA_DEV_TYPE_MASK );
#else //BACK_CHANNEL_SUPPORT
        // Setup the packet header
    tx_packet.first.byte = ( DATA_PACKET << 4 )
                            |( tx_data_toggle << 2 )
                              |(( data_dev_type << 1 | data_dev_type >> 1 )& DATA_DEV_TYPE_MASK );
#endif //BACK_CHANNEL_SUPPORT
    
    // calculate check sum
    data_length += 1;   // length = header + data
  }
  else
  {
    return PROTOCOL_FAILED;
  }

#ifdef BACK_CHANNEL_SUPPORT
  // Force endstate to idle
  RadioForceState( END_STATE_IDLE );
#endif //BACK_CHANNEL_SUPPORT
  
  if( radio_send_packet_and_adjust_DRPA( DATA_PACKET_RETRIES, data_length ))
  {
    tx_data_toggle ^= 1;

#ifdef BACK_CHANNEL_SUPPORT
    if( back_channel) 
    { 
      RadioSetBackChannelCrcSeed();
            
      rx_data_length = RadioReceivePacket();
            
      if(( rx_packet.data.hdr.type == BACK_CHANNEL_PACKET )
         &&( rx_packet.data.hdr.dt_data == rx_data_toggle ))
      {
        rx_data_toggle ^= 1;
        NotifyDownloadBackChannelData();                      
      }
            
      //return to system CRC
      radio_set_nonzero_crc_seed();
    }    
    // Force endstate to sleep
    RadioForceState( END_STATE_SLEEP );
#endif //BACK_CHANNEL_SUPPORT
    
    return PROTOCOL_SUCCESS;
  }
#ifdef BACK_CHANNEL_SUPPORT
  else
  {
    // Force endstate to sleep
    RadioForceState( END_STATE_SLEEP );
  }
#endif //BACK_CHANNEL_SUPPORT
  
  protocol_reconnect( dev_type );

  return PROTOCOL_TX_TIMEOUT;
}


//--------------------------------------------------------------------------
// protocol_reconnect
//--------------------------------------------------------------------------
void protocol_reconnect( U8 dev_type )
{
  U8 retry;
  U8 packet_length;

  U32 timer;
  
  // check for unbound device
  if( sys_params.network_id.pin == 0 )
  {
    return;
  }
#ifdef  TX_DEBUG
	SendString(( U8 *)"\r\nprotocol_reconnect");
#else
	delay10us();
#endif  
  green_led_off();
  red_led_on();
  timer = Timer_set_timer( TIMER_MS( 50 ));
  
  connected_state = PROTOCOL_DISCONNECTED;

  // select a channel, send a connect request, and
  // wait for a connect response
  tx_packet.first.byte = ( CONNECT_REQ << 4 )|( dev_type << 1 );

  // load Bridge MID from NVRAM into the tx_packet...
  tx_packet.connect_request.mid_1 = sys_params.bridge_mid.mid_1;
  tx_packet.connect_request.mid_2 = sys_params.bridge_mid.mid_2;
  tx_packet.connect_request.mid_3 = sys_params.bridge_mid.mid_3;
  tx_packet.connect_request.mid_4 = sys_params.bridge_mid.mid_4;

  // load the bind pnCode index
  RadioSetSopPnCode( sys_params.network_id.pn_code );
  // Force endstate to idle
//  RadioForceState( END_STATE_IDLE );
  
  // send a bind request and wait for response, if no response change
  // the channel and try again
  for( retry = 0; retry < CONNECT_RETRY_COUNT; retry++ )
  {
    RadioSetChannel( sys_params.network_id.channel );

    if( radio_send_packet_and_adjust_DRPA( CONNECT_REQ_RETRIES, CONNECT_REQ_LEN ))
    {
      // Auto ACK off
      RadioSetXactConfig( RadioXactConfig & ~MSK_ACK_EN );
      packet_length = radio_receive_packet();
      // Auto ACK back on
      RadioSetXactConfig( RadioXactConfig | MSK_ACK_EN );

      if(( packet_length == CONNECT_RESP_LEN )
         &&( rx_packet.connect_response.hdr.type == CONNECT_RESP )
           &&( rx_packet.connect_response.hdr.flag == 1 ))
      {
#ifdef  TX_DEBUG
			SendString(( U8 *)"\r\nprotocol_reconnect ok!");
#else
	delay10us();
#endif       
        connected_state = PROTOCOL_CONNECTED;
        break;
      } 
    }

    // Try a few times on the original channel
    if( retry > ORIG_CHAN_RECONNECT_COUNT )
    {
      get_next_channel();
    }

    if( Timer_is_timeout( timer ))
    {
      timer = Timer_set_timer( TIMER_MS( 50 ));
      red_led_flash();
    }
      
  }  // end for loop
#ifdef  TX_DEBUG
	SendString(( U8 *)"\r\nForce endstate to sleep");
#else
	delay10us();
#endif 
  // Force endstate to sleep
  RadioForceState( END_STATE_SLEEP );
  
  red_led_off();
  green_led_off();
  green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:    retrieve_bridge_mid
//
//!  Description: Retrieve the bridge MID saved in the flash
//!               
//!
//!  Inputs:      Void
//!
//!  Returns:     Void
//!
///////////////////////////////////////////////////////////////////////////////
void retrieve_bridge_mid( void )
{
  read_machine_serial();
  
  // get nvram
  NVRAM_read_sys_params();
/*  
  GetDataIdInfo( D_SONIX, &Start_Sector, &Length );
#ifdef SDBOOT_KBDM9
  sdtl_resv_read( Start_Sector, ( __X void *)Buf );
#endif
#ifdef SPIBOOT_KBDM9
  spif_resv_read( Start_Sector, ( __X void *)Buf );
#endif
  IndexDataAddress = 0;
*/   
  // check for uninitialized flash
  if (sys_params.bridge_mid.signature != SIGNATURE_BYTE )
  {
    // set channel and pn_code to a value that will allow the different
    // devices to coexist and interfere less with each other
    sys_params.network_id.channel = 0;
    sys_params.network_id.pn_code = 0;
    sys_params.network_id.seed = 0;
    sys_params.network_id.pin = 0;
  }
  else
  {
    // calculate channel, PN code, etc. from Bridge MID and store
    // values in sys_params
    calculate_network_id();
  }
}


#ifdef DYNAMIC_DATA_RATE_PA
///////////////////////////////////////////////////////////////////////////////
//
//  Function:    radio_toggle_data_mode
//
//!  Description: Toggle the trasmission data mode
//!               
//!
//!  Inputs:      Void
//!
//!  Returns:     Void
//!
///////////////////////////////////////////////////////////////////////////////
void radio_toggle_data_mode( void )
{
  // Toggle the data mode
  if(( RadioTxConfig & MSK_TX_DATMODE )== DATMODE_1MBPS )
  {
    // Change to 8DR mode, maximum PA
    RadioTxConfig = ( RadioTxConfig & ~( MSK_TX_DATMODE | MSK_PA_VAL ))|( DATMODE_8DR | DATA_MODE_PA_MAX );
    // Change the ACK timeout
    RadioSetXactConfig(( RadioXactConfig & ~MSK_ACK_TO ) | ACK_TO_8X );
    // Disable all slow 
    RadioWrite( ANALOG_CTRL_ADR, ANALOG_CTRL_RST );  
  }
  else //DATMODE_8DR
  {
    // Change to GFSK mode, maximum PA
    RadioTxConfig = ( RadioTxConfig & ~( MSK_TX_DATMODE | MSK_PA_VAL ))|( DATMODE_1MBPS | DATA_MODE_PA_MAX );
    // Change the ACK timeout
    RadioSetXactConfig(( RadioXactConfig & ~MSK_ACK_TO )| ACK_TO_4X );
    // Enable all slow 
    RadioWrite( ANALOG_CTRL_ADR, ALL_SLOW_EN );  
  }
  RadioSetTxConfig( RadioTxConfig );

  total_retry = 0;
  pkt_count = 0;
}
#endif  //DYNAMIC_DATA_RATE_PA


///////////////////////////////////////////////////////////////////////////////
//
//  Function:    radio_send_packet_and_adjust_DRPA
//
//!  Description: send a packet and do the dynamic data rate and PA
//!               
//!
//!  Inputs:      retry time and packet length
//!
//!  Returns:     
//!               - TRUE  - for successful transmission
//!               - FALSE - for unsuccessful transmission
//!
///////////////////////////////////////////////////////////////////////////////
Bool radio_send_packet_and_adjust_DRPA( U8 retry, U8 length )
{
  Bool success = radio_send_packet( retry, length );
#ifdef DYNAMIC_DATA_RATE_PA
  // If fail in one data mode, try another data mode. If both fail, the device will assume that the channel
  // has been changed and start to search the new channel.
  if( success == FALSE )
  {
    radio_toggle_data_mode();
    success = radio_send_packet( retry, length );
  }

  // Only if the data packet is transmitted successfully, we will do the dynamic data rate /PA. Otherwise, 
  // we assume the dongle has changed channel, and the device needs to search the new channel.
  if(( success == TRUE )&&( retry == DATA_PACKET_RETRIES ))
  {
#ifndef REMOVE_DYN_PA
    U8 rx_rssi = radio_get_rx_rssi();
#endif //REMOVE_DYN_PA
    U8 act_retry = retry - RadioRetryCount;
        
    // Dynamic Data Rate
    if( act_retry <( 0xFF - total_retry ))
      total_retry += act_retry;
    else
      total_retry = 0xFF;
            
    pkt_count++;

    if( pkt_count == PKT_NUM )
    {
      if( total_retry >= total_retry_threshold )
      {
        if(( total_retry >= total_retry_threshold_last )
           ||( over_retry_count == OVER_RETRY_NUM ))
        {
          total_retry_threshold = total_retry;
          total_retry_threshold_last = total_retry;
          over_retry_count = 0;
          // Toggle data mode
          radio_toggle_data_mode();
        }
        else
          over_retry_count++;
      }
      else if( total_retry == 0 )
      {
#ifndef REMOVE_DYN_PA
        // When the signal is strong, decrease PA. In this way the power will be saved.
        if(( rx_rssi > PA_RSSI_RX_THRESHOLD )
           &&(( RadioTxConfig & MSK_PA_VAL )> DATA_MODE_PA_MIN ))
        {	
          // Decrease the PA. Take advantage of PA_VAL_MSK = 0x7
          RadioTxConfig--;
          RadioSetTxConfig( RadioTxConfig );

          total_retry_threshold = total_retry_threshold_last;
        } 
        else
#endif //REMOVE_DYN_PA
        if( total_retry_threshold > TOTAL_RETRY_THRESHOLD_LOW )
          total_retry_threshold--;
      }

      total_retry = 0;
      pkt_count = 0;
    }

#ifndef REMOVE_DYN_PA
    if(( rx_rssi <= PA_RSSI_RX_THRESHOLD )    // The signal is weak
       ||( act_retry >= PA_RETRY_THRESHOLD )) // Retries exceeds the PA adjusting retry threshold
    {
      // Set the maximum PA
      radio_set_pa( DATA_MODE_PA_MAX );
    }
#endif //REMOVE_DYN_PA
  }
#endif //DYNAMIC_DATA_RATE_PA

  return success;
}

//--------------------------------------------------------------------------
// calculate_network_id
//--------------------------------------------------------------------------
void calculate_network_id( void )
{
#if 1
  sys_params.network_id.pn_code = ( sys_params.bridge_mid.mid_1 << 2 )
                                    + sys_params.bridge_mid.mid_2
                                      + sys_params.bridge_mid.mid_3;

  while( sys_params.network_id.pn_code >= NUM_PN_CODES )
  {
     sys_params.network_id.pn_code -= NUM_PN_CODES;
  }

  sys_params.network_id.channel = ( sys_params.bridge_mid.mid_2 >> 2 )
                                    - ( sys_params.bridge_mid.mid_1 << 5 )
                                      + sys_params.bridge_mid.mid_3;

  while( sys_params.network_id.channel >= NUM_CHANNELS )
  {
    sys_params.network_id.channel -= NUM_CHANNELS;
  }

  sys_params.network_id.pin = ((( sys_params.bridge_mid.mid_1 - sys_params.bridge_mid.mid_2 )& PIN_MASK ) + MIN_PIN );

  sys_params.network_id.seed = (( sys_params.bridge_mid.mid_2 >> 6 ))
                                + sys_params.bridge_mid.mid_1
                                  + sys_params.bridge_mid.mid_3;
  
  // If seed equal 0, let it to be 1
  if( sys_params.network_id.seed == 0 )
    sys_params.network_id.seed++;

#else 
  U8 temp_1,temp_2,temp_3,temp_4;

  temp_1 = sys_params.bridge_mid.mid_1;
  temp_2 = sys_params.bridge_mid.mid_2;
  temp_3 = sys_params.bridge_mid.mid_3;
  temp_4 = sys_params.bridge_mid.mid_4;
  
  sys_params.network_id.pn_code = ( temp_1 << 2 ) + temp_2 + temp_3;

  while( sys_params.network_id.pn_code >= NUM_PN_CODES )
  {
     sys_params.network_id.pn_code -= NUM_PN_CODES;
  }

  sys_params.network_id.channel = ( temp_2 >> 2 ) - ( temp_1 << 5 ) + temp_3;

  while( sys_params.network_id.channel >= NUM_CHANNELS )
  {
    sys_params.network_id.channel -= NUM_CHANNELS;
  }

  sys_params.network_id.pin = ((( temp_1 - temp_2 )& PIN_MASK ) + MIN_PIN );

  sys_params.network_id.seed = (( temp_2 >> 6 )) + temp_1 + temp_3;
  
  // If seed equal 0, let it to be 1
  if( sys_params.network_id.seed == 0 )
    sys_params.network_id.seed++;

 #ifdef  TX_DEBUG
	SendString(( U8 *)"\r\npn_code=");
	send_hex_u8(sys_params.network_id.pn_code);
	SendString(( U8 *)"\r\nchannel=");
	send_hex_u8(sys_params.network_id.channel);
	SendString(( U8 *)"\r\npin=");
	send_hex_u8(sys_params.network_id.pin);
	SendString(( U8 *)"\r\nseed=");
	send_hex_u8(sys_params.network_id.seed);

#endif 

#endif  
}


//--------------------------------------------------------------------------
// get_next_channel
//--------------------------------------------------------------------------
void get_next_channel( void )
{
  sys_params.network_id.channel += ( 6 * ( sys_params.network_id.pin + 1 ));

  if(( NUM_CHANNELS ) - 1 < sys_params.network_id.channel )
  {
    sys_params.network_id.channel -= NUM_CHANNELS;
  }
}


//--------------------------------------------------------------------------
// radio_set_pa
//--------------------------------------------------------------------------
void radio_set_pa( U8 pa )
{
  RadioTxConfig = ( RadioTxConfig & ~MSK_PA_VAL )| pa;
  RadioSetTxConfig( RadioTxConfig );
}


//--------------------------------------------------------------------------
// radio_set_nonzero_crc_seed
//--------------------------------------------------------------------------
void radio_set_nonzero_crc_seed( void )
{
  // Set CRC
  RadioSetCrcSeed(((( U16 )sys_params.network_id.seed)<< 8 ) + ( U16 )sys_params.network_id.seed );
}


#ifdef BACK_CHANNEL_SUPPORT
///////////////////////////////////////////////////////////////////////////////
//
//  Function:    RadioSetBackChannelCrcSeed
//
//!  Description: Set the back channel CRC seed
//!               
//!
//!  Inputs:      Void
//!
//!  Returns:     Void
//!
///////////////////////////////////////////////////////////////////////////////

void RadioSetBackChannelCrcSeed( void )
{
  // Set CRC
  RadioSetCrcSeed(((( U16 )( ~sys_params.network_id.seed ))<< 8 ) + ( U16 )sys_params.network_id.seed );
}
#endif //BACK_CHANNEL_SUPPORT


#ifdef DYNAMIC_DATA_RATE_PA
//--------------------------------------------------------------------------
// radio_get_rx_rssi
//-------------------------------------------------------------------------
U8 radio_get_rx_rssi( void )
{
  U8 rssi;

  rssi = RadioGetRssi();

  // Only the transmission RSSI is needed. If RSSI was NOT triggered from SOP, return 0.
  if (!(rssi & MSK_SOP_RSSI))
  {
    return 0;
  }

  return rssi & MSK_RSSI_LVL;
}
#endif

//--------------------------------------------------------------------------
// radio_send_packet
//-------------------------------------------------------------------------
Bool radio_send_packet( U8 retry, U8 length )
{
  U8 status;
  RadioSetPtr(( U8 *)&rf_tx_packet );
//  SendString((U8 *)"rf tx=");
  rf_tx_packet[ 0 ] = tx_packet.first.byte;
//  send_hex_u8(rf_tx_packet[ 0 ]);
//  SendString((U8 *)" ");
  for( status = 0; status < APP_TX_PACKET_SIZE; status++ )
  {
    rf_tx_packet[ status + 1 ] = tx_packet.data_packet.app_packet[ status ];
//    send_hex_u8(rf_tx_packet[ status + 1 ]);
//    SendString((U8 *)" ");
  }
//  SendString((U8 *)"\r\n");
   
  
  status = RadioBlockingTransmit( retry, length );
#if 0//def TX_DEBUG
   SendString((U8 *)"status=");
   send_hex_u8(status);
#else
   delay10us();
#endif
  if(( status &( RADIO_ERROR | RADIO_COMPLETE )) == RADIO_COMPLETE )
    return TRUE;
  else
    return FALSE;
}


//--------------------------------------------------------------------------
// radio_receive_packet
//--------------------------------------------------------------------------
U8 radio_receive_packet( void )
{
  U16 timeout;
  U8 status;
  U8 pkt_len = 0;

  radio_start_receive();
//  delay10us();
  // Spin until packet received or timeout
  for( timeout = 0; timeout < RX_PACKET_TIMEOUT; ++timeout )
  {
    status = RadioGetReceiveState();
#ifdef TX_DEBUG
	SendString((U8 *)"\r\nreciver status = ");
	send_hex_u8(status);
#else
	delay1ms(1);
#endif	
    if(( status &( RADIO_ERROR | RADIO_COMPLETE )) == RADIO_COMPLETE )
    {
      pkt_len = RadioEndReceive();
      break;
    }
    else if( status & RADIO_ERROR )
    {
      ( void )RadioEndReceive();
      radio_start_receive();
    }

    //delay10us( 5 );  //delay 50us
   delay10us();delay10us();delay10us();delay10us();delay10us();
     //delay100us();
  }
  if( timeout == RX_PACKET_TIMEOUT )
  {
    ( void )RadioAbort();
  }

  return pkt_len;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:    radio_start_receive
//
//!  Description: start to receive a packet
//!               
//!
//!  Inputs:      Void
//!
//!  Returns:     Void
//!
///////////////////////////////////////////////////////////////////////////////
void radio_start_receive( void )
{
  RadioSetLength( sizeof( rf_rx_packet ));
  RadioSetPtr(( U8 * )&rf_rx_packet );
  RadioStartReceive();
}

#if 0
//--------------------------------------------------------------------------
// radio_force_end_state
//--------------------------------------------------------------------------
void radio_force_end_state( U8 endstate )
{
  RadioSetXactConfig(( RadioXactConfig & ~MSK_END_STATE) | endstate | MSK_FRC_END_STATE );

  // Wait for transition to complete
  while( RadioGetXactConfig() & MSK_FRC_END_STATE );
}
#endif


void read_machine_serial( void )
{
   U16 i, j, *p = ( U16 *)&Buf[ 0 ];

   Read_WingedMonkey_Serial( Buf );

   for( i = 0; i < 256; i++ )
   {
      if( *p == 0 )
         break;
      p++;
   }

   p = ( U16 *)&Buf[ 0 ];
   machine_id = 0;
   for( j = 0; j < i; j++ )
   {
      machine_id = machine_id * 10 +( *p - '0' );
      p++;
   }
}


//--------------------------------------------------------------------------
// NVRAM_read_sys_params
//--------------------------------------------------------------------------
void NVRAM_read_sys_params( void )
{
  SPIF_ERR SPI_STATE;
  U8 i;

#if 1
   GetDataIdInfo(D_NET, &Start_Sector, &Length);
///*
   if( SPI_STATE != SDTL_No_Err )
	  {
	    sys_params.bridge_mid.signature = 0x00;
	    return;
	  }
//*/

   SPI_STATE = spif_resv_read( Start_Sector, (__X void *)Buf) ; 
  SendString((U8 *)"\r\nread spi =");
   send_hex_u8(SPI_STATE);
  memcpy(( U8 *)&sys_params, &Buf[ 0 ], sizeof( SYS_PARAMETERS ));

#if 0//def  TX_DEBUG
   SendString((U8 *)"\r\nstart_sector: ");
   send_hex_u32( Start_Sector );

   SendString((U8 *)": ");
   send_hex_u32(Length);
    SendString((U8 *)"\r\nbuf=");
   send_hex_u32(Length);
   for(i = 0;i < 9;i ++){
     send_hex_u32(Buf[i]);
	 SendString((U8 *)": ");
   	}
#endif

#if 0
  sys_params.bridge_mid.signature = 0x90;
//  sys_params.bridge_mid.signature = 0x91;
  sys_params.bridge_mid.mid_1 = 0xa4;
  sys_params.bridge_mid.mid_2 = 0x01;
  sys_params.bridge_mid.mid_3 = 0x5b;
  sys_params.bridge_mid.mid_4 = 0x99;
//  sys_params.network_id.channel=0x41;
//  sys_params.network_id.pn_code=0x08;
//  sys_params.network_id.seed=0x9b;
//  sys_params.network_id.pin=0x04;
  sys_params.network_id.channel=0x3f;
  sys_params.network_id.pn_code=0x10;
  sys_params.network_id.seed=0xff;
  sys_params.network_id.pin=0x05;
#endif
#else
  GetDataIdInfo( D_NET, &Start_Sector, &Length );
  SPI_STATE = spif_resv_read( Start_Sector, ( __X void *)Buf );
  if( SPI_STATE != SDTL_No_Err )
  {
    sys_params.bridge_mid.signature = 0x00;
    return;
  }


  //memcpy(( U8 *)&machine_id, &Buf[ 0x1FC ], sizeof( U32 ));
  /*
  memcpy(( U8 *)&sys_params, &Buf[ 0 ], sizeof( SYS_PARAMETERS ));
  memcpy(( U8 *)&sys_params_crc16, &Buf[ sizeof( SYS_PARAMETERS )], sizeof( U16 ));
  if( sys_params_crc16 != cal_crc16(( U8 *)&sys_params, sizeof( SYS_PARAMETERS )))
  {
    memcpy(( U8 *)&sys_params, &Buf[ 0+0x10 ] , sizeof( SYS_PARAMETERS ));
    memcpy(( U8 *)&sys_params_crc16, &Buf[ sizeof( SYS_PARAMETERS )+ 0x10 ] , sizeof( U16 ));
    if( sys_params_crc16 != cal_crc16(( U8 *)&sys_params, sizeof( SYS_PARAMETERS )))
    {
      sys_params.bridge_mid.signature = 0x00;
      return;
    }
    memcpy( &Buf[ 0 ], &Buf[ 0x10 ], ( sizeof( SYS_PARAMETERS )+ sizeof( U16 )));
    spif_resv_write( Start_Sector, ( __X void *)Buf );
  }
  */
#endif  
}


//--------------------------------------------------------------------------
// NVRAM_write_sys_params
//--------------------------------------------------------------------------
void NVRAM_write_sys_params( void )
{
  SPIF_ERR SPI_STATE;

#if 1
   GetDataIdInfo(D_NET, &Start_Sector, &Length);

  // SendString((U8 *)"\r\nstart_sector: ");
 //  print_hex(&Start_Sector, sizeof(Start_Sector));

   memcpy(&Buf, &sys_params, 9);

//   SPIF_Read_Write();
   SPI_STATE = spif_resv_write( Start_Sector, (__X void *)Buf) ;
   SendString((U8 *)"\r\nwrite spi =");
   send_hex_u8(SPI_STATE);
#else
  GetDataIdInfo( D_NET, &Start_Sector, &Length );
  SPI_STATE = spif_resv_read( Start_Sector, ( __X void *)Buf );
  if( SPI_STATE != SDTL_No_Err )
  {
    return;
  }
  sys_params_crc16 = cal_crc16(( U8 *)&sys_params, sizeof( SYS_PARAMETERS ));
  if( !memcmp(( U8 *)&sys_params, &Buf[ 0 ] , sizeof( SYS_PARAMETERS )))
  {
    if( !memcmp(( U8 *)&sys_params_crc16, &Buf[ sizeof( SYS_PARAMETERS )] , sizeof( U16 )))
      return;
  }
  memcpy( &Buf[ 0 ], ( U8 *)&sys_params, sizeof( SYS_PARAMETERS ));
  memcpy( &Buf[ sizeof( SYS_PARAMETERS )], ( U8 *)&sys_params_crc16, sizeof( U16 ));
  memcpy( &Buf[ 0x10 ], &Buf[ 0 ], ( sizeof( SYS_PARAMETERS )+ sizeof( U16 )));
  spif_resv_write( Start_Sector, ( __X void *)Buf );
#endif  
}


#ifdef BACK_CHANNEL_SUPPORT
void NotifyDownloadBackChannelData( void )
{            
//  back_channel_data = ( rx_packet.data_packet.app_packet[ 0x00 ]<< 2 ) & 0x0C;
}                
#endif //BACK_CHANNEL_SUPPORT


#pragma CODE = TASK_INIT_CODE

void wireless_reset( void )
{
   U32 timer;
   
   set_rf_rst();
   timer = Timer_set_timer( TIMER_MS( 500 ));
   while( !Timer_is_timeout( timer ));

   clr_rf_rst();
}

void rf_spi_master_init( void )
{
   PBEN |= ( BIT( 3 )| BIT( 2 )| BIT( 1 ));
   rf_SP1CR = SPI_SPE( 1 )|
               SPI_SPFIE( 0 )|
               SPI_SPIS( 0 )|
               SPI_SPIE( 0 )|
               SPI_MFEN( 0 )|
               SPI_MSTR( 1 )|
               SPI_MODE( SPI_Mode0 )|
               SPI_SPR( SPI_4MHz );
}


void wireless_task_init( void )
{
  SendString((U8 *)"wireless_task_init\r\n");

//	   PCEN=0;
//		PBEN=0;	 
   _bitCLR( _PBEN_b0 + 15 );
   _bitCLR( _PCEN_b0 + 10 );
   _bitCLR( _PCEN_b0 + 3 );
   _bitSET(RF_IRQ_DIR); // Set RF_IRQ with Input.
   wireless_reset();
   // Turn off Red and Green LEDs
   //Red_Led_Off();
   //Green_Led_Off();
   /*SendString((U8 *)"PBEN=");
   send_hex_u16(PBEN);
   SendString((U8 *)"\r\n");
   SendString((U8 *)"SP1CR=");
   send_hex_u16(SP1CR);
   SendString((U8 *)"\r\n");*/

   //nor_SP1CR = SP1CR;
#if( RF_SPI_MODE == DISABLE )
   _bitCLR( RF_CLK );
   _bitCLR( RF_CLK_DIR );

   _bitSET( RF_DIN );
   _bitSET( RF_DIN_DIR );

   _bitSET( RF_DOUT );
   _bitSET( RF_DOUT_DIR );

   
#endif   //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
   nor_SP1CR = SP1CR;
   rf_spi_master_init();
#endif   //#if( RF_SPI_MODE == ENABLE )
   mouse_state = MOUSE_STATE_DISCONNECTED;
   protocol_init();
   //g_b_wireless_flag = TRUE;
   SendString((U8 *)"wireless_task_init over\r\n");
}


#pragma CODE = PROTOCOL_CODE

void wireless_task( void )
{
   wireless_evt();
   if( MSCOUNTER_5 && Timer_is_timeout( MSCOUNTER_5 ))
   {
      MSCOUNTER_5 = 0;
      red_led_off();
      green_led_off();
      green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );
   }
}
#endif  // _protocol_c_
