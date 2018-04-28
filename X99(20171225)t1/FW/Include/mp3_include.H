/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: mp3_include.h
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/
#ifndef _MP3_INCLUDE_H
#define _MP3_INCLUDE_H

#include "MP3_API.H"
//#include "Music_Player.H"

//#if PACKAGE_VER!=0x0902
//# error "Package Version should be 0902 "
//#endif

#if (EQ_MAX_BAND > 32 || EQ_MAX_BAND < 1)
    #error "The frequency bands number EQ_MAX_BAND is set fail."
#endif

// Option
#define USE_RESV 0
#define USE_FAT 0
#define USE_FAT_OID 1

//ImageDeclareType( INIT_IRAM );
//ImageDeclareType( MP3_PLAY_IRAM );
ImageDeclareType( MP3_TEMP_CODE00 );
ImageDeclareType( MP3_TEMP_CODE01 );
ImageDeclareType( MP3_TEMP_CODE02 );
ImageDeclareType( MP3_TEMP_CODE03 );
ImageDeclareType( MP3_TEMP_CODE04 );
//ImageDeclareType( FS_SEEK_CODE );
//ImageDeclareType( MP3DEC_CODE_TBL );

extern   tAudioErr      AudioMP3Init_                 ( INT8U default_vol );
                                                      //ptAudioHandle );
extern   tAudioErr      AudioMP3Uninit_               ( void );
extern   tAudioErr      AudioMP3Open_                 ( INT8S *filepath );
extern   tAudioErr      AudioMP3Close_                ( void );
extern   tAudioErr      AudioMP3Start_                ( void );
extern   tAudioErr      AudioMP3Pause_                ( void );
extern   tAudioErr      AudioMP3Stop_                 ( void );
extern   tAudioErr      AudioMP3Continue_             ( void );
extern   tAudioErr      AudioMP3SetEQ_                ( tAudioEQ_Info EQ_Info,
                                                         tAudioEQ EQ_num );
extern   tAudioErr      AudioMP3SetPlayTime_          ( INT32U time_position );
extern   INT32U         AudioMP3GetPlayTime_          ( tPlayTimeMode SetMode );
extern   tAudioErr      AudioMP3VolUp_                ( int *get_volume );
extern   tAudioErr      AudioMP3VolDown_              ( int *get_volume );
extern   tAudioErr      AudioMP3HeadPhoneSetVol_      ( int *get_volume );
extern   tAudioErr      AudioMP3SpeakerSetVol_        ( int *get_volume );

extern   tAudioErr      AudioMP3HeadPhoneVolDown_     ( int *get_volume );
extern   tAudioErr      AudioMP3SpeakerVolDown_       ( int *get_volume );
extern   tAudioErr            AudioMP3HeadPhoneVolUp_ ( int *get_volume );
extern   tAudioErr            AudioMP3SpeakerVolUp_   ( int *get_volume );

extern   tAudioErr            AudioMP3ABRepeat_       ( void );
extern   tAudioErr            AudioMP3FwRwdProlog_    ( void );
extern   tAudioErr            AudioMP3Forward_        ( void );
extern   tAudioErr            AudioMP3Rewind_         ( void );
extern   tAudioErr            AudioMP3FwRwdEpilog_    ( void );
extern   tMusicStatus         AudioMP3Status_         ( void );
//extern   tSDKLoadCodeStatus   LoadMP3InitCode         ( void );
extern   tSDKLoadCodeStatus   LoadMP3PlayCode         ( void );
extern   tSDKLoadCodeStatus   LoadMP3Temp0Code        ( void );
extern   tSDKLoadCodeStatus   LoadMP3Temp1Code        ( void );
extern   tSDKLoadCodeStatus   LoadMP3Temp2Code        ( void );
extern   tSDKLoadCodeStatus   LoadMP3Temp3Code        ( void );
extern   tSDKLoadCodeStatus   LoadMP3Temp4Code        ( void );

extern   FCFILE               *( *pMP3API_FSOpen )    ( char *spec, unsigned int mode, __X FS_ERR *err );
extern   unsigned int         ( *pMP3API_FSRead )     ( __X FCFILE *file, __X void *buf, unsigned int len, __X FS_ERR *err );

extern   FS_ERR               ( *pMP3API_FSClose )    ( __X FCFILE *file );
extern   FS_ERR               ( *pMP3API_FSSeek )     ( __X FCFILE *file,
                                                         unsigned long offset );
//extern   void                 SysClkUpgrade(void);
//extern   void                 SysClkDowngrade(void);

#if USE_RESV == 1
extern   FCFILE         *_FSOpen ( char *spec,
                              unsigned int mode,
                              FS_ERR* err );
extern   unsigned long  _FSRead  ( FCFILE *file,
                                    void *buf,
                                    unsigned long len,
                                    FS_ERR* err );
extern   FS_ERR         _FSClose ( FCFILE *file );
extern   FS_ERR         _FSSeek  ( FCFILE *file,
                                    unsigned long offset );

#elif USE_FAT_OID == 1
FCFILE         *_OID_FSOpen   ( char *spec, unsigned int mode, __X FS_ERR *err );
unsigned int   _OID_FSRead    ( __X FCFILE *file, __X void *buf, unsigned int len, __X FS_ERR *err );
FS_ERR         _OID_FSClose   ( __X FCFILE *file );
FS_ERR         _OID_FSSeek    ( __X FCFILE *file, unsigned long offset );

#elif USE_FAT == 1
//extern   FS_ERR         FSSeek_overlap ( FCFILE *file,
//                                          unsigned long offset );
#endif // #if USE_FAT == 0

#endif // _MP3_INCLUDE_H