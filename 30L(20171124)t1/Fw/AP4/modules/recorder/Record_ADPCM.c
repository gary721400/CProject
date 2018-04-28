// This is KBDM9 Template File. 
/***************************************************************************************
 * Copyright (c)
 * King Billion Electronics Co., Ltd.
 * File Name	: 
 * Author		:  Robin Lin
 * Create Date	: 2009/10/30 
 * History		: 
 * Comment      :  Example code for IMA-ADPCM Encoder and Audio ADC.
                   Recording voice from microphone, converted by Audio ADC.
                   Then compressed by ADPCM encoder and stored wave file to SD card.
 **************************************************************************************/
#include <KBDM9_DDK.H>
#include <fs.h>
#include <string.h>
#include "ADC.h"
#include "compiler.h"
#include "REC_API.H"
#include "BinaryImage.h"
#include "system.h"
#include "conf_kb.h"
#include "power.h"
#include "oid.h"
#include "search.h"
#include "agc.h"
#include "fc_internal.h" 
//#include "keypad.h"
//#include "oid.h"
 

#define ADPCM_DIR   "0:\\RECORDER\\"


#define MAIN_FILE_NAME "rec_"
#define MAX_LENGTH	(32UL*1024*1024)
#define MIN_DISK_SPACE  (1UL*1024*1024)

#define MAX_WAV_NUM  99
 

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

#pragma CODE = ADPCM_RECORD_CODE
#pragma DATA = ADPCM_RECORD_DATA
 


 
//extern Bool flag_wav_play;

//extern unsigned int vol_idx;
//extern const int VolTbl[VOL_NUM];
//extern unsigned int bak_current_index;
extern unsigned int g_string2[30];
//extern unsigned int volume_inc_flag;
/*
extern unsigned int gflag;
extern unsigned int ap4_mp3_flag;

extern const tFSDrvHandle SD_Cache_Drive;


unsigned int 	WRITE_SIZE;
__Y int ADC_Buf1[PCM_BUF_SIZE];
__Y int ADC_Buf2[PCM_BUF_SIZE];

__X int PCM_Buf[PCM_BUF_SIZE];
__X int ADPCM_Buf[ADPCM_BUF_SIZE];
*/

//extern  _MEM_TYPE_SLOW_  U16                  Current_Index;


//__X unsigned int Rp_Index;
//char adpcm_dir_path[64];
char adpcm_file_path[64];
//char rp_path[128];
//extern unsigned long free_space,wav_space;
extern unsigned long wav_space;
//extern void SpeakDetect(void);
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


//extern void swap_code(U16 flag);
// NotifyFlag, NotifyError;
// int *NotifyBuf;

//unsigned long WavWriteSize;
//__X FS_ERR fserr;
//__X FCFILE *ADPCM_File;   
extern tAUADC_Err Error;


//X int PCM_Buf1[PCM_BUF_SIZE];
// int PCM_Buf2[PCM_BUF_SIZE];
//unsigned long  Frame_Count;
extern unsigned int  Sample_Rate;


//char 	 tishi_file_name[32];

//extern int DAC_Out; // 0: Headphone, 1: Speaker


//__X  U16                 Current_Index;
//__X  U16                 Index;
//__X  U16 				 State_RP_process;
__X  U16				 adpcm_record_num;
__X  U16				 adpcm_playwav_num;
__X	 U16				 dac_rate_bak;

//__X  U16                 RP_flag;

//extern _MEM_TYPE_SLOW_  U16                  Current_Index;
//extern  _MEM_TYPE_SLOW_  Record_Range		   Ap4_Record_Range;
//ImageDeclareType(AUADC_COMMON_CODE);
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(KB_RESIZE_CODE);
ImageDeclareType(ADPCM_CREATE_CODE);
ImageDeclareType(ADPCM_PLAY_CODE);
ImageDeclareType(ADPCM_REC_CODE);


extern FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );
//void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
//void Key_Process(U16 Key_Code);    
void ADPCM_WavRecStop(void);
void ADPCM_WavPlayStop(void);
void ADPCM_FillBuffer(void);
void ADPCM_FirstDecode(void);
U16	 ADPCM_judge(void);
//void Rec_process_init(void);
void ADPCM_wav_play_process_init(void);
void ADPCM_process(void);
void ADPCM_task(Bool flag);
//extern void SpeakDetect(void);
void ADPCM_Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
Bool ADPCM_dir_create(Bool flag_create);

void ADPCM_RecProcess(void);

extern void kbd_key_init(void);
extern void kbd_process(void);
extern U16	kbd_key_read(void);
extern void kbd_init_process(void);
extern void __interrupt TimerISR_ADPCM(void);

void ADPCM_Key_Process(unsigned int key_data);
//void SpeakDetect(void);
/*void ErrHappen(const char *string,unsigned int fserr)
{
    while(1);
}


void __interrupt TimerISR_ADPCM(void)
{
   if( WavRecState == REC_RECODING){
		ADPCM_RecProcess();
	}
}
*/
#pragma CODE = ADPCM_REC_CODE
void ADPCM_Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
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
Bool ADPCM_Rec_process_init(void)
{
   unsigned int i;
	SDKLoadCode(ADPCM_CREATE_CODE);
	if(flag_creat)
	 	{
	 	  ADPCM_File = FSOpen(adpcm_file_path,FS_MODE_READ_WRITE,&REC_fserr);  
	 	}
	 else
	 	{
	 	 ADPCM_File = FSOpen(adpcm_file_path,FS_MODE_CREATE,&REC_fserr); 
	 	}
	  if(REC_fserr)
	    {
	        WavRecState = WAV_ERROR;
	        return FALSE;
	    }
//	 free_space = free_space*512U;
	wav_space = BEGIN_RECORD_SPACE;
//	FSResize(ADPCM_File, wav_space);
	SDKLoadCode(KB_RESIZE_CODE);
	REC_fserr = kb_fat_resize(ADPCM_File, wav_space);
	
	if(REC_fserr != FS_NO_ERROR)
	    {
	        WavRecState = WAV_ERROR;
			FSClose(ADPCM_File);
	        return FALSE;
	    }
	OP_CLOSE();
   
    // Initial Audio ADC
  
   SDKLoadCode(AUADC_COMMON_CODE);
    //		swap_code(AUADC_CODE_FLAG);
    /*
	    for(i=0; i<PCM_BUF_SIZE; i++){
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
	}*/
		
    // Initial ADPCM encoder
    idx_L = 0;
    idx_R = 0;
    WavWriteSize = 0;

	Frame_Count=0UL;

//	System_Timer_Init(2,10); 
            
    IMA_ADPCM_Encoder_Init(Sample_Rate);

    // write 60 bytes file header  
    WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);  

    NotifyError = 0;
    NotifyFlag = 0;

   
	
  
    // Set Audio ADC parameters      
//    AUADC_Init(MIC_In, DTU1, SmpRate2ADCSR(Sample_Rate), Buffer_Full_Func);
//	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_8K, Buffer_Full_Func);

	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, ADPCM_Buffer_Full_Func);
	
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
//		SDKLoadCode(DAC_COMMON_CODE);
//		swap_code(DAC_CODE_FLAG);
        return FALSE;
    }
	return TRUE;
}


void ADPCM_RecProcess(void)
{	
    while(idx_L != idx_R)
    {
        WavWriteSize += FSWrite(ADPCM_File, (void *)ADVACE_ADPCM_Buf[idx_L++], ADPCM_BUF_SIZE*2, &REC_fserr);
        if(idx_L > ADPCM_BUF_CNT-1)
            idx_L = 0;
    }
	
    if((wav_space-WavWriteSize)<512UL*1024UL)
    	{
    		wav_space += BEGIN_RECORD_SPACE;
//			FSResize(ADPCM_File, wav_space);
			SDKLoadCode(KB_RESIZE_CODE);
	 		 REC_fserr = kb_fat_resize(ADPCM_File, wav_space);
			 if(REC_fserr != FS_NO_ERROR){
			 	ADPCM_WavRecStop();
				WavRecState = REC_ERROR;
			 	}
			 	/*
			if (((free_space - wav_space)< 1024UL*32UL)) {
				ADPCM_WavRecStop();
				WavRecState = REC_ERROR;
			}*/
    	}
}


Bool ADPCM_RecStart(void)
{
    if(!ADPCM_Rec_process_init())return FALSE;
	return TRUE;

}

void ADPCM_WavRecStop(void)
{
    AUADC_Stop();

//	System_Timer_Stop(2);
  
//    FSResize(ADPCM_File, WavWriteSize);   
	  SDKLoadCode(KB_RESIZE_CODE);
	  REC_fserr = kb_fat_resize(ADPCM_File, WavWriteSize);

    FSSeek(ADPCM_File, 0);
        
    FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr); 
         
    REC_fserr = FSClose(ADPCM_File);

	//free_space = free_space - WavWriteSize;
	
    if(REC_fserr)
    {
        WavRecState = REC_ERROR;
//		SDKLoadCode(DAC_COMMON_CODE);
//		swap_code(DAC_CODE_FLAG);
        return;
    }
//	SDKLoadCode(DAC_COMMON_CODE);
//	swap_code(DAC_CODE_FLAG);
    WavRecState = REC_STOP;
}



#pragma CODE = ADPCM_PLAY_CODE

static void ADPCMFillBuffer(void)
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

static void ADPCMFirstDecode(void)
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


void ADPCM_wav_play_process_init(void)
{

	   // Initial DAC 

    DAC_Init(DTU3, DTU2, ADPCMFillBuffer, 0);
  
    DAC_LChannelBuffer1 = PCM_Buf1;
    DAC_RChannelBuffer1 = PCM_Buf1;
    DAC_LChannelBuffer2 = PCM_Buf2;
    DAC_RChannelBuffer2 = PCM_Buf2;    
    DAC_FrameSize = PCM_Buffer_Size;     

    idx_L = 0;
    idx_R = 0;         
	flag_wav_play = TRUE;
    ADPCMFirstDecode();
	vol_idx_bak = VOL_NUM;
	SpeakDetect(TRUE);
  
//    DAC_HeadPhone_Volume(30); 
//    DAC_Enable_HeadPhone();         //  Headphone output

    WavRecState = WAV_PLAYING;   
    DAC_Go();
}


void ADPCM_WavPlayStop(void)
{
    DAC_Pause();
    DAC_Mute();

    FSClose(ADPCM_File);
    WavRecState = WAV_STOP;
}






void ADPCM_WavPlayProcess(void)
{
    int len;
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
						Frame_Count=1;
		            	break;
		        	}
		        idx_L++;
		        if(idx_L > ADPCM_BUF_CNT-1)
		            idx_L = 0;
		    }
		}
}
 




Bool ADPCM_WavPlayStart(void)
{
    int i;
    int error;
    unsigned long read_length;
    tIMA_INFO ADPCM_Info;
	FS_ERR err;

	if(!flag_creat)
	{
	    return FALSE;
	}
	
	ADPCM_File = FSOpen(adpcm_file_path,FS_MODE_READ,&REC_fserr);
	if(REC_fserr)
    {
        WavRecState = WAV_ERROR;
        return FALSE;
    }
	/*
    for(i=0; i<PCM_BUF_SIZE; i++)
		{
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
		}  
	*/
    PCM_Buffer_Size = PCM_BUF_SIZE;
    ADPCM_Buffer_Size = ADPCM_BUF_SIZE; 
//    SDKLoadCode(DAC_COMMON_CODE);
 //read the 60-byte file header 
    read_length = FSRead(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);
 
 // Initial ADPCM  Decoder 
 
    error = IMA_ADPCM_Decoder_Init(&ADPCM_Info);  
    if(error != 0)
    {
        WavRecState = WAV_ERROR;
        return FALSE;
    }
        
//    DAC_SampleRate = SmpRate2PCMSR(ADPCM_Info.IMA_SR);
	DAC_SampleRate = dac_rate_bak;
    PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;               // buffer size for output PCM
    ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;       // buffer size for input ADPCM
    Frame_Count = ADPCM_Info.IMA_Frame_No;                          // Tell the user how many frame shall be decoded.

    ADPCM_wav_play_process_init();
	WavRecState = WAV_PLAYING; 
	
    return TRUE;
}

/*
Bool Rp_PlayStart(void)
{
    if(!Rp_Rec_process_init())return FALSE;
	return TRUE;

}
*/


/* End of file ---------------------------------------------------------------*/

#pragma CODE = ADPCM_CREATE_CODE

Bool ADPCM_dir_create(Bool flag_create)
{
	U16 i,current_dir_mask,mask,temp_data_1;
	char digi[6];
	U16 temp_index;
if(flag_create)
	{	
	 while(1){
			digi[0] = '0'+adpcm_record_num%10;
			digi[1] = '0'+adpcm_record_num/10;
			digi[2] = 0;
			strcpy(adpcm_file_path,ADPCM_DIR);
			strcat(adpcm_file_path,MAIN_FILE_NAME);
			strcat(adpcm_file_path,digi);
			strcat(adpcm_file_path,".wav");
			if(FSGetFileSize(adpcm_file_path,&REC_fserr)>0)
			{
				if(adpcm_record_num==MAX_WAV_NUM)
					return TRUE;
				adpcm_record_num++;
				continue;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
	temp_index = adpcm_playwav_num;
	while(1){
		digi[0] = '0'+adpcm_playwav_num%10;
		digi[1] = '0'+adpcm_playwav_num/10;
		digi[2] = 0;
		strcpy(adpcm_file_path,ADPCM_DIR);
		strcat(adpcm_file_path,MAIN_FILE_NAME);
		strcat(adpcm_file_path,digi);
		strcat(adpcm_file_path,".wav");
		if(FSGetFileSize(adpcm_file_path,&REC_fserr)>0)
			{
				return TRUE;
			}
			else
			{
				if(adpcm_playwav_num==MAX_WAV_NUM)
					{
					adpcm_playwav_num=1;
					}
				else
					{
					adpcm_playwav_num++;
					}
				if(temp_index==adpcm_playwav_num)return TRUE;
			}
	}
  }

 return FALSE;
}

#pragma CODE = ADPCM_RECORD_CODE
Bool ADPCM_file(void)
{
  char *ptr;
  char digi[6];


  		strcpy(adpcm_file_path,"0:\\RECORDER");
		REC_fserr = FSDirectory(adpcm_file_path);
//		free_space = 0;
//		free_space = FSFreeSpace(0,&REC_fserr);	
	    //if(free_space<=(MIN_DISK_SPACE)) 
	    //	{
	    //		return FALSE;
	    //	}
	 return TRUE;
}
void ADPCM_task(Bool flag)
{
	int drive, status;
	unsigned int i;
    unsigned int count;

   unsigned int temp_key=0;


	 adpcm_record_num = 1;
	 adpcm_playwav_num = 1;
	 WavRecState = REC_ERROR;
#if 1
 		//kbd_init_process();
		if(!ADPCM_file())return;
		SDKLoadCode(ADPCM_CREATE_CODE);
		
	 if(flag )
	 	{
			flag_creat = ADPCM_dir_create(TRUE);
			SDKLoadCode(ADPCM_REC_CODE);
	 		if(!ADPCM_RecStart())return;
			WavRecState = REC_RECODING;
			BlueLed_ON();
			RedLed_ON();
	 	}
	 else
	 	{
	 	flag_creat = ADPCM_dir_create(FALSE);
	 	SDKLoadCode(ADPCM_PLAY_CODE);
		if(!ADPCM_WavPlayStart())return;
	 	}

#endif	
	while(1)
	{

//	if((g_timer_tick%(500/TICK_PERIOD) == 0))
//    CheckUSB();

//	if((KBD_IDLE == kbd_state)&&(g_timer_tick%(500/TICK_PERIOD) == 0))
//	BatteryDetect();

	scan_oid();
 
   // Keypad process
   kbd_process();
   temp_key = kbd_key_read();
   if(0!=temp_key)
   	{                           
   		ADPCM_Key_Process(temp_key);
   	}
	   
	   if(index_flag)
	   	{

		if( WavRecState == REC_RECODING)
        	ADPCM_WavRecStop();
	        else if(WavRecState == WAV_PLAYING)
	        {
		       ADPCM_WavPlayStop();
		     }
			return;
	   	}
	    
		if(WavRecState == REC_RECODING)ADPCM_RecProcess();
     	if(WavRecState == WAV_PLAYING)
        {
        	if(rec_led_is_timer_timeout())
				{
					BlueLed_FLASH();
					RedLed_OFF();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
				}
	        SpeakDetect(FALSE);
        	ADPCM_WavPlayProcess();
	        if(WavRecState == WAV_STOP)
	        {
	            ADPCM_WavPlayStop();
	            return;
	        }
		}	
		if(WavRecState == REC_ERROR)return;
	}   
}	


void ADPCM_Key_Process(unsigned int key_data)
{
	 switch(key_data)
    {	
    	case 0x82: // (KBD_KEY_RELEASED<<8|KEY_MODE)
        break;
		
        case 0x81: // (KBD_KEY_RELEASED<<8|KEY_VOL_DEC)ap4 Volume change
        if(WavRecState == WAV_PLAYING)
        	{
				if(1== volume_inc_flag)	
					{
						vol_idx++;
						if(vol_idx > (VOL_NUM-1))
							{
								vol_idx-=2;
								volume_inc_flag = 0;
							}
					}
				else
					{
						
						if(0 == vol_idx)
							{
								vol_idx = 1;
								volume_inc_flag = 1;
							}
						else
							{
								vol_idx--;
							}
					}
				SpeakDetect(FALSE);
        	}
            break;

		case 0x43:	//(KBD_KEY_LONGPRESSED<<8|KEY_POWER)
			 break;
		case 0x42:	//(KBD_KEY_LONGPRESSED<<8|KEY_MODE)

			 OP_CLOSE();
			 CommonLoadCode(C_MP3);
//			 CommonLoadCode(C_REC_PASTER);
			 break;

		case 0x41:	//(KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC)
			  break;	
        default:
            break;
    }	
}


