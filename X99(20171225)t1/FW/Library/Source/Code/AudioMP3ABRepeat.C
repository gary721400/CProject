/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3ABRepeat.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3ABRepeat_(void)
{
    if(Audio_Handle.Status == PLAYING_STATUS)
    {
        A_time_ = MP3_Information.dwTimePosition;
        Audio_Handle.Status = A_STAGE_STATUS;
    }
    else if(Audio_Handle.Status == A_STAGE_STATUS)
    {
        B_time_ = MP3_Information.dwTimePosition;
        if(B_time_ < A_time_+ 261)
        {
            Audio_Handle.Status = PLAYING_STATUS;
            return AUDIO_ERROR_OK;
        }
        Audio_Handle.Status = AB_REPEAT_STATUS;
        AudioMP3SetPlayTime(A_time_);
    }
    else if(Audio_Handle.Status == AB_REPEAT_STATUS)
    {
        Audio_Handle.Status = PLAYING_STATUS;
    }
    else if(Audio_Handle.Status == PAUSE_STATUS &&
            backup_status_ == AB_REPEAT_STATUS)
    {
        backup_status_ = PAUSE_STATUS;
    }
    return AUDIO_ERROR_OK;
}
