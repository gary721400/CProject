/*
 * REC_API.H
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
#include "compiler.h"

/* Private define ------------------------------------------------------------*/
typedef enum {
    REC_STOP = 0,
    REC_RECODING,
    REC_PAUSE,
    REC_ERROR,
    WAV_STOP,
    WAV_PLAYING,
    WAV_PAUSE,
    WAV_ERROR
}tWAV_REC_State;

typedef enum {
    WAV_REC_MODE = 0,
    WAV_PLAY_MODE,
    WAV_DEL_MODE
}tWAV_MODE;



//#define RP_DIR   "0:\\RP\\"

//#define FLAG_RECORD   	0X1111
//#define FLAG_PLAY		0X2222
//#define MIN_DISK_SPACE  	(1UL*1024*1024)
#define BEGIN_RECORD_SPACE  (2UL*1024*1024)

#define MIN_LENGTH  (12UL*1024)
#define MALLOC_LENGTH  (1UL*1024*1024)
//#define MAX_LENGTH  (64UL*1024*1024)
//#define MIN_LENGTH  (1UL*1024*1024)
#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE      ((PCM_BUF_SIZE-1)/4+2)          // word
#define ADPCM_BUF_CNT       2
/* Private macro -------------------------------------------------------------*/

extern __X int MbufL1[], MbufL2[], MbufR1[], MbufR2[];
extern __X int AudioMP3FileCatchBuf[];
extern __X int MbufHeap[];

#define  PCM_Buf1      		(( __X int *)MbufL1 )
#define  PCM_Buf2      		(( __X int *)MbufL2 )
#define  ADPCM_Buf			(( __X int *)MbufR1 )

//#define REC_ADPCM_Buf		(( __X int (*)[ ADPCM_BUF_SIZE ]))
#define ADVACE_ADPCM_Buf	(( __X int (*)[ ADPCM_BUF_SIZE ])MbufHeap )

//#define  REC_ADPCM_Buf			(( __X int (*)[ ADPCM_BUF_SIZE ])0x1800 )
//#define  ADVACE_ADPCM_Buf		REC_ADPCM_Buf
//extern __X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];

//#define  ADPCM_Buf      		(( __X int *)DarBuf1 )
//#define  ADPCM_Buf      		(( __X int *)BitBtreamBuf )

/* Private function prototypes -----------------------------------------------*/
Bool WAV_Mode(tWAV_MODE wav_mode);

void WavRecStart(void);
void WavRecProcess(void);
void WavRecPause(void);
void WavRecContinue(void);
void WavRecStop(void);

void WavPlayStart(void);
void WavPlayPause(void);
void WavPlayContinue(void);
void WavPlayStop(void);
void WavPlayProcess(void);
/* Private variables ---------------------------------------------------------*/
extern tWAV_REC_State WavRecState;
/* End of file ---------------------------------------------------------------*/
