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
//#include "iom8.h"
//#include "ina90.h"
#include "protocol.h"
#include "compiler.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "lpnonstreaming.h"
#include "oid.h"
#include "stdio.h"
#include "keypad.h"
#include "com.h"
#include <string.h>
#include "com.h"
#include "int_drv.h"
#include "BinaryImage.h"
#include "power.h"
#include "system.h"

#include "spi.h"

//_____ M A C R O S ________________________________________________________
#pragma DATA = PROTOCOL_DATA
#pragma CODE = PROTOCOL_CODE

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


//--------------------------------------
// Local Variable Definitions
//--------------------------------------
// The protocol module owns the memory for the tx/rx packets
// The APIs are set up for zero-copy
//! Transmit packet
LP_PACKET tx_packet;

// System parameters for radio communication
SYS_PARAMETERS sys_params;

PROTOCOL_CONNECT_STATE  connected_state;
LP_PACKET               rx_packet;


U8 rf_recive_data[LEN_RF_RECIVE_DATA];
U8 rf_send_data[LEN_RF_RECIVE_DATA];

//***********************************************
//SYS_PARAMETERS eeprom_params @ 0x0002;//0x2002;
//U32 machine_id @ 0x01FC;//0x21FC;
//SYS_PARAMETERS eeprom_params;//0x2002;
U32 machine_id;//0x21FC;
//***********************************************

// Transmit packet data toggle
U8 tx_data_toggle = 1;

U8 RadioTxConfig;




void wireless_reset( void )
{
   U32 timer;
   
   set_rf_rst();
   RedLed_OFF();
   timer = Timer_set_timer( TIMER_MS( 500 ));
   while( !Timer_is_timeout( timer ));
   RedLed_ON();
   clr_rf_rst();
}

void rf_spi_master_init( void )
{
   PBEN |= ( BIT( 3 )| BIT( 2 )| BIT( 1 ));
//   PCEN |= ( BIT( 9 )| BIT( 8 )| BIT( 7 ));
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

//	   PCEN=0;
//		PBEN=0;	 
    _bitSET(RF_IRQ_DIR); // Set RF_IRQ with Input.
	_bitSET(RF_IRQ_PH); // Set RF_IRQ with Output.
/*
	_bitSET(RF_MISO_DIR); // Set RF_MISO with Input.

	_bitCLR(RF_MOSI_DIR); // Set RF_MOSI with Output.
	_bitSET(RF_MOSI_PH);  // Set FR_MOSI with Pull-high.

	_bitCLR(RF_SCK_DIR); // Set FR_SCK with output.
	_bitSET(RF_SCK_PH);  // Set FR_SCK with Pull-high.



	_bitCLR(RF_SS_DIR); // Set RF_RST with Output.
	_bitSET(RF_SS_PH);  // Set FR_RST with Pull-high.
	_bitCLR(RF_SS_OD);  // Set FR_SS is low.
*/

	_bitSET(RF_SS_DIR);  //set rf_cs output
	//_bitSET(RF_SS_PH);	 //set rf_cs ph
	_bitSET(RF_SS_OD);   //clr rf_cs low


	_bitCLR(RF_RST_DIR); // Set RF_RST with Output.
	_bitSET(RF_RST_PH);  // Set FR_RST with Pull-high.
	
	_bitSET(RF_RST_OD);  // Set FR_RST.

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
   //mouse_state = MOUSE_STATE_DISCONNECTED;
   //protocol_init();
   //g_b_wireless_flag = TRUE;
}


//--------------------------------------------------------------------------
// protocol_init
//--------------------------------------------------------------------------
void protocol_init( void )
{
   U8  ttt;
  // Configure the Radio
  // The ACK Timeout (Bit[1:0], Register XACT_CFG_ADR) sets the timeout period
  // after transmission of a packet during which an ACK must be correctly
  // received in order to prevent a transmit error condition from being detected.
  // This design uses the 11b setting for this field which is equivalent to 
  // 15 times the SOP_CODE_ADR code length. This gives the radio amble time to
  // safely detect Timeout condition. Theoretically, the "8x" setting (01b) 
  // should be sufficient, and it will slightly improve the device power 
  // consumption. Complete testing is recommended if this field is changed.

	U8 temp[6];
//  RadioInit( MSK_ACK_EN | END_STATE_SLEEP | ACK_TO_15X, PA_4_DBM | DATCODE_LEN_32 | DATMODE_8DR );
  // Configure the Radio
//  RadioTxConfig = DATA_MODE_PA_MAX | DATCODE_LEN_32 | DATMODE_8DR;
  RadioTxConfig = DATA_MODE_PA_MAX | DATCODE_LEN_32 | DATMODE_1MBPS;
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
///*
  {
   U8 _MEM_TYPE_SLOW_ temp[ 6 ]; // Only need 6 bytes, but leave it for now.
   // Retrieve the bridge MID
   // Set receive length
   RadioSetLength( sizeof( temp ));
   // Set buffer pointer
   RadioSetPtr( temp );
   // Get MID
   RadioGetFuses();

   SendString((char *)"mid1=");
   send_hex_u8(rf_recive_data[ 0 ]);
   SendString((char *)"\r\n");
   SendString((char *)"mid2=");
   send_hex_u8(rf_recive_data[ 1 ]);
   SendString((char *)"\r\n");
   SendString((char *)"mid3=");
   send_hex_u8(rf_recive_data[ 2 ]);
   SendString((char *)"\r\n");
   SendString((char *)"mid4=");
   send_hex_u8(rf_recive_data[ 3 ]);
   SendString((char *)"\r\n");
    SendString((char *)"mid5=");
   send_hex_u8(rf_recive_data[ 4 ]);
   SendString((char *)"\r\n");
 SendString((char *)"mid6=");
   send_hex_u8(rf_recive_data[ 5 ]);
   SendString((char *)"\r\n");

   
  }
 // */
  // Retrieve the bridge MID
  retrieve_bridge_mid();

  
  
  // Set CRC
  radio_set_nonzero_crc_seed();

  connected_state = PROTOCOL_DISCONNECTED;

  
#if 0
  RadioSetLength( sizeof( temp ));
   // Set buffer pointer
   RadioSetPtr( temp );
   // Get MID
  
   RadioGetFuses();

   SendString((U8 *)"\r\n1111111111111111111temp: ");
   print_hex(&temp, sizeof(temp));
#endif
//	red_led_on();
//   green_led_on();
  
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
  
  RedLed_ON();
  BlueLed_OFF();
  timer = Timer_set_timer( TIMER_MS( 250 ));
  
  // Enter disconnected state
  connected_state = PROTOCOL_DISCONNECTED;

  // Initialize tx packet header
  tx_packet.first.byte = ( BIND_REQ << 4 )|( dev_type << 1 );

  // Set the bind pn code
#ifdef  TX_DEBUG 
  SendString((char *)"\r\nss1");
#endif
  RadioSetSopPnCode( BUTTON_BIND_PN_CODE );

  // seed the channel algorithm with bind parameters 0,0
  sys_params.network_id.channel = BIND_BASE_CHANNEL;

  // Set Zero CRC
#ifdef  TX_DEBUG  
  SendString((char *)"\r\nss2");
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
  SendString((char *)"\r\nss3");
#endif
  // Force endstate to idle
  RadioForceState( END_STATE_IDLE );
#ifdef  TX_DEBUG
  SendString((char *)"\r\nss4");
#endif
  // send a bind request and wait for response, if no response change
  // the channel and try again
  while( bind_retry < BIND_RETRY_COUNT )
  {
    bind_retry++;

    RadioSetChannel( sys_params.network_id.channel );
#ifdef  TX_DEBUG
    SendString((char *)"\r\nss5");
#endif
    if( radio_send_packet( BIND_REQ_RETRIES, BIND_REQ_LEN ))
    {
#ifdef  TX_DEBUG    
	  SendString((char *)"\r\nss6");
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
	SendString(( char *)"\r\nbind ok!");
#endif	        
        break;
      }
    }
#ifdef  TX_DEBUG	
  SendString((char *)"\r\nss7");
#endif
    get_next_channel();
#ifdef  TX_DEBUG
     SendString((char *)"\r\nss8");
#endif
    if( Timer_is_timeout( timer ))
    {
      timer = Timer_set_timer( TIMER_MS( 250 ));
      RedLed_FLASH();
    }
#ifdef  TX_DEBUG	
	SendString((char *)"\r\nss9");
#endif
    
   // kbd_process();
  //  system_power_off();
  }

  // Force endstate to sleep
  RadioForceState( END_STATE_SLEEP );
  
  RedLed_OFF();
  BlueLed_OFF();
  // Restore PA setting to default state
//  radio_set_pa( PA_4_DBM );
  radio_set_pa( DATA_MODE_PA_MAX );
    
  if( status == PROTOCOL_SUCCESS )
  {

#ifdef  TX_DEBUG
	SendString(( char *)"\r\nPROTOCOL_SUCCESS!");
#endif	  
    // store parameters in NVRAM
    NVRAM_write_sys_params();
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
	SendString(( char *)"\r\nPROTOCOL_FAIL!");
#endif	  
    // can't Bind, reset to original bind parameters
    retrieve_bridge_mid();
    
    // Set CRC
    radio_set_nonzero_crc_seed();
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
                                    )
{

  if( connected_state == PROTOCOL_DISCONNECTED )
  {
    protocol_reconnect( dev_type );

    return PROTOCOL_TX_TIMEOUT;
  }

  if( data_length < APP_TX_PACKET_SIZE + 1 )
  {
     // Setup the packet header
    tx_packet.first.byte = ( DATA_PACKET << 4 )
                            |( tx_data_toggle << 2 )
                              |(( data_dev_type << 1 | data_dev_type >> 1 )& DATA_DEV_TYPE_MASK );
    // calculate check sum
    data_length += 1;   // length = header + data
  }
  else
  {
    return PROTOCOL_FAILED;
  }
/*
#ifdef BACK_CHANNEL_SUPPORT
  // Force endstate to idle
  RadioForceState( END_STATE_IDLE );
#endif //BACK_CHANNEL_SUPPORT
*/  
#if 0//def  TX_DEBUG
{
	U8 i=0;
	SendString(( U8 *)"\r\ntx_packet=");
	send_hex_u8(tx_packet.first.byte);
	for(;i<15;i++){
	SendString(( U8 *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[i]);
		}
		SendString(( U8 *)"\r\n");
}
#endif	

  if( radio_send_packet_and_adjust_DRPA( DATA_PACKET_RETRIES, data_length ))
  {
    tx_data_toggle ^= 1;
    
    return PROTOCOL_SUCCESS;
  }
  
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
  
/*
#ifdef  TX_DEBUG
	SendString(( char *)"\r\sys_params=");
	send_hex_u8(sys_params.bridge_mid.signature);
	SendString(( char *)":");
	send_hex_u8(sys_params.bridge_mid.mid_1);
	SendString(( char *)":");
	send_hex_u8(sys_params.bridge_mid.mid_2);
	SendString(( char *)":");
	send_hex_u8(sys_params.bridge_mid.mid_3);
	SendString(( char *)":");
	send_hex_u8(sys_params.bridge_mid.mid_4);
	SendString(( char *)":");
	send_hex_u8(sys_params.network_id.channel);	
	SendString(( char *)":");
	send_hex_u8(sys_params.network_id.pn_code);
	SendString(( char *)":");
	send_hex_u8(sys_params.network_id.seed);
	SendString(( char *)":");
	send_hex_u8(sys_params.network_id.pin);
#endif  
*/
  // check for unbound device
  if( sys_params.network_id.pin == 0 )
  {
    return;
  }
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nprotocol_reconnect");

#endif  
  BlueLed_OFF();
  RedLed_ON();
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
      RedLed_FLASH();
    }
      
  }  // end for loop

  // Force endstate to sleep
  RadioForceState( END_STATE_SLEEP );
  
  RedLed_OFF();
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
  // get nvram
  NVRAM_read_sys_params();
   
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
/*  
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

    U8 rx_rssi = radio_get_rx_rssi();
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

        if( total_retry_threshold > TOTAL_RETRY_THRESHOLD_LOW )
          total_retry_threshold--;
      }

      total_retry = 0;
      pkt_count = 0;
    }


    if(( rx_rssi <= PA_RSSI_RX_THRESHOLD )    // The signal is weak
       ||( act_retry >= PA_RETRY_THRESHOLD )) // Retries exceeds the PA adjusting retry threshold
    {
      // Set the maximum PA
      radio_set_pa( DATA_MODE_PA_MAX );
    }

  }
#endif //DYNAMIC_DATA_RATE_PA
*/
  return success;
}

//--------------------------------------------------------------------------
// calculate_network_id
//--------------------------------------------------------------------------
void calculate_network_id( void )
{
#if 0
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

     sys_params.network_id.channel=0x3f;
  sys_params.network_id.pn_code=0x10;
  sys_params.network_id.seed=0xff;
  sys_params.network_id.pin=0x05;
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

 #if 0//def  TX_DEBUG
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
  U8 i;
  RadioSetPtr(( U8 *)&tx_packet );
//#ifdef  TX_DEBUG
//#ifdef  0
#if 0
	SendString(( char *)"\r\ntx_packet=");
	send_hex_u8(tx_packet.first.byte);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[0]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[1]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[2]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[3]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[4]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[5]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[6]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[7]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[8]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[9]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[10]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[11]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[12]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[13]);
	SendString(( char *)":");
	send_hex_u8(tx_packet.data_packet.app_packet[14]);
//#else	
	oid_pen_delay10us(30);
#endif 
	{
		rf_send_data[0]=tx_packet.first.byte;
		for(i=1;i<15;i++)
			{
			 rf_send_data[i] = tx_packet.data_packet.app_packet[i-1];
			}
	}
  status = RadioBlockingTransmit( retry, length );
	SendString(( char *)"status:");
	send_hex_u8(status);
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

  // Spin until packet received or timeout
  for( timeout = 0; timeout < RX_PACKET_TIMEOUT; ++timeout )
  {
    status = RadioGetReceiveState();

    if(( status &( RADIO_ERROR | RADIO_COMPLETE )) == RADIO_COMPLETE )
    {
      pkt_len = RadioEndReceive();
	  rx_packet.first.byte = rf_recive_data[0];
	  rx_packet.data_packet.app_packet[0]=rf_recive_data[1];
	  rx_packet.data_packet.app_packet[1]=rf_recive_data[2];
	  rx_packet.data_packet.app_packet[2]=rf_recive_data[3];
	  rx_packet.data_packet.app_packet[3]=rf_recive_data[4];
#if 0//def  TX_DEBUG
		SendString(( U8 *)"\r\nrx_packet=");
	    {
		U8 i=0;
		for(;i<6;i++){
			send_hex_u8( rf_recive_data[i]);
			SendString(( U8 *)":");
			}
	  	}
#endif 	  
      break;
    }
    else if( status & RADIO_ERROR )
    {
      ( void )RadioEndReceive();
      radio_start_receive();
    }

//    oid_pen_delay10us( 15 );  //delay 150us
    delay100us();
//	delay100us();
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
#if 1
  memset(rf_recive_data,0,LEN_RF_RECIVE_DATA);
  RadioSetLength(LEN_RF_RECIVE_DATA);
  RadioSetPtr((void *)&rf_recive_data);
  RadioStartReceive();
#else
  RadioSetLength( sizeof( rx_packet ));
  RadioSetPtr(( U8 * )&rx_packet );
  RadioStartReceive();
#endif  
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


void  init_wireless_port( void )
{
// 	_bitCLR(_PBEN_b0+1);

//	_bitCLR(_PBEN_b0+2);

//	_bitCLR(_PBEN_b0+3);


	_bitSET(RF_MISO_DIR); // Set RF_MISO with Input.

	_bitCLR(RF_MOSI_DIR); // Set RF_MOSI with Output.
	_bitSET(RF_MOSI_PH);  // Set FR_MOSI with Pull-high.

	_bitCLR(RF_SCK_DIR); // Set FR_SCK with output.
	_bitSET(RF_SCK_PH);  // Set FR_SCK with Pull-high.
}

//--------------------------------------------------------------------------
// NVRAM_read_sys_params
//--------------------------------------------------------------------------
void NVRAM_read_sys_params( void )
{
   __X unsigned long Start_Sector, Length;
   __X unsigned char Buf[512];   
   __X int res;

 //  Int_disable();

   GetDataIdInfo(D_WIRE_LESS, &Start_Sector, &Length);
 
#ifdef  TX_DEBUG
   SendString((char *)"\r\nstart_sector: ");
   send_hex_u32( Start_Sector );

   SendString((char *)"\r\nlen: ");
   send_hex_u32(Length);
#endif

   res = spif_resv_read( Start_Sector, (__X void *)Buf) ; 

   memcpy(&sys_params, Buf, 9);

 //  Int_enable();

  // SendString((U8 *)"\r\nres: ");
  // print_hex(&res, sizeof(res));

  // SendString((U8 *)"\r\nsys_params: ");
  // print_hex(&sys_params, sizeof(sys_params));

   init_wireless_port();

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

}


//--------------------------------------------------------------------------
// NVRAM_write_sys_params
//--------------------------------------------------------------------------
void NVRAM_write_sys_params( void )
{
   __X unsigned long Start_Sector, Length;
   __X unsigned char Buf[512];   
   __X int res;

 //  Int_disable();

   GetDataIdInfo(D_WIRE_LESS, &Start_Sector, &Length);

  // SendString((U8 *)"\r\nstart_sector: ");
 //  print_hex(&Start_Sector, sizeof(Start_Sector));

   memcpy(&Buf, &sys_params, 9);

//   SPIF_Read_Write();
   res = spif_resv_write( Start_Sector, (__X void *)Buf) ; 
   
//   Int_enable();

  // SendString((U8 *)"\r\nres: ");
 //  print_hex(&res, sizeof(res));

   init_wireless_port();
}


#ifdef BACK_CHANNEL_SUPPORT
void NotifyDownloadBackChannelData( void )
{            
//  back_channel_data = ( rx_packet.data_packet.app_packet[ 0x00 ]<< 2 ) & 0x0C;
}                
#endif //BACK_CHANNEL_SUPPORT

#endif  // _protocol_c_
