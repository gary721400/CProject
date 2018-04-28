/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Uninit.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE03
//------------------------------------------------------------------------------
tAudioErr AudioMP3Uninit_(void)
{
    //DAC_Pause();
    mp3_rst();
	Audio_Handle.Status = UNINIT_STATUS;
	return AUDIO_ERROR_OK;    
}