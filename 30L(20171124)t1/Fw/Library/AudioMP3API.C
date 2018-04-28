//------------------------------------------------------------------------------
#include "MP3_API.H"
#include "string.h"
#include "SFR.H"
#include "compiler.h"
#include "globalvariable.h"
#include "com.h"
//------------------------------------------------------------------------------
//__X FCFILE *FSHandle;

extern U32 mp3_begin_add,play_file_size;
extern U8 gOidXOR_Key;
extern FCFILE __X *	fat_g_cur_file;
extern Bool OID_PLAY;
extern __X   FCFILE   *ADPCM_File; 
extern Bool fs_seek( FCFILE __X* pfile, U32 addr );
//------------------------------------------------------------------------------
	#pragma CODE = MP3_TMP_CODE02
//------------------------------------------------------------------------------
const unsigned int EqConstTBL[EQ_MAX][EQ_MAX_BAND] = {
 {0xDE70,0xB2BC,0x5492,0x4728,0x5FFD,0xAAB1,0xF11B,0xFFFF,0xFFFF,0xFFFF},  //Rock                4.8 	 2.9 	-3.6 	-5.1 	-2.5 	 2.5 	 5.5 	 6.7 	 6.7 	 6.7
 {0x6E35,0xB2BC,0xD46D,0xD46D,0xB904,0x7215,0x693F,0x693F,0x6E35,0x6E35},  //Pop		-1.3 	 2.9 	 4.4 	 4.4 	 3.2 	-1.0 	-1.7 	-1.7 	-1.3 	-1.3
 {0x8000,0x8000,0x8000,0x8000,0x8000,0x8000,0x49A8,0x49A8,0x49A8,0x3DF9},  //Classical		 0.0	 0.0	 0.0	 0.0	 0.0	 0.0 	-4.8,	-4.8	-4.8	-6.3
 {0x3DF9,0x3DF9,0x3DF9,0x5BAB,0x9BAC,0xE24D,0xE10C,0xD3A3,0xD200,0xD200},  //Soft		-6.3 	-6.3 	-6.3 	-2.9 	 1.7 	 6.7 	 9.7 	 9.7 	 9.7 	10.5
 {0xB286,0x9FC9,0x874C,0x9000,0x9000,0x6689,0x6101,0x6101,0xC000,0xC000},  //Jazz
 {0xB286,0xB286,0xB286,0x9B6A,0x874C,0x6C62,0x5943,0x56D2,0x56D2,0x56D2}}; //Bass
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
	#pragma CODE = MP3_API_CODE
//------------------------------------------------------------------------------
MP3API_STATUS AudioMP3FileOpen( char *file )
{
/*
	FS_ERR fs_err;
	if(OID_PLAY){
		if (g_b_play_afx){
				AudioMP3Handle.FSHandle = ADPCM_File;
			}
		    else{
				AudioMP3Handle.FSHandle = fat_g_cur_file;
			}		
		 //( void )fs_seek( AudioMP3Handle.FSHandle, mp3_begin_add);
			AudioMP3Offset = mp3_begin_add;
			AudioMP3FileLen = play_file_size;
		}
	else{
		AudioMP3Handle.FSHandle = FSOpen( file, FS_MODE_READ, &fs_err );
		if( fs_err != MP3_RUN_NRR )
			return MP3_RUN_ERR;
		AudioMP3Offset  = 0;
		AudioMP3FileLen = AudioMP3Handle.FSHandle->size;
		}*/
	return MP3_RUN_NRR;
}
//------------------------------------------------------------------------------
MP3API_STATUS AudioMP3FileClose( void )
{
/*
	if(OID_PLAY){
		}
	else{
	if( FSClose(AudioMP3Handle.FSHandle) != MP3_RUN_NRR )
		return MP3_RUN_ERR;
		}
	*/
	return MP3API_NO_ERR;
}
//------------------------------------------------------------------------------
MP3API_STATUS AudioMP3FileSeek( unsigned long offset )
{
	AudioMP3Position=offset;
	if(OID_PLAY){
		if( !fs_seek(AudioMP3Handle.FSHandle,AudioMP3Position+AudioMP3Offset))
			return MP3_RUN_ERR;
		//if((fs_seek(AudioMP3Handle.FSHandle, mp3_begin_add+offset))!= FS_NO_ERROR)
		//	return MP3_RUN_ERR;
		}
	else{
		if( FSSeek(AudioMP3Handle.FSHandle,AudioMP3Position+AudioMP3Offset) != MP3_RUN_NRR )
			return MP3_RUN_ERR;
		}
	return MP3API_NO_ERR;
}
//------------------------------------------------------------------------------
int AudioMP3FileRead( __X char *buf, int len )
{
	int i,read_len;
	FS_ERR fs_err;
	//unsigned char *data_buf;
		read_len = FSRead( AudioMP3Handle.FSHandle, buf, len, &fs_err );
		if( fs_err != MP3_RUN_NRR )
			return MP3_RUN_ERR;
		AudioMP3Position += read_len;
		/*
	SendString("\r\nlen:key=");
	send_hex_u16(len);
	send_hex_u8(gOidXOR_Key);
	SendString("\r\n");
	for(i=0;i<len;i++){
		send_hex_u8(*(buf+i));
		}*/
	if(OID_PLAY){
		   if( gOidXOR_Key > 0 ){
		      for( i = 0; i < len; i++ )
		         *(buf+i) ^= gOidXOR_Key;
		   	}
		}	

	return read_len;
}
//------------------------------------------------------------------------------
