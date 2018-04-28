//--------------------------------------------------------------------
#include "KBDM9_DDK.H"
//--------------------------------------------------------------------
#include "sfr.h"
#include "string.h"
#include "MP3_API.H"
#include "FileSearch.h"
//#include "BinaryImage.h"
#include "com.h"
//--------------------------------------------------------------------
#pragma CODE = MAIN_CODE
#pragma DATA = MAIN_DATA
//--------------------------------------------------------------------
void Sys_Play( char* Mp3 );
//--------------------------------------------------------------------
int SysVol;
__Y long Now1, Max1, Now2, Max2;
//--------------------------------------------------------------------
void main( void )
{
	int len;
	FS_ERR fserr;
	unsigned long Sector, Len;

	IP_INIT();

#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB Mp32 begin!");
#endif	
	FSInit();
	fserr = FS_mount( 0, &SD1_Drive );

	if( fserr != FS_NO_ERROR ){
#ifdef  TX_DEBUG
		SendString(( char *)"\r\nsd1 fail!");
#endif
		}

	fserr = FS_mount( 1, &SD2_Drive );
	if( fserr != FS_NO_ERROR ){
#ifdef  TX_DEBUG
		SendString(( char *)"\r\nsd2 fail!");
#endif
		}

#ifdef  TX_DEBUG
		SendString(( char *)"\r\nmount sd ok!");
#endif
//	GetDataIdInfo( D_128, (__X unsigned long*)&Sector, (__X unsigned long*)&Len );

	Now1 = 0;
	Now2 = 0;
/*
	SysVol = 12;
	Find_Initial( FILE_LEVEL, ".mp3" );
	Find_PathFile( cFileName1, FILE_MODE_ALL, "0:\\" );
	if( (Now1>=Max1) || (Now1<0) )
		Now1 = 0;
	Max1 = lFileMax;
	Find_PathFile( cFileName2, FILE_MODE_ALL, "1:\\" );
	if( (Now2>=Max2) || (Now2<0) )
		Now2 = 0;
	Max2 = lFileMax;
	*/
	strcpy(cFileName1,"0:\\1.mp3");
	strcpy(cFileName2,"1:\\1.mp3");
	while(1)
	{
	
	Sys_Play( cFileName1 );
	Sys_Play( cFileName2 );
/*	
		if( Max1 )
		{
			lFileNow = Now1++;
			Find_FileName( cFileName1, NULL_LFN );
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nmp3_name=");
	SendString(cFileName1);
#endif	
			Sys_Play( cFileName1 );
			if( Now1 == Max1 ) Now1 = 0;
		}
		if( Max2 )
		{
			lFileNow = Now2++;
			Find_FileName( cFileName2, NULL_LFN );
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nmp3_name=");
	SendString(cFileName1);
#endif				
			Sys_Play( cFileName2 );
			if( Now2 == Max2 ) Now2 = 0;
		}
*/		
	}
}
//--------------------------------------------------------------------
void Sys_Play( char* Mp3 )
{
	int  delay;
	tAudioErr Audio_err;

	Audio_err = AudioMP3Init(SysVol);
	DAC_Enable_HeadPhone();
	if( Audio_err != AUDIO_ERROR_OK ) return;
	if( AudioMP3Open( Mp3 ) == AUDIO_ERROR_OK )
	{
//		AudioMP3SetEQ(EQ_NOMAL);
//		AudioMP3SetPlayTime( 180*1000L );
		DAC_Enable_Speaker();
		DAC_Enable_HeadPhone();
		DAC_HeadPhone_Volume(20);
		DAC_Speaker_Volume(10);
		if( AudioMP3Play() == AUDIO_ERROR_OK )
		{
//			AudioMP3SetPlayTime( 60*1000L );
			while( AudioMP3ChkPlaying() )
			{
//				AudioMP3CkBuffer();
			}
		}
		AudioMP3Stop();
		AudioMP3Close();
	}
	AudioMP3Uninit();
}
//--------------------------------------------------------------------
