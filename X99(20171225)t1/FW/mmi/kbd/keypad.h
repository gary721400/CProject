//! @file keypad.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the high-level keypad driver definitions
//!
//! @version 1.11 snd3-dvk-1_9_5 $Id: keypad.h,v 1.11 2007/04/19 14:33:40 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _KEYPAD_H_
#define _KEYPAD_H_

//_____ I N C L U D E S ____________________________________________________
#include "lib_mcu\kbd\keypad_drv.h"
#include "KBDM9_DDK.H"

#undef _GLOBEXT_
#if (defined _keypad_c_)
#  define _GLOBEXT_ extern
#elif (!defined _GLOBEXT_)
#  define _GLOBEXT_ extern
#endif

//_____ D E F I N I T I O N S ______________________________________________

#define KBD_DEBOUNCE_TIME         50    //!< debounce time (ms) depends on key type
#define KBD_REPEAT_START_TIME     400   //!< first repeat time (ms)
#define KBD_REPEAT_CONT_TIME      100   //!< time between repeat event (ms)
#define KBD_REPEAT_LONG_TIME      1000  //!< time of long press event (ms)

//#define Kbd_is_event_triggered()    (NO_KEY != kbd_decode())
#define Kbd_is_event_triggered()    ( NO_KEY != LastKeyState )
//#define Kbd_is_event_triggered() //(( P1 & 0x1F )!= 0x1F )

//!  Key qualifier values !power of 2
#define KBD_KEY_PRESSED          0x01  // kbd pressed decoded
#define KBD_KEY_REPEAT           0x02  // repeat time reached
#define KBD_KEY_LONGPRESSED      0x04  // long time reached
#define KBD_KEY_RELEASED         0x08  // release in start time
#define KBD_KEY_REPEAT_RELEASED  0x10  // release in repeat time

//! kbd_process FSM states
#define KBD_IDLE                  0x00					//0				
#define KBD_PRESSED               KBD_IDLE + 1          //1
#define KBD_START                 KBD_RELEASED - 1      //7
#define KBD_RELEASED              KBD_KEY_RELEASED      //8     // !FSM optimisation
#define KBD_REPEAT                KBD_REPEAT_RELEASED - 1  //15
#define KBD_REPEAT_RELEASED       KBD_KEY_REPEAT_RELEASED //16  // !FSM optimisation
#define KBD_LONGPRESS             KBD_KEY_REPEAT_RELEASED+1 //17  // !FSM optimisation
//_____ M A C R O S ________________________________________________________

#define Kbd_start_timer( value ) ( s_timer = Timer_set_timer( value / TICK_PERIOD ))
#define Kbd_stop_timer()         ( s_timer = 0 )
#define Kbd_is_timer_timeout()   ( s_timer && Timer_is_timeout( s_timer ))
#define Kbd_is_timer_stopped()   ( s_timer == 0 )
#define Kbd_set_evt_enabled()    (gb_keypad_evt_disabled = FALSE)
#define Kbd_set_evt_disabled()   (gb_keypad_evt_disabled = TRUE)
#define Kbd_are_evt_enabled()    (gb_keypad_evt_disabled == FALSE)

#define KEY_ID(    param)        ( MSB(param) )
#define KEY_STATUS(param)        ( LSB(param) )


ImageDeclareType(KEYPAD_DRV_V1_CODE);
ImageDeclareType(KEYPAD_DRV_V2_CODE);
//_____ D E F I N I T I O N S ______________________________________________
_GLOBEXT_ Bool gb_keypad_evt_disabled;

_GLOBEXT_ U8   _MEM_TYPE_MEDSLOW_  key;

_GLOBEXT_ U16  _MEM_TYPE_MEDSLOW_  g_kbd_key;
_GLOBEXT_ U8   _MEM_TYPE_MEDSLOW_  packet_counter;
_GLOBEXT_ Bool                     g_b_oid_data_flag;
_GLOBEXT_ Bool                     g_b_kbd_data_flag;
_GLOBEXT_ Bool                     g_b_bind_flag;

//_____ D E C L A R A T I O N S ____________________________________________

_GLOBEXT_   void     kbd_init_process( void );
_GLOBEXT_   void     kbd_process(      void );
_GLOBEXT_   void     kbd_disable_event( Bool force );

#endif   // _KEYPAD_H_ 
