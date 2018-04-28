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
#include "config.h"
#include "com.h"
#include "fc_internal.h"
#include "keypad.h"
//#include "oid.h"
 

#define RP_DIR   "0:\\RP\\"
//#define RP_DIR   "0:\\AP4\\"

#define FLAG_RECORD   	0X1111
#define FLAG_PLAY		0X2222

 

#if( OID_VERSION == OID1 )
#define STOP_INDEX  4286
#endif

#if( OID_VERSION == OID2 )
#define STOP_INDEX  64765
#endif

#define MAIN_FILE_NAME "rec_"
#define MAX_LENGTH	(32UL*1024*1024)
#define MIN_DISK_SPACE  (1UL*1024*1024)

 

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

#pragma CODE = RP_RECORD_CODE
#pragma DATA = RP_RECORD_DATA
 


 


extern unsigned int vol_idx;

//extern unsigned int bak_current_index;

extern unsigned int begin_R;
extern unsigned int over_R;
extern unsigned int begin_P;
extern unsigned int over_P;
extern unsigned int g_string2[30];
extern unsigned int volume_inc_flag;
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

extern __X  Bool         index_flag;
__X unsigned int Rp_Index;
__X  U16         rp_flag;
char dir_path[128];
//char rp_path[128];
unsigned long wav_space;

//extern unsigned long free_space;

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

// NotifyFlag, NotifyError;
// int *NotifyBuf;

//unsigned long WavWriteSize;
//__X FS_ERR fserr;
//__X FCFILE *ADPCM_File;   
tAUADC_Err Error;


//X int PCM_Buf1[PCM_BUF_SIZE];
// int PCM_Buf2[PCM_BUF_SIZE];
//unsigned long  Frame_Count;
unsigned int  Sample_Rate;


//char 	 tishi_file_name[32];

//extern int DAC_Out; // 0: Headphone, 1: Speaker

extern  __X  Bool       		 index_flag;
//__X  U16                 Current_Index;
__X  U16                 Index;
__X  U16 				 State_RP_process;
extern __X	 U16		dac_rate_bak;

//__X  U16                 RP_flag;

//extern Bool flag_wav_play;

//extern _MEM_TYPE_SLOW_  U16                  Current_Index;
extern _MEM_TYPE_SLOW_      U8                ap4_short_name[ SHORT_NAME_SIZE + 1 ];
extern _MEM_TYPE_SLOW_      U8                ap4_long_name[ G_STRING_SIZE ];
//extern void swap_code(U16 flag);
//extern  _MEM_TYPE_SLOW_  Record_Range		   Ap4_Record_Range;
//ImageDeclareType(AUADC_COMMON_CODE);
//ImageDeclareType(KB_RESIZE_CODE);

//void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
//void Key_Process(U16 Key_Code);    

//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(AUADC_COMMON_CODE);


ImageDeclareType(RP_REC_CODE);
ImageDeclareType(RP_PLAY_CODE);
ImageDeclareType(RP_CREATE_DIR01_CODE);
ImageDeclareType(RP_CREATE_DIR02_CODE);
ImageDeclareType(WAV_REC_PLAY_CODE);


void Rp_WavPlayStop(void);
void RP_FillBuffer(void);
void RP_FirstDecode(void);
U16	 RP_judge(void);
void Rec_process_init(void);
void RP_wav_play_process_init(void);
void RP_process(void);
void rp_task(void);
Bool Rp_file(void);
//extern void SpeakDetect(void);
extern void rp_Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
Bool dir_create(Bool flag_create);
void RP_WavRecStop(void);
void Rp_RecProcess(void);

extern void __interrupt TimerISR_RP(void);
extern FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );
//void SpeakDetect(void);
/*void ErrHappen(const char *string,unsigned int fserr)
{
    while(1);
}

void __interrupt TimerISR_RP(void)
{
   if( WavRecState == REC_RECODING){
		Rp_RecProcess();
	}
}
*/

#pragma CODE = RP_REC_CODE
void rp_Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
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


void Rp_RecProcess(void)
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
			//FSResize(ADPCM_File, wav_space);
			SDKLoadCode(KB_RESIZE_CODE);
			REC_fserr = kb_fat_resize(ADPCM_File, wav_space);
			if(REC_fserr != FS_NO_ERROR){
				RP_WavRecStop();
				WavRecState = REC_ERROR;
				bDiskFullFlag = true;
				}
			/*
			if (((free_space - wav_space)< 1024UL*32UL)) {
				RP_WavRecStop();
				WavRecState = REC_ERROR;
			}*/
    	}
}


Bool Rp_Rec_process_init(void)
{
   unsigned int i;

	//if(dir_create(TRUE))
	  if(flag_creat)
	 	{
	 	  ADPCM_File = FSOpen(dir_path,FS_MODE_READ_WRITE,&REC_fserr);  
	 	}
	 else
	 	{
	 	 ADPCM_File = FSOpen(dir_path,FS_MODE_CREATE,&REC_fserr); 
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
	if(REC_fserr != FS_NO_ERROR){
		FSClose(ADPCM_File);
		//rp_flag &= 0x0004;
		bDiskFullFlag = true;
		return FALSE;
		}
	OP_CLOSE();
    
    // Initial Audio ADC
  
    SDKLoadCode(AUADC_COMMON_CODE);
//    swap_code(AUADC_CODE_FLAG);
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

	Frame_Count=0UL;
            
    IMA_ADPCM_Encoder_Init(Sample_Rate);

    // write 60 bytes file header  
    WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);  

    NotifyError = 0;
    NotifyFlag = 0;


	//System_Timer_Init(4,10); 
	
    // Set Audio ADC parameters      
//    AUADC_Init(MIC_In, DTU1, SmpRate2ADCSR(Sample_Rate), Buffer_Full_Func);
//	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_8K, Buffer_Full_Func);
	// Set Audio ADC parameters 
    AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, rp_Buffer_Full_Func);
	
	AUADC_MicGain(MIC_Gain_0dB);
    //AUADC_MicGain(MIC_Gain_20dB);
    AUADC_InputGain(AUADC_Gain_19P5dB);;
	
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


Bool Rp_RecStart(void)
{
    if(!Rp_Rec_process_init())return FALSE;
	return TRUE;

}

void RP_WavRecStop(void)
{
    AUADC_Stop();

//	System_Timer_Stop(4);
  
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
	 rp_flag &= 0x0001;
}

#pragma CODE = RP_PLAY_CODE
static void RPFillBuffer(void)
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

static void RPFirstDecode(void)
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


void rp_wav_play_process_init(void)
{

	   // Initial DAC 

    DAC_Init(DTU3, DTU2, RPFillBuffer, 0);
  
    DAC_LChannelBuffer1 = PCM_Buf1;
    DAC_RChannelBuffer1 = PCM_Buf1;
    DAC_LChannelBuffer2 = PCM_Buf2;
    DAC_RChannelBuffer2 = PCM_Buf2;    
    DAC_FrameSize = PCM_Buffer_Size;     

    idx_L = 0;
    idx_R = 0;      
	flag_wav_play = TRUE;
    RPFirstDecode();
	set_ex();
	vol_idx_bak = VOL_NUM;
	SpeakDetect(TRUE);
  
//    DAC_HeadPhone_Volume(30); 
//    DAC_Enable_HeadPhone();         //  Headphone output

    WavRecState = WAV_PLAYING;   
    DAC_Go();
}


void Rp_WavPlayStop(void)
{
    DAC_Pause();
    DAC_Mute();
	clr_ex();
    FSClose(ADPCM_File);
    WavRecState = WAV_STOP;
	 rp_flag &= 0x0002;
}





void Rp_WavPlayProcess(void)
{
#if 1
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
#else
    int len;
    
    while(idx_L != idx_R)
    {
        len = FSRead(ADPCM_File, ADVACE_ADPCM_Buf[idx_L], ADPCM_Buffer_Size*2, &REC_fserr);
        if(len == 0)
        	{
        	    WavRecState = WAV_STOP;
            	break;
        	}
        idx_L++;
        if(idx_L > ADPCM_BUF_CNT-1)
            idx_L = 0;
    }
#endif	
}
 

//===========================================================================

//------------------------------------------------------------------------------
void Rp_Key_Process(U16 Key_Code)
{
    
    switch(Key_Code)
    {
    	case (STATE_KEY_PRESSED|KEY_MODE): // ap4 pause
			
		  if((WAV_PAUSE == WavRecState)){
				WavRecState = WAV_PLAYING;
				Wav_Continue();
				return;
				}
		   if((WAV_PLAYING == WavRecState)){
				WavRecState = WAV_PAUSE;
				Wav_Pause();
				}
            break;	

		/*           
		case (STATE_KEY_RELEASED|KEY_VOL_INC):
			if(vol_idx < (VOL_NUM-1)){
				vol_idx++;
				}

			break;
		case (STATE_KEY_RELEASED|KEY_VOL_DEC):
				if(vol_idx > 0){
					vol_idx--;
				}
				if(!g_b_player_on)
				{
					(void)act_do(All_Control.Addr_Antion_key, 0);
					state_search_to_play();
				}
		break;

		case (STATE_KEY_LONGPRESSED|KEY_POWER):
			 interrupt_play();
			 scan_oid_disable();
			 Kbd_set_evt_disabled();
			 tishi_mp3(FLAG_POWER_OFF_FILE);
		 	 CommonLoadCode(C_POWEROFF);
			 break;	
	//*/		 
        default:
            break;
    }
}




Bool RP_WavPlayStart(void)
{
    int i;
    int error;
    unsigned long read_length;
    tIMA_INFO ADPCM_Info;
	FS_ERR err;
/*
	if(!dir_create(FALSE))
	{
	    return FALSE;
	}
	*/
	ADPCM_File = FSOpen(dir_path,FS_MODE_READ,&REC_fserr);
	if(REC_fserr)
    {
        WavRecState = WAV_ERROR;
        return FALSE;
    }
#ifdef TX_DEBUG
	SendString((char *)"\r\nfind and open ok!");
#endif
	/*
    for(i=0; i<PCM_BUF_SIZE; i++)
		{
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
		}  
	*/
    PCM_Buffer_Size = PCM_BUF_SIZE;
    ADPCM_Buffer_Size = ADPCM_BUF_SIZE; 
    SDKLoadCode(DAC_COMMON_CODE);
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

    rp_wav_play_process_init();
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
#pragma CODE = RP_RECORD_CODE

void wavPlayLed(void)
{

#if 0		
		if(rec_led_is_timer_timeout())
			{
				BlueLed_FLASH();
				RedLed_OFF();
				rec_led_start_timer(LED_NORMAL_TIME);
			}
#else		
		if(rec_led_is_timer_timeout())
			{
#if(QUICKLY_FLASH == ENABLE)
				if(b_flash_led){
					BlueLed_ON();
					if(bPenOn){
						rec_led_start_timer(REC_OID_FLASH_TIME);
						}
					else{
						//BlueLed_ON();
						rec_led_start_timer(REC_LED_ON_QUICK_TIME);
						}
					}
				else{
					   BlueLed_OFF();
						if(bPenOn){
						rec_led_start_timer(REC_OID_FLASH_TIME);
						}
					else{
						//BlueLed_ON();
						rec_led_start_timer(REC_LED_OFF_NORMAL_TIME);
						}
					}
				//BlueLed_FLASH();
				b_flash_led = ~b_flash_led;
				RedLed_OFF();
		
#endif			
#if(QUICKLY_FLASH == DISABLE)

				BlueLed_FLASH();
				RedLed_OFF();
				if(bPenOn){
					rec_led_start_timer(REC_OID_FLASH_TIME);
					}
				else{
					//BlueLed_ON();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
					}
#endif				
			}
#endif
}
void rp_task(void)
{
	int drive, status;
	unsigned int i;
    U16 temp_key=0;
    unsigned int count;

	SDKLoadCode(WAV_REC_PLAY_CODE);
	 WavRecState = REC_ERROR;
#if 1
		 index_flag=FALSE;
		 Rp_Index =(U16) Current_Index;
		 Sample_Rate = 16000;
	     dac_rate_bak	= DAC_SAMPLE_RATE_16K;
		//i = RP_judge();
		//if(0==i)return;
		SDKLoadCode(RP_CREATE_DIR01_CODE);
		i = RP_judge();
		if(0==i)return;
		if(!Rp_file())return;
		SDKLoadCode(RP_CREATE_DIR02_CODE);
	 if(FLAG_RECORD ==i )
	 	{
#ifdef TX_DEBUG
		SendString((char *)"\r\nrp_re!");
#endif	 	
	 		flag_creat = dir_create(TRUE);
			SDKLoadCode(RP_REC_CODE);
	 		if(!Rp_RecStart())return;
			WavRecState = REC_RECODING;
			Kbd_set_evt_disabled();
			BlueLed_OFF();
			RedLed_ON();
	 	}
	 else
	 	{
#ifdef TX_DEBUG
		SendString((char *)"\r\nrp_p!");
#endif		 	
	 	if(!dir_create(FALSE))return;
		SDKLoadCode(RP_PLAY_CODE);
		if(!RP_WavPlayStart())return;
		Kbd_set_evt_enabled();
	 	}

#endif	
	while(1)
	{
	   adc_get();

		// Keypad process
   		kbd_process();
	   
	   scan_oid();
	   if(index_flag)
	   	{
		if( WavRecState == REC_RECODING)
        	RP_WavRecStop();
	        else if(WavRecState == WAV_PLAYING)
	        {
		        Rp_WavPlayStop();
		     }
			return;
	   	}
	    
		if(WavRecState == REC_RECODING)Rp_RecProcess();
     	if(WavRecState == WAV_PLAYING)
        {
        /*
        	if(rec_led_is_timer_timeout())
				{
					BlueLed_ON();
					RedLed_OFF();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
				}
			*/
		    wavPlayLed();
	        SpeakDetect(FALSE);
        	Rp_WavPlayProcess();
			
	        if(WavRecState == WAV_STOP)
	        {
	            Rp_WavPlayStop();
	            return;
	        }
		}	
		temp_key = kbd_key_read();
		if(temp_key != 0){
			Rp_Key_Process(temp_key);
			}
		if(WavRecState == REC_ERROR)return;
	}   
}	


/* End of file ---------------------------------------------------------------*/
#pragma CODE = RP_CREATE_DIR02_CODE
Bool dir_create(Bool flag_create)
{
	U16 i,current_dir_mask,mask,temp_data_1;
	char digi[6];
	U16 temp_index;
	temp_index = Rp_Index;
if(flag_create)
	{	
		 current_dir_mask = 0xF000;
		 mask = 0xF000 >> 4;
		 while(mask)
			{
			        temp_data_1 = current_dir_mask&temp_index;
					digi[0] = '0'+temp_data_1/10000;
					digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
					digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
					digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
					digi[4] = '0'+temp_data_1%10;
					digi[5] = 0;

					strcat(dir_path,"REC");
		    		strcat(dir_path,digi);
					
				
					REC_fserr = FSDirectory(dir_path);
					current_dir_mask |= mask;
		      		mask >>= 4;
					strcat(dir_path,"\\");
#ifdef TX_DEBUG
					SendString((char *)"\r\ncreat_dir=");
					SendString(dir_path);
#endif						
			}
 			temp_data_1 = current_dir_mask&temp_index;
			digi[0] = '0'+temp_data_1/10000;
			digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+temp_data_1%10;
			digi[5] = 0;
			strcat(dir_path,"rec");
	 		strcat(dir_path,digi);
			strcat(dir_path,".wav");			
#ifdef TX_DEBUG
					SendString((char *)"\r\ncreate_file=");
					SendString(dir_path);
#endif			
			if(FSGetFileSize(dir_path,&REC_fserr)>0)
				{
				   return TRUE;
				}
			else
				{
					return FALSE;
				}
	}
	else
	{
		 current_dir_mask = 0xF000;
		 mask = 0xF000 >> 4;
		 while(mask)
			{
			        temp_data_1 = current_dir_mask&temp_index;
					digi[0] = '0'+temp_data_1/10000;
					digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
					digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
					digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
					digi[4] = '0'+temp_data_1%10;
					digi[5] = 0;

					strcat(dir_path,"REC");
		    		strcat(dir_path,digi);
					strcat(dir_path,"\\");				
					current_dir_mask |= mask;
		      		mask >>= 4;
			}
 			temp_data_1 = current_dir_mask&temp_index;
			digi[0] = '0'+temp_data_1/10000;
			digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+temp_data_1%10;
			digi[5] = 0;
			strcat(dir_path,"rec");
	 		strcat(dir_path,digi);
			strcat(dir_path,".wav");			
#ifdef TX_DEBUG
			SendString((char *)"\r\nfind_file=");
			SendString(dir_path);
#endif		
		  if(FSGetFileSize(dir_path,&REC_fserr)==0)
			{
				return FALSE;
			}
		   return TRUE;
	}
}


#pragma CODE = RP_CREATE_DIR01_CODE

U16	RP_judge(void)
{
	if((Rp_Index >= Ap4_Record_Range.Record_Index_Min)&&(Rp_Index <= Ap4_Record_Range.Record_Index_Max))
		{
		 return FLAG_RECORD;
		}
		
	if((Rp_Index >= Ap4_Record_Range.Play_Index_Min)&&(Rp_Index <= Ap4_Record_Range.Play_Index_Max))
		{
		 Rp_Index = Rp_Index-(U16)(Ap4_Record_Range.Play_Index_Min-Ap4_Record_Range.Record_Index_Min);
		return FLAG_PLAY;
		}
	return 0;
}

Bool Rp_file(void)
{
  char *ptr;
  char digi[6];
  U8 i;
		ptr = 0;
  		strcpy(dir_path, "0:\\RP");
	
		 //REC_fserr = FSDirectory(dir_path);
		  strcat(dir_path,"\\");		 
/*	
#if 0
  	     ptr = strchr((char *)g_string2,'.');
		 if(ptr!=0)
		 	{
		 		*ptr = '\0';
		 		
		 	}
		 else
		 	{

				g_string2[8]='\0';
		 	}
#else
		for(i=0;i<15;i++)
		{
			if((g_string2[i]>126)||(g_string2[i]<32))
			 break;
		}
				strncat(dir_path,(char *)g_string2,i);

#endif
		 
		

		 REC_fserr = FSDirectory(dir_path);
//*/
/*
		if( *( U16 *)ap4_long_name == 0 )
	   {
	      strcat( dir_path, ( char *)ap4_short_name );
	      REC_fserr = FSDirectory(( char *)dir_path );
	      if( REC_fserr == FS_DIRECTORY_FULL )
	      {
	         //g_b_disk_is_full = TRUE;
	         return FALSE;
	      }
	      if(( REC_fserr != FS_DIRECTORY_CONFLICT )
	         &&( REC_fserr != FS_NO_ERROR ))
	      {
	         return FALSE;
	      }
	      goto record_creat_short_name;
	   }

	   //unicode
	   REC_fserr = FSDirForUnicode( "0:\\RP", ( char *)ap4_short_name, ap4_long_name );
	   if( REC_fserr == FS_DIRECTORY_FULL )
	   {
	      //g_b_disk_is_full = TRUE;
	      return FALSE;
	   }
	   if(( REC_fserr != FS_DIRECTORY_CONFLICT )
	      &&( REC_fserr != FS_NO_ERROR ))
	   {
	      return FALSE;
	   }
	   strcat( dir_path, ( char *)ap4_short_name );
record_creat_short_name:
		strcat( dir_path, "\\" );
*/		

//		 free_space = 0;
//		free_space = FSFreeSpace(0,&REC_fserr);	
/*
	    if(free_space<=(MIN_DISK_SPACE/512)) 
	    	{
	    	    rp_flag &= 0x0004;
	    		return FALSE;
	    	}*/
/*
	// Craete a file for writing.
			digi[0] = '0'+Rp_Index/10000;
			digi[1] = '0'+((Rp_Index -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((Rp_Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((Rp_Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+Rp_Index%10;
			digi[5] = 0;

    		strcpy(rp_path,dir_path);
			strcat(rp_path,digi);
			strcat(rp_path,".wav");
			
  if(flag){		
			if(FSGetFileSize(path,&REC_fserr)>0)
			 	{
			 	  ADPCM_File = FSOpen(path,FS_MODE_READ_WRITE,&REC_fserr);  
			 	}
			 else
			 	{
			 	 ADPCM_File = FSOpen(path,FS_MODE_CREATE,&REC_fserr); 
			 	}
			  if(REC_fserr)
			    {
			        WavRecState = WAV_ERROR;
			        return FALSE;
			    }
			 free_space = free_space*512U;
			wav_space = BEGIN_RECORD_SPACE;
			FSResize(ADPCM_File, wav_space);
			OP_CLOSE();
			
  	}
  else{
  		if(FSGetFileSize(path,&REC_fserr)==0)
			{
			    return FALSE;
			}
		ADPCM_File = FSOpen(path,FS_MODE_READ,&REC_fserr);
		if(REC_fserr)
		    {
		        WavRecState = WAV_ERROR;
		        return FALSE;
		    }
  	}
 */ 
#ifdef TX_DEBUG
	SendString((char *)"\r\ncreate ok!");
#endif
	 return TRUE;
}


