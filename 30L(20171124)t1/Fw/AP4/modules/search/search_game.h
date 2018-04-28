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

#include "conf_sio.h"
//#include "game_play.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________


#define LINK_PAKA                 0x02
#define EXIT_PRN_EN               0x04
#define QUESTION_INDEX_EN         0x08
#define NOT_AUTO_GAME_EN          0x10

#define MAX_GAME_ANSWER           32

#define SORT_TYPE                 1
#define COMBINED_TYPE             2
#define CONNECT_TYPE              3
#define COUNT_TYPE				  4
#define NEW_CONNECT_TYPE		  5


#define GAME_MAX_RANGE_RAND         4       // 32 files

#define SCRIPT_VARIABLE_NUMBER   100

typedef struct{
  U8 Command;
  U8 Type;
}SCRIPT_CMD,*PSCRIPT_CMD;
typedef struct{
  Uint16 Parameter1;
  Uint16 Parameter2;
}SCRIPT_PARA,*PSCRIPT_PARA;

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
   U32   Addr_Extern_Project_Game;
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

typedef struct
{
   Coor  Game_Coor;
   U8    Type_Key;
   U8    Index_key;
   U32   Action;
   U8    caption[ Length_Caption ];
}Game_key_Table;

typedef struct
{
   U32   Act_Right;
   U32   Act_First_Wrong;
   U32   Act_Second_Wrong;
   U32   Act_Third_Wrong;
}Game_concent_Table;

typedef struct
{
   U8   Index_Wrong_Answer;
   U32  Act_Wrong_Answer;
}Game_Wrong_Answer_Table;


extern   _MEM_TYPE_SLOW_   Game_Table              Current_GameNews;
extern   _MEM_TYPE_SLOW_   Game_Project_Table      Current_Game;
extern   _MEM_TYPE_SLOW_   U32                     GameTipTick;
extern   _MEM_TYPE_SLOW_   U32                     GameTimeOverTick;
//extern 	_MEM_TYPE_SLOW_   Game_Wrong_Answer_Table  current_game_wrong_answer_table;

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
//	unsigned   rev				   :2;
  }bits;
  U16     value;
}GAME_STATUS;
#else
typedef union
{
  struct
  {
  	unsigned   rev				   :1;
	unsigned   bGameExitFlag	   :1;
	unsigned   bSearchFound        :1;
	/* bNewLink=FALSE,means leapfrog type,
       bNewLink=TRUE,means paka type.*/
	unsigned   bNewLink            :1; 
	
    unsigned   bWaitSubject        :1;   
    unsigned   bGameClicked        :1;
 	unsigned   bGameTimeOverPlay   :1;
	unsigned   bTipPlay            :1;

    unsigned   bTimeOverEnable     :1;
    unsigned   bTimerEnable        :1;  
  	unsigned   bAnswerWrong        :1;	
  	unsigned   bGameFirst          :1;	/*  */
	
	unsigned   bGameSpeech         :1; /* speech end flag */
	unsigned   bSubject            :1;	/* current control mode,bit2 */
    unsigned   bWrong              :1;	/* search result flag,bit1 */
    unsigned   bInGame             :1;	/* check new key flag,bit0 */
  }bits;
  U16     value;
}GAME_STATUS;
#endif

extern _MEM_TYPE_SLOW_ GAME_STATUS bGameStatus;

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
#define bGameExitFlag         bGameStatus.bits.bGameExitFlag


#if 0
typedef union	_GAME_BIT_NEW
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

		unsigned    brev				:9;
	  }bits;
	  U16      value;
}GAME_BIT_NEW;
#else
typedef union	_GAME_BIT_NEW
{
	  struct
	  {

		unsigned    brev				:9;
		
	    unsigned    bSkipPrevious       :1;//bit 6
	    unsigned    bSkipNext           :1;//bit 5
	    unsigned    bSpeechEnd          :1;//bit 4 

		
		unsigned    bQuestion           :1;////bit 3
		unsigned    bExitGamePRN        :1;////bit 2
	    unsigned    bGamePrev           :1;////bit 1
	    unsigned    bNotAutoNextGame    :1;//bit 0
	    
	  }bits;
	  U16      value;
}GAME_BIT_NEW;
#endif

extern _MEM_TYPE_SLOW_ GAME_BIT_NEW game_bits;

#define bNotAutoNextGame      game_bits.bits.bNotAutoNextGame
#define bGamePrev             game_bits.bits.bGamePrev
#define bExitGamePRN          game_bits.bits.bExitGamePRN
#define bQuestion             game_bits.bits.bQuestion
#define bSpeechEnd            game_bits.bits.bSpeechEnd
#define bSkipNext             game_bits.bits.bSkipNext
#define bSkipPrevious         game_bits.bits.bSkipPrevious



extern   _MEM_TYPE_SLOW_   Game_Table              Current_GameNews;
extern   _MEM_TYPE_SLOW_   Game_Project_Table      Current_Game;
extern   _MEM_TYPE_SLOW_   U32                     GameTipTick;
extern   _MEM_TYPE_SLOW_   U32                     GameTimeOverTick;

extern _MEM_TYPE_SLOW_ GAME_STATUS bGameStatus;
 
extern _MEM_TYPE_SLOW_ GAME_BIT_NEW game_bits;

//_MEM_TYPE_SLOW_ Game_key_Table      Current_GameKey;
extern _MEM_TYPE_SLOW_ U8                  Current_GameKeyIndex[ MAX_GAME_ANSWER ];
extern _MEM_TYPE_SLOW_ Game_concent_Table  Current_GameKeyConcent[ MAX_GAME_ANSWER ];

extern _MEM_TYPE_SLOW_ U8                  GameCount;
//_MEM_TYPE_SLOW_ U8                  ErrorCount;
extern _MEM_TYPE_SLOW_ U8                  RightNumber;
//_MEM_TYPE_SLOW_ U16                 CurrentGame;


extern _MEM_TYPE_SLOW_ U8                  AnswerCount;
extern _MEM_TYPE_SLOW_ U8                  AnswerIndex;
extern _MEM_TYPE_SLOW_ U8                  AnswerCur;
extern _MEM_TYPE_SLOW_ U8                  CurrentAnswerCount;
extern _MEM_TYPE_SLOW_ U8                  ErrCur;
//_MEM_TYPE_SLOW_ U8                  Answered[ MAX_GAME_ANSWER ];
extern _MEM_TYPE_SLOW_ U8                  Answered[(( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 ];
extern _MEM_TYPE_SLOW_ U8                  Wrong_Answered[(( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 ];
extern _MEM_TYPE_SLOW_ U8					Wrong_Answered_count;
extern _MEM_TYPE_SLOW_ U8                  iCur;
extern _MEM_TYPE_SLOW_ U8                  QuestionIndex;
extern _MEM_TYPE_SLOW_ U32                 QuestionSpeech;
extern _MEM_TYPE_SLOW_ U8                  HintAnswerIndex;

extern _MEM_TYPE_SLOW_  U8				   game_timer_stop;


extern _MEM_TYPE_SLOW_   U8                g_game_rand_tab[ GAME_MAX_RANGE_RAND ];
extern _MEM_TYPE_SLOW_ Game_Wrong_Answer_Table current_game_wrong_answer_table;
extern _MEM_TYPE_SLOW_ U32                 Addr_current_game_wrong_answer_table;
extern _MEM_TYPE_SLOW_ U32                 Addr_current_count_game_voice_table;
extern  _MEM_TYPE_SLOW_  U8					count_type_num;



//! command type define 
#define CMD_TYPE_NONE       ((Byte)0)
#define CMD_TYPE_VER_CONST  ((Byte)1)
#define CMD_TYPE_VER_VER    ((Byte)2)

//! cmp result type
#define CMP_GREAT						((Int8)1)
#define CMP_EQUAL						((Int8)0)
#define CMP_LESS						((Int8)-1)

#define TEST_EQUAL					((Int8)0)
#define TEST_NOT_EQUAL			~TEST_EQUAL


//! command set define 
#define CMD_END							((Byte)0)
#define CMD_CLEARVER				((Byte)1)
#define CMD_SET							((Byte)2)
#define CMD_CMP							((Byte)3)
#define CMD_AND							((Byte)4)
#define CMD_OR							((Byte)5)
#define CMD_NOT							((Byte)6)
#define CMD_TEST						((Byte)7)
#define CMD_JMP							((Byte)8)
#define CMD_JE							((Byte)9)
#define CMD_JNE							((Byte)10)
#define CMD_JG							((Byte)11)
#define CMD_JGE							((Byte)12)
#define CMD_JB							((Byte)13)
#define CMD_JBE							((Byte)14)
#define CMD_ADD							((Byte)15)
#define CMD_SUB							((Byte)16)
#define CMD_PLAY						((Byte)17)
#define CMD_PLAYRANDOM			((Byte)18)
#define CMD_PLAYNUMBER			((Byte)19)

Bool  game_initial                  ( void );
void  in_game                       ( void );
void  check_game_time               ( void );
void  game_timeout_deal             ( void );
void  init_current_game             ( void );
void  play_timer_tip                ( void );
void  game_next                     ( void );
void  game_exit                     ( void );
void  game_initial_1				( void );
void  script_init					( void );
void  script_decode					( void );
#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
