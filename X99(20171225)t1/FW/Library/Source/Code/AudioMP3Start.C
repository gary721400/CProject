/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Start.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE02
//------------------------------------------------------------------------------
//extern void __interrupt AudioMP3_Left_ISR(void);
void Clear_Buf(void);
INT16S MP3API_FirstDecode(void);
//------------------------------------------------------------------------------
tAudioErr AudioMP3Start_( void )
{
//	LoadMP3PlayCode();
	if(MP3API_FirstDecode())
	{
	    pMP3API_FSClose(Audio_Handle.FSHandle);
	    Audio_Handle.FSHandle = 0;
	    return AUDIO_ERROR_MP3DECODE;
	}
	Fill_MP3CatchBuffer();
	//DAC_FadeIn();
	DAC_Go();
	if(CHECK_API_FLAG(MUTE_FLAG))
		DAC_Mute();

    Audio_Handle.Status = PLAYING_STATUS;
    return AUDIO_ERROR_OK;
}

//------------------------------------------------------------------------------
const unsigned int SampeRate[]={DAC_SAMPLE_RATE_8K,
                                DAC_SAMPLE_RATE_16K,
                                DAC_SAMPLE_RATE_32K,
                                DAC_SAMPLE_RATE_11P025K,
                                DAC_SAMPLE_RATE_22P05K,
                                DAC_SAMPLE_RATE_44P1K,
                                DAC_SAMPLE_RATE_12K,
                                DAC_SAMPLE_RATE_24K,
                                DAC_SAMPLE_RATE_48K};

//------------------------------------------------------------------------------
INT16S MP3API_FirstDecode(void)
{
	INT16U bs_len;
    INT16U OkCnt;
    INT16U i;
    void *LeftBuf;
    void *RightBuf;
    
	OkCnt=0;
	Clear_Buf();
	do {	// This loop is for searching the header of the MP3 bitstream
	 	mp3ErrorCnt=0;
	    if(Fill_MP3BitStream())
	        return -1;
	  mp3Clear();      
		mp3dec();
  	mp3_GetPlayBufPtr(&LeftBuf, &RightBuf);			
	    if(mp3ErrorCnt!=0){ 
	        OkCnt = 0;
	    }    
	  	else{
	        OkCnt++;
	    }    
    // make sure that mp3dec has audio data to play
	} while( !(mp3Flag & 0x8000) || OkCnt<1);
//	Fill_MP3BitStream();

    mp3StereoMode = (mp3Flag&0x4000) >> 14;	// Get stereo status
    if(mp3SmpID>=0 && mp3SmpID<=9)
        DAC_SampleRate = SampeRate[mp3SmpID-1];
	else
	    return -1;
      
    if(mp3StereoMode)
    {
		DAC_LChannelBuffer1 = (__X int *)LeftBuf;
		DAC_RChannelBuffer1 = (__X int *)RightBuf;
	}
    else
    {
		DAC_LChannelBuffer1 = (__X int *)LeftBuf;
		DAC_RChannelBuffer1 = (__X int *)LeftBuf;
	}
	DAC_FillBuffer1();
	DAC_FrameSize = MP3_FRAME_SIZE;
	return 0;
}

//------------------------------------------------------------------------------
void Clear_Buf(void)
{
    memset(PCM_PlayBuf_L1_,0,sizeof(INT16S)*MP3_FRAME_SIZE);
    memset(PCM_PlayBuf_R1_,0,sizeof(INT16S)*MP3_FRAME_SIZE);
    memset(PCM_PlayBuf_L2_,0,sizeof(INT16S)*MP3_FRAME_SIZE);
    memset(PCM_PlayBuf_R2_,0,sizeof(INT16S)*MP3_FRAME_SIZE);
    memset(MP3_HEAP_,0,sizeof(INT16S)*MP3_FRAME_SIZE);
}
