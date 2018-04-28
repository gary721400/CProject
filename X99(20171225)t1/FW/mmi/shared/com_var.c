//!
//! @file com_var.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 2.4 snd3-dvk-1_9_5 $Id: com_var.c,v 2.4 2007/07/26 14:57:49 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _com_var_c_
#define _com_var_c_


//_____  I N C L U D E S ___________________________________________________
//#include "config.h"

//#include LIB_MEM

#include "com_var.h"
//#include "conf\conf_mmi.h"
#include <string.h>
#include "compiler.h"

#pragma DATA = COM_VAR_DATA
#pragma CODE = COM_VAR_CODE


//_MEM_TYPE_SLOW_   select_appli      g_select_appli = DEFAULT_APPLI;
//_MEM_TYPE_SLOW_   select_appli      g_old_select_appli = DEFAULT_APPLI;

//_MEM_TYPE_SLOW_   power_off_mode    g_power_off_mode;
//_MEM_TYPE_SLOW_   U8                wav_encrypt = FALSE;
_MEM_TYPE_SLOW_   U8                audio_is_wav /*= FALSE*/;

UnicodeBuffer     g_string   [G_STRING_SIZE ];
UnicodeBuffer     g_string2  [G_STRING_SIZE2];
/*
_MEM_TYPE_SLOW_   UnicodeBuffer     g_string1  [ 16 ];

_MEM_TYPE_SLOW_   struct_setting    g_setting_list;

_MEM_TYPE_SLOW_   U8                setting_language_ihm;
_MEM_TYPE_SLOW_   U8                setting_poweroff_value;
_MEM_TYPE_SLOW_   U8                setting_backlight_mode;
_MEM_TYPE_SLOW_   U8                setting_backlight_timeout;

const U8  setting_version[] = "SETTING V2";
*/
//_____ D E F I N I T I O N S ______________________________________________
#if 0
void com_reset_setting( void )
{
// fill structure with 0   
   memset( &g_setting_list , 0 , sizeof(g_setting_list) ); 
   g_setting_list.size_of_struct    = sizeof(g_setting_list);
// GLOBAL
	memcpy( g_setting_list.version, SETTING_VERSION , sizeof(SETTING_VERSION) );

   g_setting_list.player_volume        = 7;
}


Bool com_check_setting(void)
{
	return (0 == memcmp_code2ram( g_setting_list.version, SETTING_VERSION , sizeof(SETTING_VERSION) ));
}
#endif
#endif

