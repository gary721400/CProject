/*!@{\ingroup KBDM9_SoftWareIPGroup

	\section intro Introduction
 	KBDM9 Low Bitrate Speech Decoder\n
 	
	KBDM9 的 low bitrate speech decoder，可將KB的WaveEdit所產生之LBS3格式的語音壓縮檔，解壓縮並還原為 16-bit PCM語音資料。 \n
	LBS3的格式是將8KHz, 16-bit(即128 kbps)的PCM壓縮為 3.6K/4.8K/6K/7.2K/8.4K/9.6K/10.8K/12K bps 的 LBS3 資料.

   	\defgroup KBLBSGroup KBLBS Interface API Introduction

@}*/

#ifndef _KBLBSAPI_
#define _KBLBSAPI_


//!\defgroup KBLBSTypeDefGroup KBLBS Parse Head Category Lists
//!@{\ingroup KBLBSGroup

/*!
KBLBS_ParseHead function的回傳值\n
查詢用，提供user作進一步的處理。
*/
typedef enum{
	KBLBS_PARSE_HEAD_LAST_HEAD			=	0,			/**<Parse此KBC file已經是最後一個head了 */
	KBLBS_PARSE_HEAD_FURTHERMORE_HEAD	=	1,			/**<Parse此KBC file還不是最後一個head，此head所告知的長度播完後，還有下一筆資料 */
	KBLBS_PARSE_HEAD_ERR_VERSION		=	-1,			/**<Parse此KBC file發現不支援的Version */
	KBLBS_PARSE_HEAD_ERR_MODE			=	-2			/**<Parse此KBC file發現不支援的mode*/
}tKBLBSParseHeadStatus;

/*!
KBLBS_ParseHead function所得到head information
*/
typedef struct{
    unsigned int	Version ;						/**<Parse此KBC file得到的版本別 */	
   	unsigned int	Mode ;							/**<Parse此KBC file得到壓縮格式 */	
 	unsigned int	Filter ;						/**<Parse此KBC file得到所設定的filter參數 */
 	unsigned int	Length ;						/**<Parse此KBC file得到所要播放的資料有幾個frame */
  	unsigned int	Timber ;						/**<Parse此KBC file得到所設定的Timber參數 */
  	unsigned int	Pitch ;							/**<Parse此KBC file得到所設定的Pitch參數 */
   	unsigned int	Speed ;							/**<Parse此KBC file得到所設定的Speed參數 */
    unsigned int	HeadLength ;					/**<Parse此KBC file得到此Head的長度 */
}tKBLBSHeadInfo;
//!@}


//!\defgroup KBLBSParseContentGroup PCM Parse Content Return Category Lists
//!@{\ingroup KBLBSGroup
/*!
KBLBS_ParseContent function的回傳值\n
查詢用，提供user作進一步的處理。
*/
typedef enum{
	KBLBS_PARSE_CONTENT_OK						=	1,			/**<成功解出一個frame*/
	KBLBS_PARSE_CONTENT_LAST_FRAME				=	0,			/**<Parse Content到最後一個frame了 */
	KBLBS_PARSE_CONTENT_BITSTREAM_NOT_ENOUGH	=	-1,			/**<Bitstream buffer內資料不足解一個frame*/
	KBLBS_PARSE_CONTENT_ERR_FORMAT				=	-2			/**<不支援的BitStream格式*/
}tKBLBSParseContentStatus;
//!@}

//! \defgroup KBLBSFunctionGroup KBLBS Functions
//!@{ \ingroup KBLBSGroup

/*!
設定BitStream的Input Buffer與長度，此Input Buffer需為Circle Buffer
@param bs_buf : BitStream的Input Buffer所在位置
@param bs_len : 此Input Buffer的大小
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_Init(__circ int * bs_buf, int bs_len);

/*!
讀取Input Buffer的內容值，分析此KBC file的檔頭資訊。
@param HeadInfo : 檔頭資訊的存放位置
@return tKBLBSParseHeadStatus
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern tKBLBSParseHeadStatus KBLBSAPI_ParseHead(tKBLBSHeadInfo *  HeadInfo);

/*!
每執行一次此function解一個frame。
@param buffer : 解出來的資料，所要存放的位置
@param FrameSize : 解出來的資料有多大，此訊息的存放位置
@return 	tKBLBSParseContentStatus
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/	
extern tKBLBSParseContentStatus KBLBSAPI_ParseContent(int * buffer , unsigned int *FrameSize); 

/*!
訊問目前BitStream Input Buffer還有多少空間
@return 回傳剩餘空間，以words為單位
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/	 
extern unsigned int KBLBSAPI_QueryBuf(void);


/*!
執行此function，將從BitStream所指到的位置，搬長度為len的資料，到BitStream Input Buffer內。
@param BitStreamPtr :BitStream的所在位置
@param len :要搬到Input Buffer內的資料長度，以word為單位，通常會等於KBLBS_QueryBuf()的回傳值
@sa KBLBS_QueryBuf
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/
extern void KBLBSAPI_FillInputBuf(const int * BitStreamPtr , unsigned int len);

/*!
不改任何初始化設定參數之重新初始化function，清空BitStream內資料
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_Reset(void );

/*!
將LBS3 Decoder掛入此API

@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_LBS3_PlugIn(void);

//!@}
#endif