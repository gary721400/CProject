/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: MP3_API.H
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#ifndef _MP3_API_H
#define _MP3_API_H

/*!@{

 \section intro Introduction
 - version 0.18\n
	提供播放File System(FAT)里的MP3 file的各种操作功能，使用此API　搭配PCMv2 Driver、FAT以及SDK Load Code。\n
	支援的?案格式：MPEG 1/2/2.5 Layer 3, MPEG 1 Layer 2, VBR & CBR format
 - 使用此API Library，　注意一些?划Address Space位置的??：(?考MP3v2_API_Example里的lcf file)\n
    1.以下二?Address Space　要共用同一?域 \n
		\arg MP3_TEMP1_IRAM
		\arg MP3_TEMP2_IRAM
		
    2.MP3DEC_CODE_TBL的所　空?不可跨越32K的位置(ex:0x17000~0x19000 是不允?的)\n
    3.以下的Address Space必?依序配置所　的空?\n
        \arg MP3DEC_CODE_TBL
        \arg MP3_PLAY_IRAM
        \arg MP3_RUN01_IRAM 与 MP3_RUN02_IRAM 共用同一?域
        
    4.INIT_IRAM与第三?配置方式所占用的空?可以共用同一?域，此空?可以供初始?定的程式使用 \n
	5.MP3_COMMON_IRAM不可与其他Address Space共用同一?域，且程式一?始要自行用SDKLoadCode()??入\n
	6.FS_SEEK_CODE的空?是?AudioMP3API.C里的FSSeek_overlap()呼叫FSSeek()使用的，而在MP3播放?API是不?呼叫FSSeek()，因此再做完AudioMP3Open()之后，此空?可以供其他程式使用。
	\defgroup KBMP3_MP3_API_Group KBMP3 KB003 Software MP3 API	
		
		
@}*/

#include <KBDM9_DDK.H>
#include "fs.h"

//**************MP3 API Enum List**************************************************
//! \defgroup MP3_API_ENUM_GROUP MP3 API Enumerate List
//! @{ \ingroup KBMP3_MP3_API_Group

#ifndef _TYPE_DEF_
#define _TYPE_DEF_
typedef unsigned char  BOOLEAN;     //!< BOOLEAN(TURE/FALSE) 8 bit quantity 
typedef unsigned char  INT8U;       //!< Unsigned  8 bit quantity             
typedef signed   char  INT8S;       //!< Signed    8 bit quantity             
typedef unsigned int   INT16U;      //!< Unsigned 16 bit quantity             
typedef signed   int   INT16S;      //!< Signed   16 bit quantity             
typedef unsigned long  INT32U;      //!< Unsigned 32 bit quantity             
typedef signed   long  INT32S;      //!< Signed   32 bit quantity             
typedef float          FP32;        //!< Single precision floating point      
typedef double         FP64;        //!< Double precision floating point      
#endif

/*!
提供 MP3 API function 回?值使用
*/
typedef enum
{
    AUDIO_ERROR_OK = 0,                         //!< ?有??
    AUDIO_ERROR_PCM_INIT = -1,                  //!< PCM driver 初始?定失?
    AUDIO_ERROR_PCM_UNINIT = -2,                //!< PCM driver 解除?定失?
    AUDIO_ERROR_OPNE_FILE = -3,                 //!< File Open 失?
    AUDIO_ERROR_READ_FILE = -4,                 //!< File Read 失?
    AUDIO_ERROR_SEEK_FILE = -5,                 //!< File Seek 失?
    AUDIO_ERROR_PCM_GO = -6,                    //!< PCM Active 失?
    AUDIO_ERROR_MP3DECODE = -7,                 //!< MP3 Decode 失?
    AUDIO_ERROR_PARAMETER = -8,                 //!< MP3 API ?入????
    AUDIO_ERROR_CLOSE_FILE = -9,                //!< File Close 失?
    AUDIO_ERROR_SETVOLUME = -10,                //!< 音量?置失?
    AUDIO_ERROR_FW_RWD = -11,                   //!< 快?退?置失?
    AUDIO_ERROR_EOF = -12,                      //!< 超出?案?度
    AUDIO_ERROR_LOADCODE = -13                  //!< ?入程式失?
}tAudioErr;

/*!
提供 AudioMP3GetPlayTime() ???定
*/
typedef enum
{
    PLAY_TIME_MODE = 0,     //!< 目前已播放??
    REMAIN_TIME_MODE = 1,   //!< 剩余可播放??
    TOTAL_TIME_MODE = 2     //!< 全部可播放??
}tPlayTimeMode;

/*!
提供 AudioMP3SetEQ()所使用的EQ Table的矩?大小?定值 \n
必?与AudioMP3API.C的eqConstTBL矩?搭配
*/
#define EQ_MAX          6               //!< 提供EQ模式的最大?量，　与eqConstTBL矩?搭配
#define EQ_MAX_BAND     10              //!< EQ Table ?段?量(1~32)

/*!
提供AudioMP3API.C的AudioMP3SetEQ_()???定 \n
??考AudioMP3API.C的MP3API_CustomInit()?之?定
*/
typedef struct {
    INT16U EQ_Max;                      //!< ?定EQ_MAX值
    INT16U EQ_Band;                     //!< ?定EQ_MAX_BAND值
    const unsigned int **EQ_ConstTBL;   //!< ?定eqUserTBL的pointer
    unsigned int *EQ_UserTBL;           //!< ?定使用者自行定?的eqUserTBL之pointer
}tAudioEQ_Info;

/*!
提供 AudioMP3SetEQ() ???定 \n
除了 EQ_USER = -2, EQ_NOMAL = -1 不可改?外，其他的名?或?序皆可改?，唯?量　与EQ_MAX定?相同
*/
typedef enum
{
    EQ_USER = -2,   //!< 使用者自定，可???整eqUserTab[]的?容
    EQ_NOMAL = -1,  //!< 自然
    EQ_ROCK = 0,    //!< ??
    EQ_POP,         //!< 流行
    EQ_CLASSICAL,   //!< 古典
    EQ_SOFT,        //!< 柔和
    EQ_JAZZ,        //!< 爵士
    EQ_BASS         //!< 重低音
}tAudioEQ;

/*!
提供 AudioMP3Status() 回?值使用
*/
typedef enum
{
    INIT_STATUS = 0,      //!< 初始?定??
    OPEN_STATUS,          //!< ???案??
    CLOSE_STATUS,         //!< ???案??
    UNINIT_STATUS,        //!< 解除?定??
    PLAYING_STATUS,       //!< 播放?行??
    PAUSE_STATUS,         //!< 播放?停??
    STOP_STATUS,          //!< 播放停止??
    EOF_STATUS,           //!< ?案?尾??
    AB_REPEAT_STATUS,     //!< AB段重复??
    A_STAGE_STATUS,       //!< 已?定A段??
    FWRWD_PROLOG_STATUS,  //!< 快?退初始??
    FORWARD_STATUS,       //!< 快???
    REWIND_STATUS,        //!< 快退??
    FWRWD_EPILOG_STATUS,  //!< 快?退?束??
    ERROR_STATUS          //!< ????
}tMusicStatus;
//! @}

//**************MP3 API Audio Handle Struct**************************************************
//! \defgroup AudioHandleStructGROUP MP3 API Audio Handle List
//! @{\ingroup KBMP3_MP3_API_Group
/*!
MP3 File ID3v1 Tag. It takes always 128 Bytes at the very end of file (after the last audio frame).
*/
typedef struct 
{
    INT8U TAG_id[3];        //!< (offset:0-2)Tag identifier. Must contain "TAG" string if Tag is valid.
	INT8U Song_title[30];   //!< (offset:3-32)Song Name
	INT8U Artist[30];       //!< (offset:33-62)Artist
	INT8U Album[30];        //!< (offset:63-92)Album
	INT8U Year[4];          //!< (offset:93-96)Year
	INT8U Comment[30];      //!< (offset:97-126)Comment
	INT8U Genre[1];	        //!< (offset:127)Genre
}tID3v1TagStr;

/*!
Audio Handle Struct
*/
typedef struct 
{
    __X FCFILE  *FSHandle;                              //!< File System handle
    tMusicStatus Status;                            //!< API Status
    unsigned int MP3SpectrumData[32];               //!< Spectrum Data
    tID3v1TagStr ID3v1_Data;                        //!< ID3v1 Data
}tAudioHandle;

/*!
Audio Handle Pointer
*/
typedef __X tAudioHandle* ptAudioHandle;
//! @}

//**************MP3 API MP3_Information**************************************************
//! \defgroup MP3InformationGROUP MP3 API MP3 Information
//! @{\ingroup KBMP3_MP3_API_Group

/*!
MP3_Information.bVersionID
*/
#define MPEG_VERSION_1			1 //!< MPEG Version 1 (ISO/IEC 11172-3)
#define MPEG_VERSION_2			2 //!< MPEG Version 2 (ISO/IEC 13818-3)
#define MPEG_VERSION_RESERVED	3 //!< reserved
#define MPEG_VERSION_2P5		4 //!< MPEG Version 2.5 (later extension of MPEG 2)

/*!
MP3_Information.bLayer
*/
#define MPEG_LAYER_1			1 //!< Layer I
#define MPEG_LAYER_2			2 //!< Layer II
#define MPEG_LAYER_3			3 //!< Layer III
#define MPEG_LAYER_RESERVED		4 //!< reserved

/*!
MP3_Information.bChannel
*/
#define STEREO_SOUND 			0 //!< Stereo
#define JOINT_STEREO_SOUND		1 //!< Joint Stereo (Stereo)
#define DUAL_CHANNEL_SOUND		2 //!< Dual channel (2 Mono channels)
#define SINGLE_CHANNEL_SOUND	3 //!< Single channel (Mono)

/*!
MP3_Information.bEmphasis
*/
#define EMPHASIS_NONE 			0 //!< None
#define EMPHASIS_50_DIV_15_MS	1 //!< 50/15 ms
#define EMPHASIS_RESERVED		2 //!< reserved
#define CCIT_JP17				3 //!< CCIT J.17

/*!
MP3_Information.bVBRType
*/
#define CONSTANT_BITRATE 		0 //!< Constant Bitrate
#define XING_VARIABLE_BITRATE	1 //!< Xing Variable Bitrate
#define VBRI_VARIABLE_BITRATE	2 //!< VBRI Variable Bitrate
#define NO_TOC_VARIABLE_BITRATE	3 //!< No Header Variable Bitrate

/*!
MP3 information struct
*/
typedef struct tagMP3_INFORMATION {
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
} MP3_INFORMATION;

/*!
存放MP3??后得到的MP3??
*/
#ifndef _VARIABLE_C_
extern MP3_INFORMATION __X MP3_Information;
#endif
//! @}

//**************MP3 API VBR_Information**************************************************
//! \defgroup VBRInformationGROUP MP3 API VBR Information
//! @{\ingroup KBMP3_MP3_API_Group

/*!
MP3 VBR information struct
*/
typedef struct tagVBR_INFORMATION{
	unsigned long	dwTOC_Offset;           //!< VBR TOC offset
	unsigned int	wVbriTOC_Size;          //!< VBRI TOC size
	unsigned int	wVbriDelay;             //!< VBRI delay
	unsigned int	wVbriTOC_Scale;         //!< VBRI TOC Scale
    unsigned int	wVbriTOC_EntryBytes;    //!< VBRI TOC Entry Bytes
    unsigned int	wVbriTOC_EntryFrames;   //!< VBRI TOC Entry Frames
} VBR_INFORMATION;

/*!
存放MP3??后得到的VBR??
*/
#ifndef _VARIABLE_C_
extern VBR_INFORMATION __X VBR_Information;
#endif

//! @}


//**************MP3 API Function List**************************************************
//! \defgroup MP3_API_FUN_GROUP MP3 API Function List
//! @{\ingroup KBMP3_MP3_API_Group 
//! <IMG SRC=fun_relation.PNG >

/*!
?MP3 API以及PCM driver做初始化?定
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Init( void );

/*!
?放MP3 API以及PCM driver占用到的?硬体?源
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Uninit( void );

/*!
??MP3 file，并且取得播放MP3所　的相???
@param filepath : 欲播放的MP3?案之完整路?
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Open( INT8S *filepath );

/*!
播放??的MP3?案
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Play( void );

/*!
??MP3?案
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Close( void );

/*!
停止播放MP3
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Stop( void );

/*!
?停播放MP3
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Pause( void );

/*!
??播放MP3
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3Continue( void );

/*!
取得目前MP3 API的??
@return MusicStatus : 回?播放??
*/
extern tMusicStatus AudioMP3Status( void );

/*!
取得MP3歌曲的播放??
@param SetMode : ?定回???的模式\n
@return PlayTime : 依照??SetMode的?定，回???的??
@note SetMode依照下列?定值，?定回???的种?:\n
    PLAY_TIME_MODE = 0   : 取得播放??\n
    REMAIN_TIME_MODE = 1 : 取得剩余??\n
    TOTAL_TIME_MODE = 2  : 取得???\n
*/
extern INT32U AudioMP3GetPlayTime(tPlayTimeMode SetMode);

/*!
?定MP3歌曲的?始播放的??
@param time_position : 欲?置的??位置值
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3SetPlayTime( INT32U time_position );

/*!
MP3音效?定(Equalizer)
@param EQ_num : ?定音效?性
@return AudioErr : 回??????考值
@note 音效?定(????，可依　求自行?整?量及模式)\n
    EQ_USER      : 使用者自定，?整eqUserTab[]\n
    EQ_NOMAL     : 自然\n
    EQ_ROCK      : ??\n
    EQ_POP       : 流行\n
    EQ_CLASSICAL : 古典\n
    EQ_SOFT      : 柔和\n
    EQ_JASS      : 爵士\n
    EQ_BASS      : 重低音\n
*/
extern tAudioErr AudioMP3SetEQ(tAudioEQ EQ_num);

/*!
MP3 A-B段重复播放
@return AudioErr : 回??????考值
@note 播放?行中 : \n
    第一次呼叫，?置?前播放的??位置?A段；\n
    第二次呼叫，?置?前播放的??位置?B段，且?始A-B段重复播放；\n
    第三次呼叫，取消A-B段重复播放，??正常播放。\n
    <IMG SRC=abrepeat.PNG >
*/
extern tAudioErr AudioMP3ABRepeat( void );

/*!
?定音量的初始值，并且透?*get_volume取得?定后的音量值
@param *get_volume : 利用call by address，取得音量值
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3SetVol(int volume);

/*!
?目前音量值往上提升一?，并且透?*get_volume取得?定后的音量值
@param *get_volume : 利用call by address，取得音量值
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3VolUp(int *get_volume);

/*!
?目前音量值往下降低一?，并且透?*get_volume取得?定后的音量值
@param *get_volume : 利用call by address，取得音量值
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3VolDown(int *get_volume);

/*!
?先?置MP3快?快退
@return AudioErr : 回??????考值
@note ?行AudioMP3Forward()或AudioMP3Rewind()之前必?先呼叫此function。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdProlog( void );

/*!
?置MP3快?，以目前的播放位置增加播放???的百分之一，若???的百分之一不足三秒，?只增加三秒
@return AudioErr : 回??????考值\n
@note ?行AudioMP3Forward()之前必?先呼叫此AudioMP3FwRwdProlog()，?束?要呼叫AudioMP3FwRwdEpilog()。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Forward( void );

/*!
?置MP3快退，以目前的播放位置?少播放???的百分之一，若???的百分之一不足三秒，?只?少三秒
@return AudioErr : 回??????考值\n
@note ?行AudioMP3Rewind()之前必?先呼叫此AudioMP3FwRwdProlog()，?束?要呼叫AudioMP3FwRwdEpilog()。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Rewind( void );

/*!
?束?置MP3快?快退
@return AudioErr : 回??????考值\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdEpilog( void );
//! @}

//**************MP3 API Reserved Area Function List**************************************************
//! \defgroup MP3_API_FUN_GROUP MP3 API Reserved Area Function List
//! @{\ingroup KBMP3_MP3_API_Reserved_Area_Group

/*!
??于保留?的MP3 file，并且取得播放MP3所　的相???
@param base_sector : MP3 file放置于保留?的起始sector
@param offset : MP3 file放置于保留?sector的偏移值
@param file_size : MP3 file放置于保留?的?度
@return AudioErr : 回??????考值
*/
extern tAudioErr AudioMP3OpenReserveArea(INT32U base_sector,
                                         INT32U offset,
                                         INT32U file_size,
                                         INT32U XOR_Key);
//! @}
extern tAudioErr AudioMP3OpenOIDFile(INT32U offset,
                              		 INT32U file_size,
                              		 INT32U XOR_Key);
#endif // _MP3_API_H