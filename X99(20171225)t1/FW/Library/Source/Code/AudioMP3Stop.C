/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Stop.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"
#include "sio\com.h"


//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3Stop_( void )
{
    A_time_ = B_time_ = mp3FrameCount = MP3_Information.dwTimePosition = 0;
	DAC_Pause();
	DAC_FadeOut();
//	MP3API_FSSeek(0);
   Audio_Handle.Status = STOP_STATUS;
//    LoadMP3InitCode();
	mp3_rst();
    return AUDIO_ERROR_OK;
}
