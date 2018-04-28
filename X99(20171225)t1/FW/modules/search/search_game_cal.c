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


#ifndef _search_game_cal_c_
#define _search_game_cal_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "search.h"
#include "search_game.h"
#include "modules\search\search_game_cal.h"
#include "search_remote.h"
#include "conf\conf_oid.h"
#include "string.h"
#include <stdlib.h> // Contains Funktion "rand()"
#include "lib_mcu\timer\timer_drv.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "mmi\ap3\mmi_ap3.h"
#include "mmi\player\srv_player.h"
#include "mmi\shared\com_evt.h"
#include "modules\file_system\file.h"
#include "modules\player_ap3\player_ap3.h"
#include "modules/text/text_viewer.h"
#include "conf\conf_mmi.h"
//#include "conf\conf_kbd.h"                   // Keyboard configuration
#include "modules\player\player_task.h"
#include <intrins.h>
#include <math.h>

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

//***********************************************

//_____ D E C L A R A T I O N ______________________________________________
void     print_key_char             ( U8 x, U8 y, U8 asc, U8 iasc );
void     print_word_lettle          ( U8 count );
void     print_lettle_cance         ( U8 count );
void     print_word_identifier      ( Bool inverse );
void     print_clear_input          ( void );

void     play_float                 ( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length );
void     calculate_old              ( void );
void     display_operation          ( void );

//***********************************************
void  initial_console               ( void );


void initial_console( void )
{
   GameState = GAME_LETTLE1;
   lettle_count = 0;
   clr_input_buffer();
   print_dic_screen();
   print_lettle_screen( lettle_count );
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
   _MEM_TYPE_SLOW_ Game_key_Table gkt_arry[ length_gkt ];
   U8  _MEM_TYPE_SLOW_ j, n, m;
   U16 _MEM_TYPE_SLOW_ i;
	
	bAnswerWrong = FALSE;
   Address = Current_GameNews.Point_Game_keyTable;
   if( !Address )
   {
      return;
   }
   if( !Current_GameNews.Num_Game_keyTable )
      return;
   i = Current_GameNews.Num_Game_keyTable / length_gkt;
   j = Current_GameNews.Num_Game_keyTable % length_gkt;
   /*if( i == 0 )
   {
      read_data_from_nf( Address, ( U8 *)&gkt_arry, ( U16 )sizeof( Game_key_Table )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         bSearchFound = TRUE;
         for( m = 0; m < WORD_MAX; m++ )
         {
            if( word[ m ] != gkt_arry[ n ].caption[ m ] )
            {
               bSearchFound = FALSE;
               break;
            }
         }
         if( bSearchFound )
            goto do_game_next;
      }
   }
   else*/
   //{
      while( i )
      {
         read_data_from_nf( Address, ( U8 *)&gkt_arry, ( U16 )sizeof( Game_key_Table )*( U16 )length_gkt );
         for( n = 0; n < length_gkt; n++ )
         {
            bSearchFound = TRUE;
            for( m = 0; m < WORD_MAX; m++ )
            {
               if( word[ m ] != gkt_arry[ n ].caption[ m ] )
               {
                  bSearchFound = FALSE;
                  break;
               }
            }
            if( bSearchFound )
               goto do_game_next;
         }
         i--;
         Address += ( U32 )sizeof( Game_key_Table )* length_gkt;
      }
      //if( j == 0 )
         //goto do_game_next;
      read_data_from_nf( Address, ( U8 *)&gkt_arry, ( U16 )sizeof( Game_key_Table )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         bSearchFound = TRUE;
         for( m = 0; m < WORD_MAX; m++ )
         {
            if( word[ m ] != gkt_arry[ n ].caption[ m ] )
            {
               bSearchFound = FALSE;
               break;
            }
         }
         if( bSearchFound )
            goto do_game_next;
      }
   //}
   
do_game_next:
   //if( bSearchFound )
   //{
      answer_logic_normal(( U8 _MEM_TYPE_SLOW_ *)&gkt_arry[ n ]);
   //}
   //else
   //{
      //game_wrong( 0 );
   //}
   if( News_Control.Type_Control == GAME_TYPE )
   {
      if( !bAnswerWrong )
      {
         initial_console();
      }
   }
}


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
   if( word_count < WORD_MAX )
   {
      if((( CurrentKey >= 'a' )&&( CurrentKey <= 'z' ))
         ||(( CurrentKey >= '0' )&&( CurrentKey <= '9' )))
      {
         word[ word_count ] = CurrentKey;
         print_word_lettle( word_count );
         word_count++;
         //bSpeechEnd = TRUE;
         return;
      }
   }
   if( CurrentKey == RM_ENTER )
   {
      if( word_count )
         do_game();
      else
      {
         game_wrong( 0 );
         print_clear_input();
      }
      state_search_to_play();
      //bSpeechEnd = TRUE;
      return;
   }
   if( CurrentKey == RM_CANCLE )
   {
      print_clear_input();
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
         print_lettle_cance( word_count );
      }
      //bSpeechEnd = TRUE;
      return;
   }
   if( CurrentKey == RM_DOT )
   {
      if( word_count < WORD_MAX )
      {
         word[ word_count ] = '\'';
         print_word_lettle( word_count );
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


//play float number
void play_float( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length )
{
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;

   if( point_length )
   {
      ulen = int_length + point_length - 1;
      for( i = 0; i < point_length; i++ )
      {
         if( s[ ulen ]!= '0' )
            break;
         s[ ulen ] = 0;
         ulen--;
      }
      if( i == point_length )
         s[ ulen ] = 0;
      int_length--;
   }
   else
   {
      int_length--;
      s[ int_length ] = 0;
   }

   point_length = int_length;
   if( s[ 0 ] == '-' )
   {
      play_RMT_speech( RemoteSpeech.SpeechNeg );
      temp = 1;
      int_length--;
   }
   else
      temp = 0;

   if( RemoteVersion == 0x01 )
      goto play_float_v1;
   if( !( RemoteSpeech.SpeechTen || RemoteSpeech.SpeechHundred || RemoteSpeech.SpeechThousand || RemoteSpeech.SpeechTenThousand ))
      goto play_float_v1;
   
   while(( s[ temp ] != 0 )&& int_length )
   {
      switch( int_length )
      {
         case 0x07:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         play_RMT_speech( RemoteSpeech.SpeechHundred );
         temp++;
         if( check_zero_number( &s[ temp ], 6 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         break;

         case 0x06:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
            {
               play_RMT_speech( RemoteSpeech.Number[ i ]);
               play_RMT_speech( RemoteSpeech.SpeechTen );
            }
            else
            {
               if( !check_zero_number( &s[ temp + 1 ], 1 ))
                  play_RMT_speech( RemoteSpeech.Number[ i ]);
            }
         }
         temp++;
         if( check_zero_number( &s[ temp ], 5 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x05:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
               play_RMT_speech( RemoteSpeech.Number[ i ]);
         }
         play_RMT_speech( RemoteSpeech.SpeechTenThousand );
         temp++;
         if( check_zero_number( &s[ temp ], 4 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x04:
         zero_flag = FALSE;
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechThousand );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 3 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x03:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechHundred );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 2 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x02:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechTen );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 1 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x01:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         int_length--;
         break;

         default:
         return;
      }
   }

   temp = point_length;

play_float_v1:
   while( s[ temp ] != 0 )
   {
      if( s[ temp ] == '.')
         play_RMT_speech( RemoteSpeech.SpeechDot );
      else
      {
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
      }
      temp++;
   }
}


void cal_act_do( void )
{
   switch( cal_state )
   {
      case CAL_STATE_NUM1:
      if((( RemoteValue >= REMOTE_KEY_0 )
         &&( RemoteValue <= REMOTE_KEY_9 ))
            ||( RemoteValue == REMOTE_CAL_BACK )
               ||( RemoteValue == REMOTE_KEY_DOT ))
      {
         if( bCalClear )
         {
            CalNum1 = 0.0;
            bCalClear = FALSE;
         }
         if( RemoteValue == REMOTE_CAL_BACK )
         {
            //if( CalNum1 )
            if( _chkfloat_( CalNum1 )== 0 )
            {
               if( !bCalPoint )
                  CalNum1 = ( U32 )CalNum1 / 10;
               else
               {
                  switch( cal_point_length )
                  {
                     case 0x00:
                     bCalPoint = FALSE;
                     CalNum1 = ( U32 )CalNum1 / 10;
                     break;

                     case 0x01:
                     cal_point_length--;
                     bCalPoint = FALSE;
                     CalNum1 = ( U32 )CalNum1;
                     break;

                     case 0x02:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10 ))/ 10.0;
                     break;

                     case 0x03:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100 ))/ 100.0;
                     break;

                     case 0x04:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000 ))/ 1000.0;
                     break;
#if 0
                     case 0x05:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 ))/ 10000.0;
                     break;

                     case 0x06:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100000 ))/ 100000.0;
                     break;

                     case 0x07:
                     cal_point_length--;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000000 ))/ 1000000.0;
                     break;
#endif
                  }
               }
            }
            else
            {
               if( bCalPoint )
               {
                  cal_point_length = 0;
                  bCalPoint = FALSE;
               }
            }
            display_float( CalNum1, DIS_RESULT_ONLY );
            if( cal_input_length )
               cal_input_length--;
            break;
         }
         //if( cal_input_length >= 11 )
         if( cal_input_length >= 7 )
            break;
         if( RemoteValue == REMOTE_KEY_DOT )
         {
            bCalPoint = TRUE;
            break;
         }
         if( bCalPoint )
         {
            //if( cal_point_length >= 4 )
            if(( cal_point_length >= 4 )
               ||(( cal_point_length + cal_int_length )>= 7 ))
               break;
            cal_point_length++;
            cal_input_length++;
            switch( cal_point_length )
            {
               case 0x01:
               //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10.0;
               CalNum1 = (( U32 )CalNum1 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
               break;

               case 0x02:
               //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100.0;
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
               break;

               case 0x03:
               //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 1000.0;
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
               break;

               case 0x04:
               //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10000.0;
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
               break;
#if 0
               case 0x05:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
               break;

               case 0x06:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000000.0;
               break;

               case 0x07:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000000.0;
               break;
#endif
            }
         }
         else
         {
            if( cal_int_length >= 7 )
               break;
            cal_input_length++;
            cal_int_length++;
            CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
         }
         display_float( CalNum1, DIS_RESULT_ONLY );
         break;
      }
      switch( RemoteValue )
      {
         case REMOTE_CAL_M_ADD:
         cal_memory_num += CalNum1;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_SUB:
         cal_memory_num -= CalNum1;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_READ:
         display_float( cal_memory_num, DIS_RESULT_ONLY );
         CalNum1 = cal_memory_num;
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_CLEAR:
         cal_memory_num = 0.0;
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_KEY_PLUS:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         CalOP = RemoteValue;
         CalNum2 = 0.0;
         cal_state = CAL_STATE_NUM2;
         display_float( CalNum2, DIS_RESULT_ONLY );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "+" );
         break;

         case REMOTE_KEY_MINUS:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         CalOP = RemoteValue;
         CalNum2 = 0.0;
         cal_state = CAL_STATE_NUM2;
         display_float( CalNum2, DIS_RESULT_ONLY );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "-" );
         break;

         case REMOTE_KEY_MUL:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         CalOP = RemoteValue;
         CalNum2 = 0.0;
         cal_state = CAL_STATE_NUM2;
         display_float( CalNum2, DIS_RESULT_ONLY );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "*" );
         break;

         case REMOTE_KEY_DIV:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         CalOP = RemoteValue;
         CalNum2 = 0.0;
         cal_state = CAL_STATE_NUM2;
         display_float( CalNum2, DIS_RESULT_ONLY );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "/" );
         break;

         case REMOTE_KEY_EQUAL:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         calculate_old();
         CalOP = REMOTE_KEY_NULL;
         bCalClear = TRUE;
         display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "=" );
         //play_float( CalNum1 );
         break;
      }
      break;

      case CAL_STATE_NUM2:
      if((( RemoteValue >= REMOTE_KEY_0 )
         &&( RemoteValue <= REMOTE_KEY_9 ))
            ||( RemoteValue == REMOTE_CAL_BACK )
               ||( RemoteValue == REMOTE_KEY_DOT ))
      {
         if( bCalMemory )
         {
            CalNum2 = 0.0;
            bCalMemory = FALSE;
         }
         if( RemoteValue == REMOTE_CAL_BACK )
         {
            //if( CalNum2 )
            if( _chkfloat_( CalNum2 )== 0 )
            {
               if( !bCalPoint )
                  CalNum2 = ( U32 )CalNum2 / 10;
               else
               {
                  switch( cal_point_length )
                  {
                     case 0x00:
                     bCalPoint = FALSE;
                     CalNum2 = ( U32 )CalNum2 / 10;
                     break;

                     case 0x01:
                     cal_point_length--;
                     bCalPoint = FALSE;
                     CalNum2 = ( U32 )CalNum2;
                     break;

                     case 0x02:
                     cal_point_length--;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10 ))/ 10.0;
                     break;

                     case 0x03:
                     cal_point_length--;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 100 ))/ 100.0;
                     break;

                     case 0x04:
                     cal_point_length--;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 1000 ))/ 1000.0;
                     break;
#if 0
                     case 0x05:
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
                     break;

                     case 0x06:
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000000.0;
                     break;

                     case 0x07:
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000000.0;
                     break;
#endif
                  }
               }
            }
            else
            {
               if( bCalPoint )
               {
                  cal_point_length = 0;
                  bCalPoint = FALSE;
               }
               else
               {
                  CalNum1 = 0.0;
                  CalNum2 = 0.0;
                  CalOP = REMOTE_KEY_NULL;
                  cal_state = CAL_STATE_NUM1;
                  cal_input_length = 0;
                  cal_point_length = 0;
                  cal_int_length = 0;
                  /*bNewCal = FALSE;
                  bCalErr = FALSE;
                  bRmoteExitAp4 = FALSE;
                  bCalPoint = FALSE;*/
                  remote_bits.value &= ~0x4414u;//0x2822;
                  /*bCalClear = TRUE;
                  bInCal = TRUE;*/
                  remote_bits.value |= 0x0300u;//0x00C0;
                  display_float( CalNum1, DIS_RESULT_ONLY );
                  break;
               }
            }
            if( cal_input_length )
               cal_input_length--;
            bNewCal = TRUE;
            display_float( CalNum2, DIS_RESULT_ONLY );
            display_operation();
            break;
         }
         //if( cal_input_length >= 11 )
         if( cal_input_length >= 7 )
            break;
         if( RemoteValue == REMOTE_KEY_DOT )
         {
            bCalPoint = TRUE;
            break;
         }
         if( bCalPoint )
         {
            //if( cal_point_length >= 4 )
            if(( cal_point_length >= 4 )
               ||(( cal_point_length + cal_int_length )>= 7 ))
               break;
            cal_point_length++;
            cal_input_length++;
            switch( cal_point_length )
            {
               case 0x01:
               //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10.0;
               CalNum2 = ( Float16 )(( U32 )CalNum2 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
               break;

               case 0x02:
               //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100.0;
               CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
               break;

               case 0x03:
               //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 1000.0;
               CalNum2 = ( Float16 )(( U32 )( CalNum2 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
               break;

               case 0x04:
               //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10000.0;
               CalNum2 = ( Float16 )(( U32 )( CalNum2 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
               break;
#if 0
               case 0x05:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
               break;

               case 0x06:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000000.0;
               break;

               case 0x07:
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000000.0;
               break;
#endif
            }
         }
         else
         {
            if( cal_int_length >= 7 )
               break;
            cal_input_length++;
            cal_int_length++;
            CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
         }
         bNewCal = TRUE;
         display_float( CalNum2, DIS_RESULT_ONLY );
         display_operation();
      }
      switch( RemoteValue )
      {
         case REMOTE_CAL_M_ADD:
         cal_memory_num += CalNum2;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_SUB:
         cal_memory_num -= CalNum2;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_READ:
         display_float( cal_memory_num, DIS_RESULT_ONLY );
         CalNum2 = cal_memory_num;
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_CLEAR:
         cal_memory_num = 0.0;
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_KEY_PLUS:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         if( bNewCal )
         {
            calculate_old();
            display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
            if( bCalErr )
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
               clear_cal_error();
               break;
            }
         }
         bNewCal = FALSE;
         CalNum2 = 0.0;
         CalOP = RemoteValue;
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "+" );
         break;

         case REMOTE_KEY_MINUS:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         if( bNewCal )
         {
            calculate_old();
            display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
            if( bCalErr )
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
               clear_cal_error();
               break;
            }
         }
         bNewCal = FALSE;
         CalNum2 = 0.0;
         CalOP = RemoteValue;
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "-" );
         break;

         case REMOTE_KEY_MUL:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         if( bNewCal )
         {
            calculate_old();
            display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
            if( bCalErr )
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
               clear_cal_error();
               break;
            }
         }
         bNewCal = FALSE;
         CalNum2 = 0.0;
         CalOP = RemoteValue;
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "*" );
         break;

         case REMOTE_KEY_DIV:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         if( bNewCal )
         {
            calculate_old();
            display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
            if( bCalErr )
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
               clear_cal_error();
               break;
            }
         }
         bNewCal = FALSE;
         CalNum2 = 0.0;
         CalOP = RemoteValue;
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "/" );
         break;

         case REMOTE_KEY_EQUAL:
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;
         calculate_old();
         CalOP = REMOTE_KEY_NULL;
         bCalClear = TRUE;
         cal_state = CAL_STATE_NUM1;
         display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts( "=" );
         //play_float( CalNum1 );
         break;
      }
      break;
   }
}


void display_operation( void )
{
   lcd_goto_xy( 0, 16 );
   Ih_set_curr_font( IDC_FONT_16 );
   switch( CalOP )
   {
      case REMOTE_KEY_PLUS:
      lcd_puts( "+" );
      break;

      case REMOTE_KEY_MINUS:
      lcd_puts( "-" );
      break;

      case REMOTE_KEY_MUL:
      lcd_puts( "*" );
      break;

      case REMOTE_KEY_DIV:
      lcd_puts( "/" );
      break;

   }
}

#if 1
//calculate old result
void calculate_old( void )
{
   switch( CalOP )
   {
      case REMOTE_KEY_PLUS:
      CalNum1 = CalNum1 + CalNum2;
      break;

      case REMOTE_KEY_MINUS:
      CalNum1 = CalNum1 - CalNum2;
      break;

      case REMOTE_KEY_MUL:
      CalNum1 = CalNum1 * CalNum2;
      break;

      case REMOTE_KEY_DIV:
      if( !CalNum2 )
         //CalNum1 = 0.0;
         CalNum1 = 10000000.0;
      else
         CalNum1 = CalNum1 / CalNum2;
      break;
   }
}


//display float number
void display_float( Float16 result, U8 dis_type )
{
   _MEM_TYPE_SLOW_ U8 s[ 26 ], int_length, point_length;
   _MEM_TYPE_SLOW_ Float16 int_data, point_data;
   _MEM_TYPE_SLOW_ U32 point;
   
   int_length = sprintf( s, "%.4f", result );
   //bCalNagative = FALSE;
   if( s[ 0 ] == '-' )
   {
      if( int_length <= 13 )
      {
         //bCalNagative = TRUE;
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/

         point_data = modf( result, &int_data );
display_float_int1:
         point = ( U32 )( -int_data );
         int_length = sprintf( s + 1, "%lu.", point );
         switch( int_length )
         {
            case 0x02:
            case 0x03:
            case 0x04:
            point = ( U32 )(( -point_data )* 10000 + 0.5 );
            if( point >= 10000 )
            {
               point_data = ( Float16 )( point - 10000 )/ -10000.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -10000.0;
            point_length = sprintf( s + 1 + int_length, "%04lu", point );
            break;

            case 0x05:
            point = ( U32 )(( -point_data )* 1000 + 0.5 );
            if( point >= 1000 )
            {
               point_data = ( Float16 )( point - 1000 )/ -1000.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -1000.0;
            point_length = sprintf( s + 1 + int_length, "%03lu", point );
            break;

            case 0x06:
            point = ( U32 )(( -point_data )* 100 + 0.5 );
            if( point >= 100 )
            {
               point_data = ( Float16 )( point - 100 )/ -100.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -100.0;
            point_length = sprintf( s + 1 + int_length, "%02lu", point );
            break;

            case 0x07:
            point = ( U32 )(( -point_data )* 10 + 0.5 );
            if( point >= 10 )
            {
               point_data = ( Float16 )( point - 10 )/ -10.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -10.0;
            point_length = sprintf( s + 1 + int_length, "%01lu", point );
            break;

            case 0x08:
            point = ( U32 )(( -point_data )+ 0.5 );
            if( point >= 1 )
            {
               point_data = -( Float16 )( point - 1 );
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = -( Float16 )point;
            //length += sprintf( s + 1 + length, "%00lu", point )+ 1;
            point_length = 0;
            break;
         }
         if( dis_type & DIS_RESULT_ONLY )
         {
            lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_goto_xy( 128 -( int_length + 1 + point_length )* 8, 16 );
            lcd_puts( s );
         }
         if( dis_type &( DIS_MEMORY | GET_MEMORY ))
         {
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 0, 8 );
            lcd_puts( "M" );
            cal_memory_num = int_data + point_data;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length + 1, point_length );
         }
      }
      else
      {
         lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         bCalErr = TRUE;
         lcd_goto_xy( 0, 16 );
         lcd_puts( "Error" );
      }
   }
   else
   {
      if( int_length <= 12 )
      {
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/

         point_data = modf( result, &int_data );
         point = ( U32 )int_data;
display_float_int2:
         int_length = sprintf( s, "%lu.", point );
         switch( int_length )
         {
            case 0x02:
            case 0x03:
            case 0x04:
            point = ( U32 )( point_data * 10000 + 0.5 );
            if( point >= 10000 )
            {
               point_data = ( Float16 )( point - 10000 )/ 10000.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 10000.0;
            point_length = sprintf( s + int_length, "%04lu", point );
            break;

            case 0x05:
            point = ( U32 )( point_data * 1000 + 0.5 );
            if( point >= 1000 )
            {
               point_data = ( Float16 )( point - 1000 )/ 1000.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 1000.0;
            point_length = sprintf( s + int_length, "%03lu", point );
            break;

            case 0x06:
            point = ( U32 )( point_data * 100 + 0.5 );
            if( point >= 100 )
            {
               point_data = ( Float16 )( point - 100 )/ 100.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 100.0;
            point_length = sprintf( s + int_length, "%02lu", point );
            break;

            case 0x07:
            point = ( U32 )( point_data * 10 + 0.5 );
            if( point >= 10 )
            {
               point_data = ( Float16 )( point - 10 )/ 10.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 10.0;
            point_length = sprintf( s + int_length, "%01lu", point );
            break;

            case 0x08:
            point = ( U32 )( point_data + 0.5 );
            if( point >= 1 )
            {
               point_data = ( Float16 )( point - 1 );
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point;
            point_length = 0;
            break;
         }
         if( dis_type & DIS_RESULT_ONLY )
         {
            lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_goto_xy( 128 -( int_length + point_length )* 8, 16 );
            lcd_puts( s );
         }
         if( dis_type &( DIS_MEMORY | GET_MEMORY ))
         {
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 0, 8 );
            lcd_puts( "M" );
            cal_memory_num = int_data + point_data;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length, point_length );
         }
      }
      else
      {
         lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         bCalErr = TRUE;
         lcd_goto_xy( 0, 16 );
         lcd_puts( "Error" );
      }
   }
}


void clear_cal_error( void )
{
   CalNum1 = 0.0;
   CalNum2 = 0.0;
   cal_memory_num = 0.0;
   CalOP = REMOTE_KEY_NULL;
   cal_state = CAL_STATE_NUM1;
   cal_input_length = 0;
   cal_point_length = 0;
   cal_int_length = 0;
   /*bNewCal = FALSE;
   bCalErr = FALSE;
   bRmoteExitAp4 = FALSE;
   bCalPoint = FALSE;*/
   remote_bits.value &= ~0x4414u;//0x2822;
   /*bCalClear = TRUE;
   bInCal = TRUE;*/
   remote_bits.value |= 0x0300u;//0x00C0;
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
}
#endif

#endif  //#if( AP3 == ENABLE )

#endif  //_search_game_cal_c_
