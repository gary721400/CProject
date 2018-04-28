// This is KBMP3 SDK Template File. 
/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Variable.C
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#define _VARIABLE_C_
#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
// XRAM
//------------------------------------------------------------------------------
#pragma DATA = MP3_API_XRAM
//------------------------------------------------------------------------------
INT8U *pHead_;
INT8U *pTail_;
__X FCFILE *FS_Handle_;

tAudioHandle* MP3_Handle_;
INT16U MP3LibVer_;
INT16U MP3_API_Flag_; // bit 0 : mute flag
                      // bit 1 : fill catch buffer flag
                      // bit 2 : Set FILLBS_FLAG
                      // bit 3 : Can't fill catch buffer, becoure NFTL busy.
INT16S MP3_HEAP_[MP3_FRAME_SIZE];
INT16S PCM_PlayBuf_L1_[MP3_FRAME_SIZE];
INT16S PCM_PlayBuf_L2_[MP3_FRAME_SIZE];
INT16S PCM_PlayBuf_R1_[MP3_FRAME_SIZE];
INT16S PCM_PlayBuf_R2_[MP3_FRAME_SIZE];
INT32U Remain_Bitstream_len_;
INT16U free_len_;
INT32U dwMP3_FrameHeader_;
INT32U dwMP3_CompareHeader_;
//INT16U max_vol_;
INT16U set_vol_;
INT16U BitStreamEmptyCount_;
INT32U A_time_;
INT32U B_time_;
INT32U WordCntTmpBak_;
tMusicStatus backup_status_;
MP3_INFORMATION MP3_Information;
VBR_INFORMATION VBR_Information;
