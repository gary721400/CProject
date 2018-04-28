//!
//! @file wireless.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the wireless.
//!
//! @version 1.53 snd3b-dvk-1_9_0 $Id: wireless.c,v 1.53 2007/04/03 09:49:23 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef _wireless_c_
#define _wireless_c_

//_____  I N C L U D E S ___________________________________________________
//#include "iom8.h"
//#include "ina90.h"
#include "config.h"
#include "compiler.h"
#include "wireless.h"
#include "protocol.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "lpnonstreaming.h"
#include "oid.h"
#include "keypad.h"
//#include "sio_drv.h"
#include "com.h"
#include "system.h"
//#include "power.h"
//#include "timer_drv.h"



#pragma CODE= WIRELESS_CODE
#pragma DATA= WIRELESS_DATA
//_____ M A C R O S ________________________________________________________

#define MOUSE_BUTTON_BIND     ( PIND &( 1 << PD3 ))

//===========================================================================


//_____ D E F I N I T I O N ________________________________________________
// Packet delivery timeout
#define MOUSE_TX_TIMEOUT_MS          	2000

//=============================================
 Bool   g_b_oid_data_flag = FALSE;
 Bool   g_b_kbd_data_flag = FALSE;
 Bool   g_b_wireless_bind_flag = FALSE;
 U32 	g_kbd_key;
 MOUSE_STATES mouse_state;

 extern _MEM_TYPE_SLOW_      U32  u32_oid_scan_time;
 extern __X unsigned int global_key_value ;
//--------------------------------------
// API Defines and Types
//--------------------------------------
typedef struct
{
  U32   oid_pen_index;
  U32   oid_pen_id;
  U8    packet_type;
  U8    packet_id;
}TX_PACKET;


extern  U32 machine_id;//0x21FC;

//extern _MEM_TYPE_SLOW_  U32  u32_red_green_led_time;
//extern _MEM_TYPE_SLOW_  U32  u32_red_green_flash_time;

U32  packet_counter;
//_____ D E C L A R A T I O N ______________________________________________

void  mouse_go_disconnected   ( void );
void  mouse_go_active         ( void );
void  mouse_do_bind           ( void );
void  mouse_do_report         ( void );
Bool  data_packet_resp        ( PROTOCOL_STATUS resp );

//the function is realize the wireless
void wireless_evt( void )
{
  if( mouse_state == MOUSE_STATE_DISCONNECTED )
  {
    mouse_go_disconnected();
  }
  else if( mouse_state == MOUSE_STATE_ACTIVE )
  {
    mouse_go_active();
  }
  mouse_do_bind();
}


//--------------------------------------------------------------------------
// mouse_go_disconnected
//--------------------------------------------------------------------------
void mouse_go_disconnected( void )
{
  TX_PACKET *radio_pkt = ( TX_PACKET *)protocol_get_tx_pkt();
  U32 send_timer;
  
  // State action
  if( mouse_state == MOUSE_STATE_DISCONNECTED )
  {
    // Force LP to sleep
//    RadioForceState( END_STATE_SLEEP );
    
    if( g_b_oid_data_flag || g_b_kbd_data_flag )
    {
//      g_b_oid_data_flag = FALSE;
//      g_b_kbd_data_flag = FALSE;
      // Force LP to sleep
      RadioForceState( END_STATE_SLEEP );
        
      // Send a null packet to establish connection
      radio_pkt->oid_pen_index = 0;
//      protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 2 );
      
      radio_pkt->oid_pen_id = machine_id;
      radio_pkt->packet_type = NULL_PACKET_TYPE;
      radio_pkt->packet_id = 0;
		  
      protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 10 );
//      protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 15 );
    }

    // The remaining lines of this block will affect the mouse_state.
    // Use caution in changing the order of evaluation.
    if( connected_state == PROTOCOL_CONNECTED )
    {
   
      send_timer = Timer_set_timer( TIMER_MS( 10 ));
      while( !Timer_is_timeout( send_timer ));
      mouse_state = MOUSE_STATE_ACTIVE;
      return;
    }
    
    g_b_oid_data_flag = FALSE;
    g_b_kbd_data_flag = FALSE;
  }
}


//--------------------------------------------------------------------------
// mouse_go_active
//--------------------------------------------------------------------------
void mouse_go_active( void )
{
  // State action
  if( mouse_state == MOUSE_STATE_ACTIVE )
  {
    // The following four lines will affect the mouse_state.
    // Use caution in changing the order of evaluation.
    mouse_do_report();
  }
}


//--------------------------------------------------------------------------
// mouse_do_bind
//--------------------------------------------------------------------------
void mouse_do_bind( void )
{
//  U32 timer;

//  if( MOUSE_BUTTON_BIND == 0 )
//  if( key == KBD_RELEASED )
  if( g_b_wireless_bind_flag )
  {
     SendString((char *)"\r\ngo bind 1");
//    g_kbd_key = NO_KEY;
    g_b_wireless_bind_flag = FALSE;

    // Perform the button bind procedure
    protocol_button_bind( MOUSE_DEVICE_TYPE );

    // Wait for button up
//    while( MOUSE_BUTTON_BIND == 0 );

    // Debouncing
//    timer = Timer_set_timer( TIMER_MS( 50 ));
//    while( !Timer_is_timeout( timer ));

    // Go to disconnected state to verify connection after bind
    g_b_oid_data_flag = FALSE;
    mouse_state = MOUSE_STATE_DISCONNECTED;
   // power_rearm_autopoweroff_delay();
  }
}


//--------------------------------------------------------------------------
// mouse_do_report
//--------------------------------------------------------------------------
#define DATA_PACKET_RESP            0x05
#define DATA_PACKET_RESP_LEN        5


void mouse_do_report( void )
{
  if( g_b_oid_data_flag || g_b_kbd_data_flag )
  {
    U32 index_temp;

    U32 timer,send_timer;
  
    PROTOCOL_STATUS resp = PROTOCOL_FAILED;
    TX_PACKET *radio_pkt = ( TX_PACKET * )protocol_get_tx_pkt();

    timer = Timer_set_timer( TIMER_MS( MOUSE_TX_TIMEOUT_MS ));
    // Send packet
    do
    {
      if( Timer_is_timeout( timer ))
      {
        mouse_state = MOUSE_STATE_DISCONNECTED;
        break;
      }

      if( g_b_oid_data_flag )
      {
//        index_temp = u32_current_index << 24;
//        index_temp |= ( u32_current_index << 8 )& 0x00FF0000;
//        index_temp |= ( u32_current_index >> 8 )& 0x0000FF00;
//        index_temp |= u32_current_index >> 24;
		  index_temp = Current_Index;
#if( OID_VERSION == OID1 )
        radio_pkt->packet_type = INDEX_PACKET_TYPE;
#elif( OID_VERSION == OID2 )
        radio_pkt->packet_type = INDEX_PACKET_TYPE | 0x80;
#endif
      }
      else if( g_b_kbd_data_flag )
      {
//        index_temp = ( U32 )g_kbd_key << 24;
//        index_temp |= (( U32 )g_kbd_key << 8 )& 0x00FF0000;
//		index_temp = ( U32 )g_kbd_key << 24;
//        index_temp |= (( U32 )g_kbd_key << 8 )& 0x00FF0000;
	//	index_temp = 0x0108;
	 index_temp = global_key_value ;
//	SendString("\r\n global_key_value = ");
//	send_hex_u16(global_key_value);
	
        radio_pkt->packet_type = KEYBOARD_PACKET_TYPE;
      }


	  
      radio_pkt->oid_pen_index = index_temp;
      radio_pkt->oid_pen_id = machine_id;
      radio_pkt->packet_id = packet_counter;
  
      resp = protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 10 );
//      resp = protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 15 );
      
      if( !data_packet_resp( resp ))
      {	      
        send_timer = Timer_set_timer( TIMER_MS( 10 ));
        while( !Timer_is_timeout( send_timer ));
        resp = PROTOCOL_FAILED;
        
        if( key != (( U16 )KBD_KEY_LONGPRESSED<< 8 |( U16 )KEY_POWER ))
          kbd_process();
      }
      
      //kbd_process();
//      system_power_off();
    }
    while( resp != PROTOCOL_SUCCESS );

#ifdef TX_DEBUG
      SendString((char *)"\r\ndata send finish!");
#endif	
    g_b_oid_data_flag = FALSE;
    g_b_kbd_data_flag = FALSE;
//	/*
//	if(flag_lowpower==0){
		rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
		RedLed_ON();
		BlueLed_ON();
		
//		}
//	*/
  }
}


Bool data_packet_resp( PROTOCOL_STATUS resp )
{
  U8 packet_length;
  if( resp == PROTOCOL_SUCCESS )
  {
    packet_length = radio_receive_packet();
    if(( packet_length == DATA_PACKET_RESP_LEN )
       &&( rx_packet.data_packet.hdr.type == DATA_PACKET_RESP )
         /*&&( rx_packet.data_packet.hdr.flag == 1 )*/)
    {
  //    SendString((U8 *)"\r\ndata resp s1");
    /*  if(( rx_packet.data_packet.app_packet[ 0 ] == ( U8 )machine_id )
         &&( rx_packet.data_packet.app_packet[ 1 ] ==( U8 )( machine_id >> 8 ))
           &&( rx_packet.data_packet.app_packet[ 2 ] ==( U8 )( machine_id >> 16 ))
             &&( rx_packet.data_packet.app_packet[ 3 ] ==( U8 )( machine_id >> 24 )))*/
      {
#ifdef TX_DEBUG
        SendString((char *)"\r\ndata resp s2");
#endif
     //   green_led_on();
        //u32_red_green_led_time = Timer_set_timer( TIMER_MS( 300 ));
		//u32_red_green_flash_time = Timer_set_timer( TIMER_MS( 50 ));
	//	u32_oid_scan_time = Timer_set_timer(TIMER_MS(500));
	//	green_led_start_timer(GREEN_LED_ON_READ_TIME);
        return TRUE;
      }
    }
  }
  return FALSE;
}
#endif  //_wireless_c_
