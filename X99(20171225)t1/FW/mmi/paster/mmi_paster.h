//!
//! @file mmi_debug.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application DEBUG
//!
//! @version 1.7 snd3b-dvk-1_0_33 $Id: mmi_debug.h,v 1.7 2006/03/29 08:02:51 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_PASTER_H_
#define _MMI_PASTER_H_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_lcd.h"
//#include "mmi\ap3\mmi_ap3.h"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if( defined _mmi_paster_c_ )
  #define _GLOBEXT_ extern
#else
  #define _GLOBEXT_ extern
#endif

//_____ D E C L A R A T I O N S ____________________________________________
//_GLOBEXT_                     Bool       b_kbd_process_disable;
#define mmi_paster_kbd_disable()    ( b_kbd_process_disable = TRUE )
#define mmi_paster_kbd_enable()     ( b_kbd_process_disable = FALSE )
#define mmi_paster_kbd_is_disable() ( b_kbd_process_disable == TRUE )

//#define apf_play_state              ap4_play_state

//_____ M A C R O S ________________________________________________________
#if( LCD_PRESENT == ENABLE )
void  mmipaster_display_name     ( void );
void  mmipaster_play_time        ( void );
void  mmi_paster_display_icon    ( void );
void  mmi_paster_display_volume  ( void );

#else
#define mmipaster_display_name()
#define mmipaster_play_time()
#define mmi_paster_display_icon()
#define mmi_paster_display_volume()

#endif   //#if( LCD_PRESENT == ENABLE )
//_____ D E C L A R A T I O N S ____________________________________________

#endif  //! _MMI_PASTER_H_
