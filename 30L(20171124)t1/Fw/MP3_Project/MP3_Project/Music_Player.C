/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: Music_Player.C
 * Author		: Nick
 * Create Date	: December 2, 2008
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/
#include "Music_Player.H"
#include "KBDM9_DDK.H"
#include "MP3_API.H"

//------------------------------------------------------------------------------
#pragma CODE = MUSIC_PLAY_DATA
//------------------------------------------------------------------------------
//static __X tAudioHandle Audio_Handle;

//------------------------------------------------------------------------------
#pragma CODE = MUSIC_PLAY_CODE
const char OidCodeTab[] = "0:\\PEN\\OidTab.bin";
const char StrMP3Offset[] = "0:\\PEN\\OidOfst.bin";
const char StrMP3Data[] = "0:\\PEN\\OidData.bin";

//------------------------------------------------------------------------------
unsigned int GetSongIndex(unsigned int OID_Code)
{
    tOID_CODE_TBL buffer;
    unsigned int ret;
    unsigned long position;
    unsigned long Data_length;
    long TBL_index;
    long TBL_index_L;
    long TBL_index_R;
    __X FS_ERR fserr;
    __X FCFILE *fs_handle;

    if(OID_Code == 0xFFFFu)
        return 0;
    ret = 0;
    Data_length = FSGetFileSize((char *)OidCodeTab,&fserr);
    if(Data_length == 0)
    	return 0;
    fs_handle = FSOpen((char *)OidCodeTab,FS_MODE_READ,&fserr);
    TBL_index_L = 0;
    TBL_index_R = ((Data_length-CHECK_SUM_WIDTH)/sizeof(tOID_CODE_TBL))-1;
    while(TBL_index_L <= TBL_index_R)
    {
    	int read_len;
    	
        TBL_index = (TBL_index_R+TBL_index_L)/2;
        position = TBL_index * sizeof(tOID_CODE_TBL);
        fserr = FSSeek(fs_handle,position);
        read_len = FSRead(fs_handle,&buffer,sizeof(tOID_CODE_TBL),&fserr);
        if(OID_Code > buffer.OID_Code)
            TBL_index_L = TBL_index+1;
        else if(OID_Code < buffer.OID_Code)
            TBL_index_R = TBL_index-1;
        else
        {
        	ret = (buffer.index);
        	break;
		}
    }
    fserr = FSClose(fs_handle);
    return ret;
}

//------------------------------------------------------------------------------
tOIDDataInfo GetSongInfo(unsigned int index)
{
    tOIDDataInfo Info = {0L,0L,0L,FALSE};
    unsigned long MP3Data_length;
    unsigned long Data_length;
    unsigned long Data_offset;
    unsigned long MusicAmount;
    unsigned long XOR_Key;
    __X FS_ERR fserr;
    __X FCFILE *fs_handle;

    Data_length = FSGetFileSize((char *)StrMP3Offset,&fserr);
    if(Data_length == 0)
    	return Info;
    fs_handle = FSOpen((char *)StrMP3Offset,FS_MODE_READ,&fserr);
    FSRead(fs_handle,&MusicAmount,sizeof(MusicAmount),&fserr);
    FSRead(fs_handle,&XOR_Key,sizeof(XOR_Key),&fserr);
    if(index > MusicAmount || index == 0)
        goto Exit;
    fserr = FSSeek(fs_handle,OFFSET_TBL_WIDTH*2*index);
    FSRead(fs_handle,&Data_offset,sizeof(Data_offset),&fserr);
    FSRead(fs_handle,&Data_length,sizeof(Data_length),&fserr);
    MP3Data_length = FSGetFileSize((char *)StrMP3Data,&fserr);
    if(Data_offset > MP3Data_length)
        goto Exit;
    Info.offset = Data_offset;
    Info.length = Data_length;
    Info.XOR_Key = XOR_Key;
    Info.valid = TRUE;
    //Info.FileHandle = FSOpen((char *)StrMP3Data,FS_MODE_READ,&fserr);
Exit:
    FSClose(fs_handle);
    return Info;
}

//------------------------------------------------------------------------------
/*tAudioErr Music_Play(tOIDDataInfo OID_Data_Info)
{
	tAudioErr   Audio_err;
    ptAudioHandle pAudio_Handle;
    __X FS_ERR fserr;
    __X FCFILE *fs_handle;
	
    if((Audio_err = AudioMP3Init()) != AUDIO_ERROR_OK) 
        goto EXIT;
    if((Audio_err = AudioMP3OpenOIDFile()) != AUDIO_ERROR_OK)
    if((Audio_err = AudioMP3OpenReserveArea(OID_Data_Info.sector,
                                        OID_Data_Info.offset,
                                        OID_Data_Info.length,
                                        OID_Data_Info.XOR_Key,
                                        pAudio_Handle)) != AUDIO_ERROR_OK)
        goto EXIT;
    AudioMP3SetEQ(EQ_NOMAL);
    Audio_err = AudioMP3Play(pAudio_Handle);
EXIT:
    return Audio_err;
}*/

//------------------------------------------------------------------------------
/*tAudioErr Music_Stop(void)
{
	tAudioErr   Audio_err;
    ptAudioHandle pAudio_Handle;
    
    pAudio_Handle = &Audio_Handle;
    if((Audio_err = AudioMP3Pause(pAudio_Handle)) != AUDIO_ERROR_OK)
        goto EXIT;
    if((Audio_err = AudioMP3Close(pAudio_Handle)) != AUDIO_ERROR_OK)
        goto EXIT;
    Audio_err = AudioMP3Uninit(pAudio_Handle);
EXIT:
    return Audio_err;
}*/

//------------------------------------------------------------------------------
/*const int PlayStatusTable[] = {PLAYING_STATUS,
                               PAUSE_STATUS,
                               AB_REPEAT_STATUS,
                               A_STAGE_STATUS,
                               FWRWD_PROLOG_STATUS,
                               FORWARD_STATUS,
                               REWIND_STATUS,
                               FWRWD_EPILOG_STATUS,
                               (-1)};
//------------------------------------------------------------------------------
int CheckPlaying(void)
{
    tMusicStatus status;
    int i = 0;
    
    status = AudioMP3Status(&Audio_Handle);
    while(1)
    {
        if(PlayStatusTable[i] == (-1))
            break;
        else if(PlayStatusTable[i] == status)
            return (-1);
        else
            i++;
    }
    return 0;
}*/

//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------