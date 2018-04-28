//! @file player_ap3.c,v
//!
//! Copyright (c) 2005 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player ap3.
//!
//! @version 1.2 snd3b-dvk-1_0_33 $Id: player_ap3.c,v 1.2 2005/10/19 15:09:19 eaudreze Exp $
//!
//! @todo
//! @bug
//!

#pragma DATA = PLAYER_AP3_DATA
#pragma CODE = PLAYER_AP3_CODE

#ifndef _player_ap3_c_
#define _player_ap3_c_


//_____  I N C L U D E S ___________________________________________________

#include "compiler.h"
#include "search.h"
//nclude "player.h"
#include "player_ap3.h"

// Include Audio module definition
//#include "audio.h"
// Include the file system access
//nclude "navigation.h"
#include "file.h"
#include "fat.h"
// Include for hardware access
//#include "ctrl_status.h"
//#include "ctrl_access.h"

//#include "file_aux.h"

#include "conf_sio.h"
#include "conf_oid.h"
//#include "apf.h"

#include "search_game.h"
#include "string.h"
//#include "paster_base.h"
//#include "com_var.h"
//#include "cue.h"

#include <stdlib.h> // Contains Funktion "rand()"
#include "string.h"
//#include "mail_evt.h"
//#include "srv_player.h"
//#include "mail.h"
//#include "power.h"             // Power module definition
//#include "mmi_player.h"
//#include "timer_drv.h"
#include "system.h"
#include "search_task.h"
#include "com.h"
#include "search_remote.h"
#include "player_task.h"
#include "MP3_api.h"    // MP3 ApplicationLayer header file

//_____ M A C R O S ________________________________________________________
void vol_inc(void);

//_____ D E F I N I T I O N ________________________________________________

//extern void SpeakDetect(void);
extern void judge_op(void);
//extern void swap_code(U16 flag);
extern  Bool                  g_b_play_ap3;
extern FCFILE __X *	fat_g_cur_file;
extern  _MEM_TYPE_SLOW_ Bool                  g_b_player_on;

extern _MEM_TYPE_SLOW_ U8 flag_ap4_play_pause;
extern _MEM_TYPE_SLOW_   Bool      flag_load_mp3_dac;

//extern  _MEM_TYPE_SLOW_  U32 					Key_encrypt_mp3;
//extern   _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;
//extern   _MEM_TYPE_SLOW_  U32				   mp3_over_clock;
//_____ D E C L A R A T I O N ______________________________________________
//extern xdata U8  g_b_player_on;
extern  void  player_stop_stream    ( void );
//extern  void  player_codec_stop     ( void );
extern  void  player_volume_off     ( void );
extern void DAC_Stereo_to_Mono(unsigned Option);
//extern Bool flag_wav_play;
//extern unsigned int vol_idx;
//extern const int VolTbl[VOL_NUM];

#if( AP3 == ENABLE )

#endif  //#if( AP3 == ENABLE )


#if( PASTER == ENABLE )
/*
Bool player_apf_start( struct APF_CUE_INFO _MEM_TYPE_SLOW_ *cue_info, U8 codec )
{
   if(player_ap3_is_busy())
      return FALSE;
   if( !file_seek( cue_info->offset, FS_SEEK_SET ))
      return FALSE;
   file_size_count = cue_info->offset;
   play_file_size = cue_info->size;
   switch( codec )
   {
      case 0:
//      varAl.WAVE_Flg = 0;
      break;

      case 2:
 //     varAl.WAVE_Flg = 1;
      break;

      default:
      return FALSE;
   }
   play_file_size += file_size_count;
   return player_music_start();
}
*/
#endif  //#if( PASTER == ENABLE )


#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
//#if( PASTER == ENABLE )
#if 0
Bool paster_back_to_top_dir( void )
{
   if( g_paster_dir_index.u8_lun == 0xFF )
   {
      if( !nav_dir_root())
      {
         return FALSE;
      }
      unicode_init_buffer_enc( paster_unicode_buffer(), G_STRING_SIZE2, ENCODING_UTF16BE );
      string_ascii_to_unicode( paster_unicode_buffer(), g_paster_dir );
      if( !nav_filelist_findname( string_unicode( paster_unicode_buffer()), FALSE ))
      {
         if( !nav_dir_make( string_unicode( paster_unicode_buffer())))
         {
            if( FS_ERR_FILE_EXIST != fs_g_status )
               return FALSE;
         }
      }
      if( !nav_dir_cd())
      {
         return FALSE;
      }
      g_paster_dir_index = nav_getindex();
   }
   else
   {
      if( !nav_gotoindex( &g_paster_dir_index ))
      {
         return  FALSE;
      }
   }
   return TRUE;
}

#endif

#if 0
Bool cue_play( U8 index )
{
   U8 nav_id_saved;
   Bool result = FALSE;
   if( player_ap3_is_busy())
   {
      return FALSE;
   }
   nav_id_saved = nav_get();
   unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
   Unicode_prepare_buffer_for_mailing( g_string2, Unicode_get_char_space( g_string2 ));
   nav_select( FS_NAV_ID_CUE );
   //nav_drive_set( LUN_ID_NF_DISKMASS );
   if( !nav_drive_set( FILE_AUX_DRIVE ))
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !nav_dir_root())
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
//   paster_back_to_top_dir();
    unicode_init_buffer_enc( paster_unicode_buffer(), G_STRING_SIZE2, ENCODING_UTF16BE );
    string_ascii_to_unicode( paster_unicode_buffer(), g_paster_dir );
    if( !nav_filelist_findname( string_unicode( paster_unicode_buffer()), FALSE ))
      {
         if( !nav_dir_make( string_unicode( paster_unicode_buffer())))
         {
            if( FS_ERR_FILE_EXIST != fs_g_status )
            	{
            	     nav_select( nav_id_saved );
              		 return FALSE;
            	}
         }
      }
      if( !nav_dir_cd())
      {
         nav_select( nav_id_saved );
         return FALSE;
      } 
//   paster_back_to_top_dir();
   nav_filelist_reset();
#if 1
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( APF_DEFAULT_FILE_NAME ))
         {
            result = TRUE;
            break;
         }
      }
   }
   if( !result || !file_open( FOPEN_MODE_R ))
#else
   nav_string_unicode();
   tx_hex_return();
   if( !nav_filelist_findname( Unicode_get_addr_string(g_string1), FALSE ))
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !file_open( FOPEN_MODE_R ))
#endif
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !file_seek( APF_OFFSET_CUE_INDEX_TABLE + ( U32 )index * sizeof( g_cue_info ), FS_SEEK_SET ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_cue_info, sizeof( g_cue_info ))!= sizeof( g_cue_info ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if(( g_cue_info.offset == 0 )||( g_cue_info.size < 1024 ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   /*
   if( !player_apf_start( &g_cue_info, CODEC_MP3 ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   */
   nav_select( nav_id_saved );
   return TRUE;
}


void cue_off( void )
{
   U8 nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_CUE );
   file_close();
   nav_select( nav_id_saved );
}

#endif

#if 0
Bool player_file_start( U32 start_addr, U8 codec )
{
   if( player_ap3_is_busy())
   {
      return FALSE;
   }
   if( !file_seek( start_addr, FS_SEEK_SET ))
   {
      return FALSE;
   }

   
   file_size_count = start_addr;
   play_file_size = file_size_count + file_getsize();
   return player_music_start();
}
#endif
//void player_volume_on( void );


void AudioMP3FileOpenFirst(char *file)
{
	FS_ERR fs_err;
	if(OID_PLAY){
			if (g_b_play_afx){
				AudioMP3Handle.FSHandle = ADPCM_File;
			}
		    else{
				AudioMP3Handle.FSHandle = fat_g_cur_file;
			}		
			AudioMP3Offset = mp3_begin_add;
			AudioMP3FileLen = play_file_size;
		
		}
	else{
		AudioMP3Handle.FSHandle = FSOpen( file, FS_MODE_READ, &fs_err );
		AudioMP3Offset  = 0;
		AudioMP3FileLen = AudioMP3Handle.FSHandle->size;
		}
}
MP3API_STATUS AudioMP3FileCloseAfter( void )
{
   	if(OID_PLAY) return MP3API_NO_ERR;	
	if( FSClose(AudioMP3Handle.FSHandle) != MP3_RUN_NRR )
		return MP3_RUN_ERR;
	return MP3API_NO_ERR;
}


Bool player_music_start( void )
{
   tAudioErr   Audio_err; 
   OID_PLAY = TRUE;
//***********************************************
	AudioMP3FileOpenFirst(0);
   Audio_err = AudioMP3Init1k0(0);
    if(Audio_err != AUDIO_ERROR_OK){
 		SendString("\r\n314");		
		goto play_fail;
    	}

//   g_b_player_on = TRUE;
   
#if 0
		DAC_HeadPhone_Volume(0);
		judge_op();
		vol_inc();
//#else	
//	 flag_wav_play = FALSE;
	if(!flag_load_mp3_dac){
   		AudioMP3SetEQ(EQ_NOMAL);
//		SpeakDetect();
//		OP_CLOSE();
		DAC_Disable_Speaker();
		DAC_Enable_HeadPhone();
		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
		flag_load_mp3_dac = TRUE;
		}
    else{
		
		DAC_HeadPhone_Volume(0);
		judge_op();
		vol_inc();
    	}
#endif
	vol_idx_bak = VOL_NUM;
	flag_wav_play = FALSE;
	SpeakDetect(TRUE);
	//DAC_Stereo_to_Mono(1);
  // Audio_err = AudioMP3OpenOIDFile(mp3_begin_add,play_file_size,Key_encrypt_mp3);
    Audio_err = AudioMP3Open((char *)"0");
      //Audio_err =  AudioMP3FileOpen( (char *)0 );
   if( Audio_err == AUDIO_ERROR_OK)
   {    
//      Audio_err = AudioMP3SetVol(VolTbl[vol_idx]);  
//        if(Audio_err != AUDIO_ERROR_OK)
//            SendString("The MP3 Volume initial fail."); 
		g_b_player_on = TRUE;
		//DAC_FadeIn();
		clkup_time(FALSE);
         Audio_err = AudioMP3Play();
       if(Audio_err != AUDIO_ERROR_OK)
       	{
 //      		g_b_player_on = FALSE;
 		 	SendString("\r\n358");
    		goto play_fail;
       	}
	   
//	  	player_volume_on();
      	power_forbid_autopoweroff();
		
		
//	    SendString("\r\nplay ok!");
//      GameTimeOverTick = 0;
//      GameTipTick = 0;
//	   while(1){};
//      srvplayer_g_player_state = MMI_PLAYER_STATE_PLAY;
      	op_stop_timer();
		//clkup_time(FALSE);
      return TRUE;
   }
play_fail:   
 	SendString("\r\nplay fail!\r\n");
	op_start_timer();
	b_vol_play	= FALSE;
//   player_volume_off();
//   Audio_err = AudioMP3Uninit();
//    if(Audio_err != AUDIO_ERROR_OK)
//        SendString(( char *)"The ap4 play uninitial fail.");
   
   return FALSE;
//***********************************************
}

void interrupt_play(void)
{
	if(player_ap3_is_busy()||(b_play_pause)){
		SDKLoadCode(DAC_COMMON_CODE);
		SDKLoadCode(MP3_COMMON_CODE);
		player_ap3_stop();
		init_array_v();
		}
}

void player_ap3_stop( void )
{
  tAudioErr   Audio_err;
//   player_volume_off();
//   player_stop_stream();
 //  player_codec_stop();
   g_b_player_on = FALSE;
	b_vol_play	= FALSE;
	b_play_pause = FALSE;
//   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;

   power_authorize_autopoweroff();
//   GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
//   GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
//   bSpeechEnd = TRUE;
//   player_task_stop();
   //speech_status = SPEECH_IDLE;
//   	vol_dec();
   	// DAC_FadeOut();
//     OP_CLOSE();
     AudioMP3Stop();
   Audio_err = AudioMP3Uninit();
   	 Audio_err = AudioMP3Close();
	clkup_time(TRUE);
   op_start_timer();
//   swap_code(FILE_CODE_FLAG);
//    if(Audio_err != AUDIO_ERROR_OK)
//        SendString(( char *)"The ap4 play uninitial fail.");
}

#if 0
void player_ap3_pause( void )
{
   g_b_player_on = FALSE;
   power_authorize_autopoweroff();
   AudioMP3Pause();
}


void player_ap3_continue( void )
{
     g_b_player_on = TRUE;
     power_authorize_autopoweroff();
     AudioMP3Continue();
}
#endif

Bool player_ap3_is_busy( void ) 
{
   //return g_b_play_ap3 && g_b_player_on;
   return ( g_b_player_on );
}
#endif  //#if(( PASTER == ENABLE )&&( AP3 == ENABLE ))


#endif  //_player_ap3_c_
