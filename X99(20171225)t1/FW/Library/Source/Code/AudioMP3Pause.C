/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Pause.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3Pause_( void )
{
    int i;
    
    DAC_Pause();
	for(i=0;i<32;i++)
        Audio_Handle.MP3SpectrumData[i] = 0;

    backup_status_ = Audio_Handle.Status;
    Audio_Handle.Status = PAUSE_STATUS;
    return AUDIO_ERROR_OK;
}
