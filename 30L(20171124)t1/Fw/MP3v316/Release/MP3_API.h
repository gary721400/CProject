#ifndef _MP3_API_H
#define _MP3_API_H

/*!@{

 \section intro Introduction
 - version 0.18\n
	���Ѽ���File System(FAT)�̪�MP3 file���U�ؾާ@�\��A�ϥΦ�API�ݷf�tPCMv2 Driver�BFAT�H��SDK Load Code�C\n
	�䴩���ɮ׮榡�GMPEG 1/2/2.5 Layer 3, MPEG 1 Layer 2, VBR & CBR format
 - �ϥΦ�API Library�A�ݪ`�N�@�ǳW��Address Space��m���W�h�G(�Ѧ�MP3v2_API_Example�̪�lcf file)\n
    1.�H�U�G��Address Space�ݭn�@�ΦP�@�ϰ� \n
		\arg MP3_TEMP1_IRAM
		\arg MP3_TEMP2_IRAM

    2.MP3DEC_CODE_TBL���һݪŶ����i��V32K����m(ex:0x17000~0x19000 �O�����\��)\n
    3.�H�U��Address Space�����̧ǰt�m�һݪ��Ŷ�\n
        \arg MP3DEC_CODE_TBL
        \arg MP3_PLAY_IRAM
        \arg MP3_RUN01_IRAM �P MP3_RUN02_IRAM �@�ΦP�@�ϰ�

    4.INIT_IRAM�P�ĤT�I�t�m�覡�Ҧ��Ϊ��Ŷ��i�H�@�ΦP�@�ϰ�A���Ŷ��i�H�Ѫ�l�]�w���{���ϥ� \n
	5.MP3_COMMON_IRAM���i�P��LAddress Space�@�ΦP�@�ϰ�A�B�{���@�}�l�n�ۦ��SDKLoadCode()�Ӹ��J\n
	6.FS_SEEK_CODE���Ŷ��O��AudioMP3API.C�̪�FSSeek_overlap()�I�sFSSeek()�ϥΪ��A�ӦbMP3�����API�O���|�I�sFSSeek()�A�]���A����AudioMP3Open()����A���Ŷ��i�H�Ѩ�L�{���ϥΡC
	\defgroup KBMP3_MP3_API_Group KBMP3 KB003 Software MP3 API


@}*/
#include <KBDM9_DDK.H>
#include "fs.h"
//**************MP3 API Enum List**************************************************
//! \defgroup MP3_API_ENUM_GROUP MP3 API Enumerate List
//! @{ \ingroup KBMP3_MP3_API_Group
//	#ifndef _TYPE_DEF_
//	#define _TYPE_DEF_
//	typedef unsigned char  BOOLEAN;     //!< BOOLEAN(TURE/FALSE) 8 bit quantity
//	typedef unsigned char  INT8U;       //!< Unsigned  8 bit quantity
//	typedef          char  INT8S;       //!< Signed    8 bit quantity
//	typedef unsigned int   INT16U;      //!< Unsigned 16 bit quantity
//	typedef          int   INT16S;      //!< Signed   16 bit quantity
//	typedef unsigned long  INT32U;      //!< Unsigned 32 bit quantity
//	typedef long           INT32S;      //!< Signed   32 bit quantity
//	typedef float          FP32;        //!< Single precision floating point
//	typedef double         FP64;        //!< Double precision floating point
//	#endif
/*!
���� MP3 API function �^�ǭȨϥ�
*/
#define MP3_AUDIO_OK		0
#define MP3_AUDIO_PCM_INIT	-1
#define MP3_AUDIO_PCM_UNINIT	-2
#define MP3_AUDIO_OPNE_FILE	-3
#define MP3_AUDIO_READ_FILE	-4
#define MP3_AUDIO_SEEK_FILE	-5
#define MP3_AUDIO_PCM_GO	-6
#define MP3_AUDIO_MP3DECODE	-7
#define MP3_AUDIO_PARAMETER	-8
#define MP3_AUDIO_CLOSE_FILE	-9
#define MP3_AUDIO_SETVOLUME	-10
#define MP3_AUDIO_FW_RWD	-11
#define MP3_AUDIO_EOF		-12
#define MP3_AUDIO_LOADCODE	-13
typedef enum
{
	AUDIO_ERROR_OK		= MP3_AUDIO_OK,		//!< �S�����~
	AUDIO_ERROR_PCM_INIT	= MP3_AUDIO_PCM_INIT,	//!< PCM driver ��l�]�w����
	AUDIO_ERROR_PCM_UNINIT	= MP3_AUDIO_PCM_UNINIT,	//!< PCM driver �Ѱ��]�w����
	AUDIO_ERROR_OPNE_FILE	= MP3_AUDIO_OPNE_FILE,	//!< File Open ����
	AUDIO_ERROR_READ_FILE	= MP3_AUDIO_READ_FILE,	//!< File Read ����
	AUDIO_ERROR_SEEK_FILE	= MP3_AUDIO_SEEK_FILE,	//!< File Seek ����
	AUDIO_ERROR_PCM_GO	= MP3_AUDIO_PCM_GO,	//!< PCM Active ����
	AUDIO_ERROR_MP3DECODE	= MP3_AUDIO_MP3DECODE,	//!< MP3 Decode ����
	AUDIO_ERROR_PARAMETER	= MP3_AUDIO_PARAMETER,	//!< MP3 API ��J�Ѽƿ��~
	AUDIO_ERROR_CLOSE_FILE	= MP3_AUDIO_CLOSE_FILE,	//!< File Close ����
	AUDIO_ERROR_SETVOLUME	= MP3_AUDIO_SETVOLUME,	//!< ���q�]�m����
	AUDIO_ERROR_FW_RWD	= MP3_AUDIO_FW_RWD,	//!< �ֶi�h�]�m����
	AUDIO_ERROR_EOF		= MP3_AUDIO_EOF,	//!< �W�X�ɮת���
	AUDIO_ERROR_LOADCODE	= MP3_AUDIO_LOADCODE	//!< ���J�{������
}tAudioErr;

/*!
���� AudioMP3SetEQ()�ҨϥΪ�EQ Table���x�}�j�p�]�w�� \n
�����PAudioMP3API.C��eqConstTBL�x�}�f�t
*/
#define EQ_MAX          6               //!< ����EQ�Ҧ����̤j�ƶq�A�ݻPeqConstTBL�x�}�f�t
#define EQ_MAX_BAND     10              //!< EQ Table �W�q�ƶq(1~32)

/*!
����AudioMP3API.C��AudioMP3SetEQ_()�ѼƳ]�w \n
�аѦ�AudioMP3API.C��MP3API_CustomInit()�����]�w
*/
typedef struct {
	unsigned int EqMax;				//!< �]�wEQ_MAX��
	unsigned int EqBand;				//!< �]�wEQ_MAX_BAND��
	unsigned int **EqConstTBL;		//!< �]�weqUserTBL��pointer
	unsigned int *EqUserTBL;		//!< �]�w�ϥΪ̦ۦ�w�q��eqUserTBL��pointer
}_MP3_AUDIO_EQINFO;

/*!
���� AudioMP3SetEQ() �ѼƳ]�w \n
���F EQ_USER = -2, EQ_NOMAL = -1 ���i���ܥ~�A��L���W�٩ζ��Ǭҥi���ܡA�߼ƶq�ݻPEQ_MAX�w�q�ۦP
*/
#define MP3_EQ_USER		-2
#define MP3_EQ_NOMAL		-1
#define MP3_EQ_ROCK		 0
#define MP3_AUDIO_OPNE_FILE	-3
#define MP3_AUDIO_READ_FILE	-4
#define MP3_AUDIO_SEEK_FILE	-5
typedef enum
{
    EQ_USER  = MP3_EQ_USER,	//!< �ϥΪ̦۩w�A�i�ʺA�վ� AudioMP3EqUserTBL[] �����e
    EQ_NOMAL = MP3_EQ_NOMAL,	//!< �۵M
    EQ_ROCK  = MP3_EQ_ROCK,	//!< �n�u
    EQ_POP,			//!< �y��
    EQ_CLASSICAL,		//!< �j��
    EQ_SOFT,			//!< �X�M
    EQ_JAZZ,			//!< ��h
    EQ_BASS			//!< ���C��
}tAudioEQ;

/*!
���� AudioMP3Status() �^�ǭȨϥ�
*/
#define MP3_STATUS_INIT		0
#define MP3_STATUS_OPEN		1
#define MP3_STATUS_CLOSE	2
#define MP3_STATUS_UNINIT	3
#define MP3_STATUS_PLAYING	4
#define MP3_STATUS_PAUSE	5
#define MP3_STATUS_STOP		6
#define MP3_STATUS_EOF		7
#define MP3_STATUS_AB_REPEAT	8
#define MP3_STATUS_A_STAGE	9
#define MP3_STATUS_FWRWD_PROLOG	10
#define MP3_STATUS_FORWARD	11
#define MP3_STATUS_REWIND	12
#define MP3_STATUS_FWRWD_EPILOG	13
#define MP3_STATUS_ERROR	14
typedef enum
{
	INIT_STATUS		= MP3_STATUS_INIT,		//!< ��l�]�w���A
	OPEN_STATUS		= MP3_STATUS_OPEN,		//!< �}���ɮת��A
	CLOSE_STATUS		= MP3_STATUS_CLOSE,		//!< �����ɮת��A
	UNINIT_STATUS		= MP3_STATUS_UNINIT,		//!< �Ѱ��]�w���A
	PLAYING_STATUS		= MP3_STATUS_PLAYING,		//!< ����i�檬�A
	PAUSE_STATUS		= MP3_STATUS_PAUSE,		//!< ����Ȱ����A
	STOP_STATUS		= MP3_STATUS_STOP,		//!< ���񰱤�A
	EOF_STATUS		= MP3_STATUS_EOF,		//!< �ɮ׵������A
	AB_REPEAT_STATUS	= MP3_STATUS_AB_REPEAT,		//!< AB�q���ƪ��A
	A_STAGE_STATUS		= MP3_STATUS_A_STAGE,		//!< �w�]�wA�q���A
	FWRWD_PROLOG_STATUS	= MP3_STATUS_FWRWD_PROLOG,	//!< �ֶi�h��l���A
	FORWARD_STATUS		= MP3_STATUS_FORWARD,		//!< �ֶi���A
	REWIND_STATUS		= MP3_STATUS_REWIND,		//!< �ְh���A
	FWRWD_EPILOG_STATUS	= MP3_STATUS_FWRWD_EPILOG,	//!< �ֶi�h�������A
	ERROR_STATUS		= MP3_STATUS_ERROR,		//!< ���~���A
}_MP3_AUDIO_STATUS;
/*!
���� MP3API_STATUS �^�ǭȨϥ�
*/
#define MP3_RUN_ERR	-1
#define MP3_RUN_NRR	0	// FS_NO_ERROR
typedef enum
{
	MP3API_ERR    = MP3_RUN_ERR,	//!< MP3API������~
	MP3API_NO_ERR = MP3_RUN_NRR,	//!< MP3API���榨�\

}MP3API_STATUS;

//**************MP3 API Audio Handle Struct**************************************************
//! \defgroup AudioHandleStructGROUP MP3 API Audio Handle List
//! @{\ingroup KBMP3_MP3_API_Group
/*!
MP3 File ID3v1 Tag. It takes always 128 Bytes at the very end of file (after the last audio frame).
*/
typedef struct
{
	unsigned char TAG_id[3];        //!< (offset:0-2)Tag identifier. Must contain "TAG" string if Tag is valid.
	unsigned char Song_title[30];   //!< (offset:3-32)Song Name
	unsigned char Artist[30];       //!< (offset:33-62)Artist
	unsigned char Album[30];        //!< (offset:63-92)Album
	unsigned char Year[4];          //!< (offset:93-96)Year
	unsigned char Comment[30];      //!< (offset:97-126)Comment
	unsigned char Genre[1];	        //!< (offset:127)Genre
}tID3v1TagStr;

/*!
Audio Handle Struct
*/
typedef struct
{
	__X FCFILE  *FSHandle;			//!< File System handle
	_MP3_AUDIO_STATUS Status;			//!< API Status
	unsigned int SpectrumData[32];		//!< Spectrum Data
	tID3v1TagStr ID3v1_Data;			//!< ID3v1 Data
}_MP3_AUDIO_HANDLE;
//**************MP3 API AudioMP3Information**************************************************
//! \defgroup MP3InformationGROUP MP3 API MP3 Information
//! @{\ingroup KBMP3_MP3_API_Group

/*!
AudioMP3Information.bVersionID
*/
#define MPEG_VERSION_1			1 //!< MPEG Version 1 (ISO/IEC 11172-3)
#define MPEG_VERSION_2			2 //!< MPEG Version 2 (ISO/IEC 13818-3)
#define MPEG_VERSION_RESERVED		3 //!< reserved
#define MPEG_VERSION_2P5		4 //!< MPEG Version 2.5 (later extension of MPEG 2)

/*!
AudioMP3Information.bLayer
*/
#define MPEG_LAYER_1			1 //!< Layer I
#define MPEG_LAYER_2			2 //!< Layer II
#define MPEG_LAYER_3			3 //!< Layer III
#define MPEG_LAYER_RESERVED		4 //!< reserved

/*!
AudioMP3Information.bChannel
*/
#define STEREO_SOUND 			0 //!< Stereo
#define JOINT_STEREO_SOUND		1 //!< Joint Stereo (Stereo)
#define DUAL_CHANNEL_SOUND		2 //!< Dual channel (2 Mono channels)
#define SINGLE_CHANNEL_SOUND		3 //!< Single channel (Mono)

/*!
AudioMP3Information.bEmphasis
*/
#define EMPHASIS_NONE 			0 //!< None
#define EMPHASIS_50_DIV_15_MS		1 //!< 50/15 ms
#define EMPHASIS_RESERVED		2 //!< reserved
#define CCIT_JP17			3 //!< CCIT J.17

/*!
AudioMP3Information.bVBRType
*/
#define CONSTANT_BITRATE 		0 //!< Constant Bitrate
#define XING_VARIABLE_BITRATE		1 //!< Xing Variable Bitrate
#define VBRI_VARIABLE_BITRATE		2 //!< VBRI Variable Bitrate
#define NO_TOC_VARIABLE_BITRATE		3 //!< No Header Variable Bitrate

/*!
MP3 information struct
*/
typedef struct {
	unsigned long	dwFileSize;         //!< unit is byte
	unsigned long	dwHeaderOffset;     //!< unit is byte
	unsigned long	dwAudioFileSize;    //!< unit is byte !!
	unsigned long	dwFilePosition;     //!< unit is byte (auto update)
	unsigned long	dwTimeLength;       //!< unit is 1ms !!
	unsigned long	dwTimePosition;     //!< unit is 1ms (auto update)
	unsigned long	dwFrameTotal;       //!< total frame count
	unsigned long	dwFrameCount;       //!< deocde frame count
	unsigned char	bVersionID;         //!< see MP3_Information.bVersionID define
	unsigned char	bLayer;             //!< see MP3_Information.bLeayer define
	unsigned char	bVBRType;           //!< see MP3_Information.bVBRType define
	unsigned char	bChannel;           //!< see MP3_Information.bChannel define
	unsigned int	wBitRate;           //!< unit is 1 kbit/sec
	unsigned int	wSampleRate;        //!< unit is Hz
	unsigned int	wFrameSize;         //!< unit is byte
	unsigned int	wSamplePerFrame;    //!< sample per frame
	unsigned int    wFrameDuration;     //!< time per frame
	unsigned char	bPaddingSize;       //!< unit is byte
	unsigned char	bCRCs;              //!< CRC
	unsigned char	bPrivate;           //!< Private
	unsigned char	bCopyright;         //!< Original
	unsigned char	bOriginal;          //!< Emphasis
	unsigned char	bEmphasis;          //!< see MP3_Information.bEmphasis define
	unsigned int	wID3v1Size;         //!< ID3v1 Size
	unsigned long	dwID3v2Size;        //!< ID3v2 Size
}_MP3_INFORMATION;

//**************MP3 API VBR_Information**************************************************
//! \defgroup VBRInformationGROUP MP3 API VBR Information
//! @{\ingroup KBMP3_MP3_API_Group

/*!
MP3 VBR information struct
*/
typedef struct{
	unsigned long	dwTOC_Offset;           //!< VBR TOC offset
	unsigned int	wVbriTOC_Size;          //!< VBRI TOC size
	unsigned int	wVbriDelay;             //!< VBRI delay
	unsigned int	wVbriTOC_Scale;         //!< VBRI TOC Scale
	unsigned int	wVbriTOC_EntryBytes;    //!< VBRI TOC Entry Bytes
	unsigned int	wVbriTOC_EntryFrames;   //!< VBRI TOC Entry Frames
}_MP3_VBR_INFORMATION;

//typedef struct
//{
//	MP3API_STATUS (*FpSeek)( unsigned long offset );
//	int           (*FpRead)( __X char *buf, int len );
//	MP3API_STATUS (*FpOpen)( char* p );
//	MP3API_STATUS (*FpClose)( void );
//	unsigned long (*FpGetSize)( void );
//	unsigned long (*FpGetPosition)( void );
//}_MP3_FILE_HANDLE;
//
//extern const _MP3_FILE_HANDLE AudioMP3FsHandle;
//extern const _MP3_FILE_HANDLE AudioMP3RsHandle;
//
//extern MP3API_STATUS AudioMP3FsSeek( unsigned long offset );
//extern int           AudioMP3FsRead( __X char *buf, int len );
//extern MP3API_STATUS AudioMP3FsOpen( char *file );
//extern MP3API_STATUS AudioMP3FsClose( void );
//
//extern MP3API_STATUS AudioMP3RsSeek( unsigned long offset );
//extern int           AudioMP3RsRead( __X char *buf, int len );
//extern MP3API_STATUS AudioMP3RsOpen( char *file );
//extern MP3API_STATUS AudioMP3RsClose( void );
//
//extern unsigned long AudioMP3FpGetSize( void );
//extern unsigned long AudioMP3FpGetPosition( void );
//
extern void AudioMP3CkBuffer( void );

//extern int AudioMP3ResvRead( unsigned long sector, __X void *buf );
//
#ifndef _MP3_INTERNAL_
extern __X unsigned int AudioMP3VolNow;
extern __X _MP3_AUDIO_HANDLE AudioMP3Handle;
extern __X unsigned long AudioMP3RsSector, AudioMP3FileLen, AudioMP3Offset, AudioMP3Position;
#endif

ImageDeclareType(MP3_API_CODE);

//**************MP3 API Function List**************************************************
//! \defgroup MP3_API_FUN_GROUP MP3 API Function List
//! @{\ingroup KBMP3_MP3_API_Group
//! <IMG SRC=fun_relation.PNG >

/*!
��MP3 API�H��PCM driver����l�Ƴ]�w
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Init( int Voln );

/*!
����MP3 API�H��PCM driver���Ψ쪺�n�w��귽
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Uninit( void );

/*!
�}��MP3 file�A�åB���o����MP3�һݪ�������T
@param filepath : ������MP3�ɮפ�������|
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Open( char *file );

/*!
����}�Ҫ�MP3�ɮ�
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Play( void );

/*!
�T�{MP3����
*/
extern int AudioMP3ChkPlaying( void );

/*!
����MP3�ɮ�
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Close( void );

/*!
�����MP3
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Stop( void );

/*!
�Ȱ�����MP3
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Pause( void );

/*!
�~�򼽩�MP3
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3Continue( void );

/*!
���o�ثeMP3 API�����A
@return MusicStatus : �^�Ǽ��񪬺A
*/
extern _MP3_AUDIO_STATUS AudioMP3Status( void );
extern unsigned int *AudioMP3SpectrumData(void);

/*!
���oMP3�q��������ɶ�
@param SetMode : �]�w�^�Ǯɶ����Ҧ�\n
@return PlayTime : �̷ӰѼ�SetMode���]�w�A�^�ǹ������ɶ�
@note SetMode�̷ӤU�C�]�w�ȡA�M�w�^�Ǯɶ�������:\n
*/
#define PLAY_TIME_MODE		0 //!< �ثe�w����ɶ�
#define REMAIN_TIME_MODE	1 //!< �Ѿl�i����ɶ�
#define TOTAL_TIME_MODE		2 //!< �����i����ɶ�
extern unsigned long AudioMP3GetPlayTime( int SetMode );

/*!
�]�wMP3�q�����}�l���񪺮ɶ�
@param time_position : ���]�m���ɶ���m��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3SetPlayTime( unsigned long time_position );

/*!
MP3���ĳ]�w(Equalizer)
@param EQ_num : �]�w�����ݩ�
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
@note ���ĳ]�w(�w�]�ѼơA�i�̻ݨD�ۦ�վ�ƶq�μҦ�)\n
    EQ_USER      : �ϥΪ̦۩w�A�վ� AudioMP3EqUserTBL[]\n
    EQ_NOMAL     : �۵M\n
    EQ_ROCK      : �n�u\n
    EQ_POP       : �y��\n
    EQ_CLASSICAL : �j��\n
    EQ_SOFT      : �X�M\n
    EQ_JASS      : ��h\n
    EQ_BASS      : ���C��\n
*/
extern tAudioErr AudioMP3SetEQ(tAudioEQ EQ_num);

/*!
MP3 A-B�q���Ƽ���
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
@note ����i�椤 : \n
    �Ĥ@���I�s�A�]�m��e���񪺮ɶ���m��A�q�F\n
    �ĤG���I�s�A�]�m��e���񪺮ɶ���m��B�q�A�B�}�lA-B�q���Ƽ���F\n
    �ĤT���I�s�A����A-B�q���Ƽ���A�~�򥿱`����C\n
    <IMG SRC=abrepeat.PNG >
*/
extern tAudioErr AudioMP3ABRepeat( void );

/*!
�]�w���q����l�ȡA�z�LSet_volume�]�w�_�l�����q��
@param Set_volume  : �]�w��l�Ȫ����q��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3VolInit(int Set_volume);

/*!
�N�ثe���q�ȩ��W���ɤ@���A�åB�z�L*get_volume���o�]�w�᪺���q��
@param *get_volume : �Q��call by address�A���o���q��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3VolUp(int *get_volume);

/*!
�N�ثe���q�ȩ��U���C�@���A�åB�z�L*get_volume���o�]�w�᪺���q��
@param *get_volume : �Q��call by address�A���o���q��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
*/
extern tAudioErr AudioMP3VolDown(int *get_volume);

/*!
�w���]�mMP3�ֶi�ְh
@return AudioErr : �^�ǿ��~���A�Ѧҭ�
@note ����AudioMP3Forward()��AudioMP3Rewind()���e�������I�s��function�C\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdProlog( void );

/*!
�]�mMP3�ֶi�A�H�ثe�������m�W�[�����`�ɶ����ʤ����@�A�Y�`�ɶ����ʤ����@�����T��A�h�u�W�[�T��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�\n
@note ����AudioMP3Forward()���e�������I�s��AudioMP3FwRwdProlog()�A�����h�n�I�sAudioMP3FwRwdEpilog()�C\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Forward( void );

/*!
�]�mMP3�ְh�A�H�ثe�������m��ּ����`�ɶ����ʤ����@�A�Y�`�ɶ����ʤ����@�����T��A�h�u��֤T��
@return AudioErr : �^�ǿ��~���A�Ѧҭ�\n
@note ����AudioMP3Rewind()���e�������I�s��AudioMP3FwRwdProlog()�A�����h�n�I�sAudioMP3FwRwdEpilog()�C\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Rewind( void );

/*!
�����]�mMP3�ֶi�ְh
@return AudioErr : �^�ǿ��~���A�Ѧҭ�\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdEpilog( void );
//! @}

//**************MP3 API Reserved Area Function List**************************************************
//! \defgroup MP3_API_FUN_GROUP MP3 API Reserved Area Function List
//! @{\ingroup KBMP3_MP3_API_Reserved_Area_Group

/*!
Function ���СAMP3 file��Open�{���A�åB���o����MP3�һݪ�������T�C
User�i�H�z�L���{��Open�O�d�ϩΫD�O�d�Ϫ��ɮסC
@param *filepath 		: MP3 file ����ڸ��|(FAT)�Ϊ̬Ofile�����Ǹ��X(Resevd area)
@return MP3API_STATUS 	: �^�ǿ��~���A�Ѧҭ�
*/
#define MP3API_FileOpen AudioMP3FileOpen
extern MP3API_STATUS AudioMP3FileOpen( char *filepath );

/*!
Function ���СAMP3 file��seek�{���A�N�ɮ׫��Ц�m������w����m�C
User�i�H�z�L���{��Seek�O�d�ϩΫD�O�d�Ϫ��ɮצ�m�C
@param offset 			: MP3 file��m��sector��������
@return MP3API_STATUS 	: �^�ǿ��~���A�Ѧҭ�
*/
#define MP3API_FileSeek AudioMP3FileSeek
extern MP3API_STATUS AudioMP3FileSeek( unsigned long offset );

/*!
Function ���СAMP3 file��Read�{���AŪ��sector����ơC
User�i�H�z�L���{��Read�O�d�ϩΫD�O�d�Ϫ��ɮ׸�ơC
@param *buf 	: ��mŪ����ƪ��Ŷ���m
@param len 		: Ū��������
@return  		: �^�ǿ��~���A(-1)�Ϊ�Ū��������
*/
#define MP3API_FileRead AudioMP3FileRead
extern int AudioMP3FileRead( __X char *buf, int len );

/*!
Function ���СAMP3 file��Closed�{���A�����ثe�ɮסC
User�i�H�z�L���{��Close�O�d�ϩΫD�O�d�Ϫ��ɮסC
@return MP3API_STATUS 	: �^�ǿ��~���A�Ѧҭ�
*/
#define MP3API_FileClose AudioMP3FileClose
extern MP3API_STATUS AudioMP3FileClose( void );

/*!
Function ���СAMP3 file��Ū���ثe��m�{���A�o��ثe�ɮ׫��w����m�C
User�i�H�z�L���{���o��O�d�ϩΫD�O�d�Ϫ��ɮ׫��w��m�C
@return 			 	: �^�ǥثe�b�ɮפ����w����m
*/
#define MP3API_GetPosition AudioMP3GetSize
extern unsigned long AudioMP3GetPosition( void );

/*!
Function ���СAMP3 file��Ū���ɮפj�p�{���A�o��ثe�ɮת���Ƥj�p�C
User�i�H�z�L���{���o��O�d�ϩΫD�O�d�Ϫ��ɮ׸�Ƥj�p�C
@return 			 	: �^�ǥثe���ɮת���Ƥj�p
*/
#define MP3API_GetSize AudioMP3GetSize
extern unsigned long AudioMP3GetSize( void );
//! @}
#endif // _MP3_API_H