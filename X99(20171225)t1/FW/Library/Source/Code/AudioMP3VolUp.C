/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3VolUp.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3HeadPhoneVolUp_(int *get_volume)
{
	unsigned int Vol;	
    if(CHECK_API_FLAG(MUTE_FLAG))
    {
        CLR_API_FLAG(MUTE_FLAG);
        //PCMv2_Unmute();
        DAC_Unmute();
    }  
    
    Vol = *get_volume;      
//    if(set_vol_ < max_vol_)
        Vol++;
        
    DAC_HeadPhone_Volume(Vol);  

    *get_volume = Vol;
    return AUDIO_ERROR_OK;    
}

tAudioErr AudioMP3SpeakerVolUp_(int *get_volume)
{
	unsigned int Vol;
    if(CHECK_API_FLAG(MUTE_FLAG))
    {
        CLR_API_FLAG(MUTE_FLAG);
        //PCMv2_Unmute();
        DAC_Unmute();
    } 
    Vol = *get_volume;            
//    if(set_vol_ < max_vol_)
        Vol++;
    *get_volume = Vol;
    Vol = Vol/2;        
    DAC_Speaker_Volume(Vol);  

//    set_vol_ = *get_volume; 
    return AUDIO_ERROR_OK;    
}