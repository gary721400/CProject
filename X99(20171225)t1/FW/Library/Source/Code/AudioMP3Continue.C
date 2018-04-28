/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Continue.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3Continue_( void )
{
    DAC_Continue();
		DAC_Go();     
	if(backup_status_ == AB_REPEAT_STATUS ||
        backup_status_ == A_STAGE_STATUS)
    {
	    Audio_Handle.Status = backup_status_;
	}
    else
    {
	    Audio_Handle.Status = PLAYING_STATUS;
    }
    return AUDIO_ERROR_OK;
}
