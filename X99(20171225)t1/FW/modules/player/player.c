//!
//! @file player.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 1.87 snd3-dvk-1_9_5 $Id: player.c,v 1.87 2007/10/15 09:59:28 sguyon Exp $
//!
//! @todo see a sequence record, SG !!see a optimization -> remove division
//! @bug
#ifndef _player_c_
#define _player_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_audio.h"                 // Audio user's configuration
#include "player.h"

// Include Audio module definition
#include "modules\audio\audio.h"
// Include the file system access
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"
//#include "modules\file_system\fat.h"
// Include for hardware access
//#include "modules\control_access\ctrl_status.h"
//#include "modules\control_access\ctrl_access.h"
// Include the timer macros
#include "lib_mcu\timer\timer_drv.h"
// Include a macro to disable the power off
#include "drivers\lcd\lcd_SSD1303.h"
#include "drivers\oid\oid.h"
//#include "modules\search\search.h"
//#include "modules\player_ap3\player_ap3.h"
//#include "modules\search\search_game.h"
//#include "mmi\player\srv_player.h"
//#include "SSSMP3_Regs.h"
//#include "mmi\player\mmi_player.h"
#include "modules\player\player_task.h"
#include "modules\power\power.h"             // Power module definition
#include "sio\com.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
//#include "sdhost.h"    // MP3 ApplicationLayer header file
#include "mp3_include.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "mmi\player\srv_player.h"

ImageDeclareType( DAC_COMMON_CODE );
ImageDeclareType( MP3_COMMON_CODE );

ImageDeclareType( PLAYER_WAV_CODE );
ImageDeclareType( READ_OID_SOUND_CODE );
ImageDeclareType( WAV_PALY_TIME_CODE );

#pragma DATA = PLAYER_DATA
#pragma CODE = PLAYER_CODE
//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

//----------------
// Codec Control (wm2 instead of wma to start with the codec 1 first when wma file to play)
//_MEM_TYPE_SLOW_ U8  s_player_extensions[6][4]={"mp3","wma","wav","mtv","mp2","wm2"};
// End Codec Control
//----------------


//----------------
// Sound control 
#define  PLAYER_FADEIN_TIME         200     // time of fade in after play    (unit ms)
#define  PLAYER_FADE_TIME_PAUSE     1000    // time of fade out before pause (unit ms)
#define  PLAYER_FADE_TIME_STOP      500     // time of fade out before stop  (unit ms)

#define  PLAYER_FFW1_TIME           4000    // max time in fast foward mode x1
#define  PLAYER_FFW2_TIME           4000    // max time in fast foward mode x2

//_MEM_TYPE_SLOW_ U8  g_u8_player_volume_r      = 0;       // volume right call by MMI
_MEM_TYPE_MEDSLOW_ U8  g_u8_player_volume_l      /*= 0*/;       // volume left  call by MMI
Bool                g_b_volume_on             /*= FALSE*/;   // state of volume
// End sound control 
//----------------


//----------------
// Streams Control
_MEM_TYPE_MEDSLOW_ U8  p_g_Id_stream         /*= ID_STREAM_ERR*/;  // ID of stream running, by default no stream
// End streams Control
//----------------


//***********************************************
//add by wangwei
Bool                 g_b_play_ap3 /*= 0*/;
//***********************************************
//extern   xdata    AL_Variables      varAl;

_MEM_TYPE_MEDSLOW_   ENCRYPT_DATA      ap4_audio_var;
//_MEM_TYPE_MEDSLOW_   Player_status     player_g_status;
_MEM_TYPE_MEDSLOW_   U32               Key_encrypt_mp3;
_MEM_TYPE_MEDSLOW_   U32               media_paly_all_time;
_MEM_TYPE_SLOW_      U8                mmipaster_play_id_timerdisplay;

//_____ D E C L A R A T I O N ______________________________________________

//! Codec fonctions
Bool player_codec_start          ( void );
void player_codec_stop           ( void );

//! Stream fonctions
Bool player_stream_is_on         ( void );
Bool player_stream_is_off        ( void );
Bool player_start_stream         ( void );
//void player_stop_stream          ( void );
Ctrl_status player_stream_ctrl   ( void );
void player_send_playmode        ( void );

//! Volume control
void player_volume_on            ( void );
void player_volume_off           ( void );

//! Timeout control
void player_timeout_set          ( U16 timeout );
Bool player_is_timeout           ( void );

extern void DAC_Stereo_to_Mono   ( unsigned Option );

//play wav
Bool WavPlay                     ( char *path );
void volume_on                   ( void );

#define  Default_PCM_Buffer_Size       505                
#define  FileBuffer_Size               Default_PCM_Buffer_Size    // word 
#define  READ_SIZE                     ( FileBuffer_Size * 2 )    // byte
#define  Default_ADPCM_Buffer_Size     (( Default_PCM_Buffer_Size - 1 )/ 4 + 2 )                     // word

void  FillBuffer  ( void );                                                  
void  FirstDecode ( void );
#if 0
__X   int   PCM_Buf_1[ Default_PCM_Buffer_Size ], PCM_Buf_2[ Default_PCM_Buffer_Size ];
__X   int   ADPCM_Buf[ Default_ADPCM_Buffer_Size ];
#endif
unsigned long  Frame_Count;
unsigned int   PCM_Buffer_Size, ADPCM_Buffer_Size, Sample_Rate;

__X   FCFILE   *ADPCM_File; 


#pragma CODE = SRV_PLAYER_TEMP_CODE00

//! Enable the player
//!
Bool player_on( void )
{
   // check if no stream is on going
   if( player_stream_is_on())
      return FALSE;

   player_volume_off();
   return TRUE;
}

/*
//! Power down the player
//!
Bool player_off( void )
{
   player_stop_stream();
   //player_codec_stop();
   return TRUE;
}
*/
#pragma CODE = WAV_PALY_TIME_CODE
void wav_all_play_time(char *path)
{
   __X FS_ERR fserr;
   tIMA_INFO ADPCM_Info;
   unsigned long read_length,all_length;
   U8 i,temp_1,temp_2;
   current_play_time = 0;
//    count_AB = 0;
   all_length = FSGetFileSize(path, &fserr);
   
   ADPCM_File = ( __X FCFILE *)FSOpen( path, FS_MODE_READ, &fserr );
   if( fserr )
   {
      FSClose(( __X FCFILE *)ADPCM_File );
      return;
   }
   
   fserr =  FSSeek(( __X FCFILE *)ADPCM_File, 24);
   read_length = FSRead(( __X FCFILE *)ADPCM_File, ( void *)&temp_1, 1, &fserr );
   fserr =  FSSeek(( __X FCFILE *)ADPCM_File, 25);
   read_length = FSRead(( __X FCFILE *)ADPCM_File, ( void *)&temp_2, 1, &fserr );
   size_psec_wav = temp_2<<8;
   size_psec_wav += temp_1;

	//size_psec_wav = size_psec_wav/256;
	size_psec_wav = size_psec_wav>>9;
   
   FSClose(( __X FCFILE *)ADPCM_File );
   //*ADPCM_File = 0;
  /*
   SendString((U8 *)"\r\nall_play_time:all_length:size_psec_wav=");
   send_hex_u32(all_play_time);
   SendString((U8 *)":");
   send_hex_u32(all_length);
   SendString((U8 *)":");
   send_hex_u16(size_psec_wav);
   //*/
   return ; 
   
 
}

#pragma CODE = PLAYER_CODE

//! Start a play file
//!
//! @param  u16_time_to_play define the time in the song to start to play 
//!         nav_id   ID of navigator to play
//!
Bool player_play_file( char *path )
{
   // check if no stream is on going
   if( player_stream_is_on() )
      return FALSE;
   
   //player_g_nav_id = nav_get();  // Record the navigator asked
   
   // check if a time is specified to start to play
   //if( 0 != u16_time_to_play) 
   //{ 
   //}

   //if( !player_codec_start() )
   //   return FALSE;
   SDKLoadCode( DAC_COMMON_CODE );
   if( audio_is_wav == 0 )
   {
   	  SDKLoadCode( MP3_COMMON_CODE );
      //SendString((U8 *)"mp3init\r\n");

      if( AudioMP3Init() != AUDIO_ERROR_OK )
         return FALSE;

      DAC_Stereo_to_Mono( 1 );   // 參數1是Stereo 混成mono 輸出，參數0 則是不混音直接輸出

//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
         //add by wangwei
      if( TRUE == g_b_play_ap3 )
      {
         SDKLoadCode( READ_OID_SOUND_CODE );
         //SendString((U8 *)"openoid\r\n");
         if( AudioMP3OpenOIDFile( ap4_audio_var.audio_var.file_size_count, ap4_audio_var.audio_var.play_file_size, Key_encrypt_mp3 ) != AUDIO_ERROR_OK )
            return FALSE;
      }
      else
#endif
//***********************************************
      {
         //SendString((U8 *)"mp3open\r\n");
         if( AudioMP3Open(( signed char *)path ) != AUDIO_ERROR_OK )
            return FALSE;
      }
      //SendString((U8 *)"EQ\r\n");
      AudioMP3SetEQ( EQ_NOMAL );
      //if( AudioMP3SetEQ( EQ_NOMAL ) != AUDIO_ERROR_OK )
      //   return FALSE;
      
   }
   else
   {
      //SendString((U8 *)"\r\n277");
      SDKLoadCode(WAV_PALY_TIME_CODE);
 	  wav_all_play_time(path);
      SDKLoadCode( PLAYER_WAV_CODE );
      if( !WavPlay( path ))
         return FALSE;
   }
   //SendString((U8 *)"\r\nplay_star");
   /*srvplayer_volume_send();
   //SendString((U8 *)"star\r\n");
   if( !player_start_stream() )
   {
      //player_codec_stop();
      return FALSE;
   }
   SendString((U8 *)"play ok\r\n");
   return TRUE;   // Play OK, but differed*/
   count_AB = 0;
   return player_start_stream();
}


#pragma CODE = SRV_PLAYER_TEMP_CODE04

//! Stop the player
//!
Bool player_stop( void )
{
   player_volume_off();
   //AudioMP3Stop();
   return TRUE;
}


#pragma CODE = SRV_PLAYER_TEMP_CODE06

//! Restart a play
//!
Bool player_restart_play( void )
{
   // check if no stream is on going
   if( player_stream_is_off() )
      return FALSE;
   
   //Wait_POP_CTL();
   //player_volume_on();
   volume_on();
   
   //SendString((U8 *)"resart\r\n");
   if( audio_is_wav == 0 )
      AudioMP3Continue();
   else
      DAC_Continue();
   return TRUE;
}


#pragma CODE = SRV_PLAYER_TEMP_CODE05

//! Set the player in pause mode
//!
Bool player_pause( void )
{
   player_volume_off();
   if( audio_is_wav == 0 )
      AudioMP3Pause();
   else
      DAC_Pause();
   //SendString((U8 *)"pause\r\n");
   return TRUE;
}


#pragma CODE = PLAYER_CODE

//! Set volume
//!
//! @param u16_volume    LSB = U8 right volume, MSB = U8 left volume
//!
Bool player_set_volume( U16 u16_volume )
{
   extern Bool g_b_player_on;

   // save value of volume 
   //g_u8_player_volume_r = LSB(u16_volume);
   g_u8_player_volume_l = ( U8 )u16_volume;
   if( g_b_volume_on && g_b_player_on )
   {
      // Set volume
      audio_set_vol( g_u8_player_volume_l /*, g_u8_player_volume_l*/ );
   }
   return TRUE;
}


#pragma CODE = DAC_COMMON_CODE

void volume_on( void )
{
   g_b_volume_on = TRUE;
#if( EARPHONE_DET == ENABLE )
   if( !g_b_earphone_present/* && g_u8_player_volume_l */)
   {
      op_enable();
      if( !u32_op_delay_off_time )
      {
         _MEM_TYPE_SLOW_ U32 timer = Timer_set_timer( TIMER_MS(300) );
         while (!Timer_is_timeout(timer));
      }
      u32_op_delay_off_time = 0;
   }
#endif   //#if( EARPHONE_DET == ENABLE )
}


//! Enable volume
//!
void player_volume_on( void )
{
   /*g_b_volume_on = TRUE;
#if( EARPHONE_DET == ENABLE )
   if( !g_b_earphone_present )
   {
      op_enable();
      if( !u32_op_delay_off_time )
      {
         _MEM_TYPE_SLOW_ U32 timer = Timer_set_timer( TIMER_MS(300) );
         while (!Timer_is_timeout(timer));
      }
      u32_op_delay_off_time = 0;
   }
#endif   //#if( EARPHONE_DET == ENABLE )
*/
   volume_on();

   DAC_FadeOut();
   //DAC_Disable_HeadPhone();

   //audio_set_vol( g_u8_player_volume_l /*, g_u8_player_volume_l*/ );

   if( !b_HeadPhone_on )
   {
      //b_HeadPhone_on = TRUE;
      DAC_Disable_Speaker();
      DAC_Enable_HeadPhone();
      if( g_b_earphone_present )
         DAC_Speaker_Volume( EARPHONE_SPEAKER_VOL );
      else
      {
         if( g_b_battery_low )
            DAC_Speaker_Volume( EARPHONE_SPEAKER_VOL );
         else
         {
            if( audio_is_wav == 0 )
               DAC_Speaker_Volume( MP3_SPEAKER_VOL );
            else
               DAC_Speaker_Volume( WAV_SPEAKER_VOL );
         }
      }

      audio_set_vol( g_u8_player_volume_l );
   }
   DAC_FadeIn();
}


#pragma CODE = PLAYER_CODE

//! Disable volume
//!
void player_volume_off( void )
{
   g_b_volume_on = FALSE;
   //audio_set_vol( 0 , 0 );
   //op_disable();
   u32_op_delay_off_time = Timer_set_timer( TIMER_MS( 3000 ));
}


//******************************* CODEC FUNCTIONS *****************************************

#if 0
//! Detect & load & start the codec player
//!
Bool player_codec_start( void )
{
   if( !file_ispresent() )
   {
      player_g_status = PLAYER_ERR_ACCESS_FILE;
      return FALSE;
   }

#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
   if( FALSE == g_b_play_ap3 )
   {
#endif   //#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
//***********************************************
      if( nav_file_checkext( player_dir ))
      {
         varAl.WAVE_Flg = 0;
         wav_encrypt = FALSE;
         return TRUE;
      }
      if( nav_file_checkext( ext_filemusic ))
      {
         varAl.WAVE_Flg = 1;
         wav_encrypt = FALSE;
         return TRUE;
      }
      return FALSE;
//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
   }
#endif   //#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
//***********************************************
   return TRUE;
}


//! Stop the codec player
//!
void player_codec_stop( void )
{
}
#endif
//******************************* STREAM FONCTIONS *****************************************

//! Check if no stream is on going
//!
Bool  player_stream_is_on( void )
{
   //player_g_status = PLAYER_ERR_STREAM_ONGOING;
   return (ID_STREAM_ERR != p_g_Id_stream);
}


#pragma CODE = SRV_PLAYER_TEMP_CODE06

//! Check if a stream is on going
//!
Bool  player_stream_is_off( void )
{
   //player_g_status = PLAYER_ERR_NO_STREAM_ONGOING;
   return ((ID_STREAM_ERR == p_g_Id_stream)/* && (ID_STREAM_ERR == p_g_Id_stream_empty)*/);
}


#pragma CODE = DAC_COMMON_CODE

//! Start the stream
//!
Bool  player_start_stream( void )
{
  // if( player_stream_is_on() )
  //    return FALSE;
/*
//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
   //add by wangwei
   if( FALSE == g_b_play_ap3 )
   {
#endif
//***********************************************
   // start the stream
   player_g_status = PLAYER_ERR_ACCESS_FILE;
   if( !file_open(FOPEN_MODE_R))
      return FALSE;
   if( 1024 > nav_file_lgt() )
   {
      // Codec don't support file < 1KB
      file_close();
      return FALSE;
   }
   ap4_audio_var.audio_var.file_size_count = 0;
   ap4_audio_var.audio_var.play_file_size = fs_g_nav_entry.u32_size;

   wav_encrypt = FALSE;
   LROM_SCRAMBLE = 0x00;
   //LROM_EXCHANGE = 0x00;
//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
   //add by wangwei
   }
#endif
//***********************************************
*/

   srvplayer_volume_send();
   player_volume_on();
   exrcen_enable();
   //if( !b_HeadPhone_on )
   {
      //b_HeadPhone_on = TRUE;
      //DAC_Disable_Speaker();
      //DAC_Enable_HeadPhone();
   }
   //DAC_FadeIn();
   if( audio_is_wav == 0 )
   {
      //SendString((U8 *)"mp3play\r\n");
      media_paly_all_time = AudioMP3GetPlayTime( TOTAL_TIME_MODE );
	  /*
	  if(count_AB == 4){
			count_AB = 0;
			goto next_point;
	  	}
	  if(count_AB == 3){
next_point:	  	
	  	AudioMP3SetPlayTime(point_A);
	  	}*/
      if( AudioMP3Play() != AUDIO_ERROR_OK )
      {
//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
         //add by wangwei
         if( TRUE == g_b_play_ap3 )
         {
            AudioMP3Stop();
         }
         else
#endif
//***********************************************
         {
            AudioMP3Close();
         }
         AudioMP3Uninit();
         return FALSE;
      }
   }
   else
   {
      DAC_Go();
   }
   p_g_Id_stream = 0;
//   TM2ICR = 0x0031;
   player_task_start();       // Enable event of player task
   power_forbid_autopoweroff();
   srvplayer_g_player_state = MMI_PLAYER_STATE_PLAY;
   return TRUE;
}


#pragma CODE = PLAYER_CODE

//! Stop the stream
//!
void player_stop_stream( void )
{
   //U8 nav_id_save;
//***********************************************
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
   extern Bool g_b_player_on;
#endif   //#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
//***********************************************
   if( ID_STREAM_ERR != p_g_Id_stream)
   {
      
      //nav_id_save = nav_get();         // Get current navigator
      //nav_select( player_g_nav_id );
#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
      //change by wangwei
      if( g_b_play_ap3 )
      {
         g_b_player_on = FALSE;
         g_b_play_ap3 = FALSE;
         //bSpeechEnd = TRUE;

         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         AudioMP3Stop();
         AudioMP3Uninit();
         
         //add by xuanyu
         //power_authorize_autopoweroff();
         //GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
         //GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
      }
      else
#endif   //#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
      {
         if( audio_is_wav == 0 )
         {
            AudioMP3Close();
            AudioMP3Uninit();
         }
         else
         {
            DAC_Pause();
            DAC_Mute();
            FSClose(( __X FCFILE *)ADPCM_File );
         }
      }

      power_authorize_autopoweroff();
      //nav_select( nav_id_save );
      p_g_Id_stream = ID_STREAM_ERR;   // reinit value with no stream
      count_AB = 0;
   }
}


#if 0
extern   xdata    SSSCODECREG       CodecReg;      // PlayBack ADPCM/PCM CTL
extern   xdata    SSSRECCODECREG    RecCodecReg;   // Recording ADPCM/PCM CTL
extern   xdata    SSSADCREG         ADCReg;        // ADC Ctl Reg
//extern   xdata    AL_Variables      varAl;

// Mason@0425 modified
void Codec_Init( void )
{
   CodecReg.EndofFileCnt = FileData.AudioDataSz / 2;

   //Set DMA start address where audio data start.
   CodecReg.DMAStartAdr = ( USHORT )( FileData.DataChukoffsetBytes >> 1 )& 0x000003FF;

   if( FileData.wfx.wFormatTag == WAVE_FORMAT_IMA_ADPCM )
   {
      // Set Block align for only ADPCM
      CodecReg.BlkAlign = (( FileData.wfx.nBlockAlign / 2 )- 1 );
      //SendString("ADPCM\r\n");
      CodecReg.Ctl |= 0x04;   //Enable ADPCM
   }
   else
   {
      //SendString(" PCM  -\r\n");
      CodecReg.Ctl &= ~0x04;  //Enable PCM
   }

	if( FileData.wfx.nChannels == 0x02 )
	{
      //SendString(" Stereo \r\n");
      CodecReg.Ctl |= 0x08;   //Stereo
   }
   else
   {
      //SendString(" Mono \r\n");
      CodecReg.Ctl &= ~0x08;  //Mono
   }

   /*if( varAl.fWaveFastWind )
   {
      CodecReg.Ctl  = varAl.Codec_CTL_Val;
      SendString("FW - CodecReg.Ctl:",CodecReg.Ctl);
   }
   else*/
   {
      CodecReg.Ctl |= 0x01;        // Eanble Play
      //varAl.Codec_CTL_Val = CodecReg.Ctl; // Saveing CodecCtl for FastWind in WAV
   }
}


void ClassD_Init( void )
{
   CLASS_D_FU_L_CTL = 0x01;
   CLASS_D_FU_R_CTL = 0x01;

   /* Set Class D Sample rate */
   //SendString("ClassD:");
   switch( FileData.wfx.nSamplesPerSec )
   {
      case 0xBB80:   // 48K
      //CLASS_D_DAC_CTL = 0x50;
      CLASS_D_DAC_CTL = 0xD0;
      //SendString("48K\r\n");
      break;

      case 0xAC44:   //44.1K
      //CLASS_D_DAC_CTL = 0x40;
      CLASS_D_DAC_CTL = 0xC0;
      //SendString("44.1K\r\n");
      break;

      case 0x7D00:   //32K
      //CLASS_D_DAC_CTL = 0x60;
      CLASS_D_DAC_CTL = 0xE0;
      //SendString("32K\r\n");
      break;

      case 0x5DC0:   // 24K
      CLASS_D_DAC_CTL = 0x90;
      //SendString("24K\r\n");
      break;

      case 0x5622:   //22.05K
      CLASS_D_DAC_CTL = 0x80;
      //SendString("22.05K\r\n");
      break;

      case 0x3E80:   //16K
      CLASS_D_DAC_CTL = 0xA0;
      //SendString("16K\r\n");
      break;

      case 0x2EE0:   //12K
      CLASS_D_DAC_CTL = 0x10;
      //SendString("12K\n");
      break;

      case 0x2B11:   //11.025K
      CLASS_D_DAC_CTL = 0x00;
      //SendString("11.025K\r\n");
      break;

      case 0x1F40:   // 8K
      CLASS_D_DAC_CTL = 0x20;
      //SendString("8K\r\n");
      break;

      default:
      //SendString("Not Support Fs\r\n");
      break;
   }
   CodecReg.INT_MASK_PLAY = 0xB3; // open bit6,3 and 2

   //Wait_POP_CTL();   // Ron@2009/3/27
}


void WAV_ActivePlayBack( void )
{
   Codec_Init(); //Setting codec
   ClassD_Init();//Setting class D
}

//void MP3_Init( void )
void SSSCodeInit( void )
{
   mp3_isr_flag = 0;
   tmpINT3 = 0;
   g_b_music_end = 0; // must reset this for INT3 use

   // set-up MP3 Left DMA
   MP3_DMASTART0700_LEFT = 0x00; // in MP3 0x0 Byte
   MP3_DMASTART1508_LEFT = 0x00;
   MP3_DMALEN0700_LEFT   = 0xFF;  // in MP3
   //MP3_DMALEN1508_LEFT   = 0x03; // -> 0x0 ~ 0x01FF= 512 Bytes
   MP3_DMALEN1508_LEFT   = 0x07; // -> 0x0 ~ 0x07FF= 2048 Bytes

   // Start MP3 decoder
   MP3_CTL = STEREO_WAVE | L_ENDIAN; // Bit5->Stero mode, Bit4->Little Endian,MP3 mode
}


void MP3_INT_Enable( void )
{
   EIP |= 0x02;
   // set-up INT3
   MP3_INT_EN = 0x7F;         // -> move to before _getkey()
   //MP3_INT_MASK = 0x00;
   MP3_INT_MASK &= 0xC0;      // 2007@0626 3S1120

   if( !varAl.WAVE_Flg )
   {
      MP3_DMACTL = DMA_READY_L;  // set left DMA is ready
      MP3_FIFOCTL = FIFO_EN;     // PCM_output FIFO enable -> Must first enable it for playback WAVE file
   }
   else
   {
   }
   //MP3_I2S_RATE = 0x07;
   //MP3_I2S_DUTY = 0x04;
   //CLASS_D_CTL_OUT |= 0x80;
   EIE |= 0x02;               // enable INT3
}


//void MP3_ActiveFAT( void )
void SSS_CodecActive( void )
{
   if( varAl.WAVE_Flg == 0 )
   {
      //MP3_Init();
      SSSCodeInit();
      SYSDelay();
      //Wait_POP_CTL();  // Ron@2009/3/27
      CLASS_D_DAC_CTL &= 0xF0;
   }
   
   MP3_INT_Enable();
   if( varAl.WAVE_Flg )
      WAV_ActivePlayBack();
   SDIFCON0 &= 0xFE;  // Disable SD_CLK Mason@1031
   varAl.Run_status = SysPlayMusic ;// Playback
   SYSDelay();
}


Bool  CheckWAVFrameHeader  ( void );
void  encrypt_wav_header   ( void );

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


#if 1

#pragma CODE = PLAYER_WAV_CODE

//play wav
const unsigned int SR2PCMSR_TBL[] =
{
   ( unsigned int )96000,
   ( unsigned int )48000, 
   ( unsigned int )44100, 
   ( unsigned int )32000, 
   ( unsigned int )24000, 
   ( unsigned int )22050, 
   ( unsigned int )16000, 
   ( unsigned int )12000, 
   ( unsigned int )11025, 
   ( unsigned int )9600,
   ( unsigned int )8000
};


static unsigned int SmpRate2PCMSR( unsigned int smp_rate )
{
   int i;
   for( i = 0; i < sizeof( SR2PCMSR_TBL )/ sizeof( unsigned int ); i++ )
   {
      if(SR2PCMSR_TBL[ i ] == smp_rate )
         return i;
   }
   return DAC_SAMPLE_RATE_44P1K;
}


Bool WavPlay( char *path )
{
   __X FS_ERR fserr;
   tIMA_INFO ADPCM_Info;
   unsigned long read_length;
   int error;

   // calculate how many frames of PCM samples
   PCM_Buffer_Size = Default_PCM_Buffer_Size;
   ADPCM_Buffer_Size = Default_ADPCM_Buffer_Size;
 //  all_length = FSGetFileSize(path, &fserr);
 // SendString((U8 *)"\r\n1071\r\n");
 // SendString((U8 *)path);
   ADPCM_File = ( __X FCFILE *)FSOpen((char *)path, FS_MODE_READ_WRITE, &fserr );
   if( fserr )
   {
   	  // SendString((U8 *)"\r\nwav open fail");
      //FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
      //goto wav_play_error1;
   }
 // SendString((U8 *)"\r\n1082");
   //read the 60-byte file header 
   read_length = FSRead(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_File_Header, 60, &fserr );
   if( read_length == 0 )
   {
wav_play_error1:
	 // SendString((U8 *)"\r\nwav ko");
      FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
   }
   //SendString((U8 *)"\r\n1092");
   // Initial ADPCM  Decoder 
   error = IMA_ADPCM_Decoder_Init( &ADPCM_Info );  

   if( error == 0 )
   {
      Sample_Rate = ADPCM_Info.IMA_SR;                   // Sampling rate for playing the decoded PCM samples    
      PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;      // buffer size for output PCM
      ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;  // buffer size for input ADPCM
      Frame_Count = ADPCM_Info.IMA_Frame_No;             // Tell the user how many frame shall be decoded.
      media_paly_all_time = Frame_Count;
   }
   else
   {
      goto wav_play_error1;
   }
   //SendString((U8 *)"\r\n1108");
   //b_flush_lcd = FALSE;
   // Initial DAC 
   DAC_Init( DTU3, DTU2, FillBuffer, 0 );

   DAC_LChannelBuffer1 = ( __X int *)PCM_Buf_1;
   DAC_RChannelBuffer1 = ( __X int *)PCM_Buf_1;
   DAC_LChannelBuffer2 = ( __X int *)PCM_Buf_2;
   DAC_RChannelBuffer2 = ( __X int *)PCM_Buf_2;    
   DAC_FrameSize = PCM_Buffer_Size;     

   FirstDecode();
   //SendString((U8 *)"\r\n1120");
   return TRUE;
}


//#pragma CODE = PLAYER_CODE
#pragma CODE = PLAYER_WAV_CODE

void FillBuffer( void )
{
   unsigned long len;
   __X int *BufPtr;
   __X FS_ERR fserr;
   void ( *Fill_Buf )( void );

   //if( !Frame_Count )
   //   return;
   if(Frame_Count == 0)
   		return;

   if(b_flush_lcd)return;
   if( DAC_Buffer1Empty())
   {
      BufPtr = DAC_LChannelBuffer1 ;
      Fill_Buf = DAC_FillBuffer1;       
   }
   else if( DAC_Buffer2Empty())
   {
      BufPtr = DAC_LChannelBuffer2 ;
      Fill_Buf = DAC_FillBuffer2;
   }


  if(count_AB == 3){
		if(( point_B - Frame_Count)<=50){
			
			FSSeek(ADPCM_File,(((media_paly_all_time - point_A)<<8)+60));
			Frame_Count = point_A;
		}
  	}
  
   len = FSRead(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_Buf, ADPCM_Buffer_Size * 2, &fserr );

   if(( len == 0 )||( fserr != FS_NO_ERROR ))
   {
      Frame_Count = 0;
      return;
   }
   
   IMA_ADPCM_Decoder(( __X int *)ADPCM_Buf, BufPtr );

   ( *Fill_Buf )();

   //if( Frame_Count )
    Frame_Count--; 
    current_play_time = media_paly_all_time - Frame_Count;
}


#pragma CODE = PLAYER_WAV_CODE

void FirstDecode( void )
{
   unsigned long len;
   __X FS_ERR fserr;

   DAC_SampleRate = SmpRate2PCMSR( Sample_Rate );

   len = FSRead(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_Buf, ADPCM_Buffer_Size * 2, &fserr );

   IMA_ADPCM_Decoder(( __X int *)ADPCM_Buf, DAC_LChannelBuffer1 );

   DAC_FillBuffer1();
}
#endif

#endif 	// _player_c_
