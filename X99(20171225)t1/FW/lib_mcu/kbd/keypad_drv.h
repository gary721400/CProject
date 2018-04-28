//! @file keypad_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains definition for the keypad drivers
//!
//! @version 1.8 snd3-dvk-1_9_5 $Id: keypad_drv.h,v 1.8 2007/04/03 10:52:32 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _DRV_KEYPAD_H_
#define _DRV_KEYPAD_H_

#include "modules\power\power.h"             // Power module definition


#undef _GLOBEXT_
#if (defined _keypad_drv_c_)
#  define _GLOBEXT_
#elif (!defined _GLOBEXT_)
#  define _GLOBEXT_ extern
#endif
//_____ I N C L U D E S ____________________________________________________

//#include "lib_mcu\kbd\kbd_drv.h"             // Keyboard driver definition


//_____ M A C R O S ________________________________________________________



//----- Hardware Definition -----
/*#define P_KBD           P1       // keypad port
#define KEY1            P1_0
#define KEY2            P1_1
#define KEY3            P1_2
#define KEY4            P1_3*/
#if 0
#define KEY5            ( _PBID_b0 + 15 )

#define KBD_PWR_DIR     ( _PBDIR_b0 + 15 )
#define KBD_PWR_PH      ( _PBPH_b0 + 15 )
#define KBD_PWR         KEY5
#endif

#define SW0      	0x00
#define SW1      	0x01
#define SW2      	0x02
#define SW3      	0x03
#define SW4    		0x04
#define SW5       	0x05
#define SW6	    	0x06
#define SW7        	0x07
#define SW8	    	0x08
#define SW9       	0x09
#define SW10	   	0x0A
#define SW11		0x0B
#define SW12		0x0C
#define SW13		0x0D
#define SW14		0x0E
#define SW15		0x0F

#define NO_KEY          SW0


#define KEY_VOL_UP      SW1
#define KEY_VOL_DOWN    SW4

#define KEY_NEXT	    SW7
#define KEY_PREV        SW6
#define KEY_POWER       SW3

#define KEY_MODE        SW2
#define KEY_REC       	SW5
#define KEY_BAK         SW8




#define KEY_FANYI       SW9
#define KEY_FUDU	    SW10
#define KEY_GENDU       SW11
#define KEY_DUIBI	    SW12

/*
#define NO_KEY          0x00
#define KEY_VOL_UP      0x01
#define KEY_MODE        0x02
#define KEY_POWER       0x03
#define KEY_VOL_DOWN    0x04
#define KEY_FANYI       0x05
#define KEY_NEXT	    0x06
#define KEY_PREV        0x07
#define KEY_FUDU	    0x08
#define KEY_GENDU       0x09
#define KEY_DUIBI	    0x0A
*/

//_____ D E C L A R A T I O N S ____________________________________________



//_____ D E C L A R A T I O N S ____________________________________________

void  kbd_init    ( void );
Byte  kbd_decode  ( void );
Byte  kbd_V1_decode  ( void );
Byte  kbd_V2_decode  ( void );


void  adc_get     (void);

#endif  // _DRV_KEYPAD_H_
