//!
//! @file player_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: player_task.c,v 2.1 2007/06/25 09:20:11 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _player_task_c_
#define _player_task_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_mmi.h"
//#include "conf\conf_explorer.h"
#include "modules\power\power.h"             // Power module definition
#include "modules\player\player_task.h"

// Include for mail box
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "drivers\oid\oid.h"
#include "modules/text/text_viewer.h"
#include "mmi\dictionary\mmi_dictionary.h"

// Include player module
#include "player.h"
//#include "modules\control_access\ctrl_access.h"
#include "string.h"
#include "stdio.h"
//#include "modules\file_system\explorer.h"
//#include "mmi\player\mmi_player.h"
//#include "SSSMP3_regs.h"
#include "modules\file_system\file.h"           // for file management 
#include "sio\com.h"
//#include "lib_mcu\mmc\mmc_drv.h"
#include "lib_mcu\kbd\keypad_drv.h"
#include "mmi\kbd\keypad.h"
#include "conf\conf_oid.h"
//#include "modules\search\search.h"
#include "modules\search\search_remote.h"
//#include "modules\search\search_task.h"
#include "modules\power\power_task.h"
//#include "modules\search\search_game.h"
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
//#include "mmi\ap3\mmi_ap3.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "mmi\player\srv_player.h"
#include "mmi\shared\com_var.h"
#include "mp3_include.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#pragma DATA = PLAYER_TASK_DATA
#pragma CODE = PLAYER_TASK_CODE

//_____ M A C R O S ________________________________________________________
ImageDeclareType( READBNL_CODE );
ImageDeclareType( SOME_INDEX_CODE );
//-------------------------------------------------------------------------
//extern   xdata    AL_Variables     	varAl;
U16 read_bnl_file(U16 off_t);
Bool some_index_dowith( void );

//_____ D E F I N I T I O N ________________________________________________
Bool  CheckWAVFrameHeader  ( void );
void  encrypt_wav_header   ( void );

extern void player_volume_off( void );

void  remote_mp3_task      ( void );

Bool g_b_player_on         /*= FALSE*/;

extern   unsigned long  Frame_Count;

#pragma CODE = TASK_INIT_CODE

//! Initialization of player task
//!
void player_task_init(void)
{
   extern Bool g_b_play_ap3;

   mail_send_event( EVT_UPDATE_START, UPDATE_STOPPED );

   g_b_volume_on = FALSE;
   p_g_Id_stream = ID_STREAM_ERR;
   g_b_player_on = FALSE;
   g_b_play_ap3 = FALSE;
   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
}


#pragma CODE = PLAYER_TASK_CODE

//! Start player task
//!
void player_task_start(void)
{
   g_b_player_on = TRUE;
}


//! Stop player task
//!
void player_task_stop(void)
{
   g_b_player_on = FALSE;
}

//! This fonction manage the player commands
//!
void player_task(void)
{
   tMusicStatus play_status;
   
   if( g_b_player_on )
   {
      if( Power_is_poweroff_activated() )
         return;

      if( DAC_CCMC_Status != 0 )    // check headphone short-circuit status
      {
         DAC_Headphone_Reset();
      }

      if( audio_is_wav == 0 )
      {
         play_status = AudioMP3Status();
      }
      else
      {
         if( Frame_Count > 0 )
         {
            play_status = PLAYING_STATUS;
         }
         else
         {
            play_status = EOF_STATUS;
         }
      }
      /*switch( play_status )
      {
         case PAUSE_STATUS:
         case AB_REPEAT_STATUS:
         case A_STAGE_STATUS:
         case FWRWD_PROLOG_STATUS:
         case FORWARD_STATUS:
         case REWIND_STATUS:
         case FWRWD_EPILOG_STATUS:
         play_status = PLAYING_STATUS;
         case PLAYING_STATUS:
         break;

         default:
         play_status = EOF_STATUS;
         break;
      }*/
      if(( play_status != PLAYING_STATUS )
         &&( play_status != PAUSE_STATUS ))
      {
         player_volume_off();
         player_off();
         g_b_player_on = FALSE;
         srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         mail_send_event( EVT_PLAYER, PLAYER_EOF );
         //SendString((U8 *)"music end\r\n");
         //power_authorize_autopoweroff();
      }
   }
//***********************************************
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   //if( g_b_remote_on /*&&( g_select_appli != AP3_APPLI )*/)
   {
      remote_mp3_task();
   }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
//***********************************************
}


#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
//
void remote_mp3_task( void )
{
   _MEM_TYPE_SLOW_ U8 i;
   if( Search_Record.news_sys.flag_index )
   {
      Search_Record.news_sys.flag_index = 0;
	   SendString((U8 *)"\r\nindex=");send_hex_u16(Current_Index);
      if( !Current_Index )
         return;
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
      if( g_b_lcd_backlight_off )
      {
         Lcd_rearm_backlight_on();
		 g_mmi_lcd_backlight_off = FALSE;
         return;
      }
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )	 	  
///*
		SDKLoadCode(READBNL_CODE);
      if(0xFFFF != read_bnl_file(Current_Index)){
	  	g_b_xuanshu_flag = TRUE;
		exit_dictext_to_appli = IDLE_APPLI;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
		return;
      	}
//*/	

	  SDKLoadCode(SOME_INDEX_CODE);
	  if(some_index_dowith()){
	  	g_oid = FALSE;
		return;
	  	}

      if( !g_b_remote_on )
         return;
      
      //bRemotePress = FALSE;
      for( i = 0; i < REMOTE_INDEX_MAX; i++ )
      {
         if( Current_Index == remote_index_tab[ i ])
         {
            
			g_oid = FALSE;
            mail_send_event( EVT_REMOTE, i );
            break;
         }
      }
   }
}
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#pragma CODE = READBNL_CODE

U16 read_bnl_file(U16 off_t)
{
	U16 offset,bid = 0;
	U32 temp_off;
	//char tmpbuf[32];
	char rbuf[4];
	  __X FCFILE *REC_File;
	__X FS_ERR err;
 
   U16 temp_min,temp_max,book_off;
#if 0
   if(g_setting_list.struct_setting.bluetooth){
   	temp_min = DPR3_XUANSHU_MIN;
	temp_max = DPR3_XUANSHU_MAX;
   	}
   else{
   
	temp_min = OID2_XUANSHU_MIN;
	temp_max = OID2_XUANSHU_MAX;
   	}
#else
#if 0// (REREAD_MODE == ENABLE)   
    if(Current_Index == NORMAL_MODE_INDEX){
        return 0XFFFF;
        }
    if(Current_Index == REREAD_MODE_INDEX){
        return 0xFFFF;
        }
#endif    //(REREAD_MODE == ENABLE)
   if(g_setting_list.struct_setting.bluetooth){
   	temp_min = OID2_XUANSHU_MIN;
	temp_max = OID2_XUANSHU_MAX;
	book_off = OID2_XUANSHU_OFFSET;
   	}
   else{
   	temp_min = DPR3_XUANSHU_MIN;
	temp_max = DPR3_XUANSHU_MAX;
	book_off = DPR3_XUANSHU_OFFSET;
   	}
#endif   
	//temp_min = 59001;
	//temp_max = 61250;
    if((off_t < temp_min)||(off_t > temp_max)){
#if 0//def TX_DEBUG
	SendString(( U8 *)"\n\rout range!");
#endif		
		return 0XFFFF;
    	}
/*	
	off_t -= book_off;
	REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_READ_WRITE,&err);
	if(err != FS_NO_ERROR){
#if 0//def TX_DEBUG
	SendString(( U8 *)"\n\ropen fail!");
#endif			
		return 0XFFFF;
		}
	temp_off = (U32)(off_t<<1);
	FSSeek(REC_File, temp_off);
	FSRead(REC_File, rbuf, sizeof(rbuf), &err);
	//FSClose(Temp_File);
	MSB(bid) = rbuf[0x00];
	LSB(bid) = rbuf[0x01];
	FSClose(REC_File);
#ifdef TX_DEBUG
	SendString(( U8 *)"\n\rid=");
	send_hex_u16(bid);
#endif	
*/
    g_xuanshu_num = off_t;
	return off_t;
}

#pragma CODE = SOME_INDEX_CODE

Bool some_index_dowith( void )
{
   U16 dic_temp,mp3_temp,ap4_temp,v_adc,v_dec,v_min,v_max,diy_min,diy_max;
	if(g_setting_list.struct_setting.bluetooth){
		//dic_temp = OID2_GO_DIC;
		mp3_temp = OID2_GO_MP3;
		ap4_temp = OID2_GO_AP4;
		v_adc    = OID2_V_ADC_INDEX;
		v_dec    = OID2_V_DEC_INDEX;
		v_min	 = OID2_V_1_INDEX;
		v_max    = OID2_V_7_INDEX;

		diy_min	 = OID2_DIY_RECORD_MIN;
		diy_max  = OID2_DIY_RECORD_MAX;
		}
	else{
		//dic_temp = DPR3_GO_DIC;
		mp3_temp = DPR3_GO_MP3;
		ap4_temp = DPR3_GO_AP4;
		v_adc    = DPR3_V_ADC_INDEX;
		v_dec    = DPR3_V_DEC_INDEX;
		v_min	 = DPR3_V_1_INDEX;
		v_max    = DPR3_V_7_INDEX;

		diy_min	 = DPR3_DIY_RECORD_MIN;
		diy_max  = DPR3_DIY_PLAY_MAX;
		}
	/*
	  if(Current_Index == dic_temp){
      	g_select_appli = g_old_select_appli = DIC_APPLI;//DIC_APPLI;
      	Mmgr_kill_this_mmi_appli();
		CommonLoadCode(C_DIC);
		return TRUE;
	  	}
	  */
#if (DIY_REC == ENABLE)	
	 if((Current_Index >= diy_min)&&(Current_Index <= diy_max)){
	  	reread_num = Current_Index;
	 	exit_dictext_to_appli = REREAD_APPLI;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
		
      	//Mmgr_kill_this_mmi_appli();
		//CommonLoadCode(C_PLAYER);
		return TRUE;
		}
#endif	  
	   if(Current_Index == mp3_temp){
      	//g_select_appli = g_old_select_appli = PLAYER_APPLI;//MP3_APPLI;
      	exit_dictext_to_appli = PLAYER_APPLI;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
      	//Mmgr_kill_this_mmi_appli();
		//CommonLoadCode(C_PLAYER);
		return TRUE;
	  	}
	    if(Current_Index == ap4_temp){
      	//g_select_appli = g_old_select_appli = AP3_APPLI;//AP3_APPLI;
      	exit_dictext_to_appli = AP3_APPLI;
		mail_send_event( EVT_GOTO_OID_MODE, 0 );
      	//Mmgr_kill_this_mmi_appli();
		//CommonLoadCode(C_AP4);
		return TRUE;
	  	}
		if(Current_Index == v_adc){
			//mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_UP<<8)|KBD_KEY_RELEASED));
			// mail_send_event( EVT_REMOTE,(Current_Index - v_min + REMOTE_VOL0) );
			//mail_send_event( EVT_REMOTE, REMOTE_VOL_UP);
			//srvplayer_volume_change( SRVPLAYER_VOL_UP);
			//mmi_dic_display_volume();
			//mail_send_event(EVT_DIC_VOL_PLAY,0);
		      if( g_volume < (VOL_LEVEL-1))
		      {
		         g_volume++;
		      }
			mail_send_event(EVT_DIC_VOL_CHANGE,1);
			return TRUE;
			}
		if(Current_Index == v_dec){
			//mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED));
			//mail_send_event( EVT_REMOTE, REMOTE_VOL_DOWN);
			//srvplayer_volume_change( SRVPLAYER_VOL_DOWN);
			//mmi_dic_display_volume();
			//mail_send_event(EVT_DIC_VOL_PLAY,0);

		      if( g_volume )
		      {
		         g_volume--;
		      }
			mail_send_event(EVT_DIC_VOL_CHANGE,1);
			return TRUE;
			}
		if((Current_Index >= v_min)&&(Current_Index <= v_max)){
			g_volume = Current_Index- v_min;
			//srvplayer_volume_send();
            //srvplayer_volume_send();
           // mmi_dic_display_volume();
            //mail_send_event( EVT_REMOTE,(Current_Index - v_min + REMOTE_VOL0) );
           // srvplayer_volume_change( SRVPLAYER_VOL_UP);
			//mmi_dic_display_volume();
			mail_send_event(EVT_DIC_VOL_CHANGE,0);
			return TRUE;
			}
		if(Current_Index == CHANGE_OID_TYPE){
			mail_send_event( EVT_CHANGE_OID_TYPE, 0 );
			return TRUE;
	    	}
	  return FALSE;
}


#if 0
Bool play_file_start( void )
{
   /*SendString("file_size_count:");
   send_hex_u32(ap4_audio_var.audio_var.file_size_count);
   SendString("\r\n");
   SendString("play_file_size:");
   send_hex_u32(ap4_audio_var.audio_var.play_file_size);
   SendString("\r\n");
   */
   
   //MP3_PrevInit();
   SSSCode_PrevInit();
   file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET );
   if( !varAl.WAVE_Flg )
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)0xC800, FS_512B*4 );
   else
   {
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)0xB800, FS_512B*4 );
      if( wav_encrypt )
      {
         encrypt_wav_header();
      }
   }
   ap4_audio_var.audio_var.file_size_count += FS_512B*4;
   
   file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET );
   if( !varAl.WAVE_Flg )
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)0xD000, FS_512B*4 );
   else
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)0xC000, FS_512B*4 );
   ap4_audio_var.audio_var.file_size_count += FS_512B*4;
   
   if( varAl.WAVE_Flg )
   {
      if( !CheckWAVFrameHeader())
         return FALSE;
      if( wav_encrypt )
      {
         file_seek( ap4_audio_var.audio_var.file_size_count - FS_512B*4*2, FS_SEEK_SET );
         file_read_buf(( U8 _MEM_TYPE_SLOW_ *)0xB800, FS_512B*4 );
      }
   }
   //MP3_ActiveFAT();
   SSS_CodecActive();
   //SendString("SSS_CodecActive\r\n");
   audio_on();
   power_forbid_autopoweroff();
   return TRUE;
}


Bool CheckWAVFrameHeader( void )
{
   idata unsigned long factSz = 0;   //'fact' chunk size
   /* Check if 'RIFF' and 'WAVE' */
   if((((( WAVE_CHUNK *)ADPCMBufL )->ChunkID ) != CHUNKID_RIFF )
      ||(((( WAVE_CHUNK *)ADPCMBufL )->Format) != WAVE_STRING ))
   {
      //SendString(" Not 'RIFF' 'WAVE'chunk =.=' \r\n");
      return 0;
   }

   /* Check if 'fmt' subchunk*/
   if(((( WAVE_CHUNK *)ADPCMBufL )->SubChunk1ID ) != CHUNKID_FMT )
   {
      //SendString(" Not 'fmt'chunk =.=' \r\n");
      return 0;
   }

   //Size of 'fmt' subchunk
   ENDIAN_ASSIGN_4( &FileData.fmtchukSz, &((( WAVE_CHUNK *)ADPCMBufL )->SubChunk1Size ));

   // Audio Format
   ENDIAN_ASSIGN_2( &( FileData.wfx.wFormatTag ), &((( WAVE_CHUNK *)ADPCMBufL )->wFormatTag ));
   ENDIAN_ASSIGN_2( &( FileData.wfx.wBitsPerSample ), &(((WAVE_CHUNK *)ADPCMBufL )->wBitsPerSample ));

   /* Check Support format */
   if((( FileData.wfx.wFormatTag == WAVE_FORMAT_PCM )&&( FileData.wfx.wBitsPerSample == _16BitsPerSample ))
      ||(( FileData.wfx.wFormatTag == WAVE_FORMAT_IMA_ADPCM )&&( FileData.wfx.wBitsPerSample == _4BitsPerSample )))
   {
      ;
   }
   else
   {  // Neither 16bits-PCM  nor 4bits-IMA-ADPCM
      /*SendString(" Not Support AudioFormat:");
      send_hex_u16( FileData.wfx.wFormatTag);
      SendString("\r\n");
      SendString(" Not Support BitsPerSample:");
      send_hex_u16( FileData.wfx.wBitsPerSample);
      SendString("\r\n");*/
      return 0;
   }

   //Sample rate
   ENDIAN_ASSIGN_4( &( FileData.wfx.nSamplesPerSec ), &((( WAVE_CHUNK *)ADPCMBufL )->nSamplesPerSec ));

   /*Checking if sampling freq. is among 48kHz,44.1kHz,32kHz,24kHz,22.05kHz,16khz,12khz,11.025khz,8khz*/
   if(( FileData.wfx.nSamplesPerSec == 0x5622 )//||( FileData.wfx.nSamplesPerSec == 0xAC44 )||( FileData.wfx.nSamplesPerSec == 0xBB80 )
      ||( FileData.wfx.nSamplesPerSec == 0x5DC0 )||( FileData.wfx.nSamplesPerSec == 0x3E80 )||( FileData.wfx.nSamplesPerSec == 0x7D00 )
         ||( FileData.wfx.nSamplesPerSec == 0x1F40 )||( FileData.wfx.nSamplesPerSec == 0x2B11 )||( FileData.wfx.nSamplesPerSec == 0x2EE0 ))
   {
      ;
   }
   else
   {
      /*SendString(" Not Support SampleFreq:");
      send_hex_u32(FileData.wfx.nSamplesPerSec);
      SendString("\r\n");*/
      return 0;
   }

   //Number of channels
   ENDIAN_ASSIGN_2( &FileData.wfx.nChannels, &((( WAVE_CHUNK *)ADPCMBufL )->nChannels ));
   //Byte Rate
   ENDIAN_ASSIGN_4( &FileData.wfx.nAvgBytesPerSec, &((( WAVE_CHUNK *)ADPCMBufL )->nAvgBytesPerSec ));
   //Block Align
   ENDIAN_ASSIGN_2( &FileData.wfx.nBlockAlign, &((( WAVE_CHUNK *)ADPCMBufL )->nBlockAlign ));
   //Total playback time
   FileData.TotalTimeSec = ( ap4_audio_var.audio_var.play_file_size / FileData.wfx.nAvgBytesPerSec );

   //Searching 'data' chunk
   FileData.DataChukoffsetBytes = FileData.fmtchukSz + 20;  //offset to 'fmt' subchunk
   FileData.pChar = ( UCHAR volatile xdata *)ADPCMBufL;
   FileData.pLong = ( ULONG volatile xdata *)&FileData.pChar[ FileData.DataChukoffsetBytes ];

   if( FileData.wfx.wFormatTag == WAVE_FORMAT_IMA_ADPCM )
   {  //'fact' chunk
      //ENDIAN_ASSIGN_2( &FileData.wfx.wSamplesPerBlock, &((( WAVE_CHUNK *)sbuf1)->cbSize ));
      //'fact' chunk
      if( FileData.pLong[ 0 ] == CHUNKID_FACT )
      {
         ENDIAN_ASSIGN_4( &factSz, &( FileData.pLong[ 1 ]));   //Decode subchunck size
         FileData.DataChukoffsetBytes += ( factSz + 8 );       //Offset to 'data' subchunk
         FileData.pLong = ( ULONG volatile xdata *)&FileData.pChar[ FileData.DataChukoffsetBytes ];
      }
      else
      {
         //SendString(" Not 'fact' chunk =.=' \r\n");
         return 0;
      }
   }

   if( FileData.pLong[ 0 ] == CHUNKID_DATA )
   {  //Get 'data' subckunk
      ENDIAN_ASSIGN_4( &FileData.AudioDataSz, &( FileData.pLong[ 1 ])); //Decode subchunck size
      FileData.DataChukoffsetBytes += 8;  //start of Audio data
   }
   else
   {
      //SendString(" Not 'data' chunk =.=' \r\n");
      return 0;
   }
   return 1;
}


void encrypt_wav_header( void )
{
   U8 _MEM_TYPE_SLOW_ *p =( U8 _MEM_TYPE_SLOW_ *)0xB800;
   U16 _MEM_TYPE_SLOW_ length;
   U8 _MEM_TYPE_SLOW_ temp;
   for( length = 0; length < FS_512B*4; length++ )
   {
      temp = *p ^ LROM_SCRAMBLE;
      *p++ = temp;
   }
}
#endif
#endif   //_player_task_c_


