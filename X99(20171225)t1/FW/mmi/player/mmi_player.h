//!
//! @file mmi_player.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: mmi_player.h,v 2.1 2007/08/22 14:07:01 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_PLAYER_H_
#define _MMI_PLAYER_H_


//_____ I N C L U D E S ____________________________________________________

#include "lib_mcu\compiler.h"

#include "mmi\ap3\mmi_ap3.h"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmi_player_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


//_____ D E F I N I T I O N S ______________________________________________
//_GLOBEXT_ _MEM_TYPE_SLOW_ U32 file_size_count;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U32 play_file_size;
//_GLOBEXT_                     Bool  g_b_music_end;
//_GLOBEXT_   _MEM_TYPE_MEDFAST_   U8    tmpINT3;
//_GLOBEXT_                     Bool  mp3_isr_flag;

typedef struct
{
   U32   file_size_count;
   U32   play_file_size;
}AUDIO_VAR;

typedef union
{
   AUDIO_VAR   audio_var;
   U8          encrypt[ 8 ];
}ENCRYPT_DATA;

//_GLOBEXT_ _MEM_TYPE_SLOW_ ENCRYPT_DATA ap4_audio_var;


//_____ M A C R O S ________________________________________________________
#define mmi_player_kbd_disable()    ( b_kbd_process_disable = TRUE )
#define mmi_player_kbd_enable()     ( b_kbd_process_disable = FALSE )
#define mmi_player_kbd_is_disable() ( b_kbd_process_disable == TRUE )

#define mp3_player_state            ap4_play_state

//#define SCROLLING_AREA_START_X      24//0
#define SCROLLING_AREA_START_X      0
#define SCROLLING_AREA_END_X        128

#define RUNTIME_BEGIN_X        		0
#define RUN_STATE_X        			0

/*_GLOBEXT_   const    U8    player_dir[]
#if 0//def _mmi_player_c_
= "Mp3"
#endif
;

_GLOBEXT_   const    U8    ext_filemusic[]
#if 0//def _mmi_player_c_
= "wav"
#endif
;*/

#endif  //! _MMI_PLAYER_H_
