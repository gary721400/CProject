/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3API.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

//------------------------------------------------------------------------------
#include "lib_mcu\compiler.h"                // Compiler definitions

#include "mp3_include.h"
#include "string.h"
//#include "SYSTEM.H"
#include "fs.h"
//#include "power.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\file_system\file.h"           // for file management 
//#include "com.h"

//------------------------------------------------------------------------------
// Option
//------------------------------------------------------------------------------


//#define	USE_HeadPhone	1		//1: Use HeadPhone, 0:Use Speaker
//extern __X char CurrentShortFilePath[512];
extern   FCFILE   __X   **fat_g_cur_file;
extern   Bool           g_b_play_ap3;

//extern U32   _MEM_TYPE_MEDSLOW_   g_timer_tick;

//------------------------------------------------------------------------------
// Equalizer Table
//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE01
//------------------------------------------------------------------------------
const unsigned int eqConstTBL[ EQ_MAX ][ EQ_MAX_BAND ] = {
   { 0xDE70, 0xB2BC, 0x5492, 0x4728, 0x5FFD, 0xAAB1, 0xF11B, 0xFFFF, 0xFFFF, 0xFFFF }, //Rock             4.8 	 2.9 	-3.6 	-5.1 	-2.5 	 2.5 	 5.5 	 6.7 	 6.7 	 6.7 
   { 0x6E35, 0xB2BC, 0xD46D, 0xD46D, 0xB904, 0x7215, 0x693F, 0x693F, 0x6E35, 0x6E35 }, //Pop				-1.3 	 2.9 	 4.4 	 4.4 	 3.2 	-1.0 	-1.7 	-1.7 	-1.3 	-1.3 
   { 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x49A8, 0x49A8, 0x49A8, 0x3DF9 }, //Classical		 0.0	 0.0	 0.0	 0.0	 0.0	 0.0 	-4.8,	-4.8	-4.8	-6.3
   { 0x3DF9, 0x3DF9, 0x3DF9, 0x5BAB, 0x9BAC, 0xE24D, 0xE10C, 0xD3A3, 0xD200, 0xD200 }, //Soft			-6.3 	-6.3 	-6.3 	-2.9 	 1.7 	 6.7 	 9.7 	 9.7 	 9.7 	10.5 
   { 0xB286, 0x9FC9, 0x874C, 0x9000, 0x9000, 0x6689, 0x6101, 0x6101, 0xC000, 0xC000 }, //Jazz
   { 0xB286, 0xB286, 0xB286, 0x9B6A, 0x874C, 0x6C62, 0x5943, 0x56D2, 0x56D2, 0x56D2 }  //Bass
};

//------------------------------------------------------------------------------
#pragma DATA = MP3_API_XRAM
//------------------------------------------------------------------------------
unsigned int   eqUserTBL[ EQ_MAX_BAND ]; 
tAudioEQ_Info  eqInfo;
tAudioHandle   Audio_Handle;
extern int     DAC_Out; // 0: Headphone, 1: Speaker


//------------------------------------------------------------------------------
#pragma CODE = MP3_TEMP_CODE03
//------------------------------------------------------------------------------
void MP3API_CustomInit( void )
{

   pMP3API_FSOpen = _FSOpen;
   pMP3API_FSRead = FSRead;
   pMP3API_FSClose = FSClose;
   pMP3API_FSSeek = FSSeek;

   eqInfo.EQ_Max = EQ_MAX;
   eqInfo.EQ_Band = EQ_MAX_BAND;
   eqInfo.EQ_ConstTBL = ( const unsigned int **)eqConstTBL;
   eqInfo.EQ_UserTBL = eqUserTBL;

   memset( (char*)0x8000, -1, 4858 );
}


void OIDMP3API_CustomInit( void )
{

   pMP3API_FSOpen = _OID_FSOpen;
   pMP3API_FSRead = _OID_FSRead;
   pMP3API_FSClose = _OID_FSClose;
   pMP3API_FSSeek = _OID_FSSeek;
   eqInfo.EQ_Max = EQ_MAX;
   eqInfo.EQ_Band = EQ_MAX_BAND;
   eqInfo.EQ_ConstTBL = ( const unsigned int **)eqConstTBL;
   eqInfo.EQ_UserTBL = eqUserTBL;

   memset( (char*)0x8000, -1, 4858 );
}


tAudioErr Audio_MP3_Init( void )
{
   if( !g_b_play_ap3 )
   {
      MP3API_CustomInit();
   }
   else
   {
      OIDMP3API_CustomInit();
   }

   return AudioMP3Init_( 0 );
}


//------------------------------------------------------------------------------
#pragma CODE = MP3_COMMON_CODE
//------------------------------------------------------------------------------
tAudioErr AudioMP3Init( void )
{
   //LoadMP3InitCode();
   //LoadMP3Temp3Code();
   SDKLoadCode( MP3_TEMP_CODE03 );
#if 0
   if( !g_b_play_ap3 )
   {
      MP3API_CustomInit();
   }
   else
   {
      OIDMP3API_CustomInit();
   }
   return AudioMP3Init_( 0 );
#else
   return Audio_MP3_Init();
#endif
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Uninit( void )
{
   //LoadMP3InitCode();
   //LoadMP3Temp3Code();
   SDKLoadCode( MP3_TEMP_CODE03 );
   return AudioMP3Uninit_();
}

//------------------------------------------------------------------------------
tAudioErr AudioMP3Open( INT8S *filepath )
{
   //LoadMP3InitCode();
   //LoadMP3Temp0Code();
   SDKLoadCode( MP3_TEMP_CODE00 );
   return AudioMP3Open_( filepath );
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Close( void )
{
   DAC_Pause();
   //LoadMP3InitCode();
   //LoadMP3Temp3Code();
   SDKLoadCode( MP3_TEMP_CODE03 );
   return AudioMP3Close_();
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Pause( void )
{
   //LoadMP3Temp1Code();
   SDKLoadCode( MP3_TEMP_CODE01 );
   return AudioMP3Pause_();
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Stop( void )
{
   //LoadMP3Temp1Code();
   SDKLoadCode( MP3_TEMP_CODE01 );
   DAC_Pause();
   return AudioMP3Stop_();
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Continue( void )
{
   //LoadMP3Temp1Code();
   SDKLoadCode( MP3_TEMP_CODE01 );
   return AudioMP3Continue_();
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3SetEQ( tAudioEQ EQ_num )
{
   //LoadMP3Temp1Code();
   SDKLoadCode( MP3_TEMP_CODE01 );
   return AudioMP3SetEQ_( eqInfo, EQ_num );
}

///*
//------------------------------------------------------------------------------
INT32U AudioMP3GetPlayTime( tPlayTimeMode SetMode )
{
   //LoadMP3Temp4Code();
   return AudioMP3GetPlayTime_( SetMode );
}
//*/
//------------------------------------------------------------------------------
/*tAudioErr AudioMP3ABRepeat( void )
{
   LoadMP3Temp1Code();
   return AudioMP3ABRepeat_();
}*/


//------------------------------------------------------------------------------
#if 0
tAudioErr AudioMP3SetVol( int volume )
{
   LoadMP3Temp1Code();
//#if	USE_HeadPhone

   if(SpeakerDET())
   {
      DAC_Out = SPEAKER;
      //OP_OPEN();
      return AudioMP3SpeakerSetVol_( &volume );
   }
   else
   {
      DAC_Out = HEADER_PHONE;
      //OP_CLOSE();
      return AudioMP3HeadPhoneSetVol_( &volume );
   }
//#else
//#endif
}
//#else
tAudioErr AudioMP3SetVol( int volume )
{
   LoadMP3Temp1Code();
   //DAC_Out = HEADER_PHONE;
   /*if( SpeakerDET())
   {
      OP_OPEN();
   }
   else
   {
      OP_CLOSE();
   }*/
   return AudioMP3HeadPhoneSetVol_( &volume );
}
#endif

#if 0
//------------------------------------------------------------------------------
tAudioErr AudioMP3VolUp( int *get_volume )
{
   LoadMP3Temp1Code();
//#if	USE_HeadPhone
   //if( SpeakerDET())
   //   return AudioMP3SpeakerVolUp_( get_volume );
   //else     
      return AudioMP3HeadPhoneVolUp_( get_volume );
//#else
//#endif
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3VolDown( int *get_volume )
{
   LoadMP3Temp1Code();
//#if	USE_HeadPhone
   //if( SpeakerDET())     
   //   return AudioMP3SpeakerVolDown_( get_volume );
   //else    
      return AudioMP3HeadPhoneVolDown_( get_volume );
//#else
//#endif
}
#endif

//------------------------------------------------------------------------------
/*tAudioErr AudioMP3FwRwdProlog( void )
{
   LoadMP3Temp1Code();
   return AudioMP3FwRwdProlog_();
}*/

/*
//------------------------------------------------------------------------------
tAudioErr AudioMP3Forward( void )
{
   LoadMP3Temp1Code();
   return AudioMP3Forward_();
}
//*/

//------------------------------------------------------------------------------
/*tAudioErr AudioMP3Rewind( void )
{
   LoadMP3Temp1Code();
   return AudioMP3Rewind_();
}*/


//------------------------------------------------------------------------------
/*tAudioErr AudioMP3FwRwdEpilog( void )
{
   LoadMP3Temp1Code();
   return AudioMP3FwRwdEpilog_();
}*/


//------------------------------------------------------------------------------
tMusicStatus AudioMP3Status( void )
{
   //LoadMP3Temp1Code();
   return AudioMP3Status_();
}


//------------------------------------------------------------------------------
tAudioErr AudioMP3Play( void )
{
   //LoadMP3Temp2Code();
   SDKLoadCode( MP3_TEMP_CODE02 );
   return AudioMP3Start_();    
}


//------------------------------------------------------------------------------
extern   INT16S   MP3API_FirstDecode   ( void );


tAudioErr AudioMP3SetPlayTime( INT32U time_position )
{
   int Status;

   //LoadMP3Temp4Code();
   SDKLoadCode( MP3_TEMP_CODE04 );
   Status = AudioMP3SetPlayTime_( time_position );
   //LoadMP3Temp2Code();
   SDKLoadCode( MP3_TEMP_CODE02 );
   MP3API_FirstDecode();
   
   if( Audio_Handle.Status != FWRWD_EPILOG_STATUS )   
   {
      if( Audio_Handle.Status != AB_REPEAT_STATUS )
         Audio_Handle.Status = PLAYING_STATUS;
      DAC_Continue();
   }  
   DAC_Go();    
   //LoadMP3Temp1Code();
   SDKLoadCode( MP3_TEMP_CODE01 );
   return Status;
}


//============================================================================
// MP3 API Callback function
//============================================================================

//------------------------------------------------------------------------------
//static int __X sys_frq_;
/*
void SysClkUpgrade( void )
{
   //sys_frq_ = QuerySysClock();
   //PowerSysClock( FEQ_48M );
}


//------------------------------------------------------------------------------
void SysClkDowngrade( void )
{
   //PowerSysClock( sys_frq_ );
}
*/


//------------------------------------------------------------------------------
//tSDKLoadCodeStatus LoadMP3InitCode( void )
//{
//   return SDKLoadCode( INIT_IRAM );
//}
//

#if 0
//------------------------------------------------------------------------------
tSDKLoadCodeStatus LoadMP3Temp0Code( void )
{
   return SDKLoadCode( MP3_TEMP_CODE00 );
}


//------------------------------------------------------------------------------
tSDKLoadCodeStatus LoadMP3Temp1Code( void )
{
   return SDKLoadCode( MP3_TEMP_CODE01 );
}


//------------------------------------------------------------------------------
tSDKLoadCodeStatus LoadMP3Temp2Code( void )
{
   return SDKLoadCode( MP3_TEMP_CODE02 );
}


//------------------------------------------------------------------------------
tSDKLoadCodeStatus LoadMP3Temp3Code( void )
{
   return SDKLoadCode( MP3_TEMP_CODE03 );
}

//------------------------------------------------------------------------------
tSDKLoadCodeStatus LoadMP3Temp4Code( void )
{
   return SDKLoadCode( MP3_TEMP_CODE04 );
}
#endif

//------------------------------------------------------------------------------
//tSDKLoadCodeStatus LoadMP3PlayCode( void )
//{
//   //SDKLoadCode( MP3DEC_CODE_TBL );
//   return SDKLoadCode( MP3_PLAY_IRAM );
//}
//


//------------------------------------------------------------------------------
/*
void ToDoCheckNandFlash( void )
{
}
*/

//============================================================================
// 如果不是使用FAT的方式取得MP3 Bitstream ，所使用的Call Back Function
//
// If you can't used the FAT System ,and your MP3 bit stream source from to NFTL 
// reseved area. You can used these Call Back Function.
//============================================================================
#if USE_RESV == 1
#pragma CODE = READ_RESERVE_SOUND_CODE
#pragma DATA = READ_RESERVE_SOUND_DATA

//------------------------------------------------------------------------------
__X unsigned char greadreservedatabuf[ 512 ];
__X unsigned long gAddrMp3File;
__X unsigned long gResvFileSize;
__X unsigned long MP3_XOR_Key;
__X FCFILE greservedfile;

//------------------------------------------------------------------------------
extern tAudioErr AudioMP3OpenReserveArea( INT32U base_sector,
                                          INT32U offset,
                                          INT32U file_size,
                                          INT32U XOR_Key)
{
   gResvFileSize = file_size;
   gAddrMp3File = base_sector * 512ul + offset;
   MP3_XOR_Key = XOR_Key;
   //LoadMP3InitCode();
   //LoadMP3Temp0Code();
   SDKLoadCode( MP3_TEMP_CODE00 );
   return AudioMP3Open_( 0 );
}

//------------------------------------------------------------------------------
unsigned long ReadFromReservedArea( FCFILE *file,
                                    unsigned char *pBuf,
                                    unsigned int len,
                                    FS_ERR *fserr)
{

   unsigned long i, byte_len;
   unsigned long secIndex1;
   unsigned long secIndex2;
   int err;
   int moveCount;

   byte_len = len;
   if( file->position >= file->size )
   {
      *fserr = 0;
      return 0;
   }
   secIndex1 = ( file->position + gAddrMp3File )/ 512;
   secIndex2 = (( file->position + gAddrMp3File ) + len - 1 )/ 512;

   for( i = secIndex1; i <= secIndex2; i++ )
   {
      if(( err = nftl_resv_read( i, greadreservedatabuf, 1 ))< 0 ) 		
      {
         *fserr = err;
         return 0;
      }
      moveCount = ( int )( 512 - ( file->position + gAddrMp3File ) % 512 );
      moveCount = moveCount <= len ? moveCount : len;
      memcpy(( void *)pBuf, ( void *)&greadreservedatabuf[( file->position + gAddrMp3File ) % 512 ], moveCount );
      pBuf += moveCount;
      file->position += moveCount;
      len -= moveCount;
   }
   *fserr = 0;
   if( file->position <= file->size )
      return byte_len;
   else
      return ( byte_len - ( file->position - file->size ));
}	


//------------------------------------------------------------------------------
FCFILE *_API_FSOpen( char *spec, unsigned int mode, __X FS_ERR * err )
{
	greservedfile.size = gResvFileSize;
	greservedfile.position = 0; 		//current address
	*err = FS_NO_ERROR;	
	return &greservedfile;
}

//------------------------------------------------------------------------------
unsigned int _API_FSRead( __X FCFILE *file, __X void *buf, unsigned int len, __X FS_ERR* err )
{
   unsigned char *XOR_Key_ptr;
   unsigned char *data_buf;
   unsigned int read_byte;
   int i;
   //int index;

   //index = ( int )file->position % 4;
   XOR_Key_ptr = ( unsigned char *)&MP3_XOR_Key;
   read_byte = ReadFromReservedArea( file, buf, len, err );
   data_buf = ( unsigned char *)buf;
   if( MP3_XOR_Key > 0 )
   {
      for( i = 0; i < read_byte; i++ )
      {
         /*data_buf[ i ] ^= XOR_Key_ptr[ index ];
         index++;
         if( index >= 4 )
            index = 0;*/
         data_buf[ i ] ^= XOR_Key_ptr[ 0 ];
      }
   }
   return read_byte;
}


//------------------------------------------------------------------------------
FS_ERR _API_FSClose( FCFILE *file )
{
   return 0;
}


//------------------------------------------------------------------------------
FS_ERR _API_FSSeek( FCFILE *file, unsigned long offset )
{
   return 0;
}
//------------------------------------------------------------------------------
#endif


#if USE_FAT_OID == 1
#pragma CODE = READ_OID_SOUND_CODE
#pragma DATA = READ_OID_SOUND_DATA

//------------------------------------------------------------------------------
__X unsigned long gOidFileSize;
__X unsigned long gOidPosition;
__X unsigned long gOidXOR_Key;
__X FCFILE *gOidFileHandle;
__X FCFILE gVirtualFileHandle;


//------------------------------------------------------------------------------
///unsigned char StrMP3Data[ 100 ];
tAudioErr AudioMP3OpenOIDFile( INT32U offset,
                                 INT32U file_size,
                                 INT32U XOR_Key )
{
   //strcpy( StrMP3Data, ( unsigned char *)FilePath );
   gOidFileSize = file_size;
   gOidPosition = offset;
   gOidXOR_Key = XOR_Key;
   //LoadMP3Temp0Code();
   SDKLoadCode( MP3_TEMP_CODE00 );
   return AudioMP3Open_( 0 );
}


#if 1
//------------------------------------------------------------------------------
FCFILE *_OID_FSOpen( char *spec, unsigned int mode, __X FS_ERR *err )
{
   //gOidFileHandle = FSOpen(( char *)CurrentShortFilePath, mode, err );
   //if( *err != FS_NO_ERROR )
   //   return 0;
   //FSSeek( gOidFileHandle, gOidPosition );
   if(b_play_adb_mp3){
   		gOidFileHandle = adb_g_file;
   	}
   else{
   		gOidFileHandle = *fat_g_cur_file;
   	}
#if( FAT_CLUSTER_BUF == DISABLE )
   FSSeek( gOidFileHandle, gOidPosition );
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )
#if( FAT_CLUSTER_BUF == ENABLE )
	if(b_play_adb_mp3){
   		FSSeek( gOidFileHandle, gOidPosition );
   	}
   else{
   		( void )fs_seek( gOidPosition );
   	}
   
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

   gVirtualFileHandle.size = gOidFileSize;
   gVirtualFileHandle.position = 0;
   return &gVirtualFileHandle;
}


//------------------------------------------------------------------------------
unsigned int _OID_FSRead( __X FCFILE *file, __X void *buf, unsigned int len, __X FS_ERR* err )
{
   unsigned char *XOR_Key_ptr;
   unsigned char *data_buf;
   int i;
   //int index;	
   unsigned int temp_len;

   if(( *file ).position+len > ( *file ).size)
      len = ( unsigned int )(( *file ).size - ( *file ).position );
   if( len == 0 )
      return len;
   //index = ( int )( *file ).position % 4;
   XOR_Key_ptr = ( unsigned char *)&gOidXOR_Key;

   temp_len = FSRead( gOidFileHandle, buf, len, err );
   data_buf = ( unsigned char *)buf;
   if( gOidXOR_Key > 0 )
   {
      for( i = 0; i < temp_len; i++ )
      {
         /*data_buf[ i ] ^= XOR_Key_ptr[ index ];
         index++;
         if( index >= 4 )
            index = 0;*/
         data_buf[ i ] ^= XOR_Key_ptr[ 0 ];
      }
   }

   ( *file ).position += temp_len;
   //SendString((U8 *)"\r\ntime:le=");
  // send_hex_u32(g_timer_tick);
   //send_hex_u16(temp_len);
   return temp_len;
}


//------------------------------------------------------------------------------
FS_ERR _OID_FSClose( __X FCFILE *file )
{
   ( *file ).size = 0;
   ( *file ).position = 0;
   b_play_adb_mp3 = FALSE;
   return 0;//FSClose( gOidFileHandle );
}


//------------------------------------------------------------------------------
FS_ERR _OID_FSSeek( __X FCFILE *file, unsigned long offset )
{
   ( *file ).position = offset;
#if( FAT_CLUSTER_BUF == DISABLE )
   return FSSeek( gOidFileHandle, gOidPosition + offset );
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )
#if( FAT_CLUSTER_BUF == ENABLE )
   return fs_seek( gOidPosition + offset )? FS_NO_ERROR : FS_SEEK_OUT_OF_RANGE;
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )
}
#endif
//------------------------------------------------------------------------------
#endif


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#pragma CODE = MP3_COMMON_CODE
#pragma DATA = MP3_API_XRAM

FS_ERR MP3API_FSSeek( unsigned long offset )
{
   return pMP3API_FSSeek( Audio_Handle.FSHandle, offset );
}


unsigned int MP3API_FSRead( __X void *buf, unsigned int len, __X FS_ERR* err )
{
   return pMP3API_FSRead( Audio_Handle.FSHandle, buf, len, err );
}


unsigned int MP3API_FSClose( void )
{
   return pMP3API_FSClose( Audio_Handle.FSHandle );
}


//------------------------------------------------------------------------------
//extern unsigned int vol_idx;
//const int VolTbl[ 5 ] = { 7, 13, 19, 25, 29 };
#if 0
void SpeakDetect( void )
{
   if(SpeakerDET() && DAC_Out == HEADER_PHONE )
   {
      DAC_Out = SPEAKER;
      DAC_Disable_HeadPhone();
      DAC_Enable_Speaker();
      DAC_Speaker_Volume(VolTbl[vol_idx]/2);
   }
   else if( !SpeakerDET() && DAC_Out == SPEAKER )
   {
      DAC_Out = HEADER_PHONE;
      DAC_Disable_Speaker();
      DAC_Enable_HeadPhone();
      DAC_HeadPhone_Volume( VolTbl[ vol_idx ]);
   }
}
#else
/*void SpeakDetect( void )
{
   DAC_Disable_Speaker();
   DAC_Enable_HeadPhone();
   DAC_HeadPhone_Volume( VolTbl[ vol_idx ]);
   //DAC_Unmute();
}*/
#endif


