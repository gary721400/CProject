//!
//! @file lcd_SSD1303.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file gives definitions of the SSD1303 lcd controller driver functions used in MMI applications.
//!		   Code only implemented if "LCD_CONTROLLER_PART_NUMBER" has been defined with the label "LCD_CONTROLLER_SSD1303".
//!        This definition is to do in the file "conf_lcd.h".
//!		   Note that the SSD1303 is a monochrom lcd with horizontal pages
//!
//! @version 1.25 snd3b-dvk-1_9_3 $Id: lcd_SSD1303.c,v 1.25 2007/07/26 15:01:03 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _lcd_SSD1303_c_
#define _lcd_SSD1303_c_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_lcd.h"
#if( LCD_PRESENT == ENABLED )
#if ( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303 )
//#include "conf\conf_mmi.h"
//#include "conf\conf_oid.h"
//#include "drivers\mmi\img_handler.h"
#include "lib_mcu\lcd\lcd_drv.h"
//#include "lib_system\unicode\unicode.h"
//#include "lib_system\auto_disp\auto_disp.h"
//#include "modules\control_access\ctrl_access.h"
//#include "mmi\shared\com_var.h"
//#include "mmi\kbd\keypad.h"
#include "lcd_SSD1303.h"

#include "sio\com.h"
//#include "lib_mcu\sio\sio_drv.h"
//#include "drivers\oid\oid.h"
//#include "modules\mmi_manager\mmgr_kernel.h"
//#include "mmi\images\tab_pict.h"


#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fs.h"

#pragma DATA = LCD_SSD1303_DATA
#pragma CODE = LCD_SSD1303_CODE


ImageDeclareType( LCD_SSD1303_TEMP_CODE01 );

//ImageDeclareType( UNICODE_TEMP_CODE03 );

//ImageDeclareType( IMG_HANDLER_TEMP_CODE00 );


//! Global x and y variables for display
U8  _MEM_TYPE_SLOW_   lcd_g_lcd_x = 0;
U8  _MEM_TYPE_SLOW_   lcd_g_lcd_y = 0;

//! Global one-bit variable for the merging or not of superimposed displays
Bool b_merging = FALSE;
Bool b_flush_lcd;
//! Set in "lcd_puts()" if effective text scrolling done
//! and only read by MMI via the macro "Lcd_have_scrolling_been_effective()"
Bool b_scrolling_mode_effective;

//! Variable used in "lcd_puts()" to define the viewport
_MEM_TYPE_SLOW_ lcd_viewport lcd_g_viewport = {LCD_WIDTH - 1};

//! Pointer used in "lcd_puts()" for the text scrolling
st_txt_scrolling _MEM_TYPE_SLOW_*ptr_txt_scrolling_prm = NULL;

//! Timer to delay the backlight switching off
//! Only declared if backlight authorized
#if (BACKLIGHT == ENABLE)
   U32 _MEM_TYPE_SLOW_ g_timer_backlight = 0x00;

//! Allow dynamic disable of backlight
   U8 _MEM_TYPE_SLOW_ g_backlight_en = TRUE;
#endif

#if (BACKLIGHT == ENABLE)
_MEM_TYPE_SLOW_ U8 g_mmi_lcd_backlight_off = FALSE;
#endif

U8 timer_display_array[ 14 ];
Bool  lcd_g_inverse /*= FALSE*/;
const U8 display_char[ 4 ] = {0X00,0X0F,0XF0,0XFF};
      U8 display_ready_char[ 4 ];
	  U8  display_char_width;
#if 0
const U8 ascii16_uncode[  ][ 8 ] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   //' '
   { 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'!'
   { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00 },   //'"'
   { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00, 0x00 },   //'#'
   { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00, 0x00 },   //'$'
   { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00, 0x00 },   //'%'
   { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00, 0x00 },   //'&'
   { 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'''
   { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'('
   { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00 },   //')'
   { 0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, 0x00, 0x00 },   //'*'
   { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00 },   //'+'
   { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00 },   //','
   { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00 },   //'-'
   { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'.'
   { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00 },   //'/'
   
   { 0x70, 0x88, 0x98, 0xA8, 0xC8, 0x88, 0x70, 0x00 },   //'0'
   { 0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00 },   //'1'
   { 0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8, 0x00 },   //'2'
   { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00 },   //'3'
   { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00 },   //'4'
   { 0xF8, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70, 0x00 },   //'5'
   { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00 },   //'6'
   { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00 },   //'7'
   { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00 },   //'8'
   { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00 },   //'9'
   { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00 },   //':'
   { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00 },   //';'
   { 0x08, 0x14, 0x22, 0x40, 0x00, 0x00, 0x00, 0x00 },   //'<'
   { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00 },   //'='
   { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00 },   //'>'
   { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, 0x00 },   //'?'
   
   { 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, 0x00, 0x00 },   //'@'
   { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00 },   //'A'
   { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00 },   //'B'
   { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00 },   //'C'
   { 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00 },   //'D'
   { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00 },   //'E'
   { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00 },   //'F'
   { 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00, 0x00 },   //'G'
   { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, 0x00 },   //'H'
   { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'I'
   { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00 },   //'J'
   { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00 },   //'K'
   { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00 },   //'L'
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, 0x00 },   //'M'
   { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00, 0x00 },   //'N'
   { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00 },   //'O'
   
   { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00 },   //'P'
   { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00 },   //'Q'
   { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00 },   //'R'
   { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00 },   //'S'
   { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00 },   //'T'
   { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00 },   //'U'
   { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00 },   //'V'
   { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00, 0x00 },   //'W'
   { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00 },   //'X'
   { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00, 0x00, 0x00 },   //'Y'
   { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00 },   //'Z'
   { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'['
   { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00 },   //'\'
   { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00 },   //']'
   { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00 },   //'^'
   { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00 },   //'_'
   
   { 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'`'
   { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00 },   //'a'
   { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00 },   //'b'
   { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00 },   //'c'
   { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00, 0x00 },   //'d'
   { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00 },   //'e'
   { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, 0x00, 0x00 },   //'f'
   { 0x06, 0x49, 0x49, 0x49, 0x3F, 0x00, 0x00, 0x00 },   //'g'
   { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00 },   //'h'
   { 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, 0x00 },   //'i'
   { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, 0x00 },   //'j'
   { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00 },   //'k'
   { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00 },   //'l'
   { 0x7C, 0x04, 0x18, 0x04, 0x7C, 0x00, 0x00, 0x00 },   //'m'
   { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00 },   //'n'
   { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00 },   //'o'
   
   { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00 },   //'p'
   { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00, 0x00, 0x00 },   //'q'
   { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00 },   //'r'
   { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, 0x00 },   //'s'
   { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, 0x00, 0x00 },   //'t'
   { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, 0x00 },   //'u'
   { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, 0x00 },   //'v'
   { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00 },   //'w'
   { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00 },   //'x'
   { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, 0x00 },   //'y'
   { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00 },   //'z'
   { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'{'
   { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'|'
   { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00 },   //'}'
   { 0x02, 0x01, 0x02, 0x04, 0x02, 0x00, 0x00, 0x00 },   //'~'
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }    //DEL
};
#else
const U8 ascii8_uncode[  ][ 8 ] =
{
	{0X0,0X0,0X0,0X0,0X0,0X0,0X0,0X0},
	{0X20,0X20,0X20,0X20,0X0,0X0,0X20,0X0},
	{0X50,0X50,0X50,0X0,0X0,0X0,0X0,0X0},
	{0X50,0X50,0XF8,0X50,0XF8,0X50,0X50,0X0},
	{0X20,0X78,0XA0,0X70,0X28,0XF0,0X20,0X0},
	{0XC0,0XC8,0X10,0X20,0X40,0X98,0X18,0X0},
	{0X60,0X90,0XA0,0X40,0XA8,0X90,0X68,0X0},
	{0X60,0X20,0X40,0X0,0X0,0X0,0X0,0X0},
	{0X10,0X20,0X40,0X40,0X40,0X20,0X10,0X0},
	{0X40,0X20,0X10,0X10,0X10,0X20,0X40,0X0},
	{0X0,0X20,0XA8,0X70,0XA8,0X20,0X0,0X0},
	{0X0,0X20,0X20,0XF8,0X20,0X20,0X0,0X0},
	{0X0,0X0,0X0,0X0,0X60,0X20,0X40,0X0},
	{0X0,0X0,0X0,0XF8,0X0,0X0,0X0,0X0},
	{0X0,0X0,0X0,0X0,0X0,0X60,0X60,0X0},
	{0X0,0X8,0X10,0X20,0X40,0X80,0X0,0X0},
	{0X70,0X88,0X98,0XA8,0XC8,0X88,0X70,0X0},
	{0X20,0X60,0X20,0X20,0X20,0X20,0X70,0X0},
	{0X70,0X88,0X8,0X10,0X20,0X40,0XF8,0X0},
	{0XF8,0X10,0X20,0X10,0X8,0X88,0X70,0X0},
	{0X10,0X30,0X50,0X90,0XF8,0X10,0X10,0X0},
	{0XF8,0X80,0XF0,0X8,0X8,0X88,0X70,0X0},
	{0X30,0X40,0X80,0XF0,0X88,0X88,0X70,0X0},
	{0XF8,0X8,0X10,0X20,0X40,0X40,0X40,0X0},
	{0X70,0X88,0X88,0X70,0X88,0X88,0X70,0X0},
	{0X70,0X88,0X88,0X78,0X8,0X10,0X60,0X0},
	{0X0,0X60,0X60,0X0,0X60,0X60,0X0,0X0},
	{0X0,0X60,0X60,0X0,0X60,0X20,0X40,0X0},
	{0X10,0X20,0X40,0X80,0X40,0X20,0X10,0X0},
	{0X0,0X0,0XF8,0X0,0XF8,0X0,0X0,0X0},
	{0X40,0X20,0X10,0X8,0X10,0X20,0X40,0X0},
	{0X70,0X88,0X8,0X10,0X20,0X0,0X20,0X0},
	{0X70,0X88,0X8,0X68,0XA8,0XA8,0X70,0X0},
	{0X70,0X88,0X88,0X88,0XF8,0X88,0X88,0X0},
	{0XF0,0X88,0X88,0XF0,0X88,0X88,0XF0,0X0},
	{0X70,0X88,0X80,0X80,0X80,0X88,0X70,0X0},
	{0XE0,0X90,0X88,0X88,0X88,0X90,0XE0,0X0},
	{0XF8,0X80,0X80,0XF0,0X80,0X80,0XF8,0X0},
	{0XF8,0X80,0X80,0XF0,0X80,0X80,0X80,0X0},
	{0X70,0X88,0X80,0XB8,0X88,0X88,0X78,0X0},
	{0X88,0X88,0X88,0XF8,0X88,0X88,0X88,0X0},
	{0X70,0X20,0X20,0X20,0X20,0X20,0X70,0X0},
	{0X38,0X10,0X10,0X10,0X10,0X90,0X60,0X0},
	{0X88,0X90,0XA0,0XC0,0XA0,0X90,0X88,0X0},
	{0X80,0X80,0X80,0X80,0X80,0X80,0XF8,0X0},
	{0X88,0XD8,0XA8,0XA8,0X88,0X88,0X88,0X0},
	{0X88,0X88,0XC8,0XA8,0X98,0X88,0X88,0X0},
	{0X70,0X88,0X88,0X88,0X88,0X88,0X70,0X0},
	{0XF0,0X88,0X88,0XF0,0X80,0X80,0X80,0X0},
	{0X70,0X88,0X88,0X88,0XA8,0X90,0X68,0X0},
	{0XF0,0X88,0X88,0XF0,0XA0,0X90,0X88,0X0},
	{0X78,0X80,0X80,0X70,0X8,0X8,0XF0,0X0},
	{0XF8,0X20,0X20,0X20,0X20,0X20,0X20,0X0},
	{0X88,0X88,0X88,0X88,0X88,0X88,0X70,0X0},
	{0X88,0X88,0X88,0X88,0X88,0X50,0X20,0X0},
	{0X88,0X88,0X88,0XA8,0XA8,0XA8,0X50,0X0},
	{0X88,0X88,0X50,0X20,0X50,0X88,0X88,0X0},
	{0X88,0X88,0X88,0X50,0X20,0X20,0X20,0X0},
	{0XF8,0X8,0X10,0X20,0X40,0X80,0XF8,0X0},
	{0X70,0X40,0X40,0X40,0X40,0X40,0X70,0X0},
	{0X0,0X80,0X40,0X20,0X10,0X8,0X0,0X0},
	{0X70,0X10,0X10,0X10,0X10,0X10,0X70,0X0},
	{0X20,0X50,0X88,0X0,0X0,0X0,0X0,0X0},
	{0X0,0X0,0X0,0X0,0X0,0X0,0XF8,0X0},
	{0X80,0X40,0X20,0X0,0X0,0X0,0X0,0X0},
	{0X0,0X0,0X70,0X8,0X78,0X88,0X78,0X0},
	{0X80,0X80,0XB0,0XC8,0X88,0X88,0XF0,0X0},
	{0X0,0X0,0X70,0X80,0X80,0X88,0X70,0X0},
	{0X8,0X8,0X68,0X98,0X88,0X88,0X78,0X0},
	{0X0,0X0,0X70,0X88,0XF8,0X80,0X70,0X0},
	{0X30,0X48,0X40,0XE0,0X40,0X40,0X40,0X0},
	{0X78,0X88,0X88,0X78,0X8,0X8,0X70,0X0},
	{0X80,0X80,0XB0,0XC8,0X88,0X88,0X88,0X0},
	{0X20,0X0,0X60,0X20,0X20,0X20,0X70,0X0},
	{0X10,0X0,0X30,0X10,0X10,0X90,0X60,0X0},
	{0X80,0X80,0X90,0XA0,0XC0,0XA0,0X90,0X0},
	{0X60,0X20,0X20,0X20,0X20,0X20,0X70,0X0},
	{0X0,0X0,0XD8,0XA8,0XA8,0X88,0X88,0X0},
	{0X0,0X0,0XB0,0XC8,0X88,0X88,0X88,0X0},
	{0X0,0X0,0X70,0X88,0X88,0X88,0X70,0X0},
	{0X0,0X0,0XF0,0X88,0XF0,0X80,0X80,0X0},
	{0X0,0X0,0X68,0X98,0X78,0X8,0X8,0X0},
	{0X0,0X0,0XB0,0XC8,0X80,0X80,0X80,0X0},
	{0X0,0X0,0X70,0X80,0X70,0X8,0XF0,0X0},
	{0X40,0X40,0XE0,0X40,0X40,0X48,0X30,0X0},
	{0X0,0X0,0X88,0X88,0X88,0X98,0X68,0X0},
	{0X0,0X0,0X88,0X88,0X88,0X50,0X20,0X0},
	{0X0,0X0,0X88,0X88,0XA8,0XA8,0X50,0X0},
	{0X0,0X0,0X88,0X50,0X20,0X50,0X88,0X0},
	{0X0,0X0,0X88,0X88,0X78,0X8,0X70,0X0},
	{0X0,0X0,0XF8,0X10,0X20,0X40,0XF8,0X0},
	{0X10,0X20,0X20,0X40,0X20,0X20,0X10,0X0},
	{0X20,0X20,0X20,0X20,0X20,0X20,0X20,0X0},
	{0X40,0X20,0X20,0X10,0X20,0X20,0X40,0X0},
	{0X40,0XA8,0X10,0X0,0X0,0X0,0X0,0X0},
	{0X0,0X0,0X0,0X0,0X0,0X0,0X0,0X0}
/*
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   //' '
   { 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x20, 0x00 },   //'!'
   { 0x50, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00 },   //'"'
   { 0x50, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x50, 0x00},   //'#'
   { 0x20, 0x78, 0xA0, 0x70, 0x28, 0xF0, 0x20, 0x00},   //'$'
   { 0xC0, 0xC8, 0x10, 0x20, 0x40, 0x98, 0x18, 0x00 },   //'%'
   { 0xC0, 0xC8, 0x10, 0x20, 0x40, 0x98, 0x18, 0x00 },   //'&'
   { 0x60, 0x90, 0xA0, 0x40, 0xA8, 0x90, 0x68, 0x00 },   //'''
   { 0x60, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'('
   { 0x10, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x00 },   //')'
   { 0x40, 0x20, 0x10, 0x10, 0x10, 0x20, 0x40, 0x00 },   //'*'
   { 0x00, 0x20, 0xA8, 0x70, 0xA8, 0x20, 0x00, 0x00 },   //'+'
   { 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0x00 },   //','
   { 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x40, 0x00 },   //'-'
   { 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00 },   //'.'
   { 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00 },   //'/'
   { 0x70, 0x88, 0x98, 0xA8, 0xC8, 0x88, 0x70, 0x00 },   //'0'
   { 0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00 },   //'1'
   { 0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8, 0x00 },   //'2'
   { 0xF8, 0x10, 0x20, 0x10, 0x08, 0x88, 0x70, 0x00 },   //'3'
   { 0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10, 0x00 },   //'4'
   { 0xF8, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70, 0x00 },   //'5'
   { 0x30, 0x40, 0x80, 0xF0, 0x88, 0x88, 0x70, 0x00 },   //'6'
   { 0xF8, 0x08, 0x10, 0x20, 0x40, 0x40, 0x40, 0x00 },   //'7'
   { 0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00 },   //'8'
   { 0x70, 0x88, 0x88, 0x78, 0x08, 0x10, 0x60, 0x00 },   //'9'
   { 0x00, 0x60, 0x60, 0x00, 0x60, 0x60, 0x00, 0x00 },   //':'
   { 0x00, 0x60, 0x60, 0x00, 0x60, 0x20, 0x40, 0x00 },   //';'
   { 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x00 },   //'<'
   { 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00 },   //'='
   { 0x40, 0x20, 0x10, 0x08, 0x10, 0x20, 0x40, 0x00 },   //'>'
   { 0x70, 0x88, 0x08, 0x10, 0x20, 0x00, 0x20, 0x00 },   //'?'
   { 0x70, 0x88, 0x08, 0x68, 0xA8, 0xA8, 0x70, 0x00 },   //'@'
   { 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x00 },   //'A'
   { 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x00 },   //'B' 
   { 0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70, 0x00 },   //'C'
   { 0xE0, 0x90, 0x88, 0x88, 0x88, 0x90, 0xE0, 0x00 },   //'D'
   { 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00 },   //'E'
   { 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x00 },   //'F'
   { 0x70, 0x88, 0x80, 0xB8, 0x88, 0x88, 0x78, 0x00 },   //'G'
   { 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00 },   //'H'
   { 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00 },   //'I'



   
   { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00 },   //'J'
   { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00 },   //'K'
   { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00 },   //'L'
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, 0x00 },   //'M'
   { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00, 0x00 },   //'N'
   { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00 },   //'O'
   { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00 },   //'P'
   { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00 },   //'Q'
   { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00 },   //'R'
   { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00 },   //'S'
   { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00 },   //'T'
   { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00 },   //'U'
   { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00 },   //'V'
   { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00, 0x00 },   //'W'
   { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00 },   //'X'
   { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00, 0x00, 0x00 },   //'Y'
   { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00 },   //'Z'
   { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'['
   { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00 },   //'\'
   { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00 },   //']'
   { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00 },   //'^'
   { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00 },   //'_'
   
   { 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'`'
   { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00 },   //'a'
   { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00 },   //'b'
   { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00 },   //'c'
   { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00, 0x00 },   //'d'
   { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00 },   //'e'
   { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, 0x00, 0x00 },   //'f'
   { 0x06, 0x49, 0x49, 0x49, 0x3F, 0x00, 0x00, 0x00 },   //'g'
   { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00 },   //'h'
   { 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, 0x00 },   //'i'
   { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, 0x00 },   //'j'
   { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00 },   //'k'
   { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00 },   //'l'
   { 0x7C, 0x04, 0x18, 0x04, 0x7C, 0x00, 0x00, 0x00 },   //'m'
   { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00 },   //'n'
   { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00 },   //'o'
   
   { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00 },   //'p'
   { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00, 0x00, 0x00 },   //'q'
   { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00 },   //'r'
   { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, 0x00 },   //'s'
   { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, 0x00, 0x00 },   //'t'
   { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, 0x00 },   //'u'
   { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, 0x00 },   //'v'
   { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00 },   //'w'
   { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00 },   //'x'
   { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, 0x00 },   //'y'
   { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00 },   //'z'
   { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00 },   //'{'
   { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00 },   //'|'
   { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00 },   //'}'
   { 0x02, 0x01, 0x02, 0x04, 0x02, 0x00, 0x00, 0x00 },   //'~'
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }    //DEL
*/   
};
#endif

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
static _MEM_TYPE_SLOW_ U8 lcd_g_page          = 0;
static _MEM_TYPE_SLOW_ U8 lcd_g_shift_in_page = 0;
static _MEM_TYPE_SLOW_ U8 lcd_g_line          = 0;
static Bool b_merging_old;

#if( LCD_SCREEN_BUFFER == ENABLED )
static U8 _MEM_TYPE_SLOW_   s_screen_buffer[NB_LCD_PAGES*NB_LCD_LINES];
static U8 _MEM_TYPE_SLOW_*  s_ptr_screen_buffer;
static U8 _MEM_TYPE_SLOW_   s_display_data_read;
#endif

static U8 const PosBin[8]   = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

// Table MaskBin
//  used on right display boundary : mask to keep the left part only
//  input    : the MSB position of the display data used
//  output   : the mask giving the consecutive bits NOT used by the display data
//  examples : bit7 is MSB, so bit0 -> 7 used to code display data, so no bit free, return 0
//           : bit4 is MSB, so bit0 -> 4 used to code display data, so bit5 -> 7 are free, return 0xE0
static U8 const MaskBin[8]  = {0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00};

// Table MaskBin2
//  used on left display boundary : mask to keep the right part only
//  input    : the MSB position of the display data used
//  output   : the mask giving the consecutive bits used by the display data
//  examples : bit7 is MSB, so bit0 -> 6 used to code display data, so bit7 free, return 0x01
//           : bit4 is MSB, so bit0 -> 3 used to code display data, so bit4 -> 7 are free, return 0x0F
//static U8 const MaskBin2[8] = {0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01};


U8 _MEM_TYPE_SLOW_ up_last_percent = 0;

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
U8 lcd_get_nb_pages(        U8 nb_pixels );
U8 lcd_get_start_page_mask( U8 nb_pixels );
U8 lcd_get_end_page_mask(   U8 nb_pixels );
U8 lcd_get_end_page_ws(     U8 nb_pixels );
U8 lcd_get_end_page(        U8 nb_pixels );
U8 lcd_get_nb_lines(        U8 nb_pixels );

#if (BACKLIGHT == ENABLE)
Bool  g_b_lcd_backlight_off = FALSE;
#endif

//add by xuanyu
void  lcd_puts_simple      ( U8* string );

//void  update_hbar             ( U8 x0, U8 y0 );
//void  update_percent_bar      ( U8 u8_percent );
//void  update_purcent_bar_init ( void );
//U8    read_lcd_display_data   ( void );


//_____ D E F I N I T I O N S ______________________________________________

#if 1

void  Lcd_goto_x( U8 x0 )
{
   /* Store the new X-coordinate in the dedicated global variable */
   lcd_g_lcd_x = (x0);

   /* Write the new LCD current line to the controller */
//   Lcd_set_line( lcd_g_lcd_x );
}


void Lcd_goto_y( U8 y0 )
{
   /* Store the new Y-coordinate in the dedicated global variable */
   lcd_g_lcd_y = (y0);

   /* Write the new LCD current page to the controller */
   //Lcd_set_page( lcd_g_lcd_y /8 );
//   Lcd_set_page( lcd_g_lcd_y >> 3 );

   /* Get the shifting to apply to the display data */
   //lcd_g_shift_in_page = PosBin[ lcd_g_lcd_y %8 ];
//   lcd_g_shift_in_page = PosBin[ lcd_g_lcd_y & 0x07 ];
}
#endif


//! Write a data in the LCD controller display ram
void Lcd_write_display_data( U8 val )
{
   U8 i;
   SetStorageBusy();
   b_flush_lcd = TRUE;
   Lcd_drv_enable_interface();
   /* Waiting display interface available to lauch a writing */
   Lcd_drv_test_lcbusy();

   /* Transmit display data */
   /*Lcd_drv_write_data(val);*/
   Lcd_drv_write_data(( lcd_g_inverse )?( ~( val )):( val ));
   Lcd_drv_test_lcbusy();
   Lcd_drv_disable_interface();
   ClearStorageBusy();
   b_flush_lcd = FALSE;
}


//! Write a command to the LCD controller
void Lcd_write_command( U8 cmd )
{
   SetStorageBusy();
   b_flush_lcd = TRUE;
   Lcd_drv_enable_interface();
   
   /* Waiting display interface available to lauch a writing */
   Lcd_drv_test_lcbusy();

   /* Select instruction register */
   Lcd_drv_select_instr_reg();

   /* Transmit command value */
   Lcd_drv_write_cmd(cmd);

   /* Data register access by default */
   /* ! MUST BE ALWAYS IMPLEMENTED HERE AS LAST INSTRUCTION ! */
   Lcd_drv_select_data_reg();

   Lcd_drv_test_lcbusy();
   
   Lcd_drv_disable_interface();
   ClearStorageBusy();
   b_flush_lcd = FALSE;
}
#if 0
//! Write a display data in the LCD controller or in the screen buffer
#if( LCD_SCREEN_BUFFER == DISABLED )
//! Write a data in the LCD controller display ram
//extern   Bool  lcd_g_inverse;
#  define Lcd_write_display_data(val)                                \
   {                                                                 \
      /* Waiting display interface available to lauch a writing */   \
      Lcd_drv_test_lcbusy();                                         \
                                                                     \
      /* Transmit display data */                                    \
      /*Lcd_drv_write_data(val);*/                                   \
      Lcd_drv_write_data(( lcd_g_inverse )?( ~( val )):( val ));     \
   }
#else
   //! Write a data in the screen buffer
#  define Lcd_write_display_data(val)                                \
   {                                                                 \
      *(s_ptr_screen_buffer++) = (val);                              \
   }
#endif

//! Read a display data from the LCD controller or the screen buffer
#if( LCD_SCREEN_BUFFER == DISABLED )
   //! The read from the LCD controller display ram is done in two steps :
   //!   1) A read request is to sent to the controller via the macro
   //!      "Lcd_read_display_data()"
   //!   2) The read value is got with the help of a second macro
   //!      "display_data_read"
   //! These two macros avoids the call of a unique function that would be slower in execution.
#  define Lcd_read_display_data()                                       \
{                                                                       \
   /* Waiting display interface available to lauch a writing */         \
   Lcd_drv_test_lcbusy();                                               \
                                                                        \
   /* Set the read command */                                           \
   Lcd_drv_set_read_command();                                          \
                                                                        \
   /* Waiting display interface available to get a correct reading  */  \
   Lcd_drv_test_lcbusy();                                               \
                                                                        \
   /* Get the display data by adressing Lcd_drv_display_data_read() */  \
   /* just after the implementation of this macro                   */  \
}
#  define display_data_read             0//Lcd_drv_read_data()
#else
   //! The read from the screen buffer is also done in the two steps 
   //! defined above in order to keep a code compatibility
   //!   1) "Lcd_read_display_data()" is an empty macro
   //!   2) The read value is got with the help of a second macro
   //!      "display_data_read"
   //! These two macros avoids the call of a unique function that would be slower in execution.
#  define Lcd_read_display_data()
#  define display_data_read             *(s_ptr_screen_buffer)
#endif
#endif

//! lcd_write_command
//!
//! To write a command to the LCD controller.
//! 
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 cmd : the command value to write
//!
//! @return none
//!
void lcd_write_command( U8 cmd )
{
   Lcd_write_command( cmd );
}
//#define  lcd_write_command( cmd )   Lcd_write_command( cmd )

//! lcd_get_nb_pages
//!
//! To get the number of 8-bit lcd pages from the picture height or width in pixels
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd pages
//!
//! @return U8: number of lcd pages
//!
/*
U8 lcd_get_nb_pages( U8 nb_pixels )
{
	//return( (nb_pixels -1) / 8 + 1 );
	return(((nb_pixels -1)>> 3 )+ 1 );
}

*/
#pragma CODE = LCD_SSD1303_TEMP_CODE01

//! lcd_get_nb_lines
//!
//! To get the number of lines that could be displayed
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd lines
//!
//! @return U8: number of lcd lines
//!
/*
U8 lcd_get_nb_lines( U8 nb_pixels )
{
	if( Lcd_get_line() + nb_pixels > NB_LCD_LINES )
		return( NB_LCD_LINES - Lcd_get_line() );
	else
		return( nb_pixels ); 
}
*/

#pragma CODE = LCD_SSD1303_CODE

//! lcd_get_end_page_ws (ws as Without Shifting)
//!
//! To get the index of the end page from the start page and the number of pixels 
//! along lcd pages without shifting-in-page
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd pages
//!
//! @return U8: number of lcd pages
//!
/*
U8 lcd_get_end_page_ws( U8 nb_pixels )
{
	U8 end_page;   

	end_page = Lcd_get_page() + lcd_get_nb_pages( nb_pixels ) -1;

	if (end_page >= NB_LCD_PAGES )
	 	return( NB_LCD_PAGES - 1 );
	else
		return( end_page );
}
*/
//! lcd_get_end_page 
//!
//! To get the index of the end page from the start page and the number of pixels 
//! along lcd pages with shifting-in-page
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd pages
//!
//! @return U8: number of lcd pages
//!
/*
U8 lcd_get_end_page( U8 nb_pixels )
{
	U8 end_page;

   	//end_page = ( Lcd_get_y() + nb_pixels -1 ) / 8;
   	end_page = ( Lcd_get_y() + nb_pixels -1 )>> 3;

   	if ( end_page >= NB_LCD_PAGES )
		return( NB_LCD_PAGES - 1 );
	else
		return( end_page );
}
*/
//! lcd_get_start_page_mask
//!
//! To get the mask for the start page in which begins the display of a picture
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd pages
//!
//! @return U8: binary mask
//!
/*
U8 lcd_get_start_page_mask( U8 nb_pixels )
{
	if ( Lcd_is_merging() )
		return( 0xFF );
   	else if ( nb_pixels < 8 )
		return( ( Lcd_get_shift_in_page() * (PosBin[nb_pixels] - 1) ) ^ 0xFF );
   	else
		return( Lcd_get_shift_in_page() - 1 );
}
*/
//! lcd_get_end_page_mask
//!
//! To get the mask for the end page in which ends the display of a picture
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 nb_pixels: the number of pixels along lcd pages
//!
//! @return U8: binary mask
//!
/*
U8 lcd_get_end_page_mask( U8 nb_pixels )
{
	if (Lcd_is_merging())
		return( 0xff );
	else
		//return( MaskBin[ (Lcd_get_y() + nb_pixels - 1) % 8 ] );
		return( MaskBin[ (Lcd_get_y() + nb_pixels - 1) & 7 ] );
}
*/

#pragma CODE = LCD_SSD1303_TEMP_CODE01
#if 0
//! lcd_goto_x
//!
//! To set the new X-coordinate of the cursor position
//! This function is dedicated to be called by higher level files than lcd_controller.c/h
//! Call of this routine wastes time but enables to save bytes of program memory compared to the macro
//! Moreover, the function implements a boundary test, not integrated in the macro
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 x0: the new X-coordinate
//!
//! @return none
//!
void lcd_goto_x( U8 x0 )
{
   // Check if the X-coordinate is out of boundary
	if ( x0 >= LCD_WIDTH )
		x0 = 0;

   	Lcd_goto_x( x0 );
}


//! lcd_goto_y
//!
//! To set the new Y-coordinate of the cursor position
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 y0: the new Y-coordinate
//!
//! @return none
//!
void lcd_goto_y( U8 y0 )
{
   // Check if the Y-coordinate is out of boundary
   if ( y0 >= LCD_HEIGHT )
      y0 = 0;

   Lcd_goto_y( y0 );
}
#endif


#pragma CODE = LCD_SSD1303_CODE

//! lcd_goto_xy
//!
//! To set the new coordinates of the cursor position
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 x0: the new X-coordinate
//! @param U8 y0: the new Y-coordinate
//!
//! @return none
//!
void lcd_goto_xy( U8 x0, U8 y0 )
{
   // Check if the X-coordinate is out of boundary
   if ( x0 >= LCD_WIDTH )
      x0 = 0;

   Lcd_goto_x( x0 );

   // Check if the Y-coordinate is out of boundary
   if ( y0 >= LCD_HEIGHT )
      y0 = 0;

   Lcd_goto_y( y0 );
}

void lcd_change_char(U8 data1)
{
	U8 i,m;
#if 1
/*
	if(display_char_width == 2){
		data1 = data1 >> 6;
		i = 1;
		}
	if(display_char_width == 4){
		data1 = data1 >> 4;
		i = 2;
		}
	if(display_char_width == 6){
		data1 = data1 >> 2;
		i = 3;
		}
	if(display_char_width == 8){
		i = 4;
		}

*/	
	m = 8 - display_char_width;
	data1 = data1>>m;
	i = display_char_width/2;
	for(;i > 0;i--){
		display_ready_char[i-1] = display_char[(0x03&data1)];
		data1 = data1>>2;
		}
#else
	m = display_char_width/2-1;
	//m = 0x3<<(display_char_width-2);
    //m = 0x
	for(i=0;i<(display_char_width/2);i++){
		display_ready_char[i] = display_char[((0xC0&data1)>>6)];
		data1 = data1<<2;
		}
#endif	
}
/*
void check_char_width(void)
{
	 display_char_width = 0;
	 if( ih_g_curr_font == IDC_FONT_08 )display_char_width = 6;
	 if( ih_g_curr_font == IDC_FONT_16 )display_char_width = 8;
	 if( ih_g_curr_font == IDC_FONT_24 )display_char_width = 12;
}
*/
void lcd_write_one_width(U8 x0,U8 y0,U8 data_width)
{
  U8 row_number,column_number,i=0;
  lcd_write_command(0x40);
  for(row_number=y0;row_number<(y0+1);row_number++)
   {
	 lcd_write_command(0xB0);
	 lcd_write_command(row_number);
     for(column_number=x0;column_number<(x0+data_width);column_number++)
     {   
	 lcd_write_command(0x10|((column_number/16)));
     lcd_write_command(0x00|((column_number%16))); 
	 Lcd_write_display_data(display_ready_char[i++]);
     }
   }
}



#pragma CODE = LCD_SSD1303_TEMP_CODE02

//! lcd_clear_screen
//!
//! Clear the LCD screen entirely
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void lcd_clear_screen(void)
{
#if 0
#define  Wait_10ms()    timer = Timer_set_timer(TIMER_MS(20)); \
                           while (!Timer_is_timeout(timer));
   U32 timer;
  U8 row_number,column_number;
///*  
  lcd_write_command(0x40);
  for(row_number=0;row_number<64;row_number++)
   {
     lcd_write_command((0xB0));
	 lcd_write_command(row_number);
//     lcd_write_command(0x00);
//     lcd_write_command(0x10);
     for(column_number=0;column_number<128;column_number++)
     {
     	lcd_write_command(0x10 | (0x07 & (( column_number + 0 )>> 4)) );
	    lcd_write_command(0x00 | ( 0x0F &( column_number + 0 )));
    //    lcd_write_command(0x00);
        Lcd_write_display_data(0x00);
//		Wait_10ms();
     }
   }
//*/
  lcd_write_command(0x40);
  for(row_number=0;row_number<64;row_number++)
   {
  //   lcd_write_command(0x10 | ((0x0F&((column_number+0)>> 4))<<1));
	//    lcd_write_command(0x00 | (( 0x0F &( column_number+0))<<1));
//	lcd_write_command(0xB0);
//	 lcd_write_command(row_number); 
//	 lcd_write_command(0x00);
//     lcd_write_command(0x10);
	lcd_write_command(0xB0);
	 lcd_write_command(row_number);
     for(column_number=0;column_number<128;column_number++)
     {   
         	
	 
	 lcd_write_command(0x10|((column_number/16)));
     lcd_write_command(0x00|((column_number%16)));
   //     lcd_write_data(0xff); 
  	 Lcd_write_display_data(0xFF);
		Wait_10ms();

     }
   }
 /* 
  for(column_number=0;column_number<128;column_number++)
   {
     for(row_number=0;row_number<64;row_number++)
     {
        lcd_write_command(0x10 | (0x07 & (( column_number + 0 )>> 4)) );
	    lcd_write_command(0x00 | ( 0x0F &( column_number + 0 )));
     	lcd_write_command((0xB0));
	    lcd_write_command(row_number);
        Lcd_write_display_data(0xF0);
		Wait_10ms();
     }
   }
  //*/
 while(1);

#else
	lcd_clear_part_of_screen( 0, 0, LCD_WIDTH, LCD_HEIGHT);
//	while(1);
#endif
}

//! lcd_clear_part_of_screen
//!
//! Clear the screen partially
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 x0: X-coordinate of the start point
//!         U8 y0: Y-coordinate of the start point
//!         U8 width : area width to clear
//!         U8 height: area height to clear
//! @return none

// Drawing direction
//      (0,0) __________
//           |          |
//           | |    /|  |
//           | |   / |  |
//           | |  /  |  |
//           | | /   |  |
//           | |/   \|/ |
//           |__________|

void lcd_clear_part_of_screen(U8 x0, U8 y0, U8 width, U8 height)
{
#if 1
   U8 row_number,column_number;
  lcd_write_command(0x40);
  for(row_number=y0;row_number<(y0+height);row_number++)
   {
//   lcd_write_command(0x10 | ((0x0F&((column_number+0)>> 4))<<1));
//    lcd_write_command(0x00 | (( 0x0F &( column_number+0))<<1));
//	lcd_write_command(0xB0);
//	 lcd_write_command(row_number); 
//	 lcd_write_command(0x00);
//     lcd_write_command(0x10);
	lcd_write_command(0xB0);
	 lcd_write_command(row_number);
     for(column_number=x0;column_number<(x0+width);column_number++)
     {   
         	
	 
	 lcd_write_command(0x10|((column_number/16)));
     lcd_write_command(0x00|((column_number%16))); 
	 Lcd_write_display_data(0);
     }
   }
#else
   U8 _MEM_TYPE_MEDFAST_ mask;
   U8 _MEM_TYPE_SLOW_    end_page_mask;
   U8 _MEM_TYPE_SLOW_    curr_page;
   U8 _MEM_TYPE_SLOW_    end_page;
   U16 _MEM_TYPE_MEDFAST_ line;


	#define  Wait_10ms()    timer = Timer_set_timer(TIMER_MS(100)); \
                           while (!Timer_is_timeout(timer));
   U32 timer;
   Lcd_save_status_merging();
   Lcd_unset_merging();
   // Unset merging to get proper mask with the functions 
   // "lcd_get_start_page_mask()" and "lcd_get_end_page_mask()"


   // Set the new cursor position
   lcd_goto_xy(x0, y0);


   #define NB_LINES  width
   // "end_page_ws" has got the index of the end page without shifting-in-page
   end_page = lcd_get_end_page( height );

   SendString((U8 *)"\r\nend_page=");
   send_hex_u8(end_page);
   
   #define START_MASK  lcd_get_start_page_mask(  height );
   #define END_MASK    lcd_get_end_page_mask(    height );


   // WARNING : all the functions "lcd_get_start_page_mask()", "lcd_get_end_page_mask()", 
   //           "lcd_get_end_page()" must be called before the loop begins.
   //           Why? Because the cursor coordinates are changed in the loop

   // The different values taken by the variable "mask"
   // if      (curr_page == Lcd_get_page())   mask = START_MASK;
   // else if (curr_page == end_page      )   mask = end_page_mask;
   // else                                    mask = 0; 

   curr_page = Lcd_get_page();
   mask      = START_MASK;
   end_page_mask  = END_MASK;
///*
   SendString((U8 *)"\r\nx0:y0:width:heigh:curr_page:mask:end_page_mask=");
   send_hex_u8(x0);
   SendString((U8 *)":");
   send_hex_u8(y0);
   SendString((U8 *)":");
   send_hex_u16(width);
   SendString((U8 *)":");
   send_hex_u8(height);
   SendString((U8 *)":");
   send_hex_u8(curr_page);
   SendString((U8 *)":");
   send_hex_u8(mask);
   SendString((U8 *)":");
   send_hex_u8(end_page_mask);
//*/   
   for(; curr_page <= end_page; curr_page++ )
//	for(curr_page=y0; curr_page <= height; curr_page++ )
   {
   	  SendString((U8 *)"\r\ncurr_page=");
     send_hex_u8(curr_page);
      Lcd_set_page( curr_page      );
      Lcd_set_line( Lcd_get_line() );
      Lcd_set_cursor();

      if( curr_page == end_page )  
      { mask = end_page_mask; }


      line = NB_LINES; // Init the loop variable

      if( mask != 0 )
      { // curr_page == start_page or curr_page == end_page
         for(; line != 0; line-- )
         {
            //Lcd_dummy_read();
            //Lcd_read_display_data();
            Lcd_set_cursor();
            //Lcd_write_display_data( display_data_read & mask );
            Lcd_write_display_data( display_data_read );
         }
         mask = 0;
      }
      else
      {
//      	 SendString((U8 *)"\r\nline=");
         for(; line != 0; line-- )
         {
         	Wait_10ms();
   			send_hex_u16(line);
			SendString((U8 *)":");
   			
            Lcd_write_display_data( 0 );
         }
      }
   }

   // Restore the status that had the bit b_merging just before the calling of this function
   Lcd_restore_status_merging();

   // Require the display of the new built screen
   Lcd_new_screen_to_display();

   #undef NB_LINES
   #undef START_MASK
   #undef END_MASK
#endif   
}

#if 0
//! @brief lcd_set_dot
//!
//! Display a dot on the LCD screen
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param   U8 x0: X-coordinate of the dot to display
//!          U8 y0: Y-coordinate of the dot to display
//!
//! @return none
//!
void lcd_set_dot(const U8 x0, const U8 y0)
{
   U8 disp_data;

   lcd_goto_xy(x0, y0);
   Lcd_set_cursor();

   // The bit mask to set properly the dot is already elaborated by the call of Lcd_goto_xy(x0, y0);
   Lcd_dummy_read();
   Lcd_read_display_data();
   disp_data = Lcd_get_shift_in_page() | display_data_read;
   Lcd_set_cursor();
   Lcd_write_display_data( disp_data );

   // Require the display of the new built screen
   Lcd_new_screen_to_display();
}
#endif

#if 0//(GAME_SNAKE == ENABLE)
//! @brief lcd_toggle_dot
//!
//! Set/Remove a dot on the LCD screen
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param   U8 x0: X-coordinate of the dot to display
//!          U8 y0: Y-coordinate of the dot to display
//!
//! @return none
//!
void lcd_toggle_dot(const U8 x0, const U8 y0)
{
   U8 disp_data;

   lcd_goto_xy(x0, y0);
   Lcd_set_cursor();

   // The bit mask to set properly the dot is already elaborated by the call of Lcd_goto_xy(x0, y0);
   Lcd_dummy_read();
   Lcd_read_display_data();
   disp_data = Lcd_get_shift_in_page() ^ display_data_read;
   Lcd_write_display_data( disp_data );

   // Require the display of the new built screen
   Lcd_new_screen_to_display();
}
#endif   // #if (GAME_SNAKE == ENABLE)


#pragma CODE = LCD_SSD1303_CODE

#if 0//(EMBEDDED_CODE_FONT == ENABLE)
//! @brief lcd_puts_code
//!
//! Display text on the LCD screen from the code character table
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  const U8* string : string to display
//!
//! @return none

// Drawing direction
//      (0,0) __________
//           |          |
//           | |    /|  |
//           | |   / |  |
//           | |  /  |  |
//           | | /   |  |
//           | |/   \|/ |
//           |__________|

void lcd_puts_code( const U8* string )
{
   Bool b_suppl_page;
   Bool b_unicode_string;
   _MEM_TYPE_SLOW_    U8   char_x_pos;
   _MEM_TYPE_MEDFAST_ U8   start_page;
   _MEM_TYPE_MEDFAST_ U8   nb_lines;
   _MEM_TYPE_SLOW_    U8   end_page_ws;
   _MEM_TYPE_SLOW_    U8   height;
   UnicodeBuffer* p_string;
   _MEM_TYPE_SLOW_    U8   start_page_mask;


   // The 4 following unions "v*" declares variables which are not used at the same time,
   // but in separate code sections. 
   // Thus this enables to save bytes and to get explicit name for these variables

   union
   {
      U8 string_length;
      U8 end_page_mask;
   }_MEM_TYPE_SLOW_ v1;

   union
   {
      U8 curr_page;
      U8 char_width;
   }_MEM_TYPE_SLOW_ v2;

   union
   {
      U8 end_page;
      U8 prev_disp_data;
   }_MEM_TYPE_SLOW_ v3;

   union
   {
      U16 wc;
      U16 char_disp_data;
   }v4;


   // No string pointed, so exit out of this function
   if( NULL == string )
      { return; }


   if( Unicode_is_string_of_unicode_buffer(string) )    
   {
      b_unicode_string = TRUE;
      //Point the first character of the unicode string
      p_string = Unicode_get_addr_string(string);
   }
   else
   {
      b_unicode_string = FALSE;
      //Point the first character of the ascii string
      p_string = string;
   }

   Ih_open_code_font_access();
   height = Ih_get_code_font_data();


   // Get the start X-coordinate of the first character if "offset_x" were equal to 0
   char_x_pos = Lcd_get_x();


   while( p_string != NULL )
   {
      if( b_unicode_string )
         { MSB(v4.wc) = *p_string++; LSB(v4.wc) = *p_string++; }
      else
         { v4.wc = *p_string++; }

      if( 0 == v4.wc )
      { // NULL character => end of string detected
         
         // All characters of the static string have been displayed
         // Define the last position of the cursor for an eventual next consecutive text display
         lcd_goto_xy( char_x_pos, Lcd_get_y() );

         // Exit out of the loop
         p_string = NULL; 
         continue;
      }

      ih_ptr_code_char( v4.wc );                  // Set pointer on the data of the character
      v2.char_width  = Ih_get_code_font_data();   // Get the width of this character in pixel unit


      // Define the new position of the cursor to the lcd controller
      lcd_goto_xy( char_x_pos, Lcd_get_y() );


      nb_lines      = lcd_get_nb_lines( v2.char_width );
      // "end_page_ws" gets the index of the end page without shifting-in-page
      end_page_ws   = lcd_get_end_page_ws( height );
      // "v3.end_page" gets the index of the end page with shifting-in-page
      v3.end_page   = lcd_get_end_page(    height );
      // BE CAREFUL : start_page_mask == end_page_mask == 0xff if b_merging set
      start_page_mask    = lcd_get_start_page_mask(  height ); 
      v1.end_page_mask   = lcd_get_end_page_mask(    height );
    
 
      b_suppl_page = FALSE;

      if( v3.end_page != end_page_ws )
      { // A supplementary page is required with the shifting-in-page for the display of this picture
         if ( v3.end_page < NB_LCD_PAGES )
         { // The supplementary page is allowed because of no danger of page overflow
            b_suppl_page = TRUE;
         }
      }
      start_page = Lcd_get_page();

      //Calculate the x-coordinate of the next character to display
      char_x_pos += v2.char_width;

      // KEEP IN MIND : v0.start_page_mask == v1.end_page_mask == 0xff if b_merging set

      for(; nb_lines != 0; nb_lines--)
      {
         v2.curr_page = start_page;
         Lcd_set_page( v2.curr_page );

         // Get the display data byte of the start page in which the drawing of the picture starts.
         // As a pixel shifting can exist, a part of the display data byte can be cleared to paste
         // the picture by a OR operation.
         // The clearing of these bits is done with the help of the mask "start_page_mask"
         Lcd_set_cursor();
         //Lcd_dummy_read();
         //Lcd_read_display_data();
         //v3.prev_disp_data = display_data_read & start_page_mask;
         v3.prev_disp_data = display_data_read;
      
         while( 1 )
         {          
            if( v2.curr_page > end_page_ws )    { break; }

            v4.char_disp_data = (U16)( Ih_get_code_font_data() * Lcd_get_shift_in_page() );
            LSB(v4.char_disp_data) |= v3.prev_disp_data;
            v3.prev_disp_data = MSB(v4.char_disp_data);


            if( v2.curr_page == end_page_ws )
            {
               if( start_page != end_page_ws )
               { // Number of pages > 1
                  if( FALSE == b_suppl_page )
                  { //The picture doesn't require a supplementary building of page,
                    //thus the mixing of the end part of the picture is to do right now
                     v3.prev_disp_data = LSB(v4.char_disp_data);
                     break;
                  }
               }
            }

            /*if( Lcd_is_merging() )
            {
               Lcd_dummy_read();
               Lcd_read_display_data();
               LSB(v4.char_disp_data) |= display_data_read;
            }*/

            Lcd_write_display_data( LSB(v4.char_disp_data) );
            Lcd_set_line( Lcd_get_line() );
            Lcd_set_page( ++v2.curr_page );
            Lcd_set_cursor();
         } 


         // Get the display data byte of the end page in which the drawing of the picture ends
         // As a pixel shifting can exist, a part of the display data byte can be cleared to paste
         // the picture by a OR operation.
         // The clearing of these bits is done with the help of the mask "end_page_mask"
         // The mixing of the end part of the character to display and the background is done here if
         // the number of pages is greater than 1 or a supplementary page exists.
         // Indeed, in the case where the number of pages is 1 and no supplementary page, 
         // this mixing has already be done with the help of the mask "start_page_mask" above.
         if( ( start_page != end_page_ws ) || ( TRUE == b_suppl_page ) )
         {
            //Lcd_dummy_read();
            //Lcd_read_display_data();
            //Lcd_write_display_data( (v3.prev_disp_data) | (display_data_read & v1.end_page_mask) );
            Lcd_write_display_data( v3.prev_disp_data );
         }

         Lcd_set_line( Lcd_get_line()+1 );
      }
   }

   Ih_end_code_font_access();

   // Require the display of the new built screen
   Lcd_new_screen_to_display();
}
#endif
#if 0
//! @brief lcd_clear_text_line_area
//!
//! Clear the area reserved in the drawing of a text line
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 x_end : end x-coordinate of the area to clear
//!
//! @return none
void lcd_clear_text_line_area( const U8 x_end )
{
   U8 height;

   Ih_open_font_access();
   height = ih_get_font_height(); //height of the current font
   Ih_end_font_access();

   lcd_clear_part_of_screen( 
        Lcd_get_x()
      , Lcd_get_y()
      , x_end-Lcd_get_x()+1      //width of the area to clear
      , height );
}
#endif

//! @brief lcd_display_screen
//!
//! Display the full screen from the screen buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return void
#if 0//( LCD_SCREEN_BUFFER == ENABLED )
void lcd_display_screen( void )
{
   U8 i_page;
   U8 nb_lines;

   s_ptr_screen_buffer = &s_screen_buffer[0];

   for(i_page = 0; i_page < NB_LCD_PAGES; i_page++)
   {
      Lcd_set_page( i_page );
      Lcd_set_line( 0 );
      Lcd_set_cursor() 

      for( nb_lines = NB_LCD_LINES; nb_lines !=0 ; nb_lines-- )
      {
         /* Waiting display interface available to lauch a writing */
         Lcd_drv_test_lcbusy();
         /* Transmit display data */
         Lcd_drv_write_data( *(s_ptr_screen_buffer++) ); 
      }
   }
}
#endif







#pragma CODE = LCD_SSD1303_TEMP_CODE01

#endif   // #if ( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303 )

#else

#endif   //#if( LCD_PRESENT == ENABLED )

#endif   // _lcd_SSD1303_c
