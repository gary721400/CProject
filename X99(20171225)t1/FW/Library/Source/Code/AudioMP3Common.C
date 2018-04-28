/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Common.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

//------------------------------------------------------------------------------
extern void MP3_API_Decode(void);
extern void Calculate_Time(unsigned int MP3WordCount);

//------------------------------------------------------------------------------
#pragma CODE = MP3_COMMON_CODE
//------------------------------------------------------------------------------
//extern void SysClkUpgrade(void);
//extern void SysClkDowngrade(void);

//------------------------------------------------------------------------------
void Fill_MP3CatchBuffer(void)
{
    INT16U count;
    INT16U Difference;
    INT32U  length_temp;
    FS_ERR fserr;
    
    //SysClkUpgrade();
    if(CHECK_API_FLAG(FILL_FLAG))      // Á×§Kre-entry
        goto RE_ENTRY_EXIT;
    SET_API_FLAG(FILL_FLAG);
    if(mp3VolumeCtrl < MP3_MAX_VOLUME)
        mp3VolumeCtrl+= 0x11;
    Difference = (INT16U)(mp3WordCount - WordCntTmpBak_);
    WordCntTmpBak_ = mp3WordCount;
    Calculate_Time(Difference);
    if(IsFileSystemBusy(0))
    {
        SET_API_FLAG(BUSY_FLAG);
        pStorageBusyCallBackFunc = (tStorageBusyCallBackFunc)&Fill_MP3CatchBuffer;
        goto EXIT;
    }
    CLR_API_FLAG(BUSY_FLAG);
    count = free_len_>>8;
    while ((count--)>0 && Remain_Bitstream_len_>0)
    {
        int read_size;
        
        if(pTail_>=(END_catch_Buffer_))
            pTail_ = catch_Buffer_;
        if(Remain_Bitstream_len_ < 512)
            read_size = (int)Remain_Bitstream_len_;
        else
            read_size = 512;
        length_temp = MP3API_FSRead((void*) pTail_,
                                     read_size,
                                     &fserr);
        if(fserr != 0)
        {
            MP3_Handle_->Status = ERROR_STATUS;
            break;
        }
        else if(length_temp == 0)
        {
            Remain_Bitstream_len_ = 0;
            break;
        }
        
        Remain_Bitstream_len_ -= length_temp;
        free_len_=free_len_-(INT16U)(length_temp>>1);
        pTail_=pTail_+length_temp;
    }
EXIT:
    CLR_API_FLAG(FILL_FLAG);
RE_ENTRY_EXIT:
    //SysClkDowngrade();
    return;
}

//------------------------------------------------------------------------------
void MP3API_EndPlay(void)
{
    __asm{
        BCLR CODECCON.12
        BCLR CODECCON.14
        BCLR DACLICR.0
        BCLR DACRICR.0
    }
    __asm("BCLR SOCR.3\n");
    mp3FrameCount = 0;
    MP3_Information.dwTimePosition = 
        (INT32U)(mp3FrameCount*MP3_Information.wFrameDuration)/10;
    MP3_Handle_->Status = EOF_STATUS;
	CLR_API_FLAG(FILL_FLAG);
}
        
//------------------------------------------------------------------------------
void FillPCMChannel1Buffer(void)
{
    void *LeftBuf, *RightBuf;
    void *buf_temp;

    //SysClkUpgrade();
    mp3Clear();
    MP3_API_Decode();    
    mp3_GetPlayBufPtr(&LeftBuf, &RightBuf);
    __asm("BCLR SOCR.3\n");
    if(mp3StereoMode)
        buf_temp = RightBuf;
    else
        buf_temp = LeftBuf;
    if(DAC_Buffer1Empty())
    {
		DAC_LChannelBuffer1 = (__X int *)LeftBuf;
		DAC_RChannelBuffer1 = (__X int *)buf_temp;
        DAC_FillBuffer1();
    }
    else if(DAC_Buffer2Empty())
    {
		DAC_LChannelBuffer2 = (__X int *)LeftBuf;
		DAC_RChannelBuffer2 = (__X int *)buf_temp;
        DAC_FillBuffer2();
    }    
    else
      return;
        //goto Exit;

    Fill_MP3CatchBuffer();
//Exit:
    //SysClkDowngrade();
}

//------------------------------------------------------------------------------
void MP3_DTUErrorOut(void)
{
    __asm("BCLR SOCR.3\n");
}
