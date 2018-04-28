#ifndef _IMA_ADPCM_H_
#define _IMA_ADPCM_H_

/*!@{\ingroup KBDM9_SoftWareIPGroup

	\section intro Introduction
 	KBDM9 IMA-ADPCM Codec API Function Libraries\n
 	 -       version 1.0\n\n
	
	 This IMA-ADPCM Codec provides the functions to convert the mono 16-bit signed PCM to 4-bit IMA-ADPCM data, and vice versa.\n\n

   	\defgroup IMA_ADPCMGroup IMA-ADPCM Codec API Introduction

   	

@}*/

//!    \defgroup ADPCMStructGroup IMA-ADPCM Codec Struct Define
//! @{	\ingroup IMA_ADPCMGroup

/*!
     IMA-ADPCM File Information
*/
typedef struct {
    __X unsigned int	 IMA_SR;								/*!< Sampling rate for playing the decoded PCM samples. */ 
    __X unsigned int	 IMA_ADPCM_BufSize;	  	/*!< Frame size of ADPCM data. Its unit is word.     		
    																				 	 The frame size depends on the sampling rate. 
    																				 	 The relationship between the sampling rate \b Fs and \b IMA_ADPCM_BufSize are listed below. \n\n
    																				 	 If Fs <= \b 16KHz, then IMA_ADPCM_BufSize = \b 128 words. \n\n
    																				 	 If \b 16KHz < Fs <= \b 32KHz, then IMA_ADPCM_BufSize = \b 256 words. \n\n
    																				 	 If Fs > \b 32KHz, then IMA_ADPCM_BufSize = \b 512 words*/  																				 	       																				 	 
    __X unsigned int   IMA_PCM_BufSize;		  	/*!< Frame size of decoded PCM samples. Its unit is word.
    																					 The frame size depends on the sampling rate. 
    																				 	 The relationship between the sampling rate \b Fs and \b IMA_PCM_BufSize are listed below. \n\n 
    																				 	 If Fs <= \b 16KHz, then IMA_PCM_BufSize = \b 505 words. \n\n
     																				 	 If \b 16KHz < Fs <= \b 32KHz, then IMA_PCM_BufSize = \b 1017 words. \n\n
    																				 	 If Fs > \b 32KHz, then IMA_PCM_BufSize = \b 2041 words*/    	
    __X unsigned long  IMA_Frame_No;					/*!< The number of frames to be decoded. */
}tIMA_INFO;
//! @}	

//!    \defgroup EncodeGroup Mono IMA-ADPCM Encoder 
//! @{	\ingroup IMA_ADPCMGroup

/*!	Initialize the IMA-ADPCM Encoder \n\n
		This function initializes the parameters of encoder before the first time to call it.\n\n

 @param  SampleRate  --> The sampling rate of the PCM data to be encoded.		
 @return     no return value
*/
extern void IMA_ADPCM_Encoder_Init(unsigned int SampleRate);

/*!	Encoding one frame of PCM values to IMA-ADPCM data package \n\n
 @param  In_Buffer  --> The buffer that contains the PCM data to be encoded.
 @param  Out_Buffer  --> The buffer that is used to store the IMA-ADPCM data.
                                                
 @return no return value
         
 @note
- The size of In_Buffer is fixed to \b 505 words. That is, one frame contains \b 505 PCM samples. \n
- The size of Out_Buffer is fixed to \b 128 words.  
*/
extern void IMA_ADPCM_Encoder(__X int *In_Buffer, __X int *Out_Buffer);

/*!	The ADPCM_File_Header contains the 60-byte file header of IMA-ADPCM.   
		At first user shall save the file header before saving the IMA-ADPCM data into a file.
		Since the file header contains the information of file size,  user shall also update the file header 
		before closing the file. 
*/
extern char ADPCM_File_Header[];	
//! @}	\ingroup IMA_ADPCMGroup



//!    \defgroup DecodeGroup Mono IMA-ADPCM Decoder  
//! @{	\ingroup IMA_ADPCMGroup

/*!	Initialize the Mono IMA-ADPCM Decoder \n\n
		This function initializes the parameters of decoder before the first time to call it.\n\n
		
 @return     0 =>  no error
*/
extern int IMA_ADPCM_Decoder_Init(__X tIMA_INFO *ptr);

/*!	Decoding one frame of IMA-ADPCM data to 16-bit signed PCM data \n\n
 @param  In_Buffer  --> The buffer that contains the IMA-ADPCM data to be decoded.
 @param  Out_Buffer  --> The buffer that is used to store the PCM data.
                                                
 @return no return value
 @note
- The size of In_Buffer must be larger than or equal to the \b IMA_ADPCM_BufSize given in the IMA-ADPCM File Information. \ref tIMA_INFO 
- The size of Out_Buffer must be larger than or equal to the \b IMA_PCM_BufSize given IMA-ADPCM File Information. \ref tIMA_INFO
*/
extern void IMA_ADPCM_Decoder(__X int *In_Buffer, __X int *Out_Buffer);
//! @}	\ingroup IMA_ADPCMGroup

#endif /* _IMA_ADPCM_H_ */