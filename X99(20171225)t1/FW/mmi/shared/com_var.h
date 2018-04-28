//!
//! @file com_var.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI common var
//!
//! @version 2.5 snd3-dvk-1_9_5 $Id: com_var.h,v 2.5 2007/07/26 14:57:49 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _com_var_h_
#define _com_var_h_


//_____ I N C L U D E S ____________________________________________________

#include "compiler.h"

#include "lib_system\unicode\unicode.h"
//#include "modules\file_system\explorer.h"
//#include "modules\usb\device_drivers\usb_device_ms.h"

#include "conf\conf_sio.h"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _com_var_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

// File extensions supported by the player & viewer
/*
typedef enum
{
   IDLE_APPLI = 0
,  PLAYER_APPLI  
,  PASTER_APPLI
,  AP3_APPLI
,  RECORDER_APPLI
,  DIC_APPLI
,  SETTING_APPLI
,  MSTORAGE_APPLI
,  POWER_OFF_APPLI
,  WIRELESS_APPLI
,  STUDY_APPLI
,  REREAD_APPL
}select_appli;
*/
typedef enum
{
   IDLE_APPLI = 0
,  AP3_APPLI
,  STUDY_APPLI
,  PLAYER_APPLI
,  DIC_APPLI
,  SETTING_APPLI
,  MSTORAGE_APPLI
,  POWER_OFF_APPLI
,  WIRELESS_APPLI
,  REREAD_APPLI
,  RECORDER_APPLI
,  PASTER_APPLI

,  MSTORAGE_HID_APPLI
}select_appli;

#define DEFAULT_APPLI         0  //IDLE_APPLI
//#define DEFAULT_APPLI         3  //AP3_APPLI
//#define DEFAULT_APPLI         2  //PASTER_APPLI
//#define DEFAULT_APPLI         1  //PLAYER_APPLI
/*
_GLOBEXT_   _MEM_TYPE_SLOW_   select_appli   g_select_appli
#if 0//def _com_var_c_
   = DEFAULT_APPLI
#endif
;
_GLOBEXT_   _MEM_TYPE_SLOW_   select_appli   g_old_select_appli
#if 0//def _com_var_c_
   = DEFAULT_APPLI
#endif
;
*/
typedef enum
{
   POWER_OFF = 0
,  BATTERY_LOW  
}power_off_mode;

//_GLOBEXT_   _MEM_TYPE_SLOW_   power_off_mode    g_power_off_mode;
/*_GLOBEXT_   _MEM_TYPE_SLOW_   U8                wav_encrypt
#if 0//def _com_var_c_
   = FALSE
#endif
;*/
_GLOBEXT_   _MEM_TYPE_SLOW_   U8                audio_is_wav
#if 0//def _com_var_c_
   = FALSE
#endif
;


// File extensions supported by the player & viewer
/*#ifdef _com_var_c_
   _MEM_TYPE_SLOW_   U8    ext_filemusic[]    = "mp3";
#else
   extern _MEM_TYPE_SLOW_   U8    ext_filemusic[];
#endif
*/

// Definition of the repeat modes 
typedef U8  Repeat;
#define SINGLE          0     // Play one file and stop at the end
#define NORMAL          1     // Play one folder and stop at the end
//#define RANDOME			2	  // randome play one folder
#define REPEAT_SINGLE   2     // Play and repeat one file
#define REPEAT_ALL      3     // Play and repaet folder/disk according to the value g_setting_list.player_exp_mode
	

#define NB_REPEAT       4

//#define RANDOME			5	  // randome play one folder

// Values taken by the argument "param" passed at the calling of the application MMI_UPDATE
typedef enum
{
   UPDATE_STOPPED = 0            // Update not started
,  UPDATE_START                  // Update with the selected file 
,  UPDATE_RESTART                // Update with the saved file in g_setting_list.last_update_file
,  UPDATE_ON_GOING
}Update_mode;

// Values taken by the argument "param" passed at the calling of the application MMI_INFO
#define INFO_ERROR               0     // default value
#define INFO_MSG                 1



//_____ D E F I N I T I O N S ______________________________________________


// Definition of the elasped time structure
typedef struct
{
   U8 hours;
   U8 minutes;
   U8 seconds;
}Elapsed_time;

/***************** Equalizer ********************/
// Sound level structure 
// (takes values of the ambiance sound levels defined just below)
typedef struct
{
   U8 bass;
   U8 medium;
   U8 treble;
}Eq_bands;

// Ambiance sound levels
#define p6DB   0  //+6dB
#define p4DB   1
#define p2DB   2
#define p0DB   3
#define m2DB   4
#define m4DB   5
#define m6DB   6  //-6dB

// Index of the user ambiance
#define AMBIANCE_USER_IDX    9



//_____ D E C L A R A T I O N S ____________________________________________
//Variables

// Strings to store a text (Global to optimize the data space)
#define  G_STRING_SIZE     (127 *2)
#define  G_STRING_SIZE2    (30 *2)
_GLOBEXT_ UnicodeBuffer g_string   [G_STRING_SIZE ];
_GLOBEXT_ UnicodeBuffer g_string2  [G_STRING_SIZE2];

//_GLOBEXT_ _MEM_TYPE_SLOW_ UnicodeBuffer g_string1  [ 16 ];

//***********************************************
#if( PASTER_MODE == ENABLE )
//_GLOBEXT_ _MEM_TYPE_SLOW_ UnicodeBuffer g_string1  [G_STRING_SIZE2];
#endif   //#if( PASTER_MODE == ENABLE )
/*_GLOBEXT_ Bool g_b_volume_change
#ifdef _com_var_c_
   = TRUE
#endif
;*/
//***********************************************

/**************************** User setting structure **********************************/
#if( afaya_pen == ENABLE )
#define DEFAULT_VOLUME        4
#endif   //#if( afaya_pen == ENABLE )
#if( i_speak == ENABLE )
#define DEFAULT_VOLUME        4
#endif   //#if( i_speak == ENABLE )

// Version of setting (in case of structure change after update)
//#define  SETTING_VERSION         "SETTING V2"
//#define  SETTING_VERSION         10//"SETTING V2"

// Language supported
#define LANG_ENGLISH          0
#define LANG_CHINESE_SIMP     1
#define LANG_CHINESE_TRAD     2

#define LANG_FRENCH           3
// Backlight mode
#define BACKLIGHT_MODE_OFF       0  // Backlight mode is disable
#define BACKLIGHT_MODE_FULL_OFF  1  // When Backlight is activated, this deactivates the OLED
#define BACKLIGHT_MODE_LOW       2  // When Backlight is activated, OLED is faintly running

// Number of stations storage
#define NB_STATIONS_NORMAL_FM    12 // Number of radio stations on normal band
#define NB_STATIONS_JAPAN_FM     6  // Number of radio stations on japan band

#if 0
typedef struct
{  
   U8          size_of_struct;                     // size of setting
   //U8          version[sizeof(SETTING_VERSION)];   // version of setting
   U8          version[SETTING_VERSION];   // version of setting
// GLOBAL
   U8          language_ihm;
   //U8          language_file;
   U8          poweroff_value;
// DISPLAY
   U8          backlight_mode;
   U8          backlight_timeout;
// GAMES
   /*U8          snake_highscore;
// RADIO
   U8          fm_volume;
   U8          fm_mode;
   U8          fm_band;
   U8          fm_norm_pos;
   U8          fm_japan_pos;
   U16         fm_norm_freq;
   U16         fm_japan_freq;
   U16         fm_norm_freq_preset[NB_STATIONS_NORMAL_FM];
   U16         fm_japan_freq_preset[NB_STATIONS_JAPAN_FM];
// PLAY LIST
   Fs_index    playlist_index;
// VIEWER
   Fs_index    viewer_file;
   U8          viewer_exp_use;
   U16         viewer_exp_pos;
   Exp_mode    viewer_exp_mode;
   Exp_rand    viewer_exp_rand;
   Repeat      viewer_repeat;
   U8          viewer_configue;
// PLAYER
   Elapsed_time player_start_time;
   Fs_index    player_file;
   U8          player_exp_use;
   U16         player_exp_pos;
   Exp_mode    player_exp_mode;
   Exp_rand    player_exp_rand;*/
   Repeat      player_repeat;
   U8          player_volume;
   /*U8          player_eq_ambiance;
   Eq_bands    player_eq_user;
   U8          player_speed;
   U8          player_bassboost;
   U8          player_vsurround;*/
// Data for Password management

}struct_setting;

// Declaration of the setting structure
//_GLOBEXT_ _MEM_TYPE_SLOW_  struct_setting  g_setting_list;
#endif

//_____ M A C R O S ________________________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

void com_reset_setting( void );
Bool com_check_setting( void );

#endif  //! _com_var_h_
