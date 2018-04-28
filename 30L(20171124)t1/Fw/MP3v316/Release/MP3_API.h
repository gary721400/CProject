#ifndef _MP3_API_H
#define _MP3_API_H

/*!@{

 \section intro Introduction
 - version 0.18\n
	提供播放File System(FAT)裡的MP3 file的各種操作功能，使用此API需搭配PCMv2 Driver、FAT以及SDK Load Code。\n
	支援的檔案格式：MPEG 1/2/2.5 Layer 3, MPEG 1 Layer 2, VBR & CBR format
 - 使用此API Library，需注意一些規劃Address Space位置的規則：(參考MP3v2_API_Example裡的lcf file)\n
    1.以下二個Address Space需要共用同一區域 \n
		\arg MP3_TEMP1_IRAM
		\arg MP3_TEMP2_IRAM

    2.MP3DEC_CODE_TBL的所需空間不可跨越32K的位置(ex:0x17000~0x19000 是不允許的)\n
    3.以下的Address Space必須依序配置所需的空間\n
        \arg MP3DEC_CODE_TBL
        \arg MP3_PLAY_IRAM
        \arg MP3_RUN01_IRAM 與 MP3_RUN02_IRAM 共用同一區域

    4.INIT_IRAM與第三點配置方式所佔用的空間可以共用同一區域，此空間可以供初始設定的程式使用 \n
	5.MP3_COMMON_IRAM不可與其他Address Space共用同一區域，且程式一開始要自行用SDKLoadCode()來載入\n
	6.FS_SEEK_CODE的空間是給AudioMP3API.C裡的FSSeek_overlap()呼叫FSSeek()使用的，而在MP3播放時API是不會呼叫FSSeek()，因此再做完AudioMP3Open()之後，此空間可以供其他程式使用。
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
提供 MP3 API function 回傳值使用
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
	AUDIO_ERROR_OK		= MP3_AUDIO_OK,		//!< 沒有錯誤
	AUDIO_ERROR_PCM_INIT	= MP3_AUDIO_PCM_INIT,	//!< PCM driver 初始設定失敗
	AUDIO_ERROR_PCM_UNINIT	= MP3_AUDIO_PCM_UNINIT,	//!< PCM driver 解除設定失敗
	AUDIO_ERROR_OPNE_FILE	= MP3_AUDIO_OPNE_FILE,	//!< File Open 失敗
	AUDIO_ERROR_READ_FILE	= MP3_AUDIO_READ_FILE,	//!< File Read 失敗
	AUDIO_ERROR_SEEK_FILE	= MP3_AUDIO_SEEK_FILE,	//!< File Seek 失敗
	AUDIO_ERROR_PCM_GO	= MP3_AUDIO_PCM_GO,	//!< PCM Active 失敗
	AUDIO_ERROR_MP3DECODE	= MP3_AUDIO_MP3DECODE,	//!< MP3 Decode 失敗
	AUDIO_ERROR_PARAMETER	= MP3_AUDIO_PARAMETER,	//!< MP3 API 輸入參數錯誤
	AUDIO_ERROR_CLOSE_FILE	= MP3_AUDIO_CLOSE_FILE,	//!< File Close 失敗
	AUDIO_ERROR_SETVOLUME	= MP3_AUDIO_SETVOLUME,	//!< 音量設置失敗
	AUDIO_ERROR_FW_RWD	= MP3_AUDIO_FW_RWD,	//!< 快進退設置失敗
	AUDIO_ERROR_EOF		= MP3_AUDIO_EOF,	//!< 超出檔案長度
	AUDIO_ERROR_LOADCODE	= MP3_AUDIO_LOADCODE	//!< 載入程式失敗
}tAudioErr;

/*!
提供 AudioMP3SetEQ()所使用的EQ Table的矩陣大小設定值 \n
必須與AudioMP3API.C的eqConstTBL矩陣搭配
*/
#define EQ_MAX          6               //!< 提供EQ模式的最大數量，需與eqConstTBL矩陣搭配
#define EQ_MAX_BAND     10              //!< EQ Table 頻段數量(1~32)

/*!
提供AudioMP3API.C的AudioMP3SetEQ_()參數設定 \n
請參考AudioMP3API.C的MP3API_CustomInit()內之設定
*/
typedef struct {
	unsigned int EqMax;				//!< 設定EQ_MAX值
	unsigned int EqBand;				//!< 設定EQ_MAX_BAND值
	unsigned int **EqConstTBL;		//!< 設定eqUserTBL的pointer
	unsigned int *EqUserTBL;		//!< 設定使用者自行定義的eqUserTBL之pointer
}_MP3_AUDIO_EQINFO;

/*!
提供 AudioMP3SetEQ() 參數設定 \n
除了 EQ_USER = -2, EQ_NOMAL = -1 不可改變外，其他的名稱或順序皆可改變，唯數量需與EQ_MAX定義相同
*/
#define MP3_EQ_USER		-2
#define MP3_EQ_NOMAL		-1
#define MP3_EQ_ROCK		 0
#define MP3_AUDIO_OPNE_FILE	-3
#define MP3_AUDIO_READ_FILE	-4
#define MP3_AUDIO_SEEK_FILE	-5
typedef enum
{
    EQ_USER  = MP3_EQ_USER,	//!< 使用者自定，可動態調整 AudioMP3EqUserTBL[] 的內容
    EQ_NOMAL = MP3_EQ_NOMAL,	//!< 自然
    EQ_ROCK  = MP3_EQ_ROCK,	//!< 搖滾
    EQ_POP,			//!< 流行
    EQ_CLASSICAL,		//!< 古典
    EQ_SOFT,			//!< 柔和
    EQ_JAZZ,			//!< 爵士
    EQ_BASS			//!< 重低音
}tAudioEQ;

/*!
提供 AudioMP3Status() 回傳值使用
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
	INIT_STATUS		= MP3_STATUS_INIT,		//!< 初始設定狀態
	OPEN_STATUS		= MP3_STATUS_OPEN,		//!< 開啟檔案狀態
	CLOSE_STATUS		= MP3_STATUS_CLOSE,		//!< 關閉檔案狀態
	UNINIT_STATUS		= MP3_STATUS_UNINIT,		//!< 解除設定狀態
	PLAYING_STATUS		= MP3_STATUS_PLAYING,		//!< 播放進行狀態
	PAUSE_STATUS		= MP3_STATUS_PAUSE,		//!< 播放暫停狀態
	STOP_STATUS		= MP3_STATUS_STOP,		//!< 播放停止狀態
	EOF_STATUS		= MP3_STATUS_EOF,		//!< 檔案結尾狀態
	AB_REPEAT_STATUS	= MP3_STATUS_AB_REPEAT,		//!< AB段重複狀態
	A_STAGE_STATUS		= MP3_STATUS_A_STAGE,		//!< 已設定A段狀態
	FWRWD_PROLOG_STATUS	= MP3_STATUS_FWRWD_PROLOG,	//!< 快進退初始狀態
	FORWARD_STATUS		= MP3_STATUS_FORWARD,		//!< 快進狀態
	REWIND_STATUS		= MP3_STATUS_REWIND,		//!< 快退狀態
	FWRWD_EPILOG_STATUS	= MP3_STATUS_FWRWD_EPILOG,	//!< 快進退結束狀態
	ERROR_STATUS		= MP3_STATUS_ERROR,		//!< 錯誤狀態
}_MP3_AUDIO_STATUS;
/*!
提供 MP3API_STATUS 回傳值使用
*/
#define MP3_RUN_ERR	-1
#define MP3_RUN_NRR	0	// FS_NO_ERROR
typedef enum
{
	MP3API_ERR    = MP3_RUN_ERR,	//!< MP3API執行錯誤
	MP3API_NO_ERR = MP3_RUN_NRR,	//!< MP3API執行成功

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
對MP3 API以及PCM driver做初始化設定
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Init( int Voln );

/*!
釋放MP3 API以及PCM driver佔用到的軟硬體資源
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Uninit( void );

/*!
開啟MP3 file，並且取得播放MP3所需的相關資訊
@param filepath : 欲播放的MP3檔案之完整路徑
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Open( char *file );

/*!
播放開啟的MP3檔案
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Play( void );

/*!
確認MP3播放中
*/
extern int AudioMP3ChkPlaying( void );

/*!
關閉MP3檔案
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Close( void );

/*!
停止播放MP3
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Stop( void );

/*!
暫停播放MP3
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Pause( void );

/*!
繼續播放MP3
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3Continue( void );

/*!
取得目前MP3 API的狀態
@return MusicStatus : 回傳播放狀態
*/
extern _MP3_AUDIO_STATUS AudioMP3Status( void );
extern unsigned int *AudioMP3SpectrumData(void);

/*!
取得MP3歌曲的播放時間
@param SetMode : 設定回傳時間的模式\n
@return PlayTime : 依照參數SetMode的設定，回傳對應的時間
@note SetMode依照下列設定值，決定回傳時間的種類:\n
*/
#define PLAY_TIME_MODE		0 //!< 目前已播放時間
#define REMAIN_TIME_MODE	1 //!< 剩餘可播放時間
#define TOTAL_TIME_MODE		2 //!< 全部可播放時間
extern unsigned long AudioMP3GetPlayTime( int SetMode );

/*!
設定MP3歌曲的開始播放的時間
@param time_position : 欲設置的時間位置值
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3SetPlayTime( unsigned long time_position );

/*!
MP3音效設定(Equalizer)
@param EQ_num : 設定音效屬性
@return AudioErr : 回傳錯誤狀態參考值
@note 音效設定(預設參數，可依需求自行調整數量及模式)\n
    EQ_USER      : 使用者自定，調整 AudioMP3EqUserTBL[]\n
    EQ_NOMAL     : 自然\n
    EQ_ROCK      : 搖滾\n
    EQ_POP       : 流行\n
    EQ_CLASSICAL : 古典\n
    EQ_SOFT      : 柔和\n
    EQ_JASS      : 爵士\n
    EQ_BASS      : 重低音\n
*/
extern tAudioErr AudioMP3SetEQ(tAudioEQ EQ_num);

/*!
MP3 A-B段重複播放
@return AudioErr : 回傳錯誤狀態參考值
@note 播放進行中 : \n
    第一次呼叫，設置當前播放的時間位置為A段；\n
    第二次呼叫，設置當前播放的時間位置為B段，且開始A-B段重複播放；\n
    第三次呼叫，取消A-B段重複播放，繼續正常播放。\n
    <IMG SRC=abrepeat.PNG >
*/
extern tAudioErr AudioMP3ABRepeat( void );

/*!
設定音量的初始值，透過Set_volume設定起始的音量值
@param Set_volume  : 設定初始值的音量值
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3VolInit(int Set_volume);

/*!
將目前音量值往上提升一階，並且透過*get_volume取得設定後的音量值
@param *get_volume : 利用call by address，取得音量值
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3VolUp(int *get_volume);

/*!
將目前音量值往下降低一階，並且透過*get_volume取得設定後的音量值
@param *get_volume : 利用call by address，取得音量值
@return AudioErr : 回傳錯誤狀態參考值
*/
extern tAudioErr AudioMP3VolDown(int *get_volume);

/*!
預先設置MP3快進快退
@return AudioErr : 回傳錯誤狀態參考值
@note 執行AudioMP3Forward()或AudioMP3Rewind()之前必須先呼叫此function。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdProlog( void );

/*!
設置MP3快進，以目前的播放位置增加播放總時間的百分之一，若總時間的百分之一不足三秒，則只增加三秒
@return AudioErr : 回傳錯誤狀態參考值\n
@note 執行AudioMP3Forward()之前必須先呼叫此AudioMP3FwRwdProlog()，結束則要呼叫AudioMP3FwRwdEpilog()。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Forward( void );

/*!
設置MP3快退，以目前的播放位置減少播放總時間的百分之一，若總時間的百分之一不足三秒，則只減少三秒
@return AudioErr : 回傳錯誤狀態參考值\n
@note 執行AudioMP3Rewind()之前必須先呼叫此AudioMP3FwRwdProlog()，結束則要呼叫AudioMP3FwRwdEpilog()。\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3Rewind( void );

/*!
結束設置MP3快進快退
@return AudioErr : 回傳錯誤狀態參考值\n
<IMG SRC=FwRwd.PNG >
*/
extern tAudioErr AudioMP3FwRwdEpilog( void );
//! @}

//**************MP3 API Reserved Area Function List**************************************************
//! \defgroup MP3_API_FUN_GROUP MP3 API Reserved Area Function List
//! @{\ingroup KBMP3_MP3_API_Reserved_Area_Group

/*!
Function 指標，MP3 file的Open程式，並且取得播放MP3所需的相關資訊。
User可以透過此程式Open保留區或非保留區的檔案。
@param *filepath 		: MP3 file 的實際路徑(FAT)或者是file的順序號碼(Resevd area)
@return MP3API_STATUS 	: 回傳錯誤狀態參考值
*/
#define MP3API_FileOpen AudioMP3FileOpen
extern MP3API_STATUS AudioMP3FileOpen( char *filepath );

/*!
Function 指標，MP3 file的seek程式，將檔案指標位置移到指定的位置。
User可以透過此程式Seek保留區或非保留區的檔案位置。
@param offset 			: MP3 file放置於sector的偏移值
@return MP3API_STATUS 	: 回傳錯誤狀態參考值
*/
#define MP3API_FileSeek AudioMP3FileSeek
extern MP3API_STATUS AudioMP3FileSeek( unsigned long offset );

/*!
Function 指標，MP3 file的Read程式，讀取sector的資料。
User可以透過此程式Read保留區或非保留區的檔案資料。
@param *buf 	: 放置讀取資料的空間位置
@param len 		: 讀取的長度
@return  		: 回傳錯誤狀態(-1)或者讀取的長度
*/
#define MP3API_FileRead AudioMP3FileRead
extern int AudioMP3FileRead( __X char *buf, int len );

/*!
Function 指標，MP3 file的Closed程式，關閉目前檔案。
User可以透過此程式Close保留區或非保留區的檔案。
@return MP3API_STATUS 	: 回傳錯誤狀態參考值
*/
#define MP3API_FileClose AudioMP3FileClose
extern MP3API_STATUS AudioMP3FileClose( void );

/*!
Function 指標，MP3 file的讀取目前位置程式，得到目前檔案指定的位置。
User可以透過此程式得到保留區或非保留區的檔案指定位置。
@return 			 	: 回傳目前在檔案中指定的位置
*/
#define MP3API_GetPosition AudioMP3GetSize
extern unsigned long AudioMP3GetPosition( void );

/*!
Function 指標，MP3 file的讀取檔案大小程式，得到目前檔案的資料大小。
User可以透過此程式得到保留區或非保留區的檔案資料大小。
@return 			 	: 回傳目前的檔案的資料大小
*/
#define MP3API_GetSize AudioMP3GetSize
extern unsigned long AudioMP3GetSize( void );
//! @}
#endif // _MP3_API_H