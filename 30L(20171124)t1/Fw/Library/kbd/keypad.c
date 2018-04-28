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
#pragma CODE = KEYPAD_CODE
#pragma DATA = KEYPAD_DATA

#ifndef _keypad_c_
#define _keypad_c_


//_____  I N C L U D E S ___________________________________________________


#include "compiler.h"
#include "system.h"
#include "keypad.h"
#include "stdio.h"
#include "power.h"
#include "com.h"
//#include "Com_var.h"
#include "adc.h"
//_____ M A C R O S ________________________________________________________

 Bool gb_keypad_evt_disabled;
//_____ D E F I N I T I O N S ______________________________________________

//! Global variable used for the different timers.
U32  _MEM_TYPE_SLOW_  s_timer;
U8   _MEM_TYPE_SLOW_  kbd_state;
U8   _MEM_TYPE_SLOW_  key;
U8   _MEM_TYPE_SLOW_  s_long_press_cpt;
U8  _MEM_TYPE_SLOW_  key_bak;
U8  _MEM_TYPE_SLOW_  double_key;

KEY_SCAN _MEM_TYPE_SLOW_ key_scan;
extern U8 LastKey;
void kbd_key_init(void);
//extern Bool gfSysUSBMP3Mode;

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
//void  kbd_send_key_pressed ( U8 qualifier );


void kbd_key_init(void)
{	
//	U8 i=0;
	key_scan.key_read_point = 0;
    key_scan.key_write_point = 0;
//    for(;i<MAX_KEY_WORD;i++)
//   	{
//   		key_scan.key_word[i].key_type = 0;
//		key_scan.key_word[i].key_num = 0;
//   	}
}
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
   U8 i=0;
   Kbd_stop_timer();
   kbd_init();                               // install driver
   key = NO_KEY;
   key_bak = 0;
   double_key = 0;
   kbd_state = KBD_IDLE;
   for(;i<MAX_KEY_WORD;i++)
   	{
   		key_scan.key_word[i].key_type = 0;
		key_scan.key_word[i].key_num = 0;
   	}
   kbd_key_init();
 //  if()
   Kbd_set_evt_enabled();
   if(Kbd_is_event_triggered())
   	{
   		kbd_state = KBD_LONG_PRESSED;
   	}
}

void kbd_begin_write(U8 st,U8 key_num)
{
	key_scan.key_word[key_scan.key_write_point].key_type = st;
	key_scan.key_word[key_scan.key_write_point].key_num = key_num;
	key_scan.key_write_point++;
}

void kbd_key_write(U8 qualifier)
{
//      KEY_ID(    param)   =  key;
//      KEY_STATUS(param)   =  qualifier;
//      mail_send_event(EVT_KEY_PRESSED, param);
   if( Kbd_are_evt_enabled() )
   	{
		if(key_scan.key_write_point>=MAX_KEY_WORD)
			{
				kbd_key_init();
				return ;
			}
		//key_scan.key_word[key_scan.key_write_point].key_type = qualifier;
		//key_scan.key_word[key_scan.key_write_point].key_num = key;
		//key_scan.key_write_point++;
		kbd_begin_write(qualifier, key);
   	}
/*   
else
   {
      if( (qualifier==KBD_KEY_RELEASED)
      ||  (qualifier==KBD_KEY_REPEAT_RELEASED) )
      {
         Kbd_set_evt_enabled();
      }
   }
*/
}

U16 kbd_key_read(void)
{
//      KEY_ID(    param)   =  key;
//      KEY_STATUS(param)   =  qualifier;
//      mail_send_event(EVT_KEY_PRESSED, param);
	U16 i=0;
	if(key_scan.key_write_point==key_scan.key_read_point)
		{
			kbd_key_init();
			return i;
		}
	MSB(i) = key_scan.key_word[key_scan.key_read_point].key_type;
	LSB(i) = key_scan.key_word[key_scan.key_read_point].key_num;
	key_scan.key_read_point++;
	if(key_scan.key_write_point==key_scan.key_read_point)
		{
			kbd_key_init();
		}
	return i;
}
//! @brief The keypad process is in charge to provide keyboard services
//! to MMI applications by posting events.
//!
//! @param none
//!
//! @return none
//!
void kbd_process(void)
{
	(void)kbd_decode();
   // --- Key state machine polling mode ---
   switch (kbd_state)
   {
      case KBD_IDLE:
         if ( NO_KEY != LastKey )
         {
//         	SendString("\r\n1\r\n");
///			SystemClock(FEQ_48M);
			key = LastKey;
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state = KBD_PRESSED;                     // increment state machine
         }
         break;

      case KBD_PRESSED:
         if (Kbd_is_timer_timeout())         // debounce time-out
         {
         	Kbd_stop_timer();
            if( key != LastKey )
            {
kbd_idle_status:
               kbd_state = KBD_IDLE;  // spurious kbd event
               break;
            }

            key = LastKey;
            //SendString((U8 *)"key=");
            //send_hex_u8(key);
            //SendString((U8 *)"\r\n");
            if (NO_KEY != key)
            {
               kbd_key_write(KBD_KEY_PRESSED);
               Kbd_start_timer(KBD_REPEAT_START_TIME);   
               kbd_state = KBD_START;
               break;
            }
/*         
//        	 SendString("\r\n2\r\n");
            Kbd_stop_timer();
           // key = kbd_decode();              // decode pressed key
            if (LastKey != key)
            {
//               kbd_send_key_pressed(KBD_KEY_PRESSED);
//			   SendString("\r\n1\3\n");
			   kbd_key_write(KBD_KEY_PRESSED);
               Kbd_start_timer(KBD_REPEAT_START_TIME);   
               kbd_state = KBD_START;
            }
            else
            {
//            	SendString("\r\n4\r\n");
               kbd_state = KBD_IDLE;  // spurious kbd event
            }
*/			
			goto kbd_idle_status;
         }
         break;

      case KBD_RELEASED:
      case KBD_REPEAT_RELEASED:
	  case KBD_DOUBLE_RELESED:
         if (Kbd_is_timer_timeout())         // debounce time-out
         {
//            send_hex_u8(key);
//			SendString("\r\n");
//            kbd_send_key_pressed(kbd_state); // qualifier = state (optimisation)
//			SendString("\r\n5\r\n");
			kbd_key_write(kbd_state);
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state = KBD_PRESSED;         // take care of key if any
         }
         break;

	  case KBD_WAIT_DOUBLE:
	  	//SendString("\r\nwait double");
	  	if (Kbd_is_timer_timeout()) {
			//SendString("\r\nwait double");
			kbd_state = KBD_RELEASED;
			break;
	  		}
		if(LastKey == double_key){
			kbd_state = KBD_DOUBLE;
			Kbd_start_timer(KBD_DEBOUNCE_TIME);
			}
	  	break;
	  case KBD_DOUBLE:
	  		//SendString("\r\ndouble");
	  	if (Kbd_is_timer_timeout()) {
			//SendString("\r\ndouble");
			key = double_key;
			kbd_key_write(KBD_KEY_DOUBLE);
			//Kbd_start_timer(KBD_REPEAT_START_TIME);   
			//Kbd_start_timer(KBD_REPEAT_CONT_TIME);
			//kbd_state = KBD_START;
			s_long_press_cpt = KBD_REPEAT_LONG_TIME / KBD_REPEAT_CONT_TIME;
			kbd_state = KBD_REPEAT;
			break;
	  		}
	  	if ( NO_KEY == LastKey )     // no key
         {
           //Kbd_start_timer(KBD_DEBOUNCE_TIME);
            //kbd_state++;                     // increment state machine
           // SendString("\r\nC");
            kbd_state = KBD_IDLE;
         }
	  	break;
      case KBD_START:                        // repeat start period
         if ( NO_KEY == LastKey )     // no key
         {
 //        	SendString("\r\n6\r\n");
 #if 1
            Kbd_start_timer(KBD_DEBOUNCE_TIME);
            kbd_state = KBD_RELEASED;
#else
        	kbd_state = KBD_WAIT_DOUBLE;
			double_key = key;
			Kbd_start_timer(KBD_DOUBLE_WAIT_TIME);
#endif
         }
         if (Kbd_is_timer_timeout())         // end of start period
         {
 //       	 SendString("\r\n7\r\n");
            s_long_press_cpt = KBD_REPEAT_LONG_TIME / KBD_REPEAT_CONT_TIME;
            kbd_state = KBD_REPEAT;
         }
         // no break; here so that KBD_KEY_REPEAT state is entered

      case KBD_REPEAT:
         if (Kbd_is_timer_timeout())
         {
            if (s_long_press_cpt != 0)
            {
 //           	SendString("\r\n8\r\n");
               if (--s_long_press_cpt == 0)
               {
//                  kbd_send_key_pressed(KBD_KEY_LONGPRESSED);
				  kbd_key_write(KBD_KEY_LONGPRESSED);
//				 SendString("\r\n9\r\n");
//				  if(key==KEY_MEN)
//				  	{
						key_bak = key;
				  		kbd_state = KBD_LONG_PRESSED;
//				  	}
                  break;
               }
            }
            if (LastKey == key)
            {
//               kbd_send_key_pressed(KBD_KEY_REPEAT);
//				SendString("\r\na\r\n");
			   //kbd_key_write(KBD_KEY_REPEAT);
               Kbd_start_timer(KBD_REPEAT_CONT_TIME);
            }
            else
            {  // an other key may be pressed
            	//SendString("\r\nb\r\n");
               kbd_state = KBD_REPEAT_RELEASED;
            }
         }
//		 SendString("\r\nc\r\n");
         break;

	   case KBD_LONG_PRESSED:
			{
//					SendString("\r\nd\r\n");
				if (LastKey == NO_KEY)
					{
//			         SendString("\r\ne\r\n");
					 key = key_bak;
//					 kbd_send_key_pressed(KBD_KEY_LONGRELEASED);
					 kbd_key_write(KBD_KEY_LONGRELEASED);
				     kbd_state = KBD_IDLE;
					 key_bak = 0;
					}
			}
	 	break;
	 	
   }

}

#if 0
//! @brief Send a key event to the MMI applications
//!
//! @param  none
//!
//! @return none
//!
void kbd_send_key_pressed( U8 qualifier )
{
   U16 param;

//   SendByte(key);
   
   if( Kbd_are_evt_enabled() )
   {	
//      KEY_ID(    param)   =  key;
//      KEY_STATUS(param)   =  qualifier;
//      mail_send_event(EVT_KEY_PRESSED, param);
	if(key_scan.key_write_point<MAX_KEY_WORD)
		{
			key_scan.key_word
		}
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

#endif
#if 0
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
#endif
#endif   // _keypad_c_
