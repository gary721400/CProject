/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3SetEQ.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3SetEQ_(tAudioEQ_Info EQ_Info,
                         tAudioEQ EQ_num)
{
    if(EQ_num >= EQ_Info.EQ_Max || EQ_num < EQ_USER)
    {
    	  if(EQ_num == EQ_NOMAL)
    		{
        	mp3_DisableEQ();
        	return AUDIO_ERROR_OK;
    		}
        return AUDIO_ERROR_PARAMETER;
    }
    
    if(EQ_Info.EQ_Band > 32 || EQ_Info.EQ_Band < 1)
        return AUDIO_ERROR_PARAMETER;

    if(EQ_num == EQ_USER)
        mp3_SetEQ((unsigned int *) &EQ_Info.EQ_UserTBL, EQ_Info.EQ_Band);
    else
       mp3_SetEQ((unsigned int *) &EQ_Info.EQ_ConstTBL[EQ_num], EQ_Info.EQ_Band);
	    
	mp3_EnableEQ();
	return AUDIO_ERROR_OK;
}
