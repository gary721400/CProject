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
#ifndef _keypad_drv_c_
#define _keypad_drv_c_

//_____  I N C L U D E S ___________________________________________________

#include "lib_mcu\compiler.h"                          // lib configuration header
#include "keypad_drv.h"                      // Keypad definition

#include "sio\com.h"
//#include <intrins.h>
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "adc.h"
#include "lib_mcu\timer\timer_drv.h"
#include "lib_mcu\int\int_drv.h"                // Interrupt definitions
#include "drivers\oid\oid.h"
#include "variable.h"

#pragma DATA = KEYPAD_DRV_DATA
#pragma CODE = KEYPAD_DRV_CODE

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________
/*static*/ U32  _MEM_TYPE_MEDSLOW_  kbd_s_timer;

//static U8 LastKeyState = NO_KEY;
/*static*/ U8 adc_chal/* = 3*/;
static U8 adc_key_flag = 0;

//#define  Begin_Offset	18


typedef struct
{
   U16   min_val;
   U16   max_val;
   U8    sw;
}tKEY;
#if 0
#define  Begin_Offset	0x02
const tKEY ADCKeyLevel[] =
{
//   { (1020-Begin_Offset),   NO_KEY         },
   { (0x9A-Begin_Offset),   KEY_DUIBI	},
   { (0x8E-Begin_Offset),   KEY_POWER	},
   { (0x81-Begin_Offset),   KEY_FANYI	},
   { (0x70-Begin_Offset),   KEY_GENDU	},
   { (0x5E-Begin_Offset),   KEY_FUDU		},
   { (0x2E-Begin_Offset),   KEY_NEXT		},
   { (0x36-Begin_Offset),   KEY_PREV		},
   { (0x23-Begin_Offset),   KEY_VOL_DOWN	},
   { (0x10 -Begin_Offset),   KEY_VOL_UP	},
   { (Begin_Offset -Begin_Offset),   KEY_MODE		},
};
#else

#endif



//#define  NUM_KEY		(sizeof( ADCKeyLevel ) / sizeof( tKEY ))
#define  NUM_KEY		(8)
#define  TWO_KEY		(2)


#define  NO_KEY_ADC2    0x01
#define  NO_KEY_ADC3    0x02

//U16   Get_ADC_Value  ( U16 channel );
//_____ D E C L A R A T I O N S ____________________________________________

#pragma CODE = TASK_INIT_CODE

//! @brief Initialise the keypad interface to detect key press
//! Keypad rows are asserted to 0 so that a key press is detected by
//! column inputs KINx
//
void kbd_init( void )
{
   //P1DIR &= ~(BIT0 | BIT1 | BIT2 | BIT4 );
   //P2DIR &= ~( BIT4 | BIT5 | BIT6 );
   ADC_init();
   kbd_s_timer = Timer_set_timer( TIMER_MS( 10 ));
   adc_key_flag = 0;
   adc_chal = 3;
}


#pragma CODE = KEYPAD_DRV_CODE
//==================================================
void adc_get(void)
{
   U16 value;
   if( !(Timer_is_timeout( kbd_s_timer )))
   {
      return;
   }
   //send_hex_u8(kb);
   kbd_s_timer = Timer_set_timer( TIMER_MS( 15 ));
   value = Get_ADCValue(( U16 )adc_chal );

  if( adc_chal == 0 ){              //batt low test
   		battery_value = (U16)((value*100UL)/adc7_value);
		//battery_value = value;
	    adc_chal =2;
	    return;
   	}

   if( adc_chal == 2 ){            //usb link
   	  adc_chal = 3;
	  adc2_value = value;
   	  return;
   	}

   if( adc_chal == 3 ){            //key scan
   	  adc_chal = 7;
	  adc3_value = value;
	  //adc3_value = (U16)((value*100UL)/adc7_value);
   	  return;
   	}
    if( adc_chal == 7 ){            //v biaozhun
   	  adc_chal = 0;
	  adc7_value = value;
   	  return;
   	}
   adc_chal = 0;
}


#pragma CODE = KEYPAD_DRV_V1_CODE

const tKEY V1ADCKeyLevel[] =
{
//   { (1020-Begin_Offset),   NO_KEY         },
#if 1
   { SW1_MIN,SW1_MAX,SW1},
   { SW2_MIN,SW2_MAX,SW2},
   { SW3_V1_MIN,SW3_V1_MAX,SW3},
   { SW4_V1_MIN,SW4_V1_MAX,SW4},
   { SW5_MIN,SW5_MAX,SW5},
   { SW6_MIN,SW6_MAX,SW6},
   { SW7_MIN,SW7_MAX,SW7},
   { SW8_MIN,SW8_MAX,SW8},
#else
   { (0x387-Begin_Offset),   KEY_DUIBI	},
   { (0x345-Begin_Offset),   KEY_POWER	},
   { (0x2F9-Begin_Offset),   KEY_FUDU	},
   { (0x295-Begin_Offset),   KEY_GENDU	},
   { (0x22B-Begin_Offset),   KEY_FANYI	},
   { (0x113-Begin_Offset),   KEY_PREV	},
   { (0x143-Begin_Offset),   KEY_NEXT	},
   { (0xce-0x12),   		 KEY_VOL_DOWN	},
   { (0x61-Begin_Offset),    KEY_VOL_UP	},
   { (Begin_Offset-Begin_Offset),   KEY_MODE		},
#endif   
};

//! @brief keypad decoding function
//! Keypad rows are asserted from row 0 to row 2 and columns are checked to 0
//! indicating a key is pressed at corresponding intersection row/colum
//!
//! @return key: decoded key from the matrix
//!
Byte kbd_V1_decode( void )
{
#if 0
   unsigned int value;
   unsigned char i;
   if( !( /*kbd_s_timer && */Timer_is_timeout( kbd_s_timer )))
   {
      return LastKeyState;
   }
   kbd_s_timer = Timer_set_timer( TIMER_MS( 10 ));
   LastKeyState = NO_KEY;
   value = Get_ADCValue(( U16 )adc_chal );

   for( i = 0; i < sizeof( ADCKeyLevel ) / sizeof( tKEY ); i++ )
   {
      if(( value >= ADCKeyLevel[ i ].value )
         &&( value <=( ADCKeyLevel[ i ].value + Level_Offset )))
      {
         //return ADCKeyLevel[ i ].sw;
          LastKeyState = ADCKeyLevel[ i ].sw;
          return LastKeyState;
         
  		 
      }
   }
   
   return LastKeyState;
//   return NO_KEY;
#else
   
   U8 i;
   U16 value,minV,maxV;
   	
   /*if( value >= 900 )
   {
      //return NO_KEY;
      goto adc2_no_key;
   }*/
   value = adc3_value;
   //for( i = 0; i < sizeof( ADCKeyLevel ) / sizeof( tKEY ); i++ )
   for( i = 0; i < NUM_KEY ; i++ )
   {
   	  minV = V1ADCKeyLevel[ i ].min_val;
	  maxV = V1ADCKeyLevel[ i ].max_val;
	  /*
	  if(!g_b_V1Hard_Flag){
	  	if(i==2){
			minV -= Power_Offset;
			maxV -= Power_Offset;
	  		}
	  	}*/
	  
      if(( value >= minV)
         &&( value <= maxV))
      {
         //if( !i )
         //   break;
//         adc_key_flag &= ~NO_KEY_ADC2;
         LastKeyState = V1ADCKeyLevel[ i ].sw;
         return LastKeyState;
      }
   }
   LastKeyState = NO_KEY;
   return LastKeyState;
#endif
}

#pragma CODE = KEYPAD_DRV_V2_CODE

const tKEY V2ADCKeyLevel[] =
{
//   { (1020-Begin_Offset),   NO_KEY         },
#if 1
   { SW1_MIN,SW1_MAX,SW1},  //0x209,0x229,vol up
   { SW2_MIN,SW2_MAX,SW2},  //0,0x16        
   { SW3_V2_MIN,SW3_V2_MAX,SW3}, //0x2F7,0x2e4 power
   { SW4_V2_MIN,SW4_V2_MAX,SW4}, //34A,36A
   { SW5_MIN,SW5_MAX,SW5},  //c1,E1 
   { SW6_MIN,SW6_MAX,SW6},  //284   ,2A4  //prev
   { SW7_MIN,SW7_MAX,SW7},  //378, 398  next
   { SW8_MIN,SW8_MAX,SW8},  //102   ,122
#else
   { (0x387-Begin_Offset),   KEY_DUIBI	},
   { (0x345-Begin_Offset),   KEY_POWER	},
   { (0x2F9-Begin_Offset),   KEY_FUDU	},
   { (0x295-Begin_Offset),   KEY_GENDU	},
   { (0x22B-Begin_Offset),   KEY_FANYI	},
   { (0x113-Begin_Offset),   KEY_PREV	},
   { (0x143-Begin_Offset),   KEY_NEXT	},
   { (0xce-0x12),   		 KEY_VOL_DOWN	},
   { (0x61-Begin_Offset),    KEY_VOL_UP	},
   { (Begin_Offset-Begin_Offset),   KEY_MODE		},
#endif   
};

//! @brief keypad decoding function
//! Keypad rows are asserted from row 0 to row 2 and columns are checked to 0
//! indicating a key is pressed at corresponding intersection row/colum
//!
//! @return key: decoded key from the matrix
//!
Byte kbd_V2_decode( void )
{
#if 0
   unsigned int value;
   unsigned char i;
   if( !( /*kbd_s_timer && */Timer_is_timeout( kbd_s_timer )))
   {
      return LastKeyState;
   }
   kbd_s_timer = Timer_set_timer( TIMER_MS( 10 ));
   LastKeyState = NO_KEY;
   value = Get_ADCValue(( U16 )adc_chal );

   for( i = 0; i < sizeof( ADCKeyLevel ) / sizeof( tKEY ); i++ )
   {
      if(( value >= ADCKeyLevel[ i ].value )
         &&( value <=( ADCKeyLevel[ i ].value + Level_Offset )))
      {
         //return ADCKeyLevel[ i ].sw;
          LastKeyState = ADCKeyLevel[ i ].sw;
          return LastKeyState;
         
  		 
      }
   }
   
   return LastKeyState;
//   return NO_KEY;
#else
   
   U8 i;
   U16 value,minV,maxV;
   	
   /*if( value >= 900 )
   {
      //return NO_KEY;
      goto adc2_no_key;
   }*/
   value = adc3_value;
   //for( i = 0; i < sizeof( ADCKeyLevel ) / sizeof( tKEY ); i++ )
   for( i = 0; i < NUM_KEY ; i++ )
   {
   	  minV = V2ADCKeyLevel[ i ].min_val;
	  maxV = V2ADCKeyLevel[ i ].max_val;
	  /*
	  if(!g_b_V1Hard_Flag){
	  	if(i==2){
			minV -= Power_Offset;
			maxV -= Power_Offset;
	  		}
	  	}*/
	  
      if(( value >= minV)
         &&( value <= maxV))
      {
         //if( !i )
         //   break;
//         adc_key_flag &= ~NO_KEY_ADC2;
         LastKeyState = V2ADCKeyLevel[ i ].sw;
         return LastKeyState;
      }
   }
   LastKeyState = NO_KEY;
   return LastKeyState;
#endif
}


#if 0
U16 Get_ADC_Value( U16 channel )
{
   U8 i;
   channel <<= 3;
   channel |= 0x8000;
   ADCCON = channel;

   //for( i = 0; i < 10; i++ )
   {
      _bitSET( _ADCCON_b0 + 0 );
      while( _bit_get( _ADCCON_b0 + 0 ));
   }

   for( i = 0; i < 5; i++ );
   return ( ADCD >> 6 );
}
#endif
#endif
