//!
//! @file mmi_startup.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file declares the MMI application STARTUP
//!
//! Is the base application of the MMI.
//! Must be lauched first by the kernel.
//!
//! @version 1.1 snd3-dvk-1_9_5 $Id: mmi_startup.h,v 1.1 2006/09/19 17:08:19 ppatron Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_STARTUP_H_
#define _MMI_STARTUP_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmi_startup_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U8    internal_speech
#if 0//def _mmi_startup_c_
= 0
#endif
;

#define STARTUP_SPEECH        0x01
#define POWEROFF_SPEECH       0x02
#define BATTERYLOW_SPEECH     0x04
/*
_GLOBEXT_   Bool              play_startup_flag
#if 0//def _mmi_startup_c_
   = FALSE
#endif
;
*/

#define  SETTING_VERSION         "SETTING V2"
#define  SETTING_VERSION_LEN     sizeof( SETTING_VERSION )

//_____ D E F I N I T I O N S ______________________________________________
_GLOBEXT_   void  startup_display (void);

Bool  search_subdir_mp3_file        ( char *str );
Bool  play_sbdir_mp3_file           ( void );
void  search_startup                ( void );
void  search_poweroff               ( void );
void  search_batterylow             ( void );
Bool  mmi_startup_playintro         ( void );

#endif  //! _MMI_STARTUP_H_
