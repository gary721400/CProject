//!
//! @file lcd_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the Qundau display interface driver routines
//! This on-chip hardware interface is directly connected to the LCD device
//! The files "lcd_controller.c/h" give the upper LCD layer to drive properly the controller of the LCD device
//!
//! @version 1.15 snd3-dvk-1_9_5 $Id: lcd_drv.c,v 1.15 2006/02/03 15:16:28 coger Exp $
//!
//! @todo
//! @bug
//!

#ifndef _lcd_drv_c_
#define _lcd_drv_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "lcd_drv.h"
#include "conf\conf_lcd.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "sio\com.h"

#if( LCD_PRESENT == ENABLED )

#pragma DATA = LCD_DRV_DATA
#pragma CODE = LCD_DRV_CODE

//_____ D E F I N I T I O N S ______________________________________________


#pragma CODE = LCD_SSD1303_TEMP_CODE00

//! lcd_drv_basic_init_interface
//!
//! To init the display interface basically
//! 
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
void lcd_drv_basic_init_interface( void )
{
#if 1
   _bitSET( _PCEN_b0 + 2 );
   _bitSET( _PCEN_b0 + 5 );
   _bitSET( _PCEN_b0 + 6 );
   _bitSET( _PCEN_b0 + 7 );
#else
   PCEN |= 0xE4;
#endif   
   PDEN = 0xFF;

   P80TMS = 0x1363;

   _bitSET( _SOCR_b0 + 1 );
   // Enable display interface
   //Lcd_drv_enable_interface();

   _bitSET( _MODCON_b0 + 12 );
   
}
#endif  //#if( LCD_PRESENT == ENABLED )

#endif   // _lcd_drv_c_
