/*
 * REC_API.C
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
#include "REC_API.H"
#include "conf_kb.h"
#include "com.h"
#include "compiler.h"
#include "oid.h"
#include "power.h"
#include "search.h"
#include "agc.h"
#include "system.h"
#include "search_remote.h"
#include "fc_internal.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//ImageDeclareType(AUADC_COMMON_CODE);
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(WAV_REC_CODE);
//ImageDeclareType(WAV_PLAY_CODE);

/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//extern __X int PCM_CACHE_Buf[];
//extern __X int ADPCM_Buf[];
//extern __X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
//extern __X int PCM_Buf1[];
//extern __X int PCM_Buf2[];

extern __X int idx_L,idx_R;
extern __X int NotifyFlag, NotifyError;
extern __X int *NotifyBuf;
extern __X unsigned long WavWriteSize;
extern __X FS_ERR REC_fserr;
extern __X FCFILE *ADPCM_File;   
extern __X unsigned long  Frame_Count;
extern __X unsigned int PCM_Buffer_Size, ADPCM_Buffer_Size;

extern __X unsigned long mp3_begin_clock;
extern __X unsigned long mp3_over_clock;
extern _MEM_TYPE_SLOW_  U32      rec_timer;
extern __X	 U16		dac_rate_bak;

extern  __X  Bool       		 index_flag;
extern  __X  Bool flag_reread;
extern unsigned int  Sample_Rate;

//extern unsigned long free_space,wav_space;
extern unsigned long wav_space;
extern unsigned int current_wav_num;
extern char dir_path[128];
extern unsigned int vol_idx;
extern const int VolTbl[VOL_NUM];
//extern Bool flag_wav_play;
//extern   _MEM_TYPE_SLOW_   U16            remote_index_tab[ REMOTE_INDEX_MAX ];
extern void kbd_key_init(void);
extern void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);

//extern void swap_code(U16 flag);
extern void judge_op(void);
extern void kbd_key_init(void);
extern void kbd_process(void);
extern U16	kbd_key_read(void);
extern void kbd_init_process(void);
//======================================================================
//ImageDeclareType(KB_RESIZE_CODE);

ImageDeclareType(GENDU_REC_CODE);
ImageDeclareType(GENDU_PLAY_CODE);
ImageDeclareType(WAV_REC_PLAY_CODE);
extern FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );

//======================================================================
//void ADC_INIT( void );
//Bool WavDel(void);

//#define AP4_GENDU_FILE   "0:\\ap4\\record.wav"
//#define REC_FILE		 "0:\\rec\\record.wav"

extern char REC_FILE[32];
/* Private functions ---------------------------------------------------------*/
///*
//#pragma CODE = IMA_CODEC_CODE
#pragma CODE = GENDU_CODEC_CODE
Bool WAV_Mode(tWAV_MODE wav_mode)
{
	
	Sample_Rate = 16000;
	dac_rate_bak	= DAC_SAMPLE_RATE_16K;
	SDKLoadCode(WAV_REC_PLAY_CODE);
    if(wav_mode == WAV_REC_MODE){
        SDKLoadCode(GENDU_REC_CODE);
		WavRecStart();
		return TRUE;
    	}
    else if(wav_mode == WAV_PLAY_MODE){
        SDKLoadCode(GENDU_PLAY_CODE);
		WavPlayStart();
		return TRUE;
    	}
	 else if(wav_mode == WAV_DEL_MODE){
//        SDKLoadCode(WAV_PLAY_CODE);
		//return WavDel();
    	}
	 return FALSE;
}
//*/
//#pragma CODE = WAV_REC_CODE
/*
static const unsigned int SR2ADCSR_TBL[][2] ={{(unsigned int)96000,AUADC_SAMPLE_RATE_96K},
                                             {(unsigned int)48000,AUADC_SAMPLE_RATE_48K}, 
                                             {(unsigned int)44100,AUADC_SAMPLE_RATE_44P1K}, 
                                             {(unsigned int)32000,AUADC_SAMPLE_RATE_32K}, 
                                             {(unsigned int)24000,AUADC_SAMPLE_RATE_24K}, 
                                             {(unsigned int)22050,AUADC_SAMPLE_RATE_22P05K}, 
                                             {(unsigned int)16000,AUADC_SAMPLE_RATE_16K}, 
                                             {(unsigned int)12000,AUADC_SAMPLE_RATE_12K}, 
                                             {(unsigned int)11025,AUADC_SAMPLE_RATE_11P025K}, 
                                             {(unsigned int)9600,AUADC_SAMPLE_RATE_9P6K},
                                             {(unsigned int)8000,AUADC_SAMPLE_RATE_8K}};
static unsigned int SmpRate2ADCSR(unsigned int smp_rate)
{
    int i;
    
    for (i=0; i<sizeof(SR2ADCSR_TBL)/sizeof(SR2ADCSR_TBL[0]); i++)
    {
        if (SR2ADCSR_TBL[i][0] == smp_rate)
            return SR2ADCSR_TBL[i][1];
    }
    return AUADC_SAMPLE_RATE_16K;
}
*/




void ADC_INIT( void )
{
///*   
//   SDKLoadCode(AUADC_COMMON_CODE);  
// 	swap_code(AUADC_CODE_FLAG);
//    // Set Audio ADC parameters      
//	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, Buffer_Full_Func);
    
//	AUADC_MicGain(MIC_Gain_0dB);
//    AUADC_InputGain(AUADC_Gain_21dB);

	Sample_Rate = 16000;

//	DAC_SampleRate = DAC_SAMPLE_RATE_16K;
	dac_rate_bak	= DAC_SAMPLE_RATE_16K;
//	SDKLoadCode(DAC_COMMON_CODE);
//	swap_code(DAC_CODE_FLAG);
//*/


}

//==========================================================================
/*
Bool find_wav(Bool flag)
{
	char digi[4];
	unsigned long temp_leng=0;
	
	strcpy(dir_path,"0:\\REC\\");
	digi[0] = '0'+current_wav_num/100;
	digi[1] = '0'+((current_wav_num -(digi[0]-'0')*100)/10);
	digi[2] = '0'+current_wav_num%10;
	digi[3] = 0;		
	strcat(dir_path,"rec");
	strcat(dir_path,digi);
	strcat(dir_path,".wav");	
	temp_leng = FSGetFileSize(dir_path,&REC_fserr);
	if(flag){
		 if(temp_leng==0){
				return FALSE;
			}
		 else{
		 	 free_space += temp_leng;
	         return TRUE;
		 	}
		}
	else{
			if(temp_leng==0)
			{
				return FALSE;
			}
	   		return TRUE;
		}

}
*/
/*
Bool WavDel(void)
{
 unsigned int i;
#if 0
	BlueLed_ON();
	RedLed_ON();
	i=100;
	while(i>0){
		if(find_wav((i-1))){
			FSDelete(dir_path);
			tishi_mp3(13);
			return TRUE;
			}
		i--;
		}
	tishi_mp3(11);
	return FALSE;
#else
	BlueLed_ON();
	RedLed_ON();
	while(current_wav_num>0){
		if(find_wav(FALSE)){
			FSDelete(dir_path);
			return TRUE;
			}
		current_wav_num--;
		}
	if(current_wav_num==0){
		if(find_wav(FALSE)){
			FSDelete(dir_path);
			return TRUE;
			}
		}
	return FALSE;
#endif	
}
*/
#pragma CODE = GENDU_REC_CODE
void Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
{   
    if (NotifyFlag > 0){
        
        NotifyError = 1;
    }   
	IMA_ADPCM_Encoder(buffer, ADVACE_ADPCM_Buf[idx_R++]);
	if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;

    Frame_Count++;
//	if(Frame_Count > 32){
//		  AGC(buffer);
//	}	
//    NotifyFlag++;
    
//    NotifyBuf = buffer;
}


Bool Rec_process_init(void)
{
   unsigned int i;
    // Initial Audio ADC
  
    SDKLoadCode(AUADC_COMMON_CODE);
//		swap_code(AUADC_CODE_FLAG);
/*
	    for(i=0; i<PCM_BUF_SIZE; i++){
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
	}
		*/
		
    // Initial ADPCM encoder
    idx_L = 0;
    idx_R = 0;
    WavWriteSize = 0;
            
    IMA_ADPCM_Encoder_Init(Sample_Rate);

    // write 60 bytes file header  
    WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);  

    NotifyError = 0;
    NotifyFlag = 0;
	Frame_Count=0UL;

   
	// Set Audio ADC parameters 
    AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, Buffer_Full_Func);
	
	AUADC_MicGain(MIC_Gain_0dB);
	//AUADC_MicGain(MIC_Gain_20dB);
    AUADC_InputGain(AUADC_Gain_19P5dB);
	
    AUADC_Set_Buffer((__X int *)PCM_Buf1, (__X int *)PCM_Buf2, PCM_BUF_SIZE);
         

 
//    AUADC_MicGain(MIC_Gain_20dB);  
//    AUADC_InputGain(AUADC_Gain_22P5dB);

/*
	 AGC_Init(PCM_BUF_SIZE);
     AGC_Sensitive(Sample_Rate/1000);
*/

    if(AUADC_Start() != AUADC_NO_ERR)
    {
        WavRecState = REC_ERROR;
		//SDKLoadCode(DAC_COMMON_CODE);
//		swap_code(DAC_CODE_FLAG);
        return FALSE;
    }
	rec_timer += timer_get_time();
	return TRUE;
}


void WavRecStart(void)
{
    unsigned int i;
    unsigned long free_space,rec_len;
	FS_ERR err;
    unsigned int temp_key;
	char path[32];
	Bool temp_interrupt_flag;

 	temp_interrupt_flag = FALSE;
    flag_reread = FALSE;
	strcpy(path,REC_FILE);
/*	i=0;
	while(i<100){
		if(!find_wav(i))goto begin_record;
		i++;
		}
*/
//	 return;

	wav_space = FSGetFileSize((char *)path, &err);
	if(wav_space==0)return;
	// mp3_over_clock = timer_get_time();
	//rec_timer = (mp3_over_clock - mp3_begin_clock)*2;
#ifdef TX_DEBUG
	SendString((char *)"\r\nreread start!");
#endif	
		rec_timer = (mp3_over_clock - mp3_begin_clock)*6;
	rec_timer = rec_timer/5;
	if(rec_timer<1500)rec_timer=1500;
begin_record:
//	rec_len = ((mp3_over_clock - mp3_begin_clock)/500)*6144;
//	if(rec_len<MIN_LENGTH)rec_len = MIN_LENGTH;
    ADPCM_File = FSOpen(path,FS_MODE_READ_WRITE,&REC_fserr);    
    if(REC_fserr)
    {
        WavRecState = REC_ERROR;
        return;
    }
 //   FSResize(ADPCM_File, wav_space);
//	 FSResize(ADPCM_File, rec_len);
	index_flag = FALSE;
	OP_CLOSE();
    if(!Rec_process_init())return;
	
    WavRecState = REC_RECODING;
#if 1
	while(!timer_check_timeout(rec_timer)){
		 WavRecProcess();
		if(WavRecState == REC_ERROR)break;	
		scan_oid();
		if(index_flag){
			if(INDEX_STOP == Current_Index){
#ifdef TX_DEBUG
			SendString((char *)"\r\nreread stop!");
#endif	
				break;
		   		}
	   	}
		/*
		scan_oid();
		if(index_flag){
				if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
					{
					 index_flag=FALSE;
					 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL; 
					}
				else{
					    temp_interrupt_flag = TRUE;
						break;
					}
			}
		kbd_process();
		temp_key = kbd_key_read();
		if(temp_key!=0){
			temp_interrupt_flag = TRUE;
			break;
			}
		*/
		}
#else
	while(1){
		WavRecProcess();
	
		scan_oid();
		if(index_flag){
				if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_FIRST_VOL+6)))
					{
					 index_flag=FALSE;
					 vol_idx=Current_Index-INDEX_FIRST_VOL; 
					}
				else{
						break;
					}
			}
		kbd_process();
		temp_key = kbd_key_read();
		if(temp_key!=0){
			break;
			}
		}
#endif	
	WavRecStop();
	flag_reread = TRUE;
/*
	kbd_key_init();
	if(!temp_interrupt_flag){
		
		}
	*/
}

void WavRecProcess(void)
{
#if 0
    if (NotifyFlag > 0)
    {
        NotifyFlag--;
     
//        memcpy(PCM_CACHE_Buf, NotifyBuf, PCM_BUF_SIZE*2);
//        IMA_ADPCM_Encoder(PCM_CACHE_Buf, ADVACE_ADPCM_Buf[idx_R]);  
		idx_R++;
        if(idx_R>=2){
			WavWriteSize += FSWrite(ADPCM_File, (void *)ADVACE_ADPCM_Buf, ADPCM_BUF_SIZE*4, &REC_fserr);      
			idx_R=0;
        	}
        
                 
    }
#else	
    while(idx_L != idx_R)
    {
        WavWriteSize += FSWrite(ADPCM_File, (void *)ADVACE_ADPCM_Buf[idx_L++], ADPCM_BUF_SIZE*2, &REC_fserr);
        if(idx_L > ADPCM_BUF_CNT-1)
            idx_L = 0;
    }

 if((wav_space-WavWriteSize)<MIN_LENGTH)
	{
		wav_space += MALLOC_LENGTH;
		
		//if (((free_space - wav_space)< MALLOC_LENGTH)||(wav_space>=free_space)) {
		//	WavRecStop();
		//	WavRecState = REC_ERROR;
		//}
		//FSResize(ADPCM_File, wav_space);
		SDKLoadCode(KB_RESIZE_CODE);
		REC_fserr = kb_fat_resize(ADPCM_File, wav_space);
		if(REC_fserr != FS_NO_ERROR){
			WavRecStop();
			WavRecState = REC_ERROR;
			}
		///free_space = free_space - MALLOC_LENGTH;
	}

/*
    if((wav_space-WavWriteSize)<512UL*1024UL)
    	{
    		wav_space += MIN_LENGTH;
			FSResize(ADPCM_File, wav_space);
			if (((free_space - wav_space)< 1024UL*32UL)) {
				WavRecStop();
				WavRecState = REC_STOP;
			}
    	}
	*/
#endif  
}
/*
void WavRecPause(void)
{
    __asm("BCLR MICICR.0");
    WavRecState = REC_PAUSE;
}

void WavRecContinue(void)
{
    __asm("BSET MICICR.0");
    WavRecState = REC_RECODING;
}
*/
void WavRecStop(void)
{
    AUADC_Stop();
  
//    FSResize(ADPCM_File, WavWriteSize);   

    FSSeek(ADPCM_File, 0);
        
    FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr); 
         
    REC_fserr = FSClose(ADPCM_File);

	
	
	//free_space = free_space - WavWriteSize;
  
    if(REC_fserr)
    {
        WavRecState = REC_ERROR;
		//SDKLoadCode(DAC_COMMON_CODE);
//		swap_code(DAC_CODE_FLAG);
        return;
    }
	//SDKLoadCode(DAC_COMMON_CODE);
//	swap_code(DAC_CODE_FLAG);
    WavRecState = REC_STOP;
	
}

#pragma CODE = GENDU_PLAY_CODE
/*
static const unsigned int SR2PCMSR_TBL[][2] ={{(unsigned int)96000,DAC_SAMPLE_RATE_96K},
                                             {(unsigned int)48000,DAC_SAMPLE_RATE_48K}, 
                                             {(unsigned int)44100,DAC_SAMPLE_RATE_44P1K}, 
                                             {(unsigned int)32000,DAC_SAMPLE_RATE_32K}, 
                                             {(unsigned int)24000,DAC_SAMPLE_RATE_24K}, 
                                             {(unsigned int)22050,DAC_SAMPLE_RATE_22P05K}, 
                                             {(unsigned int)16000,DAC_SAMPLE_RATE_16K}, 
                                             {(unsigned int)12000,DAC_SAMPLE_RATE_12K}, 
                                             {(unsigned int)11025,DAC_SAMPLE_RATE_11P025K}, 
                                             {(unsigned int)9600,DAC_SAMPLE_RATE_9P6K},
                                             {(unsigned int)8000,DAC_SAMPLE_RATE_8K}};
                                    
static unsigned int SmpRate2PCMSR(unsigned int smp_rate)
{
    int i;
    for (i=0; i<sizeof(SR2PCMSR_TBL)/sizeof(SR2PCMSR_TBL[0]); i++)
    {
        if (SR2PCMSR_TBL[i][0] == smp_rate)
            return SR2PCMSR_TBL[i][1];
    }
    return DAC_SAMPLE_RATE_44P1K;
}
*/
static void FillBuffer(void)
{
    unsigned long len;
    __X int *BufPtr;
    void (*Fill_Buf)(void);
    
    if(Frame_Count == 0)
    {
        WavRecState = WAV_STOP;
        return; 
    }
    Frame_Count-- ;
    if(DAC_Buffer1Empty()){
        BufPtr = DAC_LChannelBuffer1 ;
      Fill_Buf = DAC_FillBuffer1;       
    }else if(DAC_Buffer2Empty()) {
        BufPtr = DAC_LChannelBuffer2 ;
        Fill_Buf = DAC_FillBuffer2;
    }
    
    IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[idx_R++], BufPtr);
    if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;
    
    (*Fill_Buf)();
}               

static void FirstDecode(void)
{
    unsigned long len;
    int i;
    
    len = FSRead(ADPCM_File, (void *)ADVACE_ADPCM_Buf[0], ADPCM_Buffer_Size*2, &REC_fserr);
    IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[0], DAC_LChannelBuffer1);
    for(i=0;i<ADPCM_BUF_CNT;i++)
    {
        len = FSRead(ADPCM_File, (void *)ADVACE_ADPCM_Buf[i], ADPCM_Buffer_Size*2, &REC_fserr);
        if(len == 0)
            break;
    }                      
    DAC_FillBuffer1();
}




void wav_play_process_init(void)
{

	   // Initial DAC 

    DAC_Init(DTU3, DTU2, FillBuffer, 0);
  
    DAC_LChannelBuffer1 = PCM_Buf1;
    DAC_RChannelBuffer1 = PCM_Buf1;
    DAC_LChannelBuffer2 = PCM_Buf2;
    DAC_RChannelBuffer2 = PCM_Buf2;    
    DAC_FrameSize = PCM_Buffer_Size;     

    idx_L = 0;
    idx_R = 0;                                                    
    FirstDecode();
  	flag_wav_play = TRUE;
 //   DAC_HeadPhone_Volume(30); 
 //   DAC_Enable_HeadPhone();         //  Headphone output
//	judge_op();
//	inc_volume();
	set_ex();
	vol_idx_bak = VOL_NUM;
 	SpeakDetect(TRUE);

	WavRecState = WAV_PLAYING;
    DAC_Go();
}



void reread_fix_index(void)
{
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
		{
		 vol_idx=(U16)Current_Index-INDEX_FIRST_VOL;
		}
	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
				}
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;

			} 
		}
}


void WavPlayStart(void)
{
    int i;
    int error;
    unsigned long read_length;
    tIMA_INFO ADPCM_Info;
	FS_ERR err;
	unsigned int temp_key;
	char path[32];
#if 1
	flag_reread = FALSE;
	strcpy(path,REC_FILE);

	if(!(FSGetFileSize(path, &err))){
		return;
		}
#else	
/*
	i=99;
	while(i>=0){
		if(find_wav(i))goto begin_play;
		i--;
		}
*/	
	 return;
	 
begin_play:
#endif
    ADPCM_File = FSOpen(path,FS_MODE_READ,&REC_fserr);
    if(REC_fserr)
    {
        WavRecState = WAV_ERROR;
        return;
    }
	/*
	for(i=0; i<PCM_BUF_SIZE; i++){
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
    }  */
    PCM_Buffer_Size = PCM_BUF_SIZE;
    ADPCM_Buffer_Size = ADPCM_BUF_SIZE; 
    SDKLoadCode(DAC_COMMON_CODE);
//	swap_code(DAC_CODE_FLAG);
 //read the 60-byte file header 
    read_length = FSRead(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);
 
 // Initial ADPCM  Decoder 
 
    error = IMA_ADPCM_Decoder_Init(&ADPCM_Info);  
    if(error != 0)
    {
        WavRecState = WAV_ERROR;
        return;
    }
        
//    DAC_SampleRate = SmpRate2PCMSR(ADPCM_Info.IMA_SR);
	DAC_SampleRate = dac_rate_bak;
    PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;               // buffer size for output PCM
    ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;       // buffer size for input ADPCM
    Frame_Count = ADPCM_Info.IMA_Frame_No;                          // Tell the user how many frame shall be decoded.
    
	index_flag =FALSE;
	kbd_init_process();
	
    wav_play_process_init();
	
    while(1)
    {
        WavPlayProcess();
		scan_oid();
		if(index_flag){
			index_flag = FALSE;
			//reread_fix_index();
			}
		//judge_op();
		SpeakDetect(FALSE);
        if(WavRecState == WAV_STOP)
        {
/*			
            break;
        }
/*		
	   scan_oid();
		if(index_flag){
//		   WavPlayStop();
            break;
			}
		kbd_process();
		temp_key = kbd_key_read();
		if(temp_key!=0){
//*/		
			break;
			}
		
    }
	WavPlayStop();
	//kbd_key_init();
#ifdef TX_DEBUG
	SendString((char *)"\r\nreread play over!");
#endif		
}

void WavPlayProcess(void)
{
    int len;
	if(Frame_Count <= 0){
		 WavRecState = WAV_STOP;
		 return;
		}
    if(Frame_Count==1){
		if(idx_L == idx_R){
			WavRecState = WAV_STOP;
			return;
			}
    	}
	else
		{
	    while(idx_L != idx_R)
	    {
	        len = FSRead(ADPCM_File, ADVACE_ADPCM_Buf[idx_L], ADPCM_Buffer_Size*2, &REC_fserr);
	        if(len == 0)
	        	{
//	        	    WavRecState = WAV_STOP;
					Frame_Count=0;
	            	break;
	        	}
	        idx_L++;
	        if(idx_L > ADPCM_BUF_CNT-1)
	            idx_L = 0;
	    }
		}
}

void WavPlayStop(void)
{
    DAC_Pause();
    DAC_Mute();
	clr_ex();
    FSClose(ADPCM_File);
    WavRecState = WAV_STOP;
	op_start_timer();
}
/* End of file ---------------------------------------------------------------*/



