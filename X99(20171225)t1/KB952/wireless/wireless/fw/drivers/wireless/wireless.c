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
#include "config.h"
#include "compiler.h"
#include "wireless.h"
#include "timer_drv.h"
#include "protocol.h"
#include "lpradio.h"
#include "lpregs.h"
#include "e2radio.h"
#include "lpnonstreaming.h"
#include "oid.h"
#include "keypad.h"
#include "sio\com.h"
#include "lib_mcu\timer\timer_drv.h"
#include "modules\power\power.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "string.h"
//#include "i2c_drv.h"


#pragma CODE = WIRELESS_CODE
#pragma DATA = WIRELESS_DATA

//_____ M A C R O S ________________________________________________________

//#define MOUSE_BUTTON_BIND     ( PIND &( 1 << PD3 ))
#define DATA_RESP_TIME        TIMER_MS( 250 )

//_____ D E F I N I T I O N ________________________________________________
// Packet delivery timeout
#define MOUSE_TX_TIMEOUT_MS          	2000


//--------------------------------------
// API Defines and Types
//--------------------------------------
typedef struct
{
  U32   oid_pen_index;
  U32   oid_pen_id;
  U8    packet_type;
  U8    packet_id;

  U16   i2c_rx_data_ex0;
}TX_PACKET;


extern U32 machine_id;//0x21FC;

MOUSE_STATES mouse_state = MOUSE_STATE_DISCONNECTED;

//_____ D E C L A R A T I O N ______________________________________________

void  mouse_go_disconnected   ( void );
void  mouse_go_active         ( void );
void  mouse_do_bind           ( void );
void  mouse_do_report         ( void );
Bool  data_packet_resp        ( PROTOCOL_STATUS resp );

//-------------------------------------------------------
extern void wireless_index_display(U16 temp_data);
extern void wireless_lcd_bind(U8 temp_flag);
//-------------------------------------------------------

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
  //TX_PACKET *radio_pkt = ( TX_PACKET *)protocol_get_tx_pkt();
  U32 send_timer;
  
  // State action
  if( mouse_state == MOUSE_STATE_DISCONNECTED )
  {
    // Force LP to sleep
//    RadioForceState( END_STATE_SLEEP );
    
    if( g_b_oid_data_flag || g_b_kbd_data_flag/* || g_b_i2c_flag */)
    {
      // Force LP to sleep
      RadioForceState( END_STATE_SLEEP );
        
      // Send a null packet to establish connection
      /*radio_pkt->oid_pen_index = 0;
//      protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 2 );
      
      radio_pkt->oid_pen_id = machine_id;
      radio_pkt->packet_type = NULL_PACKET_TYPE;
      radio_pkt->packet_id = 0;*/
      protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, 1 );
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
    //g_b_i2c_flag = FALSE;
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
  if( g_b_bind_flag )
  {
     SendString((U8 *)"\r\ngo bind 1");
	 
#if( WIRELESS == ENABLE)
    wireless_lcd_bind(0);
#endif
//    g_kbd_key = NO_KEY;
    g_b_bind_flag = FALSE;
    // Perform the button bind procedure
    protocol_button_bind( MOUSE_DEVICE_TYPE );

    // Wait for button up
//    while( MOUSE_BUTTON_BIND == 0 );

    // Debouncing
//    timer = Timer_set_timer( TIMER_MS( 50 ));
//    while( !Timer_is_timeout( timer ));

    // Go to disconnected state to verify connection after bind
    g_b_oid_data_flag = FALSE;
    g_b_kbd_data_flag = FALSE;
    //g_b_i2c_flag = FALSE;
    mouse_state = MOUSE_STATE_DISCONNECTED;
    power_rearm_autopoweroff_delay();
  }
}


//--------------------------------------------------------------------------
// mouse_do_report
//--------------------------------------------------------------------------
#define DATA_PACKET_RESP            0x05
#define DATA_PACKET_RESP_LEN        5


void mouse_do_report( void )
{
  if( g_b_oid_data_flag || g_b_kbd_data_flag/* || g_b_i2c_flag */)
  {
    //U32 index_temp;
    U32 timer;
    U8 length;
    U32 send_timer;
    
    PROTOCOL_STATUS resp = PROTOCOL_FAILED;
    //TX_PACKET *radio_pkt = ( TX_PACKET * )protocol_get_tx_pkt();
    U8 *radio_pkt = ( U8 * )protocol_get_tx_pkt();

   if(g_b_oid_data_flag){
        wireless_index_display(u16_current_index);
    	}
    timer = Timer_set_timer( TIMER_MS( MOUSE_TX_TIMEOUT_MS ));
    // Send packet
   
    packet_counter++;
#if 1
    memset( radio_pkt, 0, 4 );
    radio_pkt[ 9 ] = packet_counter;
    if( g_b_oid_data_flag )
    {
      radio_pkt[ 2 ] = MSB( u16_current_index );
      radio_pkt[ 3 ] = LSB( u16_current_index );
#if( OID_VERSION == OID1 )
      radio_pkt[ 8 ] = INDEX_PACKET_TYPE;
#elif( OID_VERSION == OID2 )
      radio_pkt[ 8 ] = INDEX_PACKET_TYPE | 0x80;
#endif
      radio_pkt[ 4 ] = MSB0( machine_id );
      radio_pkt[ 5 ] = MSB1( machine_id );
      radio_pkt[ 6 ] = MSB2( machine_id );
      radio_pkt[ 7 ] = MSB3( machine_id );

      length = 10;
    }
    else if( g_b_kbd_data_flag )
    {
      radio_pkt[ 2 ] = MSB( g_kbd_key );
      radio_pkt[ 3 ] = LSB( g_kbd_key );
      
      radio_pkt[ 8 ] = KEYBOARD_PACKET_TYPE;
      radio_pkt[ 4 ] = MSB0( machine_id );
      radio_pkt[ 5 ] = MSB1( machine_id );
      radio_pkt[ 6 ] = MSB2( machine_id );
      radio_pkt[ 7 ] = MSB3( machine_id );
      
      length = 10;
    }
    /*else if( g_b_i2c_flag )
    {
      radio_pkt[ 8 ] = I2C_PACKET_TYPE1 + i2c_rf_state;
      memcpy( radio_pkt, i2c_data_point, 8 );
      i2c_data_point += 8;
      radio_pkt += 10;
      memcpy( radio_pkt, i2c_data_point, 5 );
      i2c_data_point += 5;

      length = 15;
    }*/
#endif
    
    do
    {
      if( Timer_is_timeout( timer ))
      {
        mouse_state = MOUSE_STATE_DISCONNECTED;
        break;
      }
#if 0
      if( g_b_oid_data_flag )
      {
        /*index_temp = u32_current_index << 24;
        index_temp |= ( u32_current_index << 8 )& 0x00FF0000;
        index_temp |= ( u32_current_index >> 8 )& 0x0000FF00;
        index_temp |= u32_current_index >> 24;*/
        //index_temp = ( U32 )u16_current_index;
        radio_pkt->oid_pen_index = ( U32 )u16_current_index;
#if( OID_VERSION == OID1 )
        radio_pkt->packet_type = INDEX_PACKET_TYPE;
#elif( OID_VERSION == OID2 )
        radio_pkt->packet_type = INDEX_PACKET_TYPE | 0x80;
#endif
      }
      else if( g_b_kbd_data_flag )
      {
        /*index_temp = ( U32 )g_kbd_key << 24;
        index_temp |= (( U32 )g_kbd_key << 8 )& 0x00FF0000;*/
        //index_temp = ( U32 )g_kbd_key;
        radio_pkt->oid_pen_index = ( U32 )g_kbd_key;
        radio_pkt->packet_type = KEYBOARD_PACKET_TYPE;
      }
      else if( g_b_i2c_flag )
      {
         //index_temp = g_i2c_rx_data;
         radio_pkt->oid_pen_index = g_i2c_rx_data;
         radio_pkt->packet_type = I2C_PACKET_TYPE1;

         radio_pkt->i2c_rx_data_ex0 = g_i2c_rx_data_ex0;
      }
      //radio_pkt->oid_pen_index = index_temp;
      radio_pkt->oid_pen_id = machine_id;
      radio_pkt->packet_id = packet_counter;
      length = 15;
#endif
      resp = protocol_send_packet( MOUSE_DEVICE_TYPE, MOUSE_DEVICE_TYPE, length );
      if( length == 15 )
      {
         if( resp == PROTOCOL_SUCCESS )
         {
           red_led_on();
           green_led_on();
           MSCOUNTER_5 = Timer_set_timer( DATA_RESP_TIME );
           break;
         }
         send_timer = Timer_set_timer( TIMER_MS( 4 ));
         while( !Timer_is_timeout( send_timer ));
         continue;
         //return;
      }
      
      if( !data_packet_resp( resp ))
      {
        send_timer = Timer_set_timer( TIMER_MS( 4 ));
        while( !Timer_is_timeout( send_timer ));
        resp = PROTOCOL_FAILED;
        
        /*if( g_kbd_key != (( U16 )KEY_POWER << 8 |( U16 )KBD_KEY_LONGPRESSED ))
        {
          kbd_process();
        }*/
      }

      //kbd_process();
      //system_power_off();
      /*if( !power_is_vbus_present())
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
      }*/
    }
    while( resp != PROTOCOL_SUCCESS );
    
    g_b_oid_data_flag = FALSE;
    g_b_kbd_data_flag = FALSE;
    //g_b_i2c_flag = FALSE;
    
#if (POWER_OFF_AUTO == ENABLE)
    power_rearm_autopoweroff_delay();
#endif
  }
}


Bool data_packet_resp( PROTOCOL_STATUS resp )
{
  U8 packet_length;
  if( resp == PROTOCOL_SUCCESS )
  {
    packet_length = radio_receive_packet();
    if(( packet_length == DATA_PACKET_RESP_LEN )
       &&( rx_packet.data_packet.hdr.type == DATA_PACKET_RESP ))
    {
      if(( rx_packet.data_packet.app_packet[ 0 ] == MSB0( machine_id ))
         &&( rx_packet.data_packet.app_packet[ 1 ] == MSB1( machine_id ))
           &&( rx_packet.data_packet.app_packet[ 2 ] == MSB2( machine_id ))
             &&( rx_packet.data_packet.app_packet[ 3 ] == MSB3( machine_id )))
      {
        red_led_on();
        green_led_on();
        MSCOUNTER_5 = Timer_set_timer( DATA_RESP_TIME );
        return TRUE;
      }
    }
  }
  return FALSE;
}

#endif  //_wireless_c_
