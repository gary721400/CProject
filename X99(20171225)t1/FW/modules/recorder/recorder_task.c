//!
//! @file recorder_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: recorder_task.c,v 2.0 2007/03/30 12:11:45 sguyon Exp $
//!
//! @todo
//! @bug


//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_audio.h"                 // Audio user's configuration
#include "conf\conf_explorer.h"
//#include "conf\conf_lcd.h"                   // Needs IMAGE_SUPPORT definition only

// Include for mail box
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"

#include "recorder_task.h"
//#include "recorder.h"

#include "modules\power\power.h"             // Power module definition
#include "modules\control_access\ctrl_access.h"
#include "lib_system\unicode\unicode.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include <fs.h>
#include <string.h>
#include "agc.h"

#include "modules\recorder\recorder.h"
#include "mmi\recorder\srv_recorder.h"
#include "sio\com.h"
#include "fc_internal.h"

#pragma DATA = RECORDER_TASK_DATA
#pragma CODE = RECORDER_TASK_CODE

ImageDeclareType( AUADC_COMMON_CODE );
ImageDeclareType( KB_RESIZE_CODE );

#if (AUDIO_RECORD == ENABLE)

//_____ M A C R O S ________________________________________________________
extern   __X   FCFILE   *ADPCM_File;

#if( AUDIO_RECORD_BUF == DISABLE )
extern         int      NotifyFlag;
extern   __X   int      *NotifyBuf;
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

//_____ D E F I N I T I O N ________________________________________________

//Bool g_b_recorder_on       = FALSE;
extern   Bool     g_b_recorder_on;


#pragma CODE = TASK_INIT_CODE

//! Initialization of recorder task
//!
void recorder_task_init(void)
{
   SDKLoadCode( AUADC_COMMON_CODE );
   // Initial Audio ADC
   // Set Audio ADC parameters
#if 0
   AUADC_Init( MIC_In, DTU1, AUADC_SAMPLE_RATE_8K, Buffer_Full_Func );
   //AUADC_Set_Buffer(( __X int *)ADC_Buf1, ( __X int *)ADC_Buf2, PCM_BUF_SIZE );

   AUADC_MicGain( MIC_Gain_20dB );
   AUADC_InputGain( AUADC_Gain_16P5dB );
#else
   AUADC_Init( MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, Buffer_Full_Func );
//   AUADC_Init( MIC_In, DTU1, AUADC_SAMPLE_RATE_8K, Buffer_Full_Func );
   //AUADC_Set_Buffer(( __X int *)ADC_Buf1, ( __X int *)ADC_Buf2, PCM_BUF_SIZE );

   AUADC_MicGain( MIC_Gain_0dB );
   //AUADC_MicGain( MIC_Gain_20dB );
   AUADC_InputGain( AUADC_Gain_22P5dB );
#endif

   g_recorder_state = MMI_RECORDER_STATE_STOP;
   recorder_id_stream = ID_STREAM_ERR;
   rp_flag_bits.value = 0x00;
}


//#pragma CODE = RECORDER_CODE
#pragma CODE = AUADC_COMMON_CODE

void Buffer_Full_Func( __X int* buffer, unsigned int buf_size )
{
#if( AUDIO_RECORD_BUF == DISABLE )
   if( NotifyFlag > 0 )
   {
      NotifyError = 1;
   }
   NotifyFlag++;

   NotifyBuf = buffer;

   Frame_Cnt++;
#if( AUDIO_RECORD_AGC == ENABLE )
   if( Frame_Cnt > 32 )
   {
      AGC( NotifyBuf );
   }
#endif   //#if( AUDIO_RECORD_AGC == ENABLE )
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

#if( AUDIO_RECORD_BUF == ENABLE )
   if( idx_R ==( idx_L - 1 )
      ||( idx_R ==( ADPCM_BUF_CNT - 1 )&&( idx_L == 0 )))
      return;

   Frame_Cnt++;
#if( AUDIO_RECORD_AGC == ENABLE )
   if( Frame_Cnt > 32 )
   {
      AGC( buffer );
   }
#endif   //#if( AUDIO_RECORD_AGC == ENABLE )

   IMA_ADPCM_Encoder( buffer, REC_ADPCM_Buf[ idx_R++ ]);
   if( idx_R >( ADPCM_BUF_CNT - 1 ))
      idx_R = 0;
#endif   //#if( AUDIO_RECORD_BUF == ENABLE )
}

//#pragma CODE = RECORDER_TASK_CODE
#pragma CODE = RECORDER_TEMP_CODE01

#if(( AP3 == ENABLE )||( PASTER == ENABLE ))
//! Start recorder task
//!
void recorder_task_start(void)
{
   g_b_recorder_on = TRUE;
   g_b_set_led = TRUE;
}


#pragma CODE = RECORDER_TEMP_CODE03

//! Stop recorder task
//!
void recorder_task_stop(void)
{
   g_b_recorder_on = FALSE;
}
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))


#pragma CODE = RECORDER_TASK_CODE

//! Send a command to the recorder task and/or get a event from recorder task
//!
void recorder_task(void)
{
   //_MEM_TYPE_MALLOC_MAIL_ Msg Cmd;
   //__X FS_ERR fserr;

   if( g_b_recorder_on )
   {
      record_process();
#if 0
      if( Power_is_poweroff_activated() )
         return;

      // Look recorder command
      /*if( MAILBOX_NO_MSG != mail_get_command(TASK_RECORDER, &Cmd) )
      {
      }*/

      if( NotifyFlag > 0 )
      {
         NotifyFlag--;

         memcpy( PCM_Buf, NotifyBuf, PCM_BUF_SIZE * 2 );
         IMA_ADPCM_Encoder(PCM_Buf, ADPCM_Buf);      

         WavWriteSize += FSWrite(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_Buf, ADPCM_BUF_SIZE * 2, &fserr );      

         if(( WavStepSize - WavWriteSize )< 512ul*1024ul )
         {
            WavStepSize += BEGIN_RECORD_SPACE;
            if(( free_space - WavStepSize )< 1024ul * 32ul )
            {
               srvrec_stop();
               mail_send_event( EVT_RECORDER, RECORD_EOF );
            }
            else
            {
               FSResize(( __X FCFILE *)ADPCM_File, WavStepSize );
            }
         }
      }
#endif
   }
}


#pragma CODE = AUADC_COMMON_CODE

void record_process( void )
{
   __X FS_ERR fserr;
	U16 data_length;
#if( AUDIO_RECORD_BUF == ENABLE )
   __X int count;
#endif   //#if( AUDIO_RECORD_BUF == ENABLE )
	
   if( Power_is_poweroff_activated() )
      return;

   // Look recorder command
   /*if( MAILBOX_NO_MSG != mail_get_command(TASK_RECORDER, &Cmd) )
   {
   }*/

#if( AUDIO_RECORD_BUF == DISABLE )
   if( NotifyFlag > 0 )
   {
      NotifyFlag--;

      memcpy( PCM_Buf, NotifyBuf, PCM_BUF_SIZE * 2 );
      IMA_ADPCM_Encoder( PCM_Buf, ADPCM_Buf );

#if 0
      WavWriteSize += FSWrite(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_Buf, ADPCM_BUF_SIZE * 2, &fserr );
#else
      data_length = FSWrite(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_Buf, ADPCM_BUF_SIZE * 2, &fserr );

      if( fserr != FS_NO_ERROR )
      {
         //SendString((U8 *)"write err\r\n");
         srvrec_stop();
       	mail_send_event( EVT_RECORDER, RECORD_EOF );
         return;
      }
      WavWriteSize += data_length;
#endif

      if( WavStepSize <( 512ul*1024ul + WavWriteSize ))
      {
         WavStepSize += BEGIN_RECORD_SPACE;
#if 0
         if(( free_space < WavStepSize )
            ||(( free_space - WavStepSize )< 1024ul * 32ul ))
         {
            srvrec_stop();
            mail_send_event( EVT_RECORDER, RECORD_EOF );
         }
         else
         {
            FSResize(( __X FCFILE *)ADPCM_File, WavStepSize );
         }
#else
         SDKLoadCode( KB_RESIZE_CODE );
         fserr = kb_fat_resize(( __X FCFILE *)ADPCM_File, WavStepSize );
         if( fserr != FS_NO_ERROR )
         {
            //SendString((U8 *)"serr\r\n");
            srvrec_stop();
            mail_send_event( EVT_RECORDER, RECORD_EOF );
         }
#endif
      }
   }
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

#if( AUDIO_RECORD_BUF == ENABLE )
   while( idx_L != idx_R )
   {
      if( idx_R > idx_L )
         count = idx_R - idx_L;
      else
         count = ADPCM_BUF_CNT - idx_L;

      data_length = FSWrite(( __X FCFILE *)ADPCM_File, ( void *)REC_ADPCM_Buf[ idx_L ], count * ADPCM_BUF_SIZE * 2, &fserr );
      if( fserr != FS_NO_ERROR )
      {
         //SendString((U8 *)"write err\r\n");
         srvrec_stop();
         mail_send_event( EVT_RECORDER, RECORD_EOF );
         return;
      }
      WavWriteSize += data_length;

      idx_L += count;
      if( idx_L >( ADPCM_BUF_CNT - 1 ))
         idx_L = 0;

      if( WavStepSize <( 512ul*1024ul + WavWriteSize ))
      {
         WavStepSize += BEGIN_RECORD_SPACE;
         
         SDKLoadCode( KB_RESIZE_CODE );
         fserr = kb_fat_resize(( __X FCFILE *)ADPCM_File, WavStepSize );
         if( fserr != FS_NO_ERROR )
         {
            //SendString((U8 *)"serr\r\n");
            srvrec_stop();
            mail_send_event( EVT_RECORDER, RECORD_EOF );
         }
      }
   }
#endif   //#if( AUDIO_RECORD_BUF == ENABLE )
}

#endif // AUDIO_RECORD == ENABLE




