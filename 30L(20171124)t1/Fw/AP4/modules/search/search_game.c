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

#pragma DATA = SEARCH_GAME_DATA
#pragma CODE = SEARCH_GAME_CODE

#ifndef _search_game_c_
#define _search_game_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "search.h"
#include "search_game.h"
#include "search_remote.h"
#include "conf_oid.h"
#include "string.h"
//#include "stdlib.h" // Contains Funktion "rand()"
#include "system.h"
//#include "mail.h"
//#include "mail_evt.h"
#include "mmi_ap3.h"
//#include "srv_player.h"
//#include "com_evt.h"
#include "file.h"
#include "player_ap3.h"

#include "player_task.h"
//#include "int_drv.h"
#include "com.h"
//#include "game_play.h"
#include "BinaryImage.h"
#include "oid.h"
#include "keypad.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

ImageDeclareType(ANSWER_LOGIC_NORMAL_CODE);
ImageDeclareType(ANSWER_LOGIC_CONVERSE_CODE);
ImageDeclareType(GAME_SEARCH_1_CODE);

//_____ D E F I N I T I O N ________________________________________________
   _MEM_TYPE_SLOW_   Game_Table              Current_GameNews;
   _MEM_TYPE_SLOW_   Game_Project_Table      Current_Game;
   _MEM_TYPE_SLOW_   U32                     GameTipTick;
   _MEM_TYPE_SLOW_   U32                     GameTimeOverTick;

//   _MEM_TYPE_SLOW_ Game_Wrong_Answer_Table current_game_wrong_answer_table;

 _MEM_TYPE_SLOW_ GAME_STATUS bGameStatus;
 
 _MEM_TYPE_SLOW_ GAME_BIT_NEW game_bits;

//_MEM_TYPE_SLOW_ Game_key_Table      Current_GameKey;
_MEM_TYPE_SLOW_ U8                  Current_GameKeyIndex[ MAX_GAME_ANSWER ];
_MEM_TYPE_SLOW_ Game_concent_Table  Current_GameKeyConcent[ MAX_GAME_ANSWER ];

_MEM_TYPE_SLOW_ U8                  GameCount;
//_MEM_TYPE_SLOW_ U8                  ErrorCount;
_MEM_TYPE_SLOW_ U8                  RightNumber;
//_MEM_TYPE_SLOW_ U16                 CurrentGame;
#define CurrentGame                 Current_Coor.Y0

_MEM_TYPE_SLOW_ U8                  AnswerCount;
_MEM_TYPE_SLOW_ U8                  AnswerIndex;
_MEM_TYPE_SLOW_ U8                  AnswerCur;
_MEM_TYPE_SLOW_ U8                  CurrentAnswerCount;
_MEM_TYPE_SLOW_ U8                  ErrCur;
//_MEM_TYPE_SLOW_ U8                  Answered[ MAX_GAME_ANSWER ];
_MEM_TYPE_SLOW_ U8                  Answered[(( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 ];
_MEM_TYPE_SLOW_ U8                  Wrong_Answered[(( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 ];
_MEM_TYPE_SLOW_ U8					Wrong_Answered_count;
_MEM_TYPE_SLOW_ U8                  iCur;
_MEM_TYPE_SLOW_ U8                  QuestionIndex;
_MEM_TYPE_SLOW_ U32                 QuestionSpeech;
_MEM_TYPE_SLOW_ U8                  HintAnswerIndex;
_MEM_TYPE_SLOW_ Game_Wrong_Answer_Table current_game_wrong_answer_table;
_MEM_TYPE_SLOW_ U32                 Addr_current_game_wrong_answer_table;
_MEM_TYPE_SLOW_ U32                 Addr_current_count_game_voice_table;

_MEM_TYPE_SLOW_   U8                g_game_rand_tab[ GAME_MAX_RANGE_RAND ];
 _MEM_TYPE_SLOW_  U8				count_type_num;

_MEM_TYPE_SLOW_    U8				flag_play_game;
 _MEM_TYPE_SLOW_  Game_key_Table    gkt_arry;
//***********************************************

//_MEM_TYPE_SLOW_ Game_key_Table gkt;

 _MEM_TYPE_SLOW_  U8				   game_timer_stop;
extern unsigned int ap4_mp3_flag;
extern _MEM_TYPE_SLOW_  U16            Point_Write;
extern _MEM_TYPE_SLOW_  U16            Point_Read;



//! script varialbe define
_MEM_TYPE_SLOW_ U32 ScriptCurrentAddress;
_MEM_TYPE_SLOW_ U32 ScriptCurrentAddress_bak;
_MEM_TYPE_SLOW_ SCRIPT_CMD ScriptCmd;
_MEM_TYPE_SLOW_ SCRIPT_PARA ScriptPara;
_MEM_TYPE_SLOW_ U16 ScriptOffsetAddress;
_MEM_TYPE_SLOW_ U16 ScriptNumber;
_MEM_TYPE_SLOW_ Int8  ScriptTest;

_MEM_TYPE_SLOW_ Uint16 ScriptVariable[SCRIPT_VARIABLE_NUMBER];

//_____ D E C L A R A T I O N ______________________________________________
void  game_rand_init                ( void );
Bool  game_rand_table_is_full       ( void );
void  compare_coordinate            ( void );
//void  answer_logic_normal           ( U32 Addr );
void answer_logic_normal            ( void );
void  game_wrong                    ( U32 AnswerSpeech );
Bool  check_question_index          ( void );
void  play_wrong_hint               ( void );
void  play_exit_speech              ( void );
void  check_exit_this               ( U8 CurCount );
//void  answer_logic_converse         ( U32 Addr );
//void  answer_logic_converse         ( U8 _MEM_TYPE_SLOW_ *Addr );
void  find_first_valid_control      ( void );
U8    find_first_unanswered         ( void );
U8    find_first_even               ( void );
U8    find_first_odd                ( void );
U8    check_even_answered           ( void );
U8    check_odd_answered            ( void );
Bool  check_odd_position            ( void );
Bool  check_answer_list             ( void );
void  read_current_game_news        (U32 Address, Game_Project_Table *temp_project);
//void  push_back                     ( U8 index );
extern   void  srand                ( U32 s );
extern   U16   rand                 ( void );
void 	script_play					(void);
void 	script_play_number			(U16 number);
void 	script_play_random			(void);
void 	script_get_cmd				(void);
void 	script_get_number			(void);
void 	script_get_para				(void);
void 	script_get_address			(void);
void 	script_jmp					(void);
void	clear_script_variable		(void);
Bool 	game_answer_wrong_search	(void);
Bool 	sort_game_answer_wrong_search(void);
Bool 	combind_game_answer_wrong_search(void);
//void 	answer_logic_converse( U8 _MEM_TYPE_SLOW_ *Addr );
void 	answer_logic_converse( void );


extern void  read_control_mode_news        ( U16 num_control );
ImageDeclareType(ANSWER_LOGIC_SORT_CODE);
ImageDeclareType(ANSWER_LOGIC_COMBINE_CODE);
ImageDeclareType(ANSWER_LOGIC_CONNECT01_CODE);
ImageDeclareType(ANSWER_LOGIC_CONNECT02_CODE);

////////////////////////////////////////////////////////////
U16 get_game_rand( void );
#if 1
void check_game_time( void )
{
   if( bTimerEnable )
   {
      if( GameTipTick && Timer_is_timeout( GameTipTick ))
      {
         bTipPlay = TRUE;
         GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
      }
   }
   if( bTimeOverEnable )
   {
      if( GameTimeOverTick && Timer_is_timeout( GameTimeOverTick ))
      {
      	 GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
         if((Point_Read != Point_Write)||(g_b_player_on == TRUE))return;
         bGameTimeOverPlay = TRUE;
		 bWrong = TRUE;
      }
   }
}


void game_timeout_deal( void )
{
	
   if( bTipPlay )
   {
   	  interrupt_play();
//      SendString("\r\nbTipPlay\r\n");
      SDKLoadCode(GAME_SEARCH_1_CODE);
      game_exit();
      if( !bExitGamePRN )
      {
         play_timer_tip();
      }
      /*bSearchFound = FALSE;
      bTipPlay = FALSE;*/
      bGameStatus.value &= ~(U16)0x2100;
	  
   }
   if( bGameTimeOverPlay )
   {
   	 interrupt_play();
      bGameTimeOverPlay = FALSE;
      switch( Current_GameNews.Fuction_Question )
      {
     
         case GAME_TYPE_USER:
         if( !bSubject )
            break;
         // else, no break
          
         case GAME_TYPE_SEQU:
         case GAME_TYPE_RAND:
         if( bInGame )
         {
            init_array_v();
            (void)act_do( Current_Game.Act_Game_Concent, 0 );
         }
         break;

         //case GAME_TYPE_REV:
         // no break
         //default:
         //break;
      }
   }
   if( bSkipNext )
   {
   	interrupt_play();
      bSkipNext = FALSE;
      switch( Current_GameNews.Fuction_Question )
      {
         case GAME_TYPE_RAND:
         case GAME_TYPE_SEQU:
         case GAME_TYPE_USER:
		 SDKLoadCode(GAME_SEARCH_1_CODE);
         if( bNotAutoNextGame )
         {
            bNotAutoNextGame = FALSE;
            game_next();
            bNotAutoNextGame = TRUE;
         }
         else
            game_next();
         break;
      }
   }
   if( bSkipPrevious )
   {
   	  interrupt_play();
      bSkipPrevious = FALSE;
//	  SendString("\r\nbSkipPrevious\r\n");
      switch( Current_GameNews.Fuction_Question )
      {
         case GAME_TYPE_RAND:
         case GAME_TYPE_SEQU:
         case GAME_TYPE_USER:
		 SDKLoadCode(GAME_SEARCH_1_CODE);
         if( bNotAutoNextGame )
         {
 //          SendString("\r\nbSkipPrevious_1\r\n");
            bNotAutoNextGame = FALSE;
            bGamePrev = TRUE;
            game_next();
            bGamePrev = FALSE;
            bNotAutoNextGame = TRUE;
         }
         else
         {
 //        	SendString("\r\nbSkipPrevious_2\r\n");
            bGamePrev = TRUE;
            game_next();
            bGamePrev = FALSE;
         }
         break;
      }
   }
}
#endif
#pragma CODE = GAME_SEARCH_1_CODE

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
//	  SendString("\r\nGameCount=");
//	  send_hex_u8(GameCount);
      GameCount--;
   }
   switch( Current_GameNews.Fuction_Question )
   {
      case GAME_TYPE_RAND:
#ifdef TX_DEBUG
	  SendString("\r\nGame_rand!");
#endif
      CurrentGame = get_game_rand();
      init_current_game();
	  bGameSpeech = TRUE;
      if( bGameSpeech )
         (void)act_do( Current_Game.Act_Game_Concent, 0 );	
      break;

      case GAME_TYPE_SEQU:
      if( bGameFirst )
      {
         bGameFirst = FALSE;
         init_current_game();
		 bGameSpeech = TRUE;
         if( bGameSpeech )
            (void)act_do( Current_Game.Act_Game_Concent, 0 );
         break;
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
      init_current_game();
      if( bGameSpeech )
         (void)act_do( Current_Game.Act_Game_Concent, 0 );
      break;

      case GAME_TYPE_REV:
      bInGame = TRUE;
      break;

      case GAME_TYPE_USER:
      bInGame = TRUE;
      bSubject = FALSE;
      break;

   }
}


//initial current game
void init_current_game( void )
{
   _MEM_TYPE_SLOW_ U32 Address,temp_addr;
   //_MEM_TYPE_SLOW_ U8 i;
//   Address = Current_GameNews.Point_Game_Project + CurrentGame * sizeof( Game_Project_Table ); 
   Address = Current_GameNews.Point_Game_Project + CurrentGame * sizeof( U8 )*24; 
   memset(&Current_Game,0,sizeof(Game_Project_Table));
   read_current_game_news(Address,&Current_Game);
   flag_play_game = FALSE;
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
   bGameStatus.value |= 0x0801;
   read_data_from_nf( Current_Game.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyIndex, sizeof( U8 )* AnswerCount );
   read_data_from_nf( Current_Game.Addr_key_Concent, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyConcent, sizeof( Game_concent_Table )* AnswerCount );
   memset( (char *)Answered, 0, (( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 );
   memset( (char *)Wrong_Answered, 0, (( MAX_GAME_ANSWER + 7 )& 0xF8 )/ 8 );
   Wrong_Answered_count = 0;
   count_type_num = 0;

   if(All_Control.Version >= 7)
		{
		 temp_addr = Current_GameNews.Addr_Extern_Project_Game+CurrentGame*8+1;
		 read_data_from_nf( temp_addr, ( U8 _MEM_TYPE_SLOW_ *)&Addr_current_count_game_voice_table, sizeof( U8 )*4 );
		}
   
   if(All_Control.Version >= 12)
		{
		 temp_addr = Current_GameNews.Addr_Extern_Project_Game+CurrentGame*8+5;
		 read_data_from_nf( temp_addr, ( U8 _MEM_TYPE_SLOW_ *)&Addr_current_game_wrong_answer_table, sizeof( U8 )*4 );
		}
}





//search coordinate area in game mode
void in_game( void )
{
   _MEM_TYPE_SLOW_ U32 Address,temp_addr;
   //_MEM_TYPE_SLOW_ U16 Number;
   //_MEM_TYPE_SLOW_ Game_key_Table gkt;
   _MEM_TYPE_SLOW_ U16 m;

//   _MEM_TYPE_SLOW_ Game_key_Table gkt_arry[ length_gkt ];
//   _MEM_TYPE_SLOW_ Game_key_Table gkt_arry;
   U8  _MEM_TYPE_SLOW_ j, n;
   U16 _MEM_TYPE_SLOW_ i;
   _MEM_TYPE_SLOW_ U32 game_coor_index;
   
   if( !bInGame )
   {
      U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
      switch( Coor_Type )
      {
         case IMMOBILITY_COOR_MODE:
         if( !News_Go.Point_DefaultTab )
         {
            break;
         }
         kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof( U32 );
         read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_*)&action_address, sizeof( U32 ));
         if( act_do( action_address, 1 ))
         {
            return;
         }
         break;
      }
      return;
   }
   
   Address = Current_GameNews.Point_Game_keyTable;
   if( !Address )
   {
      return;
   }
   if( !Current_GameNews.Num_Game_keyTable )
      return;
#if 0   
	   SendString("\r\nCurrent_GameNews.Point_Game_keyTable=");
	   send_hex_u32(Current_GameNews.Point_Game_keyTable);
	   SendString("\r\nCurrent_GameNews.Num_Game_keyTable=");
	   send_hex_u32(Current_GameNews.Num_Game_keyTable);
#endif	   
   bSearchFound = FALSE;
//      while( i )
      {
//         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_*)&gkt_arry, ( U16 )sizeof( Game_key_Table )*( U16 )length_gkt );
		 for(m=0;m<Current_GameNews.Num_Game_keyTable;m++)
		 	{
//		 	 temp_addr = Address+m*( U32 )sizeof( Game_key_Table );
		 	 temp_addr = Address+m* sizeof( U8 )*30;
//				 SendString("\r\ntemp_addr=");
//				   send_hex_u32(temp_addr);
			 (void)file_seek( temp_addr, FS_SEEK_SET );
   			 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&gkt_arry.Game_Coor.X0, sizeof(U8)*8);
#if 1			 
			 (void)file_seek( (temp_addr+8), FS_SEEK_SET );
   			 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&gkt_arry.Type_Key, sizeof(U8)*1);
			 (void)file_seek( (temp_addr+9), FS_SEEK_SET );
   			 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&gkt_arry.Index_key, sizeof(U8)*1);
			 (void)file_seek( (temp_addr+10), FS_SEEK_SET );
   			 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&gkt_arry.Action, sizeof(U8)*20);
#endif	 
		if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
            {
               game_coor_index = gkt_arry.Game_Coor.X0;
            }
            else
            {
               MSW( game_coor_index ) = gkt_arry.Game_Coor.X0;
               LSW( game_coor_index ) = gkt_arry.Game_Coor.Y0;
            }
	         if( Current_Index == game_coor_index )
            //if( Current_Index == gkt_arry.Game_Coor.X0 )
	           {
          
	              bSearchFound = TRUE;
				  flag_play_game = TRUE;
//				  SendString("find game answer!\r\n");
	              goto in_game_next;
	           }
			 
		 	}
//			SendString("no find game answer!\r\n");
      }
in_game_next:
   //if( All_Control.Mask_Gloablext & AP3_TO_AEB_FLAG )
   {
      if( !bSearchFound )
      {
         U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
         switch( Coor_Type )
         {
            case IMMOBILITY_COOR_MODE:
//	         kuang_adress = News_Control.Point+ ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof( U32 );
//	         read_data_from_nf( kuang_adress, ( _MEM_TYPE_SLOW_ U8 *)&action_address, sizeof( U32 ));
//	         if( News_Control.Num_Control )
//	         {
//	            if( act_do( action_address, 0 ))
//	            {
//	               return;
//	            }
//	         }	
 			SendString("no game find!\r\n");
            if( !News_Go.Point_DefaultTab )
            {
//               SendString("News_Go.Point_DefaultTab=0\r\n");
               break;
            }
            kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof( U32 );
            (void)read_data_from_nf( kuang_adress, ( U8 _MEM_TYPE_SLOW_*)&action_address, sizeof( U32 ));
//			SendString("123=");
//			send_hex_u32(News_Go.Point_DefaultTab);
//			SendString("\r\n");
//			send_hex_u32(kuang_adress);
//			SendString("\r\n");
//			send_hex_u32(action_address);
//			SendString("\r\n");
            if( act_do( action_address, 1 ))
            {
               return;
            }
            break;

         }
      }
   }
   switch( Current_GameNews.Fuction_Question )
   {
  
      case GAME_TYPE_REV:
	  SDKLoadCode(ANSWER_LOGIC_CONVERSE_CODE);	
      //answer_logic_converse( ( U8 _MEM_TYPE_SLOW_ *)&gkt_arry );
      answer_logic_converse();
 #ifdef TX_DEBUG
   		SendString("\r\n550");
#endif 	  
      break;

      case GAME_TYPE_SEQU:
      case GAME_TYPE_RAND:
	  SDKLoadCode(ANSWER_LOGIC_NORMAL_CODE);
      answer_logic_normal(  );
      break;

      case GAME_TYPE_USER:
      if( bSubject )
      {
          SDKLoadCode(ANSWER_LOGIC_NORMAL_CODE);
         answer_logic_normal(  );
      }
      else
      {
         if( bSearchFound )
         {
            (void)act_do( gkt_arry.Action, 0 );
            QuestionIndex = gkt_arry.Index_key;
            QuestionSpeech = 0;
            (void)check_question_index();
         }
         else
         {
            //haven't get subject,play null speech
            (void)act_do( News_Go.Action_NoVoice, 0 );
         }				
      }
      break;
   }
}


//////========================================================================
//GAME_SEARCH_1_CODE
/////=========================================================================
Bool game_answer_wrong_search(void)
{
	U8 temp_num;
	U32 temp_addr,temp_addr_2;
	U16 i,j;
//    Game_Wrong_Answer_Table current_game_wrong_answer_table;
		if(Addr_current_game_wrong_answer_table!=0)
			{
			 (void)read_data_from_nf(Addr_current_game_wrong_answer_table,( U8 _MEM_TYPE_SLOW_ *)&i, sizeof(i));
			 temp_addr_2 =Addr_current_game_wrong_answer_table + 2;
			 for(j=0;j<i;j++)
			 	{
			 	 read_data_from_nf(temp_addr_2,( U8 _MEM_TYPE_SLOW_ *)&current_game_wrong_answer_table.Index_Wrong_Answer, sizeof(U8));
				 temp_addr_2 +=1;
				  read_data_from_nf(temp_addr_2,( U8 _MEM_TYPE_SLOW_ *)&current_game_wrong_answer_table.Act_Wrong_Answer, sizeof(U8)*4);
				  temp_addr_2 +=4;
				 if(QuestionIndex==current_game_wrong_answer_table.Index_Wrong_Answer)
				 	{
//				 	 act_do(current_game_wrong_answer_table.Act_Wrong_Answer, 0);
					 Wrong_Answered_count=i;
					 return TRUE;
				 	}
			 	}
			}
	return FALSE;
}

Bool sort_game_answer_wrong_search(void)
{
	if(game_answer_wrong_search())
		{
		 (void)act_do(current_game_wrong_answer_table.Act_Wrong_Answer, 0);
		 ErrCur++;
		 if(( ErrCur >= Current_GameNews.Num_Wrong_Game )&&(Current_Game.Type_Game != NEW_CONNECT_TYPE))
		   {
		      bGameStatus.value |= 0x202A;
		      (void)act_do( Current_Game.Act_Wrong, 0 );
		      bSubject = FALSE;   //exit user define
		      if( Current_GameNews.Mast_Game & 0x01 )
		         game_exit();
		      else
		         game_next();
		   }

		   if(( ErrCur >= Current_GameNews.Num_Wrong_Game )&&(Current_Game.Type_Game == NEW_CONNECT_TYPE))
		   	{
		   	 ErrCur = 0;
		   	}
		 return TRUE;
		}
		return FALSE;
}

Bool combind_game_answer_wrong_search(void)
{
	if(game_answer_wrong_search())
		{
            if(( Wrong_Answered[ Wrong_Answered_count/ 8 ]&( 1 << ( Wrong_Answered_count % 8 ))))
            {
               
               (void)act_do( Current_GameKeyConcent[ Wrong_Answered_count ].Act_Third_Wrong, 0 );
			   return TRUE;
            }
            else
            {
               (void)act_do(current_game_wrong_answer_table.Act_Wrong_Answer, 0);
               Wrong_Answered[ Wrong_Answered_count / 8 ] |= 1 << ( Wrong_Answered_count % 8 );
               return TRUE;
            }
		}
		return FALSE;
}

//void read_current_game_news(U32 Address, Game_Project_Table *temp_project);
//***********************************************

void read_current_game_news(U32 Address, Game_Project_Table *temp_project)
{
/*
   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Num_Key, sizeof( U8 ));
   (void)file_seek( (Address+1), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Addr_Key_Index, sizeof(U8)*8);
   (void)file_seek( (Address+9), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Type_Game, sizeof(U8)*1);
   (void)file_seek( (Address+10), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Act_Game_Concent, sizeof(U8)*4);
   (void)file_seek( (Address+14), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Index_Game, sizeof(U8)*1);
   (void)file_seek( (Address+15), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Act_Right, sizeof(U8)*8);
   (void)file_seek( (Address+23), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_project->Num_Answer, sizeof(U8)*1);
 */  
  read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Num_Key, sizeof( U8 ));
   read_data_from_nf( Address + 1, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Addr_Key_Index, 9 );
   //read_data_from_nf( Address + 9, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Type_Game, 5 );
   read_data_from_nf( Address + 10, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Act_Game_Concent, 5 );
   //read_data_from_nf( Address + 14, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Index_Game, 9 );
   read_data_from_nf( Address + 15, ( U8 _MEM_TYPE_SLOW_ *)&temp_project->Act_Right, 9 );
   //read_data_from_nf( Address + 23, ( U8 _MEM_TYPE_SLOW_ *)&buf->Num_Answer, sizeof( U8 ));
}


//game exit
void game_exit( void )
{
//  U8 temp_answercount;
	
   if( bWrong )
   {
      (void)act_do( Current_GameNews.Act_Havewrong, 0 );
   }
   else
   {
   	 
	  if(RightNumber>=Current_GameNews.Num_Every_Game)
		 (void)act_do( Current_GameNews.Act_Right, 0 );
			
   }
   if( bExitGamePRN )
      play_timer_tip();
   find_first_valid_control();

//   bGameStatus.value &= (U16)(~0x00C2);
//   bNotAutoNextGame = FALSE;

//   bGameClicked = FALSE;
	bGameExitFlag = TRUE;
	state_search_to_play();
//	scan_oid_disable();
//	Kbd_set_evt_disabled();
//   bak_to_news();
//   CommonLoadCode(C_OID);
}
void game_rand_init( void )
{
   // Init rand table
   memset( (char *)g_game_rand_tab, 0, GAME_MAX_RANGE_RAND );
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
      if( 0 == ( g_game_rand_tab[ number / 8 ]&( 1 << ( number % 8 ))))
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
   srand( timer_get_time());
   while( 1 )
   {
      if( game_rand_table_is_full())
         game_rand_init();
      m = rand() % Current_GameNews.Num_Game;
      i = m %( GAME_MAX_RANGE_RAND * 8 );
      if( 0 == ( g_game_rand_tab[ i / 8 ]&( 1 << ( i % 8 ))))
      {
         g_game_rand_tab[ i / 8 ] |= 1 << ( i % 8 ); // Update table
         break;   // A no used position find
      }
   }
   return m;
}



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


//game answer wrong
void game_wrong( U32 AnswerSpeech )
{
   if( bQuestion )
   {
      if( check_question_index())
         return;
	}
   switch( Current_Game.Type_Game )
   {
      case SORT_TYPE:
      (void)act_do( AnswerSpeech, 0 );
      play_wrong_hint();
      break;

      case CONNECT_TYPE:
	  case NEW_CONNECT_TYPE:
      play_wrong_hint();
      break;
	  
	  case COUNT_TYPE:
      case COMBINED_TYPE:
      (void)act_do( AnswerSpeech, 0 );
      if( !ErrCur )
      {
         (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      }
      else
      {
         if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
         {
            (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
         }
         else
         {
            (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
         }
      }
      break;
   }
   ErrCur++;

/*  
	if(All_Control.Version>=12)
  	{
  	  if(( ErrCur >= 3 )&&(Current_Game.Type_Game != NEW_CONNECT_TYPE))
		{
		  bGameStatus.value |= 0x202A;
		  act_do( Current_Game.Act_Wrong, 0 );
		  bSubject = FALSE;   //exit user define
		  if( Current_GameNews.Mast_Game & 0x01 )
		     game_exit();
		  else
		     game_next();
		}

		if(( ErrCur >= 3 )&&(Current_Game.Type_Game == NEW_CONNECT_TYPE))
			{
			 ErrCur = 0;
			}

  	}
  else
  	{
*/
		if(( ErrCur >= Current_GameNews.Num_Wrong_Game )&&(Current_Game.Type_Game != NEW_CONNECT_TYPE))
		{
		  /*bWrong = TRUE;
		  bAnswerWrong = TRUE;
		  bGameSpeech = TRUE;
		  bSearchFound = TRUE;  //Don't search the other table;*/
		  bGameStatus.value |= 0x202A;
		  (void)act_do( Current_Game.Act_Wrong, 0 );
		  bSubject = FALSE;   //exit user define
		  if( Current_GameNews.Mast_Game & 0x01 )
		     game_exit();
		  else
		     game_next();
		}

		if(( ErrCur >= Current_GameNews.Num_Wrong_Game )&&(Current_Game.Type_Game == NEW_CONNECT_TYPE))
			{
			 ErrCur = 0;
		//	 bWaitSubject = FALSE;
			}
//  	}
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
	 read_current_game_news(Address,&tempGameMsg);

      if( tempGameMsg.Index_Game == QuestionIndex )
      {
         CurrentGame = i;
         init_current_game();
         (void)act_do( QuestionSpeech, 0 );				
         (void)act_do( Current_Game.Act_Game_Concent, 0 );
         bSubject = TRUE;
         return TRUE;
      }
//      Address += sizeof( Game_Project_Table );
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
      (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      break;

      case 1:
      if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
      {
         (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
      }
      else
      {
         (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
      }
      break;

      default:
      if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Third_Wrong == 0 )
      {
         if( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong == 0 )
         {
            (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_First_Wrong, 0 );
         }
         else
         {
            (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Second_Wrong, 0 );
         }
      }
      else
      {
         (void)act_do( Current_GameKeyConcent[ HintAnswerIndex ].Act_Third_Wrong, 0 );
      }
      break;
   }
}


void play_exit_speech( void )
{
   if( bAnswerWrong )
   {
      (void)act_do( Current_Game.Act_Wrong, 0 );
   }
   else
   {
      (void)act_do( Current_Game.Act_Right, 0 );
   }
}


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


//find first match even index
U8 find_first_unanswered( void )
{
   _MEM_TYPE_SLOW_ U8 i;

   for( i = 0; i < AnswerCount; i++ )
   {
      if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
      {
         break;
      }
   }
   return i;
}


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


//check even answered index
U8 check_even_answered( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for( i = 1; i < AnswerCount; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
            break;
      }
   }
   return i;
}


//check odd answered index
U8 check_odd_answered( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   for(i = 0; i < AnswerCount - 1; i += 2 )
   {
      if( iCur == Current_GameKeyIndex[ i ])
      {
         if(( Answered[ i / 8 ]&( 1 <<( i % 8 )))== 0 )
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

#if 0
//judge the answer index after find answer
void answer_logic_converse( U8 _MEM_TYPE_SLOW_ *Addr )
{
   _MEM_TYPE_SLOW_ Game_key_Table gkt;
   static _MEM_TYPE_SLOW_ U8 /*iCur, */j, cindex[ MAX_GAME_ANSWER ];
   _MEM_TYPE_SLOW_ U32 Address;
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ Game_Project_Table iGameMsg;
   Bool bFind;

   memset(&cindex,0,sizeof(U8)*MAX_GAME_ANSWER);
   memmove( (char *)&gkt, Addr, sizeof( Game_key_Table ));
   if( bSearchFound )
   {
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      act_do( gkt.Action, 0 );
      /*iCur = gkt.Index_key;
      push_back( iCur );*/
      //push_back( gkt.Index_key );
      Current_GameKeyIndex[ AnswerIndex ] = gkt.Index_key;
      AnswerIndex = ( AnswerIndex + 1 )& 0x1F;
      
      Address = Current_GameNews.Point_Game_Project;
      for( i = 0; i < Current_GameNews.Num_Game; i++ )
      {
         bFind = TRUE;		 
		 read_current_game_news(Address,&iGameMsg);
	  
         read_data_from_nf( iGameMsg.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&cindex, sizeof( U8 )* iGameMsg.Num_Key );
         for( j = 0; j < iGameMsg.Num_Key; j++ )
         {
            if( Current_GameKeyIndex[( AnswerIndex - iGameMsg.Num_Key + j )& 0x1F ] != cindex[ j ])
            {
               Address += sizeof( Game_Project_Table );
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
         act_do( iGameMsg.Act_Game_Concent, 0 );
         RightNumber++;
      }
   }
   else
   {
      act_do( News_Go.Action_NoVoice, 0 );
   }
}
#endif


/*
void push_back( U8 index )
{
   Current_GameKeyIndex[ AnswerIndex ] = index;
   AnswerIndex = ( AnswerIndex + 1 )& 0x1F;
}*/


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
//      read_data_from_nf( ControlAddress, ( U8 _MEM_TYPE_SLOW_ *)&TempControl, sizeof( Control_Table ));
  		// read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&ControlAddress, sizeof( U32 ));
	   //(void)file_seek( (ControlAddress+24), FS_SEEK_SET );
	   //(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&TempControl.Type_Control, sizeof(U8));
	   read_data_from_nf( ControlAddress + 16, ( U8 _MEM_TYPE_SLOW_ *)&TempControl.Point, 9 );
      //read_data_from_nf( ControlAddress + 24, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Type_Control, sizeof( U8 ));
      //if( All_Control.Version < 15 )
      if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
      {
         U16 _MEM_TYPE_SLOW_ temp;

         read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
         TempControl.Num_Control = ( U32 )temp;
         file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&TempControl.Flag, sizeof( U8 ));
      }
      else
         read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&TempControl.Num_Control, 5 );
      if(( TempControl.Type_Control == SINGLE_TABLE_TYPE ))
        {
         News_Control = TempControl;
         Current_Record.Control_num = i;
		 SDKLoadCode(READ_CONTR_NEWS_CODE);
		 read_control_mode_news( Current_Record.Control_num );
         Search_Record.news_sys.table_flag = CONTROL_TABLE;
         //Search_Record.news_sys.mode_flag = SINGLE_TABLE;
//         Ap3_MenuPath = CONTROL_MODE_MENU_PATH;
         return;
       }
   }
    //Search_Record.news_sys.mode_flag = 0;
   News_Control.Type_Control = NO_TYPE;
   if( News_Go.Point_DefaultTab )
   {

         Search_Record.news_sys.table_flag = DEFAULT_TABLE;
         //Search_Record.news_sys.disable_next_key_flag = 1;
//         Ap3_MenuPath = CONTROL_MENU_PATH;
         return;
   }

   if( News_Go.Point_BackgroudTab )
   {

         Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
         //Search_Record.news_sys.disable_next_key_flag = 1;
//         Ap3_MenuPath = CONTROL_MENU_PATH;
         return;
   }
}




void game_initial_1(void)
{
   game_rand_init();
   game_next();
}

#pragma CODE = ANSWER_LOGIC_SORT_CODE

void game_sort_type(void)
{
//	   SendString("\r\nSORT_TYPE");
      if( bSearchFound )
      {
         //memmove( &gkt, Addr, sizeof( Game_key_Table ));
         iCur = gkt_arry.Index_key;
         QuestionIndex = gkt_arry.Index_key;
         QuestionSpeech = gkt_arry.Action;
         if( iCur != Current_GameKeyIndex[ AnswerCur ])
         {
           if((All_Control.Version >= 12)&&sort_game_answer_wrong_search())
           	{
           	}
		   else
		   	{
            	game_wrong( gkt_arry.Action );		
		   	}
            return;
         }
         else
         {
            (void)act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
            ErrCur = 0;
            AnswerCur++;
            HintAnswerIndex = AnswerCur;
			CurrentAnswerCount = AnswerCur;
            check_exit_this( AnswerCur );
         }
      }
      else
      {
         QuestionIndex = 0;
         game_wrong( 0 );
      }
}
#pragma CODE = ANSWER_LOGIC_COMBINE_CODE
void game_combine_type(void)
{
	  Bool bPass;
	  _MEM_TYPE_SLOW_ U8 i;
//	  	 SendString("\r\nCOMBINED_TYPE");
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
               (void)act_do( Current_GameKeyConcent[ i ].Act_Third_Wrong, 0 );
            }
            else
            {
            if(Current_Game.Type_Game==COMBINED_TYPE)
            	{
               		(void)act_do( Current_GameKeyConcent[ i ].Act_Right, 0 );
            	}
			if(Current_Game.Type_Game==COUNT_TYPE)
            	{
            		write_media_addr((Addr_current_count_game_voice_table+2+count_type_num*9), 0);
               		count_type_num++;
            	}
               Answered[ i / 8 ] |= 1 << ( i % 8 );
               AnswerCur++;
               ErrCur = 0;
			   CurrentAnswerCount = AnswerCur;
               check_exit_this( AnswerCur );
            }
         }
         else
         {
            if((All_Control.Version >= 12)&&combind_game_answer_wrong_search())
            	{
            	}
			else
				{
		            HintAnswerIndex = find_first_unanswered();
		            game_wrong( gkt_arry.Action );
				}
         }
      }
      else
      {
         HintAnswerIndex = find_first_unanswered();
         QuestionIndex = 0;
         game_wrong( 0 );
      }
}
#pragma CODE = ANSWER_LOGIC_CONNECT01_CODE
void game_connect01_type(void)
{
	//Bool bPass;
	  _MEM_TYPE_SLOW_ U8 i;
	_MEM_TYPE_SLOW_ U16	temp_num;
	_MEM_TYPE_SLOW_ U32 temp_add;
//	  SendString("CONNECT_TYPE\r\n");	
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
//					 SendString("769\r\n");
                     game_wrong( 0 );
                  }
                  else
                  {
                     /* find at least one odd have not link,
                     play answer voice+right speech,
                     new link have get */
//                     SendString("776\r\n");
                     ErrCur = 0;
                     AnswerCur = i;
                     HintAnswerIndex = i;
                     bWaitSubject = FALSE;
                     (void)act_do( gkt_arry.Action, 0 );
                     (void)act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );

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
//					 SendString("810\r\n");
					 if(Current_Game.Type_Game != NEW_CONNECT_TYPE)
					 	{
                         game_wrong( 0 );
					 	}
					 else
					 	{
					 	 (void)act_do( gkt_arry.Action, 0 );
					 	}
                  }
                  else
                  {
                     /* have haven't clicked even,
                     play current answer voice */
//                     SendString("816\r\n");
                     (void)act_do( gkt_arry.Action, 0 );
                  }
               }
            }
            else
            {
               // It isn't in current subject answer list
               // click wrong answer index
//               SendString("827\r\n");
               HintAnswerIndex = AnswerCur;
               (void)act_do( gkt_arry.Action, 0 );
            }
         }
         else
         {
            // It Isn't in answer list
            (void)act_do( News_Go.Action_NoVoice, 0 );
         }   
}
#pragma CODE = ANSWER_LOGIC_CONNECT02_CODE
void game_connect02_type(void)
{
	//Bool bPass;
	  _MEM_TYPE_SLOW_ U8 i;
	_MEM_TYPE_SLOW_ U16	temp_num;
	_MEM_TYPE_SLOW_ U32 temp_add;
//	  SendString("CONNECT_TYPE\r\n");	
      {
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
//			   SendString("857\r\n");
               bWaitSubject = TRUE;
               (void)act_do( Current_GameKeyConcent[ AnswerCur + 1 ].Act_Right, 0 );
			 if(Current_Game.Type_Game == NEW_CONNECT_TYPE)
				{
					read_data_from_nf(Current_GameNews.Addr_Extern_Project_Game,( U8 _MEM_TYPE_SLOW_ *)&temp_num, sizeof(temp_num));
				if(temp_num!=0)
					{
					if((CurrentAnswerCount/2)<temp_num)
						{
//							send_hex_u32(Current_GameNews.Addr_Extern_Project_Game);
//							SendString("\r\n");
							
							temp_add = Current_GameNews.Addr_Extern_Project_Game+((CurrentAnswerCount/2)-1)*9+2;
//							send_hex_u32(temp_add);
//							SendString("\r\n");
		//					read_data_from_nf(temp_add,( U8 _MEM_TYPE_SLOW_ *)&temp_add, sizeof(temp_add));	
		//					send_hex_u32(temp_add);
		//					SendString("\r\n");
							write_media_addr( temp_add, 0 );
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
//                        SendString("875\r\n");
						if(Current_Game.Type_Game == NEW_CONNECT_TYPE)
							{
							   if((ErrCur+1)>= Current_GameNews.Num_Wrong_Game)
								bWaitSubject = FALSE;;
							}
                        (void)act_do( gkt_arry.Action, 0 );
                        HintAnswerIndex = AnswerCur;
                        game_wrong( 0 );
                     }
                     else
                     {
                        /* find at least one odd have not link,
                        new link have get,2 state */
//                        SendString("888\r\n");
                        ErrCur = 0;
                        AnswerCur = i;
                        HintAnswerIndex = i;
                        bWaitSubject = FALSE;
                        (void)act_do( gkt_arry.Action, 0 );
                        (void)act_do( Current_GameKeyConcent[ AnswerCur ].Act_Right, 0 );
                     }
                  }
                  else
                  {
                     //is in even position,but not correct answer
                     if(Current_Game.Type_Game != NEW_CONNECT_TYPE)
                     	{
 //                    	    SendString("901\r\n");
                     		(void)act_do( gkt_arry.Action, 0 );
                     		HintAnswerIndex = AnswerCur;
                     		game_wrong( 0 );
                     	}
					 else
					 	{
//					 	    act_do( gkt.Action, 0 );
                     		HintAnswerIndex = AnswerCur;
//							SendString("911\r\n");
							if((ErrCur+1)>= Current_GameNews.Num_Wrong_Game)
								{
//								 SendString("919\r\n");
							    	bWaitSubject = TRUE;
								}
                     		game_wrong( 0 );
					 	}
                  }
               }
               else
               {
                  // click wrong answer index
//                  SendString("920\r\n");
                  if(Current_Game.Type_Game == NEW_CONNECT_TYPE)
                  (void)act_do( gkt_arry.Action, 0 );
                  HintAnswerIndex = AnswerCur;							
                  game_wrong( 0 );
               }
            }
         }
         else
         {
            // Isn't in answer list
//            SendString("1509\r\n");
            QuestionIndex = 0;
            HintAnswerIndex = AnswerCur;					
            game_wrong( 0 );
         }
      }
}
#pragma CODE = ANSWER_LOGIC_NORMAL_CODE
//judge the answer index after find answer
void answer_logic_normal( void )
{
 //  _MEM_TYPE_SLOW_ Game_key_Table gkt;
 //  Bool bPass;
 //  _MEM_TYPE_SLOW_ U8 i;

//	_MEM_TYPE_SLOW_ U16	temp_num;
//	_MEM_TYPE_SLOW_ U32 temp_add;


   //memmove( (char *)&gkt, Addr, sizeof( Game_key_Table ));
   switch( Current_Game.Type_Game )
   {
      case SORT_TYPE:
	  SDKLoadCode(ANSWER_LOGIC_SORT_CODE);
	  game_sort_type();
      break;

      case COMBINED_TYPE:
	  case COUNT_TYPE:
	  SDKLoadCode(ANSWER_LOGIC_COMBINE_CODE);
	  game_combine_type();
      break;

      case CONNECT_TYPE:
	  case NEW_CONNECT_TYPE:
	   if( bWaitSubject )
      {
	  	SDKLoadCode(ANSWER_LOGIC_CONNECT01_CODE);
	  	game_connect01_type();
	   	}
	   else{
	   	SDKLoadCode(ANSWER_LOGIC_CONNECT02_CODE);
	  	game_connect02_type();
	   	}
      break;
   }
}


#pragma CODE = ANSWER_LOGIC_CONVERSE_CODE


//judge the answer index after find answer
//void answer_logic_converse( U8 _MEM_TYPE_SLOW_ *Addr )
void answer_logic_converse( void )
{
   //_MEM_TYPE_SLOW_ Game_key_Table gkt;
   _MEM_TYPE_SLOW_ U8 /*iCur, */j, cindex[ MAX_GAME_ANSWER ];
   _MEM_TYPE_SLOW_ U32 Address;
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ Game_Project_Table iGameMsg;
   Bool bFind;
#ifdef TX_DEBUG
   SendString("\r\nanswer_logic_converse!");
#endif   
   //memmove( &gkt, Addr, sizeof( Game_key_Table ));
   if( bSearchFound )
   {
      //memmove( &gkt, Addr, sizeof( Game_key_Table ));
      (void)act_do( gkt_arry.Action, 0 );
      /*iCur = gkt.Index_key;
      push_back( iCur );*/
      //push_back( gkt.Index_key );
#if 0//def TX_DEBUG
   SendString("\r\n1600");
#endif        
      Current_GameKeyIndex[ AnswerIndex ] = gkt_arry.Index_key;
      AnswerIndex = ( AnswerIndex + 1 )& 0x1F;
      
      Address = Current_GameNews.Point_Game_Project;
      for( i = 0; i < Current_GameNews.Num_Game; i++ )
      {
         bFind = TRUE;
         //read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&iGameMsg, sizeof( Game_Project_Table ));
         
         read_current_game_news( Address, &iGameMsg );
         read_data_from_nf( iGameMsg.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&cindex, sizeof( U8 )* iGameMsg.Num_Key );
         for( j = 0; j < iGameMsg.Num_Key; j++ )
         {
            if( Current_GameKeyIndex[( AnswerIndex - iGameMsg.Num_Key + j )& 0x1F ] != cindex[ j ])
            {
               //Address += sizeof( Game_Project_Table );
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
         (void)act_do( iGameMsg.Act_Game_Concent, 0 );
         RightNumber++;
      }
   }
   else
   {
      (void)act_do( News_Go.Action_NoVoice, 0 );
   }
}



#pragma CODE = GAME_INITIAL_CODE

// initial game information
Bool game_initial( void )
{
//   U32 temp_data = 0;

//    bInGame = TRUE;
//	bInGame = FALSE;
//	bSearchFound = TRUE;
//	bSearchFound = FALSE;
	
   if( !News_Control.Point )
      return FALSE;
#ifdef TX_DEBUG
   SendString("News_Control.Point=");send_hex_u32(News_Control.Point);
#endif   
   read_data_from_nf( News_Control.Point, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews, sizeof(U16)*6);
   (void)file_seek( (News_Control.Point+12), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Num_Every_Game, sizeof(U8));
   (void)file_seek( (News_Control.Point+13), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Num_Wrong_Game, sizeof(U8));
   (void)file_seek( (News_Control.Point+14), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Act_Right, sizeof(U16)*4);
   (void)file_seek( (News_Control.Point+22), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Fuction_Question, sizeof(U8));
   (void)file_seek( (News_Control.Point+23), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Time, sizeof(U16)*3);
   (void)file_seek( (News_Control.Point+29), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Mast_Game, sizeof(U8));

  if(All_Control.Version >= 7)
  	{
   	(void)file_seek( (News_Control.Point+30), FS_SEEK_SET );
   	(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Addr_Extern_Project_Game, sizeof(U8)*4);

  	}
   
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
   

   GameTipTick = 0;
   GameTimeOverTick = 0;
   game_timer_stop = 0;
   /*bTipPlay = FALSE;
   bGameTimeOverPlay = FALSE;
	bTimerEnable = FALSE;
   bTimeOverEnable = FALSE;
   bNewLink = FALSE;
   bGameFirst = TRUE;
   bGameSpeech = TRUE;*/
   bGameStatus.value = 0x0018;
   
   /*bNotAutoNextGame = FALSE;
   bGamePrev = FALSE;
   bExitGamePRN = FALSE;
   bQuestion = FALSE;*/
   game_bits.value = 0;
   bSpeechEnd = TRUE;
   
   if( Current_GameNews.Time )
   {
#ifdef TX_DEBUG
   SendString("\r\nCurrent_GameNews.Time=");send_hex_u32(Current_GameNews.Time);
#endif   
      bTimerEnable = TRUE;
	  game_timer_stop = 1;
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
#ifdef TX_DEBUG
   SendString("\r\nCurrent_GameNews.OverTime_Game=");send_hex_u32(Current_GameNews.OverTime_Game);
#endif       
         bTimeOverEnable = TRUE;
		 game_timer_stop = 1;
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
//      SendString("\r\n");
//      send_hex_u8(bNewLink);
//	  send_hex_u8(bExitGamePRN);
//	  send_hex_u8(bQuestion);
//	  send_hex_u8(bNotAutoNextGame);
//	  SendString("\r\n");
   return TRUE;
}

#pragma CODE = GAME_Script_CODE
/*F**************************************************************************
* NAME:script_play
* PARAMS:script play funtion
* EXAMPLE:
* NOTE: modify "CurrentScriptAddress"
* REQUIREMENTS:
*****************************************************************************/
void script_play(void)
{
	Uint8 Count;
	Uint32 SpeechAddress;
//	read_book_data(ScriptCurrentAddress,&Count,sizeof(Count));;
	read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
	ScriptCurrentAddress+=sizeof(Count);
	for(;Count;Count--)
	{
//		read_book_data(ScriptCurrentAddress,&SpeechAddress,sizeof(SpeechAddress));
//		play_speech(SpeechAddress);
//		read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_*)&SpeechAddress, sizeof( SpeechAddress ));
		write_media_addr(ScriptCurrentAddress,0);
		ScriptCurrentAddress+=9;
	}
}
/*F**************************************************************************
* NAME:script_play_random
* PARAMS:script play random funtion
* NOTE: modify "CurrentScriptAddress"
* REQUIREMENTS:
*****************************************************************************/
void script_play_random(void)
{
	Uint8 Count,i;
	Uint32 SpeechAddress;
	read_data_from_nf( ScriptCurrentAddress, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
	ScriptCurrentAddress+=sizeof(Count);
//	init_rand(Count,RAND_GAME);
//	i=get_rand(Count,RAND_GAME);
//		i = rand_choose(0,Count-1);
	i = rand()%(Count-1);

//	read_book_data((ScriptCurrentAddress+4*i),&SpeechAddress,sizeof(SpeechAddress));
//	read_data_from_nf( (ScriptCurrentAddress+4*i), ( U8 _MEM_TYPE_SLOW_*)&SpeechAddress, sizeof( SpeechAddress ));
	
	write_media_addr((ScriptCurrentAddress+9*i),0);
	ScriptCurrentAddress+=9*Count;
}

/*F**************************************************************************
* NAME:script_play_number
* PARAMS:script play number funtion
* NOTE: modify "CurrentScriptAddress"
* REQUIREMENTS:
*****************************************************************************/
#if 1
void script_play_number(U16 number)
{
	U32 Address;
	U32 SpeechAddress;
	U16 Count;
	if(News_Book.Point_ScritNumber==0)return;
	Address = News_Book.Point_ScritNumber;
//	read_book_data(Address,&Count,sizeof(Count));
	read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));
	if((Count==0)||(number>=Count))
	{
		return;
	}
	
	Address+=sizeof(Count)+9*number;
//	read_book_data(Address,&SpeechAddress,sizeof(SpeechAddress));
	write_media_addr(Address,0);
}
#endif
/*F**************************************************************************
* NAME:script_get_cmd
* PARAMS:script decode funtion
* NOTE: use global variable "ScriptCmd",modify "CurrentScriptAddress"
* REQUIREMENTS:
*****************************************************************************/
void script_get_cmd(void)
{
	U16 temp_data;
	read_data_from_nf(ScriptCurrentAddress,( U8 _MEM_TYPE_SLOW_*)&temp_data,sizeof(U16));
	ScriptCmd.Command = (U8)(temp_data>>8);
	ScriptCmd.Type = (U8)temp_data;
	ScriptCurrentAddress+=2;
}
/*F**************************************************************************
* NAME:script_get_address
* PARAMS:script get address
* EXAMPLE:
* NOTE:
* REQUIREMENTS:
*****************************************************************************/
void script_get_address(void)
{
	read_data_from_nf(ScriptCurrentAddress,( U8 _MEM_TYPE_SLOW_*)&ScriptOffsetAddress,sizeof(U16));
	ScriptCurrentAddress+=sizeof(ScriptOffsetAddress);
}
/*F**************************************************************************
* NAME:script_get_number
* PARAMS:script get parameter
* NOTE:
* REQUIREMENTS:
*****************************************************************************/
void script_get_number(void)
{
	read_data_from_nf(ScriptCurrentAddress,( U8 _MEM_TYPE_SLOW_*)&ScriptNumber,sizeof(U16));
	ScriptCurrentAddress+=sizeof(ScriptNumber);
}
/*F**************************************************************************
* NAME:script_jmp
* PARAMS:script update address
* NOTE:
* REQUIREMENTS:
*****************************************************************************/
void script_jmp(void)
{
	ScriptCurrentAddress=ScriptCurrentAddress_bak+(Uint32)ScriptOffsetAddress;
}
/*F**************************************************************************
* NAME:script_get_para
* PARAMS:script get parameter
* EXAMPLE:
* NOTE:use global variable "ScriptPara",modify "CurrentScriptAddress"
* REQUIREMENTS:
*****************************************************************************/
void script_get_para(void)
{
	read_data_from_nf(ScriptCurrentAddress,( U8 _MEM_TYPE_SLOW_*)&ScriptPara,sizeof(U16)*2);
	ScriptCurrentAddress+=sizeof(SCRIPT_PARA);	
}
/*F**************************************************************************
* NAME:clear_script_variable
* PARAMS:clear all script variable
* NOTE:	This function shall be excuted after page load.
* REQUIREMENTS:
*****************************************************************************/
void clear_script_variable(void)
{
	Uint8 i;
	for(i=0;i<SCRIPT_VARIABLE_NUMBER;i++)
		ScriptVariable[i] = 0;
}
/*F**************************************************************************
* NAME:script_decode
* PARAMS:script decode funtion
* REQUIREMENTS:
*****************************************************************************/
void script_init(void)
{
	ScriptCurrentAddress = News_Control.Point;	
	ScriptCurrentAddress_bak = ScriptCurrentAddress;
}

/*F**************************************************************************
* NAME:script_decode
* PARAMS:script decode funtion
* REQUIREMENTS:
*****************************************************************************/
void script_decode(void)
{

	for(;;)
	{
		script_get_cmd();					// get script command.
		switch(ScriptCmd.Command)
		{
			case CMD_CLEARVER:
			{
				clear_script_variable();
				break;
			}
			case CMD_SET:		
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] = ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{   
						if(ScriptPara.Parameter2==99)
						   {
						     ScriptVariable[ScriptPara.Parameter1] = ScriptVariable[99];
						   }
						else if(ScriptPara.Parameter2==98)
						   {
						     ScriptVariable[ScriptPara.Parameter1]=rand();
						   }
						else
						{						
							ScriptVariable[ScriptPara.Parameter1] = ScriptVariable[ScriptPara.Parameter2];
						}
						break;

					}
					default:
					break;
				}
				break;
			}
			case CMD_CMP:		
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						(ScriptVariable[ScriptPara.Parameter1]  \
							>ScriptPara.Parameter2)?(ScriptTest=1):\
							(ScriptVariable[ScriptPara.Parameter1]\
							<ScriptPara.Parameter2)?(ScriptTest=-1):(ScriptTest=0);
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						(ScriptVariable[ScriptPara.Parameter1]  \
							>ScriptVariable[ScriptPara.Parameter2])?(ScriptTest=1):\
							(ScriptVariable[ScriptPara.Parameter1]\
							<ScriptVariable[ScriptPara.Parameter2])?(ScriptTest=-1):(ScriptTest=0);
						break;
					}
					default:
					break;
				}				
				break;
			}				
			case CMD_AND:
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] &= ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						ScriptVariable[ScriptPara.Parameter1] &= ScriptVariable[ScriptPara.Parameter2];
						break;
					}
					default:
					break;
				}					
				break;
			}				
      		case CMD_OR:
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] |= ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						ScriptVariable[ScriptPara.Parameter1] |= ScriptVariable[ScriptPara.Parameter2];
						break;
					}
					default:
					break;
				}					
				break;
			}				
 			case CMD_NOT:	
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] =~ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						ScriptVariable[ScriptPara.Parameter1] =~ScriptVariable[ScriptPara.Parameter2];
						break;
					}
					default:
					break;
				}	
				break;
 			}
			case CMD_TEST:
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						((ScriptVariable[ScriptPara.Parameter1]\
						& ScriptPara.Parameter2)==ScriptPara.Parameter2)?\
						(ScriptTest = TEST_EQUAL):(ScriptTest=TEST_NOT_EQUAL);
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						((ScriptVariable[ScriptPara.Parameter1]\
						& ScriptVariable[ScriptPara.Parameter2])\
						==ScriptVariable[ScriptPara.Parameter2])?\
						(ScriptTest = TEST_EQUAL):(ScriptTest=(TEST_NOT_EQUAL));
						break;

					}
					default:
					break;
				}	
				break;
 			}		
			case CMD_ADD:	
			{
				script_get_para();				
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] += ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						ScriptVariable[ScriptPara.Parameter1] += ScriptVariable[ScriptPara.Parameter2];
						break;
					}
					default:
					break;
				}	
				break;
 			}				
			case CMD_SUB:	
			{
				script_get_para();
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						ScriptVariable[ScriptPara.Parameter1] -= ScriptPara.Parameter2;
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						ScriptVariable[ScriptPara.Parameter1] -= ScriptVariable[ScriptPara.Parameter2];
						break;
					}
					default:
					break;
				}	
				break;
 			}				
			case CMD_JMP:
			{
				script_get_address();
				script_jmp();
				break;
			}
			case CMD_JE:
			{
				script_get_address();
				if(CMP_EQUAL==ScriptTest){
					script_jmp();
				}
				break;
			}
			case CMD_JNE:	
			{
				script_get_address();
				if(CMP_EQUAL!=ScriptTest){
					script_jmp();
				}
				break;
			}				
			case CMD_JG:	
			{
				script_get_address();
				if(CMP_GREAT==ScriptTest){
					script_jmp();
				}
				break;
			}				
			case CMD_JGE:
			{
				script_get_address();
				if((CMP_EQUAL==ScriptTest)||(CMP_GREAT==ScriptTest)){
					script_jmp();
				}
				break;
			}				
			case CMD_JB:	
			{
				script_get_address();
				if(CMP_LESS==ScriptTest){
					script_jmp();
				}
				break;
			}
			case CMD_JBE:	
			{
				script_get_address();
				if((CMP_EQUAL==ScriptTest)||(CMP_LESS==ScriptTest)){
					script_jmp();
				}
				break;
			}
			case CMD_PLAY:	
			{
				script_play();
				break;
			}
			case CMD_PLAYRANDOM:		
			{
				script_play_random();
				break;
			}
			case CMD_PLAYNUMBER:
			{
				script_get_number();
				switch(ScriptCmd.Type)
				{
					case CMD_TYPE_VER_CONST:
					{
						script_play_number(ScriptNumber);
						break;
					}
					case CMD_TYPE_VER_VER:
					{
						script_play_number(ScriptVariable[ScriptNumber]); 
						break;
					}
					default:
					break;
				}	
				break;
			}
			case CMD_END:
			//! return this time script,not break;		
			default:
			{
				//! error script command,exit
				return;
				break;
			}
		}
	}

}

#endif  //#if( AP3 == ENABLE )

#endif  //_search_game_c_
