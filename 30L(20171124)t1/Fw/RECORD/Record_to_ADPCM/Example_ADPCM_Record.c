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
//#include "KEY_SCAN.H"
#include "SYSTEM.H"
#include "SDImg.h"
#include "com.h"
#include "MP3_API.H"
#include "mp3_include.h"
#include "keypad.h"
#include "oid.h"
#include "conf_oid.h"

#define RECORDER_DIR   "0:\\Recorder\\"

#if( OID_VERSION == OID1 )
#define DIR_1      "0:\\REC\\"

#define STOP_INDEX  4286
#endif

#if( OID_VERSION == OID2 )
#define DIR_1      "0:\\REC\\REC_1\\"
#define DIR_2      "0:\\REC\\REC_2\\"
#define DIR_3      "0:\\REC\\REC_3\\"
#define DIR_4      "0:\\REC\\REC_4\\"

#define STOP_INDEX  64765
#endif

#define MAIN_FILE_NAME "rec_"
#define MAX_LENGTH	(32UL*1024*1024)
#define MIN_DISK_SPACE  (1UL*1024*1024)

#define MAX_WAV_NUM	  100

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

#pragma CODE = MAIN_CODE
#pragma DATA = MAIN_DATA

#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE		((PCM_BUF_SIZE-1)/4+2)			// word


tLED LED_state;

extern unsigned int gflag;
extern unsigned int vol_idx;
extern unsigned int ap4_mp3_flag;
extern unsigned int volume_inc_flag;
extern unsigned int bak_current_index;
extern unsigned int begin_R;
extern unsigned int over_R;
extern unsigned int begin_P;
extern unsigned int over_P;
extern unsigned int g_string2[30];


extern const tFSDrvHandle SD_Cache_Drive;


unsigned int 	WRITE_SIZE;
__Y int ADC_Buf1[PCM_BUF_SIZE];
__Y int ADC_Buf2[PCM_BUF_SIZE];

__X int PCM_Buf[PCM_BUF_SIZE];
__X int ADPCM_Buf[ADPCM_BUF_SIZE];

int NotifyFlag, NotifyError;
__X int *NotifyBuf;

unsigned long WavWriteSize;
__X FS_ERR fserr;
__X FCFILE *ADPCM_File;   
tAUADC_Err Error;
__X tREC_State REC_state;

__X int PCM_Buf1[PCM_BUF_SIZE];
__X int PCM_Buf2[PCM_BUF_SIZE];
unsigned long  Frame_Count;
unsigned int PCM_Buffer_Size, ADPCM_Buffer_Size, Sample_Rate;

extern unsigned long free_space;
unsigned int  Wav_Count;
		 char dir_path[256];
extern int DAC_Out; // 0: Headphone, 1: Speaker

__X  Bool       		 index_flag;
__X  U16                 Current_Index;

//__X  U16                 RP_flag;

ImageDeclareType(AUADC_COMMON_CODE);
ImageDeclareType(RECORD_FILE_CODE);

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
void Key_Process(U16 Key_Code);    
void RecStart(void);
void RecProcess(void);
void RecStop(void);
void WavPlay(void);
void WavStop(void);
void FillBuffer(void);
void FirstDecode(void);
void led_control(void);
//void SpeakDetect(void);
/*void ErrHappen(const char *string,unsigned int fserr)
{
    while(1);
}*/

void main(void)
{
	int drive, status;
	unsigned int i;
    U16 temp_key=0;
    unsigned int count;

	char *ptr;

	
	IP_INIT();
 	GPIO_init();
 	ADC_init();
	PowerKeepOn();
	SystemClock(FEQ_60M);
	SDKLoadCodInit();  
	SetVector(EXCEPTION_TRAP,Exception);
    Wav_Count = 0;
//	RP_flag = 0;
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"KB record begin!\r\n");
#endif
	

	
	for(i=0; i<PCM_BUF_SIZE; i++){
		ADC_Buf1[i] = 0;
		ADC_Buf2[i] = 0; 	
	}  

	REC_state = STOP;
	System_Timer_Init(3,TIME_TICK);  
	FSInit(1);
	
#if USE_SD
	fserr = FS_mount(&SD_Cache_Drive);
#ifdef  TX_DEBUG
		if( fserr == FS_DRIVE_ERROR ) 
			{
			SendString(( char *)"The File system initial fail.");
			__asm{RESET};
			}
#endif	
#endif

#if USE_NF
	fserr = FS_mount(&NF_Drive);	
#ifdef  TX_DEBUG
		if( fserr == FS_DRIVE_ERROR ) 
			{
			SendString(( char *)"The File system initial fail.");
			__asm{RESET};
			}
#endif
#endif
	
    fserr = FSDirectory(RECORDER_DIR);
	
    kbd_init_process();
#if 0	
	reinit_oid_pen();
#endif	
	// Initial Audio ADC
	SDKLoadCode(AUADC_COMMON_CODE); 
/*
	if(volume_inc_flag&0x8000)
		{
		 volume_inc_flag &= 0x7FFF;
		 index_flag = TRUE;
		 RP_flag |= 0x8000;
		 Current_Index = bak_current_index;
         ptr = strchr((char *)g_string2,'.');
		 *ptr = '\0';
		 
		 strcpy(dir_path,SD_DEVICE);
		 strcat(dir_path,(char *)g_string2);
		 strcat(dir_path,"\\");

		 fserr = FSDirectory(dir_path);
		 
		}
*/	
	BlueLed_ON();
	led_start_timer(LED_NORMAL_TIME);
	while(1)
	{

		if(TimerCount%(50/TIME_TICK) == 0)
			CheckUSB();
		if((KBD_IDLE == kbd_state)&&(TimerCount%(500/TICK_PERIOD) == 0))
	    	BatteryDetect();

	   // Keypad process
//	   CheckUSB();
#if 0	   
	   scan_oid();
	   if(index_flag)
	   	{
	   		index_flag=FALSE;
#ifdef  TX_DEBUG  			
			SendString("\r\ncurrent_index=");
			send_hex_u16(Current_Index);
#endif
	   	}
#endif	   
	   kbd_process();
	   temp_key = kbd_key_read();
	   if(0!=temp_key)
	   	{   
#ifdef  TX_DEBUG   	
	   	    SendString("\r\ntemp_key=");
	   	    send_hex_u16(temp_key);
#endif		
	   		Key_Process(temp_key);
	   	}

	   
		if(REC_state == REC)
        	RecProcess();
        else if(REC_state == PLAY)
        {
	        SpeakDetect();
	        if(Frame_Count == 0)
	        	WavStop();
		}	
	}   
}	

void RecStart(void)
{
	char path[256];
	char digi[3];
//	int file_count;
//	unsigned long size;
	free_space = 0;
	free_space = FSFreeSpace(0,&fserr);	
    if(free_space<=(MIN_DISK_SPACE/512)) 
    	{
    		return;
    	}
	// Craete a file for writing.
//	file_count = 0;
		while(1){
	//		SendString("\r\nWav_Count=");
	//		send_hex_u16(Wav_Count);
			digi[0] = '0'+Wav_Count%10;
			digi[1] = '0'+Wav_Count/10;
			digi[2] = 0;
			strcpy(path,RECORDER_DIR);
			strcat(path,MAIN_FILE_NAME);
			strcat(path,digi);
			strcat(path,".wav");
			if(FSGetFileSize(path,&fserr)>0)
			{
				if(Wav_Count==MAX_WAV_NUM)
					break;
				Wav_Count++;
				continue;
			}
			else
			{
				break;
			}
		}

//		SendString("\r\npath=");
//		send_hex_u16(Wav_Count);	
	ADPCM_File = FSOpen(path,FS_MODE_CREATE,&fserr);    
#ifdef  TX_DEBUG	
	if(fserr)
		SendString("\r\nFail to open file from SD card.\r\n");
#endif	
    if(free_space>(MAX_LENGTH/512))  
		FSResize(ADPCM_File, MAX_LENGTH);
	else       
		FSResize(ADPCM_File, free_space*512U);
		
    OP_CLOSE();
	REC_state = REC;
	
	// Initial ADPCM encoder
	Sample_Rate = 8000;										// 8KHz	
	WavWriteSize = 0;
			
	IMA_ADPCM_Encoder_Init(Sample_Rate);

	// write 60 bytes file header  
	WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);  

	NotifyError = 0;
	NotifyFlag = 0;   

	// Set Audio ADC parameters      
	AUADC_Init(MIC_In, DTU1,	AUADC_SAMPLE_RATE_8K, Buffer_Full_Func);
	AUADC_Set_Buffer((__X int *)ADC_Buf1, (__X int *)ADC_Buf2, PCM_BUF_SIZE);
 
	AUADC_MicGain(MIC_Gain_20dB);  
	AUADC_InputGain(AUADC_Gain_22P5dB);

	Error = AUADC_Start();
}

void RecProcess(void)
{
	if (NotifyFlag > 0)
	{
		NotifyFlag--;
	 
		memcpy(PCM_Buf, NotifyBuf, PCM_BUF_SIZE*2);
		IMA_ADPCM_Encoder(PCM_Buf, ADPCM_Buf);	
	
		WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_Buf, ADPCM_BUF_SIZE*2, &fserr);      

		if ((WavWriteSize > 1024UL*61440UL)|| ((free_space-WavWriteSize)< 1024UL*32UL)) {
			RecStop();
		}				    
	}
}

void RecStop(void)
{

	REC_state = STOP;
	AUADC_Stop();
    
	FSResize(ADPCM_File, WavWriteSize);   

	FSSeek(ADPCM_File, 0);
		
	FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);	
	     
	fserr = FSClose(ADPCM_File);
#ifdef  TX_DEBUG  
	if(fserr)
		SendString("\r\nClose file(SD) is fail.\r\n");
#endif	
}

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
{	
	if (NotifyFlag > 0){
		
		NotifyError = 1;
	}	
	NotifyFlag++;
	
	NotifyBuf = buffer;
}

void WavPlay(void)
{
	char path[256];
	char digi[3];
//	int file_count;
	unsigned int temp_wav_count;
	int error;
	unsigned long read_length;
    tIMA_INFO ADPCM_Info;

	temp_wav_count = Wav_Count;
//	file_count = 00;
	while(1){
		digi[0] = '0'+Wav_Count%10;
		digi[1] = '0'+Wav_Count/10;
		digi[2] = 0;
		strcpy(path,RECORDER_DIR);
		strcat(path,MAIN_FILE_NAME);
		strcat(path,digi);
		strcat(path,".wav");
		if(FSGetFileSize(path,&fserr)==0)
		{
			if(Wav_Count==MAX_WAV_NUM)
				Wav_Count=0;
			else
				Wav_Count++;

			if(temp_wav_count==Wav_Count)return;
			continue;
		}
		break;
	}
 	
//	SendString("\r\nplay_wav=");
//	send_hex_u16(Wav_Count);	
//	LED_param = 200;
	PCM_Buffer_Size	= PCM_BUF_SIZE;
	ADPCM_Buffer_Size = ADPCM_BUF_SIZE;	
	ADPCM_File = FSOpen(path,FS_MODE_READ,&fserr);
#ifdef  TX_DEBUG 	
    if(fserr)
		SendString("\r\nFail to open file from SD Card.\r\n");
#endif	
 //read the 60-byte file header 
 	read_length = FSRead(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);
 
 // Initial ADPCM  Decoder 
 
	error = IMA_ADPCM_Decoder_Init(&ADPCM_Info);  
	        
	if(error == 0){
		Sample_Rate = ADPCM_Info.IMA_SR;										// Sampling rate for playing the decoded PCM samples 	
		PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;				// buffer size for output PCM
		ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;		// buffer size for input ADPCM
		Frame_Count = ADPCM_Info.IMA_Frame_No;							// Tell the user how many frame shall be decoded.
	}
   // Initial DAC 

  	DAC_Init(DTU3, DTU2, FillBuffer, 0);
  
	DAC_LChannelBuffer1 = PCM_Buf1;
	DAC_RChannelBuffer1 = PCM_Buf1;
	DAC_LChannelBuffer2 = PCM_Buf2;
	DAC_RChannelBuffer2 = PCM_Buf2;    
  	DAC_FrameSize = PCM_Buffer_Size;     
														
  	FirstDecode();
	SpeakDetect();
/*  
	if(SpeakerDET())
	{
		DAC_Out = SPEAKER;
		DAC_Speaker_Volume(VolTbl[vol_idx]/2);
		DAC_Enable_Speaker();
	}
	else
	{
		DAC_Out = HEADER_PHONE;
		DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
		DAC_Enable_HeadPhone();         //  Headphone output
	}
*/
	REC_state = PLAY;   
	DAC_Go();
}

/*
void SpeakDetect(void)
{
	if(SpeakerDET() && DAC_Out == HEADER_PHONE)
	{
		DAC_Out = SPEAKER;
		DAC_Disable_HeadPhone();
		DAC_Enable_Speaker();
		DAC_Speaker_Volume(VolTbl[vol_idx]/2);
	}
	else if(!SpeakerDET() && DAC_Out == SPEAKER)
	{
		DAC_Out = HEADER_PHONE;
		DAC_Disable_Speaker();
		DAC_Enable_HeadPhone();
		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
	}
}
*/
void WavStop(void)
{
	DAC_Pause();
	DAC_Mute();

	FSClose(ADPCM_File);
	REC_state = STOP;

}

void FillBuffer(void)
{
	unsigned long len;
	__X int *BufPtr;
	void (*Fill_Buf)(void);
	
	if(DAC_Buffer1Empty()){
		BufPtr = DAC_LChannelBuffer1 ;
	  Fill_Buf = DAC_FillBuffer1;	    
	}else if(DAC_Buffer2Empty()) {
		BufPtr = DAC_LChannelBuffer2 ;
		Fill_Buf = DAC_FillBuffer2;
	}
	
	len = FSRead(ADPCM_File, (void *)ADPCM_Buf, ADPCM_Buffer_Size*2, &fserr);
	
	IMA_ADPCM_Decoder(ADPCM_Buf, BufPtr);
	
	(*Fill_Buf)();
	
	Frame_Count-- ;	
}			  	

const unsigned int SR2PCMSR_TBL[] ={(unsigned int)96000,
	                                  (unsigned int)48000, 
                                    (unsigned int)44100, 
                                    (unsigned int)32000, 
                                    (unsigned int)24000, 
                                    (unsigned int)22050, 
                                    (unsigned int)16000, 
                                    (unsigned int)12000, 
                                    (unsigned int)11025, 
                                    (unsigned int)9600,
                                    (unsigned int)8000  };
                                    
static unsigned int SmpRate2PCMSR(unsigned int smp_rate)
{
    int i;
    for (i=0; i<sizeof(SR2PCMSR_TBL)/sizeof(unsigned int); i++)
    {
        if (SR2PCMSR_TBL[i] == smp_rate)
            return i;
    }
    return DAC_SAMPLE_RATE_44P1K;
}

void FirstDecode(void)
{
	unsigned long len;
	
	DAC_SampleRate=SmpRate2PCMSR(Sample_Rate);
		     
	len = FSRead(ADPCM_File, (void *)ADPCM_Buf, ADPCM_Buffer_Size*2, &fserr);
	
	IMA_ADPCM_Decoder(ADPCM_Buf, DAC_LChannelBuffer1);
		
	DAC_FillBuffer1();
}
////===========================================================================
///-----------------------------------
void led_control( void )
{
	if((REC_state == REC))
		{
			BlueLed_ON();
			RedLed_ON();
		}
	else
		{
		if(led_is_timer_timeout())
			{
			 BlueLed_FLASH();
			 RedLed_OFF();
			 led_start_timer(LED_NORMAL_TIME);
			}
		}
}
//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Key_Process(U16 Key_Code)
{
    switch(Key_Code)
    {
    	case (KBD_KEY_RELEASED<<8|KEY_POWER): // rec pause
          	if(REC_state == STOP)
        		RecStart();  		
            break;
		case (KBD_KEY_RELEASED<<8|KEY_MODE): // 
        	if(REC_state == REC)
        		RecStop();
        	else if(REC_state == STOP)
        		WavPlay();
        	else if(REC_state == PLAY)
        		WavStop();
            break;	
//		case (KBD_KEY_REPEAT<<8 |KEY_VOL_DEC):	
        case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // ap4 Volume change
        	if(REC_state == PLAY)
        	{
#if 1        
			if(1== volume_inc_flag)	
				{
					vol_idx++;
					if(vol_idx > 4)
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
#endif			
			DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
			DAC_Enable_HeadPhone();
			}
            break;

		case (KBD_KEY_LONGPRESSED<<8|KEY_POWER):
        	if(REC_state == REC)
        		RecStop();
        	else if(REC_state == PLAY)
        		WavStop();
			 PowerTurnOff();
			 if(USB_DET())
				{
					CommonLoadCode(C_OPEN);
				}
			while(PWD_Key_Press());
			Delay_ms(5);
			CommonLoadCode(C_OPEN);
			 break;
		case (KBD_KEY_LONGPRESSED<<8|KEY_MODE):
        	if(REC_state == REC)
        		RecStop();
        	else if(REC_state == PLAY)
        		WavStop();
			 CommonLoadCode(C_OID);
			 break;

		case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC):
			if((REC_state == REC)||(REC_state == PLAY))
        		RecStop();
			  Wav_Count++;
			  WavPlay();
			  break;	
        default:
            break;
    }	
}
#pragma CODE = RECORD_FILE_CODE