/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: mp3_internal_include.h
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/
/////////////////////////////////////////////////////////////////////////
#ifndef _MP3_INTERNAL_INCLUDE_H
#define _MP3_INTERNAL_INCLUDE_H
/////////////////////////////////////////////////////////////////////////
#define OPEN_DEF   1
#define CLOSE_DEF  0

#define MP3_LIB_VERSION 0x8012
/////////////////////////////////////////////////////////////////////////
#include <KBDM9_DDK.H>
#include "MP3_API.H"
#include "FS.H"
#include "string.h"
#include "SFR.H"
#include "mp3_include.h"

/////////////////////////////////////////////////////////////////////////
#define MP3_FRAME_SIZE  576     // WORD, buffer size
#define BS_BUF_SZ       722     // WORD, 必須與mp3PlayFAT.asm的BS_BUF_SZ相同
#define CATCH_BUF_SZ    (256*4) // WORD, 必須與mp3PlayFAT.asm的CATCH_BUF_SZ相同
#define MP3_MAX_VOLUME  0xDD    // MP3 decode volume
#define PCM_FADEOUT_TIME 500    // Fade out time
#define ID3v1SIZE       128
#define CHECK_FRAME_DATA_MASK 0xFFFE0C0FUL // Search MP3 frame header mask
                                           // 用來判別正確frame header
#define DHA_LCH_TRAP_NO 0x30
// API flag bit-mask define
#define MUTE_FLAG   0 // bit 0
#define FILL_FLAG   1 // bit 1
#define FILLBS_FLAG 2 // bit 2
#define BUSY_FLAG   3 // bit 3

#define CHECK_API_FLAG(x1) ((MP3_API_Flag_)&(0x0001u<<(x1)))
#define SET_API_FLAG(x1)   MP3_API_Flag_ |= (0x0001u<<(x1))
#define CLR_API_FLAG(x1)   MP3_API_Flag_ &= (~(0x0001u<<(x1)))

extern __X tAudioHandle Audio_Handle;
/////////////////////////////////////////////////////////////////////////
#ifndef _VARIABLE_C_
extern INT16U MP3_CalculateFlag_;

extern __X INT8U * pHead_;
extern __X INT8U * pTail_;
extern __X FCFILE * __X FS_Handle_;

extern ptAudioHandle MP3_Handle_;
extern INT16S __X MP3_HEAP_[MP3_FRAME_SIZE];
extern INT16S __X PCM_PlayBuf_L1_[MP3_FRAME_SIZE];
extern INT16S __X PCM_PlayBuf_L2_[MP3_FRAME_SIZE];
extern INT16S __X PCM_PlayBuf_R1_[MP3_FRAME_SIZE];
extern INT16S __X PCM_PlayBuf_R2_[MP3_FRAME_SIZE];
extern INT16S __X bitstream_buf_[];
extern INT32U __X Remain_Bitstream_len_;
extern INT8U __X catch_Buffer_[];
extern INT8U __X END_catch_Buffer_[];
extern INT16U __X free_len_;
//extern tPCM_ChannelHandle __X PCM_CH1_;
extern tID3v1TagStr __X ID3v1_data_;
extern INT32U __X dwMP3_FrameHeader_;
extern INT32U __X dwMP3_CompareHeader_;
//extern INT16U __X max_vol_;
extern INT16U __X min_vol_;
extern INT16U __X set_vol_;
extern INT16U __X MP3_API_Flag_; // bit 0 : MUTE Flag
extern INT16U __X BitStreamEmptyCount_;
extern INT32U __X A_time_;
extern INT32U __X B_time_;
extern tMusicStatus __X backup_status_;
extern INT32U __X WordCntTmpBak_;
extern INT16U __X MP3LibVer_;
extern int MP3CallBackExtendedRAM[];
extern unsigned int eqUserTab[];
extern INT16U __X MP3LibVer_;
#endif // _VARIABLE_C_

/////////////////////////////////////////////////////////////////////////
extern void FillPCMChannel1Buffer(void);
extern int Fill_MP3BitStream(void);
extern void Fill_MP3CatchBuffer(void);
extern void MP3_DTUErrorOut(void);
extern int MP3_GetAudioHeader(void *, int );
extern int MP3_GetAudioHeader_1(void *, int );
extern int MP3_SearchAudioHeader(void *, int , int );
extern int MP3_GetVbrHeader(void *, int );
extern int MP3_ReadFrameHeader(void);

/////////////////////////////////////////////////////////////////////////
//extern tSDKLoadCodeStatus LoadMP3InitCode(void);
extern tSDKLoadCodeStatus LoadMP3PlayCode(void);
extern tSDKLoadCodeStatus LoadMP3Temp0Code(void);
extern tSDKLoadCodeStatus LoadMP3Temp1Code(void);
extern tSDKLoadCodeStatus LoadMP3Temp2Code(void);
extern tSDKLoadCodeStatus LoadMP3Temp3Code(void);

extern INT16U SysClkUpdate(void);
/////////////////////////////////////////////////////////////////////////
extern FS_ERR MP3API_FSSeek(unsigned long offset);
extern unsigned int MP3API_FSRead(__X void *buf, unsigned int len, __X FS_ERR* err);
extern unsigned int MP3API_FSClose(void);
#endif // _MP3_INTERNAL_INCLUDE_H