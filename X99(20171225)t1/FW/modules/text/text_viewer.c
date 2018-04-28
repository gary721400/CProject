#ifndef _text_viewer_c_
#define _text_viewer_c_

#include "config.h"
#include <stdio.h>
#include <string.h>
#include "mmi/shared/com_var.h"
//-------------文件系统-------------
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
//-------------通信模块-------------
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "mmi/shared/com_var.h"
#include "mmi\shared\com_evt.h"
//-------------系统接口-------------
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"
#include "conf/conf_mmi.h"
//-------------键盘-------------
#include "mmi\kbd\keypad.h"
//----
#include "mmi/player/srv_player.h"
//-------------键盘-------------
#include "mmi\kbd\keypad.h"
//-------------调试工具-------------
//-------------C标准库的字符处理-------------
#include <ctype.h>
//-------------语音接口-------------
#include "modules/player_ap3/player_ap3.h"
//=========================================
#include "text.h"
#include "text_base.h"
#include "text_viewer.h"
#include "text_remote.h"
#include "mmi\dictionary\mmi_dictionary.h"
#include "modules\search\search_remote.h"
#include "modules\power\power.h"             // Power module definition
#include "mmi\player\mmi_player.h"
#include "drivers\mmi\img_handler.h"
#include "drivers\oid\oid.h"
#include "conf\conf_lcd.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "drivers\lcd\lcd_SSD1303.h"
#include "mmi\images\tab_pict.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_sio.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = TEXT_VIEWER_DATA
#pragma CODE = TEXT_VIEWER_CODE

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE00 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE01 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE03 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE04 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );

ImageDeclareType( TEXT_VIEWER_TEMP_CODE00 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE01 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE02 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE03 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE04 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE05 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE06 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE07 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE08 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE09 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE10 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE11 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE12 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE13 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE14 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE15 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE16 );
ImageDeclareType( TEXT_VIEWER_TEMP_CODE17 );

ImageDeclareType( TEXT_VIEWER_REMOTE_CODE );
ImageDeclareType( TEXT_VIEWER_END_CODE );
ImageDeclareType( DIC_SEARCH_WORD_CODE00 );
ImageDeclareType( DIC_SEARCH_WORD_CODE01 );

ImageDeclareType( TEXT_VIEWER_INIT_FAIL_CODE );
ImageDeclareType( TEXT_VIEWER_ERROR_CODE );

ImageDeclareType( TEXT_TEMP_CODE00 );
ImageDeclareType( TEXT_TEMP_CODE01 );
ImageDeclareType( TEXT_TEMP_CODE02 );

ImageDeclareType( TEXT_REMOTE_TEMP_CODE00 );

ImageDeclareType( TEXT_BASE_TEMP_CODE00 );

ImageDeclareType( FS_CREATE_BUF_CODE );

ImageDeclareType( READY_DISPLAY_CODE );

#if( DIC == ENABLE )

U32   _MEM_TYPE_SLOW_   time_out;

/////////////////////////////////////////////////////////////////////////
//Bool                    dictext_g_shine = FALSE;
//U8    _MEM_TYPE_SLOW_   dictext_g_timer_id;

U32   _MEM_TYPE_SLOW_   dictext_g_last_file_pos;
U8    _MEM_TYPE_SLOW_   dictext_g_back_state;
U32   _MEM_TYPE_SLOW_   dictext_g_voice_start_addr;
U32   _MEM_TYPE_SLOW_   dictext_g_voice_end_addr;
U8    _MEM_TYPE_SLOW_   dictext_g_voice_codec;


extern   U8    _MEM_TYPE_SLOW_   dicremote_g_state;
extern char  _MEM_TYPE_SLOW_   tisiFileShortpath[];
extern __X   char     CurrentShortFilePath[];
//U8    _MEM_TYPE_SLOW_   dic_g_repeat_word_count = 0;

/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////



U16   Get_DicFile_Number            ( U8 *sz_filterext );
Bool  Get_DicFile_Name              ( U8 *sz_filterext, U16 Number );
Bool  dic_next_drive                ( void );
void  dic_file_display              ( void );
void  dic_file_select_deal          ( void );
Bool  dic_find_default_file         ( void );
void  dic_no_word_display           ( void );
void  dictext_reset_cmd             ( void );
void  dic_draw_small_input_word     ( void );
void  dic_draw_small_input_cursor   ( Bool inverse );
void  dic_word_find_prepare         ( void );
void  dic_enter_prepare             ( void );
void  dic_remote_voice_end_deal     ( void );
void  dic_word_display              ( void );
Bool  dic_word_voice_deal           ( void );
void  dic_end_exit_deal             ( void );
void  viewer_init_fail_deal         ( void );
void  dic_error_deal                ( void );


U8             _MEM_TYPE_SLOW_   textviewer_g_strings[ TEXTVIEWER_Size_ExtBuffer ];//256
U8             _MEM_TYPE_SLOW_   *textviewer_g_lines_content[ DICTEXT_Screen_Height / 16 ];//4
Text_line_info _MEM_TYPE_SLOW_   textviewer_g_lines_info[ DICTEXT_Screen_Height / 16 ];
U8             _MEM_TYPE_SLOW_   textviewer_g_str_buffer[ TEXTVIEWER_Num_LineLength + 2 ];


U8             _MEM_TYPE_SLOW_   textviewer_g_start_x = 0;
U8             _MEM_TYPE_SLOW_   textviewer_g_start_y = 0;
U8             _MEM_TYPE_SLOW_   textviewer_g_lines = 2;

U8             _MEM_TYPE_SLOW_   textviewer_g_text_type = TEXTBASE_Type_TextFile;
U32            _MEM_TYPE_SLOW_   textviewer_g_start_offset = 0;
U32            _MEM_TYPE_SLOW_   textviewer_g_size = 0;
U8             _MEM_TYPE_SLOW_   textviewer_state_back;





_MEM_TYPE_SLOW_  U16              leng_en;//原文英文长度
_MEM_TYPE_SLOW_  U16              leng_ch;//原文中文长度

_MEM_TYPE_SLOW_  U16             Display_Page_Num;	
_MEM_TYPE_SLOW_  U16             current_display_num;
_MEM_TYPE_SLOW_  U16             current_display_num_bak;
_MEM_TYPE_SLOW_  U32             ch_begin_address;
_MEM_TYPE_SLOW_  U32             ch_display_address;
_MEM_TYPE_SLOW_  U32             ch_display_address_bak;

_MEM_TYPE_SLOW_  U8   			current_dic_short_name[ 32 ];
_MEM_TYPE_SLOW_  U8   			current_dic_dir[ 32 ];

_MEM_TYPE_SLOW_      U16        text_off_addr[ DIC_MAX_PAGE_NUMBER ];
_MEM_TYPE_SLOW_      U8         text_line_buf[ LEN_DISPLAY_BUF ];




#pragma CODE = TEXT_VIEWER_TEMP_CODE12

Bool textviewer_init( U32 _MEM_TYPE_SLOW_ *start_offset, U32 _MEM_TYPE_SLOW_ *size )
{
   U8 _MEM_TYPE_SLOW_ iter,i;
   U8 _MEM_TYPE_SLOW_ *pstr;
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
   {
      textviewer_g_lines_info[ iter ].line_num = TEXT_Num_MaxLines;
      textviewer_g_lines_content[ iter ]= (( U8 _MEM_TYPE_SLOW_ *)textviewer_g_strings)+ iter * TEXTVIEWER_Num_LineLength;
      textviewer_g_lines_content[ iter ][ 0 ]= '\0';
   }
   SDKLoadCode( TEXT_TEMP_CODE00 );
   if( !text_init( start_offset, size, TEXTVIEWER_Num_CharLength * 8 ))
      return FALSE;
   if( text_get_encode()== TEXT_Encode_UTF16BE || text_get_encode()== TEXT_Encode_UTF16LE )
   {
      for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
      {
         SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
         unicode_init_buffer_enc( textviewer_g_lines_content[ iter ], TEXTVIEWER_Num_LineLength, ENCODING_UTF16 );
         pstr = Unicode_get_addr_string( textviewer_g_lines_content[ iter ]);
         if( text_get_encode()== TEXT_Encode_UTF16BE )
         {
            pstr[ 0 ] = 0xFE;
            pstr[ 1 ] = 0xFF;
         }
         else
         {
            pstr[ 0 ] = 0xFF;
            pstr[ 1 ] = 0xFE;
         }
         pstr[ 2 ] = 0x00;
         pstr[ 3 ] = 0x00;
      }
   }
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
   {
      textviewer_g_lines_info[ iter ].line_num = iter;
      if( textviewer_g_lines_info[ iter ].line_num >= TEXT_Num_MaxLines )
         textviewer_g_lines_info[ iter ].line_num = TEXT_Num_MaxLines;
      SDKLoadCode( TEXT_TEMP_CODE01 );
      if( text_get_encode()== TEXT_Encode_ASCII )
      {
         if( !text_read_line( textviewer_g_lines_info[ iter ].line_num, textviewer_g_lines_content[ iter ]))
            return FALSE;
      }
      else
      {
         if( !text_read_line( textviewer_g_lines_info[ iter ].line_num, Unicode_get_addr_string( textviewer_g_lines_content[ iter ])+ 2 ))
            return FALSE;
      }
      if( text_offset()>= text_size())
         break;
   }
  /*
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
   {
     SendString((U8 *)"\r\n iter = ");
	 send_hex_u8(iter);SendString((U8 *)":");
	 for(i=0;i<42;i++){
	 	send_hex_u8(textviewer_g_lines_content[ iter ][ i ]);
	 	SendString((U8 *)":");
	 	}
   }
   //*/
   return TRUE;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE14

/*Bool*/void textviewer_scroll_down( void )
{
   U8 _MEM_TYPE_SLOW_ iter;
   if( textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num >= TEXT_Num_MaxLines )
      return;
      //return TRUE;
   if( text_offset() >= text_size())
      return;
      //return TRUE;
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines - 1; ++iter )
      textviewer_g_lines_info[ iter ] = textviewer_g_lines_info[ iter + 1 ];
   textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num = textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num + 1;
   if( textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num >= TEXT_Num_MaxLines )
      textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num = TEXT_Num_MaxLines;
   SDKLoadCode( TEXT_TEMP_CODE02 );
   if( !text_lines_cache_update( textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num ))
      return;
      //return FALSE;
   SDKLoadCode( TEXT_TEMP_CODE01 );
   if( text_get_encode()== TEXT_Encode_ASCII )
   {
      if( !text_read_line( textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num, textviewer_g_lines_content[ textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num % TEXTVIEWER_Num_ContentLines ]))
         return;
         //return FALSE;
   }
   else
   {
      if( !text_read_line( textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num, Unicode_get_addr_string( textviewer_g_lines_content[ textviewer_g_lines_info[ TEXTVIEWER_Num_ContentLines - 1 ].line_num % TEXTVIEWER_Num_ContentLines ])+ 2 ))
         return;
         //return FALSE;
   }
   //return TRUE;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE15

/*Bool*/void textviewer_scroll_up( void )
{
   U8 _MEM_TYPE_SLOW_ iter;
   if( textviewer_g_lines_info[ 0 ].line_num == 0 )
      //return TRUE;
      return;
   for( iter = TEXTVIEWER_Num_ContentLines - 1; iter != 0; --iter )
      textviewer_g_lines_info[ iter ] = textviewer_g_lines_info[ iter - 1 ];
//   textviewer_g_lines_info[ 0 ].line_num = textviewer_g_lines_info[ 0 ].line_num - 1;
   textviewer_g_lines_info[ 0 ].line_num = textviewer_g_lines_info[ 0 ].line_num - 1;
   SDKLoadCode( TEXT_TEMP_CODE02 );
   if( !text_lines_cache_update( textviewer_g_lines_info[ 0 ].line_num ))
      //return FALSE;
      return;
   SDKLoadCode( TEXT_TEMP_CODE01 );
   if( text_get_encode()== TEXT_Encode_ASCII )
   {
      if( !text_read_line( textviewer_g_lines_info[ 0 ].line_num, textviewer_g_lines_content[ textviewer_g_lines_info[ 0 ].line_num % TEXTVIEWER_Num_ContentLines ]))
         //return FALSE;
         return;
   }
   else
   {
      if( !text_read_line( textviewer_g_lines_info[ 0 ].line_num, Unicode_get_addr_string( textviewer_g_lines_content[ textviewer_g_lines_info[ 0 ].line_num % TEXTVIEWER_Num_ContentLines ])+ 2 ))
         //return FALSE;
         return;
   }
   //return TRUE;
}

#if 0
/*Bool*/void textviewer_scroll_to( U16 line_num )
{
   U8 _MEM_TYPE_SLOW_ iter;
   if( TEXT_Num_MaxLines - 1 - line_num < TEXTVIEWER_Num_ContentLines )
      line_num = TEXT_Num_MaxLines - 1 - TEXTVIEWER_Num_ContentLines ;
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
   {
      textviewer_g_lines_info[ iter ].line_num = line_num + iter;
      if( !text_lines_cache_update( textviewer_g_lines_info[ iter ].line_num ))
         //return FALSE;
         return;
      if( text_get_encode()== TEXT_Encode_ASCII )
      {
         if( !text_read_line( textviewer_g_lines_info[ iter ].line_num, textviewer_g_lines_content[( line_num + iter )% TEXTVIEWER_Num_ContentLines ]))
            //return FALSE;
            return;
      }
      else
      {
         if( !text_read_line( textviewer_g_lines_info[ iter ].line_num, Unicode_get_addr_string( textviewer_g_lines_content[( line_num + iter )% TEXTVIEWER_Num_ContentLines ])+ 2 ))
            //return FALSE;
            return;
      }
   }
   //return TRUE;
}
#endif


#pragma CODE = TEXT_VIEWER_TEMP_CODE13

U8 *textviewer_get_line_str( U8 index )
{
   if( index >= TEXTVIEWER_Num_ContentLines )
      index = TEXTVIEWER_Num_ContentLines - 1;
   return textviewer_g_lines_content[ index ];
}


char *convert_U16( char *p, U16 i )
{
   if( i / 10 )
      p = convert_U16( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


void textviewer_show_page( void )
{
   extern U16 _MEM_TYPE_SLOW_ text_g_total_lines_num;
   U8 i,c_page;
   
	Ih_set_curr_font( IDC_FONT_08 );
//   if( text_g_total_lines_num == 0 )
//      text_g_total_lines_num = 1;
   //sprintf( textviewer_g_str_buffer, "%d/%d",(( int )textviewer_g_lines_info[ 0 ].line_num + 1 ), ( int )text_g_total_lines_num );
   memset( textviewer_g_str_buffer, 0, TEXTVIEWER_Num_LineLength + 2 );
   c_page = textviewer_g_lines_info[ 0 ].line_num + 1;
   convert_U16(( char *)textviewer_g_str_buffer, c_page);
   strcat(( char *)textviewer_g_str_buffer, "/" );
   i = strlen(( char *)textviewer_g_str_buffer );
  // convert_U16(( char *)&textviewer_g_str_buffer[ i ],  text_g_total_lines_num );
  
   if(text_g_total_lines_num%2){
   	i = text_g_total_lines_num+1;
   	}
   i = text_g_total_lines_num/2;
   if(c_page >= i)return;
   convert_U16(( char *)&textviewer_g_str_buffer[ i ],  i );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 40, 0, 8, 8 );
   lcd_goto_xy( 40, 0 );
   lcd_puts( textviewer_g_str_buffer );//show page
}


void textviewer_display_text( void )
{
   U8 _MEM_TYPE_SLOW_ iter,nc;
   U8 _MEM_TYPE_SLOW_ old_lang = g_setting_list.struct_setting.language_ihm;
   U8 _MEM_TYPE_SLOW_ *pstr;
   
   Ih_set_curr_font( DICTEXT_BigFont ); 
///////////////////////////////////以下这几行代码是用于去除bug
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines - 1; ++iter )
   {
      if( textviewer_g_lines_info[ iter ].line_num % TEXTVIEWER_Num_ContentLines == textviewer_g_lines_info[ iter + 1 ].line_num % TEXTVIEWER_Num_ContentLines )
      {
         for( iter = iter + 1; iter < TEXTVIEWER_Num_ContentLines; ++iter )
            textviewer_g_lines_info[ iter ].line_num = TEXT_Num_MaxLines;
         break;
      }
   }
///////////////////////////////////
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, 128, 32 );

   g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
   for( iter = 0; iter < TEXTVIEWER_Num_ContentLines; ++iter )
   {
      if( textviewer_g_lines_info[ iter ].line_num >= TEXT_Num_MaxLines )
         pstr = ( U8 _MEM_TYPE_SLOW_ *)textviewer_get_line_str( textviewer_g_lines_info[ iter ].line_num );
      else
         pstr = ( U8 _MEM_TYPE_SLOW_ *)textviewer_get_line_str( textviewer_g_lines_info[ iter ].line_num % TEXTVIEWER_Num_ContentLines );
      if( text_get_encode()== TEXT_Encode_ASCII )
      {
         for( nc = 0; nc < TEXTVIEWER_Num_LineLength; ++nc )
         {
            if( pstr[ nc ] == 0 )
               break;
            textviewer_g_str_buffer[ nc ] = pstr[ nc ];
         }
         for( ; nc < TEXTVIEWER_Num_LineLength; ++nc )
            textviewer_g_str_buffer[ nc ] = ' ';
         textviewer_g_str_buffer[ nc ] = '\0';
      }
      else
      {
         for( nc = 0; nc < TEXTVIEWER_Num_LineLength / 2; ++nc )
         //for( nc = 0; nc < TEXTVIEWER_Num_LineLength; ++nc )
         {
            if((( U16 *)pstr )[ nc ]== 0 )
               break;
            (( U16 *)textviewer_g_str_buffer )[ nc ] = (( U16 *)pstr )[ nc ];
         }
        for( ; nc < TEXTVIEWER_Num_LineLength / 2; ++nc )
         // for( ; nc < TEXTVIEWER_Num_LineLength; ++nc )
            (( U16 *)textviewer_g_str_buffer )[ nc ] = ' ';
         (( U16 *)textviewer_g_str_buffer )[ nc ] = '\0';
      }
//      lcd_clear_part_of_screen( TEXTVIEWER_PosText_StartX, TEXTVIEWER_PosText_StartY + iter * TEXTVIEWER_Height_TextLine, 128, TEXTVIEWER_Height_TextLine );
      lcd_goto_xy( TEXTVIEWER_PosText_StartX, TEXTVIEWER_PosText_StartY + iter * TEXTVIEWER_Height_TextLine );
      //lcd_goto_xy(0,24);
      /*
      SendString((U8 *)"\r\nstr=");
	  for(nc=0;nc<74;nc++){
	  	send_hex_u8(textviewer_g_str_buffer[nc]);
		SendString((U8 *)":");
	  	}
	  //*/
      lcd_puts( textviewer_g_str_buffer );//show word meaning
   }
   textviewer_show_page();
   g_setting_list.struct_setting.language_ihm = old_lang;
}

#if 0
void output_process( U8 process )
{
   U8 _MEM_TYPE_SLOW_ old_lang;
   if( dictext_g_main_choice == DICTEXT_Choice_Dictionary )
      return;
   sprintf( g_string2, "文本分析中%d%%", ( int )process );
   if( dictext_g_main_choice == DICTEXT_Choice_Textviewer )
   {
      lcd_clear_part_of_screen( 0, 16, TEXTVIEWER_Num_CharLength * 8, 16 );
      lcd_goto_xy( 0, 16 );
   }
   /*
   else if( dictext_g_main_choice == DICTEXT_Choice_Dictionary )
   {
      lcd_clear_part_of_screen( 0, 16, TEXTVIEWER_Num_CharLength * 8, 16 );
      lcd_goto_xy( 0, 16 );
   }
   */
   old_lang = g_setting_list.language_ihm;
   g_setting_list.language_ihm = LANG_CHINESE_SIMP;
   lcd_puts( g_string2 );
   g_setting_list.language_ihm = old_lang;
}
#endif


#pragma CODE = TEXT_VIEWER_CODE

//////////////////////////////////////////////////////////////////////////

U8       _MEM_TYPE_SLOW_   dictext_g_state;
U8       _MEM_TYPE_SLOW_   dictext_g_last_state;
U8       _MEM_TYPE_SLOW_   dictext_g_cmd_id;
U16      _MEM_TYPE_SLOW_   dictext_g_param;

//const    U8    dic_g_ext[] = "adb";

_MEM_TYPE_SLOW_   select_appli   exit_dictext_to_appli;
_MEM_TYPE_SLOW_   U8             dic_file_open_flag;

//const    U8    dic_g_dir[] = "Dic";

U16      _MEM_TYPE_SLOW_   dic_g_version;

//define the number of the voice address 
#define Length_v                    64

_MEM_TYPE_SLOW_  U8              Point_Write = 0;
_MEM_TYPE_SLOW_  U8              Point_Read = 0;
_MEM_TYPE_SLOW_  U32             Array_V[ Length_v ];

_MEM_TYPE_SLOW_  U8              save_Point_Write = 0;

void  dic_write_media_addr ( U32 address );

U16      _MEM_TYPE_SLOW_   adb_file_number;
U16      _MEM_TYPE_SLOW_   current_adb_file_number;

U16		 _MEM_TYPE_SLOW_   adb_file_display_num;
U16		 _MEM_TYPE_SLOW_   adb_current_display_num;
U16		 _MEM_TYPE_SLOW_   adb_current_mark;


#define  DIC_Num_CharsPerLine       ( DIC_MaxLength_InputWord )
#define  DIC_MaxLength_InputWord    ( DICTEXT_Screen_Width / 8 )
#define  DIC_Num_InputLine          (( DICTEXT_Screen_Width/* - 50 */)/ DICTEXT_Title_CharWidth )

U8    _MEM_TYPE_SLOW_   dic_g_input_word[ DIC_Num_InputLine + 1 ];
U8    _MEM_TYPE_SLOW_   dic_g_input_cursor;


//U8       _MEM_TYPE_SLOW_   dic_g_default_file_name[] = "Default.adb";

//extern   xdata    AL_Variables     	varAl;
/*extern   U16      _MEM_TYPE_SLOW_   dic_remote_version;
extern   U8       _MEM_TYPE_SLOW_   DicRemoteEncryptKey;
extern   U32      _MEM_TYPE_SLOW_   DicRemoteFlag;
*/
//extern   _MEM_TYPE_SLOW_   U8       Usb_connect;
void key_next_prev(U16 param)
{
	if(param == ((KEY_PREV<<8)|KBD_KEY_RELEASED)){
		  if(Display_Page_Num <= 1)return;
		   		Display_Page_Num -= 2;
		   		current_display_num = text_off_addr[Display_Page_Num];
		   		ch_display_address = ch_begin_address + (U32)current_display_num;
		   		mail_send_event( EVT_DIC_FLUSH_TEXT, 0);
		}

	if(param == ((KEY_NEXT<<8)|KBD_KEY_RELEASED)){
		   if(b_LastPage_Flag)return;
		 	current_display_num = text_off_addr[Display_Page_Num];
	     	ch_display_address = ch_begin_address + (U32)current_display_num;
			mail_send_event( EVT_DIC_FLUSH_TEXT, 0);
		}
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE01

//the function is find the number of adb in the root;
U16 Get_DicFile_Number( U8 *sz_filterext )
{
   U16 _MEM_TYPE_SLOW_ Number = 0;
   //SendString((U8 *)"file num\r\n");
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext ))
         {
            Number++;
            //SendString((U8 *)"file+\r\n");
         }
      }
   }
   /*SendString((U8 *)"Number=");
   send_hex_u16(Number);
   SendString((U8 *)"\r\n");*/
   return Number;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE02

//the function is get the file name;
Bool Get_DicFile_Name( U8 *sz_filterext, U16 Number )
{
   U16 _MEM_TYPE_SLOW_ Number_Temp = 0;
   SDKLoadCode(NAVIGATION_CODE);
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext )
            && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string )/*unicode_get_required_size( Unicode_get_addr_string( g_string ))*/, FS_NAME_GET, FALSE ))
         {
            Number_Temp++;
         }
         if(( Number + 1 )== Number_Temp )
         {
            return TRUE;
         }
      }
   }
   return FALSE;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE00

//const    U8    dic_g_dir[] = "Dic";

Bool dic_next_drive( void )
{
   //U16 DIC_DIR_NAME[] = {0x7C,0xFB,0x7E,0xDF,0x65,0x87,0x4E,0xF6,0x00};
   U16 DIC_DIR_NAME[] = {0x8BCD,0x5E93,0x6587,0x4EF6,0x0000};//词库文件
   SDKLoadCode( NAVIGATION_TEMP_CODE05 );
   SDKLoadCode( NAVIGATION_CODE );
   file_init();

   SDKLoadCode( NAVIGATION_TEMP_CODE00 );
   if( !nav_dir_root())
      return FALSE;

#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   #if( UNICODE_TYPE == DISABLE )
   nav_string_ascii();
   #endif
   #if( UNICODE_TYPE == ENABLE )
   nav_string_unicode();
   #endif	
#endif
   SDKLoadCode( NAVIGATION_TEMP_CODE03 );
#if( UNICODE_TYPE == DISABLE )
   if( !nav_filelist_findname(( U8 *)DIC_DIR, FALSE ))
#endif  
#if( UNICODE_TYPE == ENABLE )
   if( !nav_filelist_findname(( U8 *)DIC_DIR_NAME, FALSE ))
#endif 
   {
   /*
      SDKLoadCode( NAVIGATION_TEMP_CODE02 );
      if( !nav_dir_make(( U8 *) DIC_DIR ))
      {
         if( FS_ERR_FILE_EXIST != fs_g_status )
         {
            SDKLoadCode( NAVIGATION_TEMP_CODE04 );
            ( void )nav_file_del( TRUE );
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
            nav_string_unicode();
#endif
            return FALSE;
         }
      }*/
	   return FALSE;
   }

	


#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   nav_string_unicode();
#endif
   //SendString((U8 *)"cd\r\n");
   SDKLoadCode( NAVIGATION_TEMP_CODE01 );
   if( !nav_dir_cd())
      return FALSE;

   SDKLoadCode( TEXT_VIEWER_TEMP_CODE01 );
   current_adb_file_number = 0;
   adb_current_mark = 0;
   adb_file_number = Get_DicFile_Number(( U8 *)DIC_EXT);
   
   if( !adb_file_number )
      return FALSE;
   adb_file_display_num = (adb_file_number+1)/2;
   adb_current_display_num = 1;
   SendString((U8 *)"\r\nnumber=");
   send_hex_u16(adb_file_number);
   //SendString((U8 *)":");
   //send_hex_u16(adb_file_display_num);
   return TRUE;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE04

void dic_file_display( void )
{
   scan_oid_enable();
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
   lcd_goto_xy( 0, 8 );
   Ih_set_curr_font( DICTEXT_BigFont );
#if 0
	lcd_puts(( U8 *)"电子词典:" );
#else
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
//      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
       lcd_puts(( U8 *)"Dictionary" );
      break;

      case LANG_CHINESE_SIMP:
//      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"电子词典" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )   
#endif
/*
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //  lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
 //  lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
   lcd_goto_xy( 12, 26 );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts( g_string );
 */  
   //dictext_g_state = FILEBROWSER_State_Interface;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE17
void adb_filename_display(U16 display_num_mark)
{
	U16 i,j,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //     lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
 //     lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   temp_file = (display_num_mark-1)*DICTEXT_LINE_PAGE;
   if((adb_file_number - temp_file) >= DICTEXT_LINE_PAGE ){
   	SendString((U8 *)"\r\n2 page");
   	j = DICTEXT_LINE_PAGE;
   	}
   else{
   	SendString((U8 *)"\r\n1 page");
   	j = DICTEXT_LINE_PAGE-1;
   	}
   Ih_set_curr_font( DICTEXT_BigFont );
  for(i=0;i<j;i++ ){
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
	  ( void )Get_DicFile_Name(( U8 *)DIC_EXT, temp_file );
 	  lcd_goto_xy( 12, 28+i*20 );
 /*     
	  SendString((U8 *)"\r\nall:page:num:string=");
	  send_hex_u16(adb_file_number);
	  SendString((U8 *)":");
	  send_hex_u16(adb_file_display_num);
	  SendString((U8 *)":");
	  send_hex_u16(temp_file);
	  SendString((U8 *)":");
	  for(j=0;j<64;j++){
	  	send_hex_u8(g_string[j]);
	  	SendString((U8 *)":");
	  	}
*/	  
	  lcd_puts( g_string );
	  temp_file ++;
  	}
}

void adb_mark_display(void)
{
	U16 i,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, 10, (LCD_HEIGHT-24));
	lcd_draw_picture(5,32+20*(adb_current_mark%DICTEXT_LINE_PAGE), IDP_PLAYER_PLAY);
	
}

#pragma CODE = TEXT_VIEWER_TEMP_CODE05
void dic_file_select_deal( void )
{
//   SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
   switch( dictext_g_cmd_id )
   {
      case FILEBROWSER_Cmd_PrevFile:
/*
      if( !current_adb_file_number)
      {
         current_adb_file_number = adb_file_number - 1;
      }
      else
         current_adb_file_number--;
      goto dic_file_display_flag;
*/	  
//      SendString((U8 *)"\r\nFILEBROWSER_Cmd_PrevFile");
	  SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
	  if((adb_current_mark%DICTEXT_LINE_PAGE)!=0){
	  	adb_current_mark --;
		adb_mark_display();
	  	}
	  else{
//	  	SendString((U8 *)"\r\nnowpage");
//	    send_hex_u16(adb_file_display_num);
	  	if(adb_current_display_num!=1){
			adb_current_display_num--;
			adb_current_mark --;
//			SendString((U8 *)"\r\nnewpage");
//	        send_hex_u16(adb_file_display_num);
			adb_filename_display(adb_current_display_num);
			adb_mark_display();
	  		}
	  	}
      break;

      case FILEBROWSER_Cmd_NextFile:
//	  	SendString((U8 *)"\r\nFILEBROWSER_Cmd_NextFile");
		SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
	  if((adb_current_mark%DICTEXT_LINE_PAGE)==0){
	  	if((adb_current_mark+1)==adb_file_number)break;
	  	adb_current_mark ++;
		adb_mark_display();
	  	}
	  else{
//	  	SendString((U8 *)"\r\nnowpage");
//	    send_hex_u16(adb_file_display_num);
	  	if(adb_current_display_num<adb_file_display_num){
			
			adb_current_display_num++;
//			SendString((U8 *)"\r\nnewpage");
//	    send_hex_u16(adb_file_display_num);
			adb_filename_display(adb_current_display_num);
			adb_current_mark ++;
			adb_mark_display();
	  		}
	  	}
/*	  	
      current_adb_file_number++;
      if( current_adb_file_number >= adb_file_number )
      {
         current_adb_file_number = 0;
      }
dic_file_display_flag:
	  SendString((U8 *)"\r\ncurrent_adb_file_number = ");
	  send_hex_u16(current_adb_file_number);
      adb_filename_display();
*/	  
      break;
/*
	  case FILEBROWSER_Cmd_CurFile:
	  adb_filename_display(adb_current_display_num);
	  adb_mark_display();	
	  break;
*/	  
      case FILEBROWSER_Cmd_Enter:
	  current_adb_file_number = adb_current_mark;
	   SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
	  ( void )Get_DicFile_Name(( U8 *)DIC_EXT, current_adb_file_number );
//	  SendString((U8 *)"\r\ncurrent_adb_file_number=");
//	  send_hex_u16(current_adb_file_number);
      dictext_g_state = DIC_State_Init;//FILEBROWSER_State_End;
      break;

      /*case FILEBROWSER_Cmd_Back:
      if( !navext_file_dir( FALSE ))
      {
         if( fs_g_status == FS_ERR_IS_ROOT )
            dictext_g_state = DRIVEBROWSER_State_Refresh;
         else
            break;
      }
      else
      {
         filebrowser_g_cur_filelist = nav_getindex();
         dictext_g_state = FILEBROWSER_State_Refresh;
      }
      break;

      case FILEBROWSER_Cmd_Exit:
      dictext_g_state = DICTEXT_State_End;
      break;*/
   }
   dictext_reset_cmd();
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE16

void dic_no_word_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_part_of_screen( 32, 0, DIC_Num_InputLine * DICTEXT_Title_CharWidth - 61, DICTEXT_Title_Height );
   lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, (LCD_HEIGHT - 8) );
   //dic_draw_small_input_word();
  // dic_draw_small_input_cursor( FALSE );

   Ih_set_curr_font( DICTEXT_BigFont );
   //lcd_clear_part_of_screen( 0, 16, DICTEXT_Screen_Width, 16 );
            
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 8, 24 );
   lcd_puts(( U8 *)"Word not found" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 24 );
   lcd_puts(( U8 *)"查无此字" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 24 );
   lcd_puts(( U8 *)"琩礚" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   lcd_goto_xy( 8, 26 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_puts(( U8 *)"Word not found" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_puts(( U8 *)"查无此字" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_puts(( U8 *)"琩礚" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#if 0
Bool dic_file_sys_init( void )
{    
   if( !dic_next_drive())
      //goto __ErrorOthers;
      return FALSE;
#if 0
   if( !nav_drive_set( FILEBROWSER_Drive ))
      goto __ErrorOthers;
   if( !nav_dir_root())
      goto __ErrorOthers;
   /*if( !nav_filelist_findname( string_get_unicode( textviewer_g_ext ), FALSE ))
   {
      return FALSE;
   }
   if( !nav_dir_cd())
      goto __ErrorOthers;
   if( !nav_filelist_first( FS_FILE ))
   {
      return FALSE;
   }
   */
   //navext_file_set_ext( textviewer_g_ext );
   navext_file_use_dir( TRUE );
   if( !navext_file_first())
   {
      return FALSE;
   }
#endif
   return TRUE;
//__ErrorOthers:
//   return FALSE;
}
#endif


#pragma CODE = TEXT_VIEWER_TEMP_CODE03

Bool dic_find_default_file( void )
{
   U8 _MEM_TYPE_SLOW_ dic_g_default_file_name[] = "Default.adb";

#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   nav_string_ascii();
#endif
   SDKLoadCode( NAVIGATION_TEMP_CODE03 );
   SDKLoadCode( NAVIGATION_CODE );
   ( void )nav_filelist_reset();
   if( nav_filelist_findname(( U8 *)dic_g_default_file_name, FALSE ))
   {
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
      nav_string_unicode();
#endif
      return TRUE;
   }
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   nav_string_unicode();
#endif
   return FALSE;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE06

//////////////////////////////////////////////////////////////////////////
Bool dic_file_open( void )
{
   SDKLoadCode( FILE_TEMP_CODE00 );
   return file_open( FOPEN_MODE_R );
}

/*
Bool dic_file_is_open( void )
{
   return Fat_file_is_open;
}
*/


#pragma CODE = TEXT_VIEWER_CODE

Bool dic_file_seek_and_read( U8 *buffer, U32 pos, U16 size )
{
   if( !file_seek( pos, FS_SEEK_SET ))
      return FALSE;
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)buffer, size )!= size )
      return FALSE;
   return TRUE;
}


#pragma CODE = TEXT_VIEWER_END_CODE

void dic_file_close( void )
{
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
}

#if 0
Bool dic_is_alpha( U8 c )
{
   return ( c >= 'A' && c <= 'Z' )||( c >= 'a' && c <= 'z' );
}
#endif


#pragma CODE = TEXT_VIEWER_CODE

U8 dic_to_lower( U8 c )
{
   if( c >= 'A' && c <= 'Z' )
      c += 'a' - 'A';
   return c;
}

#if 0
U8	dic_to_higher( U8 c )
{
   if( c >= 'a' && c <= 'z' )
      c -= 'a' - 'A';
   return c;
}
#endif


#pragma CODE = TEXT_VIEWER_TEMP_CODE09

S8	dic_string_comp( U8 *str_left, U8 *str_right )
{
   while( *str_left && dic_to_lower( *str_left )== dic_to_lower( *str_right ))
   {
      ++str_left;
      ++str_right;
   };
   return ( S8 )dic_to_lower( *str_left )-( S8 )dic_to_lower( *str_right );
}


#pragma CODE = TEXT_VIEWER_CODE

typedef struct _DicMainInfo
{
   U32   word_table_addr;
   U16   char_num;
   U32   total_words_count;
   U32   word_info_addr;
   //U32   char_table_addr;
   U32   remote_addr;
}DicMainInfo;

typedef struct _DicWordTableItem
{
   U8	   first_char;
   U32   addr;
   U32   total_count;
}DicWordTableItem;

typedef struct _DicWordInfoItem
{
   U8	   word[ 16 ];
   U32   content_addr;
}DicWordInfoItem;

typedef struct _DicWordContentItem
{
   U32   voice_addr;
   U32   voice_size;
   U8    voice_codec;
   U16   content_size;
}DicWordContentItem;

typedef struct _DicWordExternAction
{
   U16   action_type;
   U8    voice_num;
   U8    play_mode;
   U8    play_num;
}DicWordExternAction;

#define  DIC_TYPE_PLAY           1

//U8                   _MEM_TYPE_SLOW_   dic_extern_act_play_count;
DicWordExternAction  _MEM_TYPE_SLOW_   dic_extern_action;
U32                  _MEM_TYPE_SLOW_   dic_extern_action_point;

DicMainInfo          _MEM_TYPE_SLOW_   dic_g_main_info;
DicWordTableItem     _MEM_TYPE_SLOW_   dic_g_word_table_item;
DicWordInfoItem      _MEM_TYPE_SLOW_   dic_g_word_info_item;
DicWordContentItem   _MEM_TYPE_SLOW_   dic_g_word_content_item;


#pragma CODE = DIC_SEARCH_WORD_CODE00

Bool dic_search_word( U8 *word )
{
   U8    _MEM_TYPE_SLOW_   first_char = 0;
   U32   _MEM_TYPE_SLOW_   pos_left;
   U32   _MEM_TYPE_SLOW_   pos_right;
   S8    _MEM_TYPE_SLOW_   result;

   U8    _MEM_TYPE_SLOW_   i, j;
   U16   _MEM_TYPE_SLOW_   text_length;
   U32   _MEM_TYPE_SLOW_   address;
   Bool  _MEM_TYPE_SLOW_   find_flag = FALSE;

   //dic_extern_act_play_count = 0;
	/*if( !dic_file_is_open())
	{
      return FALSE;
   }*/
   SendString((U8 *)"\r\nstring=");
   SendString((U8 *)dic_g_input_word);
   init_array_v();
   if( !isalpha( word[ 0 ]))
   {
      return FALSE;
   }
   first_char = dic_to_lower( word[ 0 ]);
   if( first_char != dic_g_word_table_item.first_char )
   {
      for( pos_left = 0; pos_left < dic_g_main_info.char_num; pos_left++ )
      {
         if( !dic_file_seek_and_read(( U8 *)( &dic_g_word_table_item.first_char ), dic_g_main_info.word_table_addr + pos_left * 9, sizeof( U8 )))
            return FALSE;
         if( !dic_file_seek_and_read(( U8 *)( &dic_g_word_table_item.addr ), dic_g_main_info.word_table_addr + pos_left * 9 + sizeof( U8 ), 8 ))
         //if( !dic_file_seek_and_read(( U8 *)( &dic_g_word_table_item ), dic_g_main_info.word_table_addr + pos_left * sizeof( dic_g_word_table_item ), sizeof( dic_g_word_table_item )))
         {
            return FALSE;
         }
         dic_g_word_table_item.first_char = dic_to_lower( dic_g_word_table_item.first_char );
         if( first_char == dic_g_word_table_item.first_char )
            break;
      }
      if( pos_left == dic_g_main_info.char_num )
      {
         return FALSE;
      }
   }
   if( dic_g_word_table_item.total_count == 0 )
   {
      return FALSE;
   }
   pos_left = 0;
   pos_right = dic_g_word_table_item.total_count;
   do
   {
      //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_info_item, dic_g_word_table_item.addr +(( pos_left + pos_right )/ 2 )* sizeof( dic_g_word_info_item ), sizeof( dic_g_word_info_item )))
      if( !dic_file_seek_and_read(( U8 *)&dic_g_word_info_item, dic_g_word_table_item.addr +(( pos_left + pos_right )>> 1 )* sizeof( dic_g_word_info_item ), sizeof( dic_g_word_info_item )))
         return FALSE;
      result = dic_string_comp( word, dic_g_word_info_item.word );
      if( result > 0 )
      {
         //pos_left = ( pos_left + pos_right )/ 2 + 1;
         pos_left = (( pos_left + pos_right )>> 1 )+ 1;
      }
      else if( result < 0 )
      {
         //pos_right = ( pos_left + pos_right )/ 2;
         pos_right = ( pos_left + pos_right )>> 1;
      }
      else{
	  	 find_flag = TRUE;
         break;
      	}
   }
   while( pos_left < pos_right );
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, dic_g_word_info_item.content_addr, 9 ))
   //   return FALSE;
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item.content_size, dic_g_word_info_item.content_addr + 9, sizeof( U16 )))
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, dic_g_word_info_item.content_addr, sizeof( dic_g_word_content_item )))
   //   return FALSE;
   //SendString((U8 *)"\r\nw:a=");SendString((U8 *)dic_g_word_info_item.word);
  //SendString((U8 *)":");send_hex_u32(dic_g_word_info_item.content_addr);
  return find_flag;
}

#pragma CODE = DIC_SEARCH_WORD_CODE01

Bool dic_add_word_voice( void )
{
   U8    _MEM_TYPE_SLOW_   first_char = 0;
   U32   _MEM_TYPE_SLOW_   pos_left;
   U32   _MEM_TYPE_SLOW_   pos_right;
   S8    _MEM_TYPE_SLOW_   result;

   U8    _MEM_TYPE_SLOW_   i, j;
   U16   _MEM_TYPE_SLOW_   text_length;
   U32   _MEM_TYPE_SLOW_   address;
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, dic_g_word_info_item.content_addr, 9 ))
   //   return FALSE;
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item.content_size, dic_g_word_info_item.content_addr + 9, sizeof( U16 )))
   //if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, dic_g_word_info_item.content_addr, sizeof( dic_g_word_content_item )))
   //   return FALSE;
   SendString((U8 *)"\r\nw:a=");SendString((U8 *)dic_g_word_info_item.word);
  SendString((U8 *)":");send_hex_u32(dic_g_word_info_item.content_addr);
   textviewer_g_start_offset = dic_g_word_info_item.content_addr + 11;
   if( !dic_g_version )
   {
      dic_write_media_addr( dic_g_word_info_item.content_addr );
      if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item.content_size, dic_g_word_info_item.content_addr + 9, sizeof( U16 )))
         return FALSE;
      return TRUE;
   }
   if( dic_g_version == 0x01 )
   {
      if( !dic_file_seek_and_read(( U8 _MEM_TYPE_SLOW_ *)&dic_extern_action, dic_g_word_info_item.content_addr, 5 ))
         return FALSE;
      if( dic_extern_action.action_type != DIC_TYPE_PLAY )
         return FALSE;
      if( !dic_extern_action.voice_num )
         return FALSE;
      if( !dic_extern_action.play_num )
         return FALSE;
      //if( !dic_file_seek_and_read(( U8 _MEM_TYPE_SLOW_ *)&dic_g_word_content_item, dic_extern_action_point + 5, 9 ))
      //   return FALSE;
      //dic_extern_act_play_count++;
      for( i = 0, j = 0; i < dic_extern_action.play_num; i++ )
      {
         address = dic_g_word_info_item.content_addr + 5;
         address += ( U32 )j * 9;
         dic_write_media_addr( address );
         j++;
         if( j >= dic_extern_action.voice_num )
            j = 0;
      }
      address = dic_g_word_info_item.content_addr + 5;
      address += ( U32 )dic_extern_action.voice_num * 9;
      if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item.content_size, address, sizeof( U16 )))
         return FALSE;

      textviewer_g_start_offset = address + 2;
      return TRUE;
   }

   if( !dic_file_seek_and_read(( U8 *)&text_length, dic_g_word_info_item.content_addr + 9, sizeof( U16 )))
      return FALSE;
   address = dic_g_word_info_item.content_addr + 9 + 2 + ( U32 )text_length;
   if( !dic_file_seek_and_read(( U8 *)&dic_extern_action_point, address, sizeof( U32 )))
      return FALSE;
   if( !dic_extern_action_point )
   {
      dic_write_media_addr( dic_g_word_info_item.content_addr );
   }
   else
   {
      if( !dic_file_seek_and_read(( U8 _MEM_TYPE_SLOW_ *)&dic_extern_action, dic_extern_action_point, 5 ))
         return FALSE;
      if( dic_extern_action.action_type != DIC_TYPE_PLAY )
         return FALSE;
      if( !dic_extern_action.voice_num )
         return FALSE;
      if( !dic_extern_action.play_num )
         return FALSE;
      for( i = 0, j = 0; i < dic_extern_action.play_num; i++ )
      {
         address = dic_extern_action_point + 5;
         address += ( U32 )j * 9;
         dic_write_media_addr( address );
         j++;
         if( j >= dic_extern_action.voice_num )
            j = 0;
      }
   }
   if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item.content_size, dic_g_word_info_item.content_addr + 9, sizeof( U16 )))
      return FALSE;
   return TRUE;
}

// record the news of the media 
void dic_write_media_addr( U32 address )
{
   if( Point_Write >=( Length_v - 1 ))
   {
      return;
   }
   Array_V[ Point_Write ] = address;
   Point_Write++;

   save_Point_Write = Point_Write;
}


#pragma CODE = TEXT_VIEWER_CODE

//the function is clear array array-v;
void  init_array_v( void )
{
   Point_Read = 0;
   Point_Write = 0;
   //memset( Array_V, 0, Length_v );
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE09

void dic_input_word_putc( U8 c )
{
   //if( !dic_is_alpha( c ))
      //return;
   if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
      return;
   dic_g_input_word[ dic_g_input_cursor++ ] = dic_to_lower( c );
   dic_g_input_word[ dic_g_input_cursor ] = '\0';
}


void dic_input_word_backward( void )
{
   if( dic_g_input_cursor == 0 )
      return;
   dic_g_input_word[ --dic_g_input_cursor ] = '\0';
}


#pragma CODE = TEXT_VIEWER_CODE

void dic_input_word_clear( void )
{
   dic_g_input_cursor = 0;
   dic_g_input_word[0] = '\0';
}


typedef struct _DicKeyBoardCoord
{
   U8    pos_x;
   U8    pos_y;
}DicKeyBoardCoord;

#define  DIC_Direction_Up           0
#define  DIC_Direction_Down         1
#define  DIC_Direction_Left         2
#define  DIC_Direction_Right        3

#define  DIC_Num_KeyboardLines      2
#define  dic_g_display_buffer       textviewer_g_str_buffer

#if 0
DicKeyBoardCoord  _MEM_TYPE_SLOW_   dic_g_keyboard_cursor;
U8                code              dic_g_keyboard[ DIC_Num_KeyboardLines ][ DIC_Num_CharsPerLine + 1 ] =
{
   { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', '\'', ' ', 'e', '\0' },
   { 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\'', ' ', 'e', '\0' }
};

#if 0
DicKeyBoardCoord dic_char_to_coord( U8 c )
{
   DicKeyBoardCoord coord = { DIC_Num_CharsPerLine - 1, 0 };
   for( coord.pos_y = 0; coord.pos_y < DIC_Num_KeyboardLines; ++coord.pos_y )
   {
      for( coord.pos_x = 0; coord.pos_x < DIC_Num_CharsPerLine; ++coord.pos_x )
      {
         if( dic_g_keyboard[ coord.pos_y ][ coord.pos_x ] == dic_to_lower( c ))
            return coord;
      }
   }
   return coord;
}
#endif
#if 0
U8 dic_coord_to_char( DicKeyBoardCoord coord )
{
   coord.pos_x %= DIC_Num_CharsPerLine;
   coord.pos_y %= DIC_Num_KeyboardLines;
   return dic_g_keyboard[ coord.pos_y ][ coord.pos_x ];
}


/*Bool*/void dic_keyboard_cursor_move( U8 direction )
{
   switch( direction )
   {
      case DIC_Direction_Up:
      if( dic_g_keyboard_cursor.pos_y == 0 )
         dic_g_keyboard_cursor.pos_y = DIC_Num_KeyboardLines;
      else
         dic_g_keyboard_cursor.pos_y--;
      //return FALSE;
      break;

		case DIC_Direction_Down:
      if( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines )
         dic_g_keyboard_cursor.pos_y = 0;
      else
         dic_g_keyboard_cursor.pos_y++;
      //return FALSE;
      break;

		case DIC_Direction_Left:
      if( dic_g_keyboard_cursor.pos_x == 0 )
         dic_g_keyboard_cursor.pos_x = DIC_Num_CharsPerLine - 1;
      else
         dic_g_keyboard_cursor.pos_x--;
      //return TRUE;
      break;

		case DIC_Direction_Right:
      if( dic_g_keyboard_cursor.pos_x == DIC_Num_CharsPerLine - 1 )
         dic_g_keyboard_cursor.pos_x = 0;
      else
         dic_g_keyboard_cursor.pos_x++;
      //return TRUE;
      break;
	}
	//return FALSE;
}
#endif
#endif


#pragma CODE = TEXT_VIEWER_TEMP_CODE06

Bool dic_init( void )
{
   U32 _MEM_TYPE_SLOW_ signature;
   U16 _MEM_TYPE_SLOW_ dic_version;
   U8 i;
   if( !dic_file_open())
   {
 //     SendString((U8 *)"\r\n dic open fale!");
      return FALSE;
   }


	
   SDKLoadCode( FS_CREATE_BUF_CODE );
   ( void )FSCreateBuf();
   
   if( !dic_file_seek_and_read(( U8 *)(&dic_g_main_info ), 0, sizeof( dic_g_main_info )))
   {
 //     SendString((U8 *)"\r\n dic seek fale!");
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      return FALSE;
   }
   //SendString((U8 *)"\r\nr:v=");
  // send_hex_u32(dic_g_main_info.word_table_addr);SendString((U8 *)":");
  // send_hex_u32(dic_g_main_info.total_words_count);SendString((U8 *)":");
  // send_hex_u32(dic_g_main_info.word_info_addr);SendString((U8 *)":");
   //send_hex_u32(dic_g_main_info.remote_addr);SendString((U8 *)":");
   
   
   dic_g_word_table_item.first_char = 0xFF;
   dic_g_word_table_item.addr = 0;
   dic_g_word_table_item.total_count = 0;

   dic_g_input_cursor = 0;
   dic_g_input_word[ 0 ] = '\0';

   //add dictionary version
   dic_g_version = 0x00;
   if( !file_seek( dic_g_main_info.remote_addr - sizeof( U32 ), FS_SEEK_SET ))
      return TRUE;
// SendString((U8 *)"\r\n1417!");  
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ * )( &signature ), sizeof( U32 )) != sizeof( U32 ))
      return TRUE;
// SendString((U8 *)"\r\n1420!");  
   if( signature != 0xFFFFEEEE )
      return TRUE;
//  SendString((U8 *)"\r\n 1423!");  
   if( !file_seek( dic_g_main_info.remote_addr - sizeof( U32 )- sizeof( U16 ), FS_SEEK_SET ))
      return TRUE;
//   SendString((U8 *)"\r\n 1426!"); 
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ * )( &dic_version ), sizeof( U16 )) != sizeof( U16 ))
      return TRUE;
  
   dic_g_version = dic_version;
   //send_hex_u16(dic_g_version);
   return TRUE;
}


//#pragma CODE = TEXT_VIEWER_CODE
#pragma CODE = TEXT_VIEWER_REMOTE_CODE

void dic_trans_remote( void )
{
   DicRemoteResult *result = dic_remote_get_result();
   if(( dictext_g_cmd_id == DICTEXT_Cmd_None )&& dic_remote_get_info())
   {
      if( result->remote_code_type == DICREMOTE_Code_AlphaStart )
      {
         dictext_g_cmd_id = DIC_Cmd_KeyInput;
         dictext_g_param = 'A' + result->remote_code - DICREMOTE_Code_AlphaStart;
      }
	  else if( result->remote_code_type == DICREMOTE_Code_Dot)
      {
         dictext_g_cmd_id = DIC_Cmd_KeyInput;
		 dictext_g_param = '.';
      }
	  else if( result->remote_code_type == DICREMOTE_Code_Hyphen)
      {
         dictext_g_cmd_id = DIC_Cmd_KeyInput;
		 dictext_g_param = '-';
      }
	  else if( result->remote_code_type == DICREMOTE_Code_Space)
      {
         dictext_g_cmd_id = DIC_Cmd_KeyInput;
		 dictext_g_param = ' ';
      }
      else if( result->remote_code_type == DICREMOTE_Code_Enter )
      {
         dictext_g_cmd_id = DIC_Cmd_Enter;
      }
      else if( result->remote_code_type == DICREMOTE_Code_CancelInput )
      {
         dictext_g_cmd_id = DIC_Cmd_CancelInput;
      }
      else if( result->remote_code_type == DICREMOTE_Code_Backspace)
      {
         dictext_g_cmd_id = DIC_Cmd_Backspace;
      }
      else if( result->remote_code_type == DICREMOTE_Code_Exit )
      {
         dictext_g_cmd_id = DIC_Cmd_Exit;
      }
      else if( result->remote_code_type == DICREMOTE_Code_PowerOff )
      {
         dictext_g_cmd_id = DIC_Cmd_PowerOff;
      }
      else if( result->remote_code_type == DICREMOTE_Code_VolUp )
      {
         dictext_g_cmd_id = DIC_Cmd_VolUp;
      }
      else if( result->remote_code_type == DICREMOTE_Code_VolDown )
      {
         dictext_g_cmd_id = DIC_Cmd_VolDown;
      }
      dic_remote_reset_info();
	   SendString((U8 *)"\r\ncmd_id=");
  		send_hex_u8(dictext_g_cmd_id);
   }
  
}


#pragma CODE = TEXT_VIEWER_CODE

#define  DIC_PosX_InputWord         0
//#define  DIC_PosY_InputWord         8
#define  DIC_PosY_InputWord         26
#define  DIC_PosX_InputCursor       0
//#define  DIC_PosY_InputCursor       8
#define  DIC_PosY_InputCursor       26
#define  DIC_PosX_Keyboard          0
#define  DIC_PosY_Keyboard          16
#define  DIC_PosX_KeyboardCursor    0
#define  DIC_PosY_KeyboardCursor    16


void lcd_set_inverse( Bool v )
{
   lcd_g_inverse = v;
}


void lcd_puts_inverse( U8 *str, Bool inverse )
{
   Bool old;
   old = lcd_g_inverse;
   lcd_set_inverse( inverse );
   lcd_puts( str );
   lcd_set_inverse( old );
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE16

void dic_draw_small_input_word( void )
{
   U8 _MEM_TYPE_SLOW_ iter;
   for( iter = 0; iter < dic_g_input_cursor; ++iter )
   {
      dic_g_display_buffer[ iter ] = dic_g_input_word[ iter ];
   }
   for( ; iter < DIC_Num_InputLine; ++iter )
      dic_g_display_buffer[ iter ] = ' ';
   dic_g_display_buffer[ DIC_Num_InputLine ] = '\0';
   lcd_goto_xy( DIC_PosX_InputWord, DIC_PosY_InputWord );
  // Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts( dic_g_display_buffer );
}


void dic_draw_small_input_cursor( Bool inverse )
{
   //inverse = inverse;
   lcd_goto_xy( DIC_PosX_InputCursor + dic_g_input_cursor * 3, DIC_PosY_InputCursor );
    //Ih_set_curr_font( DICTEXT_SmallFont );
  Ih_set_curr_font( DICTEXT_BigFont );
   if( inverse )
      lcd_puts_inverse(( U8 *)" ", TRUE );
   else
      lcd_puts(( U8 *)"_" );
}


#pragma CODE = TEXT_VIEWER_CODE

void dic_draw_input_word( void )
{
   U8 _MEM_TYPE_SLOW_ iter;
   for( iter = 0; iter < dic_g_input_cursor; ++iter )
   {
      dic_g_display_buffer[ iter ] = dic_g_input_word[ iter ];
   }
   for( ; iter < DIC_Num_InputLine; ++iter )
      dic_g_display_buffer[ iter ] = ' ';
   dic_g_display_buffer[ DIC_Num_InputLine ] = '\0';
   lcd_goto_xy( 0, 8 );
   //Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts(( U8 *)"请输入字母:");
   
   lcd_goto_xy( DIC_PosX_InputWord, DIC_PosY_InputWord );
   //Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts( dic_g_display_buffer );
    g_b_flush_display = FALSE;
}


void dic_draw_input_cursor( Bool inverse )
{
   //inverse = inverse;
   lcd_goto_xy( DIC_PosX_InputCursor + dic_g_input_cursor * DICTEXT_Title_CharWidth, DIC_PosY_InputCursor );
   //Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   if( inverse )
      lcd_puts_inverse(( U8 *)" ", TRUE );
   else
      lcd_puts(( U8 *)"_" );
}

#if 0
void dic_draw_keyboard( void )
{
#if 0
   U8 _MEM_TYPE_SLOW_ y;
   U8 _MEM_TYPE_SLOW_ x;
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
   lcd_clear_part_of_screen( DIC_PosX_Keyboard, DIC_PosY_Keyboard, DICTEXT_Screen_Width, 16 );
   Ih_set_curr_font( DICTEXT_SmallFont );
   for( y = 0; y < DIC_Num_KeyboardLines; ++y )
   {
      for( x= 0; x < DIC_Num_CharsPerLine; ++x )
      {
         lcd_goto_xy( DIC_PosX_Keyboard + x * 8, DIC_PosY_Keyboard + y * 8 );
         buffer[ 0 ] = dic_g_keyboard[ y ][ x ];
         lcd_puts( buffer );
      }
   }
#endif
   /*lcd_goto_xy( DIC_PosX_Keyboard, DIC_PosY_Keyboard );
   Ih_set_curr_font( DICTEXT_BigFont );
   if( dic_g_keyboard_cursor.pos_y >= DIC_Num_KeyboardLines )
      lcd_puts( dic_g_keyboard[ DIC_Num_KeyboardLines - 1 ]);
   else
      lcd_puts( dic_g_keyboard[ dic_g_keyboard_cursor.pos_y ]);*/
}


void dic_draw_info_cursor( Bool inverse )
{
   if( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines )
      lcd_goto_xy( 0, 0 );
   else
      lcd_goto_xy( 0, DIC_PosY_Keyboard + dic_g_keyboard_cursor.pos_y * 16 );
   if( inverse )
      lcd_puts( ">" );
   else
      lcd_puts( " " );
}


void dic_draw_keyboard_cursor( Bool inverse )
{
   U8 _MEM_TYPE_SLOW_ buffer[] = " ";
   if( dic_g_keyboard_cursor.pos_y < DIC_Num_KeyboardLines )
   {
      buffer[ 0 ] = dic_coord_to_char( dic_g_keyboard_cursor );
      buffer[ 1 ] = 0;
      //lcd_goto_xy( DIC_PosX_KeyboardCursor + dic_g_keyboard_cursor.pos_x * 8, DIC_PosY_KeyboardCursor + dic_g_keyboard_cursor.pos_y * 16 );
      lcd_goto_xy( DIC_PosX_KeyboardCursor + dic_g_keyboard_cursor.pos_x * 8, DIC_PosY_KeyboardCursor );
      Ih_set_curr_font( DICTEXT_BigFont );
      lcd_puts_inverse( buffer, inverse );
   }
}


void dic_draw_cursor( Bool inverse )
{
#if 0
   if( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines )
      dic_draw_input_cursor( inverse );
   /*else
      dic_draw_keyboard_cursor( inverse );*/
#endif
}


void dic_display_volume( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 4 ];

   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 10, 0 );
   sprintf( string , "%02bu", srvplayer_g_arg.volume );
   lcd_puts( string );
}
#endif
//////////////////////////////////////////////////////////////////////////


#pragma CODE = MMI_DIC_TEMP_CODE04

void dictext_send_cmd( U8 id, U16 param )
{
   dictext_g_cmd_id = id;
   dictext_g_param = param;
}


//#pragma CODE = TEXT_VIEWER_CODE
#pragma CODE = TEXT_VIEWER_KEY_CODE

void dictext_trans_cmd( U8 evt_id, U16 evt_param )
{
   switch( dictext_g_state )
   {
      case FILEBROWSER_State_Interface:
      case DIC_State_Interface:
      case TEXTVIEWER_State_TextView:
      if( evt_id == EVT_KEY_PRESSED )
      {
         if( KBD_KEY_LONGPRESSED == KEY_STATUS( evt_param ))
         {
            switch( KEY_ID( evt_param ))
            {
               case KEY_MODE:
//			   SendString((U8 *)"\r\ndictext_g_state=");
//			   send_hex_u8(dictext_g_state);
ready_comeback:			 
               dictext_g_cmd_id = DICTEXT_Cmd_Exit;
               if( bInEbookMode )
                  exit_dictext_to_appli = AP3_APPLI;
               else
                  exit_dictext_to_appli = IDLE_APPLI;
               bRemoteEnterCal = FALSE;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
               //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
               break;

               case KEY_POWER:
               if( power_is_vbus_present())
               {
                  exit_dictext_to_appli = MSTORAGE_APPLI;
               }
               else
               {
                  exit_dictext_to_appli = POWER_OFF_APPLI;
                  g_power_off_mode = POWER_OFF;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
                  //g_b_dic_remote_on = FALSE;
                  g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

               }
               dictext_g_cmd_id = DICTEXT_Cmd_Exit;
               break;

				case KEY_FANYI:		
				recorder_mode = 1;
				exit_dictext_to_appli = RECORDER_APPLI;
				dictext_g_cmd_id = DICTEXT_Cmd_Exit;
				break;
/*
				case KEY_GENDU:
				recorder_mode = 1;
				goto back_reread;
				break;

				case KEY_DUIBI:
				recorder_mode = 0;
				
back_reread:				
				exit_dictext_to_appli = REREAD_APPLI;
				 dictext_g_cmd_id = DICTEXT_Cmd_Exit;
				break;
*/
#if ( PASTER_MODE == ENABLE )
               case KEY_MEN:
               dictext_g_cmd_id = DICTEXT_Cmd_Exit;
               exit_dictext_to_appli = PASTER_APPLI;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
               //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
               break;
#endif   //#if( PASTER_MODE == ENABLE )

            }
         }
         else if( KBD_KEY_RELEASED == KEY_STATUS( evt_param ))
         {
/*         
            if( KEY_ID( evt_param )== KEY_FANYI)
            {
              goto back_recorder;
            }
*/
			if( KEY_ID( evt_param )== KEY_GENDU)
            {
			  recorder_mode = 1;	
              goto back_reread;
            }
			if( KEY_ID( evt_param )== KEY_DUIBI)
            {
              recorder_mode = 0;
back_reread:			  
			  exit_dictext_to_appli = REREAD_APPLI;
			  dictext_g_cmd_id = DICTEXT_Cmd_Exit;
              break;
            }
			if( KEY_ID( evt_param )== KEY_MODE)
            {
				 //dictext_g_cmd_id = DICTEXT_Cmd_Exit;
				 //dictext_g_state = FILEBROWSER_State_NextInit;
				  if((dictext_g_state == DIC_State_Interface)||((dictext_g_state == TEXTVIEWER_State_TextView))){
				  	if(adb_file_number > 1){
			   			dictext_g_state = FILEBROWSER_State_NextInit;
				  		}
					else{
						goto ready_comeback;
						}
			   		}
					else{
						goto ready_comeback;
					}
            }
         }
      }
      break;
   }
   if( dictext_g_cmd_id == DICTEXT_Cmd_Exit )
      return;
   switch( dictext_g_state )
   {
      case FILEBROWSER_State_Interface:
	   SendString((U8 *)"\r\n1895");
      switch( evt_id )
      {
         case EVT_KEY_PRESSED:
         if( KBD_KEY_RELEASED == KEY_STATUS( evt_param ))
         {
            switch( KEY_ID( evt_param ))
            {
               case KEY_PREV:
               dictext_g_cmd_id = FILEBROWSER_Cmd_PrevFile;
               break;
               
               case KEY_NEXT:
               dictext_g_cmd_id = FILEBROWSER_Cmd_NextFile;
               break;
/*
			   case KEY_VOL_DOWN:
               //dictext_g_cmd_id = FILEBROWSER_Cmd_PrevFile;
               srvplayer_volume_change( SRVPLAYER_VOL_DOWN);
        		 //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        	    mmi_dic_display_volume();
				mail_send_event(EVT_DIC_VOL_PLAY,0);
               break;
               
               case KEY_VOL_UP:
               //dictext_g_cmd_id = FILEBROWSER_Cmd_NextFile;
               srvplayer_volume_change( SRVPLAYER_VOL_UP );
         	   //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
               mmi_dic_display_volume();
			   mail_send_event(EVT_DIC_VOL_PLAY,0);
               break;
*/			   
               case KEY_POWER:
               //case KBD_MMI_NEXT:
               dictext_g_cmd_id = FILEBROWSER_Cmd_Enter;
               break;
               
               /*
               case KBD_MMI_F1:
               break;
               
               cas KBD_MMI_F2:
               dictext_g_cmd_id = FILEBROWSER_Cmd_Exit;
               break;
               */
               case KEY_MODE:
                if((dictext_g_state == DIC_State_Interface)||((dictext_g_state == TEXTVIEWER_State_TextView))){
			   
				 if(adb_file_number > 1){
				 	dictext_g_state = FILEBROWSER_State_NextInit;
				 	}
				 else{
				 	goto ready_comeback;
				 	}
			   	}
				else{
					goto ready_comeback;
					}
               default:
               break;
            }
         }
         break;
				
         //case EVT_OID_OK:
         //break;
      }
      break;
      
      /*case DICTEXT_State_Menu:
      switch( evt_id )
      {
         case EVT_KEY_PRESSED:
         if( KBD_KEY_RELEASED == KEY_STATUS( evt_param ))
         {
            dictext_g_cmd_id = DICTEXT_Cmd_KeyInput;
            dictext_g_param = KEY_ID( evt_param );
         }
         break;
      }
      break;*/
#if 0
      case DIC_State_Interface:
      switch( evt_id )
      {
         case EVT_KEY_PRESSED:
         if( KBD_KEY_RELEASED == KEY_STATUS( evt_param ))
         {
            /*
            dictext_g_cmd_id = DIC_Cmd_KeyInput;
            dictext_g_param = KEY_ID( evt_param );
            */
            switch(KEY_ID(evt_param))
            {
               case KEY_VOL_DOWN:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardLeft;
               break;
               
               case KEY_VOL_UP:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardRight;
               break;
               
               case KEY_MODE:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardScroll;
               break;
               
               case KEY_POWER:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardSelect;
               break;
               /*
               case KBD_MMI_F1:
               dictext_g_cmd_id = DIC_Cmd_Enter;
               break;
               */
            }
         }
         if( KBD_KEY_REPEAT == KEY_STATUS( evt_param ))
         {
            switch( KEY_ID( evt_param ))
            {
               case KEY_VOL_DOWN:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardLeft;
               break;
               
               case KEY_VOL_UP:
               //dictext_g_cmd_id = DIC_Cmd_KeyboardRight;
               break;
            }
         }
         break;
      }
      break;
#endif
      case TEXTVIEWER_State_TextView:
	  SendString((U8 *)"\r\n2029");
	  key_next_prev(evt_param);
      switch( evt_id )
      {
         case EVT_KEY_PRESSED:
         if( KBD_KEY_RELEASED == KEY_STATUS( evt_param ))
         {
            switch( KEY_ID( evt_param ))
            {
               case KEY_PREV:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollUp;   
               break;
               
               case KEY_NEXT:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollDown;	
               break;

			   case KEY_VOL_DOWN:
               //dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollUp;
               //srvplayer_volume_change( SRVPLAYER_VOL_UP );
        		 srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         		mmi_dic_display_volume();
               break;
               
               case KEY_VOL_UP:
               //dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollDown;
               srvplayer_volume_change( SRVPLAYER_VOL_UP );
         		//srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        		 mmi_dic_display_volume();
               break;
			   
               case KEY_MODE:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_Back;
               break;

               case KEY_POWER:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_Voice;
               break;
               
               /*default:
               break;*/
            }
         }
		 /*
         else if( KBD_KEY_REPEAT == KEY_STATUS( evt_param ))
         {
            switch( KEY_ID( evt_param ))
            {
               case KEY_PREV:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollUp;
               break;
               
               case KEY_NEXT:
               dictext_g_cmd_id = TEXTVIEWER_Cmd_ScrollDown;
               break;
            }
         }
		 */
         break;
      }
      break;

      /*case DICTEXT_State_Error:
      break;*/
   }
}


#pragma CODE = TEXT_VIEWER_CODE

void dictext_reset_cmd( void )
{
   dictext_g_cmd_id = DICTEXT_Cmd_None;
   dictext_g_param = 0;
}


#pragma CODE = MMI_DIC_TEMP_CODE04

/*Bool*/void dictext_evt_init( void )
{
   dictext_g_state = dictext_g_last_state = DICTEXT_State_Idle;
   dictext_g_cmd_id = DICTEXT_Cmd_None;
   dictext_g_param = 0;
   //return TRUE;
}


#pragma CODE = TEXT_VIEWER_CODE

void dictext_evt( void )
{
   switch( dictext_g_state )
   {
      case FILEBROWSER_State_Interface:
      case DIC_State_Interface:
      case TEXTVIEWER_State_TextView:
      if( dictext_g_cmd_id == DICTEXT_Cmd_Exit )
      {
         dictext_g_state = DICTEXT_State_End;
         dictext_reset_cmd();
         return;
      }
      break;
   }
   
   switch( dictext_g_state )
   {
      case DICTEXT_State_Idle:
      switch( dictext_g_cmd_id )
      {
         case DICTEXT_Cmd_Start:
         //dictext_g_state = FILEBROWSER_State_Init;
         dictext_g_state = DICTEXT_State_Init;
         break;
      }
      dictext_reset_cmd();
      break;

      case DICTEXT_State_Init:
      //if( !dic_file_sys_init())
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE00 );
      if( !dic_next_drive())
      {
         mail_send_event( EVT_DIC_NO_FILE, 0 );
         dictext_g_state = DICTEXT_State_Idle;
         break;
      }

      scan_oid_enable();
      mmi_dic_kbd_enable();
	  /*
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE03 );
	 
      if( !dic_find_default_file())
      {
         //dic_g_state = DIC_State_Error;
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
         ( void )Get_DicFile_Name(( U8 *)dic_g_ext, current_adb_file_number );
         dictext_g_state = FILEBROWSER_State_Refresh;
         break;
      }
      //*/
      /*( void )nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE04 );
      dic_file_display();*/

	  if(adb_file_number > 1){
       dictext_g_state = FILEBROWSER_State_Refresh;
	  	}
	  else{
	  	//dictext_g_state = DIC_State_Init;
	  	current_adb_file_number = 0;
	  	 SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
	  	( void )Get_DicFile_Name(( U8 *)DIC_EXT, current_adb_file_number );
     	 dictext_g_state = DIC_State_Init;//FILEBROWSER_State_End;
	  	}
      //dictext_g_state = DIC_State_Init;
      break;

	  case FILEBROWSER_State_NextInit:
	  	//Ts_free( s_ts_id );
	   if( player_ap3_is_busy())
	      player_ap3_stop();
	   //dictext_task_stop();
	  // scan_oid_disable();
	   SDKLoadCode( FILE_TEMP_CODE01 );
	   file_close();
	   dictext_g_state = DICTEXT_State_Init;
	  break;
	  
      case FILEBROWSER_State_Refresh:
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE04 );
      dic_file_display();
	  SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
	  //adb_file_display_num = 1;
	  //adb_current_mark = 0;
	  //SendString((U8 *)"\r\nbegin refresh");
      adb_filename_display(adb_current_display_num);
	  adb_mark_display();
      dictext_g_state = FILEBROWSER_State_Interface;
	  //dictext_g_cmd_id = FILEBROWSER_Cmd_Enter;
	  
#if 0
      scan_oid_enable();
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 25, 0, 84, 8 );
      lcd_goto_xy( 32, 0 );
      Ih_set_curr_font( DICTEXT_SmallFont );
      lcd_puts(( U8 *)"FILE NAME" );

      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
      lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
      lcd_puts( g_string );
      dic_g_state = FILEBROWSER_State_Interface;
#endif
      break;

      case FILEBROWSER_State_Interface:
	  	
	  if(player_ap3_is_busy())break;
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE05 );
      dic_file_select_deal();
#if 0
      switch( dic_g_cmd_id )
      {
         case FILEBROWSER_Cmd_PrevFile:
         if( !current_adb_file_number)
         {
            current_adb_file_number = adb_file_number - 1;
         }
         else
            current_adb_file_number--;
         goto dic_file_display;
         //break;

         case FILEBROWSER_Cmd_NextFile:
         current_adb_file_number++;
         if( current_adb_file_number >= adb_file_number )
         {
            current_adb_file_number = 0;
         }
dic_file_display:
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
         Get_DicFile_Name(( U8 *)dic_g_ext, current_adb_file_number );
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
         lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
         lcd_puts( g_string );
         break;

         case FILEBROWSER_Cmd_Enter:
         dic_g_state = DIC_State_Init;//FILEBROWSER_State_End;
         break;

         /*case FILEBROWSER_Cmd_Back:
         if( !navext_file_dir( FALSE ))
         {
            if( fs_g_status == FS_ERR_IS_ROOT )
               dictext_g_state = DRIVEBROWSER_State_Refresh;
            else
               break;
         }
         else
         {
            filebrowser_g_cur_filelist = nav_getindex();
            dictext_g_state = FILEBROWSER_State_Refresh;
         }
         break;

         case FILEBROWSER_Cmd_Exit:
         dictext_g_state = DICTEXT_State_End;
         break;*/
      }
      dictext_reset_cmd();
#endif
      break;

      /*case FILEBROWSER_State_End:
      dic_g_state = DIC_State_Init;
      break;*/

      case DIC_State_Init:
      //dic_file_open_flag = TRUE;
      /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 25, 0, 84, 8 );
      lcd_goto_xy( 32, 0 );
      Ih_set_curr_font( DICTEXT_SmallFont );
      lcd_puts(( U8 *)"Wait..." );*/
//      SendString((U8 *)"\r\nDIC_State_Init");
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE06 );
      if( !dic_init())
      {
//         SDKLoadCode( TEXT_VIEWER_TEMP_CODE02 );
//         ( void )Get_DicFile_Name(( U8 *)dic_g_ext, current_adb_file_number );
		 adb_current_mark = 0;
		 adb_current_display_num = 1;
         dictext_g_state = FILEBROWSER_State_Refresh;
         break;
      }

		  strcpy((char *)current_dic_dir,(char *)CurrentShortFilePath);
		   strcat((char *)current_dic_dir,(char *)"\\");
		
			SendString((U8 *)"\r\npath=");
		   SendString((U8 *)current_dic_dir);

	 
	    strcpy((char *)current_dic_short_name,(char *)fat_g_cur_nav->name);
		//strcat((char *)current_dic_short_name,(char *)".");
		//strcat((char *)current_dic_short_name,(char *)DIC_EXT);
      SDKLoadCode( TEXT_REMOTE_TEMP_CODE00 );
      if( dic_remote_task_init( dic_g_main_info.remote_addr ))
      {
         dic_remote_task_start();
         dic_file_open_flag = TRUE;
      }
      dictext_g_state = DIC_State_Refresh;
      break;

      case DIC_State_Refresh:
//	  SendString((U8 *)"\r\n DIC_State_Refresh");
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE08 );
      dic_enter_prepare();
#if 0
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 32, 0, DICTEXT_Screen_Width - 61, 8 );
      //dic_draw_keyboard();
      //lcd_clear_part_of_screen( 0, 16, DICTEXT_Screen_Width, 16 );
      //lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 8 );
      lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 24 );
      dic_draw_input_word();
      dic_draw_input_cursor( FALSE );
      //dic_draw_cursor( TRUE );
      dictext_g_state = DIC_State_Interface;
#endif
      break;

      case DIC_State_Interface:
      dic_remote_task();
	  
      SDKLoadCode( TEXT_VIEWER_REMOTE_CODE );
      dic_trans_remote();
	  if(player_ap3_is_busy())break;
      if( dicremote_g_state == DICREMOTE_State_Voice )
         break;
	   
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE09 );
      dic_remote_voice_end_deal();
#if 0
      switch( dictext_g_cmd_id )
      {
         case DIC_Cmd_Exit:
         dictext_g_state = DIC_State_End;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         //if( g_b_dic_remote_on )
         if( g_b_remote_on )
         {
            dictext_g_state = DICTEXT_State_End;
            exit_dictext_to_appli = AP3_APPLI;
         }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         break;

         case DIC_Cmd_KeyInput:
         dic_input_word_putc( dic_to_lower( dictext_g_param ));
         //dic_g_keyboard_cursor = dic_char_to_coord( dic_to_lower( dictext_g_param ));
         dic_draw_input_word();
         dic_draw_input_cursor( FALSE );
         //dic_draw_keyboard();
         //dic_draw_cursor( TRUE );
         break;
#if 0
         case DIC_Cmd_KeyboardScroll:
         dic_draw_cursor( FALSE );
         dic_keyboard_cursor_move( DIC_Direction_Down );
         dic_draw_keyboard();
         dic_draw_cursor( TRUE );
         break;
         
         case DIC_Cmd_KeyboardLeft:
         if( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines )
         {
            dic_input_word_backward();
            dic_draw_input_word();
            dic_draw_input_cursor( TRUE );
         }
         else
         {
            dic_draw_keyboard_cursor( FALSE );
            dic_keyboard_cursor_move( DIC_Direction_Left );
            while( dic_coord_to_char( dic_g_keyboard_cursor ) == ' ')
               dic_keyboard_cursor_move( DIC_Direction_Left );
            dic_draw_keyboard_cursor( TRUE );
         }
         break;

         case DIC_Cmd_KeyboardRight:
         if( dic_g_keyboard_cursor.pos_y != DIC_Num_KeyboardLines )
         {
            dic_draw_keyboard_cursor( FALSE );
            dic_keyboard_cursor_move( DIC_Direction_Right );
            while( dic_coord_to_char( dic_g_keyboard_cursor ) == ' ')
               dic_keyboard_cursor_move( DIC_Direction_Right );
            dic_draw_keyboard_cursor( TRUE );
         }
         break;
            
         case DIC_Cmd_KeyboardSelect:
         if( dic_g_keyboard_cursor.pos_y >= DIC_Num_KeyboardLines )
         {
            goto __FlagEnter;
         }
         else
         {
            U8 c;
            c = dic_coord_to_char( dic_g_keyboard_cursor );
            switch( c )
            {
               case ' ':
               break;

               case 'b':
               dic_input_word_backward();
               dic_draw_input_word();
               dic_draw_input_cursor( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines );
               break;

               case 'e':
               dictext_g_state = DIC_State_End;
               break;

               default:
               dic_input_word_putc( c );
               dic_draw_input_word();
               dic_draw_input_cursor( FALSE );
               break;
            }
         }
         break;
#endif
         case DIC_Cmd_Enter:
         if( !dic_search_word( dic_g_input_word ))
         {
            dic_g_word_info_item.word[ 0 ] = '!';
            dic_g_word_info_item.word[ 1 ] = '\0';
            break;
         }
         if( dic_string_comp( dic_g_input_word, dic_g_word_info_item.word )!= 0 )
         {
            SDKLoadCode( TEXT_VIEWER_TEMP_CODE16 );
            dic_no_word_display();
            dictext_g_state = DIC_State_SearchFailed;
         }
         else
         {
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 32, 0, DIC_Num_InputLine * DICTEXT_Title_CharWidth - 61, DICTEXT_Title_Height );
            //lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 8 );
            lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 24 );
            lcd_goto_xy( 0, 8 );
            Ih_set_curr_font( DICTEXT_SmallFont );
            lcd_puts( dic_g_word_info_item.word );
            dictext_g_state = DIC_State_Search;
         }
         break;

         case DIC_Cmd_CancelInput:
         dic_input_word_clear();
         dic_draw_input_word();
         dic_draw_input_cursor( FALSE );
         break;

         case DIC_Cmd_PowerOff:
         if( !power_is_vbus_present())
         {
            exit_dictext_to_appli = POWER_OFF_APPLI;
            g_power_off_mode = POWER_OFF;
            dictext_g_cmd_id = DICTEXT_Cmd_Exit;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         }
         break;

         case DIC_Cmd_VolUp:
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_dic_display_volume();
         break;

         case DIC_Cmd_VolDown:
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_dic_display_volume();
         break;

         case DIC_Cmd_Backspace:
         dic_input_word_backward();
         dic_draw_input_word();
         //dic_draw_cursor( TRUE );
         break;
      }
      dictext_reset_cmd();
#endif
      break;

      case DIC_State_Search:
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE07 );
      dic_word_find_prepare();
#if 0
      textviewer_g_text_type = TEXTBASE_Type_SmallText;
      textviewer_g_start_offset = dic_g_word_info_item.content_addr + 11;
      textviewer_g_size = dic_g_word_content_item.content_size;

      textviewer_g_start_x = 0;
      textviewer_g_start_y = 16;
      textviewer_g_lines = 1;
      if( textviewer_g_lines > TEXTVIEWER_Num_MaxContentLines )
         textviewer_g_lines = TEXTVIEWER_Num_MaxContentLines;
      textviewer_state_back = DIC_State_Refresh;
      //dictext_g_state = TEXTVIEWER_State_TextView_Init;

      dictext_g_last_file_pos = ( *fat_g_cur_file )->position;
      dictext_g_back_state = TEXTVIEWER_State_TextView_Init;
      dictext_g_voice_start_addr = dic_g_word_content_item.voice_addr;
      dictext_g_voice_end_addr = dictext_g_voice_start_addr + dic_g_word_content_item.voice_size;
      dictext_g_voice_codec = dic_g_word_content_item.voice_codec;

      dictext_g_state = DICTEXT_State_VoiceStart;
#endif
      break;

      case DIC_State_SearchFailed:
      if( !Timer_is_timeout( time_out ))
         break;
	  dic_input_word_clear();
	  dic_draw_input_word();
	  dic_draw_input_cursor( FALSE );
      dictext_g_state = DIC_State_Refresh;
      break;

      case DIC_State_End:
      SDKLoadCode( TEXT_VIEWER_END_CODE );
      dic_end_exit_deal();
#if 0
      dic_file_open_flag = FALSE;
      dic_remote_task_stop();
      dic_file_close();
      dictext_g_state = FILEBROWSER_State_Refresh;
#endif
      break;

      case TEXTVIEWER_State_TextView_Init:
	  //ndString((U8 *)"\r\nr_f_w2514!");
	   SDKLoadCode( READY_DISPLAY_CODE );
	   dic_text_display();
	   dictext_g_state = TEXTVIEWER_State_TextView;
/*	  
      SDKLoadCode( TEXT_BASE_TEMP_CODE00 );
      ( void )textbase_init( textviewer_g_text_type, NULL );     
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE12 );
      if( !textviewer_init( &textviewer_g_start_offset, &textviewer_g_size ))
      {
         SDKLoadCode( TEXT_VIEWER_INIT_FAIL_CODE );
         viewer_init_fail_deal();
         break;
      }
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE13 );
      textviewer_display_text();
      dictext_g_state = TEXTVIEWER_State_TextView;
//*/	  
      break;

      case TEXTVIEWER_State_TextView:
      dic_remote_task();
      if(( dictext_g_cmd_id == DICTEXT_Cmd_None )&& dic_remote_get_info())
      {
         switch( dic_remote_get_result()->remote_code_type )
         {
            case DICREMOTE_Code_AlphaStart:
			case DICREMOTE_Code_Hyphen:
			case DICREMOTE_Code_Space:
			case DICREMOTE_Code_Dot:
            dic_input_word_clear();
            dictext_g_cmd_id = TEXTVIEWER_Cmd_Back;
            //dic_remote_reset_info();
            break;
            
            case DICREMOTE_Code_CancelInput:
            dictext_g_cmd_id = TEXTVIEWER_Cmd_Back;
            break;

            case DICREMOTE_Code_Enter:
            //case DICREMOTE_Code_Exit:
            dictext_g_cmd_id = TEXTVIEWER_Cmd_Voice;
            dic_remote_reset_info();
            break;

            case DICREMOTE_Code_Exit:
            dictext_g_cmd_id = TEXTVIEWER_Cmd_Back;
#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //if( g_b_dic_remote_on )
            if( g_b_remote_on )
            {
               dictext_g_state = DICTEXT_State_End;
               exit_dictext_to_appli = AP3_APPLI;
               dictext_g_cmd_id = DICTEXT_Cmd_None; 
            }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //dic_remote_reset_info();
            dictext_g_state = DICTEXT_State_End;
            exit_dictext_to_appli = IDLE_APPLI;
            dictext_g_cmd_id = DICTEXT_Cmd_None; 
            break;

            case DICREMOTE_Code_PowerOff:
            if( !power_is_vbus_present())
            {
               exit_dictext_to_appli = POWER_OFF_APPLI;
               g_power_off_mode = POWER_OFF;
               dictext_g_cmd_id = DICTEXT_Cmd_Exit;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
               g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            }
            break;
            
            case DICREMOTE_Code_VolUp:
            //srvplayer_volume_change( SRVPLAYER_VOL_UP );
            //mmi_dic_display_volume();
            dictext_g_cmd_id = DIC_Cmd_VolUp;
            dic_remote_reset_info();
            break;
            
            case DICREMOTE_Code_VolDown:
            //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            //mmi_dic_display_volume();
            dictext_g_cmd_id = DIC_Cmd_VolDown;
            dic_remote_reset_info();
            break;

            case DICREMOTE_Code_Backspace:
            dictext_g_cmd_id = TEXTVIEWER_Cmd_Back;
            break;
         }
      }
      switch( dictext_g_cmd_id )
      {
      	// SendString((U8 *)"\r\ndicremote_g_state=");
		 //send_hex_u8(dicremote_g_state);
         case TEXTVIEWER_Cmd_ScrollUp:
         if( dicremote_g_state == DICREMOTE_State_Voice )
            break;
/*		 
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE15 );
         textviewer_scroll_up();
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE13 );
         textviewer_display_text();
		 


		 if(b_LastPage_Flag)break;
		 current_display_num = text_off_addr[Display_Page_Num];
	     ch_display_address = ch_begin_address + (U32)current_display_num;
		 mail_send_event( EVT_SEARCH_FLUSH_TEXT, 0);
*/		 
         break;

         case TEXTVIEWER_Cmd_ScrollDown:
         if( dicremote_g_state == DICREMOTE_State_Voice )
            break;
	/*
		   if(Display_Page_Num <= 1)break;
		   Display_Page_Num -= 2;
		   current_display_num = text_off_addr[Display_Page_Num];
		   ch_display_address = ch_begin_address + (U32)current_display_num;
		   mail_send_event( EVT_SEARCH_FLUSH_TEXT, 0);

		 
		 
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE14 );
         textviewer_scroll_down();
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE13 );
         textviewer_display_text();
*/		 
         break;

         /*case TEXTVIEWER_Cmd_ScrollTo:
         textviewer_scroll_to( dictext_g_param );
         textviewer_display_text();
         break;*/

         case TEXTVIEWER_Cmd_Back:
         dictext_g_state = TEXTVIEWER_State_TextView_End;
         break;

         case TEXTVIEWER_Cmd_Voice:
         dictext_g_last_file_pos = ( *fat_g_cur_file )->position;
         dictext_g_back_state = TEXTVIEWER_State_TextView;
         //dictext_g_voice_start_addr = dic_g_word_content_item.voice_addr;
         //dictext_g_voice_end_addr = dic_g_word_content_item.voice_size;
         //dictext_g_voice_codec = dic_g_word_content_item.voice_codec;

         dictext_g_state = DICTEXT_State_VoiceStart;
         Point_Read = 0;

         Point_Write = save_Point_Write;
         break;
         
         /*
         case TEXTVIEWER_Cmd_Debug:
         {
         U8	iter;
         extern U16 _MEM_TYPE_SLOW_ text_g_lines_nums[];
         lcd_clear_part_of_screen( 0, 16, 128, 48 );
         for( iter = 0; iter < 3; ++iter )
         {
            sprintf( dic_g_display_buffer,"%d %d",( int )( textviewer_g_lines_info[ iter ].line_num ),( int )( text_g_lines_nums[ textviewer_g_lines_info[ iter ].line_num % TEXT_Num_CacheLines ]));
            lcd_goto_xy( 0, 16 + iter * 16 );
            lcd_puts( dic_g_display_buffer );
         }
         }
         break;
         */

         case DIC_Cmd_VolUp:
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_dic_display_volume();
         break;

         case DIC_Cmd_VolDown:
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_dic_display_volume();
         break;
      }
      dictext_reset_cmd();
      break;

      case TEXTVIEWER_State_TextView_End:
      if(( textviewer_state_back == FILEBROWSER_State_Refresh )
         ||( textviewer_state_back == DIC_State_Refresh ))
      {
         //text_end();
         textbase_end();
      }
      /*else if( textviewer_state_back == DIC_State_Refresh )
      {
         text_end();
         textbase_end();
      }*/
      dictext_g_state = textviewer_state_back;
      break;

      case DICTEXT_State_VoiceStart:
      SDKLoadCode( TEXT_VIEWER_TEMP_CODE11 );
      if( !dic_word_voice_deal())
         break;
#if 0
      /*if( player_ap3_is_busy())
      {
         player_ap3_stop();
      }*/
      if(( !dictext_g_voice_start_addr )||( dictext_g_voice_end_addr < 512ul ))
      {
         dictext_g_state = DICTEXT_State_VoiceEnd;
         break;
      }
      ap4_audio_var.audio_var.file_size_count = dictext_g_voice_start_addr;
      if(( ap4_audio_var.audio_var.file_size_count & 0x1FF )!= 0 )
      {
         dictext_g_state = DICTEXT_State_VoiceEnd;
         break;
      }
      ap4_audio_var.audio_var.play_file_size = dictext_g_voice_end_addr;
      switch( dictext_g_voice_codec )
      {
         case 0:
         audio_is_wav = 0;
         break;

         case 2:
         audio_is_wav = 1;
         //break;

         default:
         dictext_g_state = DICTEXT_State_VoiceEnd;
         return;
      }
      //LROM_SCRAMBLE = 0x00;
      //LROM_EXCHANGE = 0x00;
      Key_encrypt_mp3 = 0x00;
      wav_encrypt = FALSE;
      if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
      {
         dictext_g_state = DICTEXT_State_VoiceEnd;
         break;
      }
#endif
      if( !player_music_start())
      {
         //player_ap3_stop();
         //dictext_g_state = DICTEXT_State_VoiceEnd;
         break;
      }
      dictext_g_state = DICTEXT_State_Voice;
      break;

      case DICTEXT_State_Voice:
      if( !player_ap3_is_busy())
      {
         //dictext_g_state = DICTEXT_State_VoiceEnd;
         dictext_g_state = DICTEXT_State_VoiceStart;
      }
      break;

      case DICTEXT_State_VoiceEnd:
      ( void )file_seek( dictext_g_last_file_pos, FS_SEEK_SET );
      dictext_g_state = dictext_g_back_state;
      break;

      case DICTEXT_State_Error:
      SDKLoadCode( TEXT_VIEWER_ERROR_CODE );
      dic_error_deal();
#if 0
      {
         U32 time_out = Timer_set_timer( TIMER_S( 1 ));
         //text_end();
         textbase_end();
         exit_dictext_to_appli = IDLE_APPLI;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         //if( g_b_dic_remote_on )
         /*if( g_b_remote_on )
         {
            exit_dictext_to_appli = AP3_APPLI;
         }*/
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         while( !Timer_is_timeout( time_out ));
         dictext_g_state = DICTEXT_State_End;
      }
      dictext_reset_cmd();
#endif
      break;

      case DICTEXT_State_End:
      /*
      switch( dictext_g_cmd_id )
      {
         dictext_g_state = DICTEXT_State_Idle;
      }
      dictext_reset_cmd();
      */
      mail_send_event( EVT_APPLI_KILLED, ((( U16 )TASK_DIC )<< 8 )|( U16 )exit_dictext_to_appli );
      dictext_g_state = DICTEXT_State_Idle;
      break;
   }
   if( dictext_g_last_state != dictext_g_state )
   {
      
   }
   dictext_g_last_state = dictext_g_state;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE07

void dic_word_find_prepare( void )
{
   textviewer_g_text_type = TEXTBASE_Type_SmallText;
   //textviewer_g_start_offset = dic_g_word_info_item.content_addr + 11;
   textviewer_g_size = dic_g_word_content_item.content_size;

   textviewer_g_start_x = 0;
   textviewer_g_start_y = 24;
   textviewer_g_lines = 2;
   if( textviewer_g_lines > TEXTVIEWER_Num_MaxContentLines )
      textviewer_g_lines = TEXTVIEWER_Num_MaxContentLines;
   textviewer_state_back = DIC_State_Refresh;
   //dictext_g_state = TEXTVIEWER_State_TextView_Init;

   dictext_g_last_file_pos = ( *fat_g_cur_file )->position;
   dictext_g_back_state = TEXTVIEWER_State_TextView_Init;
   //dictext_g_voice_start_addr = dic_g_word_content_item.voice_addr;
   //dictext_g_voice_end_addr = dic_g_word_content_item.voice_size;
   //dictext_g_voice_codec = dic_g_word_content_item.voice_codec;

   dictext_g_state = DICTEXT_State_VoiceStart;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE08

void dic_enter_prepare( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //  lcd_clear_part_of_screen( 32, 0, DICTEXT_Screen_Width - 61, 8 );
   //dic_draw_keyboard();
   //lcd_clear_part_of_screen( 0, 16, DICTEXT_Screen_Width, 16 );
   //lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 8 );
   lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, (LCD_HEIGHT-8));
   dic_draw_input_word();
   dic_draw_input_cursor( FALSE );
   //dic_draw_cursor( TRUE );
   dictext_g_state = DIC_State_Interface;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE09

void dic_remote_voice_end_deal( void )
{
  
   switch( dictext_g_cmd_id )
   {
      case DIC_Cmd_Exit:
      dictext_g_state = DIC_State_End;
#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //if( g_b_dic_remote_on )
      if( g_b_remote_on )
      {
         dictext_g_state = DICTEXT_State_End;
         exit_dictext_to_appli = AP3_APPLI;
      }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
	  dictext_g_state = DICTEXT_State_End;
	  exit_dictext_to_appli = IDLE_APPLI;

      break;

      case DIC_Cmd_KeyInput:
      dic_input_word_putc( dic_to_lower( dictext_g_param ));
      //dic_g_keyboard_cursor = dic_char_to_coord( dic_to_lower( dictext_g_param ));
      dic_draw_input_word();
      dic_draw_input_cursor( FALSE );
      //dic_draw_keyboard();
      //dic_draw_cursor( TRUE );
      break;
#if 0
      case DIC_Cmd_KeyboardScroll:
      dic_draw_cursor( FALSE );
      dic_keyboard_cursor_move( DIC_Direction_Down );
      dic_draw_keyboard();
      dic_draw_cursor( TRUE );
      break;

      case DIC_Cmd_KeyboardLeft:
      if( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines )
      {
         dic_input_word_backward();
         dic_draw_input_word();
         dic_draw_input_cursor( TRUE );
      }
      else
      {
         dic_draw_keyboard_cursor( FALSE );
         dic_keyboard_cursor_move( DIC_Direction_Left );
         while( dic_coord_to_char( dic_g_keyboard_cursor ) == ' ')
            dic_keyboard_cursor_move( DIC_Direction_Left );
         dic_draw_keyboard_cursor( TRUE );
      }
      break;

      case DIC_Cmd_KeyboardRight:
      if( dic_g_keyboard_cursor.pos_y != DIC_Num_KeyboardLines )
      {
         dic_draw_keyboard_cursor( FALSE );
         dic_keyboard_cursor_move( DIC_Direction_Right );
         while( dic_coord_to_char( dic_g_keyboard_cursor ) == ' ')
            dic_keyboard_cursor_move( DIC_Direction_Right );
         dic_draw_keyboard_cursor( TRUE );
      }
      break;

      case DIC_Cmd_KeyboardSelect:
      if( dic_g_keyboard_cursor.pos_y >= DIC_Num_KeyboardLines )
      {
         goto __FlagEnter;
      }
      else
      {
         U8 c;
         c = dic_coord_to_char( dic_g_keyboard_cursor );
         switch( c )
         {
            case ' ':
            break;

            case 'b':
            dic_input_word_backward();
            dic_draw_input_word();
            dic_draw_input_cursor( dic_g_keyboard_cursor.pos_y == DIC_Num_KeyboardLines );
            break;

            case 'e':
            dictext_g_state = DIC_State_End;
            break;

            default:
            dic_input_word_putc( c );
            dic_draw_input_word();
            dic_draw_input_cursor( FALSE );
            break;
         }
      }
      break;
#endif
      case DIC_Cmd_Enter:
	 
      SDKLoadCode( DIC_SEARCH_WORD_CODE00 );
      if( !dic_search_word( dic_g_input_word ))goto no_find_1;
	  SDKLoadCode(DIC_SEARCH_WORD_CODE01);
	  if(!dic_add_word_voice())
      {
no_find_1:      
         SendString((U8 *)"\r\nnofind!");
         dic_g_word_info_item.word[ 0 ] = '!';
         dic_g_word_info_item.word[ 1 ] = '\0';
no_find_2:
		SDKLoadCode( TEXT_VIEWER_TEMP_CODE16 );
		dic_no_word_display();
		dictext_g_state = DIC_State_SearchFailed;
		time_out = Timer_set_timer( TIMER_S( 2 ));
		

        break;
      }
	  SendString((U8 *)"\r\nfind!");
      if( dic_string_comp( dic_g_input_word, dic_g_word_info_item.word )!= 0 )
      {
      	goto no_find_2;
      }
      else
      {
         SDKLoadCode( TEXT_VIEWER_TEMP_CODE10 );
         dic_word_display();
#if 0
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 32, 0, DIC_Num_InputLine * DICTEXT_Title_CharWidth - 61, DICTEXT_Title_Height );
         //lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 8 );
         lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 24 );
         lcd_goto_xy( 0, 8 );
         Ih_set_curr_font( DICTEXT_SmallFont );
         lcd_puts( dic_g_word_info_item.word );
         dictext_g_state = DIC_State_Search;
#endif
      }
      break;

      case DIC_Cmd_CancelInput:
      dic_input_word_clear();
      dic_draw_input_word();
      dic_draw_input_cursor( FALSE );
      break;

      case DIC_Cmd_PowerOff:
      if( !power_is_vbus_present())
      {
         exit_dictext_to_appli = POWER_OFF_APPLI;
         g_power_off_mode = POWER_OFF;
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      }
      break;

      case DIC_Cmd_VolUp:
      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      mmi_dic_display_volume();
      break;

      case DIC_Cmd_VolDown:
      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      mmi_dic_display_volume();
      break;

      case DIC_Cmd_Backspace:
      dic_input_word_backward();
      dic_draw_input_word();
      //dic_draw_cursor( TRUE );
      break;
   }
   dictext_reset_cmd();
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE10

void dic_word_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  // lcd_clear_part_of_screen( 32, 0, DIC_Num_InputLine * DICTEXT_Title_CharWidth - 61, DICTEXT_Title_Height );
   //lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 8 );
   lcd_clear_part_of_screen( 0, 8, DICTEXT_Screen_Width, 56 );
   lcd_goto_xy( 0, 8 );
   //Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts( dic_g_word_info_item.word );
   dictext_g_state = DIC_State_Search;
}


#pragma CODE = TEXT_VIEWER_TEMP_CODE11

Bool dic_word_voice_deal( void )
{
   if( Point_Write == Point_Read )
   {
      dictext_g_state = DICTEXT_State_VoiceEnd;
      return FALSE;
   }
   if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, Array_V[ Point_Read++ ], 9 ))
      return FALSE;
   dictext_g_voice_start_addr = dic_g_word_content_item.voice_addr;
   dictext_g_voice_end_addr = dic_g_word_content_item.voice_size;
   dictext_g_voice_codec = dic_g_word_content_item.voice_codec;

   if( !dictext_g_voice_start_addr 
      ||( dictext_g_voice_end_addr < 512ul ))
   {
      //dictext_g_state = DICTEXT_State_VoiceEnd;
      return FALSE;
   }
   ap4_audio_var.audio_var.file_size_count = dictext_g_voice_start_addr;
   if(( ap4_audio_var.audio_var.file_size_count & 0x1FF )!= 0 )
   {
      //dictext_g_state = DICTEXT_State_VoiceEnd;
      return FALSE;
   }
   ap4_audio_var.audio_var.play_file_size = dictext_g_voice_end_addr;
   switch( dictext_g_voice_codec )
   {
      case 0:
      audio_is_wav = 0;
      break;

      case 2:
      audio_is_wav = 1;
      //break;

      default:
      //dictext_g_state = DICTEXT_State_VoiceEnd;
      return FALSE;
   }
   //LROM_SCRAMBLE = 0x00;
   //LROM_EXCHANGE = 0x00;
   Key_encrypt_mp3 = 0x00;
   //wav_encrypt = FALSE;
   if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
   {
      //dictext_g_state = DICTEXT_State_VoiceEnd;
      return FALSE;
   }
   return TRUE;
}


#pragma CODE = TEXT_VIEWER_END_CODE

void dic_end_exit_deal( void )
{
   dic_file_open_flag = FALSE;
   dic_remote_task_stop();
   dic_file_close();
   dictext_g_state = FILEBROWSER_State_Refresh;
}


#pragma CODE = TEXT_VIEWER_INIT_FAIL_CODE

void viewer_init_fail_deal( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  // lcd_clear_screen();
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
   lcd_goto_xy( 15, 24 );
   //Ih_set_curr_font( DICTEXT_SmallFont );
   Ih_set_curr_font( DICTEXT_BigFont );
   lcd_puts(( U8 *)"text_init fail" );
   dictext_g_state = DICTEXT_State_Error;
   time_out = Timer_set_timer( TIMER_S( 1 ));
}


#pragma CODE = TEXT_VIEWER_ERROR_CODE

void dic_error_deal( void )
{
   if( !Timer_is_timeout( time_out ))
      return;
   //text_end();
   textbase_end();
   exit_dictext_to_appli = IDLE_APPLI;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   //if( g_b_dic_remote_on )
   /*if( g_b_remote_on )
   {
      exit_dictext_to_appli = AP3_APPLI;
   }*/
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   dictext_g_state = DICTEXT_State_End;
   dictext_reset_cmd();
}


#pragma CODE = TEXT_CH_DISPLAY_CODE
Bool text_chinese_display( U32 fan_add )
{
//#define TEXT_BUF_LEN    42
   U8 _MEM_TYPE_SLOW_ old_lang;
   U32 _MEM_TYPE_SLOW_ Iaddress;
   U16 _MEM_TYPE_SLOW_ text_length,temp_len,all_len = 0;
   	U16 i,j,m,temp_file;
	Bool temp_flag;
	__X FS_ERR fs_err;
   __X FCFILE *Temp_File;
	SendString((U8 *)"\r\nflushtext");
   //U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];
   
 //   b_findword_ch_display = FALSE;
    Iaddress = fan_add;
	
//	memset( text_buf, 0, TEXT_BUF_LEN );
	 //text_buf[0] = 0xFF;
	 //text_buf[1] = 0xFE;
	if(!g_b_flush_display)return FALSE;
#if 1//( UNICODE_TYPE == DISABLE )
   //strcpy( tisiFileShortpath,DIC_PATH);
   strcpy( tisiFileShortpath,(char *)&current_dic_dir);
   strcat( tisiFileShortpath,(char *)&current_dic_short_name);
#endif
  //strcpy( tisiFileShortpath,(char *)&current_ap4_short_name);


   SendString((U8 *)tisiFileShortpath);
   Temp_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ, &fs_err );
   if(fs_err != FS_NO_ERROR)return FALSE;

   old_lang = g_setting_list.struct_setting.language_ihm;
   g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
   Ih_set_curr_font( IDC_FONT_16 );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   b_LastPage_Flag = FALSE;
  for(i=0;i<DIC_LINE_PAGE;i++ ){
  	 if((leng_ch <= current_display_num))break;
	 
  	 temp_len = leng_ch - current_display_num;
	 if(temp_len >= DIC_MAX_NUM_LINE){
		temp_len = DIC_MAX_NUM_LINE;
		}
  	 //current_display_num += temp_len;
	 (void)FSSeek( Temp_File, Iaddress );
	 FSRead(Temp_File, (U8 _MEM_TYPE_SLOW_ *)&text_line_buf[2], temp_len, &fs_err);
//	 read_data_from_nf( Iaddress, (U8 _MEM_TYPE_SLOW_ *)&text_line_buf[2], temp_len );
	 text_line_buf[temp_len+2] = 0;
	 text_line_buf[(temp_len+3)] = 0;
	 //SendString((U8 *)"\r\naddress:leng=");
      //send_hex_u32(Iaddress);SendString((U8 *)":");send_hex_u16(temp_len);
	 lcd_goto_xy( 0, 24+i*20 );	
	 lcd_puts( text_line_buf );
///*	
	 SendString((U8 *)"\r\nlen_1:leng_2=");
     send_hex_u16(temp_len);
	 //SendString((U8 *)":");
	 send_hex_u16(lcd_display_num);
//*/	  
	 temp_len = (lcd_display_num)<<1;
	 all_len += temp_len;
	 current_display_num += temp_len;
	 Iaddress += (U32)temp_len;
  	}
   g_setting_list.struct_setting.language_ihm = old_lang;
   FSClose(Temp_File);
   if(current_display_num >= leng_ch){
   	b_LastPage_Flag = TRUE;
   	}
    Display_Page_Num ++;
	if(Display_Page_Num >= DIC_MAX_PAGE_NUMBER){
		b_LastPage_Flag = TRUE;
		}
	else{
		text_off_addr[Display_Page_Num] = text_off_addr[(Display_Page_Num-1)] + all_len;
		}
	 //SendString((U8 *)"\r\npage_num:off=");
     //send_hex_u16(Display_Page_Num);SendString((U8 *)":");send_hex_u16(all_len);
//#undef TEXT_BUF_LEN
   return TRUE;
}

#pragma CODE = READY_DISPLAY_CODE

void dic_text_display( void )
{
//#define TEXT_BUF_LEN    64
   //U8 _MEM_TYPE_SLOW_ old_lang;
   U32 _MEM_TYPE_SLOW_ Iaddress;
   U16 _MEM_TYPE_SLOW_ text_length;
 //  U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];
   SendString((U8*)"\r\na_d_t");
   //action_display_text = FALSE;
   //if(!g_b_enable_flush)return;
  // g_b_enable_flush = FALSE;
   
/*
  // if(Current_Act.Type == Type_Play){
  if(All_Control.Version < 15){
  	lcd_goto_xy( 12, 32 );
	 if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
	 	lcd_puts((U8 *)"本书无屏显内容");
	 	}
	 else{
	 	//lcd_puts((U8 *)"No Unit");
	 	lcd_puts((U8 *)"No display content");
	 	}
  	return;
  	}
&textviewer_g_start_offset, &textviewer_g_size
  if(!action_display_text){
   //	if(State_Search >= Browser_File_Init )return;
    //if()
   	leng_ch = Len_Cap_Book;
	ch_begin_address = News_Book.Point_Book_Cap + 4;;
   	}
   else{

   Iaddress = action_address + ( U32 )5;
   
   read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&text_length, ( U16 )sizeof( U16 ));
   if( !text_length )
      return;
   leng_ch = text_length;
   ch_begin_address = action_address + ( U32 )9;
   }*/
 //  g_b_ap4_caption = TRUE;
   Ih_set_curr_font( IDC_FONT_16 );
   ch_begin_address = textviewer_g_start_offset+2;
   leng_ch = (U16)textviewer_g_size;
  // action_display_text = FALSE;
   //SDKLoadCode( FINDWORD_TEMP01_CODE );
   //ready_find_word();
   current_display_num = 0;
   Display_Page_Num = 0;
   b_LastPage_Flag = FALSE;
   if(2 >= leng_ch)return;
   //findword_address = action_address + (U32)(Current_Act.act_info.speech_info.Num_Voice * 9 + 5);
   //ch_begin_address += (U32)4;
   //ch_begin_address = action_address + ( U32 )9;
   leng_ch -= (U16)2;
//   ch_begin_address	+= (U32)leng_en;
   memset(text_off_addr,0,DIC_MAX_PAGE_NUMBER);
   ch_display_address = ch_begin_address;
   //b_findword_ch_display = TRUE;
   mail_send_event( EVT_DIC_FLUSH_TEXT, 0);
   SendString((U8*)"\r\nt_d:l=");
   send_hex_u32(ch_begin_address);
   SendString((U8*)":");
   send_hex_u16(leng_ch);
   text_line_buf[0] = 0xFF;
   text_line_buf[1] = 0xFE;
   text_line_buf[(LEN_DISPLAY_BUF-1)] = 0;
   text_line_buf[(LEN_DISPLAY_BUF-2)] = 0;
   g_b_flush_display = TRUE;
 /*
   memset( text_buf, 0, TEXT_BUF_LEN );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
   
   old_lang = g_setting_list.struct_setting.language_ihm;
   g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
   
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_goto_xy( 0, 24 );
   if(text_length > 32){
   	temp_len = 32;
   	}
   else{
   	temp_len = text_length;
   	}
   read_data_from_nf( Iaddress + 2, text_buf, temp_len );
   text_buf[temp_len] = 0;
   lcd_puts( text_buf );
   
   if( text_length > 32 )
   {
      lcd_goto_xy( 0, 40 );
	  memset( text_buf, 0, TEXT_BUF_LEN );
	   read_data_from_nf( Iaddress + 34, text_buf, TEXT_BUF_LEN - 2 );
	   lcd_puts( text_buf );
   }
   g_setting_list.struct_setting.language_ihm = old_lang;

#undef TEXT_BUF_LEN
*/
}


#endif   //#if( DIC == ENABLE )
#endif   //_text_viewer_c_
