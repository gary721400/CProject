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

#define RP_DIR   "0:\\RP\\"

#define FLAG_RECORD   	0X1111
#define FLAG_PLAY		0X2222

//DEFINE  ALL BITS OF AP4_MP4_FLAG 
#define BANK_FORM_MP3       	0X8000
#define BANK_FORM_RP            0X4000
#define BANK_HAVE_INDEX     	0X2000
#define CURRENT_CONTROL_GAME 	0X1000

#if( OID_VERSION == OID1 )
#define STOP_INDEX  4286
#endif

#if( OID_VERSION == OID2 )
#define STOP_INDEX  64765
#endif

#define MAIN_FILE_NAME "rec_"
#define MAX_LENGTH	(32UL*1024*1024)
#define MIN_DISK_SPACE  (1UL*1024*1024)

#define MAX_WAV_NUM	  200

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

#pragma CODE = MAIN_CODE
#pragma DATA = MAIN_DATA

#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE		((PCM_BUF_SIZE-1)/4+2)			// word

enum{
	IDLE	 = 0,
    WAV_INIT = 1,
    WAV_PLAY = 2,
    WAV_OVER = 3
};



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

unsigned long free_space;
unsigned int  Wav_Count;
char 	 tishi_file_name[32];
		 char dir_path[256];
extern int DAC_Out; // 0: Headphone, 1: Speaker

__X  Bool       		 index_flag;
__X  U16                 Current_Index;
__X  U16                 Index;
__X  U16 				 State_RP_process;

//__X  U16                 RP_flag;

ImageDeclareType(AUADC_COMMON_CODE);

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
void Key_Process(U16 Key_Code);    
void RecStart(void);
void RecProcess(void);
void RecStop(void);
void WavPlay(void);
void WavStop(void);
void FillBuffer(void);
void FirstDecode(void);
U16	 RP_judge(void);
void Rec_process_init(void);
void wav_play_process_init(void);
void RP_process(void);
//void SpeakDetect(void);
/*void ErrHappen(const char *string,unsigned int fserr)
{
    while(1);
}*/


const int PlayStatusTable[] = {PLAYING_STATUS,
                         PAUSE_STATUS,
                         AB_REPEAT_STATUS,
                         A_STAGE_STATUS,
                         FWRWD_PROLOG_STATUS,
                         FORWARD_STATUS,
                         REWIND_STATUS,
                         FWRWD_EPILOG_STATUS,
                         (-1)}; 
int CheckPlaying(void)
{
    tMusicStatus status;
    int i = 0;

	if(DAC_CCMC_Status != 0)
	{
#ifdef  TX_DEBUG	
		SendString("dac reset\r\n");
#endif
    	DAC_Headphone_Reset();
    	DAC_Enable_HeadPhone();
    	DAC_HeadPhone_Volume(VolTbl[vol_idx]); // 
    	DAC_Go();
	}
    SpeakDetect();
    status = AudioMP3Status();
    while(1)
    {
        if(PlayStatusTable[i] == (-1))
            break;
        else if(PlayStatusTable[i] == status)
            return (-1);
        else
            i++;
    }
    return 0;
}


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
	SendString(( char *)"KB RP record begin!\r\n");

#endif
	
	BlueLed_ON();

//	begin_P = 65535;
//	begin_P = 65525;
//	bak_current_index = 65530;
//	strcpy((char *)g_string2,"test.ap4");
	
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
//    fserr = FSDirectory("0:\\RP");
	

	
	// Initial Audio ADC
	SDKLoadCode(AUADC_COMMON_CODE); 
	kbd_init_process();
	reinit_oid_pen();
	index_flag=FALSE;
#if 1
//	if(volume_inc_flag&0x8000)
		{
		 volume_inc_flag &= 0x7FFF;
		 index_flag=FALSE;
		 Index = bak_current_index;
         ptr = strchr((char *)g_string2,'.');
		 if(ptr!=0)
		 	{
		 		*ptr = '\0';
		 	}
		 
		 strcpy(dir_path,RP_DIR);
		 fserr = FSDirectory(dir_path);
		 
		 strcat(dir_path,(char *)g_string2);
		 strcat(dir_path,"\\");

		 fserr = FSDirectory(dir_path);
		 
		}
#ifdef  TX_DEBUG  			
			SendString("\r\begin_R:over_R:begin_P:over_P=");
			send_hex_u16(begin_R);
			SendString(":");
			send_hex_u16(over_R);
			SendString(":");
			send_hex_u16(begin_P);
			SendString(":");
			send_hex_u16(over_P);
	
#endif
	 if(FLAG_RECORD == RP_judge())
	 	{
	 		RecStart();
	 	}
	 else
	 	{
	 		WavPlay();
	 	}
#endif	
	while(1)
	{

		if(TimerCount%(50/TIME_TICK) == 0)
			CheckUSB();
		if(TimerCount%(100/TIME_TICK) == 0)
	    	BatteryDetect();

	   // Keypad process
//	   CheckUSB();
///*	   
	   scan_oid();
	   if(index_flag)
	   	{
	   		index_flag=FALSE;
			Index = Current_Index;
#ifdef  TX_DEBUG  			
			SendString("\r\ncurrent_index=");
			send_hex_u16(Index);
#endif
#if 1
            if(REC_state == REC)
        		RecStop();
			if(REC_state == PLAY)
        		WavStop();
            if(REC_state == STOP)
            	{
            	    switch(RP_judge())
            	    	{
            	    	 case FLAG_RECORD:
						 	RecStart();
						 	break;
						 case FLAG_PLAY:
						 	WavPlay();
						 	break;
						 default:
						 	ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP|BANK_HAVE_INDEX);
							bak_current_index = Index;
					 	  	CommonLoadCode(C_OID);
						 	break;
            	    	}
            	}		
#endif
	   	}
#if 0   
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
#endif
	   
		if(REC_state == REC)
        	RecProcess();
        else if(REC_state == PLAY)
        {
	        SpeakDetect();
	        if(Frame_Count == 0)
	        	{
	        		WavStop();
					ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
					CommonLoadCode(C_OID);
	        	}
		}	
	}   
}	

void RP_process(void)
{
	switch(State_RP_process)
		{
			case IDLE:
				{
					break;
				}
			case WAV_INIT:
				break;
			case WAV_PLAY:
				break;
			case WAV_OVER:
				break;
			default:
				break;
		}
}

void tishi_wav_play(void)
{

	    FS_ERR fs_err;
	    tAudioErr   Audio_err;
        Audio_err = AudioMP3Init();
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 return;
        	}
        Audio_err = AudioMP3Open((signed char *)tishi_file_name);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
				return;
        	}
        AudioMP3SetEQ(EQ_NOMAL);
		SpeakDetect();
        Audio_err = AudioMP3Play();
    	while (CheckPlaying())
    	{    		
	    	
    	}
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();



}

void RecStart(void)
{
	char path[256];
	char digi[6];
//	int file_count;
//	unsigned long size;
	free_space = 0;
	free_space = FSFreeSpace(0,&fserr);	
    if(free_space<=(MIN_DISK_SPACE/512)) 
    	{
    		ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
			CommonLoadCode(C_OID);
    	}
	// Craete a file for writing.
//	file_count = 0;
//	if(RP_flag&0x8000)
	{
			digi[0] = '0'+Index/10000;
			digi[1] = '0'+((Index -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+Index%10;
			digi[5] = 0;
//			strcpy(path,SD_DEVICE);
//			strcat(path,MAIN_FILE_NAME);
    		strcpy(path,dir_path);
//			strcat(path,MAIN_FILE_NAME);
			strcat(path,digi);
			strcat(path,".wav");
			if(FSGetFileSize(path,&fserr)>0)
			{
			   FSDelete(path);
			}
	}
	
#ifdef  TX_DEBUG 	
		SendString("\r\nrec_star=");
		SendString(path);
#endif
	
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

	Rec_process_init();

}



void WavPlay(void)
{
	char path[256];
	char digi[6];
//	int file_count;
	int error;
	unsigned long read_length;
    tIMA_INFO ADPCM_Info;


			digi[0] = '0'+Index/10000;
			digi[1] = '0'+((Index -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((Index -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+Index%10;
			digi[5] = 0;
//			strcpy(path,SD_DEVICE);
//			strcat(path,MAIN_FILE_NAME);
    		strcpy(path,dir_path);
//			strcat(path,MAIN_FILE_NAME);
			strcat(path,digi);
			strcat(path,".wav");
#ifdef  TX_DEBUG 	
			SendString("\r\nwav_play=");
			SendString(path);
#endif
			if(FSGetFileSize(path,&fserr)==0)
			{
			    ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
				CommonLoadCode(C_OID);
			}
 	

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

	REC_state = PLAY;  
	
	wav_play_process_init();
	
}





//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Key_Process(U16 Key_Code)
{
    switch(Key_Code)
    {
    	case (KBD_KEY_RELEASED<<8|KEY_POWER): // rec pause
//          	if(REC_state == STOP)
//        		RecStart();  		
            break;
		case (KBD_KEY_RELEASED<<8|KEY_MODE): // 
        	if(REC_state == REC)
        		RecStop();
//        	else if(REC_state == STOP)
//        		WavPlay();
//        	else if(REC_state == PLAY)
//        		WavStop();
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
			/*
        	if(REC_state == REC)
        		RecStop();
        	else if(REC_state == PLAY)
        		WavStop();

			 ap4_mp3_flag |= 0x8000;
			 CommonLoadCode(C_OID);
			 */
			 break;

		case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC):
			/*
			if((REC_state == REC)||(REC_state == PLAY))
        		RecStop();
			  Wav_Count++;
			  WavPlay();
			  */
			  break;	
        default:
            break;
    }	
}

U16	RP_judge()
{
	if((Index >= begin_R)&&(Index <= over_R))
		{
		 return FLAG_RECORD;
		}
		
	if((Index >= begin_P)&&(Index <= over_P))
		{
		 Index = Index-(over_P-begin_P+1);
		return FLAG_PLAY;
		}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//				rec process
/////////////////////////////////////////
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

void Rec_process_init(void)
{
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
			ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
			CommonLoadCode(C_OID);
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

//////wav play process

void wav_play_process_init(void)
{
	// Initial DAC 

  	DAC_Init(DTU3, DTU2, FillBuffer, 0);
  
	DAC_LChannelBuffer1 = PCM_Buf1;
	DAC_RChannelBuffer1 = PCM_Buf1;
	DAC_LChannelBuffer2 = PCM_Buf2;
	DAC_RChannelBuffer2 = PCM_Buf2;    
  	DAC_FrameSize = PCM_Buffer_Size;     
														
  	FirstDecode();
	SpeakDetect();
    DAC_Go();
}

void FirstDecode(void)
{
	unsigned long len;
	
	DAC_SampleRate=SmpRate2PCMSR(Sample_Rate);
		     
	len = FSRead(ADPCM_File, (void *)ADPCM_Buf, ADPCM_Buffer_Size*2, &fserr);
	
	IMA_ADPCM_Decoder(ADPCM_Buf, DAC_LChannelBuffer1);
		
	DAC_FillBuffer1();
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

void WavStop(void)
{
	DAC_Pause();
	DAC_Mute();

	FSClose(ADPCM_File);
	REC_state = STOP;

}



