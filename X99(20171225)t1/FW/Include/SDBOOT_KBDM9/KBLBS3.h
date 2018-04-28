/*!@{\ingroup KBDM9_SoftWareIPGroup

	\section intro Introduction
 	KBDM9 Low Bitrate Speech Decoder\n
 	
	KBDM9 �� low bitrate speech decoder�A�i�NKB��WaveEdit�Ҳ��ͤ�LBS3�榡���y�����Y�ɡA�����Y���٭쬰 16-bit PCM�y����ơC \n
	LBS3���榡�O�N8KHz, 16-bit(�Y128 kbps)��PCM���Y�� 3.6K/4.8K/6K/7.2K/8.4K/9.6K/10.8K/12K bps �� LBS3 ���.

   	\defgroup KBLBSGroup KBLBS Interface API Introduction

@}*/

#ifndef _KBLBSAPI_
#define _KBLBSAPI_


//!\defgroup KBLBSTypeDefGroup KBLBS Parse Head Category Lists
//!@{\ingroup KBLBSGroup

/*!
KBLBS_ParseHead function���^�ǭ�\n
�d�ߥΡA����user�@�i�@�B���B�z�C
*/
typedef enum{
	KBLBS_PARSE_HEAD_LAST_HEAD			=	0,			/**<Parse��KBC file�w�g�O�̫�@��head�F */
	KBLBS_PARSE_HEAD_FURTHERMORE_HEAD	=	1,			/**<Parse��KBC file�٤��O�̫�@��head�A��head�ҧi�������׼�����A�٦��U�@����� */
	KBLBS_PARSE_HEAD_ERR_VERSION		=	-1,			/**<Parse��KBC file�o�{���䴩��Version */
	KBLBS_PARSE_HEAD_ERR_MODE			=	-2			/**<Parse��KBC file�o�{���䴩��mode*/
}tKBLBSParseHeadStatus;

/*!
KBLBS_ParseHead function�ұo��head information
*/
typedef struct{
    unsigned int	Version ;						/**<Parse��KBC file�o�쪺�����O */	
   	unsigned int	Mode ;							/**<Parse��KBC file�o�����Y�榡 */	
 	unsigned int	Filter ;						/**<Parse��KBC file�o��ҳ]�w��filter�Ѽ� */
 	unsigned int	Length ;						/**<Parse��KBC file�o��ҭn���񪺸�Ʀ��X��frame */
  	unsigned int	Timber ;						/**<Parse��KBC file�o��ҳ]�w��Timber�Ѽ� */
  	unsigned int	Pitch ;							/**<Parse��KBC file�o��ҳ]�w��Pitch�Ѽ� */
   	unsigned int	Speed ;							/**<Parse��KBC file�o��ҳ]�w��Speed�Ѽ� */
    unsigned int	HeadLength ;					/**<Parse��KBC file�o�즹Head������ */
}tKBLBSHeadInfo;
//!@}


//!\defgroup KBLBSParseContentGroup PCM Parse Content Return Category Lists
//!@{\ingroup KBLBSGroup
/*!
KBLBS_ParseContent function���^�ǭ�\n
�d�ߥΡA����user�@�i�@�B���B�z�C
*/
typedef enum{
	KBLBS_PARSE_CONTENT_OK						=	1,			/**<���\�ѥX�@��frame*/
	KBLBS_PARSE_CONTENT_LAST_FRAME				=	0,			/**<Parse Content��̫�@��frame�F */
	KBLBS_PARSE_CONTENT_BITSTREAM_NOT_ENOUGH	=	-1,			/**<Bitstream buffer����Ƥ����Ѥ@��frame*/
	KBLBS_PARSE_CONTENT_ERR_FORMAT				=	-2			/**<���䴩��BitStream�榡*/
}tKBLBSParseContentStatus;
//!@}

//! \defgroup KBLBSFunctionGroup KBLBS Functions
//!@{ \ingroup KBLBSGroup

/*!
�]�wBitStream��Input Buffer�P���סA��Input Buffer�ݬ�Circle Buffer
@param bs_buf : BitStream��Input Buffer�Ҧb��m
@param bs_len : ��Input Buffer���j�p
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_Init(__circ int * bs_buf, int bs_len);

/*!
Ū��Input Buffer�����e�ȡA���R��KBC file�����Y��T�C
@param HeadInfo : ���Y��T���s���m
@return tKBLBSParseHeadStatus
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern tKBLBSParseHeadStatus KBLBSAPI_ParseHead(tKBLBSHeadInfo *  HeadInfo);

/*!
�C����@����function�Ѥ@��frame�C
@param buffer : �ѥX�Ӫ���ơA�ҭn�s�񪺦�m
@param FrameSize : �ѥX�Ӫ���Ʀ��h�j�A���T�����s���m
@return 	tKBLBSParseContentStatus
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/	
extern tKBLBSParseContentStatus KBLBSAPI_ParseContent(int * buffer , unsigned int *FrameSize); 

/*!
�T�ݥثeBitStream Input Buffer�٦��h�֪Ŷ�
@return �^�ǳѾl�Ŷ��A�Hwords�����
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/	 
extern unsigned int KBLBSAPI_QueryBuf(void);


/*!
���榹function�A�N�qBitStream�ҫ��쪺��m�A�h���׬�len����ơA��BitStream Input Buffer���C
@param BitStreamPtr :BitStream���Ҧb��m
@param len :�n�h��Input Buffer������ƪ��סA�Hword�����A�q�`�|����KBLBS_QueryBuf()���^�ǭ�
@sa KBLBS_QueryBuf
@note \b Code \b Address \b Name => \b KBLBSAPI_CODE
*/
extern void KBLBSAPI_FillInputBuf(const int * BitStreamPtr , unsigned int len);

/*!
��������l�Ƴ]�w�ѼƤ����s��l��function�A�M��BitStream�����
@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_Reset(void );

/*!
�NLBS3 Decoder���J��API

@note \b Code \b Address \b Name => \b KBLBSAPI_INIT_CODE
*/
extern void KBLBSAPI_LBS3_PlugIn(void);

//!@}
#endif