/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3FwRwd.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
static INT32U __X FwRwd_playtime_;
static INT32U __X FwRwd_totaltime_;
//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3FwRwdProlog_( void )
{
    tAudioErr AudioErr;
    
    if(Audio_Handle.Status == A_STAGE_STATUS || 
       Audio_Handle.Status == AB_REPEAT_STATUS)
    {
    	Audio_Handle.Status = PLAYING_STATUS;
    }
    if(Audio_Handle.Status != PLAYING_STATUS &&
       Audio_Handle.Status != PAUSE_STATUS)
    {
    	return AUDIO_ERROR_FW_RWD;
    }
    backup_status_ = Audio_Handle.Status;
    FwRwd_playtime_ = MP3_Information.dwTimePosition;
    FwRwd_totaltime_ = MP3_Information.dwTimeLength;
    AudioErr = AudioMP3Pause();
    Audio_Handle.Status = FWRWD_PROLOG_STATUS;
    return AudioErr;
}

//------------------------------------------------------------------------------
tAudioErr AudioMP3Forward_( void )
{
    INT32U gap_time;
    
    if(Audio_Handle.Status != FWRWD_PROLOG_STATUS && 
       Audio_Handle.Status != FORWARD_STATUS)
    {
    		return AUDIO_ERROR_FW_RWD;
    }
    gap_time = FwRwd_totaltime_/100L;
    if(gap_time < 3000)
    	gap_time = 3000;

   	Audio_Handle.Status = FORWARD_STATUS;
    if(FwRwd_playtime_ < FwRwd_totaltime_-gap_time)
        FwRwd_playtime_ += gap_time;
    else
        FwRwd_playtime_ = FwRwd_totaltime_;
    MP3_Information.dwTimePosition = FwRwd_playtime_;
    return AUDIO_ERROR_OK;
}

//------------------------------------------------------------------------------
tAudioErr AudioMP3Rewind_( void )
{
    INT32U gap_time;
    
    if(Audio_Handle.Status != FWRWD_PROLOG_STATUS && 
       Audio_Handle.Status != REWIND_STATUS)
    {
    		return AUDIO_ERROR_FW_RWD;
    }
    gap_time = FwRwd_totaltime_/100L;
    if(gap_time < 3000)
    	gap_time = 3000;
    Audio_Handle.Status = REWIND_STATUS;
    if(FwRwd_playtime_ > gap_time)
        FwRwd_playtime_ -= gap_time;
    else
        FwRwd_playtime_ = 0;
    MP3_Information.dwTimePosition = FwRwd_playtime_;
    return AUDIO_ERROR_OK;
}

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
tAudioErr AudioMP3FwRwdEpilog_( void )
{
    tAudioErr AudioErr;
    
    if(Audio_Handle.Status == REWIND_STATUS ||
       Audio_Handle.Status == FORWARD_STATUS ||
       Audio_Handle.Status == FWRWD_PROLOG_STATUS)
    {
        Audio_Handle.Status = FWRWD_EPILOG_STATUS;
        AudioErr = AudioMP3SetPlayTime(FwRwd_playtime_);
        if(AudioErr != AUDIO_ERROR_OK)
            return AudioErr;
        Audio_Handle.Status = backup_status_;
        if(backup_status_ == PLAYING_STATUS)
            AudioMP3Continue();
    	return AUDIO_ERROR_OK;
    }
    return AUDIO_ERROR_FW_RWD;
}
