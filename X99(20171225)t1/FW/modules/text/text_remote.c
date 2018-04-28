#ifndef _text_remote_c_
#define _text_remote_c_

#include "config.h"
#include "text_remote.h"
#include <stdio.h>

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
//-------------语音播放接口-------------
#include "modules/player_ap3/player_ap3.h"
#include "mmi/player/srv_player.h"
//-------------键盘-------------
#include "mmi\kbd\keypad.h"
//-------------调试工具-------------
//-------------C标准库的字符处理-------------
#include <ctype.h>
//=========================================
#include "text.h"
#include "text_base.h"
#include "text_viewer.h"

#include "drivers\oid\oid.h"
#include "modules\search\search_remote.h"
#include "modules\search\search.h"
#include "mmi\dictionary\mmi_dictionary.h"
#include "mmi\player\mmi_player.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = TEXT_REMOTE_DATA
#pragma CODE = TEXT_REMOTE_CODE

ImageDeclareType( TEXT_REMOTE_TEMP_CODE01 );
ImageDeclareType( TEXT_REMOTE_TEMP_CODE02 );
ImageDeclareType( READBNL_CODE );
#if( DIC == ENABLE )
#define  DICREMOTE_SignatureVersion 0xFFFFEEEE

U16               _MEM_TYPE_SLOW_   dic_remote_version;
U8                _MEM_TYPE_SLOW_   DicRemoteEncryptKey;
U32               _MEM_TYPE_SLOW_   DicRemoteFlag;

U8                _MEM_TYPE_SLOW_   dicremote_g_version /*= DICREMOTE_Version*/;

U32               _MEM_TYPE_SLOW_   dicremote_g_base_addr;
DicRemoteHeader   _MEM_TYPE_SLOW_   dicremote_g_header;
U32               _MEM_TYPE_SLOW_   dicremote_g_table_addr;

DicRemoteItem     _MEM_TYPE_SLOW_   dicremote_g_item;

Bool                                dicremote_g_info;
DicRemoteResult   _MEM_TYPE_SLOW_   dicremote_g_result;

U8                _MEM_TYPE_SLOW_   dicremote_g_state /*= DICREMOTE_State_Idle*/;


extern   U32      _MEM_TYPE_SLOW_   dictext_g_voice_start_addr;
extern   U32      _MEM_TYPE_SLOW_   dictext_g_voice_end_addr;
extern   U8       _MEM_TYPE_SLOW_   dictext_g_voice_codec;

//extern   Bool                       g_oid;

//extern   xdata    AL_Variables     	varAl;


Bool  dic_remote_voice_deal   ( void );
void  dic_remote_init_deal    ( void );


Bool dic_remote_get_info( void )
{
   return dicremote_g_info;
}


void dic_remote_reset_info( void )
{
   dicremote_g_info = FALSE;
}


#pragma CODE = TEXT_REMOTE_TEMP_CODE00
//


Bool dic_remote_get_version( void )
{
   U32 _MEM_TYPE_SLOW_ signature;
   U8 _MEM_TYPE_SLOW_ temp_flag;

   if( !file_seek( sizeof( signature ), FS_SEEK_END ))
      return FALSE;
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ * )( &signature ), sizeof( signature )) != sizeof( signature ))
      return FALSE;
   if( signature != DICREMOTE_SignatureVersion )
      return TRUE;
   if( !file_seek( sizeof( signature )+ 1, FS_SEEK_END ))
      return FALSE;
   //temp_flag = ( U8 )file_getc();
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_flag, sizeof( U8 ));
   if( !( temp_flag & NINE_BYTE_SPEECH_FLAG ))
      return FALSE;
   if( temp_flag & SPECIAL_TABLE_FLAG )
      dicremote_g_version = DICREMOTE_Version_02;
   else
      dicremote_g_version = DICREMOTE_Version_01;
   if( !file_seek( 10, FS_SEEK_END ))
      return FALSE;
   //MSB( dic_remote_version ) = ( U8 )file_getc();
   //LSB( dic_remote_version ) = ( U8 )file_getc();
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dic_remote_version, sizeof( U16 ));
   if( dic_remote_version >= 4 )
   {
      if( !file_seek( 11, FS_SEEK_END ))
         return FALSE;
      //DicRemoteEncryptKey = ( U8 )file_getc();
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&DicRemoteEncryptKey, sizeof( U8 ));
      if( !file_seek( 8, FS_SEEK_END ))
         return FALSE;
      //MSB0( DicRemoteFlag ) = ( U8 )file_getc();
      //MSB1( DicRemoteFlag ) = ( U8 )file_getc();
      //MSB2( DicRemoteFlag ) = ( U8 )file_getc();
      //MSB3( DicRemoteFlag ) = ( U8 )file_getc();
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&DicRemoteFlag, sizeof( U32 ));
   }
   return TRUE;
}



#pragma CODE = TEXT_REMOTE_CODE
DicRemoteResult *dic_remote_get_result( void )
{
   return &dicremote_g_result;
}


Bool dic_remote_search_result( void )
{
#if( DICHOTOMY == DISABLE )
   U16 _MEM_TYPE_SLOW_ iter;
   if( g_oid )
   {
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
      }
      init_array_v();
      g_oid = FALSE;
/*
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
      if( g_b_lcd_backlight_off )
      {
         Lcd_rearm_backlight_on();
		 g_mmi_lcd_backlight_off = FALSE;
         return FALSE;
      }
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )		  

	  
	  SDKLoadCode(READBNL_CODE);
	  if(0xFFFF != read_bnl_file(Current_Index)){
	  	g_b_xuanshu_flag = TRUE;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
		return FALSE;
	  	}
//*/	  
      for( iter = 0; iter < dicremote_g_header.items_num; ++iter )
      {
         if( !file_seek( dicremote_g_table_addr + iter * 13, FS_SEEK_SET ))
            return FALSE;
         if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dicremote_g_item, 13 )!= 13 )
            return FALSE;
         if( Current_Index == dicremote_g_item.oid_index )
         {
            dicremote_g_result.remote_code = dicremote_g_item.remote_code;
            dicremote_g_result.remote_code_type = dicremote_g_result.remote_code;
            if(( dicremote_g_result.remote_code >= DICREMOTE_Code_AlphaStart )&&( dicremote_g_result.remote_code <= DICREMOTE_Code_AlphaEnd ))
               dicremote_g_result.remote_code_type = DICREMOTE_Code_AlphaStart;
            else if(( dicremote_g_result.remote_code >= DICREMOTE_Code_NumStart )&&( dicremote_g_result.remote_code <= DICREMOTE_Code_NumEnd ))
               dicremote_g_result.remote_code_type = DICREMOTE_Code_NumStart;
            return TRUE;
         }
      }
   }
#elif( DICHOTOMY == ENABLE )
   U16 _MEM_TYPE_SLOW_ iter_left;
   U16 _MEM_TYPE_SLOW_ iter_right;
   U16 _MEM_TYPE_SLOW_ middle;

   if( g_oid )
   {
  	 SendString((U8 *)"\r\ng_oid");
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
      }
      init_array_v();
      g_oid = FALSE;
/*
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
      if( g_b_lcd_backlight_off )
      {
         Lcd_rearm_backlight_on();
		 g_mmi_lcd_backlight_off = FALSE;
         return FALSE;
      }
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )		  

	  
	  SDKLoadCode(READBNL_CODE);
	  if(0xFFFF != read_bnl_file(Current_Index)){
	  	g_b_xuanshu_flag = TRUE;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
		return FALSE;
	  	}
//*/	  
//	  SendString((U8 *)"\r\ndicremote_g_table_addr:num=");
//	  send_hex_u32(dicremote_g_table_addr);
//	  SendString((U8 *)":");
//	  send_hex_u16(dicremote_g_header.items_num);
      for( iter_left = 0, iter_right = dicremote_g_header.items_num; iter_left < iter_right; )
      {
         //middle = ( iter_left + iter_right )/ 2;
         middle = ( iter_left + iter_right )>> 1;
         if( !file_seek( dicremote_g_table_addr +( U32 )middle *( U32 )13, FS_SEEK_SET ))
            return FALSE;
         if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dicremote_g_item, 13 )!= 13 )
            return FALSE;
         if( Current_Index == dicremote_g_item.oid_index )
         {
            dicremote_g_result.remote_code = dicremote_g_item.remote_code;
            dicremote_g_result.remote_code_type = dicremote_g_result.remote_code;
            if(( dicremote_g_result.remote_code >= DICREMOTE_Code_AlphaStart )&&( dicremote_g_result.remote_code <= DICREMOTE_Code_AlphaEnd ))
               dicremote_g_result.remote_code_type = DICREMOTE_Code_AlphaStart;
            else if(( dicremote_g_result.remote_code >= DICREMOTE_Code_NumStart )&&( dicremote_g_result.remote_code <= DICREMOTE_Code_NumEnd ))
               dicremote_g_result.remote_code_type = DICREMOTE_Code_NumStart;
            return TRUE;
         }
         else if( Current_Index > dicremote_g_item.oid_index )
         {
            iter_left = middle + 1;
         }
         else if( Current_Index < dicremote_g_item.oid_index )
         {
            iter_right = middle ;
         }
      }
   }
#endif   //if( DICHOTOMY == DISABLE )
   return FALSE;
}


#pragma CODE = TEXT_REMOTE_TEMP_CODE00

Bool dic_remote_task_init( U32 remote_table_addr )
{
   dicremote_g_state = DICREMOTE_State_Idle;
   dicremote_g_base_addr = remote_table_addr;
   if( dicremote_g_base_addr == 0 )
      return FALSE;

//  SendString((U8 *)"\r\nremote_table_addr=");
//	  send_hex_u32(remote_table_addr);
   dicremote_g_version = DICREMOTE_Version;
   dic_remote_version = 0x01;
   if( !dic_remote_get_version())
      return FALSE;
   if( dicremote_g_version == DICREMOTE_Version_01 )
      dicremote_g_table_addr = remote_table_addr + ( U32 )sizeof( DicRemoteHeader );
   else if( dicremote_g_version == DICREMOTE_Version_02 )
      dicremote_g_table_addr = remote_table_addr + ( U32 )sizeof( DicRemoteHeader ) + sizeof( DicRemoteHeaderExtend );


//   SendString((U8 *)"\r\ndicremote_g_base_addr:dicremote_g_table_addr=");
//	  send_hex_u32(dicremote_g_base_addr);
//	  SendString((U8 *)":");
//	  send_hex_u32(dicremote_g_table_addr); 
   dicremote_g_result.remote_code = DICREMOTE_Code_None;
   dicremote_g_result.remote_code_type = DICREMOTE_Code_None;
   return TRUE;
}


void dic_remote_task_start( void )
{
   /*if( dicremote_g_base_addr == 0 )
      return;*/
   //scan_oid_enable();
   dicremote_g_state = DICREMOTE_State_Init;
}


#pragma CODE = TEXT_VIEWER_END_CODE

void dic_remote_task_stop( void )
{
   if( player_ap3_is_busy())
   {
      player_ap3_stop();
   }
   init_array_v();
   scan_oid_disable();
   dicremote_g_state = DICREMOTE_State_Idle;
}


#pragma CODE = TEXT_REMOTE_CODE

/*Bool*/void dic_remote_task( void )
{
   switch( dicremote_g_state )
   {
      case DICREMOTE_State_Idle:
      break;
      
      case DICREMOTE_State_Init:
//	  SendString((U8 *)"\r\nDICREMOTE_State_Init");
      SDKLoadCode( TEXT_REMOTE_TEMP_CODE02 );
      dic_remote_init_deal();
#if 0
      if( !file_seek( dicremote_g_base_addr, FS_SEEK_SET ))
      {
         dicremote_g_state = DICREMOTE_State_Error;
         break;
      }
      if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dicremote_g_header, sizeof( dicremote_g_header ))!= sizeof( dicremote_g_header ))
      {
         dicremote_g_state = DICREMOTE_State_Error;
         break;
      }
      dicremote_g_state = DICREMOTE_State_WaitInput;
#endif
      //return FALSE;
      break;
      
      case DICREMOTE_State_WaitInput:

      if( dic_remote_search_result() == TRUE )
      {
	  	//SendString((U8 *)"\r\nDICREMOTE_State_WaitInput");      
         switch( dicremote_g_result.remote_code_type )
         {
            case DICREMOTE_Code_Enter:
			case DICREMOTE_Code_Exit:	
            break;

			
            
            default:
            dicremote_g_info = TRUE;
            break;
         }
         dicremote_g_state = DICREMOTE_State_VoiceStart;
      }
      break;
      
      case DICREMOTE_State_VoiceStart:
     // SendString((U8 *)"\r\nt_c=");
	 // send_hex_u16(dicremote_g_result.remote_code_type);
     // SendString((U8 *)":");
	 // send_hex_u16(dicremote_g_result.remote_code);
      SDKLoadCode( TEXT_REMOTE_TEMP_CODE01 );
      if( !dic_remote_voice_deal())
         break;
#if 0
      if(( !dicremote_g_item.voice_addr )
         ||( dicremote_g_item.voice_size < 512ul ))
      {
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
         break;
      }
      /*if( player_ap3_is_busy())
      {
         player_ap3_stop();
      }*/
      ap4_audio_var.audio_var.file_size_count = dicremote_g_item.voice_addr;
      if(( ap4_audio_var.audio_var.file_size_count & 0x1FF )!= 0 )
      {
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
         break;
      }
      ap4_audio_var.audio_var.play_file_size = dicremote_g_item.voice_size;
      switch( dictext_g_voice_codec )
      {
         case 0:
         audio_is_wav = 0;
         break;

         case 2:
         audio_is_wav = 1;
         //break;

         default:
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
         return;
      }
      if(( dic_remote_version >= 4 )
         &&( DicRemoteFlag & ENCRYPT_SPEECH_FLAG ))
      {
         U8 codec;
         if( DicRemoteEncryptKey > 0x07 )
         {
            dicremote_g_state = DICREMOTE_State_VoiceEnd;
            break;
         }
         codec = ap4_audio_var.encrypt[ DicRemoteEncryptKey ];
         MSB0( Key_encrypt_mp3 ) = codec;
         MSB1( Key_encrypt_mp3 ) = codec;
         MSB2( Key_encrypt_mp3 ) = codec;
         MSB3( Key_encrypt_mp3 ) = codec;
         //LROM_EXCHANGE = 0x00;
         wav_encrypt = TRUE;
      }
      else
      {
         //LROM_SCRAMBLE = 0x00;
         Key_encrypt_mp3 = 0x00;
         //LROM_EXCHANGE = 0x00;
         wav_encrypt = FALSE;
      }
      ap4_audio_var.audio_var.file_size_count += dicremote_g_base_addr;
      if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
      {
         //return FALSE;
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
         break;
      }
#endif
      //ap4_audio_var.audio_var.play_file_size += ap4_audio_var.audio_var.file_size_count;
      if( !player_music_start())
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
      else
         dicremote_g_state = DICREMOTE_State_Voice;
      break;

      case DICREMOTE_State_Voice:
      if( dic_remote_search_result() == TRUE )
      {
         switch( dicremote_g_result.remote_code_type )
         {
            case DICREMOTE_Code_Enter:
			case DICREMOTE_Code_Exit:
            break;
            
            default:
            dicremote_g_info = TRUE;
            break;
         }
         dicremote_g_state = DICREMOTE_State_VoiceStart;
         break;
      }
      if( !player_ap3_is_busy())
      {
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
      }
      break;
      
      case DICREMOTE_State_VoiceEnd:
      switch( dicremote_g_result.remote_code_type )
      {
         case DICREMOTE_Code_Enter:
		 case DICREMOTE_Code_Exit:
         dicremote_g_info = TRUE;
         break;

		 
         default:
         break;
      }
      dicremote_g_state = DICREMOTE_State_WaitInput;
      break;
      
      /*case DICREMOTE_State_End:
      dicremote_g_state = DICREMOTE_State_Idle;
      break;*/
      
      case DICREMOTE_State_Error:
      //dicremote_g_state = DICREMOTE_State_Idle;
      break;
   }
}


#pragma CODE = TEXT_REMOTE_TEMP_CODE01

Bool dic_remote_voice_deal( void )
{
      	
   if(( !dicremote_g_item.voice_addr )
      ||( dicremote_g_item.voice_size < 512ul ))
   {
      dicremote_g_state = DICREMOTE_State_VoiceEnd;
      return FALSE;
   }
   /*if( player_ap3_is_busy())
   {
      player_ap3_stop();
   }*/
   ap4_audio_var.audio_var.file_size_count = dicremote_g_item.voice_addr;
   if(( ap4_audio_var.audio_var.file_size_count & 0x1FF )!= 0 )
   {
      dicremote_g_state = DICREMOTE_State_VoiceEnd;
      return FALSE;
   }
   ap4_audio_var.audio_var.play_file_size = dicremote_g_item.voice_size;
   dictext_g_voice_codec = dicremote_g_item.voice_codec;
	
#if 1
	if(dictext_g_voice_codec == 0){
		audio_is_wav = 0;
	}else if(dictext_g_voice_codec == 2){
		audio_is_wav = 1;
	}else{
		dicremote_g_state = DICREMOTE_State_VoiceEnd;
      	return FALSE;
		}
#else
   switch( dictext_g_voice_codec )
   {
      case 0:
      audio_is_wav = 0;
      break;

      case 2:
      audio_is_wav = 1;
      //break;

      default:
      dicremote_g_state = DICREMOTE_State_VoiceEnd;
      return FALSE;
   }
#endif   
   if(( dic_remote_version >= 4 )
      &&( DicRemoteFlag & ENCRYPT_SPEECH_FLAG ))
   {
      U8 codec;
      if( DicRemoteEncryptKey > 0x07 )
      {
         dicremote_g_state = DICREMOTE_State_VoiceEnd;
         return FALSE;
      }
      codec = ap4_audio_var.encrypt[ DicRemoteEncryptKey ];
      MSB0( Key_encrypt_mp3 ) = codec;
      MSB1( Key_encrypt_mp3 ) = codec;
      MSB2( Key_encrypt_mp3 ) = codec;
      MSB3( Key_encrypt_mp3 ) = codec;
      //LROM_EXCHANGE = 0x00;
      //wav_encrypt = TRUE;
   }
   else
   {
      //LROM_SCRAMBLE = 0x00;
      Key_encrypt_mp3 = 0x00;
      //LROM_EXCHANGE = 0x00;
      //wav_encrypt = FALSE;
   }
   ap4_audio_var.audio_var.file_size_count += dicremote_g_base_addr;
   if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
   {
      //return FALSE;
      dicremote_g_state = DICREMOTE_State_VoiceEnd;
      return FALSE;
   }
   return TRUE;
}


#pragma CODE = TEXT_REMOTE_TEMP_CODE02

void dic_remote_init_deal( void )
{
   if( !file_seek( dicremote_g_base_addr, FS_SEEK_SET ))
   {
      dicremote_g_state = DICREMOTE_State_Error;
      return;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dicremote_g_header, sizeof( dicremote_g_header ))!= sizeof( dicremote_g_header ))
   {
      dicremote_g_state = DICREMOTE_State_Error;
      return;
   }
   dicremote_g_state = DICREMOTE_State_WaitInput;
}


#endif   //#if( DIC == ENABLE )
#endif   //_text_remote_c_
