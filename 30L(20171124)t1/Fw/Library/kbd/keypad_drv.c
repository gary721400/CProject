//! @file keypad_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the keypad routines
//!
//! The keypad drivers in charge to detect if a key was pressed
//!
//! Those drivers will use differents generic macro that allow using
//! keypad connected to a bus or directly to the C51 keypad 
//! controler.
//!
//! @version 1.5 snd3-dvk-1_9_5 $Id: keypad_drv.c,v 1.5 2007/04/03 10:52:32 sguyon Exp $
//!
//! @todo
//! @bug
//!

#pragma CODE = KEYPAD_DRV_CODE
#pragma DATA = KEYPAD_DRV_DATA

#ifndef _keypad_drv_c_
#define _keypad_drv_c_

//_____  I N C L U D E S ___________________________________________________


#include "compiler.h"                          // lib configuration header
#include "keypad_drv.h"                      // Keypad definition
#include "com.h"
#include "Power.h"
//#include "intrins.h"
#include "adc.h"
#include "system.h"
//_____ M A C R O S ________________________________________________________

/*
extern unsigned int adc0_value;
extern unsigned int adc2_value;
extern unsigned int adc3_value;
extern unsigned int adc7_value;
*/
//#define LEVEL_OFFSET			(0x20)
//#define ALL_OFFSET				(LEVEL_OFFSET+LEVEL_OFFSET)





//------------------------------------------------------------------------------
typedef struct {
	unsigned int min_value;
	unsigned int max_value;
	unsigned int sw;
}tKEY;

//==============================================================================

U8 LastKey;
//_____ D E F I N I T I O N S ______________________________________________
//static const tKEY ADCKeyLevel[] = {{(0x38E),KEY_MEN},{(0x2A5),KEY_MODE},{(0x19D),KEY_VOL_DEC}};
//static const tKEY ADCKeyLevel[] = {{900,NO_KEY},{600,KEY_MODE},{300,KEY_VOL_DEC}};

#if 0
const tKEY ADCKeyLevel[] =
{
//   { (1020-Begin_Offset),   NO_KEY         },
   { (0x387),   KEY_DUIBI	},
   { (0x345),   KEY_POWER	},
   { (0x2F9),   KEY_FUDU	},
   { (0x295),   KEY_GENDU	},
   { (0x22B),   KEY_FANYI	},
   { (0x113),   KEY_PREV	},
   { (0x143),   KEY_NEXT	},
   { (0xce),   	KEY_VOL_DOWN},
   { (0x61),    KEY_VOL_UP	},
   { (Begin_Offset),    KEY_MODE	},
};
#else
const tKEY ADCKeyLevel_1[] =
{
   { (SW1_DATA-Begin_Offset),(SW1_DATA+Begin_Offset), SW1},
   { (SW2_DATA-Begin_Offset),(SW2_DATA+Begin_Offset), SW2},
   { (SW3_DATA-Begin_Offset),(SW3_DATA+Begin_Offset), SW3},
   { 0x0000,(SW4_DATA+Begin_Offset), SW4}
   //{ (SW4_DATA-Begin_Offset),(SW4_DATA+Begin_Offset), SW4}
   //{ (SW5_DATA-Begin_Offset),(SW5_DATA+Begin_Offset), SW5},
   //{ SW6_DATA,   SW6},
   //{ SW7_DATA,   SW7},
};


//#define ALL_KEY_NUMBER					(4)
#define ALL_KEY_NUMBER					(SIZE_OF_ITEM(ADCKeyLevel_1))

#endif
//_____ D E C L A R A T I O N S ____________________________________________

//! @brief Initialise the keypad interface to detect key press
//! Keypad rows are asserted to 0 so that a key press is detected by
//! column inputs KINx
//
void kbd_init( void )
{
 
}

//! @brief keypad decoding function
//! Keypad rows are asserted from row 0 to row 2 and columns are checked to 0
//! indicating a key is pressed at corresponding intersection row/colum
//!
//! @return key: decoded key from the matrix
//!
U8 kbd_decode( void )
{

	//unsigned int LastKeyState;
	unsigned int value;
	int i;
	
	LastKey = NO_KEY;
	/*
	if(PWD_Key_Press())
	{
		LastKey = KEY_POWER;
		//LastKey = KEY_REC;
		goto back;
	}//*/
	//value = Get_ADCValue(2);
	value = adc3_value;
	//if(value >= 0x3E8u)
	//	return NO_KEY;
	for(i=0;i<ALL_KEY_NUMBER;i++)
	{
		//com_value = ADCKeyLevel_1[i].value;
		if((value >= ADCKeyLevel_1[i].min_value)&&(value <= ADCKeyLevel_1[i].max_value))
		{
//			if((value<300)||PWD_Key_Press())
//				return NO_KEY;
			LastKey = ADCKeyLevel_1[i].sw;
			break;
			//goto back;
			//return LastKeyState;
		}
	}
	/*
	for(i=0;i<sizeof(ADCKeyLevel_2)/sizeof(tKEY);i++)
	{
		com_value = ADCKeyLevel_2[i].value;
		if((value >= com_value)&&(value <= (com_value+Level_2_Offset)))
		{
//			if((value<300)||PWD_Key_Press())
//				return NO_KEY;
			LastKey = ADCKeyLevel_2[i].sw;
			//return LastKeyState;
		}
	}
	*/
	/*
	if(PWD_Key_Press())
	{
		if(LastKey == KEY_VOL_DOWN){
			LastKey = KEY_LOCK;
			}
		else{
			LastKey = KEY_POWER;
			}
		
		//goto back;
		
	}	//*/
back:
    return LastKey;
}
#endif
