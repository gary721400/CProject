//!
//! @file search_game.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search game.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: search_game.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _SEARCH_GAME_CAL_H_
#define _SEARCH_GAME_CAL_H_

#include "conf\conf_sio.h"
#include "modules/text/text_viewer.h"
#include "conf\conf_mmi.h"
//#include "conf\conf_kbd.h"                   // Keyboard configuration
#include "lib_mcu\kbd\kbd_drv.h"             // Keyboard driver definition
#include "mmi\kbd\keypad.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if( defined _search_game_cal_c_ )
#  define _GLOBEXT_
#else
#  define _GLOBEXT_ extern
#endif


#define  SEARCH_GAME_PosX_InputWord          0
#define  SEARCH_GAME_PosY_InputWord          8
#define  SEARCH_GAME_PosX_InputCursor        0
#define  SEARCH_GAME_PosY_InputCursor        8
#define  SEARCH_GAME_PosX_Keyboard           0
#define	SEARCH_GAME_PosY_Keyboard           16
#define  SEARCH_GAME_Title_CharWidth         6

_GLOBEXT_ _MEM_TYPE_SLOW_ U8  CurrentKey;  //current menu key

#define ENTER                 0x0E
#define FUN                   0x06
#define PREV                  0x02
#define NEXT                  0x05

_GLOBEXT_ Bool gl_key_press
#if( defined _search_game_cal_c_ )
  = FALSE
#endif
;


enum
{
   GAME_LETTLE1 = 0,
   GAME_LETTLE2,
   GAME_INPUT 
};

#define LETTLE_MAX            31
#define LETTLE_MIN            0

#define WORD_MAX              16

_GLOBEXT_   _MEM_TYPE_SLOW_   U8    lettle_count;

_GLOBEXT_   _MEM_TYPE_SLOW_   U8    GameState;

_GLOBEXT_   _MEM_TYPE_SLOW_   U8    word_count;
_GLOBEXT_   _MEM_TYPE_SLOW_   U8    word[ WORD_MAX ];

#define  DIS_RESULT_ONLY      0x01
#define  DIS_MEMORY           0x02
#define  PLAY_RESULT          0x04
#define  GET_RESULT           0x08
#define  GET_MEMORY           0x10

void     print_dic_screen           ( void );
void     print_lettle_screen        ( U8 count );
void     do_game                    ( void );
void     clr_input_buffer           ( void );
void     cal_act_do                 ( void );
void     clear_cal_error            ( void );
void     display_float              ( Float16 result, U8 dis_type );

#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_GAME_CAL_H_
