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
#include "keypad_drv.h"
#include "adc.h"


//_____ D E F I N I T I O N S ______________________________________________

#define KBD_DEBOUNCE_TIME         50    //!< debounce time (ms) depends on key type
#define KBD_REPEAT_START_TIME     400   //!< first repeat time (ms)
#define KBD_REPEAT_CONT_TIME      100   //!< time between repeat event (ms)
#define KBD_REPEAT_LONG_TIME      1000  //!< time of long press event (ms)
#define KBD_DOUBLE_WAIT_TIME      200   //!< time between repeat event (ms)

//#define Kbd_is_event_triggered()    (kbd_decode()!=NO_KEY)
//#define Kbd_is_event_triggered() (( P1 & 0x0F )!= 0x0F || (P2&&BIT7) != BIT7)

//#define Kbd_is_event_triggered() (( P1 & 0x0F )!= 0x0F || (P2&BIT7) != BIT7)
//#define Kbd_is_event_triggered() (PWD_Key_Press() || 0xFFFFu != Get_ADCValue(2))
#define Kbd_is_event_triggered() ( NO_KEY != kbd_decode() )

//!  Key qualifier values !power of 2
#define KBD_KEY_PRESSED          0x01  // kbd pressed decoded
#define KBD_KEY_REPEAT           0x02  // repeat time reached
#define KBD_KEY_LONGPRESSED      0x04  // long time reached
#define KBD_KEY_RELEASED         0x08  // release in start time
#define KBD_KEY_REPEAT_RELEASED  0x10  // release in repeat time
#define KBD_KEY_LONGRELEASED     0X20  // release in long time
#define KBD_KEY_DOUBLE			 0X40  // double time


#define STATE_KEY_PRESSED          0x0100  // kbd pressed decoded
#define STATE_KEY_REPEAT           0x0200  // repeat time reached
#define STATE_KEY_LONGPRESSED      0x0400  // long time reached
#define STATE_KEY_RELEASED         0x0800  // release in start time
#define STATE_KEY_REPEAT_RELEASED  0x1000  // release in repeat time
#define STATE_KEY_LONGRELEASED     0X2000  // release in long time
#define STATE_KEY_DOUBLE		   0X4000  // pressed doubled
//! kbd_process FSM states
#define KBD_IDLE                  0x00
#define KBD_PRESSED               KBD_IDLE + 1   //1
#define KBD_START                 KBD_RELEASED - 1          //07
#define KBD_RELEASED              KBD_KEY_RELEASED          // 08   !FSM optimisation
#define KBD_REPEAT                KBD_REPEAT_RELEASED - 1   // 0F
#define KBD_REPEAT_RELEASED       KBD_KEY_REPEAT_RELEASED   // 10  !FSM optimisation
#define KBD_WAIT_DOUBLE			  KBD_KEY_REPEAT_RELEASED + 1	// 11 
#define KBD_DOUBLE			  	  KBD_KEY_REPEAT_RELEASED + 2	// 12 
#define KBD_DOUBLE_RELESED		  KBD_KEY_REPEAT_RELEASED + 3	// 13


#define KBD_LONG_PRESSED          KBD_KEY_LONGRELEASED
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

#define MAX_KEY_WORD            16
 
typedef struct
{
	U8 key_type;
	U8 key_num;
	
}KEY_WORD;


typedef struct
{
	U8 key_write_point;
	U8 key_read_point;
    KEY_WORD key_word[MAX_KEY_WORD];
}KEY_SCAN;
//_____ D E F I N I T I O N S ______________________________________________
extern Bool gb_keypad_evt_disabled;
extern KEY_SCAN _MEM_TYPE_SLOW_ key_scan;
extern Bool gb_keypad_evt_disabled;
//_____ D E F I N I T I O N S ______________________________________________

//! Global variable used for the different timers.
extern U32  _MEM_TYPE_SLOW_  s_timer;

extern U8   _MEM_TYPE_SLOW_  kbd_state;
extern U8   _MEM_TYPE_SLOW_  key;
extern U8   _MEM_TYPE_SLOW_  s_long_press_cpt;

extern KEY_SCAN _MEM_TYPE_SLOW_ key_scan;

//_____ D E C L A R A T I O N S ____________________________________________

   void     kbd_init_process( void );
   void     kbd_process(      void );
//   void     kbd_disable_event( Bool force );
   void     kbd_key_write    (U8 qualifier);
   U16      kbd_key_read	 (void);
   void 	kbd_begin_write(U8 st,U8 key_num);

#endif   // _KEYPAD_H_ 
