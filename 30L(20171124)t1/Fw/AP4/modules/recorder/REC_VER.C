/*
 * main.c
 *
 * Description: 
 *
 * Author: Nick Lee <linin@kingb.com.tw>
 * Copyright(c)2010 King Billion electronics Co., Ltd.<http://www.kingb.com.tw/>
 * All rights reserved.
 *
 * Modifications:
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <KBDM9_DDK.H>
#include <string.h>
#include <REC_API.H>

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma DATA = WAV_REC_DATA
//int PCM_CACHE_Buf[PCM_BUF_SIZE];
//int ADPCM_Buf[ADPCM_BUF_SIZE];
//int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
//int PCM_Buf1[PCM_BUF_SIZE];
//int PCM_Buf2[PCM_BUF_SIZE];
int NotifyFlag, NotifyError;
//int *NotifyBuf;

int idx_L,idx_R;
unsigned long WavWriteSize;
FS_ERR REC_fserr;
//FCFILE *ADPCM_File;   
tWAV_REC_State WavRecState;
unsigned long  Frame_Count;
unsigned int PCM_Buffer_Size, ADPCM_Buffer_Size;

/* Private functions ---------------------------------------------------------*/
/* End of file ---------------------------------------------------------------*/