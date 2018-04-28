//!
//! @file mmi_idle.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file declares the MMI application IDLE
//!
//! Is the base application of the MMI.
//!
//! @version 1.22 snd3-dvk-1_9_5 $Id: mmi_idle.h,v 1.22 2007/06/25 13:12:28 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_IDLE_H_
#define _MMI_IDLE_H_



//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________

//! --- Add the Update application in the idle menu ---
//! Values: - ENABLE
//!         - DISABLE
#define UPDATE_UP_FROM_MENU            DISABLE

#undef _GLOBEXT_
#if (defined _mmi_idle_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


// ____ DISPLAY DEFINITIONS ________________________________________________

typedef struct
{
   U32 IDP_inactive;
   U32 IDP_active;
   U32 IDP_name;
}Idle_appli_disp;


//Data setting coordinates of the appli icons
typedef struct
{
   U8 x;
   U8 y;
}Idle_disp_frame;


/*
typedef enum
{
   SELECT_AP4_MENU = 0x01
,  SELECT_DIC_MENU
,  SELECT_PLAY_MENU
,  SELECT_SETTING_MENU
,  SELECT_NUM
}Idle_Main_Menu;
*/

#define mmi_idle_kbd_disable()    ( b_kbd_process_disable = TRUE )
#define mmi_idle_kbd_enable()     ( b_kbd_process_disable = FALSE )
#define mmi_idle_kbd_is_disable() ( b_kbd_process_disable == TRUE )

//_____ M A C R O S ________________________________________________________
_GLOBEXT_ _MEM_TYPE_SLOW_ U8 oid_display_enable;


//_____ D E C L A R A T I O N S ____________________________________________
Bool  search_subdir_mp3_file        ( char *str );
Bool  play_sbdir_mp3_file           ( void );


#endif  //! _MMI_IDLE_H_
