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
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3HeadPhoneSetVol_(int *get_volume)
{
	
	set_vol_ = *get_volume;
    DAC_HeadPhone_Volume(set_vol_);
	DAC_Enable_HeadPhone();          //  Headphone output    
    return AUDIO_ERROR_OK;
}

tAudioErr AudioMP3SpeakerSetVol_(int *get_volume)
{
	
	set_vol_ = *get_volume;
	set_vol_ = set_vol_/2;
    DAC_Speaker_Volume(set_vol_);
	set_vol_ = *get_volume;
	DAC_Enable_Speaker();              //  Speaker output	
    return AUDIO_ERROR_OK;
}