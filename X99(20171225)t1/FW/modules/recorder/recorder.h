//!
//! @file recorder.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the record task.
//!
//! @version 1.1 snd3-dvk-1_9_5 $Id: recorder.h,v 1.1 2006/12/19 13:13:33 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _RECORD_H_
#define _RECORD_H_

//_____ I N C L U D E S ____________________________________________________

#include "modules\file_system\fs_com.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "mp3_internal_include.h"
#include "modules\player\player.h"


//_____ D E F I N I T I O N ________________________________________________
#undef _GLOBEXT_
#if (defined _recorder_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


// Definition of the recorder functionnality states 
typedef enum
{
   MMI_RECORDER_STATE_STOP = 0
,  MMI_RECORDER_STATE_RECORDING
,  MMI_RECORDER_STATE_PAUSE
}Recorder_states;

_GLOBEXT_   _MEM_TYPE_SLOW_   Recorder_states   g_recorder_state;

//----------------
// Streams Control
_GLOBEXT_   _MEM_TYPE_SLOW_   U8                recorder_id_stream;  // ID of stream running, by default no stream
// End streams Control


#define  MAX_LENGTH           ( 2ul *1024ul * 1024ul )
#define  MIN_DISK_SPACE       ( 1ul * 1024ul * 1024ul )
#define  BEGIN_RECORD_SPACE   ( 2ul * 1024ul * 1024ul )

//! @defgroup Recorder status type
//! These definitions are all the possible status values returned by the recorder
//! @{
typedef  U8                 Recorder_status;    //!< type of recorder status
#define  RECORDER_ERR_STREAM_ONGOING   FAIL+0   //!< A stream is on going
#define  RECORDER_ERR_LOAD_CODEC       FAIL+2   //!< Error during the codec loading
#define  RECORDER_ERR_ACCESS_FILE      FAIL+4   //!< Error during the access file
//! @}

#define  RECORD_NO_EVENT             0  // There aren't event
#define  RECORD_EOF                  1  // End of file, the record is stopped

//_____ D E C L A R A T I O N ______________________________________________

#define  AUDIO_RECORD_BUF           ENABLE
//#define  AUDIO_RECORD_BUF           DISABLE

//#define  AUDIO_RECORD_AGC           ENABLE
#define  AUDIO_RECORD_AGC           DISABLE

//_GLOBEXT_ _MEM_TYPE_SLOW_ Recorder_status   recorder_g_status;

#define  PCM_BUF_SIZE         505                              // It can only be 505     
#define  ADPCM_BUF_SIZE       (( PCM_BUF_SIZE - 1 )/ 4 + 2 )   // word

#if 0
extern   __X   int      PCM_Buf_1[ PCM_BUF_SIZE ];
extern   __X   int      PCM_Buf_2[ PCM_BUF_SIZE ];
#endif

#define  ADC_Buf1       (( __X int *)PCM_Buf_1 )
#define  ADC_Buf2       (( __X int *)PCM_Buf_2 )

//_GLOBEXT_   _MEM_TYPE_SLOW_   U32   free_space;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   WavWriteSize;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   WavStepSize;

#if( AUDIO_RECORD_BUF == DISABLE )
#if 0
extern   __X   int      PCM_Buf[ PCM_BUF_SIZE ];
extern   __X   int      ADPCM_Buf[ ADPCM_BUF_SIZE ];
#else
#define  PCM_Buf        PCM_PlayBuf_R2_
#endif

extern         int      NotifyFlag;
extern         int      NotifyError;
extern   __X   int      *NotifyBuf;
#endif   //#if( AUDIO_RECORD_BUF == DISABLE )

#if 1//( AUDIO_RECORD_AGC == ENABLE )
extern         unsigned long  Frame_Cnt;
#endif   //#if( AUDIO_RECORD_AGC == ENABLE )

#if( AUDIO_RECORD_BUF == ENABLE )
#define  ADPCM_BUF_CNT           4//6//

#if 0
extern   __X   int      REC_ADPCM_Buf[ ADPCM_BUF_CNT ][ ADPCM_BUF_SIZE ];
#else
#define  REC_ADPCM_Buf     (( __X int(*)[ ADPCM_BUF_SIZE ])PCM_PlayBuf_R2_ )
#endif

extern   __X   int      idx_L, idx_R;

#endif   //#if( AUDIO_RECORD_BUF == ENABLE )


extern         unsigned int   Sample_Rate;
//extern   __X   FCFILE   *ADPCM_File;

void  Buffer_Full_Func  ( __X int *buffer, unsigned int buf_size );

//! Functions of record
Bool record_on       ( void );
Bool record_start    ( void );
Bool record_off      ( void );

//U16  record_gettime  ( void );
U32  record_gettime  ( void );
U8   record_evt      ( void );

Bool record_stream_ctrl ( U8 _MEM_TYPE_SLOW_ *ram );

void record_restart  ( void );
void record_pause    ( void );

#endif  // _RECORD_H_ 
