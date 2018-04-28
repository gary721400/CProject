//!
//! @file lcd_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the LCD drivers used in Qundau
//!
//! @version 1.20 snd3-dvk-1_9_5 $Id: lcd_drv.h,v 1.20 2007/01/11 13:34:09 coger Exp $
//!
//! @todo
//! @bug
#ifndef _LCD_DRV_H_
#define _LCD_DRV_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_lcd.h"
#include "conf\conf_clock.h"
#include "lib_mcu\timer\timer_drv.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _lcd_drv_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


//_____ S E T T I N G   D E F I N I T I O N S ______________________________

#define TOSC   (1000000/FOSC+1)

//_____ M A C R O S ________________________________________________________

//! --- Lcd interface control ---
#define Lcd_drv_enable_interface()     ( _bitSET( _P80CON_b0 + 15 ))
#define Lcd_drv_disable_interface()    ( _bitCLR( _P80CON_b0 + 15 ))

//! --- Read command ---
#define Lcd_drv_set_read_command()
//NB : the bit "Read command" cleared automatically after the reading completed

//! --- Register data read / write ---
#define Lcd_drv_write_data(val)        { P80DO = val; _bitCLR( _P80CON_b0 + 4 );_bitSET( _P80CON_b0 + 3 ); _bitSET( _P80CON_b0 + 2 ); _bitSET( _P80CON_b0 + 0 );}
#define Lcd_drv_read_data()            ( P80DI )
#define Lcd_drv_write_cmd(val)         { P80CO = val; _bitCLR( _P80CON_b0 + 3 ); _bitSET( _P80CON_b0 + 2 ); _bitSET( _P80CON_b0 + 0 );}

//! --- Register selection macros ---
#if (LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL == HIGH_LEVEL)
#  define Lcd_drv_select_instr_reg()
#  define Lcd_drv_select_data_reg()
#else
#  define Lcd_drv_select_instr_reg()
#  define Lcd_drv_select_data_reg()    //{ _bitSET( _P80CON_b0 + 3 ); _bitSET( _P80CON_b0 + 2 ); _bitSET( _P80CON_b0 + 0 );}
#endif

//! --- Automatic busy process macros ---
//#define Lcd_drv_disable_busy_check()    {}//(LCDBUM = 0x00)
//#define Lcd_drv_enable_busy_check(mask) {}//(LCDBUM = mask)

#ifdef LCBUSY_TEST_IMPLEMENTED
#   define Lcd_drv_test_lcbusy()
#else
#   define Lcd_drv_test_lcbusy()          { while( _bit_get( _P80CON_b0 + 0 ));}
#endif




//_____ D E C L A R A T I O N S ______________________________________________

_GLOBEXT_   void  lcd_drv_basic_init_interface( void );

void  SetStorageBusy    ( void );
void  ClearStorageBusy  ( void );

#endif   // _LCD_DRV_H_
