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

#ifndef _SEARCH_GAME_H_
#define _SEARCH_GAME_H_

#include "conf\conf_sio.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if( defined _search_game_c_ )
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

typedef struct
{
   U32   Point_Game_Project;
   U16   Num_Game;
   U32   Point_Game_keyTable;
   U16   Num_Game_keyTable;
   U8    Num_Every_Game;
   U8    Num_Wrong_Game;
   U32   Act_Right;
   U32   Act_Havewrong;
   U8    Fuction_Question;
   U32   Time;
   U16   OverTime_Game;
   U8    Mast_Game;
   U32   Point_Extern_Game_Project;
}Game_Table;

typedef struct
{
   U8    Num_Key;
   U32   Addr_Key_Index;
   U32   Addr_key_Concent;
   U8    Type_Game;
   U32   Act_Game_Concent;
   U8    Index_Game;
   U32   Act_Right;
   U32   Act_Wrong;
   U8    Num_Answer;
}Game_Project_Table;

_GLOBEXT_   _MEM_TYPE_SLOW_   Game_Table              Current_GameNews;
_GLOBEXT_   _MEM_TYPE_SLOW_   Game_Project_Table      Current_Game;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32                     GameTipTick;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32                     GameTimeOverTick;
_GLOBEXT_   _MEM_TYPE_SLOW_   U8                      GameProjectExternNum;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32                     Addr_Count_Game;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32                     Addr_Game_Wrong_Answer;

#define  GAME_TYPE_RAND             1
#define  GAME_TYPE_SEQU             2
#define  GAME_TYPE_REV              3
#define  GAME_TYPE_USER             4

#define  length_gkt                 17
#if 0
typedef union
{
  struct
  {
    unsigned   bInGame             :1;	/* check new key flag,bit0 */
    unsigned   bWrong              :1;	/* search result flag,bit1 */
    unsigned   bSubject            :1;	/* current control mode,bit3 */
    unsigned   bGameSpeech         :1; /* speech end flag */
    
    unsigned   bGameFirst          :1;	/*  */
    unsigned   bAnswerWrong        :1;	
    unsigned   bTimerEnable        :1;
    unsigned   bTimeOverEnable     :1;
    
    unsigned   bTipPlay            :1;
    unsigned   bGameTimeOverPlay   :1;
    unsigned   bGameClicked        :1;
    unsigned   bWaitSubject        :1;
    
    unsigned   bNewLink            :1; 
    /* bNewLink=FALSE,means leapfrog type,
       bNewLink=TRUE,means paka type.*/
    unsigned   bSearchFound        :1;
    unsigned   bGameRev1           :1;
    unsigned   bGameRev2           :1;
  }bits;
  U16     value;
}GAME_STATUS;
_GLOBEXT_ _MEM_TYPE_SLOW_ GAME_STATUS bGameStatus
#if 0//( defined _search_game_c_ )
  = { 0 }
#endif
;
#define bInGame               bGameStatus.bits.bInGame
#define bWrong                bGameStatus.bits.bWrong
#define bSubject              bGameStatus.bits.bSubject
#define bGameSpeech           bGameStatus.bits.bGameSpeech
#define bGameFirst            bGameStatus.bits.bGameFirst
#define bAnswerWrong          bGameStatus.bits.bAnswerWrong
#define bTimerEnable          bGameStatus.bits.bTimerEnable
#define bTimeOverEnable       bGameStatus.bits.bTimeOverEnable
#define bTipPlay              bGameStatus.bits.bTipPlay
#define bGameTimeOverPlay     bGameStatus.bits.bGameTimeOverPlay
#define bGameClicked          bGameStatus.bits.bGameClicked
#define bWaitSubject          bGameStatus.bits.bWaitSubject
#define bNewLink              bGameStatus.bits.bNewLink
#define bSearchFound          bGameStatus.bits.bSearchFound

typedef union
{
  struct
  {
    unsigned    bNotAutoNextGame    :1;//
    unsigned    bGamePrev           :1;//
    unsigned    bExitGamePRN        :1;//
    unsigned    bQuestion           :1;//
    
    unsigned    bSpeechEnd          :1;
    unsigned    bSkipNext           :1;
    unsigned    bSkipPrevious       :1;
    unsigned    bGameRev3           :9;
  }bits;
  U16      value;
}GAME_BIT_NEW;
_GLOBEXT_ _MEM_TYPE_SLOW_ GAME_BIT_NEW game_bits
#if 0//( defined _search_game_c_ )
  = { 0x10 }
#endif
;
#define bNotAutoNextGame      game_bits.bits.bNotAutoNextGame
#define bGamePrev             game_bits.bits.bGamePrev
#define bExitGamePRN          game_bits.bits.bExitGamePRN
#define bQuestion             game_bits.bits.bQuestion
#define bSpeechEnd            game_bits.bits.bSpeechEnd
#define bSkipNext             game_bits.bits.bSkipNext
#define bSkipPrevious         game_bits.bits.bSkipPrevious
#endif


typedef struct
{
   U32   Act_Right;
   U32   Act_First_Wrong;
   U32   Act_Second_Wrong;
   U32   Act_Third_Wrong;
}Game_concent_Table;


#define MAX_GAME_ANSWER           32

_GLOBEXT_   _MEM_TYPE_SLOW_   U8                      AnswerCount;
_GLOBEXT_   _MEM_TYPE_SLOW_   U8                      Current_GameKeyIndex[ MAX_GAME_ANSWER ];
_GLOBEXT_   _MEM_TYPE_SLOW_   Game_concent_Table      Current_GameKeyConcent[ MAX_GAME_ANSWER ];

//_GLOBEXT_                     Bool                    game_remote_search;


//! Script parameter define
//!
//SET参数：V1,Number
//只有SET,CMP,AND,OR,NOT,TEST,ADD,SUB需要检查Type字节

typedef struct
{
   Byte     Command;
   Byte     Type;
}SCRIPT_CMD, *PSCRIPT_CMD;


typedef struct
{
   Uint16   Parameter1;
   Uint16   Parameter2;
}SCRIPT_PARA, *PSCRIPT_PARA;

//! command type define 
#define  CMD_TYPE_NONE           (( Byte )0 )
#define  CMD_TYPE_VER_CONST      (( Byte )1 )
#define  CMD_TYPE_VER_VER        (( Byte )2 )

//! cmp result type
#define  CMP_GREAT               (( Int8 )1 )
#define  CMP_EQUAL               (( Int8 )0 )
#define  CMP_LESS                (( Int8 )-1 )

#define  TEST_EQUAL              (( Int8 )0 )
#define  TEST_NOT_EQUAL          ~TEST_EQUAL

//! command set define 
#define  CMD_END                 (( Byte )0 )
#define  CMD_CLEARVER            (( Byte )1 )
#define  CMD_SET                 (( Byte )2 )
#define  CMD_CMP                 (( Byte )3 )
#define  CMD_AND                 (( Byte )4 )
#define  CMD_OR                  (( Byte )5 )
#define  CMD_NOT                 (( Byte )6 )
#define  CMD_TEST                (( Byte )7 )
#define  CMD_JMP                 (( Byte )8 )
#define  CMD_JE                  (( Byte )9 )
#define  CMD_JNE                 (( Byte )10 )
#define  CMD_JG                  (( Byte )11 )
#define  CMD_JGE                 (( Byte )12 )
#define  CMD_JB                  (( Byte )13 )
#define  CMD_JBE                 (( Byte )14 )
#define  CMD_ADD                 (( Byte )15 )
#define  CMD_SUB                 (( Byte )16 )
#define  CMD_PLAY                (( Byte )17 )
#define  CMD_PLAYRANDOM          (( Byte )18 )
#define  CMD_PLAYNUMBER          (( Byte )19 )

#define  SCRIPT_VARIABLE_NUMBER  100

_GLOBEXT_   _MEM_TYPE_SLOW_   Uint16   ScriptVariable[ SCRIPT_VARIABLE_NUMBER ];


_GLOBEXT_   _MEM_TYPE_SLOW_   U8       CurrentKey;  //current menu key

#define ENTER                 0x0E
#define FUN                   0x06
#define PREV                  0x02
#define NEXT                  0x05

/*_GLOBEXT_ Bool gl_key_press
#if 0//( defined _search_game_cal_c_ )
  = FALSE
#endif
;*/

enum
{
   GAME_LETTLE1 = 0,
   GAME_LETTLE2,
   GAME_INPUT 
};

#define LETTLE_MAX            31
#define LETTLE_MIN            0

#define WORD_MAX              16

//_GLOBEXT_   _MEM_TYPE_SLOW_   U8    lettle_count;

_GLOBEXT_   _MEM_TYPE_SLOW_   U8    GameState;

_GLOBEXT_   _MEM_TYPE_SLOW_   U8    word_count;
_GLOBEXT_   _MEM_TYPE_SLOW_   U8    word[ WORD_MAX ];

Bool  game_initial                  ( void );
void  in_game                       ( void );
void  check_game_time               ( void );
void  game_timeout_deal             ( void );
void  init_current_game             ( void );
void  play_timer_tip                ( void );
void  game_next                     ( void );
void  game_exit                     ( void );
void  read_game_project_table       ( U32 Address, Game_Project_Table *buf );
void  exit_game_to_control          ( void );

void  script_init                   ( U32 script_addr );
void  script_decode                 ( void );
void  clear_script_variable         ( void );

void  print_dic_screen              ( void );
void  print_lettle_screen           ( U8 count );
void  do_game                       ( void );
void  clr_input_buffer              ( void );
void  print_key_char                ( U8 x, U8 y, U8 asc, U8 iasc );
void  print_word_lettle             ( U8 count );
void  print_lettle_cance            ( U8 count );
void  print_word_identifier         ( Bool inverse );
void  print_clear_input             ( void );
void  initial_console               ( void );
void  game_console                  ( void );
void  print_lettle_screen           ( U8 count );

#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
