/*!@{
    \defgroup KBMP3_SoftWareIPGroup KBMP3 Software IP API Functions   

 	KBMP3與週邊無關，純軟體運算之IP介紹，各類的壓縮，解壓縮應用 \n
 	主要有：包含mp3 decode、WMA decode等\n

@}*/


/*!@{\ingroup KBMP3_SoftWareIPGroup

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
\b bit15: free format detection, 要求偵測 nSlots of free format \n
\b bit14: (1)free format mp3 file found and nSlots calculate OK (1)otherwise \n
 bit13: reserved \n
\b bit12: SYNCWORD_FOUND, (1)SYNCWORD已找到, 但該frame之bit stream尚未讀完 (0)需重新找SYNC_WORD \n
\b bit11: preflagCur, preflag of channel which is under processing \n
\b bit10: scDirty, 第一次進入DEQUANTIZATION_4時未必見得會碰到boundary, 因此scVar[]可能不會更新, 靠此訊號通知DEQUANTIZATION_4更新scVar[] \n
\b bit9: ms_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit8:  i_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit7: WORKING_CH, (0)正處理ch0, (1)正處理ch1, valid only in BACK_END()/POST_PROCESSING() \n
\b bit6: bsInitOK, bit stream struct (0)尚未初始化 (1)初始化完畢 \n
\b bit5: NEXT_GR, (0)下一個處理gr0, (1)下一個處理gr1 \n
\b bit4: bAUX1, temporarily bit storage \n
 bit3..0: reserved
*/
extern __DPRAM int  mp3Flag2;

/*!
Control bits to reduce MIPS requirement for decoding \n
\b bit0~5: The value should be 10~32. Set the decode band numbers \n
\b bit15: set this bit 0 to enforce mono decoding to save MIPS, default value: 1 \n
*/
extern __X int  mp3MIPsReduce;

/*!
mp3EQgain[sb0..31]: UNS0.16, 0x8000=0.5 \n
synthesis 時再將音量 double
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
\b bit4: BS_NOT_ENOUGH, _mp3_bs_buf[]中的資料不足一個frame \n
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
extern void mp3_init(	int *pu32MemPool1 ,
						int *pu32MemPool2 ,
						int *pu32MemPool3 ,
						int *pu32MemPool4, 
						int *HEAP, 
						int *bs_buf, 
						int bs_len);
					
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
@param LeftBuf =>mp3 decode出來的左聲道資料所在位置
@param RightBuf =>mp3 decode出來的右聲道資料所在位置
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
執行 mp3dec()前，必需執行此function
*/
extern void mp3Clear(void);

/*!
This function will setup the pointer of the buffer to put the spectrums data form mp3dec()
function. User can get the information to display spectrums from the buffer.
@param spectrum_ptr => spectrums資料所存放的位置，必需指到32個word的空間
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_spectrum_data(unsigned int * spectrum_ptr);

#endif
//!@}

//=============================================================================