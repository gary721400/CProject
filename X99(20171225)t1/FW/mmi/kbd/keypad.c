//! @file keypad.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the high-level keypad driver routines
//!
//! The high-level keypad driver is in charge to:
//! - install the keypad process
//! - post keypad events to the MMI applications
//!
//! @version 1.12 snd3-dvk-1_9_5 $Id: keypad.c,v 1.12 2007/04/19 14:32:28 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _keypad_c_
#define _keypad_c_


//_____  I N C L U D E S ___________________________________________________

#include "lib_mcu\compiler.h"
#include "lib_mcu\timer\timer_drv.h"
#include "keypad.h"
#include "stdio.h"
#include "modules\power\power.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail.h"
#include "sio\com.h"
#include "mmi\shared\com_var.h"
#include "drivers\oid\oid.h"


#pragma DATA = KEYPAD_DATA
#pragma CODE = KEYPAD_CODE
//_____ M A C R O S ________________________________________________________

      Bool                    gb_keypad_evt_disabled;

//_____ D E F I N I T I O N S ______________________________________________

//! Global variable used for the different timers.
/*static*/ U32  _MEM_TYPE_MEDSLOW_  s_timer;

/*static*/ U8   _MEM_TYPE_MEDSLOW_  kbd_state;
/*static */U8   _MEM_TYPE_MEDSLOW_  key;
/*static*/ U8   _MEM_TYPE_MEDSLOW_  s_long_press_cpt;

/*static*/ U8   _MEM_TYPE_MEDSLOW_  key_temp /*= NO_KEY*/;

U16   _MEM_TYPE_MEDSLOW_   g_kbd_key;
U8    _MEM_TYPE_MEDSLOW_   packet_counter = 0;
Bool                       g_b_oid_data_flag = FALSE;
Bool                       g_b_kbd_data_flag = FALSE;
Bool                       g_b_bind_flag = FALSE;

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________



void  kbd_send_key_pressed ( U8 qualifier );


#pragma CODE = TASK_INIT_CODE

//! @brief Initialisation of the keypad process
//!
//! Release the Global used for the state of that task and for the key
//!
//! @param none
//!
//! @return none
//!
void kbd_init_process(void)
{
   Kbd_stop_timer();
   kbd_init();                               // install driver
   key_temp = key = NO_KEY;
   kbd_state = KBD_IDLE;
   Kbd_set_evt_disabled();
}


//#pragma CODE = KEYPAD_CODE
#pragma CODE = KEYPAD_PROCESS_CODE

//! @brief The keypad process is in charge to provide keyboard services
//! to MMI applications by posting events.
//!
//! @param none
//!
//! @return none
//!
void kbd_process(void)
{
 //  ( void )kbd_decode();
 if(!g_b_V1Hard_Flag){
		SDKLoadCode(KEYPAD_DRV_V2_CODE);
	 	( void )kbd_V2_decode();
	  	}
	  	else{
			SDKLoadCode(KEYPAD_DRV_V1_CODE);
			( void )kbd_V1_decode();
			}
			/*
 	 if(!g_b_V1Hard_Flag){
 	     
	 	( void )kbd_V2_decode();
	  	}
	  	else{
			( void )kbd_V1_decode();
			}*/
   // --- Key state machine polling mode ---
   switch (kbd_state)
   {
      case KBD_IDLE:
         //Kbd_set_evt_enabled();
         //if (Kbd_is_event_triggered())
         if( NO_KEY != LastKeyState )
         {
            key_temp = LastKeyState;
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state++;                     // increment state machine
         }
         break;

      case KBD_PRESSED:
         if (Kbd_is_timer_timeout())         // debounce time-out
         {
            Kbd_stop_timer();
            //key = kbd_decode();              // decode pressed key

            //if( key_temp != key )
            if( key_temp != LastKeyState )
            {
kbd_idle_status:
               Kbd_set_evt_enabled();
               kbd_state = KBD_IDLE;  // spurious kbd event
               break;
            }

            key = LastKeyState;
            //SendString((U8 *)"key=");
            //send_hex_u8(key);
            //SendString((U8 *)"\r\n");
            if (NO_KEY != key)
            {
               kbd_send_key_pressed(KBD_KEY_PRESSED);
               Kbd_start_timer(KBD_REPEAT_START_TIME);   
               kbd_state = KBD_START;
               break;
            }
            /*else
            {
               Kbd_set_evt_enabled();
               kbd_state = KBD_IDLE;  // spurious kbd event
            }*/
            goto kbd_idle_status;
         }
         break;

      case KBD_RELEASED:
      case KBD_REPEAT_RELEASED:
         if (Kbd_is_timer_timeout())         // debounce time-out
         {
            kbd_send_key_pressed(kbd_state); // qualifier = state (optimisation)
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state = KBD_PRESSED;         // take care of key if any
         }
         break;

      case KBD_START:                        // repeat start period
         //if ( !Kbd_is_event_triggered())]
         if( NO_KEY == LastKeyState )
         {
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state++;                     // increment state machine
            //SendString((U8 *)"key release\r\n");
         }
         if (Kbd_is_timer_timeout())         // end of start period
         {
            s_long_press_cpt = KBD_REPEAT_LONG_TIME / KBD_REPEAT_CONT_TIME;
            kbd_state = KBD_REPEAT;
         }
         // no break; here so that KBD_KEY_REPEAT state is entered

      case KBD_REPEAT:
         if (Kbd_is_timer_timeout())
         {
            if (s_long_press_cpt != 0)
            {
               if (--s_long_press_cpt == 0)
               {
                  kbd_send_key_pressed(KBD_KEY_LONGPRESSED);
                  break;
               }
            }
            //if (kbd_decode() == key)
            if( LastKeyState == key )
            {
               kbd_send_key_pressed(KBD_KEY_REPEAT);
               Kbd_start_timer(KBD_REPEAT_CONT_TIME);
            }
            else
            {  // an other key may be pressed
               kbd_state = KBD_REPEAT_RELEASED;
            }
         }
         break;
   }
}


//! @brief Send a key event to the MMI applications
//!
//! @param  none
//!
//! @return none
//!
void kbd_send_key_pressed( U8 qualifier )
{
   U16 param;

   if( Kbd_are_evt_enabled() )
   {	
      KEY_ID(    param)   =  key;
      KEY_STATUS(param)   =  qualifier;
      b_readySysUP = TRUE;
      mail_send_event(EVT_KEY_PRESSED, param);
   }
   else
   {
      if( (qualifier==KBD_KEY_RELEASED)
      ||  (qualifier==KBD_KEY_REPEAT_RELEASED) )
      {
         Kbd_set_evt_enabled();
      }
   }
}


#pragma CODE = KEYPAD_CODE

//! @brief Disable key events if state machine is not idle
//!
//! @param  Bool force or not the disable of the keypad even if in state IDLE
//!
//! @return none
//!
void kbd_disable_event( Bool force )
{
   if( (KBD_IDLE!=kbd_state)
   ||  (force==TRUE) )
   {
      Kbd_set_evt_disabled();
   }
}
#endif   // _keypad_c_
