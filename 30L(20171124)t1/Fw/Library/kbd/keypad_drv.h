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


//_____ I N C L U D E S ____________________________________________________

////#include "lib_mcu\kbd\kbd_drv.h"             // Keyboard driver definition
#include "power.h"

//_____ M A C R O S ________________________________________________________
/*
typedef enum{
	NO_KEY = 0,
	SW1,
	SW2,
	KBD_PWR
}tKEY_SW;
*/
/*
typedef struct {
	unsigned int value;
	unsigned int sw;
}tKEY;
//*/
//----- Hardware Definition -----



//#define KBD_MODE			KEY1
//#define KBD_INC_V			KEY2
//#define KBD_DEC_V			KEY3
//#define KBD_PWR				KEY4
/*
#define NO_KEY          0x00
#define KEY_VOL_DEC		0x01
#define KEY_MODE		0x02
#define KEY_POWER       0x03
#define KEY_MEN			0x04
#define KEY_VOL_INC		0x05
*/



#define  Begin_2_Offset	(0x0016)
#define  Level_2_Offset	(Begin_2_Offset+Begin_2_Offset)



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

#define NO_KEY          SW0
#define KEY_MODE		SW2
#define KEY_POWER		SW1
#define KEY_VOL_UP		SW3
#define KEY_VOL_DOWN    SW4

#define KEY_LOCK		SW7	

#define KEY_MEN			SW6
#define KEY_REC			SW2

/*
#define KEY_FANYI       SW5
#define KEY_NEXT	    SW6
#define KEY_PREV        SW7
#define KEY_FUDU	    SW8
#define KEY_GENDU       SW9
#define KEY_DUIBI	    SW10
*/

#define KEY_VOL_DEC		KEY_VOL_DOWN
#define KEY_VOL_INC		KEY_VOL_UP

//#define KEY_VOL			0x06
//_____ D E C L A R A T I O N S ____________________________________________



//_____ D E C L A R A T I O N S ____________________________________________

void  kbd_init    ( void );
U8  kbd_decode  ( void );
void  adc_get	  ( void );

#endif  // _DRV_KEYPAD_H_
