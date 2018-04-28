/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: AudioMP3Open.c
 * Author		: Nick
 * Create Date	: September 1, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#include "mp3_internal_include.h"
#include "mp3_include.h"
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

#define byte_buf catch_Buffer_



//------------------------------------------------------------------------------

#pragma CODE = MP3_TEMP_CODE00
static INT16S FSMP3_GetInformation(void)
{
    FS_ERR fserr;
    //INT8U __X *byte_buf;
    INT32U file_remain_size;
    INT16U read_bytes;
    INT32U dwMP3_FrameHeader_b;
    INT32U dwHeaderOffset_b;
    INT16S frame_offset;
    INT16S CheckFrameCnt;
    
    //byte_buf = catch_Buffer_;
    if (Audio_Handle.FSHandle == 0)
        return (-1);
    
    MP3_CalculateFlag_ = 0;
    MP3_Information.dwFileSize = Audio_Handle.FSHandle->size;
    
    // Search Tag ID3v1
    MP3API_FSSeek(MP3_Information.dwFileSize - ID3v1SIZE);
    read_bytes = MP3API_FSRead((__X void*)&(Audio_Handle.ID3v1_Data),
                          ID3v1SIZE,
                          &fserr);
    if(read_bytes == 0)
        return (-2);
    if ((Audio_Handle.ID3v1_Data.TAG_id[0] == 'T') &&
        (Audio_Handle.ID3v1_Data.TAG_id[1] == 'A') &&
        (Audio_Handle.ID3v1_Data.TAG_id[2] == 'G'))
    {
        MP3_Information.wID3v1Size = ID3v1SIZE;
    }
    else
    {
        MP3_Information.wID3v1Size = 0;
        Audio_Handle.ID3v1_Data.Song_title[0] = 0;
    }
    
    // search Tag ID3v2
    MP3API_FSSeek(0);
    read_bytes = MP3API_FSRead((void*)byte_buf,
                                10,
                                &fserr);
    if(read_bytes == 0 ||
       fserr != FS_NO_ERROR)
        return (-2);
    if ((byte_buf[0] == (unsigned char)'I') &&
        (byte_buf[1] == (unsigned char)'D') &&
        (byte_buf[2] == (unsigned char)'3'))
    {   // Tag ID3v2
        MP3_Information.dwID3v2Size = ((unsigned long)(byte_buf[6] & 0x7F)<<21)
                                    + ((unsigned long)(byte_buf[7] & 0x7F)<<14)
                                    + ((unsigned long)(byte_buf[8] & 0x7F)<<7)
                                    + (unsigned long)(byte_buf[9] & 0x7F) + 10;
        MP3_Information.dwHeaderOffset = MP3_Information.dwID3v2Size;
    }
    else
    {
        MP3_Information.dwID3v2Size = 0;
        MP3_Information.dwHeaderOffset = 0;
    }
    
    // Find MP3 first frame header and get MP3_Information for file.
    MP3API_FSSeek(MP3_Information.dwHeaderOffset);
    file_remain_size = 
        MP3_Information.dwFileSize - MP3_Information.dwHeaderOffset;


    dwMP3_FrameHeader_b = 0;
    dwHeaderOffset_b = MP3_Information.dwHeaderOffset;
    CheckFrameCnt = 0;
    while (file_remain_size > 0)
    {
        if (file_remain_size > 512)
        {
            read_bytes = MP3API_FSRead(byte_buf,
                                        512,
                                        &fserr);
        }
        else
        {
            read_bytes = MP3API_FSRead(byte_buf,
                                        file_remain_size,
                                        &fserr);
        }
                                           
        if (read_bytes == 0)
            return (-2);
        file_remain_size -= read_bytes;
        dwMP3_FrameHeader_ = 0;
        frame_offset = MP3_GetAudioHeader((void*)byte_buf,
                                          read_bytes);
        if (frame_offset > 0)
        {
          Find_Header:
            dwMP3_FrameHeader_b = dwMP3_FrameHeader_ & CHECK_FRAME_DATA_MASK;
            dwHeaderOffset_b += frame_offset;
            MP3_Information.dwHeaderOffset = dwHeaderOffset_b;
            dwHeaderOffset_b += MP3_Information.wFrameSize;
            MP3API_FSSeek(dwHeaderOffset_b);
            CheckFrameCnt = 0;
            continue;
        }   
        else if (frame_offset == 0)
        {
            if(dwMP3_FrameHeader_b == (dwMP3_FrameHeader_ & CHECK_FRAME_DATA_MASK))             
            {
                CheckFrameCnt++;
                dwHeaderOffset_b += MP3_Information.wFrameSize;
            }
            else
            {
                if(CheckFrameCnt == 0)
                    goto Find_Header;
                dwHeaderOffset_b = MP3_Information.dwFilePosition+4;
                dwMP3_FrameHeader_b = 0;
                CheckFrameCnt = 0;
            }
            MP3API_FSSeek(dwHeaderOffset_b);
            file_remain_size = MP3_Information.dwFileSize - dwHeaderOffset_b;
            if(CheckFrameCnt > 5)
                break;
            continue;
        }
        dwHeaderOffset_b += read_bytes;
        if(dwHeaderOffset_b>0x20000)
        {
            file_remain_size = 0;
            break;
        }
    }

    if(file_remain_size == 0)
        return (-4);
        
    // Get Frame Duration 
    if(MP3_Information.wSampleRate/1000 > 0 && 
       MP3_Information.wSamplePerFrame > 0)
    {
        MP3_Information.wFrameDuration = 
            (MP3_Information.wSamplePerFrame*10)/
            (MP3_Information.wSampleRate/1000);
    }
    else
        return (-4);
    
    // Seek position to first frame, read 512 bytes data
    MP3API_FSSeek(MP3_Information.dwHeaderOffset);
    file_remain_size = MP3_Information.dwFileSize - MP3_Information.dwHeaderOffset;
    if (file_remain_size > 0)
    {
        read_bytes = MP3API_FSRead(byte_buf,
                                    512,
                                    &fserr);
    }
    else
    {
        if (MP3_Information.bLayer == MPEG_LAYER_1)
            return (-3);
        else
            return (-4);
    }

    
    // Get VBR_Infomation
    if (read_bytes > 62)
    {
        dwMP3_FrameHeader_ = 0;
        frame_offset = MP3_GetAudioHeader((void*)byte_buf,
                                          read_bytes);
        if (frame_offset == -1)
            return (-5);
        frame_offset = MP3_GetVbrHeader((void*)byte_buf,
                                        read_bytes);
    }
    
    return (0);
}


#pragma CODE = MP3_TEMP_CODE00
tAudioErr AudioMP3Open_( INT8S *filepath )
{
    FS_ERR fserr;
    INT32U read_len; // the buffer read_len
    INT32U file_offset;
    
    Audio_Handle.FSHandle = 0;
    FS_Handle_ = 0;
    
    if( (Audio_Handle.FSHandle = (__X FCFILE *)
        pMP3API_FSOpen((char *)filepath,
                                 FS_MODE_READ,
                                 &fserr)) == NULL )
    {
        Audio_Handle.Status = ERROR_STATUS;
        return AUDIO_ERROR_OPNE_FILE;
    }

    // Get MP3 information
    if(FSMP3_GetInformation())
    {
        Audio_Handle.Status = ERROR_STATUS;
        return AUDIO_ERROR_SEEK_FILE;
    }

    // Seek first frame, but if VBR then seek 2th frame.
    if(MP3_Information.bVBRType != CONSTANT_BITRATE)
    {
        file_offset = 
            MP3_Information.dwHeaderOffset +
            MP3_Information.wFrameSize;
    }
    else
    {
        file_offset = MP3_Information.dwHeaderOffset;
    }
    MP3_Information.wFrameSize =
        (unsigned int)((((unsigned long)MP3_Information.wSamplePerFrame 
        * (unsigned long)MP3_Information.wBitRate * 1000)
        / MP3_Information.wSampleRate) /8);
        
        
//    file_offset = file_offset & (unsigned long)0xFFFFFFFE; // 2009/11/10 by yj, avoid FSSeek to odd position bug
  
//    file_offset = 0;      // 2009/11/10 by YJ, don't need the offset
    MP3_Information.dwFilePosition = file_offset;

    if(MP3API_FSSeek(file_offset))
    {
        fserr = pMP3API_FSClose(Audio_Handle.FSHandle);
        Audio_Handle.Status = ERROR_STATUS;
        Audio_Handle.FSHandle = 0;
        return AUDIO_ERROR_SEEK_FILE;
    }
    
    //讀4個Sector=4*512 bytes=1024 word將Buffer填滿
    if( (read_len = MP3API_FSRead((void*) catch_Buffer_,
                                   (CATCH_BUF_SZ*2),
                                   &fserr)) == 0 )
    {
        fserr = pMP3API_FSClose(Audio_Handle.FSHandle);
        Audio_Handle.Status = ERROR_STATUS;
        Audio_Handle.FSHandle = 0;
        return AUDIO_ERROR_READ_FILE;
    }

    //MP3_Information.dwFilePosition = AudioHandle->FSHandle->position;
    //MP3_Information.dwAudioOffset = 0;
    MP3_Information.dwTimePosition = 0; 
    Remain_Bitstream_len_ = 
        MP3_Information.dwAudioFileSize +
        MP3_Information.dwHeaderOffset;  // 以Sector為單位
    Remain_Bitstream_len_ -= Audio_Handle.FSHandle->position;
    pHead_ = catch_Buffer_;
    pTail_ = END_catch_Buffer_;
    free_len_=0;
    FS_Handle_ = Audio_Handle.FSHandle;
    mp3FrameCount = 0;
    Audio_Handle.Status = OPEN_STATUS;
    mp3_spectrum_data(Audio_Handle.MP3SpectrumData);
 
    return AUDIO_ERROR_OK;
}


