/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3PlayTime.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"

INT32U AudioMP3GetPlayTime_(tPlayTimeMode SetMode);
//------------------------------------------------------------------------------
// Function:	MP3_XingSeekPoint
// Input:		TOC = table of contents from Xing header
//				time_position = number of mini-second in play time
// return:		seekpoint in bytes
//------------------------------------------------------------------------------

#pragma CODE = MP3_TEMP_CODE04
//------------------------------------------------------------------------------
static INT32U MP3_XingSeekPoint(unsigned char *TOC, unsigned long time_position)
{
	// interpolate in TOC to get file seek point in bytes
	INT32U seekpoint;
    INT32U percent;
    INT32U temp;
    INT32U get_mp3FrameCount;
	INT16U a;
    INT16U fa;
    INT16U fb;
    INT16U fx;
	
	percent = time_position/(MP3_Information.dwTimeLength/1000L);
	
	a = (INT16U)percent/10;
	fa = TOC[a];
	if (a < 99)
		fb = TOC[a+1];
	else
    	fb = 256;
    a = (INT16U)(percent - a*10);
	fx = fa + ((fb - fa) * a)/10;
	seekpoint = (fx * MP3_Information.dwAudioFileSize>>4)>>4;
	get_mp3FrameCount = (percent * MP3_Information.dwFrameTotal)/1000L;
  MP3_Information.dwFilePosition = MP3_Information.dwHeaderOffset + seekpoint;
  if(MP3_Information.dwFilePosition >= MP3_Information.dwFileSize)
    MP3_Information.dwFilePosition = MP3_Information.dwHeaderOffset +
                                     MP3_Information.dwAudioFileSize -
                                     10*MP3_Information.wFrameSize;
	return get_mp3FrameCount;
}

//------------------------------------------------------------------------------
static INT32U MP3_XingNoTOCSeekPoint(unsigned long time_position)
{
    // HeaderOffset + (Time(ms)*BitRate(Kbps))/8
    MP3_Information.dwFilePosition = MP3_Information.dwHeaderOffset +
        ((unsigned long)time_position*MP3_Information.wBitRate)/8;
    if(MP3_Information.dwFilePosition >= MP3_Information.dwFileSize)
        MP3_Information.dwFilePosition = MP3_Information.dwHeaderOffset +
            MP3_Information.dwAudioFileSize - 10*MP3_Information.wFrameSize;
	return ((time_position*10) / MP3_Information.wFrameDuration);
}

//------------------------------------------------------------------------------
//   Function:	MP3_VbriSeekPoint
//				Returns a point in the file to decode in bytes that is nearest 
//				to a given time in seconds
//   Input:		time in seconds
//   Output:	None
//   Returns:	point belonging to the given time value in bytes
//------------------------------------------------------------------------------
static INT32U MP3_VbriSeekPoint(unsigned char *TOC, unsigned long time_position)
{
	INT16U i = 0;
	INT32U seekpoint = 0;
    INT32U fraction = 0;
    INT32U get_mp3FrameCount;
  	INT32U time_per_toc_frame;
	INT32U accumulated_time = 0;
	
	time_per_toc_frame = (MP3_Information.dwTimeLength) /
                         (VBR_Information.wVbriTOC_Size+1);
	if ( time_position > MP3_Information.dwTimeLength )
		return (MP3_Information.dwAudioFileSize);
	while ( accumulated_time <= time_position)
	{
		seekpoint += ((unsigned int)(TOC[i]<<8) + (unsigned int)TOC[i+1]);
		accumulated_time += time_per_toc_frame;
		i+=2;
	}
	
	// Searched too far; correct result
	fraction = ((((accumulated_time - time_position)*1000) / time_per_toc_frame)
			 + (1000/(2*VBR_Information.wVbriTOC_EntryFrames))) *
             VBR_Information.wVbriTOC_EntryFrames;
	fraction /= 1000;
	seekpoint -= (((unsigned long)((unsigned int)(TOC[i-2]<<8) + 
                 (unsigned int)(TOC[i-1])) * fraction) 
			  / VBR_Information.wVbriTOC_EntryFrames);
	seekpoint *= VBR_Information.wVbriTOC_Scale;
	time_position += VBR_Information.wVbriDelay;
	get_mp3FrameCount = (time_position*10) / MP3_Information.wFrameDuration;
    MP3_Information.dwFilePosition = MP3_Information.dwHeaderOffset + seekpoint;
    if(MP3_Information.dwFilePosition >= MP3_Information.dwFileSize)
    {
        MP3_Information.dwFilePosition = 
            MP3_Information.dwHeaderOffset +
            MP3_Information.dwAudioFileSize - 
            10*MP3_Information.wFrameSize;
    }
	return (get_mp3FrameCount);
}


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Find MP3 frame header and checking
//------------------------------------------------------------------------------

static INT32U FindFrameHeader(__X FCFILE *FS_Handle,__X INT8U  buffer[] )
{
    INT16U CheckFrameCnt;
    INT16U wFrameSize_b;
    INT32U file_remain_size;
    INT32U dwMP3_FrameHeader_b;
    INT32S dwFilePosition_b;
    INT32S frame_offset;
    INT32S file_length;
    FS_ERR fserr;
    unsigned int BackupBitRate;
    
	if(MP3API_FSSeek(MP3_Information.dwFilePosition))
    {
	    return 0;
	}
    dwFilePosition_b = MP3_Information.dwFilePosition;
	file_remain_size = MP3_Information.dwFileSize -
        MP3_Information.dwFilePosition;
	wFrameSize_b = MP3_Information.wFrameSize;
	CheckFrameCnt = 0;
	dwMP3_FrameHeader_b = 0;
	BackupBitRate = MP3_Information.wBitRate;
	while (file_remain_size > 0)
	{
		if (file_remain_size > 512)
		{
			file_length = MP3API_FSRead(buffer,
                                         512,
                                         &fserr);
		}
        else
        {
			file_length = MP3API_FSRead(buffer,
                                         file_remain_size,
                                         &fserr);
		}
        if (file_length == 0)
			return AUDIO_ERROR_READ_FILE;
		file_remain_size -= file_length;

		dwMP3_FrameHeader_ = 0;
		frame_offset = MP3_GetAudioHeader_1((void*)buffer, file_length);
		if (frame_offset > 0)
		{
		    dwMP3_FrameHeader_b = dwMP3_FrameHeader_ & CHECK_FRAME_DATA_MASK;
			dwFilePosition_b += frame_offset;
		    MP3_Information.dwFilePosition = dwFilePosition_b;
		    dwFilePosition_b += MP3_Information.wFrameSize;
		    MP3API_FSSeek(dwFilePosition_b);
		    if(MP3_Information.dwFileSize > dwFilePosition_b)
		    {
		        file_remain_size =
                    MP3_Information.dwFileSize - dwFilePosition_b;
		    }
            else
            {
		        file_remain_size = 0;
			}
            CheckFrameCnt = 0;
		    continue;
		}   
		else if (frame_offset == 0)
		{
		    if(dwMP3_FrameHeader_b==(dwMP3_FrameHeader_&CHECK_FRAME_DATA_MASK))		        
	        {
		        CheckFrameCnt++;
    		    dwFilePosition_b += MP3_Information.wFrameSize;
		    }
		    else
		    {
		        dwMP3_FrameHeader_b = 0;
		        CheckFrameCnt = 0;
		        dwFilePosition_b = MP3_Information.dwFilePosition + 4;
		    }
        	MP3API_FSSeek(dwFilePosition_b);
        	file_remain_size = MP3_Information.dwFileSize - dwFilePosition_b;
        	if(CheckFrameCnt > 2)
        	{
			    MP3_Information.wFrameSize = wFrameSize_b;
			    break;
			}
			continue;
		}
		dwFilePosition_b += file_length;
	}
	MP3_Information.wBitRate = BackupBitRate;
	return file_remain_size;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern INT16S MP3API_FirstDecode(void);
//------------------------------------------------------------------------------
tAudioErr AudioMP3SetPlayTime_( INT32U time_position)
{
    INT8U buffer[512];
    INT16U SearchMode;
    INT16U offset;
    INT32S file_offset;
    INT32S file_offset_bak;
    INT32S file_length;
    INT32U mp3FrameCount_temp;
    __X FCFILE *FS_Handle;
    FS_ERR fserr;
    tSDKLoadCodeStatus LoadCodeStatus;
    
    FS_Handle = Audio_Handle.FSHandle;
    
    if(Audio_Handle.Status != FWRWD_EPILOG_STATUS)
    {
        DAC_Pause();
    }

    if(time_position > MP3_Information.dwTimeLength-500)
    {
        Audio_Handle.Status = EOF_STATUS;
        return AUDIO_ERROR_EOF;
    }
    if(time_position > MP3_Information.dwTimeLength/2)
        SearchMode = 1;
    else
        SearchMode = 0;
//    LoadCodeStatus = LoadMP3InitCode();
//	DAC_FadeOut();
	    
    if(MP3_Information.bVBRType == CONSTANT_BITRATE)
    {
        MP3_Information.dwFilePosition =
            MP3_Information.dwHeaderOffset +
            (INT32U)MP3_Information.wFrameSize *
            ((time_position*10L)/MP3_Information.wFrameDuration);
        if(MP3_Information.dwFilePosition >= MP3_Information.dwFileSize)
        {
            MP3_Information.dwFilePosition =
                MP3_Information.dwHeaderOffset + 
                MP3_Information.dwAudioFileSize - 
                10*MP3_Information.wFrameSize;
        }
    }
    else if(MP3_Information.bVBRType == XING_VARIABLE_BITRATE)
    {
        MP3API_FSSeek(VBR_Information.dwTOC_Offset);
        if((file_length = MP3API_FSRead(buffer,
                                         512,
                                         &fserr)) == 0)
        {
            return AUDIO_ERROR_READ_FILE;
        }
        mp3FrameCount_temp = 
            MP3_XingSeekPoint(buffer,
                              time_position);
    }
    else if(MP3_Information.bVBRType == VBRI_VARIABLE_BITRATE)
    {
        MP3API_FSSeek(VBR_Information.dwTOC_Offset);
        if((file_length = MP3API_FSRead(buffer,
                                         512,
                                         &fserr)) == 0)
        {
            return AUDIO_ERROR_READ_FILE;
        }
        mp3FrameCount_temp = MP3_VbriSeekPoint(buffer,time_position);
    }
    else if(MP3_Information.bVBRType == NO_TOC_VARIABLE_BITRATE)
    {
    		mp3FrameCount_temp = MP3_XingNoTOCSeekPoint(time_position);
    }
    
    if(FindFrameHeader(FS_Handle,buffer) == 0)
    {
        Audio_Handle.Status = EOF_STATUS;
        return AUDIO_ERROR_EOF;
    }
    file_offset = MP3_Information.dwFilePosition;
    MP3API_FSSeek(file_offset);
    file_length = MP3API_FSRead((__X void*)catch_Buffer_,
                                 CATCH_BUF_SZ*2,
                                 &fserr);
	if(fserr != 0)
	    return AUDIO_ERROR_READ_FILE;
	Remain_Bitstream_len_ = MP3_Information.dwAudioFileSize +
                            MP3_Information.dwHeaderOffset;
	Remain_Bitstream_len_ -= FS_Handle->position;
	pHead_ = catch_Buffer_;
	pTail_ = END_catch_Buffer_;
	free_len_=0;
	mp3_rst();
	mp3_spectrum_data(Audio_Handle.MP3SpectrumData);
    if(MP3_Information.bVBRType == CONSTANT_BITRATE)
    {
        mp3FrameCount = (MP3_Information.dwFilePosition -
                        MP3_Information.dwHeaderOffset) /
                        MP3_Information.wFrameSize;
    }
    else
    {
        mp3FrameCount = mp3FrameCount_temp;
    }
//	LoadCodeStatus = LoadMP3PlayCode();
    mp3VolumeCtrl = 0;
//    MP3API_FirstDecode();
//    if(Audio_Handle.Status == FWRWD_EPILOG_STATUS)
//    {
////        LoadMP3InitCode();
//DAC_Go();
//    }
//    else
//    {
//        Audio_Handle.Status = PLAYING_STATUS;
//        DAC_Continue();
//        DAC_Go();    
//    }
    return AUDIO_ERROR_OK;
}





//------------------------------------------------------------------------------
#pragma CODE = MP3_COMMON_CODE
INT32U AudioMP3GetPlayTime_(tPlayTimeMode SetMode)
{
    if(SetMode == PLAY_TIME_MODE)
        return (MP3_Information.dwTimePosition > MP3_Information.dwTimeLength)?
                (MP3_Information.dwTimeLength):
                (MP3_Information.dwTimePosition);
    else if(SetMode == REMAIN_TIME_MODE)
        return (MP3_Information.dwTimeLength - MP3_Information.dwTimePosition);
    else if(SetMode == TOTAL_TIME_MODE)
        return MP3_Information.dwTimeLength;
    return 0;
}
