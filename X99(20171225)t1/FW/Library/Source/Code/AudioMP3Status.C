/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3VolDown.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_COMMON_CODE
//------------------------------------------------------------------------------
tMusicStatus AudioMP3Status_(void)
{
    if(Audio_Handle.Status == AB_REPEAT_STATUS && 
       MP3_Information.dwTimePosition > B_time_)
    {
    	MP3_Information.dwTimePosition = A_time_;
        AudioMP3SetPlayTime(A_time_);
    }
    return Audio_Handle.Status;
}
