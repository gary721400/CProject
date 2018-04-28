//!
//! @file search_game.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search game.
//!
//! @version 1.53 snd3b-dvk-1_9_0 $Id: search_game.c,v 1.53 2007/04/03 09:49:23 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef _search_game_c_
#define _search_game_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "search.h"
#include "search_game.h"
#include "search_remote.h"
#include "conf\conf_oid.h"
#include "string.h"
//#include "stdlib.h" // Contains Funktion "rand()"
#include "lib_mcu\timer\timer_drv.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "mmi\ap3\mmi_ap3.h"
#include "mmi\player\srv_player.h"
#include "mmi\shared\com_evt.h"
#include "modules\file_system\file.h"
#include "modules\player_ap3\player_ap3.h"
//#include "SSSMP3_Regs.h"
#include "modules\player\player_task.h"
#include "lib_mcu\int\int_drv.h"
#include "sio\com.h"
#include "drivers\oid\oid.h"
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\search\search_task.h"


#pragma DATA = SEARCH_GAME_DATA
#pragma CODE = SEARCH_GAME_CODE

ImageDeclareType( SEARCH_GAME_CODE );

ImageDeclareType( SEARCH_IN_GAME_TEMP_CODE00 );
ImageDeclareType( SEARCH_IN_GAME_TEMP_CODE01 );
ImageDeclareType( SEARCH_IN_GAME_TEMP_CODE02 );
ImageDeclareType( SEARCH_IN_GAME_TEMP_CODE03 );

ImageDeclareType( SEARCH_IN_GAME_CODE );

ImageDeclareType( SEARCH_GAME_SORT_CODE );
ImageDeclareType( SEARCH_GAME_COMBINED_CODE );
ImageDeclareType( SEARCH_GAME_CONNECT_CODE00 );
ImageDeclareType( SEARCH_GAME_CONNECT_CODE01 );

//ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

//ImageDeclareType( MMI_AP4_CONTROL_TEMP_CODE00 );

ImageDeclareType( SEARCH_GAME_TEMP_CODE00 );
ImageDeclareType( SEARCH_DO_GAME_CODE );

ImageDeclareType( SEARCH_RAND_CODE );

ImageDeclareType( COMBINED_GAME_TEMP_CODE00 );
ImageDeclareType( COMBINED_GAME_TEMP_CODE01 );


#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

#define LINK_PAKA                 0x02
#define EXIT_PRN_EN               0x04
#define QUESTION_INDEX_EN         0x08
#define NOT_AUTO_GAME_EN          0x10
#define SONIX_GAME                0x20

//#define MAX_GAME_ANSWER           32

#define SORT_TYPE                 1
#define COMBINED_TYPE             2
#define CONNECT_TYPE              3
#define COUNT_TYPE                4
#define NEW_CONNECT_TYPE          5
#define NEW_SORT_TYPE             6


typedef struct
{
   Coor  Game_Coor;
   U8    Type_Key;
   U8    Index_key;
   U32   Action;
   U8    caption[ Length_Caption ];        //no use, not read
}Game_key_Table;


typedef struct
{
   U8   Index_Wrong_Answer;
   U32  Act_Wrong_Answer;
}Game_Wrong_Answer_Table;


//_____ D E F I N I T I O N ________________________________________________
//_MEM_TYPE_SLOW_ Game_key_Table      Current_GameKey;
_MEM_TYPE_SLOW_ U8                  Current_GameKeyIndex[ MAX_GAME_ANSWER ];
_MEM_TYPE_SLOW_ Game_concent_Table  Current_GameKeyConcent[ MAX_GAME_ANSWER ];

//_MEM_TYPE_SLOW_ U8                  GameCount;//s
//_MEM_TYPE_SLOW_ U8                  ErrorCount;
//_MEM_TYPE_SLOW_ U8                  RightNumber;//s
//_MEM_TYPE_SLOW_ U16                 CurrentGame;//s
//#define CurrentGame                 Current_Coor.Y0

_MEM_TYPE_SLOW_ U8					   Wrong_Answered_count;

_MEM_TYPE_SLOW_ U8                  AnswerCount;
//_MEM_TYPE_SLOW_ U8                  AnswerIndex;//s
//_MEM_TYPE_SLOW_ U8                  AnswerCur;//s
//_MEM_TYPE_SLOW_ U8                  CurrentAnswerCount;//s
//_MEM_TYPE_SLOW_ U8                  ErrCur;//s
//_MEM_TYPE_SLOW_ U8                  Answered[ MAX_GAME_ANSWER ];
//_MEM_TYPE_SLOW_ U8                  Answered[(( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 ];//s
_MEM_TYPE_SLOW_ U8                  iCur;
_MEM_TYPE_SLOW_ U8                  QuestionIndex;
_MEM_TYPE_SLOW_ U32                 QuestionSpeech;
//_MEM_TYPE_SLOW_ U8                  HintAnswerIndex;//s

#define GAME_MAX_RANGE_RAND         4       // 32 files
//_MEM_TYPE_SLOW_ U8                  g_game_rand_tab[ GAME_MAX_RANGE_RAND ];//s

//***********************************************
_MEM_TYPE_SLOW_   Game_Table              Current_GameNews;
_MEM_TYPE_SLOW_   Game_Project_Table      Current_Game;
_MEM_TYPE_SLOW_   U32                     GameTipTick;
_MEM_TYPE_SLOW_   U32                     GameTimeOverTick;
_MEM_TYPE_SLOW_   U8                      GameProjectExternNum;
_MEM_TYPE_SLOW_   U32                     Addr_Count_Game;
_MEM_TYPE_SLOW_   U32                     Addr_Game_Wrong_Answer;

//_MEM_TYPE_SLOW_   GAME_STATUS             bGameStatus = { 0 };//s

//_MEM_TYPE_SLOW_   GAME_BIT_NEW            game_bits = { 0 };//s

_MEM_TYPE_SLOW_   Game_Wrong_Answer_Table Game_Wrong_Answer;

//Bool  game_remote_search = FALSE;

_MEM_TYPE_SLOW_   Game_key_Table    gkt_arry;

//! script varialbe define
_MEM_TYPE_SLOW_   Uint32         Script_Address;

_MEM_TYPE_SLOW_   Uint32         ScriptCurrentAddress;
_MEM_TYPE_SLOW_   SCRIPT_CMD     ScriptCmd;
_MEM_TYPE_SLOW_   SCRIPT_PARA    ScriptPara;
_MEM_TYPE_SLOW_   Uint16         ScriptOffsetAddress;
_MEM_TYPE_SLOW_   Uint16         ScriptNumber;
_MEM_TYPE_SLOW_   Int8           ScriptTest;

_MEM_TYPE_SLOW_   Uint16         ScriptVariable[ SCRIPT_VARIABLE_NUMBER ];


_MEM_TYPE_SLOW_   U8             CurrentKey;  //current menu key

//                  Bool           gl_key_press = FALSE;

//_MEM_TYPE_SLOW_   U8             lettle_count;

_MEM_TYPE_SLOW_   U8             GameState;

_MEM_TYPE_SLOW_   U8             word_count;
_MEM_TYPE_SLOW_   U8             word[ WORD_MAX ];

//_____ D E C L A R A T I O N ______________________________________________
void  game_rand_init                ( void );
Bool  game_rand_table_is_full       ( void );
void  compare_coordinate            ( void );
//void  answer_logic_normal           ( U32 Addr );
void  answer_logic_normal           ( /*U8 _MEM_TYPE_SLOW_ *Addr*/void );
void  game_wrong                    ( U32 AnswerSpeech );
Bool  check_question_index          ( void );
void  play_wrong_hint               ( void );
void  play_exit_speech              ( void );
void  check_exit_this               ( U8 CurCount );
//void  answer_logic_converse         ( U32 Addr );
void  answer_logic_converse         ( /*U8 _MEM_TYPE_SLOW_ *Addr*/void );
void  find_first_valid_control      ( void );
U8    find_first_unanswered         ( void );
U8    find_first_even               ( void );
U8    find_first_odd                ( void );
U8    check_even_answered           ( void );
U8    check_odd_answered            ( void );
Bool  check_odd_position            ( void );
Bool  check_answer_list             ( void );
void  push_back                     ( U8 index );
Bool  game_defaultTab_action_deal   ( void );
void  game_key_table_search_deal    ( void );
void  game_normal_sort_deal         ( void );
void  game_normal_combined_deal     ( void );
void  game_normal_connect_deal1     ( void );
void  game_normal_connect_deal2     ( void );
Bool  sort_game_answer_wrong_search ( void );
Bool  game_answer_wrong_search      ( void );
Bool  combind_game_answer_wrong_search ( void );
void  reread_deal                   ( void );
void  combind_game_wrong            ( void );

void  script_play                   ( void );
void  script_play_number            ( Uint16 number );
void  script_play_random            ( void );
void  script_get_cmd                ( void );
void  script_get_number             ( void );
void  script_get_para               ( void );
void  script_get_address            ( void );
void  script_jmp                    ( void );
void  clear_script_variable         ( void );

extern   void  srand                ( U32 s );
extern   U16   rand                 ( void );

#if 1
//***********************************************

void game_rand_init( void )
{
   // Init rand table
   memset( g_game_rand_tab, 0, GAME_MAX_RANGE_RAND );
}


Bool game_rand_table_is_full( void )
{
   //U8 _MEM_TYPE_SLOW_ i;
   U8 _MEM_TYPE_SLOW_ number;
   if( Current_GameNews.Num_Game > GAME_MAX_RANGE_RAND * 8 )
      number = GAME_MAX_RANGE_RAND * 8;
   else
      number = Current_GameNews.Num_Game;
   //for( i = 0; i < number; i++ )
   while( number )
   {
      number--;
      //if( 0 == ( g_game_rand_tab[ number / 8 ]&( 1 << ( number % 8 ))))
      if( 0 == ( g_game_rand_tab[ number >> 3 ]&( 1 << ( number & 0x07 ))))
      {
         return FALSE;
      }
   }
   return TRUE;
}


U16 get_game_rand( void )
{
   _MEM_TYPE_SLOW_ U16 m;
   _MEM_TYPE_SLOW_ U8 i;

   SDKLoadCode( SEARCH_RAND_CODE );
   srand( timer_get_time());
   while( 1 )
   {
      if( game_rand_table_is_full())
         game_rand_init();
      m = rand() % Current_GameNews.Num_Game;
      //i = m %( GAME_MAX_RANGE_RAND * 8 );
      //if( 0 == ( g_game_rand_tab[ i / 8 ]&( 1 << ( i % 8 ))))
      i = m &( GAME_MAX_RANGE_RAND * 8 - 1 );
      if( 0 == ( g_game_rand_tab[ i >> 3 ]&( 1 << ( i & 0x07 ))))
      {
         //g_game_rand_tab[ i / 8 ] |= 1 << ( i % 8 ); // Update table
         g_game_rand_tab[ i >> 3 ] |= 1 << ( i & 0x07 ); // Update table
         break;   // A no used position find
      }
   }
   return m;
}


#pragma CODE = SEARCH_GAME_TEMP_CODE00

// initial game information
Bool game_initial( void )
{
   if( !News_Control.Point )
      return FALSE;
   //read_data_from_nf( News_Control.Point, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews, sizeof( Game_Table ));
   read_data_from_nf( News_Control.Point, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews, 23 );
   //read_data_from_nf( News_Control.Point + 12, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Num_Every_Game, sizeof( U8 ));
   //read_data_from_nf( News_Control.Point + 13, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Num_Wrong_Game, sizeof( U8 ));
   //read_data_from_nf( News_Control.Point + 14, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Act_Right, 9 );
   //read_data_from_nf( News_Control.Point + 22, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Fuction_Question, sizeof( U8 ));
   read_data_from_nf( News_Control.Point + 23, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Time, 7 );
   //read_data_from_nf( News_Control.Point + 29, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Mast_Game, sizeof( U8 ));
   read_data_from_nf( News_Control.Point + 30, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Point_Extern_Game_Project, sizeof( U32 ));

   if( All_Control.Version >= 7 && Current_GameNews.Point_Extern_Game_Project )
      read_data_from_nf( Current_GameNews.Point_Extern_Game_Project, ( U8 _MEM_TYPE_SLOW_ *)&GameProjectExternNum, sizeof( U8 ));
   
   if( !Current_GameNews.Point_Game_Project )
      return FALSE;
   if( !Current_GameNews.Num_Game )
      return FALSE;
   if( !Current_GameNews.Point_Game_keyTable )
      return FALSE;
   if( !Current_GameNews.Num_Game_keyTable )
      return FALSE;
   if( !Current_GameNews.Num_Every_Game )
      return FALSE;

   GameCount = Current_GameNews.Num_Every_Game;
   //ErrorCount = Current_GameNews.Num_Wrong_Game;
   RightNumber = 0;
   CurrentGame = 0;
   game_rand_init();

   GameTipTick = 0;
   GameTimeOverTick = 0;
   /*bTipPlay = FALSE;
   bGameTimeOverPlay = FALSE;
	bTimerEnable = FALSE;
   bTimeOverEnable = FALSE;
   bNewLink = FALSE;
   bGameFirst = TRUE;
   bGameSpeech = TRUE;*/
   bGameStatus.value = 0x1800u;//0x0018u;
   
   /*bNotAutoNextGame = FALSE;
   bGamePrev = FALSE;
   bExitGamePRN = FALSE;
   bQuestion = FALSE;*/
   game_bits.value = 0x0800u;//0x10u;
   
   if( Current_GameNews.Time )
   {
      bTimerEnable = TRUE;
      GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
   }
   /*else
      bTimerEnable = FALSE;*/

   if( Current_GameNews.Fuction_Question == GAME_TYPE_REV )
      bTimeOverEnable = FALSE;
   else
   {	
      if( Current_GameNews.OverTime_Game )
      {
         bTimeOverEnable = TRUE;
         GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
      }
      /*else
         bTimeOverEnable = FALSE;*/
   }
   if( Current_GameNews.Mast_Game & LINK_PAKA )
      bNewLink = TRUE;
   /*else
      bNewLink = FALSE;*/
   if( Current_GameNews.Mast_Game & EXIT_PRN_EN )
      bExitGamePRN = TRUE;
   /*else
      bExitGamePRN = FALSE;*/
   if( Current_GameNews.Mast_Game & QUESTION_INDEX_EN )
      bQuestion = TRUE;
   /*else
      bQuestion = FALSE;*/
   if( Current_GameNews.Mast_Game & NOT_AUTO_GAME_EN )
      bNotAutoNextGame = TRUE;
   /*else
      bNotAutoNextGame = FALSE;*/
   game_next();

   initial_console();
   return TRUE;
}


#pragma CODE = SEARCH_GAME_CODE

//load next game information
void game_next( void )
{
   if( !GameCount )
   {
      game_exit();
      return;
	}
	else
   {
      if( bNotAutoNextGame )
      {
         bInGame = FALSE;
         return;	
      }
      GameCount--;
   }
   switch( Current_GameNews.Fuction_Question )
   {
      case GAME_TYPE_RAND:
      CurrentGame = get_game_rand();
      /*init_current_game();
      if( bGameSpeech )
         ( void )act_do( Current_Game.Act_Game_Concent, 0 );	
      break;*/
      goto current_game_init;

      case GAME_TYPE_SEQU:
      if( bGameFirst )
      {
         bGameFirst = FALSE;
         /*init_current_game();
         if( bGameSpeech )
            ( void )act_do( Current_Game.Act_Game_Concent, 0 );
         break;*/
         goto current_game_init;
      }
      if( bGamePrev )
      {
         if( CurrentGame )
         {
            CurrentGame--;
         }
         else
         {
            CurrentGame = Current_GameNews.Num_Game - 1;
         }			
      }
      else
      {
         if(CurrentGame <( Current_GameNews.Num_Game - 1 ))
         {
            CurrentGame++;
         }
         else
         {
            CurrentGame = 0;
         }				
      }
current_game_init:
      init_current_game();
      if( bGameSpeech )
         ( void )act_do( Current_Game.Act_Game_Concent, 0 );
      break;

      /*case GAME_TYPE_REV:
      bInGame = TRUE;
      break;*/

      case GAME_TYPE_USER:
      bSubject = FALSE;
      case GAME_TYPE_REV:
      bInGame = TRUE;
      break;
   }
}


void read_game_project_table( U32 Address, Game_Project_Table *buf )
{
   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&buf->Num_Key, sizeof( U8 ));
   read_data_from_nf( Address + 1, ( U8 _MEM_TYPE_SLOW_ *)&buf->Addr_Key_Index, 9 );
   //read_data_from_nf( Address + 9, ( U8 _MEM_TYPE_SLOW_ *)&buf->Type_Game, 5 );
   read_data_from_nf( Address + 10, ( U8 _MEM_TYPE_SLOW_ *)&buf->Act_Game_Concent, 5 );
   //read_data_from_nf( Address + 14, ( U8 _MEM_TYPE_SLOW_ *)&buf->Index_Game, 9 );
   read_data_from_nf( Address + 15, ( U8 _MEM_TYPE_SLOW_ *)&buf->Act_Right, 9 );
   //read_data_from_nf( Address + 23, ( U8 _MEM_TYPE_SLOW_ *)&buf->Num_Answer, sizeof( U8 ));
}


//initial current game
void init_current_game( void )
{
   _MEM_TYPE_SLOW_ U32 Address;
   //_MEM_TYPE_SLOW_ U8 i;
   Address = Current_GameNews.Point_Game_Project + CurrentGame * 24;
   //read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Current_Game, sizeof( Game_Project_Table ));
   read_game_project_table( Address, &Current_Game );

   if( All_Control.Version >= 7 && Current_GameNews.Point_Extern_Game_Project )
   {
      read_data_from_nf( Current_GameNews.Point_Extern_Game_Project + 1
                           + ( U32 )CurrentGame * ( U32 )GameProjectExternNum,
                              ( U8 _MEM_TYPE_SLOW_ *)&Addr_Count_Game, sizeof( U32 ));

      if( All_Control.Version >= 12 )
      {
         read_data_from_nf( Current_GameNews.Point_Extern_Game_Project + 1 + sizeof( U32 )
                              + ( U32 )CurrentGame * ( U32 )GameProjectExternNum,
                                 ( U8 _MEM_TYPE_SLOW_ *)&Addr_Game_Wrong_Answer, sizeof( U32 ));
      }
   }
   
   AnswerCount = Current_Game.Num_Key;
   if( AnswerCount > MAX_GAME_ANSWER )
      return;
   AnswerIndex = 0;
   AnswerCur = 0;
   CurrentAnswerCount = 0;
   ErrCur = 0;
   HintAnswerIndex = 0;
   bAnswerWrong = FALSE;
   //bGameClicked = FALSE;
   /*bWaitSubject = TRUE;
   bInGame = TRUE;*/
   bGameStatus.value |= 0x8010u;//0x0801u;
   read_data_from_nf( Current_Game.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyIndex, sizeof( U8 )* AnswerCount );
   read_data_from_nf( Current_Game.Addr_key_Concent, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyConcent, sizeof( Game_concent_Table )* AnswerCount );
   memset( Answered, 0, (( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 );
   memset( Wrong_Answered, 0, (( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 );

   //Wrong_Answered_count = 0;
   count_type_num = 0;
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE01

void check_exit_this( U8 CurCount )
{
   Bool bExit = FALSE;
   if( Current_Game.Num_Answer )
   {
      if( CurCount >= Current_Game.Num_Answer )
         bExit = TRUE;
   }
   else
   {
      if( CurCount >= AnswerCount )
         bExit = TRUE;
   }
   if( bExit )
   {
      RightNumber++;
      play_exit_speech();		
      bGameSpeech = TRUE;		
      game_next();		
   }
}


#pragma CODE = SEARCH_GAME_CODE

//game exit
void game_exit( void )
{
   if( !bExitGameAction )
   {
      if( bWrong )
      //if( ErrCur >= Current_GameNews.Num_Wrong_Game )
      {
         ( void )act_do( Current_GameNews.Act_Havewrong, 0 );
      }
      else
      {
         if( RightNumber >= Current_GameNews.Num_Every_Game )
            ( void )act_do( Current_GameNews.Act_Right, 0 );
      }
   }
   bExitGameAction = FALSE;
   
   if( bExitGamePRN )
      play_timer_tip();
   find_first_valid_control();
   /*bWrong = FALSE;
   bTimeOverEnable = FALSE;
   bTimerEnable = FALSE;*/
   bGameStatus.value &= ~0x4300u;//0x00C2u;
   bNotAutoNextGame = FALSE;

   bGameClicked = FALSE;

   game_exit_to_control = TRUE;
}


#pragma CODE = GAME_EXIT_TO_CONTROL_CODE

void exit_game_to_control( void )
{
   game_exit_to_control= FALSE;
/*   
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 22, 0, 87, 8 );
   lcd_goto_xy( 52, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"READ" );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 15, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );
*/   
   mmi_ap3_display_icon(FALSE);
   /*
   switch( ap3_mode )
   {
      case SINGLE_PLAY_MODE:
      lcd_draw_picture( 98-6, 0, IDP_AP4_SINGLE );
      break;

      case SEQUENCE_PLAY_MODE:
      lcd_draw_picture( 98-6, 0, IDP_AP4_SEQUENCE );
      break;
   }
   */
   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
   {
      SDKLoadCode( MMI_AP4_CONTROL_TEMP_CODE00 );
      display_control_caption();
   }
}


#pragma CODE = SEARCH_IN_GAME_CODE

//search coordinate area in game mode
void in_game( void )
{
   //_MEM_TYPE_SLOW_ U32 Address;
   //_MEM_TYPE_SLOW_ U16 Number;
   //_MEM_TYPE_SLOW_ Game_key_Table gkt;

   ///_MEM_TYPE_SLOW_ Game_key_Table gkt_arry;
   //U8  _MEM_TYPE_SLOW_ j, n;
   //U16 _MEM_TYPE_SLOW_ i;
   
   if( !bInGame )
   {
      SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE02 );
      ( void )game_defaultTab_action_deal();
#if 0
      U32 kuang_adress, action_address;
      switch( Coor_Type )
      {
         case IMMOBILITY_COOR_MODE:
         if( !News_Go.Point_DefaultTab )
         {
            break;
         }
         kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
         read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
         if( act_do( action_address, 1 ))
         {
            return;
         }
         break;

         case MOBILE_COOR_MODE:
         case MIX_COOR_MODE:
         if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
         {
            kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
            if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
            {
               kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
               read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
               if( act_do( action_address, 1 ))
               {
                  return;
               }
            }
         }
         break;
      }
#endif
      return;
   }
   
   //Address = Current_GameNews.Point_Game_keyTable;
   //if( !Address )
   if( !Current_GameNews.Point_Game_keyTable )
   {
      return;
   }
   
   /*Number = Current_GameNews.Num_Game_keyTable;
   if( !Number )
   {
      return;
   }
   i = Number / length_gkt;
   j = Number % length_gkt;*/
   if( !Current_GameNews.Num_Game_keyTable )
      return;
   //i = Current_GameNews.Num_Game_keyTable / length_gkt;
   //j = Current_GameNews.Num_Game_keyTable % length_gkt;
   bSearchFound = FALSE;
   /*if( i == 0 )
   {
      read_data_from_nf( Address, ( U8 *)&gkt_arry, ( U16 )sizeof( Game_key_Table )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         switch( Coor_Type )
         {
            case MOBILE_COOR_MODE:
            case MIX_COOR_MODE:
            if(( X_Coor >= gkt_arry[ n ].Game_Coor.X0)
               &&( Y_Coor >= gkt_arry[ n ].Game_Coor.Y0 )
                  &&( X_Coor <= gkt_arry[ n ].Game_Coor.X1 )
                     &&( Y_Coor <= gkt_arry[ n ].Game_Coor.Y1 ))
            {
               bSearchFound = TRUE;
               goto in_game_next;
            }
            break;

            case IMMOBILITY_COOR_MODE:
            if( Current_Index == gkt_arry[ n ].Game_Coor.X0 )
            {
               bSearchFound = TRUE;
               goto in_game_next;
            }
            break;
         }
      }
   }
   else*/
   //{
      SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE03 );
      game_key_table_search_deal();
#if 0
      for( i = 0; i < Current_GameNews.Num_Game_keyTable; i++ )
      {
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&gkt_arry, sizeof( Game_key_Table ));
         //for( n = 0; n < length_gkt; n++ )
         {
            switch( Coor_Type )
            {
               case MOBILE_COOR_MODE:
               case MIX_COOR_MODE:
               if(( X_Coor >= gkt_arry.Game_Coor.X0)
                  &&( Y_Coor >= gkt_arry.Game_Coor.Y0 )
                     &&( X_Coor <= gkt_arry.Game_Coor.X1 )
                        &&( Y_Coor <= gkt_arry.Game_Coor.Y1 ))
               {
                  bSearchFound = TRUE;
                  goto in_game_next;
               }
               break;

               case IMMOBILITY_COOR_MODE:
               if( Current_Index == gkt_arry.Game_Coor.X0 )
               {
                  bSearchFound = TRUE;
                  goto in_game_next;
               }
               break;
            }
         }
         Address += ( U32 )30;
      }
#endif
   //}

in_game_next:
   //if( All_Control.Mask_Gloablext & AP3_TO_AEB_FLAG )
   {
      if( !bSearchFound )
      {
         SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE02 );
         if( game_defaultTab_action_deal())
            return;
#if 0
         U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
         switch( Coor_Type )
         {
            case IMMOBILITY_COOR_MODE:
            if( !News_Go.Point_DefaultTab )
            {
               break;
            }
            kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
            read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
            if( act_do( action_address, 1 ))
            {
               return;
            }
            break;

            case MOBILE_COOR_MODE:
            case MIX_COOR_MODE:
            if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
            {
               kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
               if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
               {
                  kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
                  read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
                  if( act_do( action_address, 1 ))
                  {
                     return;
                  }
               }
            }
            break;

            /*case MIX_COOR_MODE:
            if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
            {
               kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
               read_data_from_nf( kuang_adress, ( U8 *)&action_address, sizeof( U32 ));
               if( act_do( action_address, 1 ))
               {
                  return;
               }
            }
            break;*/
         }
#endif
      }
   }
   switch( Current_GameNews.Fuction_Question )
   {
      case GAME_TYPE_REV:
      SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE00 );
      answer_logic_converse(/*( U8 _MEM_TYPE_SLOW_ *)&gkt_arry */);
      break;

      case GAME_TYPE_SEQU:
      case GAME_TYPE_RAND:
      SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE01 );
      answer_logic_normal(/*( U8 _MEM_TYPE_SLOW_ *)&gkt_arry */);
      break;

      case GAME_TYPE_USER:
      if( bSubject )
      {
         SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE01 );
         answer_logic_normal(/*( U8 _MEM_TYPE_SLOW_ *)&gkt_arry */);
      }
      else
      {
         if( bSearchFound )
         {
            /*gkt = gkt_arry[ n ];
            act_do( gkt.Action, 0 );
            QuestionIndex = gkt.Index_key;*/
            ( void )act_do( gkt_arry.Action, 0 );
            QuestionIndex = gkt_arry.Index_key;
            QuestionSpeech = 0;
            SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE01 );
            ( void )check_question_index();
         }
         else
         {
            //haven't get subject,play null speech
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         }				
      }
      break;
   }
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE01

//judge the answer index after find answer
void answer_logic_normal( /*U8 _MEM_TYPE_SLOW_ *Addr*/void )
{
   //_MEM_TYPE_SLOW_ Game_key_Table gkt;
   //Bool bPass;
   //_MEM_TYPE_SLOW_ U8 i;

   //_MEM_TYPE_SLOW_ U16 temp_num;
   //_MEM_TYPE_SLOW_ U32 temp_add;

   //memmove( &gkt, Addr, sizeof( Game_key_Table ));
   switch( Current_Game.Type_Game )
   {
      case SORT_TYPE:
      case NEW_SORT_TYPE:
      SDKLoadCode( SEARCH_GAME_SORT_CODE );
      game_normal_sort_deal();
#if 0
      if( bSearchFound )
      {
         //memmove( &gkt, Addr, sizeof( Game_key_Table ));
         iCur = gkt_arry.Index_key;
         QuestionIndex = gkt_arry.Index_key;
         QuestionSpeech = gkt_arry.Action;
         if( iCur != Current_GameKeyIndex[ AnswerCur ])
         {
            game_wrong( gkt_arry.Action );		
            return;
         }
         else
         {
            act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
            ErrCur = 0;
            AnswerCur++;
            HintAnswerIndex = AnswerCur;
            check_exit_this( AnswerCur );
         }
      }
      else
      {
         QuestionIndex = 0;
         game_wrong( 0 );
      }
#endif
      break;

      case COMBINED_TYPE:
      case COUNT_TYPE:
      SDKLoadCode( SEARCH_GAME_COMBINED_CODE );
      game_normal_combined_deal();
#if 0
      if( bSearchFound )
      {
         //memmove( &gkt, Addr, sizeof( Game_key_Table ));
         iCur = gkt_arry.Index_key;
         QuestionIndex = gkt_arry.Index_key;
         QuestionSpeech = gkt_arry.Action;
         bPass = FALSE;
         for( i = 0; i < AnswerCount; i++ )
         {
            if( iCur == Current_GameKeyIndex[ i ])
            {
               bPass = TRUE;
               break;
            }
         }
         if( bPass )
         {
            // It is in current answer list
            if(( Answered[ i / 8 ]&( 1 << ( i % 8 ))))
            {
               act_do( Current_GameKeyConcent[ i ].Act_Third_Wrong, 0 );
            }
            else
            {
               act_do( Current_GameKeyConcent[ i ].Act_Right, 0 );
               Answered[ i / 8 ] |= 1 << ( i % 8 );
               AnswerCur++;
               ErrCur = 0;
               check_exit_this( AnswerCur );
            }
         }
         else
         {
            HintAnswerIndex = find_first_unanswered();
            game_wrong( gkt_arry.Action );
         }
      }
      else
      {
         HintAnswerIndex = find_first_unanswered();
         QuestionIndex = 0;
         game_wrong( 0 );
      }
#endif
      break;

      case CONNECT_TYPE:
      case NEW_CONNECT_TYPE:
      if( bWaitSubject )
      {
         SDKLoadCode( SEARCH_GAME_CONNECT_CODE00 );
         game_normal_connect_deal1();
#if 0
         if( bSearchFound )
         {
            // is in answer index list
            //memmove( &gkt, Addr, sizeof( Game_key_Table ));
            iCur = gkt_arry.Index_key;
            QuestionIndex = 0;
            if( check_answer_list())
            {
               // It is in current suject answer list
               if( check_odd_position())
               {
                  //It is in odd position
                  i = check_odd_answered();
                  if( i >( AnswerCount - 1 ))
                  {
                     /*all odd is clicked,find out first odd 	
                     and play first odd match even wrong hint */
                     i = find_first_odd();
                     HintAnswerIndex = i + 1;
                     game_wrong( 0 );
                  }
                  else
                  {
                     /* find at least one odd have not link,
                     play answer voice+right speech,
                     new link have get */
                     ErrCur = 0;
                     AnswerCur = i;
                     HintAnswerIndex = i;
                     bWaitSubject = FALSE;
                     act_do( gkt_arry.Action, 0 );
                     act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
                  }
               }
               else
               {
                  //else it is in even position
                  i = check_even_answered();
                  if( i > AnswerCount )
                  {
                     /*all even is clicked,find out first even 
                     and play this even wrong speech*/
                     HintAnswerIndex = find_first_even();
                     if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
                     {
                        game_wrong( 0 );
                     }
                     else
                     {
                        act_do( gkt_arry.Action, 0 );
                     }
                  }
                  else
                  {
                     /* have haven't clicked even,
                     play current answer voice */
                     act_do( gkt_arry.Action, 0 );
                  }
               }
            }
            else
            {
               // It isn't in current subject answer list
               // click wrong answer index
               HintAnswerIndex = AnswerCur;
               act_do( gkt_arry.Action, 0 );
            }
         }
         else
         {
            // It Isn't in answer list
            act_do( News_Go.Action_NoVoice, 0 );
         }
#endif
      }
      else
      {
         SDKLoadCode( SEARCH_GAME_CONNECT_CODE01 );
         game_normal_connect_deal2();
#if 0
         if( bSearchFound )
         {
            // is in answer list
            //memmove( &gkt, Addr, sizeof( Game_key_Table ));
            iCur = gkt_arry.Index_key;
            QuestionIndex = 0;
            if( iCur == Current_GameKeyIndex[ AnswerCur + 1 ])
            {
               /* It is matched answer,mask finished partnership,
               play right speech&resume to state 1. */
               ErrCur = 0;
               if( bNewLink )
               {
                  Answered[ AnswerCur / 8 ] |= 1 <<( AnswerCur % 8 );
                  Answered[( AnswerCur + 1 )/ 8 ] |= 1 <<(( AnswerCur + 1 )% 8 );
                  CurrentAnswerCount += 2;
               }
               bWaitSubject = TRUE;
               act_do( Current_GameKeyConcent[ AnswerCur + 1 ].Act_Right, 0 );

               if( Current_Game.Type_Game == NEW_CONNECT_TYPE
                  && All_Control.Version >= 7 )
               {
                  if( Addr_Count_Game )
                  {
                     read_data_from_nf( Addr_Count_Game,( U8 _MEM_TYPE_SLOW_ *)&temp_num, sizeof( temp_num ));
                     if( temp_num && bNewLink )
                     {
                        if(( CurrentAnswerCount / 2 )< temp_num )
                        {
                           temp_add = Addr_Count_Game +(( CurrentAnswerCount / 2 )- 1 )* 9 + 2;
                           write_media_addr( temp_add, 0 );
                        }
                     }
                  }
               }
               check_exit_this( CurrentAnswerCount );
            }
            else
            {
               // It isn't correct answer.
               if( check_answer_list())
               {
                  // It is in current answer list
                  if( check_odd_position())
                  {
                     //It is in odd position
                     i = check_odd_answered();
                     if( i >( AnswerCount - 1 ))
                     {
                        //all odd is clicked,play wrong speech

                        if( Current_Game.Type_Game == NEW_CONNECT_TYPE )
                        {
                           if(( ErrCur + 1 )>= Current_GameNews.Num_Wrong_Game )
                              bWaitSubject = FALSE;;
                        }

                        act_do( gkt_arry.Action, 0 );
                        HintAnswerIndex = AnswerCur;
                        game_wrong( 0 );
                     }
                     else
                     {
                        /* find at least one odd have not link,
                        new link have get,2 state */
                        ErrCur = 0;
                        AnswerCur = i;
                        HintAnswerIndex = i;
                        bWaitSubject = FALSE;
                        act_do( gkt_arry.Action, 0 );
                        act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
                     }
                  }
                  else
                  {
                     //is in even position,but not correct answer
                     if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
                     {
                        act_do( gkt_arry.Action, 0 );
                        HintAnswerIndex = AnswerCur;
                        game_wrong( 0 );
                     }
                     else
                     {
                        HintAnswerIndex = AnswerCur;
                        if(( ErrCur + 1 )>= Current_GameNews.Num_Wrong_Game )
                        {
                           bWaitSubject = TRUE;
                        }
                        game_wrong( 0 );
                     }
                  }
               }
               else
               {
                  // click wrong answer index
                  if( Current_Game.Type_Game == NEW_CONNECT_TYPE )
                     act_do( gkt_arry.Action, 0 );
                  HintAnswerIndex = AnswerCur;							
                  game_wrong( 0 );
               }
            }
         }
         else
         {
            // Isn't in answer list
            QuestionIndex = 0;
            HintAnswerIndex = AnswerCur;					
            game_wrong( 0 );
         }
#endif
      }
      break;
   }
}


//game answer wrong
void game_wrong( U32 AnswerSpeech )
{
   Bool no_play_wrong_hint_flag = FALSE;
   
   if(( Current_GameNews.Mast_Game & SONIX_GAME )
      &&(( Current_GameNews.Num_Wrong_Game - ErrCur )== 1 ))
   {
      no_play_wrong_hint_flag = TRUE;
   }
   
   if( bQuestion )
   {
      if( check_question_index())
         return;
	}
   switch( Current_Game.Type_Game )
   {
      case SORT_TYPE:
      case NEW_SORT_TYPE:
      ( void )act_do( AnswerSpeech, 0 );
      /*if( no_play_wrong_hint_flag )
         break;
      play_wrong_hint();
      break;*/

      case CONNECT_TYPE:
      case NEW_CONNECT_TYPE:
      if( no_play_wrong_hint_flag )
         break;
      play_wrong_hint();
      break;

      case COMBINED_TYPE:
      case COUNT_TYPE:
      ( void )act_do( AnswerSpeech, 0 );
      if( no_play_wrong_hint_flag )
         break;
      if( !ErrCur )
      {
         ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      }
      else
      {
         if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
         {
            ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
         }
         else
         {
            ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
         }
      }
      break;
   }
   ErrCur++;

#if 0
   if(( ErrCur >= Current_GameNews.Num_Wrong_Game )
      &&( Current_Game.Type_Game != NEW_CONNECT_TYPE ))
   {
      /*bWrong = TRUE;
      bAnswerWrong = TRUE;
      bGameSpeech = TRUE;
      bSearchFound = TRUE;  //Don't search the other table;*/
      bGameStatus.value |= 0x5404u;//0x202A;
      ( void )act_do( Current_Game.Act_Wrong, 0 );
      bSubject = FALSE;   //exit user define
      if( Current_GameNews.Mast_Game & 0x01 )
         game_exit();
      else
         game_next();
   }

   if(( ErrCur >= Current_GameNews.Num_Wrong_Game )
      &&( Current_Game.Type_Game == NEW_CONNECT_TYPE ))
   {
      ErrCur = 0;
   }
#else
   if( ErrCur >= Current_GameNews.Num_Wrong_Game )
   {
      if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
      {
         /*bWrong = TRUE;
         bAnswerWrong = TRUE;
         bGameSpeech = TRUE;
         bSearchFound = TRUE;  //Don't search the other table;*/
         bGameStatus.value |= 0x5404u;//0x202A;
         ( void )act_do( Current_Game.Act_Wrong, 0 );
         bSubject = FALSE;   //exit user define
         if( Current_GameNews.Mast_Game & 0x01 )
            game_exit();
         else
            game_next();
      }
      else
      {
         ErrCur = 0;
      }
   }
#endif
}


Bool check_question_index( void )
{
   _MEM_TYPE_SLOW_ U32 Address;
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ Game_Project_Table tempGameMsg;
   if( !QuestionIndex )
      return FALSE;
   Address = Current_GameNews.Point_Game_Project;
   for( i = 0; i < Current_GameNews.Num_Game; i++ )
   {
      //read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&tempGameMsg, sizeof( Game_Project_Table ));
      read_game_project_table( Address, &tempGameMsg );
      if( tempGameMsg.Index_Game == QuestionIndex )
      {
         CurrentGame = i;
         init_current_game();
         ( void )act_do( QuestionSpeech, 0 );				
         ( void )act_do( Current_Game.Act_Game_Concent, 0 );
         bSubject = TRUE;
         return TRUE;
      }
      Address += 24;
   }
   return FALSE;
}


//play wrong hint
void play_wrong_hint( void )
{
   switch( ErrCur )
   {
      case 0:
      ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      break;

      case 1:
new_sort_wrong_hint:
      if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
      {
         ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      }
      else
      {
         ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
      }
      break;

      default:
      if( Current_Game.Type_Game == NEW_SORT_TYPE )
         goto new_sort_wrong_hint;
      if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Third_Wrong == 0 )
      {
         if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
         {
            ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
         }
         else
         {
            ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
         }
      }
      else
      {
         ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Third_Wrong, 0 );
      }
      break;
   }
}


void play_exit_speech( void )
{
   if( bAnswerWrong )
   {
      ( void )act_do( Current_Game.Act_Wrong, 0 );
   }
   else
   {
      ( void )act_do( Current_Game.Act_Right, 0 );
   }
}


#pragma CODE = SEARCH_GAME_CODE

void play_timer_tip( void )
{
   _MEM_TYPE_SLOW_ U32 Address;
   //_MEM_TYPE_SLOW_ U32 iSpeech;
   _MEM_TYPE_SLOW_ U16 Number;

   if( !News_Book.Point_GameRightAnswer )
      return;
   Address = News_Book.Point_GameRightAnswer;
   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Number, sizeof( U16 ));
   if(( Number == 0 )||( RightNumber >= Number ))
   {
      return;
   }
	
   /*Address += sizeof( U16 )+ sizeof( U32 )* RightNumber;
   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&iSpeech, sizeof( U32 ));
   act_do( iSpeech, 0 );*/
   Address += sizeof( U16 )+( U32 )9 * RightNumber;
   write_media_addr( Address, 0 );
   RightNumber = 0;
}


#pragma CODE = SEARCH_GAME_COMBINED_CODE

//find first match even index
U8 find_first_unanswered( void )
{
   _MEM_TYPE_SLOW_ U8 i;

   for( i = 0; i < AnswerCount; i++ )
   {
      //if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
      if(( Answered[ i >> 3 ]&( 1 <<( i & 0x07 )))== 0 )
      {
         break;
      }
   }
   return i;
}


#pragma CODE = SEARCH_GAME_CONNECT_CODE00

//find first match even index
U8 find_first_even( void )
{
   _MEM_TYPE_SLOW_ U8 i;

   for( i = 1; i < AnswerCount; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         break;
      }
   }
   return i;
}


//#pragma CODE = SEARCH_GAME_CONNECT_CODE00
#pragma CODE = SEARCH_IN_GAME_TEMP_CODE01

//find first match odd
U8 find_first_odd( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for( i = 0; i < AnswerCount - 1; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         break;
      }
   }
   return i;
}


#pragma CODE = SEARCH_GAME_CONNECT_CODE00

//check even answered index
U8 check_even_answered( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for( i = 1; i < AnswerCount; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         //if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
         if(( Answered[ i >> 3 ]&( 1 <<( i & 0x07 )))== 0 )
            break;
      }
   }
   return i;
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE01

//check odd answered index
U8 check_odd_answered( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for(i = 0; i < AnswerCount - 1; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         //if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
         if(( Answered[ i >> 3 ]&( 1 <<( i & 0x07 )))== 0 )
            break;
      }
   }	
   return i;
}


//check current suject answer list
Bool check_odd_position( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   // check answer list in odd position
   for( i = 0; i < AnswerCount - 1; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         return TRUE;
      }
   }
   return FALSE;
}


//check current suject answer list
Bool check_answer_list( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for( i = 0; i < AnswerCount; i++ )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         return TRUE;
      }
   }
   return FALSE;
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE00

//judge the answer index after find answer
void answer_logic_converse( /*U8 _MEM_TYPE_SLOW_ *Addr*/void )
{
   //_MEM_TYPE_SLOW_ Game_key_Table gkt;
   _MEM_TYPE_SLOW_ U8 /*iCur, */j, cindex[ MAX_GAME_ANSWER ];
   _MEM_TYPE_SLOW_ U32 Address;
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ Game_Project_Table iGameMsg;
   Bool bFind;

   //memmove( &gkt, Addr, sizeof( Game_key_Table ));
   if( bSearchFound )
   {
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      ( void )act_do( gkt_arry.Action, 0 );
      /*iCur = gkt.Index_key;
      push_back( iCur );*/
      //push_back( gkt.Index_key );
      Current_GameKeyIndex[ AnswerIndex ] = gkt_arry.Index_key;
      AnswerIndex = ( AnswerIndex + 1 )& 0x1F;
      
      Address = Current_GameNews.Point_Game_Project;
      for( i = 0; i < Current_GameNews.Num_Game; i++ )
      {
         bFind = TRUE;
         //read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&iGameMsg, sizeof( Game_Project_Table ));
         read_game_project_table( Address, &iGameMsg );
         read_data_from_nf( iGameMsg.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&cindex, sizeof( U8 )* iGameMsg.Num_Key );
         for( j = 0; j < iGameMsg.Num_Key; j++ )
         {
            if( Current_GameKeyIndex[( AnswerIndex - iGameMsg.Num_Key + j )& 0x1F ] != cindex[ j ])
            {
               Address += 24;
               bFind = FALSE;
               break;
            }
         }
         if( bFind )
         {
            break;
         }
      }
      if( bFind )
      {
         ( void )act_do( iGameMsg.Act_Game_Concent, 0 );
         RightNumber++;
      }
   }
   else
   {
      ( void )act_do( News_Go.Action_NoVoice, 0 );
   }
}

/*
void push_back( U8 index )
{
   Current_GameKeyIndex[ AnswerIndex ] = index;
   AnswerIndex = ( AnswerIndex + 1 )& 0x1F;
}*/


#pragma CODE = SEARCH_GAME_CODE

//find first valid control function.
void find_first_valid_control( void )
{
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ U32 Address;
   _MEM_TYPE_SLOW_ U32 ControlAddress;
   _MEM_TYPE_SLOW_ Control_Table TempControl;

   for( i = 0; i < News_Go.Num_ControlTab; i++ )
   {
      Address = News_Go.Point_ControlTab + sizeof( U32 )* i;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&ControlAddress, sizeof( U32 ));
      read_data_from_nf( ControlAddress, ( U8 _MEM_TYPE_SLOW_ *)&TempControl.Caption_Control[ 0 ], Length_Caption );
      read_data_from_nf( ControlAddress + 16, ( U8 _MEM_TYPE_SLOW_ *)&TempControl.Point, 9 );
      read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&TempControl.Num_Control, 3 );
      if((( TempControl.Type_Control == SINGLE_TABLE_TYPE ) && TempControl.Point )
         ||( TempControl.Type_Control == SCRIPT_TYPE ))
      {
         News_Control = TempControl;
         Current_Record.Control_num = i;
         Search_Record.news_sys.table_flag = CONTROL_TABLE;
         //Search_Record.news_sys.mode_flag = SINGLE_TABLE;
         Ap3_MenuPath = CONTROL_MODE_MENU_PATH;
         return;
      }
   }
   //Search_Record.news_sys.mode_flag = 0;
   News_Control.Type_Control = 0;
   if( News_Go.Point_DefaultTab )
   {
      if( Coor_Type == IMMOBILITY_COOR_MODE )
      {
         Search_Record.news_sys.table_flag = DEFAULT_TABLE;
         //Search_Record.news_sys.disable_next_key_flag = 1;
         Ap3_MenuPath = CONTROL_MENU_PATH;
         return;
      }
      else
      {
         if( News_Go.Num_DefaultTab )
         {
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
            return;
         }
      }
   }

   if( News_Go.Point_BackgroudTab )
   {
      if( Coor_Type == IMMOBILITY_COOR_MODE )
      {
         Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
         //Search_Record.news_sys.disable_next_key_flag = 1;
         Ap3_MenuPath = CONTROL_MENU_PATH;
         return;
      }
      else
      {
         if( News_Go.Num_BackgroudTab )
         {
            Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
            return;
         }
      }
   }
}


Bool search_eightbytes_coordinate( U32 addr, U16 number )
{
   U16 _MEM_TYPE_SLOW_ m;
   //U32 _MEM_TYPE_SLOW_ Address;
   Coor _MEM_TYPE_SLOW_ coor_arry;
   
   //Address = addr;
   if( !addr || !number )
   {
      return FALSE;
   }
	
   /*if( number == 0 )
   {
      read_data_from_nf( addr, ( U8 *)&coor_arry, ( U16 )sizeof( Coor )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         if(( X_Coor >= coor_arry[ n ].X0 )
            &&( Y_Coor >= coor_arry[ n] .Y0 )
               &&( X_Coor <= coor_arry[ n ].X1 )
                  &&( Y_Coor <= coor_arry[ n ].Y1 ))
         {
            Current_Coor.X0 = n;
            return TRUE;
         }
      }
      return FALSE;
   }
   else*/
   //{
      for( m = 0; m < number; m++ )
      {
         read_data_from_nf( addr, ( U8 _MEM_TYPE_SLOW_ *)&coor_arry, ( U16 )sizeof( Coor ));
         {
            if(( X_Coor >= coor_arry.X0 )
               &&( Y_Coor >= coor_arry.Y0 )
                  &&( X_Coor <= coor_arry.X1 )
                     &&( Y_Coor <= coor_arry.Y1 ))
            {
               Current_Coor.X0 = number;
               return TRUE;
            }
         }
         addr += ( U32 )sizeof( Coor );
      }
   //}
   return FALSE;
}


#pragma CODE = SEARCH_GAME_TEMP_CODE01

void read_sgdb( void )
{
   U8  _MEM_TYPE_SLOW_ search_status;
   U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
   SendString((U8 *)"\r\nsgdb!");
   game_remote_search = FALSE;
   reread_enable_flag = FALSE;
   //g_b_enable_flush = TRUE;
   SDKLoadCode( SEARCH_GAME_CODE );
   if( Current_Index > News_Book.Max_Index_TheBook )
   {
      ( void )act_do( News_Go.Action_NoVoice, 0 );
      return;
   }
   if( Coor_Type == IMMOBILITY_COOR_MODE )
   {
      if(( Current_Index > News_Go.Max_Index_ThePage )
         ||( Current_Index < News_Go.page_info.Min_Index_ThePage ))
      {
         ( void )act_do( News_Go.Action_NoVoice, 0 );
         return;
      }
   }
   /*Search_Record.news_sys.reread_flag 
      = Search_Record.news_sys.repeat_flag
         = Search_Record.news_sys.sequence_flag
            = Search_Record.news_sys.index_normal_flag
               = Search_Record.news_sys.play_record_flag
                  = 0;*/
   Search_Record.value &= ~0x03C1u;//0x81C0u;
   if( Search_Record.news_sys.table_flag == DEFAULT_TABLE )
   {
      switch( Coor_Type )
      {
         case IMMOBILITY_COOR_MODE:
         search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
         break;
         
         case MOBILE_COOR_MODE:
         search_status = DEFAULT_MOBILE_COOR_STATUS;
         //search_status = BACKGROUND_MOBILE_COOR_STATUS;
         break;
         
         case MIX_COOR_MODE:
         search_status = DEFAULT_MIX_COOR_STATUS;
         break;
      }
   }
   else if( Search_Record.news_sys.table_flag == BACKGROUND_TABLE )
   {
      switch( Coor_Type )
      {
         case IMMOBILITY_COOR_MODE:
         search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
         break;
         
         case MOBILE_COOR_MODE:
         search_status = BACKGROUND_MOBILE_COOR_STATUS;
         break;
         
         case MIX_COOR_MODE:
         search_status = BACKGROUND_MIX_COOR_STATUS;
         break;
      }
   }
   else if( Search_Record.news_sys.table_flag == CONTROL_TABLE )
   {
      //if( Search_Record.news_sys.mode_flag == SINGLE_TABLE )
      if( News_Control.Type_Control == SINGLE_TABLE_TYPE )
      {
         switch( Coor_Type )
         {
            case IMMOBILITY_COOR_MODE:
            search_status = CONTROL_IMMOBILITY_COOR_STATUS;
            break;
            
            case MOBILE_COOR_MODE:
            search_status = CONTROL_MOBILE_COOR_STATUS;
            break;
            
            case MIX_COOR_MODE:
            search_status = CONTROL_MIX_COOR_STATUS;
            break;
         }
      }
      //else if( Search_Record.news_sys.mode_flag == GAME_TABLE )
      else if( News_Control.Type_Control == GAME_TYPE )
      {
         //SDKLoadCode( SEARCH_IN_GAME_CODE );
         //in_game();
         game_remote_search = TRUE;
         return;
      }
      else
      {
         return;
      }
   }
   else
   {
      ( void )act_do( News_Go.Action_NoVoice, 0 );
      return;
   }

   while( 1 )
   {
      switch( search_status )
      {
         case CONTROL_IMMOBILITY_COOR_STATUS:
         if(( !News_Go.Point_ControlTab )||( !News_Go.Num_ControlTab )||( !News_Control.Point ))
         {
            search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
            break;
         }
         kuang_adress = News_Control.Point + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
         read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
         if( News_Control.Num_Control )
         {
            if( act_do( action_address, 0 ))
            {
               reread_deal();
               Search_Record.news_sys.index_normal_flag = 1;
               return;
            }
         }
         search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
         break;

         case CONTROL_MOBILE_COOR_STATUS:
         if(( !News_Go.Point_ControlTab )||( !News_Go.Num_ControlTab )||( !News_Control.Point ))
         {
            search_status = DEFAULT_MOBILE_COOR_STATUS;
            break;
         }
         kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
         if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
         {
            kuang_adress = News_Control.Point + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
            read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
            if( News_Control.Num_Control )
            {
               if( act_do( action_address, 0 ))
               {
                  reread_deal();
                  Search_Record.news_sys.index_normal_flag = 1;
                  return;
               }
            }
         }
         search_status = DEFAULT_MOBILE_COOR_STATUS;
         //search_status = BACKGROUND_MOBILE_COOR_STATUS;
         break;
    
         case CONTROL_MIX_COOR_STATUS:
         if(( !News_Go.Point_ControlTab )||( !News_Go.Num_ControlTab )||( !News_Control.Point ))
         {
            search_status = DEFAULT_MIX_COOR_STATUS;
            break;
         }
         kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
         if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
         {
            kuang_adress = News_Control.Point + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
            read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
            if( News_Control.Num_Control )
            {
               if( act_do( action_address, 0 ))
               {
                  reread_deal();
                  Search_Record.news_sys.index_normal_flag = 1;
                  return;
               }
            }
         }
         search_status = DEFAULT_MIX_COOR_STATUS;
         break;
			
         case DEFAULT_IMMOBILITY_COOR_STATUS:
		 SendString((U8 *)"\r\nd_i!");
         if( !News_Go.Point_DefaultTab )
         {
            search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
            break;
         }
         kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
         read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
         if( act_do( action_address, 0 ))
         {
            reread_deal();
            Search_Record.news_sys.index_normal_flag = 1;
            return;
         }
         search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
         break;

         case DEFAULT_MOBILE_COOR_STATUS:
		 	SendString((U8 *)"\r\nd_m!");
         if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
         {
            kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
            if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
            {
               /*tx_send_string("Current_Coor.X0:");
               tx_hex_u16(Current_Coor.X0,1);
               tx_hex_return();*/
               
               kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
               read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
               if( act_do( action_address, 0 ))
               {
                  reread_deal();
                  Search_Record.news_sys.index_normal_flag = 1;
                  return;
               }
            }
         }
         search_status = BACKGROUND_MOBILE_COOR_STATUS;
         break;

         case DEFAULT_MIX_COOR_STATUS:
         if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
         {
            kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
            if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
            {
               kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
               read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
               if( act_do( action_address, 0 ))
               {
                  reread_deal();
                  Search_Record.news_sys.index_normal_flag = 1;
                  return;
               }
            }
         }
         search_status = BACKGROUND_MIX_COOR_STATUS;
         break;

         case BACKGROUND_IMMOBILITY_COOR_STATUS:
         if( !News_Go.Point_BackgroudTab )
         {
            ( void )act_do( News_Go.Action_NoVoice, 0 );
            return;
         }
         //if(( All_Control.Mask_Gloablext & MSK_COOR_MODE )== IMMOBILITY_COOR_MODE )
         {
            kuang_adress = News_Go.Point_BackgroudTab + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
            read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
            if( act_do( action_address, 0 ))
            {
               reread_deal();
               Search_Record.news_sys.index_normal_flag = 1;
               return;
            }
            ( void )act_do( News_Go.Action_NoVoice, 0 );
            return;
         }

         case BACKGROUND_MOBILE_COOR_STATUS:
         case BACKGROUND_MIX_COOR_STATUS:
         if( News_Go.Point_BackgroudCoor && News_Go.Num_BackgroudTab )
         {
            if( search_eightbytes_coordinate( News_Go.Point_BackgroudCoor, News_Go.page_info.Num_Sub ))
            {
               kuang_adress = News_Go.Point_BackgroudTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
               read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
               if( act_do( action_address, 0 ))
               {
                  reread_deal();
                  Search_Record.news_sys.index_normal_flag = 1;
                  return;
               }
            }
         }

         ( void )act_do( News_Go.Action_NoVoice, 0 );
         return;
      }
   }
}


void reread_deal( void )
{
   switch( Current_Act.Type )
   {
      case Type_Play:
      case Type_Play_Title:
      case Type_play_Caption:
      case Type_play_Reread:
      reread_start_time = timer_get_time();
      reread_end_time = 0;
      reread_time = 0;
      if( ap4_read_status == MMI_AP3_RERAED_STATUS )
         reread_enable_flag = TRUE;
      else if( ap4_read_status == MMI_AP3_SIMPLE_STATUS )
      {
         if( Search_Record.news_sys.reread_flag )
            reread_enable_flag = TRUE;
      }
      break;
   }
}

#pragma CODE = SEARCH_IN_GAME_TEMP_CODE02

Bool game_defaultTab_action_deal( void )
{
   U32 kuang_adress, action_address;
   switch( Coor_Type )
   {
      case IMMOBILITY_COOR_MODE:
      if( !News_Go.Point_DefaultTab )
      {
         break;
      }
      kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.page_info.Min_Index_ThePage )* ( U32 )sizeof( U32 );
      read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
      if( act_do( action_address, 1 ))
      {
         return TRUE;
      }
      break;

      case MOBILE_COOR_MODE:
      case MIX_COOR_MODE:
      if( News_Go.Point_DefaultTab && News_Go.Num_DefaultTab )
      {
         kuang_adress = News_Go.Point_PageCoorTable + ( U32 )News_Go.Num_ControlTab *( U32 )sizeof( Coor );
         if( search_eightbytes_coordinate( kuang_adress, News_Go.page_info.Num_Sub ))
         {
            kuang_adress = News_Go.Point_DefaultTab + ( U32 )Current_Coor.X0 * ( U32 )sizeof( U32 );
            read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_ *)&action_address, sizeof( U32 ));
            if( act_do( action_address, 1 ))
            {
               return TRUE;
            }
         }
      }
      break;
   }
   return FALSE;
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE03

void game_key_table_search_deal( void )
{
   _MEM_TYPE_SLOW_ U32 Address;
   U16 _MEM_TYPE_SLOW_ i;

   Address = Current_GameNews.Point_Game_keyTable;
   for( i = 0; i < Current_GameNews.Num_Game_keyTable; i++ )
   {
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&gkt_arry, 30 );
      //for( n = 0; n < length_gkt; n++ )
      {
         switch( Coor_Type )
         {
            case MOBILE_COOR_MODE:
            case MIX_COOR_MODE:
            if(( X_Coor >= gkt_arry.Game_Coor.X0)
               &&( Y_Coor >= gkt_arry.Game_Coor.Y0 )
                  &&( X_Coor <= gkt_arry.Game_Coor.X1 )
                     &&( Y_Coor <= gkt_arry.Game_Coor.Y1 ))
            {
               bSearchFound = TRUE;
               return;
            }
            break;

            case IMMOBILITY_COOR_MODE:
            if( Current_Index == gkt_arry.Game_Coor.X0 )
            {
               bSearchFound = TRUE;
               return;
            }
            break;
         }
      }
      Address += ( U32 )30;
   }
}


#pragma CODE = SEARCH_GAME_SORT_CODE

void game_normal_sort_deal( void )
{
   if( bSearchFound )
   {
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      iCur = gkt_arry.Index_key;
      QuestionIndex = gkt_arry.Index_key;
      QuestionSpeech = gkt_arry.Action;
      if( iCur != Current_GameKeyIndex[ AnswerCur ])
      {
         if( Current_Game.Type_Game == SORT_TYPE )
         {
            if(( All_Control.Version >= 12 )
               && sort_game_answer_wrong_search())
               return;
         }
         else if( Current_Game.Type_Game == NEW_SORT_TYPE )
         {
            _MEM_TYPE_SLOW_ U8 i;
            for( i = 0; i < AnswerCur; i++ )
            {
               if( iCur == Current_GameKeyIndex[ i ])
               {
                  ( void )act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Third_Wrong, 0 );
                  return;
               }
            }
         }
         game_wrong( gkt_arry.Action );
      }
      else
      {
         ( void )act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
         ErrCur = 0;
         AnswerCur++;
         HintAnswerIndex = AnswerCur;
         check_exit_this( AnswerCur );
      }
   }
   else
   {
      QuestionIndex = 0;
      game_wrong( 0 );
   }
}


Bool sort_game_answer_wrong_search( void )
{
   if( game_answer_wrong_search())
   {
      if( !(( Current_GameNews.Mast_Game & SONIX_GAME )
         &&(( Current_GameNews.Num_Wrong_Game - ErrCur )== 1 )))
         ( void )act_do( Game_Wrong_Answer.Act_Wrong_Answer, 0 );
      ErrCur++;
#if 0
      if(( ErrCur >= Current_GameNews.Num_Wrong_Game )
         &&( Current_Game.Type_Game != NEW_CONNECT_TYPE))
      {
         /*bWrong = TRUE;
         bAnswerWrong = TRUE;
         bGameSpeech = TRUE;
         bSearchFound = TRUE;  //Don't search the other table;*/
         bGameStatus.value |= 0x5404u;//0x202A;
         ( void )act_do( Current_Game.Act_Wrong, 0 );
         bSubject = FALSE;   //exit user define
         if( Current_GameNews.Mast_Game & 0x01 )
            game_exit();
         else
            game_next();
      }

      if(( ErrCur >= Current_GameNews.Num_Wrong_Game )
         &&( Current_Game.Type_Game == NEW_CONNECT_TYPE))
      {
         ErrCur = 0;
      }
#else
      if( ErrCur >= Current_GameNews.Num_Wrong_Game )
      {
         if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
         {
            /*bWrong = TRUE;
            bAnswerWrong = TRUE;
            bGameSpeech = TRUE;
            bSearchFound = TRUE;  //Don't search the other table;*/
            bGameStatus.value |= 0x5404u;//0x202A;
            ( void )act_do( Current_Game.Act_Wrong, 0 );
            bSubject = FALSE;   //exit user define
            if( Current_GameNews.Mast_Game & 0x01 )
               game_exit();
            else
               game_next();
         }
         else
         {
            ErrCur = 0;
         }
      }
#endif
      return TRUE;
   }
   return FALSE;
}


#pragma CODE = SEARCH_IN_GAME_TEMP_CODE01

Bool game_answer_wrong_search( void )
{
   U32 Address;
   U16 i, j;

   if( Addr_Game_Wrong_Answer )
   {
      read_data_from_nf( Addr_Game_Wrong_Answer,( U8 _MEM_TYPE_SLOW_ *)&i, sizeof( U16 ));
      Address = Addr_Game_Wrong_Answer + sizeof( U16 );
      for( j = 0; j < i; j++ )
      {
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Game_Wrong_Answer.Index_Wrong_Answer, sizeof( U8 ));
         Address += 1;
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Game_Wrong_Answer.Act_Wrong_Answer, sizeof(U32 ));
         Address += 4;
         if( QuestionIndex == Game_Wrong_Answer.Index_Wrong_Answer )
         {
            Wrong_Answered_count = j;
            return TRUE;
         }
      }
   }
   return FALSE;
}


#pragma CODE = SEARCH_GAME_COMBINED_CODE

void game_normal_combined_deal( void )
{
   Bool bPass;
   _MEM_TYPE_SLOW_ U8 i;
   
   if( bSearchFound )
   {
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      iCur = gkt_arry.Index_key;
      QuestionIndex = gkt_arry.Index_key;
      QuestionSpeech = gkt_arry.Action;
      bPass = FALSE;
      for( i = 0; i < AnswerCount; i++ )
      {
         if( iCur == Current_GameKeyIndex[ i ])
         {
            bPass = TRUE;
            break;
         }
      }
      if( bPass )
      {
         // It is in current answer list
         //if(( Answered[ i / 8 ]&( 1 << ( i % 8 ))))
         if(( Answered[ i >> 3 ]&( 1 << ( i & 0x07 ))))
         {
            ( void )act_do( Current_GameKeyConcent[ i ].Act_Third_Wrong, 0 );
         }
         else
         {
            //if( Current_Game.Type_Game == COMBINED_TYPE )
            {
               ( void )act_do( Current_GameKeyConcent[ i ].Act_Right, 0 );
            }
            /*else */if( Current_Game.Type_Game == COUNT_TYPE )
            {
               write_media_addr(( Addr_Count_Game + sizeof( U16 ) + count_type_num * 9 ), 0 );
               count_type_num++;
            }
            //Answered[ i / 8 ] |= 1 << ( i % 8 );
            Answered[ i >> 3 ] |= 1 << ( i & 0x07 );
            AnswerCur++;
            ErrCur = 0;
            check_exit_this( AnswerCur );
         }
      }
      else
      {
         //if(( All_Control.Version >= 12 )
         //   && combind_game_answer_wrong_search())
         SDKLoadCode( COMBINED_GAME_TEMP_CODE00 );
         if(( All_Control.Version >= 12 )
            && combind_game_answer_wrong_search())
         {
            //add 2013-05-22
//            HintAnswerIndex = find_first_unanswered();
//            game_wrong( 0 );
            SDKLoadCode( COMBINED_GAME_TEMP_CODE01 );
            combind_game_wrong();
            return;
         }
         HintAnswerIndex = find_first_unanswered();
         game_wrong( gkt_arry.Action );
      }
   }
   else
   {
      HintAnswerIndex = find_first_unanswered();
      QuestionIndex = 0;
      game_wrong( 0 );
   }
}


#pragma CODE = COMBINED_GAME_TEMP_CODE00

Bool combind_game_answer_wrong_search( void )
{
   if( game_answer_wrong_search())
   {
      if(( Current_GameNews.Mast_Game & SONIX_GAME )
         &&(( Current_GameNews.Num_Wrong_Game - ErrCur )== 1 ))
      {
         return FALSE;
      }

#if 0
      if(( Wrong_Answered[ Wrong_Answered_count / 8 ]&( 1 << ( Wrong_Answered_count % 8 ))))
      {
         //( void )act_do( Current_GameKeyConcent[ Wrong_Answered_count ].Act_Third_Wrong, 0 );
         ( void )act_do( Game_Wrong_Answer.Act_Wrong_Answer, 0 );
         return TRUE;
      }
      else
      {
         ( void )act_do( Game_Wrong_Answer.Act_Wrong_Answer, 0 );
         Wrong_Answered[ Wrong_Answered_count / 8 ] |= 1 << ( Wrong_Answered_count % 8 );
         return TRUE;
      }
#else
      ( void )act_do( Game_Wrong_Answer.Act_Wrong_Answer, 0 );
      Wrong_Answered[ Wrong_Answered_count >> 3 ] |= 1 << ( Wrong_Answered_count & 0x07 );
      return TRUE;
#endif
   }
   return FALSE;
}


#pragma CODE = COMBINED_GAME_TEMP_CODE01

void combind_game_wrong( void )
{
   ErrCur++;
   if( ErrCur >= Current_GameNews.Num_Wrong_Game )
   {
      if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
      {
         /*bWrong = TRUE;
         bAnswerWrong = TRUE;
         bGameSpeech = TRUE;
         bSearchFound = TRUE;  //Don't search the other table;*/
         bGameStatus.value |= 0x5404u;//0x202A;
         ( void )act_do( Current_Game.Act_Wrong, 0 );
         bSubject = FALSE;   //exit user define
         if( Current_GameNews.Mast_Game & 0x01 )
            game_exit();
         else
            game_next();
      }
      else
      {
         ErrCur = 0;
      }
   }
}


#pragma CODE = SEARCH_GAME_CONNECT_CODE00

void game_normal_connect_deal1( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   
   if( bSearchFound )
   {
      // is in answer index list
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      iCur = gkt_arry.Index_key;
      QuestionIndex = 0;
      if( check_answer_list())
      {
         // It is in current suject answer list
         if( check_odd_position())
         {
            //It is in odd position
            i = check_odd_answered();
            if( i >( AnswerCount - 1 ))
            {
               /*all odd is clicked,find out first odd 	
               and play first odd match even wrong hint */
               i = find_first_odd();
               HintAnswerIndex = i + 1;
               game_wrong( 0 );
            }
            else
            {
               /* find at least one odd have not link,
               play answer voice+right speech,
               new link have get */
               ErrCur = 0;
               AnswerCur = i;
               HintAnswerIndex = i;
               bWaitSubject = FALSE;
               ( void )act_do( gkt_arry.Action, 0 );
               ( void )act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
            }
         }
         else
         {
            //else it is in even position
            i = check_even_answered();
            if( i > AnswerCount )
            {
               /*all even is clicked,find out first even 
               and play this even wrong speech*/
               HintAnswerIndex = find_first_even();
               if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
               {
                  game_wrong( 0 );
               }
               else
               {
                  ( void )act_do( gkt_arry.Action, 0 );
               }
            }
            else
            {
               /* have haven't clicked even,
               play current answer voice */
               ( void )act_do( gkt_arry.Action, 0 );
            }
         }
      }
      else
      {
         // It isn't in current subject answer list
         // click wrong answer index
         HintAnswerIndex = AnswerCur;
         ( void )act_do( gkt_arry.Action, 0 );
      }
   }
   else
   {
      // It Isn't in answer list
      ( void )act_do( News_Go.Action_NoVoice, 0 );
   }
}


#pragma CODE = SEARCH_GAME_CONNECT_CODE01

void game_normal_connect_deal2(void)
{
   _MEM_TYPE_SLOW_ U8 i;

   _MEM_TYPE_SLOW_ U16 temp_num;
   _MEM_TYPE_SLOW_ U32 temp_add;
   
   if( bSearchFound )
   {
      // is in answer list
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      iCur = gkt_arry.Index_key;
      QuestionIndex = 0;
      if( iCur == Current_GameKeyIndex[ AnswerCur + 1 ])
      {
         /* It is matched answer,mask finished partnership,
         play right speech&resume to state 1. */
         ErrCur = 0;
         if( bNewLink )
         {
            //Answered[ AnswerCur / 8 ] |= 1 <<( AnswerCur % 8 );
            //Answered[( AnswerCur + 1 )/ 8 ] |= 1 <<(( AnswerCur + 1 )% 8 );
            Answered[ AnswerCur >> 3 ] |= 1 <<( AnswerCur & 0x07 );
            Answered[( AnswerCur + 1 )>> 3 ] |= 1 <<(( AnswerCur + 1 )& 0x07 );
            CurrentAnswerCount += 2;
         }
         bWaitSubject = TRUE;
         ( void )act_do( Current_GameKeyConcent[ AnswerCur + 1 ].Act_Right, 0 );

         if( Current_Game.Type_Game == NEW_CONNECT_TYPE
            && All_Control.Version >= 7 )
         {
            if( Addr_Count_Game )
            {
               read_data_from_nf( Addr_Count_Game,( U8 _MEM_TYPE_SLOW_ *)&temp_num, sizeof( temp_num ));
               if( temp_num && bNewLink )
               {
                  //if(( CurrentAnswerCount / 2 )< temp_num )
                  if(( CurrentAnswerCount >> 1 )< temp_num )
                  {
                     //temp_add = Addr_Count_Game +(( CurrentAnswerCount / 2 )- 1 )* 9 + 2;
                     temp_add = Addr_Count_Game +(( CurrentAnswerCount >> 1 )- 1 )* 9 + 2;
                     write_media_addr( temp_add, 0 );
                  }
               }
            }
         }
         check_exit_this( CurrentAnswerCount );
      }
      else
      {
         // It isn't correct answer.
         if( check_answer_list())
         {
            // It is in current answer list
            if( check_odd_position())
            {
               //It is in odd position
               i = check_odd_answered();
               if( i >( AnswerCount - 1 ))
               {
                  //all odd is clicked,play wrong speech

                  if( Current_Game.Type_Game == NEW_CONNECT_TYPE )
                  {
                     if(( ErrCur + 1 )>= Current_GameNews.Num_Wrong_Game )
                        bWaitSubject = FALSE;;
                  }

                  /*( void )act_do( gkt_arry.Action, 0 );
                  HintAnswerIndex = AnswerCur;
                  game_wrong( 0 );*/
                  i = find_first_odd();
                  HintAnswerIndex = i + 1;
                  game_wrong( 0 );
               }
               else
               {
                  /* find at least one odd have not link,
                  new link have get,2 state */
                  ErrCur = 0;
                  AnswerCur = i;
                  HintAnswerIndex = i;
                  bWaitSubject = FALSE;
                  ( void )act_do( gkt_arry.Action, 0 );
                  ( void )act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
               }
            }
            else
            {
               //is in even position,but not correct answer
               if( Current_Game.Type_Game != NEW_CONNECT_TYPE )
               {
                  ( void )act_do( gkt_arry.Action, 0 );
                  HintAnswerIndex = AnswerCur;
                  game_wrong( 0 );
               }
               else
               {
                  HintAnswerIndex = AnswerCur;
                  if(( ErrCur + 1 )>= Current_GameNews.Num_Wrong_Game )
                  {
                     bWaitSubject = TRUE;
                  }
                  game_wrong( 0 );
               }
            }
         }
         else
         {
            // click wrong answer index
            if( Current_Game.Type_Game == NEW_CONNECT_TYPE )
               ( void )act_do( gkt_arry.Action, 0 );
            HintAnswerIndex = AnswerCur;							
            game_wrong( 0 );
         }
      }
   }
   else
   {
      // Isn't in answer list
      QuestionIndex = 0;
      HintAnswerIndex = AnswerCur;					
      game_wrong( 0 );
   }
}


#pragma CODE = SEARCH_SCRIPT_CODE

/*F**************************************************************************
* NAME:script_play
*----------------------------------------------------------------------------
* PARAMS:script play funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: modify "CurrentScriptAddress"
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_play( void )
{
   Uint8 Count;
   
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
   ScriptCurrentAddress += sizeof( Count );
   for( ; Count; Count-- )
   {
      write_media_addr( ScriptCurrentAddress, 0 );
      ScriptCurrentAddress += ( U32 )9;
   }
}
/*F**************************************************************************
* NAME:script_play_random
*----------------------------------------------------------------------------
* PARAMS:script play random funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: modify "CurrentScriptAddress"
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_play_random( void )
{
   Uint8 Count, i;
   Uint32 SpeechAddress;
   
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
   ScriptCurrentAddress += sizeof( Count );
   SDKLoadCode( SEARCH_RAND_CODE );
   srand( timer_get_time());
   i = rand() % ( Count - 1 );
   write_media_addr( ScriptCurrentAddress + ( U32 )9 * i, 0 );
   ScriptCurrentAddress += 9 * Count;
}

/*F**************************************************************************
* NAME:script_play_number
*----------------------------------------------------------------------------
* PARAMS:script play number funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: modify "CurrentScriptAddress"
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_play_number( Uint16 number )
{
   Uint32 Address;
   Uint16 Count;

   Address = News_Book.Point_NumSpeechTable;
   read_data_from_nf( News_Book.Point_NumSpeechTable, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
   if(( Count == 0)||( number >= Count ))
   {
      return;
   }
	
   Address += sizeof( U16 )+( U32 )9 * number;
   write_media_addr( Address, 0 );
}
/*F**************************************************************************
* NAME:script_get_cmd
*----------------------------------------------------------------------------
* PARAMS:script decode funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: use global variable "ScriptCmd",modify "CurrentScriptAddress"
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_get_cmd( void )
{
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_ *)&ScriptCmd, sizeof( SCRIPT_CMD ));
   ScriptCurrentAddress += sizeof( SCRIPT_CMD );
}
/*F**************************************************************************
* NAME:script_get_address
*----------------------------------------------------------------------------
* PARAMS:script get address
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_get_address( void )
{
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_ *)&ScriptOffsetAddress, sizeof( ScriptOffsetAddress ));
   ScriptCurrentAddress += sizeof( ScriptOffsetAddress );
}
/*F**************************************************************************
* NAME:script_get_number
*----------------------------------------------------------------------------
* PARAMS:script get parameter
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_get_number( void )
{
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_ *)&ScriptNumber, sizeof( ScriptNumber ));
   ScriptCurrentAddress += sizeof( ScriptNumber );
}
/*F**************************************************************************
* NAME:script_jmp
*----------------------------------------------------------------------------
* PARAMS:script update address
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_jmp( void )
{
   ScriptCurrentAddress = Script_Address +( Uint32 )ScriptOffsetAddress;
}
/*F**************************************************************************
* NAME:script_get_para
*----------------------------------------------------------------------------
* PARAMS:script get parameter
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:use global variable "ScriptPara",modify "CurrentScriptAddress"
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_get_para( void )
{
   read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_ *)&ScriptPara, sizeof( SCRIPT_PARA ));
   ScriptCurrentAddress += sizeof( SCRIPT_PARA );	
}
/*F**************************************************************************
* NAME:clear_script_variable
*----------------------------------------------------------------------------
* PARAMS:clear all script variable
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:	This function shall be excuted after page load.
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void clear_script_variable( void )
{
   _MEM_TYPE_SLOW_ Uint8 i;
   for( i = 0; i < SCRIPT_VARIABLE_NUMBER; i++ )
      ScriptVariable[ i ] = 0;
}
/*F**************************************************************************
* NAME:script_decode
*----------------------------------------------------------------------------
* PARAMS:script decode funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_init( U32 script_addr )
{
   Script_Address = script_addr;
	ScriptCurrentAddress = Script_Address;
}

/*F**************************************************************************
* NAME:script_decode
*----------------------------------------------------------------------------
* PARAMS:script decode funtion
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void script_decode( void )
{
   while( 1 )
   {
      script_get_cmd();	   // get script command.
      switch( ScriptCmd.Command )
      {
         case CMD_CLEARVER:
         clear_script_variable();
         break;

         case CMD_SET:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] = ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            /*if( ScriptPara.Parameter2 == 99 )
            {
               ScriptVariable[ ScriptPara.Parameter1 ] = ScriptVariable[ 99 ];
            }
            else */if( ScriptPara.Parameter2 == 98 )
            {
               //ScriptVariable[ ScriptPara.Parameter1 ] = TL0;
               SDKLoadCode( SEARCH_RAND_CODE );
               srand( timer_get_time());
               ScriptVariable[ 98 ] = rand();
            }
            //else
            {
               ScriptVariable[ ScriptPara.Parameter1 ] = ScriptVariable[ ScriptPara.Parameter2 ];
            }
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_CMP:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ( ScriptVariable[ ScriptPara.Parameter1 ]          \
               > ScriptPara.Parameter2 )?( ScriptTest = 1 ):   \
                  ( ScriptVariable[ ScriptPara.Parameter1 ]    \
                     < ScriptPara.Parameter2 )?( ScriptTest = -1 ):( ScriptTest = 0 );
               break;

		      case CMD_TYPE_VER_VER:
            ( ScriptVariable[ ScriptPara.Parameter1 ]                         \
               > ScriptVariable[ ScriptPara.Parameter2 ])?( ScriptTest = 1 ): \
                  ( ScriptVariable[ ScriptPara.Parameter1 ]                   \
                     < ScriptVariable[ ScriptPara.Parameter2 ])?( ScriptTest = -1 ):( ScriptTest = 0 );
               break;

            /*default:
            break;*/
         }				
         break;

         case CMD_AND:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] &= ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            ScriptVariable[ ScriptPara.Parameter1 ] &= ScriptVariable[ ScriptPara.Parameter2 ];
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_OR:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] |= ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            ScriptVariable[ ScriptPara.Parameter1 ] |= ScriptVariable[ ScriptPara.Parameter2 ];
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_NOT:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] = ~ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            ScriptVariable[ ScriptPara.Parameter1 ] = ~ScriptVariable[ ScriptPara.Parameter2 ];
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_TEST:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            (( ScriptVariable[ ScriptPara.Parameter1 ]               \
               & ScriptPara.Parameter2 ) == ScriptPara.Parameter2 )? \
                  ( ScriptTest = TEST_EQUAL ):( ScriptTest = TEST_NOT_EQUAL );
               break;

            case CMD_TYPE_VER_VER:
            (( ScriptVariable[ ScriptPara.Parameter1 ]         \
               & ScriptVariable[ ScriptPara.Parameter2 ])      \
                  == ScriptVariable[ ScriptPara.Parameter2 ])? \
                     ( ScriptTest = TEST_EQUAL ):( ScriptTest = TEST_NOT_EQUAL );
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_ADD:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] += ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            ScriptVariable[ ScriptPara.Parameter1 ] += ScriptVariable[ ScriptPara.Parameter2 ];
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_SUB:
         script_get_para();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            ScriptVariable[ ScriptPara.Parameter1 ] -= ScriptPara.Parameter2;
            break;

            case CMD_TYPE_VER_VER:
            ScriptVariable[ ScriptPara.Parameter1 ] -= ScriptVariable[ ScriptPara.Parameter2 ];
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_JMP:
         script_get_address();
         script_jmp();
         break;

         case CMD_JE:
         script_get_address();
         if( CMP_EQUAL == ScriptTest )
         {
            script_jmp();
         }
         break;

         case CMD_JNE:
         script_get_address();
         if( CMP_EQUAL != ScriptTest )
         {
            script_jmp();
         }
         break;

         case CMD_JG:
         script_get_address();
         if( CMP_GREAT == ScriptTest )
         {
            script_jmp();
         }
         break;

         case CMD_JGE:
         script_get_address();
         if(( CMP_EQUAL == ScriptTest )
            ||( CMP_GREAT == ScriptTest ))
         {
            script_jmp();
         }
         break;

         case CMD_JB:
         script_get_address();
         if( CMP_LESS == ScriptTest )
         {
            script_jmp();
         }
         break;

         case CMD_JBE:
         script_get_address();
         if(( CMP_EQUAL == ScriptTest )
            ||( CMP_LESS == ScriptTest ))
         {
            script_jmp();
         }
         break;

         case CMD_PLAY:	
         script_play();
         break;

         case CMD_PLAYRANDOM:
         script_play_random();
         break;

         case CMD_PLAYNUMBER:
         script_get_number();
         switch( ScriptCmd.Type )
         {
            case CMD_TYPE_VER_CONST:
            script_play_number( ScriptNumber );
            break;

            case CMD_TYPE_VER_VER:
            script_play_number( ScriptVariable[ ScriptNumber ]);
            break;

            /*default:
            break;*/
         }
         break;

         case CMD_END:
         //! return this time script,not break;		
         default:
         //! error script command,exit
         return;
         //break;
      }
   }
}


#pragma CODE = SEARCH_GAME_TEMP_CODE00

void initial_console( void )
{
   GameState = GAME_LETTLE1;
   //lettle_count = 0;
   clr_input_buffer();
   //print_dic_screen();
   //print_lettle_screen( lettle_count );
}


void clr_input_buffer( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   word_count = 0;
   for( i = 0; i < WORD_MAX; i++ )
   {
      word[ i ] = 0; //initialization word value
   }  
}


#if 0
void print_dic_screen( void )
{
#if 0
   U8 i;
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
   //lcd_clear_part_of_screen( 25, 0, 84, 8 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( SEARCH_GAME_PosX_InputCursor, SEARCH_GAME_PosY_InputCursor );
   Ih_set_curr_font( IDC_FONT_08 );
   for( i = 0; i < word_count; i++ )
   {
      buffer[ 0 ] = word[ i ];
      buffer[ 1 ] = '\0';
      lcd_puts( buffer );
   }
   if( GameState == GAME_INPUT )
      lcd_puts_inverse( " ", TRUE );
   else
      lcd_puts( "_" );
#endif
}
#endif


#pragma CODE = SEARCH_DO_GAME_CODE

/*F**************************************************************************
* NAME: do_game
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE: 
*   do game.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void do_game( void )
{
   _MEM_TYPE_SLOW_ U32 Address;
   //_MEM_TYPE_SLOW_ Game_key_Table gkt_arry[ length_gkt ];
   U8  _MEM_TYPE_SLOW_ m;
   U16 _MEM_TYPE_SLOW_ i;
	
	bAnswerWrong = FALSE;
   Address = Current_GameNews.Point_Game_keyTable;
   if( !Address )
   {
      return;
   }
   if( !Current_GameNews.Num_Game_keyTable )
      return;
   for( i = 0; i < Current_GameNews.Num_Game_keyTable; i++ )
   {
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&gkt_arry, 30 );
      bSearchFound = TRUE;
      for( m = 0; m < WORD_MAX; m++ )
      {
         if( word[ m ] != gkt_arry.caption[ m ] )
         {
            bSearchFound = FALSE;
            break;
         }
      }
      if( bSearchFound )
         goto do_game_next;
      Address += 30;
   }
   
do_game_next:
   //if( bSearchFound )
   //{
      SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE01 );
      answer_logic_normal();
   //}
   //else
   //{
      //game_wrong( 0 );
   //}
   if( News_Control.Type_Control == GAME_TYPE )
   {
      if( !bAnswerWrong )
      {
         SDKLoadCode( SEARCH_GAME_TEMP_CODE00 );
         initial_console();
      }
   }
}


#pragma CODE = SEARCH_GAME_CONSOLE_CODE

/*F**************************************************************************
* NAME: game_console
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE: 
*   game console task.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
void game_console( void )
{
   gl_key_press = FALSE;
   
   if( word_count < WORD_MAX )
   {
      if((( CurrentKey >= 'a' )&&( CurrentKey <= 'z' ))
         ||(( CurrentKey >= '0' )&&( CurrentKey <= '9' )))
      {
         word[ word_count ] = CurrentKey;
         //print_word_lettle( word_count );
         word_count++;
         //bSpeechEnd = TRUE;
         return;
      }
   }
   if( CurrentKey == RM_ENTER )
   {
      if( word_count )
      {
         SDKLoadCode( SEARCH_DO_GAME_CODE );
         do_game();
      }
      else
      {
         SDKLoadCode( SEARCH_IN_GAME_TEMP_CODE01 );
         game_wrong( 0 );
         //print_clear_input();
      }
      state_search_to_play();
      //bSpeechEnd = TRUE;
      return;
   }
   if( CurrentKey == RM_CANCLE )
   {
      //print_clear_input();
      SDKLoadCode( SEARCH_GAME_TEMP_CODE00 );
      clr_input_buffer();
      //bSpeechEnd = TRUE;
      return;
   }
   if( CurrentKey == RM_BACK )
   {
      if( word_count )
      {
         word_count--;
         word[ word_count ] = 0x00;
         //print_lettle_cance( word_count );
      }
      //bSpeechEnd = TRUE;
      return;
   }
   if( CurrentKey == RM_DOT )
   {
      if( word_count < WORD_MAX )
      {
         word[ word_count ] = '\'';
         //print_word_lettle( word_count );
         word_count++;
      }
      return;
   }
#if 0
   switch( GameState )
   {
      case GAME_LETTLE1:
      switch( CurrentKey )
      {
         case NEXT:
         if( lettle_count != 15 )
         {
            lettle_count++;
            if( lettle_count == 14 )
               lettle_count++;
         }
         else
            lettle_count = 0;
         print_lettle_screen( lettle_count );
         break;

         case PREV:
         if( lettle_count != LETTLE_MIN )
         {
            lettle_count--;
            if( lettle_count == 14 )
               lettle_count--;
         }
         else
            lettle_count = 15;
         print_lettle_screen( lettle_count );
         break;
         
         case FUN:
         //cancel highlight
         GameState = GAME_LETTLE2;
         lettle_count += 16;
         print_lettle_screen( lettle_count );	 
         //print_lettle_identifier();
         break;

         case ENTER:
         if( word_count < WORD_MAX )
         {
            if( lettle_count == 13 )
            {
               word[ word_count ] = '\'';
            }
            else if( lettle_count == 15 )
            {
               News_Control.Type_Control = 0;
               Search_Record.news_sys.table_flag = DEFAULT_TABLE;
               Search_Record.news_sys.disable_next_key_flag = 1;
               Ap3_MenuPath = CONTROL_MENU_PATH;
               exit_game_to_control();
               break;
            }
            else
            {
               word[ word_count ] = lettle_count + 'a';
            }
            print_word_lettle( word_count );
            word_count++;
         }
         else
         {
            if( lettle_count == 15 )
            {
               News_Control.Type_Control = 0;
               Search_Record.news_sys.table_flag = DEFAULT_TABLE;
               Search_Record.news_sys.disable_next_key_flag = 1;
               Ap3_MenuPath = CONTROL_MENU_PATH;
               exit_game_to_control();
            }
         }
         break;
      }
      break;

      case GAME_LETTLE2:
      switch( CurrentKey )
      {
         case NEXT:
         if( lettle_count != LETTLE_MAX )
         {
            lettle_count++;
            if( lettle_count == LETTLE_MAX - 1 )
               lettle_count++;
         }
         else
            lettle_count = 16;
         print_lettle_screen( lettle_count );
         break;

         case PREV:
         if( lettle_count != 16 )
         {
            lettle_count--;
            if( lettle_count == LETTLE_MAX - 1 )
               lettle_count--;
         }
         else
            lettle_count = LETTLE_MAX;
         print_lettle_screen( lettle_count );
         break;

         case ENTER:
         if( word_count < WORD_MAX )
         {
            if( lettle_count == 29 )
            {
               word[ word_count ] = '.';
            }
            else if( lettle_count == 31 )
            {
               News_Control.Type_Control = 0;
               Search_Record.news_sys.table_flag = DEFAULT_TABLE;
               Search_Record.news_sys.disable_next_key_flag = 1;
               Ap3_MenuPath = CONTROL_MENU_PATH;
               exit_game_to_control();
               break;
            }
            else
            {
               word[ word_count ] = lettle_count + 'a' - 3;
            }
            print_word_lettle( word_count );
            word_count++;
         }
         else
         {
            if( lettle_count == 31 )
            {
               News_Control.Type_Control = 0;
               Search_Record.news_sys.table_flag = DEFAULT_TABLE;
               Search_Record.news_sys.disable_next_key_flag = 1;
               Ap3_MenuPath = CONTROL_MENU_PATH;
               exit_game_to_control();
            }
         }
         break;

         case FUN:
         //cancel highlight
         GameState = GAME_INPUT;
         print_lettle_screen( lettle_count );
         print_word_identifier( TRUE );
         break;
      }
      break;

      case GAME_INPUT:
      switch( CurrentKey )
      {
         case NEXT:
         //input a space.
         if( word_count < WORD_MAX )
         {
            word[ word_count ] = ' ';
            print_word_lettle( word_count );
            word_count++;
         }
         break;

         case PREV:
         if( word_count )
         {
            word_count--;
            word[ word_count ] = 0x00;
            print_lettle_cance( word_count );
         }
         break;

         case ENTER:
         if( word_count )
            do_game();
         else
         {
            game_wrong( 0 );
            print_clear_input();
         }
         state_search_to_play();
         break;
		
         case FUN:
         GameState = GAME_LETTLE1;
         lettle_count %= 16;
         //print_lettle_identifier();
         print_word_identifier( FALSE );
         print_lettle_screen( lettle_count );
         break;
      }
      break;
   }
	//bSpeechEnd = TRUE;
#endif
}

#if 0
#if 0
U8 code letter[] =
{
   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', '\'',' ','e',
   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.',' ','e'
};
#endif

void print_lettle_screen( U8 count )
{
#if 0
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
   if( count < 16 )
      print_key_char( SEARCH_GAME_PosX_Keyboard, SEARCH_GAME_PosY_Keyboard, 'A', '\'' );
   else
      print_key_char( SEARCH_GAME_PosX_Keyboard, SEARCH_GAME_PosY_Keyboard, 'N', '.' );	
   if( GameState != GAME_INPUT )
   {
      lcd_goto_xy( SEARCH_GAME_PosX_Keyboard +( count % 16 ) * 8, SEARCH_GAME_PosY_Keyboard );
      Ih_set_curr_font( IDC_FONT_16 );
      buffer[ 0 ] = letter[ count ];
      buffer[ 1 ] = '\0';
      lcd_puts_inverse( buffer, TRUE );
   }
#else
   count = count;
#endif
}

#if 0
void print_key_char( U8 x, U8 y, U8 asc, U8 iasc )
{
   U8 i;
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
	lcd_goto_xy( x, y );
	Ih_set_curr_font( IDC_FONT_16 );
   for( i = 0; i < 13; i++, asc++ )
	{
	   buffer[ 0 ] = asc;
      buffer[ 1 ] = '\0';
      lcd_puts( buffer );
	}
	buffer[ 0 ] = iasc;
   buffer[ 1 ] = '\0';
	lcd_puts( buffer );
	lcd_puts( " e" );
}


void print_word_identifier( Bool inverse )
{
   lcd_goto_xy( SEARCH_GAME_PosX_InputCursor + word_count * SEARCH_GAME_Title_CharWidth, SEARCH_GAME_PosY_InputCursor );
   Ih_set_curr_font( IDC_FONT_08 );
   if( inverse )
      lcd_puts_inverse( " ", TRUE );
   else
      lcd_puts( "_" );
}
#endif

void print_word_lettle( U8 count )
{
#if 0
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
   lcd_goto_xy( SEARCH_GAME_PosX_InputWord + count * SEARCH_GAME_Title_CharWidth, SEARCH_GAME_PosY_InputWord );
   buffer[ 0 ] = word[ count ];
   buffer[ 1 ] = '\0';
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts( buffer );
   //if( count != 7 )
   if( GameState == GAME_INPUT )
      lcd_puts_inverse( " ", TRUE );
   else
      lcd_puts( "_" );
#else
   count = count;
#endif
}


void print_lettle_cance( U8 count )
{
#if 0
   lcd_goto_xy( SEARCH_GAME_PosX_InputWord + count * SEARCH_GAME_Title_CharWidth, SEARCH_GAME_PosY_InputWord );
   Ih_set_curr_font( IDC_FONT_08 );
   if( GameState == GAME_INPUT )
   {
      lcd_puts_inverse( " ", TRUE );
      lcd_puts( " " );
   }
   else
      lcd_puts( "_ " );
#else
   count = count;
#endif
}


void print_clear_input( void )
{
#if 0
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
   lcd_goto_xy( SEARCH_GAME_PosX_InputCursor, SEARCH_GAME_PosY_InputCursor );
   Ih_set_curr_font( IDC_FONT_08 );
   if( GameState == GAME_INPUT )
      lcd_puts_inverse( " ", TRUE );
   else
      lcd_puts( "_" );
#endif
}
#endif


#endif
#endif  //#if( AP3 == ENABLE )

#endif  //_search_game_c_
