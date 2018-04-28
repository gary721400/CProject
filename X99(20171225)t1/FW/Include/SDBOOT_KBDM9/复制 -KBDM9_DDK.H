#if __DOGVER__<12220
	#error "Dogstar Version should be higher than 1.2.2-20 to compile this package!!"
#endif

//#define PACKAGE_VER 0x0904

#ifndef _KMDM9_DDK_
#define	_KMDM9_DDK_


/*!
KBDM9 目前所提供的SoftWare IP JumpTable的Trap No\n
*/
typedef enum{
	SET_FLASH_BUSY_TRAP 	= 0x40,		/**<为避免同一个储存媒体，同时被不同function呼叫时，导致重覆进入造成状态及RAM的冲突，所设定的Busy flag */
	CLEAR_FLASH_BUSY_TRAP	= 0x41,		/**<为避免同一个储存媒体，同时被不同function呼叫时，导致重覆进入造成状态及RAM的冲突，所清除的Busy flag */	
	NFC_DELAY_CALLBACK_TRAP	= 0x42,		/**<NAND-FLSAH 忙录中，会将主控权交给AP，籍由此Trap No所指到的位置，跳到AP的function，以避免效能的浪费*/
	SD_DELAY_CALLBACK_TRAP	= 0x43,		/**<SD 忙录中，会将主控权交给AP，籍由此Trap No所指到的位置，跳到AP的function，以避免效能的浪费*/
	STORAGE_SET_BUSY 		= 0x44,		/**<为避免不同的储存媒体同时存在时之Pin脚冲突，所设定的Busy flag */
	STORAGE_CLR_BUSY		= 0x45,		/**<为避免不同的储存媒体同时存在时之Pin脚冲突，所清除的Busy flag */
	Set_File_System_Busy = 0x47,
	Clr_File_System_Busy = 0x48,
	Is_CWrite_Protect = 0x49,
	FS_Callback = 0x4A,
	GET_DATE_NTIME = 0x4B, 
	I_Change_Drive = 0x4C 
}tKBMP3_IP_JumpTableTrapNo;
 
/*
**	Direct access to DMCU memory areas.
*/

#ifndef _ABSACC_H_
#define _ABSACC_H_
/*
** Direct access Byte/Word
*/
#define m8HI4(arg)			(arg>>4)
#define m8LO4(arg)			(arg&0x0F)

#define m16HI8(arg)			(*( unsigned char *)&(arg))
#define m16LO8(arg)			(*((unsigned char *)&(arg) + 1))

#define m32HI16(arg)		(*( unsigned int  *)&(arg))
#define m32LO16(arg)		(*((unsigned int  *)&(arg) + 1))

#define m32HI8(arg)			(*( unsigned char *)&(arg))
#define m32MH8(arg)			(*((unsigned char *)&(arg) + 1))
#define m32ML8(arg)			(*((unsigned char *)&(arg) + 2))
#define m32LO8(arg)			(*((unsigned char *)&(arg) + 3))

/*
** Direct access GPR/YRAM/XRAM
*/
#define XBYTE ((__X volatile unsigned char *) (unsigned int)0x0000)
#define XWORD ((__X volatile unsigned int  *) (unsigned int)0x0000)
#define XLONG ((__X volatile unsigned long *) (unsigned int)0x0000)

#define YBYTE ((__Y volatile unsigned char *) (unsigned int)0x8000)
#define YWORD ((__Y volatile unsigned int  *) (unsigned int)0x8000)
#define YLONG ((__Y volatile unsigned long *) (unsigned int)0x8000)

#define GPREG ((__X volatile unsigned int  *) (unsigned int)0xF2E0)

#define __near __X

extern void* nearcpy(__near void* d, __near void* s, unsigned int l);

extern void* nearset(__near void* p, unsigned int d,  unsigned int l);

extern int nearcmp(__near void* m, __near void* n,  unsigned int l);

extern unsigned int min(unsigned int i, unsigned int j);

extern unsigned int max(unsigned int i, unsigned int j);


#endif	//_ABSACC_H_


#ifndef _clock_drv_h
#define _clock_drv_h

//! \defgroup SysClockGroup System Clock setting 
//! @{	\ingroup KBMP3_SystemGroup


/*! 	
	KBDM9可设定的系统频率\n
*/
typedef enum{
	FEQ_80M     =  2, /**< System clock = 80 MHz    */   
	FEQ_60M		  =	 3,	/**< System clock = 60 MHz    */
	FEQ_48M		  =	 4,	/**< System clock = 48 MHz    */
	FEQ_40M		  =	 5,	/**< System clock = 40 MHz    */
	FEQ_34P28M	=	 6,	/**< System clock = 34.28 MHz	*/
	FEQ_30M		  =	 7,	/**< System clock = 30 MHz    */
	FEQ_26P66M	=	 8,	/**< System clock = 26.66 MHz */
	FEQ_24M		  =  9,	/**< System clock = 24 MHz    */
	FEQ_20M		  =	11,	/**< System clock = 20 MHz    */
	FEQ_16M  		=	14,	/**< System clock = 16 MHz    */
	FEQ_15M     = 15, /**< System clock = 15 MHz    */
	FEQ_12M     = 19, /**< System clock = 12 MHz    */
	FEQ_10M     = 23, /**< System clock = 10 MHz    */ 
	FEQ_9P6M    = 24, /**< System clock = 9.6 MHz   */ 
	FEQ_8M      = 29, /**< System clock = 8 MHz     */
	FEQ_7P5M    = 31  /**< System clock = 7.5 MHz     */
}tFEQ;



/*!	
	 执行此function设定KBDM9的System Clock
	@Param	Freq :设定的频率
	@Return 
		0 : OK @n
		-1 : 不支援此设定 \n
	@note \b Code \b Address \b Name => \b SYSDRV_CODE 	
		
		
*/
extern unsigned int	SystemClock( tFEQ Freq);

/*!
询问目前的系统频率设定值
@return 目前的系统频率设定值
@note \b Code \b Address \b Name => \b SYSDRV_CODE
*/
extern tFEQ QuerySysClock(void);

#endif	//_clock_drv_h
/*!

//==============================================================================
//! \defgroup KBMP3_SystemGroup  System tool functions

//! @{						

/*!
若　使用此body内的任何IP，必　呼叫此function，只　要执行一次即可。
@note \b Code \b Address \b Name => \b IP_INIT_CODE
*/
extern void IP_INIT(void);	





/*!
设定interrupt service routine的中断向量\n
@param  TrapNo -->中断发生装置的TrapNo编号，请参考KBMP3 data sheet
@param  pIsrFunc -->中断发生时所有执行的function，此function若以C编写，必　是__interrupt宣告，若为ASM必　是RETI。
@return None
@note \b Code \b Address \b Name => \b SPACE_ROM
*/							
extern void SetVector(unsigned int TrapNo, void (*pIsrFunc)(void));


/*!
将XRAM内某一个指定区域的内容值，copy到IRAM某一个指定区域内\n
@param source_ptr =>XRAM内复制来源的区域的起始位置。
@param destination =>IRAM内复制目标区域的起始位置。
@param data_length =>所要复制的大小，以Bytes为单位
@note data_length长度限制，必　是偶数，且最小为6
@note \b Code \b Address \b Name => \b SPACE_ROM
*/
extern void CopyXRAMtoIRAM(	__X int* source_ptr, int* destination, unsigned int data_length);


/*! \mainpage KBDM9 NFTL API function libraries

    \section intro_sec Introduction
    This is Nand Flash Transfer Layer library.\n\n
    - NFTL Version 1.00\n\n

    \defgroup ApiGroup NFTL API functions
     \defgroup IDGroup ID Code Lists
*/
#include "storbase.h"

#ifndef _NFTL_H_
#define _NFTL_H_


/******************************
* Chip Type
******************************/
#define SAMSUNG_K9F1208     0x0076
#define SAMSUNG_K9F1G08     0x00F1
//! @{	\ingroup IDGroup
#define NF1208              0x0076 /*!< samsung 1208 series compatible */
#define NF1G08              0x00F1 /*!< samsung 1g08 series compatible */
#define NF2G08              0x00DA /*!< samsung 2g08 series compatible */
#define NF4G08              0x00DC /*!< samsung 4g08 series compatible */
#define NF8G08              0x00D3 /*!< samsung 8g08 series compatible */
//! @}	\ingroup IDGroup
/******************************
* Flash Information
******************************/

/*! \struct _nand_flash_info
    NAND Flash Information
*/
typedef struct _nand_flash_info {
	unsigned int	chipType;    /*!< chip id */
	unsigned int	pageSize;    /*!< page size in bytes */
	unsigned int	spareSize;   /*!< spare size in bytes */
	unsigned int	blockCount;  /*!< total block count */
	unsigned long	blockSize;  /*!< block size in bytes */
} NAND_FLASH_INFO;

/*!< \struct _nf_block_info
    information for one block in NAND Flash
*/
typedef struct _nf_block_info {
    unsigned int    numUsedBlocks; /*!< block number is in used */
    unsigned int    numFreeBlocks; /*!< block number is free */
    unsigned int    numBadBlocks;  /*!< block number is bad */
    unsigned int    percent; /*!< percent completed */
    unsigned int    cancel;  /*!< cancel or not */
} NF_BLOCK_INFO;

/* Function prototype */

//! @{	\ingroup ApiGroup
/*!	Unmount NAND Flash\n
 @return     0 is correct, others value see \ref ErrGroup
 @note
- All APIs will be useless after this function.
*/
int nftl_unmount(void);
//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	Read one sector data from Nand Flash\n
	It reads one sector of data from a specific sector number on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_read(unsigned long sector, __X void* buf);
//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	Write one sector data to Nand Flash\n
	It writes one sector of data from a specific sector number to the NAND flash.
 @param  sector --> which the sector number will be written.
 @param  buf    --> the source data buffer.
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_write(unsigned long sector, __X void *buf);
//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	It returns the size of total usable space.\n
 @return     size in sectors.
*/
unsigned long nftl_get_size(void);
//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	THis function is useless.
*/
void nftl_prepare(unsigned long num);
//! @}	\ingroup ApiGroup


//! @{	\ingroup ApiGroup
/*!	It allocates the NAND flash so that it reserves a certain size to be a special use. \n
It also erases all blocks and builds the NFTL system.
 @param  resv_ksize --> how many KBytes of reserved area..
 @param  boot_ksize --> how many KBytes of boot code/const data area..
 @param  stamp      --> custom's stamp string, 11 bytes.
 @return      0 is correct, others value see \ref ErrGroup
 @note
- nftl_mount() must be called prior to calling nftl_allocate() and you cannot call nftl_allocate() if nftl_mount() returns NFTL_MEDIA_ERR.
- nftl_allocate() will destroy all data on the media.
- The size of reserved area must be the multiples of a block size, so the returned value may be differ from what you specified.
*/
int nftl_allocate2(unsigned long resv_ksize, unsigned int boot_ksize, char* stamp );
#define nftl_allocate( resv_ksize )  nftl_allocate2( (resv_ksize), 1, "" )
//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	It returns the size of reserved area on the NAND flash.
 @return     The size of reserved area in sectors.
 @note \n The size of reserved area must be the multiples of a block size.
*/
unsigned long nftl_resv_size(void);

//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	It reads one sector of data from the reserved area on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref ErrGroup
 @note
 -	The behavior of nftl_resv_read() is similar to nftl_read().
 -	The starting sector is number 0.
 -	The size of reserved area doesn't decrease even bad blocks increase.
*/
int nftl_resv_read(unsigned long sector, __X void* buf);

//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	It writes one sector of data to the reserved area on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to get..
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_resv_write(unsigned long sector, __X void* buf);

//! @}	\ingroup ApiGroup

//! @{	\ingroup ApiGroup
/*!	It force flush data in memory cache to NAND flash.
 @return     0
 @note \n User must call this function before ending of application
*/
int nftl_flush(void);
//! @}	\ingroup ApiGroup



/** \example example.c
 * This is an example of how to use the Test class.
 * More details about this example.
 */

#endif /* _NFTL_H_ */


/*! \mainpage KBDM9 SD/MMC Driver Function Libraries
    - SD/MMC Driver Version 0.1\n\n
    
    \section intro_sec Introduction
    This is SD and MMC Card Physical Layer driver library.\n\n
    It supports the following SD and MMC card versions. \n\n
    
    SD Card: \n
    - Physical Layer Version 1.0 ~ 1.1 Standard Capacity SD Card \n\n
    - Physical Layer Version 2.0 Standard and High Capacity(HC) SD Card \n\n
    
    MMC: \n
    - MMC System Spec. Version 3.0 ~ 3.31, 1-bit bus width
    - MMC System Spec. Version 4.0, 1-bit or 4-bit bus width  
*/

#ifndef _SD_DRIVER_H
#define _SD_DRIVER_H

//!    \defgroup ApiGroup SD/MMC Card Driver API Functions

//#define	 WIDE_4		    1
//#define	 WIDE_1		    0

typedef  unsigned char	BYTE;
typedef  unsigned int   WORD;
typedef  unsigned long  DWORD;



/* Function prototype */

//! @{	\ingroup ApiGroup

/*!	Initialize a SD or MMC card \n\n
		This function initializes registers to attach the first SD or MMC.\n\n
 	
 @return     0 is success, others value refer to the file "sd_mmc.inc"
*/
extern int SD1_Init(void);

extern int SD2_Init(void);
/*!	Write data to SD or MMC card\n\n
	It writes 'block_count' blocks of data to specific block numbers in the SD or MMC card. \n\n
 @param  block_addr --> the beginning block number to be written.
 @param  ram_sdbufW --> the data buffer which contains the the data.
 @param  block_count --> the amount of blocks to be written.
 @return     0 is correct, others value refer to the file "sd_mmc.inc"
*/

extern int SD1_read_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);


extern int SD2_read_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);

extern int SD1_write_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);


extern int SD2_write_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);


extern int SD1_erase_all(void);

extern int SD2_erase_all(void);

extern int SD1_fast_mode(void);

extern int SD2_fast_mode(void);

extern unsigned int SD_no_of_written_blocks(void);

extern unsigned long SD1_Total_Block;
extern unsigned long SD2_Total_Block;
extern unsigned int SD1_Insert;
extern unsigned int SD2_Insert;

#endif	





//****************************************************************************************************************
//!@{

/*!
	KBDM9周边Communication相关的Driver介绍 \n
	主要有：USB、SPI、I2C、UART
	
*/
//!    \defgroup KBDM9_PeripheralGroup KBDM9 Peripheral API Functions   


//!@}


//!@{\ingroup KBDM9_PeripheralGroup
/*! 

 \section intro Introduction
 USB Library for KBMPT03\n
 -       version 1.0\n\n
  \section us_sect Using Library
    User need to prepare extra memory for library usage.\n
    If user want USB to show 2 device, user may declare as:\n\n
    -  _STORAGE usb_stor[2]\n\n
    HID flow :
     <IMG SRC=hid_flow.PNG > \n
    \defgroup USBfunctionGroup	USB Library API
   
*/

//!@}





//********************************************************************************************************

/*!@{
    \defgroup KBDM9_SoftWareIPGroup KBDM9 Software IP API Functions   

 	KBMP3与周边无关，纯软体运算之IP介绍，各类的压缩，解压缩应用 \n
 	主要有：包含mp3 decode、WMA decode等\n

@}*/


/*!@{\ingroup KBDM9_SoftWareIPGroup

	\section intro Introduction
 	Mp3 decoder Interface\n
 	-       version 0.1\n\n
	-
	KB MP3 IP provides the ability for DMCU to play MP3 music. The MP3 decoder supports the MPEG 1/2/2.5
	Layer 3 , MPEG 1 Layer2 and VBR format. The input stream and the audio output all can be controlled by users
	program. So it can be used on many application fields such as mp3 player, wireless player, ELA, ED, TV, CD
	MP3…, etc. There are many other software IPs could be integrated with the MP3 IP to form applications quickly.
	The IPs are PCM_IRQ, PCM_DTU, MIXER, FAT, OS..., etc.
	

   \defgroup Mp3DecoderGroup Mp3 Decoder Introduction

@}*/

#ifndef _MP3_H_
#define _MP3_H_


//!@{\ingroup Mp3DecoderGroup
/*!
The global variables could be referenced in application assembler program by access the name with ‘_’ prefixed.
Using C program to access it need not with ‘_’ prefix. For example, to access_mp3Ctrl variable, in assembly
language user can use “_mp3Ctrl” as a direct memory address. In C program, it should use “mp3Ctrl” as an
external variable.Application should not modify these variables directly. These variables are used for passing 
the status of mp3 decoding to applications.
\defgroup Mp3VarGroup MP3 Global Variables
*/
//!@}

//!\defgroup Mp3VarListGroup MP3 Global Variables List
//!@{\ingroup Mp3VarGroup

/*!
The sampling rate be supported by mp3 decoder as follows.
*/
typedef enum{
	SMP_8K   		=	1,		/**< Sampling rate = 8Khz */
	SMP_16K  		=	2,		/**< Sampling rate = 16Khz */
	SMP_32K  		=	3,		/**< Sampling rate = 32Khz */
	SMP_11P025K  	=	4,		/**< Sampling rate = 11.025Khz */
	SMP_22P05K  	=	5,		/**< Sampling rate = 22.05Khz */
	SMP_44P1K  		=	6,		/**< Sampling rate = 44.1Khz */
	SMP_12K  		=	7,		/**< Sampling rate = 12Khz */	
	SMP_24K  		=	8,		/**< Sampling rate = 24Khz */
	SMP_48K  		=	9		/**< Sampling rate = 48Khz */
}tMp3SampleRateID;

/*!	SmpRateID could be checked to see what sampling rate is the mp3 decoder output. */
extern tMp3SampleRateID mp3SmpID;

/*!Decoded frame counter*/
extern unsigned long __X mp3FrameCount;

/*!Decoded bit stream word counter*/
extern unsigned long __X mp3WordCount;

/*!
 Information 1 \n
\B	bit0: Protection \n
\B	bit1..2: Layer \n
\B	bit3: ID \n
bit 4..7: Reserved.
*/
extern unsigned char __X mp3HeaderInfo1;

/*!
Information 2 						\n
\b bit0..1: emphasis 				\n
\b bit2: original 					\n
\b bit3: copyright					\n
\b bit4..5: mode extension			\n
\b bit6..7: mode					\n
\b bit8: private bit				\n
\b bit9: padding bit				\n
\b bit10..11: sampling frequency	\n
\b bit12..15: bit rate				\n
*/
extern unsigned int __X mp3HeaderInfo2;

/*!
 # of ancillary bits of current frame
*/
extern unsigned int __X mp3NumAncillaryBit;

/*!
ancillary bits of current frame
*/
extern unsigned char * __X mp3AncillaryData;

/*!
\b bit0..3: Right Channel Volume (16 levels) \n
\b bit4..7: Left Channel Volume (16 levels)
*/
extern unsigned char __X mp3VolumeCtrl;

/*!
Bit stream buffer (BS_BUF) tail pointer. User
should fill the bit stream to the address
pointed by this pointer and then update this
pointer
*/
extern unsigned int __X mp3Buf;

/*!
Report the bit rate of the decoded bit stream \n
Unsigned integer represent the bit rate (KHz) of the decoding bit stream
*/
extern unsigned int __X mp3Bitrate;

/*!
Decode bit stream error count \n
Use this count to know whether bit stream has error
*/
extern unsigned int __X mp3ErrorCnt;

/*!
Pause and equalizer control 			\n
\b bit0: 0: disable; 1: enable PAUSE	\n
\b bit1: Reserved						\n
\b bit2: Control EQ enable 				\n
\b bit3..15: reserved 					\n
*/
//extern __DPRAM unsigned int mp3Ctrl;

/*!
Flag on decoding \n
bit 0~9: Reserved, do not change it \n
\b bit10: 1:MPEG2/2.5, 0:MPEG1 \n
\b bit11: stereo2 \n
\b bit12~13: reserved \n
\b bit14: stereo mode \n
\b bit15: fsGot (header coded and got sampling \n
rate)
*/
extern __DPRAM int  mp3Flag;

/*!
 rst=0x0 when mp3_rst() \n
\b bit15: free format detection, 要求侦测 nSlots of free format \n
\b bit14: (1)free format mp3 file found and nSlots calculate OK (1)otherwise \n
 bit13: reserved \n
\b bit12: SYNCWORD_FOUND, (1)SYNCWORD已找到, 但该frame之bit stream尚未读完 (0)　重新找SYNC_WORD \n
\b bit11: preflagCur, preflag of channel which is under processing \n
\b bit10: scDirty, 第一次进入DEQUANTIZATION_4时未必见得会碰到boundary, 因此scVar[]可能不会更新, 靠此讯号通知DEQUANTIZATION_4更新scVar[] \n
\b bit9: ms_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit8:  i_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit7: WORKING_CH, (0)正处理ch0, (1)正处理ch1, valid only in BACK_END()/POST_PROCESSING() \n
\b bit6: bsInitOK, bit stream struct (0)尚未初始化 (1)初始化完毕 \n
\b bit5: NEXT_GR, (0)下一个处理gr0, (1)下一个处理gr1 \n
\b bit4: bAUX1, temporarily bit storage \n
 bit3..0: reserved
*/
extern __DPRAM int  mp3Flag2;


/*!
mp3EQgain[sb0..31]: UNS0.16, 0x8000=0.5 \n
synthesis 时再将音量 double
*/															
extern __Y unsigned int mp3EQgain[];

/*!
Report the stereo or mono of the decoded bit stream \n
1=>Stereo \n
0=>Mono
*/
extern __X int mp3StereoMode;


/*!
mp3 decode error status \n
\b bit0: LAYER_ERR    , not layer III or II file \n
\b bit1: CRC_ERR      , crc check fail \n
\b bit2: BAD_BYTES_ERR, bad_bytes_to_discard \n
\b bit3: HUFF_DEC_ERR, HUFFMAN_DECODER_ERROR \n
\b bit4: BS_NOT_ENOUGH, _mp3_bs_buf[]中的资料不足一个frame \n
bit15~5: reserved
*/
extern unsigned int mp3ErrorStatus; 		 

//!@}

//!\defgroup Mp3DecFunGroup MP3 Decoder function List
//!@{\ingroup Mp3DecoderGroup
/*!
Initial MP3 decoder IP and set the five buffers pointers. This function should be the first function to initialize MP3 IP.\n
In the normal concept, the first four buffers are mainly used for stereo dual buffer operation.
The first stereo buffer is so called PLAY_BUF_L1 and PLAY_BUF_R1. The second stereo
buffer is PLAY_BUF_L2 and PLAY_BUF_R2. These two stereo may switch each other during
decode and play processing. DMCU MP3 IP uses one more buffer named HEAP buffer to do
acceleration. The HEAP buffer may change with one of the four buffers after a frame decoded.
So application should call mp3_GetPlayBufPtr() to get the changed play buffer pointers to play the correct output sound.\n

The bit stream buffer should be defined as a circular buffer. There is a directive in assembly
language “.CIRCBUF”, application should use this directive to define the bit stream buffer. \n
For example,\n
.area MP3_RAM2(XRAM) \n
_bitstream_buf:: .CIRCBUF 721\n
This example defines a 721 words circular buffer for the bit stream buffer. Please note that the
circular buffer declaration should with a dedicated “.area” definition. Don’t declare it with
other variables in the same area.\n
@param pu32MemPool1 =>Buffer Size is 576 words. The buffer is for decoding buffer left channel .
@param pu32MemPool2 =>Buffer Size is 576 words. The buffer is for decoding buffer right channel.
@param pu32MemPool3 =>Buffer Size is 576 words. The buffer is for decoding buffer left channel .
@param pu32MemPool4 =>Buffer Size is 576 words. The buffer is for decoding buffer right channel.
@param HEAP => Buffer Size is 576 words. This buffer is used for store the temporary decoded data or final PCM data.
@param bs_buf => The input bit stream buffer, Buffer size could be defined by user for different application requirement.
@param bs_len=> The bit stream buffer size counted in words.
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_init(	int *pu32MemPool1 , int *pu32MemPool2 , int *pu32MemPool3 , int *pu32MemPool4, int *HEAP, int *bs_buf, int bs_len);
					
/*!
Reset the mp3 decoder, flush the bit stream buffer and temporary decoded data. When an
application needs to play another song, it should call this function to let mp3 decoder seek the
sync word to decide new sampling rate.
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_rst(void);

/*!
To know the empty word numbers in the BS_BUF, application should call this function to get
the return value (word number). The start address pointer of empty BS_BUF is stored in
_mp3Buf (global variable).\n
The BS_BUF is a circular buffer. The size is 721 words in default (could be set by user). To fill
the bit stream data into it, application should set the circular buffer pointer and size (counted in
byte) into X index registers.The operation that fill bit stream to BS_BUF will automatically scroll to the start address of
BS_BUF when the X0 exceed the end of BS_BUF. \n
Please note that in the new MP3 IP, it is not necessary to fill all the empty word numbers into BS_BUF in one time. The
application can separate two or more times to fill data into BS_BUF. After each filling, the ‘_mp3Buf’ pointer should 
be updated to let mp3dec function know the last data pointer.
@return: Return the length of empty BS_BUF word numbers that application can fill bit stream
data into it. The returned value is stored in R0. The unit is counted by word number.
@note \b Code \b Address \b Name => \b MP3DEC_CODE
*/
extern unsigned int mp3QueryBuf(void);

/*!
Decode one granual (1/2 frame ) data to play buffer. This function is the mainly routine to do
mp3 decoding. It will parse the bit stream header and recognize the format of the bit stream
include data rate, mono/stereo mode and the sampling rate, etc. \n
After decoding one granual of data, application should get the play buffer pointer by calling
mp3_GetPlayBufPtr().\n
This function will not backup and restore the registers it used for improving the performance.
When cooperate with OS, the backup and restore are not needed. But when application handle
the mp3 decoder IP directly without OS and switch to other IP like MIDI or LBS, the register
may need be saved or cleared on switching between these different IPs.\n
Mp3dec() function can be triggered to execute from interrupt service routine to ensure the
decode timing. But it can’t be called directly in interrupt service routine. There is a trigger to
execute method demo in example code. Please refer to it. Note that an important issue is that
mp3dec() can’t be re-entrant\n
@return  0 =>decode fail \n
		1 => decode OK
@note \b Code \b Address \b Name => \b MP3DEC_CODE
*/
extern int mp3dec(void);

/*!
Get the current play buffer that the decoded PCM data placed there. Because MP3 IP will auto
change the play buffer pointers, application should call this function to get the updated play
buffer address.
@param LeftBuf =>mp3 decode出来的左声道资料所在位置
@param RightBuf =>mp3 decode出来的右声道资料所在位置
@note \b Code \b Address \b Name => \b MP3DEC_CODE
*/
extern void mp3_GetPlayBufPtr(void *LeftBuf, void *RightBuf);

/*!
Register a user call back function to handle the case when some error occurred in mp3
decoding. The error case has two types. The error case is mainly come from bit stream error. To
process this situation, the user call back function can play the last frame data or play a silence
frame or not doing any play. The call back function can also display some error message on
LCD to tell user what happening. The global variable “mp3ErrorStatus” will set the
corresponding bit to let user distinguish the error type.
@sa mp3ErrorStatus
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_SetErrorFunc(void (*CallBackFun)(void));		// call back function: For handling error case

/*!
Set the equalizer attributes to the MP3 decoder. The first parameter is the equalizer attributes
array. It should be an unsigned integer array, the element number of the array can be set by the
parameter EqNo. The number should between 1~32. There are 32 frequency bands in the mp3
decoder. The default values of the equalizer attributes are 0x8000, it represent x1.000 to the
frequency bands. The maximum value settable is 0xFFFF, it means the frequency band will be
emphasize for about x2.000. The minimum value is 0x0000, it means zero volume of the
frequency band.\n
Each frequency band width is calculated by dividing the total bandwidth of the mp3 music with
32. For example, a 44.1KHz mp3 music’s bandwidth may be 22.05KHz, so each band
frequency bandwidth is about (22.05 / 32)KHz.

@param EQList=> equalizer attributes array
@param EqNo =>The number should between 1~32. There are 32 frequency bands in the mp3 decoder.
@note \b Code \b Address \b Name => \b MP3DEC_TEMP2_CODE

*/
extern void mp3_SetEQ(unsigned int *EQList, int EqNo);	

/*!
Enable the mp3 equalizer calculation.
@note \b Code \b Address \b Name => \b MP3DEC_TEMP2_CODE
*/
extern void mp3_EnableEQ(void);

/*!
Disable the mp3 equalizer calculation.
@note \b Code \b Address \b Name => \b MP3DEC_TEMP2_CODE
*/
extern void mp3_DisableEQ(void);


/*!
执行 mp3dec()前，必　执行此function
*/
extern void mp3Clear(void);

/*!
This function will setup the pointer of the buffer to put the spectrums data form mp3dec()
function. User can get the information to display spectrums from the buffer.
@param spectrum_ptr => spectrums资料所存放的位置，必　指到32个word的空间
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_spectrum_data(unsigned int * spectrum_ptr);

#endif
//!@}

//=============================================================================


void NFTL_CommonLoadCode(unsigned int CodeID);

void SPI_CommonLoadCode(unsigned int CodeID);

void SD_CommonLoadCode(unsigned int CodeID);



#ifndef _SDtl_H_
#define _SDtl_H_


typedef enum {
  SDTL_No_Err           =  0,		/*!< No Error */
  Data_CRC_Err          =	-1,		/*!< Data CRC Error */
  RSP_CRC_Err	          =	-2,		/*!< Command Response CRC Error */
  CMD_RSP_Timeout       =	-3, 		/*!< Command Response Timeout */
  PRG_Timeout           =	-4, 		/*!< Program or Erase timeout */
  SD_Not_Ready_for_Data =	-5, 		/*!< SD or MMC card is not ready for data transaction */
  Write_Data_Err 	      =	-6, 		/*!< The data are not correctly written into the SD or MMC card */
  Erase_Addr_Err 	      =	-7, 		/*!< The starting address is not the multiple of the minimum erasable blocks */
  Erase_Block_No_Err 	  =	-8, 		/*!< The amounts of erased sector is not the multiple of the minimum erasable blocks */
  Write_To_Protect_ERR  =	-9, 		/*!< Write to write-protected area */
  Unusable_Card	        =	-10, 	/*!< The SD or MMC card is not usable.  */
  No_Card_Inserted	    =	-11,   /*! No SD/MMC insert */
  Unknown_Err           = -12      	/*!< Unknown Error */
} SDTL_ERR;

extern SDTL_ERR sdtl_allocate(unsigned long  resv_ksize, char *stamp);

extern SDTL_ERR sdtl_mount(void);
 							
extern unsigned long sdtl_resv_size(void);

extern unsigned long sdtl_get_size(void);

extern SDTL_ERR sdtl_resv_read(unsigned long sector, __X void *buf);
							
extern SDTL_ERR sdtl_resv_write(unsigned long sector, __X void *buf);

extern SDTL_ERR sdtl_read(unsigned long sector, __X void *buf);

extern SDTL_ERR sdtl_write(unsigned long sector, __X void *buf);

extern int sdtl_flush(void);

extern void sdtl_prepare(unsigned long num);

extern void sdtl_cache_init(void);

extern int sdtl_cache_read(unsigned long sector, __X void *buf);

extern int sdtl_cache_flush(void); 

extern int sdtl_cache_write(unsigned long sector, __X void *buf);

#endif /* _SDtl_H_ */


typedef void (*PFWAKEUP)(void);

extern void EntrySleepMode(PFWAKEUP pfwakeup);

extern void EntryStandbyMode(PFWAKEUP pfwakeup);

extern void EntrySysMode(unsigned int modcon, PFWAKEUP pfwakeup);


/*! \mainpage USB Mass Storage API

 \section intro Introduction
 USB Mass Storage for KBMPT03\n
 -       version 1.0\n\n
  \section us_sect Using Library
    User need to prepare extra memory for library usage.\n
    If user want USB to show 2 device, user may declare as:\n\n
    -  _STORAGE usb_stor[2];
*/
#ifndef __USB_STOR_API_H__
#define __USB_STOR_API_H__

//#include "usb_def.h"
//#include "usb_stor_def.h"
void* nearcpy(__X void* d, __X void* s, unsigned int l);
// USB Storage Structure, include function pointer and some variables
typedef struct __STORAGE {
	int         (*stor_status)(void);
	unsigned long    (*stor_size)(void) ;
    int         (*stor_read)(unsigned long sector, __X void *buf);	// Storage Read
    int         (*stor_write)(unsigned long sector, __X void *buf);	// Storage Write
    int         (*stor_check_wp)(void);					        // Check Write Protect
    int         (*stor_flush)(void);							// Flush Cache of Write
    unsigned int     device_type;
    char        *device_vendor;
    char        *device_identify;
    char        *device_revision;
} _STORAGE;


int  usb_empty_func2(void);
/** \example usb_main.c
 * This is an example of how to use the Test class.
 * More details about this example.
 */

//! \defgroup APIGroup API Functions

//! @{	\ingroup APIGroup
/*!	USB debounce delay value\n
*/
void USBSetDebounceCount(unsigned long value);
/*!	USB detect attach or not function\n
 @return
    1, attached\n
    0, not\n
*/
int USBAttachCheck(void);
/*!
*/
void USBStoragePoweroff(void);
/*!	USB detect attach or not function\n
 @return
    1, not attach\n
    0, attached\n
*/
int USBRemoveCheck(void);
/*!	USB un-initialize function, close usb interrupt\n
*/
void USBStorageUninit(void);
/*!	USB initialize function, need be called before any function\n
 @param  maxlun             --> max device will be seen in USB storage.
 @param  intrrupt_priority  --> USB interrupt priority, 7 is highest.
*/
void USBStorageInit( unsigned int maxlun, int intrrupt_priority );
//! @}
//! @{	\ingroup APIGroup
/*!	Setup the device on USB storage\n
 @param  lun            --> which device order.
 @param  stor_read      --> read function pointer.
 @param  stor_write     --> write function pointer.
 @param  stor_check_wp  --> check write-propection function pointer.
 @param  stor_flush     --> flush device statue function pointer.
 @param  stor_size_sectors  --> device size in sectors.
 @param  device_type    --> device type, see \ref dev_type; e.g. (FIXED|CDROM)
 @param  decive_vendor  --> vendor string, 8 bytes; e.g. "KB345678"
 @param  decive_identify--> production identify string, 16 bytes. e.g. "KB34567890123456"
 @param  decive_revision--> revision string, 4 bytes; e.g. "1.01"
 @return
    1, parameter error\n
    0, success\n
*/
int USBStorageRegister( int lun,
			int             (*stor_read)(unsigned long sector, __X void *buf),
			int             (*stor_write)(unsigned long sector, __X void *buf),
			int             (*stor_check_wp)(void),
			int             (*stor_flush)(void),
			unsigned long        (*stor_size_sectors)(void),
			int             (*stor_status)(void),
			unsigned int         device_type,
			char            *device_vendor,
			char            *device_identify,
			char            *device_revision );
//! @}
//! @{	\ingroup APIGroup


void USBHIDInit(void);

void USBCoreInit(void);

void USBAudioInit(void);


/*!	USB loop function, need be called in USB while loop\n
    @return
        USB_WAIT,       wait for connected.
        USB_ATTACHED,   usb attached or busy.
        USB_DISCONECTED, usb disconnected, user can removed.
*/
int USBCoreLoop(void);

//! @}
//! @{	\ingroup APIGroup
/*!	Setup the function pointer when device is under specified case\n
    @param type --> which type would be registered\ref usb_func_group
    @param func --> function pointer
*/
void USBFunctionRegister( int type, void (*func)(void) );
//! @}

//! \defgroup dev_type Device type
//! \defgroup status_group Device status
//! \defgroup usb_func_group USB function register type

//! @{	\ingroup dev_type
enum
{
    DIRECT_ACCESS       =0x000,  /*!< direct-access device */
    CDROM               =0x500   /*!< CD-ROM device, not supported yet*/
};
enum
{
    REMOVABLE           =0x80, /*!< medium is removable */
    FIXED               =00    /*!< not removable */
};
//! @}

//! \defgroup stage_type USB device stage
//! @{	\ingroup stage_type
enum
{
    USB_UPLOAD   = 0x10,  /*!< device is upload data to PC side */
    USB_DOWNLOAD = 0x20   /*!< device is download data from PC side */
};
//! @}
// for usb storage status

//! @{ \ingroup status_group
#define USB_STORAGE_NOT_READY 	0  /*!< device not ready */
#define USB_STORAGE_READY 		1  /*!< device ready */
//! @}

//! @{ \ingroup usb_func_group
#define USB_ATTACH_ISR   0      /*!< function when attached */
#define USB_RESUME_ISR   1      /*!< function when usb resume command */
#define USB_SUSPEND_ISR  2      /*!< function when suspend command */
//! @}


#endif



//**************SDK load Code**************************************************
//! \defgroup SDKLoadCodeGroup SDK load code function 
//! @{	

/*!
SDKLoadCode所要载入的 image 之宣告
*/
#define ImageDeclareType(ImageName) extern __X unsigned long ImageName##_begin; \
extern __X int * ImageName##_anchor; 		\
extern __X unsigned long ImageName##_size;	\
extern __X unsigned long ImageName##_CInitFun

/*!
SDKLoadCode，将VIRTUAL_SPACE内的image，贴到IRAM要执行的位置\n
例如，某example的lcf内容部份截取如下：\n
	IRAM 0x10000 0x12C00 \n
	{\n
		SPACE_IRAM 0x0 0x6C00 : interrupt 0x0 0x100 ,image;\n
		INIT_CODE 0x6C00 0xC000 : image ,adhereto VIRTUAL_SPACE;\n
		PLAY_MP3_CODE 0x6C00 0xC000 : image ,adhereto VIRTUAL_SPACE;\n
	}\n
	EXTMEM3 0x500000 0x100000 : ws 3\n
	{\n
		VIRTUAL_SPACE 0x0 0x100000 : image;\n
	}\n
	其中INIT_CODE与PLAY_MP3_CODE就是ImageName，彼此是overlap，由SDKLoadCode(INIT_CODE);\n
	先将initiation的code载到IRAM的0x6C00，执行完後，再执行SDKLoadCode(PLAY_MP3_CODE);\n
	将PLAY_MP3_CODE载到同一个位置，执行mp3 play，这就是SDKLoadCode()的作法来使用IRAM可以overlap code的特性。\n
	

@param ImageName : addhereto的image name ，所要载入的image，载入的位置由linker control file来编辑
@note  1、SPACE_IRAM的起始位置不能改变，必　固定在0。
@note  2、程式的入口，也就是main()也必　摆在SPACE_IRAM。
@note  3、此fucntion会呼叫ToDoCheckNandFlash()，用来处理只剩下一组正常的code时，交由AP来处理 
*/									
#define SDKLoadCode(ImageName) 		LoadCode(&##ImageName##_begin , &##ImageName##_CInitFun)


	


/*!
SDKLoadCode的执行的状态
*/
typedef enum{
	SDKLOADCODE_NO_ERR		=	   	0,  /*!< no error */
	SDKLOADCODE_PARAM_ERR   =	   	-1, /*!< parameter is an invalid */
	SDKLOADCODE_DATA_ERR    =   	-5 	/*!< the data maybe incorrect */
}tSDKLoadCodeStatus;


/*!
SDKLoadCode的初始化
@return : 请参考 tSDKLoadCodeStatus
*/
extern int SDKLoadCodInit(void);		

/*!
SDKLoadCode所使用的function
*/


tSDKLoadCodeStatus LoadCode(	__X unsigned long * ImageName_begin, __X unsigned long * pInitFun);


/*!
读取Dogstar NAND-flash Builder 内有关 Data 绝对位置的资讯
@param DataID : 此Data的编号
@param DataStartSector : 1、NAND-FLASH BOOT的body说明：此function读出data存放在 NAND-FLASH 保留区内的起始sector资讯，并放置於此pointer所指到的位置\n
						2、非NAND-FLASH BOOT的body说明：读到的值为DATA存放的绝对位置。
@param DataLength :  此function读出data的长度资讯，并放置於此pointer所指到的位置，以bytes为单位。
*/
extern void GetDataIdInfo(	unsigned int DataID, __X unsigned long * DataStartSector, __X unsigned long * DataLength);



#ifndef KBDM9_SPIF
#define	KBDM9_SPIF
/*! \mainpage KBDMo SPI Flash API Function Library

    \section intro_sec Introduction
    This library provides the functions to access the MX25L8005 1M-byte serial flash. \n
    The SPI interface of KBDM9 is used as the control interface.  
*/

//!    \defgroup ApiGroup SPI Flash API Functions
//! @b Include: SPI_Flash.h 

//! @{	\ingroup ApiGroup


typedef enum {
  SPIF_No_Err           =  0,		/*!< No Error */
  Do_Not_Support        =	-1,		/*!< The SPI Flash is not supported */
  Parameter_Error       =	-2,		/*!< Command Response CRC Error */
  SPIF_No_Space         = -3,    /*!< SPI Falsh do not have enough space */
  Over_Resv_Size        = -4,
  Over_FAT_Size         = -5
} SPIF_ERR;




/*!	Initial the serial flash \n
		This function initializes registers to attach the serial flash.\n
		
 @return -> please refer to the definition of SPIF_ERR.
 @note
- spif_mount() must be called prior to calling any other function.
*/
extern SPIF_ERR spif_mount(void);

/*!	Read data byte by byte\n\n
	It reads 'num' of bytes from specific address in the serial flash. \n
 @param  byte_addr --> the beginning byte address to be read.
 @param  buf    --> the data buffer used to store the data.
 @param  num    --> the amount of byte to be read.
 @return 0 --> correct \n
        -1 --> the address is over the maximum range.
 @note
- The byte address ranges from 0x000000 to 0x0FFFFF.     
*/
extern int SPIF_Read_Byte(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);

/*!	Read data from the specific sector \n\n
	It reads one sector of data from specific sector in the serial flash. \n
	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> please refer to the definition of SPIF_ERR.
*/
extern SPIF_ERR spif_read(unsigned long sector, __X void *buf);



/*!	Write data to specific sector \n\n
	It writes one sector sector of data to specific sector in the serial flash. \n
	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> please refer to the definition of SPIF_ERR.
 
*/
extern SPIF_ERR spif_write(unsigned long sector, __X void *buf);

/*!	Read data from the reserved area.   \n\n
	It reads one sector of data from specific sector in the reserved area. \n
	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> please refer to the definition of SPIF_ERR.
*/
extern SPIF_ERR spif_resv_read(unsigned long sector, __X void *buf);


/*!	Write data to reserved area. \n\n
	It writes one sector sector of data to specific sector in the reserved area. \n
	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> please refer to the definition of SPIF_ERR.
 
*/

extern SPIF_ERR spif_resv_write(unsigned long sector, __X void *buf);

/*!	Write data page by page \n\n
	It writes 'num' page of data to specific page in the serial flash. \n
	Each page of the serial flash contains 256 bytes.\n
 @param  page --> the beginning page number to be written.
 @param  buf  --> the data buffer which contains the data.
 @param  num  --> the amount of pages to be written.
 @return 0 --> correct \n
         -1 --> the page number is over the maximum page.
*/
extern int SPIF_Page_Program(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);

/*!	It returns the sector size of total usable space.\n\n
 @return     size in sector, each sector contains 512 bytes.
*/
extern unsigned long spif_get_size(void);

/*!	It returns the sector size of total reserved space.\n\n
 @return     size in sector, each sector contains 512 bytes.
*/
extern unsigned long spif_resv_size(void);

/*!	Set the chip to deep power-down mode to minimize the power consumption. \n
 @return  no return value.
*/
extern void SPIF_Power_Down(void);

/*!	Release the chip from deep power-down mode. \n
 @return  no return value.
*/
extern void SPIF_Wake_Up(void);

/*!	Erase one of 4K-byte sector in the serial flash. \n
 @param  byte_addr --> the byte address to be erased, the address must be in the 4k-byte boundary.

 @return 0 --> correct \n
         others --> the address is not on 4k-byte boundary.
*/ 
extern int SPIF_Sector_Erase(unsigned long byte_addr);


/*!	Erase the whole chip. All the data byte become 0xFF after eraseing. \n
 @return  no return value.
*/
extern void SPIF_Chip_Erase(void);

extern int SPIF_Init(void);
extern int spif_flush(void);
extern void spif_prepare(unsigned long num);

//! @}	\ingroup ApiGroup

#endif  /*KBDM9_SPIF */


#ifndef KBDM9_DAC
#define	KBDM9_DAC
/*!@{\ingroup KBMP3_AudioInOutGroup

	\section intro Introduction
 	PCM driver Interface\n
 	-       version 0.2\n\n
	1、此PCM Library使用 KBDM9 3个Channel Mixer的硬体功能，并结合了倍频filter处理与背景播放的流程	。\n
	2、可籍由是否呼叫PCM_IRQFunctionPlugIn()，PCM_DTU_FilterFunctionPlugIn()，PCM_IRQ_FilterFunctionPlugIn()来决定此PCM driver所应具备的功能
	进一步达到所　code size与ram size的弹性，当只　最基本的应用时，可不　呼叫此3个function达到最省code与ram的作法。\n
	3、为了提供OS环境下，对ISR的完全控制，PCM提供了OS mode的设定，让此版本的PCM可以符合OS的应用\n
	4、Sample rate小於32Khz的情况会自动作倍频与filter处理，以达到高音质。

   \defgroup PCMGroup PCM Introduction

@}*/

//!\defgroup PCMQueryGroup PCM Query Category Lists
//!@{\ingroup PCMGroup
/*!
PCM driver function的回传值\n
使用PCM driver发生错误时，查询用。
*/
typedef enum{
	DAC_ERR						=	-1,				/**<PCM function发生错误的回传值 */
	DAC_NO_ERR					=	0,				/**< PCM function没有错误的回传值 */
	DAC_ERR_NO_SET_SAMPLE_RATE	=	1,				/**< PCM启动但未设定Sample rate的错误回传值*/
	DAC_ERR_NO_SET_FRAME_SIZE	=	2,				/**< PCM启动但未设定Frame size的错误回传值*/
	DAC_ERR_NO_CHANNEL_ACTIVE	=	3,				/**<没有Acquire任何Channel 却要执行pcm播放*/
	DAC_ERR_NO_CHANNEL_FREE		=	4,				/**<没有可再Acquire的Channel*/
	DAC_ERR_SETUP_IRQ			=	5,				/**<Acquire IRQ source时出现错误*/
	DAC_ERR_DTU_SETUP_ERR		=	6,				/**<Acquire DTU resource时出现错误*/
	DAC_ERR_SAMPLE_RATE_ERR		=	7,				/**<sample rate的设定大於50K或小於6K，或以DHA当IRQ Source却使用不支援的Sample rate*/
	DAC_ERR_FRAME_SIZE_ERR		=	8,				/**<Frame size设定超过0x1fff的大小*/
	DAC_ERR_CHANNEL_RUNNING		=	9,				/**<Release此Channel时，发觉此Channel还在执行中，无法Release*/	
	DAC_ERR_CHANNEL_NO_ACTIVE	=	10,				/**<此Channel非可以下命令的状态*/
	DAC_ERR_NO_INIT				=	11,				/**<尚未初始化，　先执行过PCM_Acquire即可解决此错误*/
	DAC_ERR_HANDLE_ERR			=	12,				/**<此handle非PCM driver的Handle*/
	DAC_ERR_PLL_SETUP_ERR		=	13,				/**<PLL没打开，或不符合所要播放的频率　求，无法以正常频率播放*/
	DAC_ERR_PARAMETER			=	14,				/**<参数设定错误*/
	DAC_ERR_SOCR_BIT15_DISABLE	=	15				/**<SOCR.15未打开=>Interrupt的总开关未打开*/
}tDAC_Error;

/*!
KBDM9目前提供的DTU

*/
typedef enum{
  DTU0			=	0,		/**<使用 DTU0时，请使用此参数，根据KBDM9之硬体规格，此priority 为0(最低)*/
	DTU1			=	1,		/**<使用 DTU1时，请使用此参数，根据KBDM9之硬体规格，此priority 为1*/
	DTU2			=	2,		/**<使用 DTU2时，请使用此参数，根据KBDM9之硬体规格，此priority 为2*/
	DTU3			=	3,		/**<使用 DTU3时，请使用此参数，根据KBDM9之硬体规格，此priority 为3*/
}tKBDM9_DTUNumber;
//!@}


/*!
执行PCM_ChannelSetup之PCM_CH_SETUP_SAMPLE_RATE时的ChannelSetupValue\n
其它值之设定为sample rate的直接值，范围为6000~50000，若tPCM_IRQSource选择DHAL或DHAR，则该Channel之Sample rate设定只能是tPCM_SampleRate内的列举值
@sa  PCM_ChannelSetup tPCM_ChannelSetupCategory
*/
typedef enum{
	DAC_SAMPLE_RATE_96K			=	0,				/**< DAC sample rate设定为96KHz*/	
	DAC_SAMPLE_RATE_48K			=	1,				/**< DAC sample rate设定为48KHz*/
	DAC_SAMPLE_RATE_44P1K		=	2,				/**< DAC sample rate设定为44.1KHz*/
	DAC_SAMPLE_RATE_32K			=	3,				/**< DAC sample rate设定为32KHz*/
	DAC_SAMPLE_RATE_24K			=	4,				/**< DAC sample rate设定为24KHz*/
	DAC_SAMPLE_RATE_22P05K	=	5,				/**< DAC sample rate设定为22.05KHz*/
	DAC_SAMPLE_RATE_16K			=	6,				/**< DAC sample rate设定为16KHz*/
	DAC_SAMPLE_RATE_12K			=	7,				/**< DAC sample rate设定为12KHz*/
	DAC_SAMPLE_RATE_11P025K	=	8,				/**< DAC sample rate设定为11.025KHz*/
	DAC_SAMPLE_RATE_9P6K  	=	9,				/**< DAC sample rate设定为9.6KHz*/	
	DAC_SAMPLE_RATE_8K			=	10				/**< DAC sample rate设定为8KHz*/
}tDAC_SampleRate;

//!\defgroup PCMv2FunctionGroup PCMv2 driver function
//! @{  
extern __X int  * DAC_LChannelBuffer1;				/**<左声道Buffer1的起始位址，Size必　大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型态转换即可 */
extern __X int  * DAC_RChannelBuffer1;				/**<右声道Buffer1的起始位址，Size必　大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型态转换即可 ，若所播放的音源为Mono，可与LChannelBuffer1相同*/
extern __X int  * DAC_LChannelBuffer2;				/**<左声道Buffer2的起始位址，Size必　大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型态转换即可 */
extern __X int  * DAC_RChannelBuffer2;				/**<右声道Buffer2的起始位址，Size必　大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型态转换即可 ，若所播放的音源为Mono，可与LChannelBuffer2相同*/
extern unsigned int DAC_FrameSize;					/**<存放该Channel的Frame Size，以Word为单位*/
extern tDAC_SampleRate DAC_SampleRate;				/**<存放该Channel的Sample Rate设定*/

/*!
DAC 的初始化

@param LChannelDTUNumber 左channel所使用的DTU
@param RChannelDTUNumber 右channel所使用的DTU
@param FillBufferFunc1 PCM buffer若空了, 则会进入此function pointer所指到的function内
@param FillBufferFunc2 PCM buffer若空了且FillBufferFunc1所指到的function已经执行完毕，则会再进入此function pointer所指到的function内
@return tPCM_Error，请参考KBDM9_DDK.chm内有关DAC error的说明
*/
extern tDAC_Error DAC_Init(
								tKBDM9_DTUNumber LChannelDTUNumber,	
								tKBDM9_DTUNumber RChannelDTUNumber,	
								void(*FillBufferFunc1)(void),
								void(*FillBufferFunc2)(void));

/*!
使用耳机输出声音

*/
extern void DAC_Enable_HeadPhone(void);

/*!
关闭耳机输出

*/
extern void DAC_Disable_HeadPhone(void);

/*!
使用喇叭输出声音

*/
extern void DAC_Enable_Speaker(void);

/*!
关闭喇叭输出

*/
extern void DAC_Disable_Speaker(void);
								
/*!
DAC 音量调整 \n
总共有32段音量可调整

@param Volume  必须为0 ~ 31的数值, 0为最小声, 31为最大声 

*/								
extern void DAC_Volume(unsigned int Volume);								

/*!
检查耳机是否插入耳机接孔

@return 0，没有插耳机
@return 1，有插耳机

*/
unsigned int DAC_Query_HP_Plug(void);	


/*!
执行此function开始播放
@return tPCM_Error，请参考KBDM9_DDK.chm内有关PCM error的说明
*/
extern tDAC_Error DAC_Go(void);
								
/*!
判断buffer1是否空了，通常在FillBufferFunc1()内被执行
@return 0，Buffer1还没空
@return 1，Buffer1已经空了，必　填入要播放的资料
*/
extern int DAC_Buffer1Empty(void);  	

/*!
判断buffer2是否空了，通常在FillBufferFunc1()内被执行
@return 0，Buffer2还没空
@return 1，Buffer2已经空了，必　填入要播放的资料
*/
extern int DAC_Buffer2Empty(void);	

/*!
填完Buffer1的资料後，必　执行此function，通常在FillBufferFunc1()内被执行
*/
extern void DAC_FillBuffer1(void); 	

/*!
填完Buffer2的资料後，必　执行此function，通常在FillBufferFunc1()内被执行
*/
extern void DAC_FillBuffer2(void); 	

/*!
暂停PCM的播放
*/
extern void DAC_Pause(void);

/*!
解除 PCM 播放暂停的状态，继续播放
*/
extern void DAC_Continue(void);

/*!
PCM播放静音
*/
extern void DAC_Mute(void);

/*!
解除PCM静音状态
*/
extern void DAC_Unmute(void);

/*!
播放结束时，执行此function可将输出准位拉为0，*/
extern void DAC_FadeOut(void);	

/*!

*/
extern void DAC_FadeIn(void);	

#endif  /*KBDM9_DAC */

/*!@{\ingroup KBMP3_AudioInOutGroup

	\section intro Introduction
 	
 	Auadc Driver Interface\n
 	-       version 0.1\n\n
	1、Microphone 输入 \n
	2、支援 8, 9.6, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48, 及 96 KHz 16bit 取样。

   \defgroup KBMP3_AudioInGroup KBDM9 Audio In Introduction

@}*/



#ifndef KBDM9_Audio_ADC_H
#define KBDM9_Audio_ADC_H


//!\defgroup AudioInGroup record interface
//!@{ \ingroup KBMP3_AudioInGroup

/*!
Audio ADC Device 错误讯息
*/
typedef enum {
	AUADC_UNKNOWER_ERR			=	-1,		/**<Audio ADC Device function发生未知的错误*/
	AUADC_NO_ERR				=	0,		/**<Audio ADC Device function没有错误的回传值*/
	AUADC_SampleRate_SETUP_ERR		=	1,		/**<Audio ADC Device setting IRQ resource 错误*/
	AUADC_DTU_SETUP_ERR		=	2,		/**<Audio ADC Device setting DTU resource 错误*/
	AUADC_OUT_BUF_SETUP_ERR		=	3,		/**<Audio ADC Device setting output buffer 错误*/
	AUADC_BUF_SIZE_SETUP_ERR	=	4,		/**<Audio ADC Device setting buffer size 错误*/
	AUADC_CALL_BACK_SETUP_ERR	=	5,		/**<Audio ADC Device 不正确的 call back function错误*/
	AUADC_PARAMETER_SETUP_ERR	=	6,		/**<Audio ADC Device 参数设定错误*/
	AUADC_NO_INIT_ERR			=	7,		/**<Audio ADC Device 尚未初始化错误*/
	AUADC_NO_OUT_SETUP_ERR		=	8,		/**<Audio ADC Device 尚未 setting output 错误*/
	AUADC_DATA_OVERLAP_ERR		=	9		/**<Audio ADC Device 资料发生覆盖现象错误*/
} tAUADC_Err;


/**** Initial code *****/

/*!
信号撷取采样频率ID*/
typedef enum {
	AUADC_SAMPLE_RATE_96K			=	0,				/**< DAC sample rate设定为96KHz*/	
	AUADC_SAMPLE_RATE_48K			=	1,				/**< DAC sample rate设定为48KHz*/
	AUADC_SAMPLE_RATE_44P1K		=	2,				/**< DAC sample rate设定为44.1KHz*/
	AUADC_SAMPLE_RATE_32K			=	3,				/**< DAC sample rate设定为32KHz*/
	AUADC_SAMPLE_RATE_24K			=	4,				/**< DAC sample rate设定为24KHz*/
	AUADC_SAMPLE_RATE_22P05K	=	5,				/**< DAC sample rate设定为22.05KHz*/
	AUADC_SAMPLE_RATE_16K			=	6,				/**< DAC sample rate设定为16KHz*/
	AUADC_SAMPLE_RATE_12K			=	7,				/**< DAC sample rate设定为12KHz*/
	AUADC_SAMPLE_RATE_11P025K	=	8,				/**< DAC sample rate设定为11.025KHz*/
	AUADC_SAMPLE_RATE_9P6K  	=	9,				/**< DAC sample rate设定为9.6KHz*/	
	AUADC_SAMPLE_RATE_8K			=	10				/**< DAC sample rate设定为8KHz*/
} tAUADC_SmapleRate;

/* Audio ADC Input Gain */
typedef enum{
  AUADC_Gain_0dB     = 0,           /**< Audio ADC Input Gain 设定为 0dB    */	          
  AUADC_Gain_1P5dB   = 1,           /**< Audio ADC Input Gain 设定为 1.5dB  */    
  AUADC_Gain_3dB     = 2, 	        /**< Audio ADC Input Gain 设定为 3db    */  
  AUADC_Gain_4P5dB   = 3,           /**< Audio ADC Input Gain 设定为 4.5dB  */
  AUADC_Gain_6dB     = 4,           /**< Audio ADC Input Gain 设定为 6dB    */
  AUADC_Gain_7P5dB   = 5,           /**< Audio ADC Input Gain 设定为 7.5dB  */
  AUADC_Gain_9dB     = 6,           /**< Audio ADC Input Gain 设定为 9dB    */
  AUADC_Gain_10P5dB  = 7,           /**< Audio ADC Input Gain 设定为 10.5dB */
  AUADC_Gain_12dB    = 8, 	        /**< Audio ADC Input Gain 设定为 12dB   */
  AUADC_Gain_13P5dB  = 9,           /**< Audio ADC Input Gain 设定为 13.5dB */
  AUADC_Gain_15dB    = 10,          /**< Audio ADC Input Gain 设定为 15dB   */
  AUADC_Gain_16P5dB  = 11,          /**< Audio ADC Input Gain 设定为 16.5dB */
  AUADC_Gain_18dB    = 12, 	        /**< Audio ADC Input Gain 设定为 18dB   */
  AUADC_Gain_19P5dB  = 13,          /**< Audio ADC Input Gain 设定为 19.5dB */
  AUADC_Gain_21dB    = 14,          /**< Audio ADC Input Gain 设定为 21dB   */
  AUADC_Gain_22P5dB  = 15    	      /**< Audio ADC Input Gain 设定为 22.5dB */      
}tAUADC_Input_Gain;

/* Audio ADC Microphone Gain */
typedef enum{
  MIC_Gain_0dB       = 0,          /**< Audio ADC Microphone Gain 设定为 0dB    */	 
  MIC_Gain_20dB      = 1           /**< Audio ADC Microphone Gain 设定为 20dB  */ 
}tAUADC_MIC_Gain;

/* Audio ADC Input Path */

typedef enum{
  MIC_In       = 0,          /**< 选择 Microphone  */	 
  Line_In      = 1           /**< 选择 Line In  */ 
}tAUADC_In_Path;
/*!
Audio ADC Device完成填满buffer的call-back function
@param buf	=> 存放Audio ADC转换的资料
@param buf_size	=>Buffer的大小，以Word为单位。 
*/
typedef void (*tAUADC_BufNotify)(__X int* buf, unsigned int buf_size);

/*!
Audio ADC Device初始化 function
@param Input_Type  => 选择Microphone 或 Line In 输入
@param DTUNumber	 =>	设定的DTU资源
@param SampleRate  => Audio ADC 的 sampling rate
@param notify		   =>	完成填满buffer的call-back function
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE_INIT
*/
tAUADC_Err AUADC_Init(tAUADC_In_Path Input_Type, tKBDM9_DTUNumber DTUNumber,	tAUADC_SmapleRate SampleRate, tAUADC_BufNotify notify);	

/**** Setup code *****/


/*!
输出buffer的设定, 须有两个buffer轮流接收ADC转换出来的资料
@param	OutBuf1		=>	first output buffer pointer
@param	OutBuf2		=>	second output buffer pointer
@param	OutBufSize		=>	output buffer size, unit is words
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE_INIT
*/
tAUADC_Err AUADC_Set_Buffer(__X int *OutBuf1, __X int	*OutBuf2, unsigned int OutBufSize);						



/*! 
设定 Audio ADC Input Gain 
@param gain 	=>	0~15,  一个单位为1.5dB, 所以可调整范围为 0dB 至 22.5dB
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE_TEMP
*/
tAUADC_Err AUADC_InputGain(tAUADC_Input_Gain gain);


/*!
设定 Audio ADC Microphone Gain 
@param gain		=>	0: 0dB 
              =>  1: 20dB
                
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE_TEMP
*/
tAUADC_Err AUADC_MicGain(tAUADC_MIC_Gain gain);



/**** Running code *****/
/*!
Audio ADC 开始工作 function
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE
*/
tAUADC_Err AUADC_Start(void);


/*!
Audio ADC 停止工作 function
@note \b Code \b Address \b Name => \b AUADC_DRV_CODE
*/
tAUADC_Err AUADC_Stop(void);


#endif      /* KBDM9_Audio_ADC_H */
//!@}


#ifndef KBDM9_MP3_Encode_H
#define KBDM9_MP3_Encode_H

/*! \mainpage MP3 Encoder Interface API Function

	\section intro Introduction
	MP3 Encoder Interface API Function\n
	- MP3 encoder library provides users to compress music sources which can be standard wave 
		file (*.wav) or 16-bit PCM format efficiently to normal MP3 bit stream for applying.\n 
	  There are two essential functions in this MP3 IP. The names of them as following: \n

		1. MP3Enc_Init: The function contains initializes of table and header information.\n
		2. Mp3Enc_compress: The function contains polyphase analysis filter, quantization and huffman coding.\n\n

	Note: MPEG-2.5 is not official standard. It is an extension of the standard used for lower bit rate. \n

*/

//!\defgroup MP3VarListGroup MP3 Global Variables List
//!@{\ingroup MP3EncderGroup

/*! WAVE File Status */
typedef enum {
	WAV_NoErr 	= 0,	/**< correct wave file\n*/
	WAV_Err_Samplerate = -1,/**< not available sampling rate \n*/
	WAV_Err_channel = -2 /**< only support mono \n*/
}tWaveErr;


typedef enum {
  WAV_File_In = 0,    /**< The input is a wave file \n */
  RAW_PCM_In = 1 	    /**< The input is PCM raw data \n */
}tPCM_Type;

typedef enum {
  BR_8Kbps  = 8,    /**< 8K bit-per-second \n */
  BR_16Kbps = 16,    /**< 8K bit-per-second \n */    
  BR_24Kbps = 24,    /**< 8K bit-per-second \n */
  BR_32Kbps = 32,    /**< 8K bit-per-second \n */  
  BR_40Kbps = 40,    /**< 8K bit-per-second \n */   
  BR_48Kbps = 48,    /**< 8K bit-per-second \n */   
  BR_56Kbps = 56,    /**< 8K bit-per-second \n */   
  BR_64Kbps = 64,    /**< 8K bit-per-second \n */   
  BR_80Kbps = 80,    /**< 8K bit-per-second \n */   
  BR_96Kbps = 96,    /**< 8K bit-per-second \n */   
  BR_112Kbps = 112,    /**< 8K bit-per-second \n */   
  BR_128Kbps = 128,    /**< 8K bit-per-second \n */   
  BR_144Kbps = 144,    /**< 8K bit-per-second \n */   
  BR_160Kbps = 160,    /**< 8K bit-per-second \n */   
  BR_192Kbps = 192,    /**< 8K bit-per-second \n */     
  BR_224Kbps = 224,    /**< 8K bit-per-second \n */   
  BR_256Kbps = 256,    /**< 8K bit-per-second \n */   
  BR_320Kbps = 320    /**< 8K bit-per-second \n */   
}tBitRate;

typedef enum {
	MP3_SAMPLE_RATE_8KHZ = 8000,
	MP3_SAMPLE_RATE_11P025KHZ = 11025,
	MP3_SAMPLE_RATE_12KHZ = 12000,		
	MP3_SAMPLE_RATE_16KHZ = 16000,	
	MP3_SAMPLE_RATE_22P05KHZ = 22050,	
	MP3_SAMPLE_RATE_24KHZ = 24000,	
	MP3_SAMPLE_RATE_32KHZ = 32000,	
	MP3_SAMPLE_RATE_44P1KHZ = 44100,	
	MP3_SAMPLE_RATE_48KHZ = 48000	
}tMP3_SampleRate

/* struct */
typedef struct {
    tPCM_Type	pcm_type; /**< 0: The input is a wave file.\n 1:The input is a source 16-bit PCM data. \n */
    tBitRate 	bit_rate; /**< MPEG-1 			: 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 \n
    										 MPEG-2/2.5 	: 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 \n  */
    tMP3_SampleRate 	sample_rate; /**< MPEG-1 		: 44100, 48000, 32000 \n 
    																MPEG-2 		: 22050, 24000, 16000 \n
																		MPEG_2.5 	: 11025, 12000, 8000  \n */
} MP3_Enc_Config;

//! \defgroup MP3EncFunctionGroup MP3 Encoder Functions

//! @{	\ingroup MP3EncFunctionGroup

/*!
The function used to initialize for input is wave file. If music source is from wave file, this function will check its sampling rate.\n

@param InBuffer => wave file bitstream pointer.\n
@param File_size =>	wave file size. \n
@return  1 =>The input file is correct wave file.\n
@return 0 =>Input file not available sampling rate.\n
@return -1 => Input file only support mono.\n
*/
extern tWaveErr	Mp3Enc_WaveInit(char * InBuffer, long File_size);


/*! 
This is a callback function to write MP3 bitstream to destiation file. This function \n
will be executed when one frame of MP3 data is produced by encoder. 
@param bitstream_buffer => the buffer which contains the MP3 bitstream.\n
@param size =>	Output Bitstream size.\n

*/
typedef 	void 	(*MP3_Output_Data_Func)(unsigned char* bitstream_buffer, int size);




/*!
The function is to initialize all variables, status, and flags used in MP3 encoder default value.
@param ptype =>The struct config_t contain pcm type, bit rate and sample rate. \n
@param notify => A callback function to write bitstream data to destination. \n
*/

extern int MP3Enc_Init(	MP3_Enc_Config ptype, MP3_Output_Data_Func  notify);


/*!
This function is to encode data.\n
*/
extern  void Mp3Enc_compress(void);

/*!
This function is used when users want to change the input buffer of an Mp3 encoding process.\n
*/
extern 	void 	Mp3Enc_setBuffer(__X int *Set_Buf);


#endif      /* KBDM9_MP3_Encode_H */

//! @}

 
#endif  /* _KMDM9_DDK_ */
