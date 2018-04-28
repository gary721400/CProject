//!
//! @file srv_player.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the services for the application PLAYER
//!
//! @version 2.6 snd3-dvk-1_9_5 $Id: srv_player.c,v 2.6 2007/07/26 15:51:33 sguyon Exp $
//!
//! @todo
//! @bug
//!

#pragma DATA = SRV_PLAYER_DATA
#pragma CODE = SRV_PLAYER_CODE

#ifndef _srv_player_c_
#define _srv_player_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf_audio.h"

#include <stdio.h>

#include "mail_evt.h"
#include "mail_cmd.h"

#include "com_appli.h"
#include "com_evt.h"
#include "com_var.h"
#include "srv_player.h"

#include "mmgr_kernel.h"
//#include "ctrl_access.h"
#include "player_task.h"
#include "player.h"
#include "file.h"           // for file management 
#include "power.h"             // Power module definition
#include "mmi_player.h"

#include "com.h"

//________________ D E C L A R A T I O N S ___________________


//   _MEM_TYPE_SLOW_   Srv_player_states    srvplayer_g_player_state;
//   _MEM_TYPE_SLOW_   St_player_arg        srvplayer_g_arg;
//_____ D E F I N I T I O N S ______________________________________________





//_____ D E F I N I T I O N S ______________________________________________


//! Switch the player on
//!
//Bool srvplayer_switch_on( void )
//{
//   srvplayer_g_player_state      = MMI_PLAYER_STATE_STOP;
//   return player_on();
//}


//! Switch the player off
//!
//void srvplayer_switch_off( void )
//{
//   player_off();
//   srvplayer_g_player_state      = MMI_PLAYER_STATE_STOP;
//} 


#if 0
//! Get file informations
//!
void srvplayer_file_getinfos( void )
{
   // Get folder name
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_dir_name( g_string2, G_STRING_SIZE2 );
#else
   unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
   nav_dir_name( Unicode_get_addr_string( g_string2 ), Unicode_get_char_space( g_string2 ) );
#endif

   // Get name of file
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_file_name( g_string, G_STRING_SIZE, FS_NAME_GET, FALSE );
#else
   unicode_init_buffer_enc( g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
#endif
/*
   // Get ID3 informations
   player_id3_read( &srvplayer_id3 );
   // Reset others information avialable only in play
   srvplayer_g_codec_type  = CODEC_PLAY_MAX;
   srvplayer_stream_channel_type    = 0;
   srvplayer_stream_sampling_freq   = 0;
   srvplayer_stream_bitrate         = 0;
   */
}
#endif

#if 0

extern	char	__X	CurrentShortFilePath[512];

//! Play the current selected file
//!
//! @return SRVPLAYER_PLAY_OK , SRVPLAYER_PLAY_FAIL, SRVPLAYER_PLAY_NOCODEC, SRVPLAYER_PLAY_NOSUPPORTED
//!
U8 srvplayer_play( Bool b_resume_time )
{
	tAudioErr   Audio_err;
	char *	shortFileName = CurrentShortFilePath;
	
   if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
      return SRVPLAYER_PLAY_OK;   // Already in play mode
      
	Audio_err = AudioMP3Open((signed char *)shortFileName);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
            	SendString((unsigned char *)"The MP3 file can not open.");
				return SRVPLAYER_PLAY_FAIL;
        	}
   // Init player sound and speed
   srvplayer_volume_send();


   if( !player_play_file( 0 ))
   {
      if( PLAYER_ERR_NO_CODEC_FOUND == player_g_status )
         return SRVPLAYER_PLAY_NOCODEC;
      return SRVPLAYER_PLAY_FAIL;
   }
   power_forbid_autopoweroff();

   srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
   player_task_start();       // Enable event of player task
   return SRVPLAYER_PLAY_OK;
}
#endif


#if 0
//! Stop play of the current selected file
//!
void srvplayer_stop( void )
{
   if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
      return;   // Already in stop mode

   player_set_volume( 0 );
   player_off();
   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   player_task_stop();     // Disable event of player task
}
#endif

#if 0
//! Modify the sound level
//!
//! @param   U8 : the request in the volume level : SRVPLAYER_VOL_UP or SRVPLAYER_VOL_DOWN
//!
void  srvplayer_volume_change( Bool change )
{
//   SendString("srvplayer_g_arg.volume=");
//   send_hex_u8(srvplayer_g_arg.volume);
//   SendString("\r\n");
#if 1
   if( SRVPLAYER_VOL_UP == change )
   {

      if( srvplayer_g_arg.volume < (SRVPLAYER_VOL_LEVEL-1))
      { 
      	srvplayer_g_arg.volume++; 
	  }
   }
   else
   {
      if( srvplayer_g_arg.volume > 0 )
      {
      	srvplayer_g_arg.volume--; 
	  }
   	}
   srvplayer_volume_send();
//   SendString("srvplayer_g_arg.volume=");
//   send_hex_u8(srvplayer_g_arg.volume);
//   SendString("\r\n");
#else
   if( SRVPLAYER_VOL_UP == change )
   {
     if( srvplayer_g_arg.volume < (SRVPLAYER_VOL_LEVEL-1))
      { 
      	srvplayer_g_arg.volume++; 
		srvplayer_g_arg.volume++; 
	  }
      else
      {
         g_b_volume_change = FALSE;
      }
   	}
   else
   {
      if( srvplayer_g_arg.volume > 7 )
      {
      	srvplayer_g_arg.volume--;
		srvplayer_g_arg.volume--;
	  }
      else
      {
         g_b_volume_change = TRUE;
 
      }  
   }
   
   if(srvplayer_g_arg.volume==7)
   	{
   	op_disable();
   	}
   else 
   	{
   	op_enable();
   	}
   srvplayer_volume_send();
#endif 	
   
}

#endif

//! Send the volume at the player
//!
//void  srvplayer_volume_send( void )
//{
//   player_set_volume( srvplayer_g_arg.volume );
//}

#if 0
//! Pause function
Bool srvplayer_pause( void )

{
   if(MMI_PLAYER_STATE_PLAY  != srvplayer_g_player_state)
      return FALSE;
   
   if( !player_pause())
      return FALSE;
   
   srvplayer_g_player_state = MMI_PLAYER_STATE_PAUSE;
   return TRUE;
}


//!  Stop FFW / FRW / PAUSE and restart play
//!
Bool srvplayer_restart_play( void )
{
   if( (MMI_PLAYER_STATE_PAUSE      != srvplayer_g_player_state)
   &&  (MMI_PLAYER_STATE_PLAY_FFW   != srvplayer_g_player_state)
   &&  (MMI_PLAYER_STATE_PLAY_FRW   != srvplayer_g_player_state) )
      return FALSE;
   
   // Restart play after a pause, FWW, FRW.
   if( !player_restart_play())
      return FALSE;

   srvplayer_g_player_state = MMI_PLAYER_STATE_PLAY;
   return TRUE;
}
#endif

#endif // _srv_player_c_

