/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Close.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE03
//------------------------------------------------------------------------------
tAudioErr AudioMP3Close_(void)
{
    DAC_Pause();
    if(Audio_Handle.Status == EOF_STATUS)
    	DAC_FadeOut();
//    DAC_Mute();
    if(MP3API_FSClose() != FS_NO_ERROR)
        return AUDIO_ERROR_CLOSE_FILE;
    Audio_Handle.FSHandle = 0;
    FS_Handle_ = 0;
    Audio_Handle.Status = CLOSE_STATUS;
    return AUDIO_ERROR_OK;
}
