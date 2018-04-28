//!
//! @file recorder.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 1.4 snd3-dvk-1_9_5 $Id: recorder.c,v 1.4 2007/09/24 09:40:34 sguyon Exp $
//!
//! @todo see a sequence record, SG !!see a optimization -> remove division
//! @bug

#ifndef _recorder_c_
#define _recorder_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_audio.h"                 // Audio user's configuration
#include "recorder.h"

// Include Audio module definition
#include "modules\audio\audio.h"
// Include the file system access
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"
//#include "modules\file_system\fat.h"
// Include for hardware access
#include "modules\control_access\ctrl_status.h"
#include "modules\control_access\ctrl_access.h"
// Include the timer macros
#include "lib_mcu\timer\timer_drv.h"
#include "conf\conf_sio.h"
#include "mmi\recorder\srv_recorder.h"
//#include "mmi\recorder\mmi_recorder.h"

#include "modules\player\player_task.h"
//#include "SSSMP3_Regs.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "modules\power\power.h"             // Power module definition
#include "sio\com.h"
#include "mmi\shared\com_appli.h"
#include "modules\recorder\recorder_task.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include <fs.h>
#include <string.h>
#include "agc.h"
#include "fc_internal.h"
#include "mmi\ap3\mmi_ap3.h"
#include "conf\conf_sio.h"


#if ((AUDIO_RECORD == ENABLE) && ((FS_LEVEL_FEATURES & FSFEATURE_WRITE) != FSFEATURE_WRITE))
#  error ERROR CONFIGURATION: For the AUDIO_RECORD option, you must define FS_LEVEL_FEATURES with a FSFEATURE_WRITE option in "conf_explorer.h"
#endif

#pragma DATA = RECORDER_DATA
#pragma CODE = RECORDER_CODE

ImageDeclareType( AUADC_COMMON_CODE );
//ImageDeclareType( AGC_Init_Code );
//ImageDeclareType( AGC_Code );

ImageDeclareType( KB_RESIZE_CODE );

#if (AUDIO_RECORD == ENABLE)

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

// Used a timer for the recorde time
_MEM_TYPE_SLOW_ U32 s_u32_viewer_timeout;

_MEM_TYPE_SLOW_   U32               s_u32_reocrder_time;
_MEM_TYPE_SLOW_   Recorder_states   g_recorder_state /*= MMI_RECORDER_STATE_STOP*/;

//----------------
// Streams Control
//_MEM_TYPE_SLOW_ U8  recorder_id_nav;                     // Navigator selected to run the stream
_MEM_TYPE_SLOW_ U8  recorder_id_stream /*= ID_STREAM_ERR*/;  // ID of stream running, by default no stream
//_MEM_TYPE_SLOW_ Fs_file_segment  g_recorder_seg;
// End streams Control
//----------------
//***********************************************
#if 0//(( AP3 == ENABLE )||(PASTER == ENABLE ))
//add by wangwei
//U32 _MEM_TYPE_SLOW_ g_u32_record_start = 0;
U8 _MEM_TYPE_SLOW_ g_u8_record_open_type = FOPEN_MODE_W;
#endif
//***********************************************

//----------------
// record Control
//#define RECORD_SEG_STEP    (500*1024L/FS_512B)          // Alloc segment step of 500KB to record file
// End record Control
//----------------

//extern   xdata    AL_Variables     	varAl;

#define  WAV_SAMPLE_RATE         16000 //8000	// //32000  //8000     //
#define  WAV_AGC                 8		//16     //32     //8        //

//_MEM_TYPE_SLOW_   U32   free_space;
_MEM_TYPE_SLOW_   U32   WavWriteSize;
_MEM_TYPE_SLOW_   U32   WavStepSize;

#if( AUDIO_RECORD_BUF == DISABLE )
#if 0
__X int PCM_Buf[ PCM_BUF_SIZE ];
#endif

int NotifyFlag, NotifyError;
__X int *NotifyBuf;
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

#if 1//( AUDIO_RECORD_AGC == ENABLE )
unsigned long Frame_Cnt /*= 0ul*/;
#endif   //#if( AUDIO_RECORD_AGC == ENABLE )

#if( AUDIO_RECORD_BUF == ENABLE )
__X int idx_L, idx_R;

#if 0
__X int REC_ADPCM_Buf[ ADPCM_BUF_CNT ][ ADPCM_BUF_SIZE ];
#endif
#endif   //#if( AUDIO_RECORD_BUF == ENABLE )

_MEM_TYPE_SLOW_      RP_BIT_NEW        rp_flag_bits /*= { 0x00 }*/;


//_____ D E C L A R A T I O N ______________________________________________

//! Stream fonctions
Bool record_stream_is_on     ( void );
Bool record_stream_init      ( void );
void record_stream_stop      ( void );


#if 0//(( AP3 == ENABLE )||( PASTER == ENABLE ))
//extern   xdata    SSSCODECREG       CodecReg;      // PlayBack ADPCM/PCM CTL
//extern   xdata    SSSRECCODECREG    RecCodecReg;   // Recording ADPCM/PCM CTL
//extern   xdata    SSSADCREG         ADCReg;        // ADC Ctl Reg


/*===  WAV( 16-bits PCM & 4-bits IMA ADPCM ) Record === */
void ADC_Setup( void )
{
   /* Set ADC Sample rate */
	ADCReg.Clk_Div = 0x00;  // Mason@20090324
	//SendString("ADC fs:");
   switch( FileData.wfx.nSamplesPerSec )
   {
      /*
      case 0xBB80:   // 48K
      //ADC_CTL = 0x50;
      ADCReg.Ctl = 0x50;
      SendString("48K\r\n");
	   break;

      case 0xAC44:   //  44.1K
      ADCReg.Ctl = 0x40;
      SendString("44.1K\r\n");
      break;

      case 0x7D00:   // 32K
      ADCReg.Ctl = 0x60;
      SendString("32K\r\n");
      break;

      case 0x5DC0:   // 24K
      ADCReg.Ctl = 0x90;
      SendString("24K \r\n");
      break;

      case 0x5622:   // 22.05K
      ADCReg.Ctl = 0x80;
      SendString("22.05K\r\n");
      break;

      case 0x2B11:   // 11.025K
      ADCReg.Ctl = 0x00;
      SendString("11.025K\r\n");
      break;
      */
      
      case 0x2EE0:   // 12K
      ADCReg.Ctl = 0x17;
      //SendString("12K\r\n");
      break;

      case 0x3E80:   // 16K
      //ADCReg.Ctl = 0xA0;
      ADCReg.Ctl = 0xA7;// Mason@20090324
      //SendString("16K \r\n");
      break;

      case 0x1F40:   // 8K
      //ADC_CTL = 0x20;
      ADCReg.Ctl = 0x27; // ADC input
      //SendString("8K\r\n");
      break;

      default:
      //SendString("Not Support Fs\r\n");
      break;
	}

   //ADCReg.DF_Ctl = 0x01;   //Enabke Decimention filter(bit 0) with Dis-able DC removal function(bit4) -->  open it before tape-out
   //ADCReg.DF_Ctl = 0x11;
   //ADCReg.SAR_En |= 0x01;//Enable SAR ADC
   ADCReg.SAR_En = 0x21;   //Enable SAR ADC & Mic-IN Mason@20090324
   //ADCReg.MIC_CTL = 0x10;  // Mason@20090324
   //ADCReg.PGA = 0x0C;      // Mason@20090324
}


void WAV_RecPrevInit( void )
{
   //CLASS_D_DAC_CTL = 0x03;  // suspend & Mute Class-D
   MP3_INT_MASK |= 0x3F;      // 2007@0626 3S1120
   CLASS_D_FU_L_CTL = 0x00;
   CLASS_D_FU_R_CTL = 0x00;
   EIE &= ~0x02;  // Dis-able INT3

   //CLASS_D_PWM_CTL = 0x00;  // Ron@2009/3/27

   RecCodecReg.Ctl = 0;
   RecCodecReg.EndofFileCnt = 0;
   RecCodecReg.BlkAlign = 0;
   RecCodecReg.DMAStartAdr = 0;
   RecCodecReg.INT_Mask_Record = 0x2C;
   //CodecReg.DMACtl = 0;  // once set bit0=0 will active DMA .but still need to wait output Class-D enable

	ADCReg.SAR_En = 0;
   //ADCReg.SAR_En = 0x80; // ADC power down
   //ADCReg.DF_Ctl = 0;

   //ADCReg.MIC_CTL = 0x00;  //
   //ADCReg.PGA     = 0x00;  //
   RecCodecReg.DMACtl |= 0x03;  // CLear End_Flag
}


void WAV_ActiveRecord( void )
{
   MP3_INT_EN = 0x7f;      //
   MP3_INT_MASK &= 0xC0;   // 2007@0626 3S1120
   EIE |= 0x02;            // enable INT3

   //Set Codec Register
   RecCodecReg.INT_Mask_Record &= ~0x2C;  // enable INT_Mask

   if( FileData.wfx.wFormatTag == WAVE_FORMAT_IMA_ADPCM )
   {
      RecCodecReg.DMAStartAdr = 0x1E;

      //Set Block align for only ADPCM
      RecCodecReg.BlkAlign = (( FileData.wfx.nBlockAlign / 2 )- 1 );
      RecCodecReg.Ctl = 0x05;
   }
   else
   {
      //SendString("PCM Recording...\r\n");
      RecCodecReg.DMAStartAdr = 0x16;
      RecCodecReg.Ctl = 0x01;
   }

   //Set ADC Register
   ADC_Setup();
}


void ADPCM_Rec( void )
{
   if( varAl.Run_status == SysStopMusic )
   {
      //SendString("Recording--:\r\n");
          
      WAV_RecPrevInit();
      WAV_ActiveRecord();
      varAl.Run_status = SysRecordADPCM;
   }
   else if( varAl.Run_status == SysRecordADPCM )
   {
      RecCodecReg.Ctl |= 0x02;
      //SendString("Pause Recording:");
      //send_hex_u8(RecCodecReg.Ctl);
      //SendString("\r\n");
      EIE &= 0xFD;   // Disable INT3
      varAl.Run_status = SysPauseRecord;
   }
   else if( varAl.Run_status == SysPauseRecord )
   {
      EIE |= 0x02;   // enable INT3
      RecCodecReg.Ctl &= ~0x02;
      //SendString("Resume Recording:");
      //send_hex_u8(RecCodecReg.Ctl);
      //SendString("\r\n");
      varAl.Run_status = SysRecordADPCM;
   }
}
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))


#if(( AP3 == ENABLE )||( PASTER == ENABLE ))
#if 0
unsigned int MIN( unsigned int x, unsigned int y )
{
   if( x > y )
      return ( y );
   else
      return ( x );
}


//=======================================================================
// Function name: IMAADPCM_MakeHeader ()
// Input  : IMAADPCM information structure:wfx
// Output : none.
// Return : Size of header
// Description: To create 4-Bits IMA ADPCM Header at DAC buffer left
//======================================================================= 
void IMAADPCM_MakeHeader( void )
{
   //Setting wave file header information in ADC buffer Left
   // Set 4-bits IMA ADPCM wave header //
   FileData.pChar = ( UCHAR volatile xdata *)sbufL_ADPCM;  // at 0xB800
   // RIFF header
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffID[ 0 ] = 'R';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffID[ 1 ] = 'I';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffID[ 2 ] = 'F';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffID[ 3 ] = 'F';

   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffSize = 0;//FileData.TotalRecFileSz - 8;

   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffFormType[ 0 ] = 'W';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffFormType[ 1 ] = 'A'; 
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffFormType[ 2 ] = 'V'; 
   (( IMA_ADPCM_HEADER *)FileData.pChar )->riffFormType[ 3 ] = 'E';   

   // IMA ADPCM WAVE format chunk
   (( IMA_ADPCM_HEADER *)FileData.pChar )->fmtID[ 0 ] = 'f';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->fmtID[ 1 ] = 'm';    
   (( IMA_ADPCM_HEADER *)FileData.pChar )->fmtID[ 2 ] = 't';   
   (( IMA_ADPCM_HEADER *)FileData.pChar )->fmtID[ 3 ] = ' ';      
   (( IMA_ADPCM_HEADER *)FileData.pChar )->fmtSize = 0x14000000;  //20Bytes

   (( IMA_ADPCM_HEADER *)FileData.pChar)->wFormatTag = 0x1100;    //IMA_ADPCM
   FileData.wfx.nAvgBytesPerSec = FileData.wfx.nSamplesPerSec * FileData.wfx.nChannels / 2;  

   FileData.wfx.nBlockAlign = 256 * FileData.wfx.nChannels * MIN( 1, ( FileData.wfx.nSamplesPerSec / 11 ));

   FileData.wfx.wSamplesPerBlock = (( FileData.wfx.nBlockAlign -( 4 * FileData.wfx.nChannels ))* 8 )/( 4 * FileData.wfx.nChannels )+ 1;
   //Firstly,convert big endian into little endian then set value 
   ENDIAN_ASSIGN_2(&((( IMA_ADPCM_HEADER *)ADPCMBufL )->nChannels ), &( FileData.wfx.nChannels ));
   ENDIAN_ASSIGN_4(&((( IMA_ADPCM_HEADER *)ADPCMBufL )->nSamplesPerSec ), &( FileData.wfx.nSamplesPerSec ));   
   ENDIAN_ASSIGN_4(&((( IMA_ADPCM_HEADER *)ADPCMBufL )->nAvgBytesPerSec ), &( FileData.wfx.nAvgBytesPerSec ));  
   ENDIAN_ASSIGN_2(&((( IMA_ADPCM_HEADER *)ADPCMBufL )->nBlockAlign ), &( FileData.wfx.nBlockAlign )); 
   ENDIAN_ASSIGN_2(&((( IMA_ADPCM_HEADER *)ADPCMBufL )->wSamplesPerBlock ), &( FileData.wfx.wSamplesPerBlock )); 

   (( IMA_ADPCM_HEADER*)FileData.pChar )->wBitsPerSample = 0x0400;//4-bits
   (( IMA_ADPCM_HEADER*)FileData.pChar )->cbSize = 0x0200;

   (( IMA_ADPCM_HEADER *)FileData.pChar )->factID[ 0 ] = 'f';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->factID[ 1 ] = 'a'; 
   (( IMA_ADPCM_HEADER *)FileData.pChar )->factID[ 2 ] = 'c'; 
   (( IMA_ADPCM_HEADER *)FileData.pChar )->factID[ 3 ] = 't'; 
   (( IMA_ADPCM_HEADER *)FileData.pChar )->factSize = 0x04000000; //4Bytes
   (( IMA_ADPCM_HEADER *)FileData.pChar )->dwSampleLength = ((( FileData.wfx.nBlockAlign )-( 4 * FileData.wfx.nChannels ))* 8 )\
                                                               /( 4 * FileData.wfx.nChannels )+ 1;

   (( IMA_ADPCM_HEADER *)FileData.pChar )->dataID[ 0 ] = 'd';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->dataID[ 1 ] = 'a';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->dataID[ 2 ] = 't';
   (( IMA_ADPCM_HEADER *)FileData.pChar )->dataID[ 3 ] = 'a';

   (( IMA_ADPCM_HEADER *)FileData.pChar )->dataSize = 0;//FileData.TotalRecFileSz - 0x3C;  //Set later

   //return ( 0x3C );
}

//=======================================================================
// Function name: PCM_MakeHeader ()
// Input  : PCM information structure:wfx
// Output : none.
// Return : Size of header
// Description: To create 16-Bits PCM Header at DAC buffer left
//======================================================================= 
void PCM_MakeHeader( void )
{
   //Setting wave file header information in ADC buffer Left
   // Set  16-Bits PCM wave header //
   FileData.pChar = ( UCHAR volatile xdata *)sbufL_ADPCM;   // at 0xB800
   // RIFF header
   (( PCM_HEADER *)FileData.pChar )->riffID[ 0 ] = 'R';   // Big-Endian
   (( PCM_HEADER *)FileData.pChar )->riffID[ 1 ] = 'I';
   (( PCM_HEADER *)FileData.pChar )->riffID[ 2 ] = 'F';
   (( PCM_HEADER *)FileData.pChar )->riffID[ 3 ] = 'F';

   (( PCM_HEADER *)FileData.pChar )->riffSize = 0;//FileData.TotalRecFileSz - 8;  // setting "zero" or one "ClusrerSize" ?

   (( PCM_HEADER *)FileData.pChar )->riffFormType[ 0 ] = 'W';
   (( PCM_HEADER *)FileData.pChar )->riffFormType[ 1 ] = 'A'; 
   (( PCM_HEADER *)FileData.pChar )->riffFormType[ 2 ] = 'V'; 
   (( PCM_HEADER *)FileData.pChar )->riffFormType[ 3 ] = 'E';   

   // IMA ADPCM WAVE format chunk
   (( PCM_HEADER *)FileData.pChar )->fmtID[ 0 ] = 'f';
   (( PCM_HEADER *)FileData.pChar )->fmtID[ 1 ] = 'm';    
   (( PCM_HEADER *)FileData.pChar )->fmtID[ 2 ] = 't';   
   (( PCM_HEADER *)FileData.pChar )->fmtID[ 3 ] = ' ';      
   (( PCM_HEADER *)FileData.pChar )->fmtSize = 0x10000000;  //16Bytes  --> litte endian is low-bit is at 1'st bit .. 

   (( PCM_HEADER *)FileData.pChar )->wFormatTag = 0x0100;   // PCM
   FileData.wfx.nAvgBytesPerSec = FileData.wfx.nSamplesPerSec * FileData.wfx.nChannels * 2;  
   FileData.wfx.nBlockAlign = FileData.wfx.nChannels * 2;
   //Firstly,convert big endian into little endian then set value 
   ENDIAN_ASSIGN_2(&((( PCM_HEADER *)ADPCMBufL )->nChannels ), &( FileData.wfx.nChannels ));
   ENDIAN_ASSIGN_4(&((( PCM_HEADER *)ADPCMBufL )->nSamplesPerSec ), &( FileData.wfx.nSamplesPerSec ));   
   ENDIAN_ASSIGN_4(&((( PCM_HEADER *)ADPCMBufL )->nAvgBytesPerSec ), &( FileData.wfx.nAvgBytesPerSec ));  
   ENDIAN_ASSIGN_2(&((( PCM_HEADER *)ADPCMBufL )->nBlockAlign ), &( FileData.wfx.nBlockAlign )); 

   (( PCM_HEADER *)FileData.pChar )->wBitsPerSample = 0x1000;  //16-bits

   (( PCM_HEADER *)FileData.pChar )->dataID[ 0 ] = 'd';
   (( PCM_HEADER *)FileData.pChar )->dataID[ 1 ] = 'a';
   (( PCM_HEADER *)FileData.pChar )->dataID[ 2 ] = 't';
   (( PCM_HEADER *)FileData.pChar )->dataID[ 3 ] = 'a';

   (( PCM_HEADER *)FileData.pChar )->dataSize = 0;//FileData.TotalRecFileSz - 0x2C;  //Set later

   //return ( 0x2C );
}
#endif


#pragma CODE = RECORDER_TEMP_CODE00

//! Initialise the stream for the record mode
//! 
//! @param  nav_id         ID of navigator to record
//! @param  source         source to record (AUDIO_IN_MIC_SOURCE , AUDIO_IN_LINE_SOURCE)
//! @param  u8_type_codec  type of codec to record ( CODEC_REC_WAV , CODEC_REC_G726 , CODEC_REC_G729 )
//!
Bool record_on( void )
{
   __X FS_ERR fserr;
   unsigned int i;

   op_disable();
   u32_op_delay_off_time = 0;
   // check if no stream is on going
   if( record_stream_is_on() )
      return FALSE;

   SDKLoadCode( AUADC_COMMON_CODE );
//   SDKLoadCode( AGC_Code );

   for( i = 0; i < PCM_BUF_SIZE; i++ )
   {
      ADC_Buf1[ i ] = 0;
      ADC_Buf2[ i ] = 0;
   }

   // Initial ADPCM encoder
   //Sample_Rate = WAV_SAMPLE_RATE;    // 8KHz
   WavWriteSize = 0;
   IMA_ADPCM_Encoder_Init( WAV_SAMPLE_RATE/*Sample_Rate*/ );
   
   // write 60 bytes file header
   WavWriteSize += FSWrite(( __X FCFILE *)ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr );
	/*if( fserr != FS_NO_ERROR )
		return FALSE;*/

#if( AUDIO_RECORD_BUF == DISABLE )
   NotifyError = 0;
   NotifyFlag = 0;
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

#if( AUDIO_RECORD_BUF == ENABLE )
   idx_L = 0;
   idx_R = 0;
#endif   //#if( AUDIO_RECORD_BUF == ENABLE )

   // Initial Audio ADC
   // Set Audio ADC parameters
   //AUADC_Init( MIC_In, DTU1, AUADC_SAMPLE_RATE_8K, Buffer_Full_Func );
   AUADC_Set_Buffer(( __X int *)ADC_Buf1, ( __X int *)ADC_Buf2, PCM_BUF_SIZE );

   //AUADC_MicGain( MIC_Gain_20dB );
   //AUADC_InputGain( AUADC_Gain_6dB );

   return TRUE;
}


#pragma CODE = RECORDER_TEMP_CODE01

//! Start the stream for the record mode
//! 
Bool record_start( void )
{
   // check if no stream is on going
   //if( record_stream_is_on() )
   //   return FALSE;

#if 0//( AUDIO_RECORD_AGC == ENABLE )
   // AGC Initial
   SDKLoadCode( AGC_Init_Code );
   AGC_Init( PCM_BUF_SIZE );
   AGC_Sensitive( WAV_AGC );
#endif   //#if( AUDIO_RECORD_AGC == ENABLE )
   Frame_Cnt = 0ul;
   
   if( AUADC_Start() != AUADC_NO_ERR )
      return FALSE;

   recorder_id_stream = 0;
   
   s_u32_viewer_timeout = timer_get_time();
   s_u32_reocrder_time = 0;
   g_recorder_state = MMI_RECORDER_STATE_RECORDING;

   recorder_task_start();  // Enable event of recorder task
   return TRUE;
}


#pragma CODE = RECORDER_TEMP_CODE03

//! Stop the player
//!
Bool record_off( void )
{
   __asm("BCLR MICICR.0");
   record_stream_stop();
   //audio_codec_off();
   //audio_record_off();
   //varAl.Run_status = SysStopMusic;
   recorder_task_stop();     // Disable event of recorder task
   power_authorize_autopoweroff();
   g_recorder_state = MMI_RECORDER_STATE_STOP;
   return TRUE;
}

#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))


#if( AP3 == ENABLE )
//! Return the current time of sound playing
//!
//! @return the time  MSB(U16) = U8 minutes and LSB(U16) = U8 secondes
//!
#if 0
U16 record_gettime( void )
{
   U16 u16_total_secondes;
   U16 u16_minutes_secondes;

   u16_total_secondes = (timer_get_time() - s_u32_viewer_timeout) * TICK_PERIOD /1000;
   MSB(u16_minutes_secondes) = u16_total_secondes /60; // minutes
   LSB(u16_minutes_secondes) = u16_total_secondes - (60 * MSB(u16_minutes_secondes));
   return u16_minutes_secondes;
}
#else


#pragma CODE = RECORDER_TEMP_CODE04

U32 record_gettime( void )
{
   U32 u16_total_secondes;
   U32 u32_minutes_secondes;

   //u16_total_secondes = (timer_get_time() - s_u32_viewer_timeout) * TICK_PERIOD /1000;
   if( g_recorder_state == MMI_RECORDER_STATE_RECORDING )
   {
      u16_total_secondes = ( timer_get_time() - s_u32_viewer_timeout ) + s_u32_reocrder_time;
   }
   else
   {
      u16_total_secondes = s_u32_reocrder_time;
   }
   u16_total_secondes = u16_total_secondes * TICK_PERIOD /1000;

   MSB0( u32_minutes_secondes ) = 0;
   MSB1( u32_minutes_secondes ) = u16_total_secondes / 3600;   // hours
   u16_total_secondes %= 3600;
   MSB2( u32_minutes_secondes ) = u16_total_secondes / 60;     // minutes
   MSB3( u32_minutes_secondes ) = u16_total_secondes % 60;
   return u32_minutes_secondes;
}
#endif


#pragma CODE = RECORDER_TEMP_CODE06

//! ReStart the stream for the record mode
//! 
void record_restart( void )
{
   __asm("BSET MICICR.0");
   g_recorder_state = MMI_RECORDER_STATE_RECORDING;
   s_u32_viewer_timeout = timer_get_time();
}


#pragma CODE = RECORDER_TEMP_CODE07

//! Pause the stream for the record mode
//! 
void record_pause( void )
{
   __asm("BCLR MICICR.0");
   g_recorder_state = MMI_RECORDER_STATE_PAUSE;
   s_u32_reocrder_time += timer_get_time()- s_u32_viewer_timeout;
}


#endif   //#if( AP3 == ENABLE )

#if(( AP3 == ENABLE )||( PASTER == ENABLE ))
//******************************* STREAM FONCTIONS *****************************************

//#pragma CODE = RECORDER_CODE
#pragma CODE = RECORDER_TEMP_CODE00

//! Check if no stream is on going
//!
Bool  record_stream_is_on( void )
{
   //recorder_g_status = RECORDER_ERR_STREAM_ONGOING;
   return (ID_STREAM_ERR != recorder_id_stream);
}

#if 0
//! Initialise the stream record
//!
Bool  record_stream_init( void )
{
   // start the stream
#if(( AP3 == DISABLE )&&(PASTER == DISABLE ))
   if( file_open(FOPEN_MODE_W))  // open the current file or the next correct file
//***********************************************
#else
   //change by wangwei
   if( file_open( g_u8_record_open_type ))
#endif
//***********************************************
   {
      g_recorder_seg.u16_size = RECORD_SEG_STEP;  // Define size of segment to save
      if( file_write( &g_recorder_seg )) // get the segment data to write
         return TRUE; // Initialise OK
      // error in start play file then close this one
      file_close();
      record_result = RECORD_FILE_ERROR;
   }
   recorder_g_status = RECORDER_ERR_ACCESS_FILE;
   return FALSE;
}
#endif


#pragma CODE = RECORDER_TEMP_CODE03

//! Stop the stream record
//!
void record_stream_stop( void )
{
   __X FS_ERR fserr;

   if( ID_STREAM_ERR != recorder_id_stream )
   {
      recorder_id_stream = ID_STREAM_ERR; // reinit value with no stream
      AUADC_Stop();

      //record_process();
   }
   //fserr = FSResize(( __X FCFILE *)ADPCM_File, WavWriteSize );

   if( reread_record_flag )
      reread_record_flag = FALSE;
   else
   {
      SDKLoadCode( KB_RESIZE_CODE );
      fserr = kb_fat_resize(( __X FCFILE *)ADPCM_File, WavWriteSize );
      /*SendString((U8 *)"fserr=");
      send_hex_u8( fserr );
      SendString((U8 *)"\r\n");*/
   }

   FSSeek(( __X FCFILE *)ADPCM_File, 0 );

   FSWrite(( __X FCFILE *)ADPCM_File, ( void *)ADPCM_File_Header, 60, &fserr );

   FSFlush(( __X FCFILE *)ADPCM_File );
   
   fserr = FSClose(( __X FCFILE *)ADPCM_File );
///*
   SendString((U8 *)"\r\nfserr=");
   send_hex_u8( fserr );

// */
}

#if 0
//! control the state of stream record
//!
//! @return TRUE = Running , FALSE = Finish 
//!
Bool record_stream_ctrl( U8 _MEM_TYPE_SLOW_ *ram )
{
   U8 nav_id_save;
   U8 i;

   // Try to open the next stream of same file
   nav_id_save = nav_get();   // Get current navigator
   nav_select( recorder_id_nav );
   //if( CTRL_GOOD == state_stream )
   {
      if( 0 == g_recorder_seg.u16_size )
      {
         g_recorder_seg.u16_size = RECORD_SEG_STEP;  // Define size of segment to save
         
         if( !file_write( &g_recorder_seg )) // get the segment data to write
         {
           g_recorder_seg.u16_size = 0;  // Error disk full
           nav_select( nav_id_save );
           return FALSE;  // End of stream
         }
      }
      if( 0 != g_recorder_seg.u16_size )
      {
         // Start new transfer data
         recorder_id_stream = 0;
         for( i = 0; i < 4; i++ )
         {
            if( CTRL_GOOD != ram_2_memory( g_recorder_seg.u8_lun, g_recorder_seg.u32_addr, ram ))
            {
               fs_g_status = FS_ERR_HW;
               nav_select( nav_id_save );
               return FALSE;
            }
            g_recorder_seg.u32_addr++;
            g_recorder_seg.u16_size--;
            ram += FS_512B;
         }
      }
   }
   nav_select( nav_id_save );
   return TRUE;   // stream on going
}
#endif
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))

#endif // AUDIO_RECORD == ENABLE

#endif	// _recorder_c_
