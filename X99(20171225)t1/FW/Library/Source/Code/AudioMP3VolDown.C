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
tAudioErr AudioMP3HeadPhoneVolDown_(int *get_volume)
{
	unsigned int Vol;	
	Vol = *get_volume;
	
    if(Vol > 0)
    {
        Vol--;       
    }    
  
  	if (Vol == 0)
    {
      	if(!CHECK_API_FLAG(MUTE_FLAG))
      	{
	        SET_API_FLAG(MUTE_FLAG);
//	  			DAC_Mute();   
  			}     
    }
        
   	DAC_HeadPhone_Volume(Vol);  
       		        	
    *get_volume = Vol;
    return AUDIO_ERROR_OK;
}

tAudioErr AudioMP3SpeakerVolDown_(int *get_volume)
{
	unsigned int Vol;	
	Vol = *get_volume;
			
    if(Vol > 0)
    {
        Vol--;       
    }    
    *get_volume = Vol;
        
  	Vol = Vol/2;
  	
  	if (Vol == 0)
    {
      	if(!CHECK_API_FLAG(MUTE_FLAG))
      	{
	        SET_API_FLAG(MUTE_FLAG);
//	  			DAC_Mute();   
  			}     
    }
        
   	DAC_Speaker_Volume(Vol);  
//	set_vol_ = *get_volume;       		        	
//    *get_volume = set_vol_;
    return AUDIO_ERROR_OK;
}