//!
//! @file player_task.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: player_task.h,v 2.0 2007/03/30 12:08:20 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _PLAYER_TASK_H_
#define _PLAYER_TASK_H_

//#include "unicode.h"
#include "compiler.h"
//#include "define.h"
#include "MP3_API.H"



#define sbufL_ADPCM              (( UCHAR xdata *)0xB800)
#define ADPCMBufL                (( UCHAR xdata *)0xB800)

// For ADPCM/PCM Header information
#define WAVE_FORMAT_IMA_ADPCM    ( 0x0011 )
#define WAVE_FORMAT_PCM          ( 0x0001 )

#define _16BitsPerSample         ( 0x0010 )
#define _4BitsPerSample          ( 0x0004 )

#define CHUNKID_RIFF             0x52494646
#define WAVE_STRING              0x57415645  
#define CHUNKID_FMT              0x666d7420 
#define CHUNKID_FACT             0x66616374  
#define CHUNKID_DATA             0x64617461 

#define  REC_PCM                 WAVE_FORMAT_PCM 
#define  REC_ADPCM               WAVE_FORMAT_IMA_ADPCM


typedef struct waveformat_extended_tag
{
   USHORT   wFormatTag;       /*format type*/
   USHORT   nChannels;        /*number of channels(i.e. mono,stereo...)*/
   ULONG    nSamplesPerSec;   /*sample rate*/
   ULONG    nAvgBytesPerSec;  /*for buffer estimation*/
   USHORT   nBlockAlign;      /*block size of data*/
   USHORT   wBitsPerSample;   /*Number of bits per sample of mono data*/
   USHORT   cbSize;           /*The count in bytes of extra size*/
   USHORT   wSamplesPerBlock; /* Count of number of samples per channale per Block */

}WAVEFORMATEX;


typedef struct wave_format_chunk
{
	ULONG    ChunkID;          /*'RIFF' chunck descriptor */
	ULONG    ChunkSize;        /* Total size-8 */
	ULONG    Format;           /*'WAVE' */
	ULONG    SubChunk1ID;      /*'fmt' subchunck*/
	ULONG    SubChunk1Size;    /* Total size-16 */	
   USHORT   wFormatTag;       /*format type*/
   USHORT   nChannels;        /*number of channels(i.e. mono,stereo...)*/
   ULONG    nSamplesPerSec;   /*sample rate*/
   ULONG    nAvgBytesPerSec;  /*for buffer estimation*/
   USHORT   nBlockAlign;      /*block size of data*/
   USHORT   wBitsPerSample;   /*Number of bits per sample of mono data*/
   //USHORT   cbSize;         /*The count in bytes of extra size*/
   ULONG    SubChunk2ID;  
	ULONG    SubChunk2Size; 	

}WAVE_CHUNK;


//For Encoding WAV file Header 
typedef struct
{
   // RIFF Header
   UCHAR    riffID[ 4 ];
   ULONG    riffSize;
   UCHAR    riffFormType[ 4 ];
   // IMA ADPCM WAVE Format Chunk
   UCHAR    fmtID[ 4 ];
   ULONG    fmtSize;
    
   // WAVEFORMATEX      wfmx;
   USHORT   wFormatTag;       /*format type*/
   USHORT   nChannels;        /*number of channels(i.e. mono,stereo...)*/
   ULONG    nSamplesPerSec;   /*sample rate*/
   ULONG    nAvgBytesPerSec;  /*for buffer estimation*/
   USHORT   nBlockAlign;      /*block size of data*/
   USHORT   wBitsPerSample;   /*Number of bits per sample of mono data*/
   USHORT   cbSize;           /*The count in bytes of extra size*/
   USHORT   wSamplesPerBlock; /* Count of number of samples per channale per Block */
         
   // Fact Chunk
   UCHAR    factID[ 4 ];
   ULONG    factSize;
   ULONG    dwSampleLength;
 
   // Wave Data
   UCHAR    dataID[ 4 ];
   ULONG    dataSize;

}IMA_ADPCM_HEADER;


typedef struct
{
   // RIFF Header
   UCHAR    riffID[ 4 ];
   ULONG    riffSize;
   UCHAR    riffFormType[ 4 ];
   // IMA ADPCM WAVE Format Chunk
   UCHAR    fmtID[ 4 ];
   ULONG    fmtSize;

   // WAVEFORMATEX      wfmx;
   USHORT   wFormatTag;       /*format type*/
   USHORT   nChannels;        /*number of channels(i.e. mono,stereo...)*/
   ULONG    nSamplesPerSec;   /*sample rate*/
   ULONG    nAvgBytesPerSec;  /*for buffer estimation*/
   USHORT   nBlockAlign;      /*block size of data*/
   USHORT   wBitsPerSample;   /*Number of bits per sample of mono data*/

   // Wave Data
   UCHAR    dataID[ 4 ];
   ULONG    dataSize;

} PCM_HEADER;

typedef struct _FileData_
{

//what follows is for playing ADPCM/PCM        
   ULONG          fmtchukSz;     /*Size of 'fmt' chunk*/
   WAVEFORMATEX   wfx;           /* 'fmt' chunk structure */
   ULONG          AudioDataSz;   /* Total size of audio data */
   ULONG          DataChukoffsetBytes;
   UCHAR          *pChar;
   ULONG          *pLong; 
   ULONG          TotalTimeSec;  //Save total playback time in second.	

   //ULONG          TotalRecFileSz;   // Total Recording File Size --> To FDB size(Big Endian) 
	ULONG          TotalWriteDataSz; // Total writing data Size --> To FDB size(Little Endian)
} _FileData;

//extern _MEM_TYPE_SLOW_ _FileData FileData;


//extern   xdata U8    g_b_player_on;
extern _MEM_TYPE_SLOW_ Bool g_b_player_on;

void  player_task_init  ( void );
void  player_task_start ( void );
void  player_task_stop  ( void );
void  player_task       ( void );
//Bool  play_file_start   ( void );
int   CheckPlaying		( Bool flag );
#endif  // _PLAYER_TASK_H_ 
