/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Init.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/
#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
FCFILE* (*pMP3API_FSOpen)(char *spec,
                          unsigned int mode,
                          __X FS_ERR* err);
unsigned int (*pMP3API_FSRead)(__X FCFILE *file, 
                                __X void *buf, 
                                unsigned int len, 
                                __X FS_ERR* err);
FS_ERR (*pMP3API_FSClose)(__X FCFILE *file);
FS_ERR (*pMP3API_FSSeek)(__X FCFILE *file,
                         unsigned long offset);

//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE03
//------------------------------------------------------------------------------
tAudioErr AudioMP3Init_(INT8U default_vol)
{
    INT8U temp;

    // PCM Drive Initial
    MP3_API_Flag_ = 0;
 
    DAC_Init(DTU3,
             DTU2,
             FillPCMChannel1Buffer,
             Fill_MP3CatchBuffer);

    DAC_LChannelBuffer1 = PCM_PlayBuf_L1_;
    DAC_RChannelBuffer1 = PCM_PlayBuf_R1_;
    DAC_LChannelBuffer2 = PCM_PlayBuf_L2_;
    DAC_RChannelBuffer2 = PCM_PlayBuf_R2_;
//    set_vol_ = default_vol;
//    DAC_HeadPhone_Volume(set_vol_);
    Audio_Handle.FSHandle = 0;
    FS_Handle_ = 0;
    MP3_Handle_ = &Audio_Handle;
    Audio_Handle.Status = INIT_STATUS;

    // Initial mp3 library
    mp3_init(PCM_PlayBuf_L1_,
             PCM_PlayBuf_R1_,
             PCM_PlayBuf_L2_,
             PCM_PlayBuf_R2_,
             MP3_HEAP_,
             bitstream_buf_,
             BS_BUF_SZ);
    mp3_SetErrorFunc(MP3_DTUErrorOut);
    mp3VolumeCtrl = MP3_MAX_VOLUME; // maximum volume in Left and Right Channel
    //mp3Ctrl = 0;
//    LoadMP3Temp1Code();
    mp3_DisableEQ();
    mp3StereoMode = 1;
    MP3_CalculateFlag_ = 0;
    dwMP3_FrameHeader_ = 0;
    dwMP3_CompareHeader_ = 0;
    BitStreamEmptyCount_ = 0;
    WordCntTmpBak_ = 0;
    MP3LibVer_ = MP3_LIB_VERSION;
    memset(catch_Buffer_,0,sizeof(INT16S)*CATCH_BUF_SZ);
    return AUDIO_ERROR_OK;   // PCM initial OK.
}