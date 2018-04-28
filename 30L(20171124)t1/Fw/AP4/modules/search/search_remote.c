//!
//! @file search_remote.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search remote.
//!
//! @version 1.53 snd3b-dvk-1_9_0 $Id: search_remote.c,v 1.53 2007/04/03 09:49:23 sguyon Exp $
//!
//! @todo
//! @bug

//#pragma CODE = SEARCH_REMOTE_CODE
#pragma CODE = REMOTE_TASK_INIT_CODE
#pragma DATA = SEARCH_REMOTE_DATA

#ifndef _search_remote_c_
#define _search_remote_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>
#include "com.h"
#include "search.h"
//#include "search_game.h"
#include "search_remote.h"
#include "conf_oid.h"
//#include "modules\debug\debug_sio.h"
#include "string.h"
#include "system.h"
//#include "mail.h"
//#include "mail_evt.h"
#include "mmi_ap3.h"
//#include "srv_player.h"
//#include "com_evt.h"
#include "file.h"
#include "oid.h"
//#include "com_var.h"
#include "power.h"             // Power module definition
//#include <intrins.h>
#include <math.h>

//#include "strcast.h"
#include "MP3_api.h"  


#include "keypad.h"
#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

               Bool           g_b_remote_on ;




_MEM_TYPE_SLOW_ unsigned int cal_flag;
 _MEM_TYPE_SLOW_ REMOTE_BIT_NEW  remote_bits;



 _MEM_TYPE_SLOW_ U8     remote_to_mode;

   _MEM_TYPE_SLOW_   REMOTE_TAB           RemoteTable;
   _MEM_TYPE_SLOW_   REMOTE_SPECIAL_TAB   RemoteSpecialTable;

   _MEM_TYPE_SLOW_   U16                  RemoteNumber;

   _MEM_TYPE_SLOW_   U16                  RemoteVersion;
   _MEM_TYPE_SLOW_   REMOTE_SPEECH        RemoteSpeech;


   _MEM_TYPE_SLOW_   U8                   RemoteEncryptKey;
   _MEM_TYPE_SLOW_   U32                  RemoteFlag;
   
//extern   _MEM_TYPE_SLOW_   U16            remote_index_tab[ REMOTE_INDEX_MAX ];

//extern Bool flag_wav_play;


//_____ D E F I N I T I O N ________________________________________________
//                  U8                      gl_key;
//_MEM_TYPE_SLOW_  U8                       bRemotePress;
_MEM_TYPE_SLOW_   U8                      key_state;
_MEM_TYPE_SLOW_   U32                     RemoteCoorAddress;
//_MEM_TYPE_SLOW_  REMOTE_CODE              RemoteCode;


//_MEM_TYPE_SLOW_   U16                     remote_index_tab[ REMOTE_INDEX_MAX ];

#define  CAL_STATE_NUM1             0
#define  CAL_STATE_NUM2             1

_MEM_TYPE_SLOW_   U16               RemoteValue;
#if 0
_MEM_TYPE_SLOW_   Float16           CalNum1;
_MEM_TYPE_SLOW_   Float16           M_NUM;
_MEM_TYPE_SLOW_   Float16           M_NUM_RECORD;
_MEM_TYPE_SLOW_   Float16           CalNum2;
_MEM_TYPE_SLOW_   Float16           M_NUM_TEMP;
#else
_MEM_TYPE_SLOW_   double           CalNum1;
_MEM_TYPE_SLOW_   double           M_NUM;
_MEM_TYPE_SLOW_   double           M_NUM_RECORD;
_MEM_TYPE_SLOW_   double           CalNum2;
_MEM_TYPE_SLOW_   double           M_NUM_TEMP;
#endif
_MEM_TYPE_SLOW_   U8                M_NUM_FLAG;
_MEM_TYPE_SLOW_   U16               CalOP;
_MEM_TYPE_SLOW_   U8                cal_state;
_MEM_TYPE_SLOW_   U8                cal_input_length;
_MEM_TYPE_SLOW_   U8                cal_point_length;
_MEM_TYPE_SLOW_   U8                cal_int_length;


//_MEM_TYPE_SLOW_   U8                flag_go_cal;


_MEM_TYPE_SLOW_   U32  RemoteBaseAddress ;//               All_Control.Point_Remote

_MEM_TYPE_SLOW_   U32  RemoteRecordStartAddr ;
_MEM_TYPE_SLOW_   U32  RemoteRecordEndAddr ;
_MEM_TYPE_SLOW_   U32  RemotePlayStartAddr ;
_MEM_TYPE_SLOW_   U32  RemotePlayEndAddr ;

//extern unsigned int vol_idx;
//extern const int VolTbl[VOL_NUM];

#define  DIS_RESULT_ONLY      0x01
#define  DIS_MEMORY           0x02
#define  PLAY_RESULT          0x04
#define  GET_RESULT           0x08
#define  GET_MEMORY           0x10

ImageDeclareType(REMOTE_CACU_CODE);
ImageDeclareType(REMOTE_AP4_CODE);
ImageDeclareType(REMOTE_CALCULAT_CODE);
ImageDeclareType(REMOTE_PLAY_CODE);
ImageDeclareType(REMOTE_PLAYNUM_CODE);
ImageDeclareType(REMOTE_CALBACK_CODE);
ImageDeclareType(ADDR_BAK_CODE);
ImageDeclareType(REMOTE_CALBACK01_CODE);
ImageDeclareType(REMOTE_CALBACK02_CODE);
ImageDeclareType(CAL_DOWITH_01_CODE);
ImageDeclareType(CAL_DOWITH_02_CODE);
//_____ D E C L A R A T I O N ______________________________________________
void float_to_str(double value);
void cal_back(U8 back_flag);
void  play_number_speech            ( U16 hex_data );
//void  calculate_old                 ( void );
//void  play_float                    ( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length );
//Bool  search_remote_index           ( void );
//void  display_float                 ( Float16 result, U8 dis_type );
//void  clear_cal_error               ( void );
void remote_ap4(void);
void cal_dowith_01(void);
void cal_dowith_02(void);


//extern void swap_code(U16 flag);
//***********************************************
//remote task initial
void remote_task_init( void )
{
   //bRemotePress = FALSE;
   key_state = KEY_STATE_NULL;
   bRmoteExitAp4 = FALSE;
   //remote_state = REMOTE_START;
   
}


void init_remote( void )
{
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ U32 address,temp;
   _MEM_TYPE_SLOW_ REMOTE_SPEECH_DATA speech_temp;
   _MEM_TYPE_SLOW_ U32 *p = (U32 _MEM_TYPE_SLOW_ *)&RemoteSpeech;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   _MEM_TYPE_SLOW_ U8  j, n, length_remote, speech_length = 15;
//   U8 _MEM_TYPE_SLOW_ remote_coor_buf[ 512 ];

   U8 _MEM_TYPE_SLOW_ remote_flag = 0;
	 cal_flag = 0;
	 g_b_cal = FALSE;
	 M_NUM_RECORD = 0;
	 M_NUM = 0;
#if( REMOTE_VERION == REMOTE1 )
   //RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
//   length_remote = length_remote1_coordiante;
#elif( REMOTE_VERION == REMOTE2 )
   //SpecialFunction _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
//   length_remote = length_remote2_coordiante;
#endif
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

		RemoteRecordStartAddr = 0;
		RemoteRecordEndAddr = 0;
		  RemotePlayStartAddr = 0;
		  RemotePlayEndAddr = 0;
		  
	RemoteVersion = 0x01;
   memset( (char *)&RemoteSpeech, 0, sizeof( REMOTE_SPEECH ));
   memset( (char *)&RemoteSpecialTable, 0, sizeof( REMOTE_SPECIAL_TAB ));
   if(RemoteBaseAddress>=size_current_ap4)return;
   if( !RemoteBaseAddress )
   {
      //remote_state = REMOTE_NONE;	
      bHaveRemote = FALSE;
      g_b_remote_on = FALSE;
   }
   else
   {
#if( REMOTE_DECT == ENABLE )
      (void)file_seek( 4, FS_SEEK_END );
#if 1
	 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&address, sizeof(U32));
     if(0xFFFFEEEE==address)
#else
      if(( 0xFF == ( U8 )file_getc())
         &&( 0xFF == ( U8 )file_getc())
            &&( 0xEE == ( U8 )file_getc())
               &&( 0xEE == ( U8 )file_getc()))
#endif               
      {
         U16 temp;
         (void)file_seek( 10, FS_SEEK_END );
//         MSB( temp ) = ( U8 )file_getc();
//         LSB( temp ) = ( U8 )file_getc();
		(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof(U16));
         if( !MSB( temp )
            &&( LSB( temp )>= 0x02 ))
         {
            RemoteVersion = temp;
            speech_length = 19;
			if( RemoteVersion >= 4 )
            {
               (void)file_seek( 11, FS_SEEK_END );
			   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&RemoteEncryptKey, sizeof(RemoteEncryptKey));
               (void)file_seek( 8, FS_SEEK_END );
			   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&RemoteFlag, sizeof(RemoteFlag));
            }
         }
      
         (void)file_seek( 5, FS_SEEK_END );
//         remote_flag = ( U8 )file_getc();
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&remote_flag, sizeof(U8));
         if( !( remote_flag & NINE_BYTE_SPEECH_FLAG ))
         {
            bHaveRemote = FALSE;
            g_b_remote_on = FALSE;
            return;
         }
         read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));

		 
         if( remote_flag & SPECIAL_TABLE_FLAG )
         {
            read_data_from_nf( RemoteBaseAddress + sizeof( REMOTE_TAB ), ( U8 _MEM_TYPE_SLOW_ *)&RemoteSpecialTable, sizeof( REMOTE_SPECIAL_TAB ));
            RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB ) + sizeof( REMOTE_SPECIAL_TAB );
            goto search_init_remote;
         }
         else
         {
            RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB );
            goto search_init_remote;
         }
      }
//	 else{
//	 	 bHaveRemote = FALSE;
//      	 g_b_remote_on = FALSE;
//		 return;
//	 	}
#endif   //#if( REMOTE_DECT == ENABLE )

      read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));
#if( REMOTE_VERION == REMOTE2 )
      read_data_from_nf( RemoteBaseAddress + sizeof( REMOTE_TAB ), ( U8 _MEM_TYPE_SLOW_ *)&RemoteSpecialTable, sizeof( REMOTE_SPECIAL_TAB ));
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB ) + sizeof( REMOTE_SPECIAL_TAB );
#elif( REMOTE_VERION == REMOTE1 )
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB );
#endif   //#if( REMOTE_VERION == REMOTE2 )
	bHaveRemote = TRUE;
      if( 0 == RemoteSpecialTable.SpecialFunctionNumber )
      {
         g_b_remote_on = FALSE;
         return;
      }

#if( REMOTE_DECT == ENABLE )
search_init_remote:
#endif   //#if( REMOTE_DECT == ENABLE )
      address = RemoteTable.SpeechAddress + RemoteBaseAddress;
      for( i = 0; i < speech_length; i++ )
      {
         *p = address + ( U32 )i * 9;
         //read_data_from_nf( *p, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp, sizeof( REMOTE_SPEECH_DATA ));
         read_data_from_nf( *p, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp, 9 );
         //read_data_from_nf( *p + 8, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp.SpeechType, sizeof( U8 ));
         if( !speech_temp.SpeechStartAddr 
            || !speech_temp.SpeechLength )
            *p = 0;
         p++;
      }
	  if( RemoteVersion >= 3 ){
		  RemoteRecordStartAddr = RemoteBaseAddress + RemoteTable.SpeechAddress+19*9;
		  RemoteRecordEndAddr = RemoteBaseAddress + RemoteTable.SpeechAddress+22*9;
		  RemotePlayStartAddr = RemoteBaseAddress + RemoteTable.SpeechAddress+23*9;
		  RemotePlayEndAddr = RemoteBaseAddress + RemoteTable.SpeechAddress+26*9;
	  	}
#ifdef TX_DEBUG
		SendString("\r\nR_S:R_E:P_S:P_S=");send_hex_u32(RemoteRecordStartAddr);
		SendString(":");send_hex_u32(RemoteRecordEndAddr);
		SendString(":");send_hex_u32(RemotePlayStartAddr);
		SendString(":");send_hex_u32(RemotePlayEndAddr);
#endif 	  
      //remote_state = REMOTE_DONE;
      bHaveRemote = TRUE;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = TRUE;
      memset( remote_index_tab, 0, REMOTE_INDEX_MAX );
#if( REMOTE_DECT == ENABLE )
      if( remote_flag )
      {
         if( remote_flag & SPECIAL_TABLE_FLAG )
         {
 //           length_remote = length_remote2_coordiante;
            goto init_search_remote2;
         }
         else
         {
 //           length_remote = length_remote1_coordiante;
            goto init_search_remote1;
         }
      }
      else
      {
#if( REMOTE_VERION == REMOTE2 )
         goto init_search_remote2;
#endif   //#if( REMOTE_VERION == REMOTE2 )
#if( REMOTE_VERION == REMOTE1 )
         goto init_search_remote1;
#endif   //#if( REMOTE_VERION == REMOTE2 )
      }
#endif   //#if( REMOTE_DECT == ENABLE )
#if(( REMOTE_VERION == REMOTE2 )||( REMOTE_DECT == ENABLE ))
#if( REMOTE_DECT == ENABLE )
init_search_remote2:
#endif   //#if( REMOTE_DECT == ENABLE )
      if( !RemoteSpecialTable.SpecialFunctionNumber )
      {
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         return;
      }
      address = RemoteSpecialTable.SpecialFunctionAddress + RemoteBaseAddress;
//      j = RemoteSpecialTable.SpecialFunctionNumber % length_remote;
//      i = RemoteSpecialTable.SpecialFunctionNumber / length_remote;
      {
         SpecialFunction _MEM_TYPE_SLOW_ ptr;
		 ( void )file_seek( address, FS_SEEK_SET );
//         while( i )
         {
            for( n = 0; n < RemoteSpecialTable.SpecialFunctionNumber; n++ )
            {

			     //read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, sizeof( SpecialFunction ));
            if( RemoteVersion < 7 )
            {
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
               ptr.index = ( U32 )temp;
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ptr.function, sizeof( U16 ));
            }
            else
            {
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ptr, sizeof( SpecialFunction ));
            }
               switch( ptr.function )
               {
                  case REMOTE_KEY_VINC:
                  remote_index_tab[ REMOTE_VOL_UP ] = ptr.index;
                  break;

                  case REMOTE_KEY_VDEC:
                  remote_index_tab[ REMOTE_VOL_DOWN ] = ptr.index;
                  break;

                  case REMOTE_KEY_POWER:
                  remote_index_tab[ REMOTE_POWER_OFF ] = ptr.index;
                  break;

                  case REMOTE_KEY_AP4:
                  remote_index_tab[ REMOTE_AP4_MODE ] = ptr.index;
                  break;

                  case REMOTE_KEY_ENCAL:
                  remote_index_tab[ REMOTE_CAL_MODE ] = ptr.index;
                  break;

#if( REMOTE_MP3 == ENABLE )
                  case REMOTE_KEY_EXIT_MP3:
                  remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr.index;
                  break;

                  case REMOTE_KEY_PLAY_PAUSE:
                  remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr.index;
                  break;

                  case REMOTE_KEY_MP3_PREV:
                  remote_index_tab[ REMOTE_MP3_PREV ] = ptr.index;
                  break;

                  case REMOTE_KEY_MP3_NEXT:
                  remote_index_tab[ REMOTE_MP3_NEXT ] = ptr.index;
                  break;

                  case REMOTE_KEY_GO_MP3:
                  remote_index_tab[ REMOTE_MP3_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
                  case REMOTE_KEY_PASTER:
                  remote_index_tab[ REMOTE_PASTER_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
                  case REMOTE_KEY_ENDIC:
                  remote_index_tab[ REMOTE_DIC_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
                  case REMOTE_KEY_REC:
                  remote_index_tab[ REMOTE_REC_MODE ] = ptr.index;
                  break;

                  case REMOTE_KEY_REC_START:
                  remote_index_tab[ REMOTE_REC_START ] = ptr.index;
                  break;

                  case REMOTE_KEY_REC_PLAY:
                  remote_index_tab[ REMOTE_REC_PLAY ] = ptr.index;
                  break;

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr.index;
                  break;
				  
				  case REMOTE_KEY_OK:
                  remote_index_tab[ REMOTE_OK ] = ptr.index;
                  break;

				  case  REMOTE_VOL_0:
                  remote_index_tab[  REMOTE_V_0 ] = ptr.index;
                  break;

				  case  REMOTE_VOL_1:
                  remote_index_tab[ REMOTE_V_1 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_2:
                  remote_index_tab[ REMOTE_V_2 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_3:
                  remote_index_tab[ REMOTE_V_3 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_4:
                  remote_index_tab[ REMOTE_V_4 ] = ptr.index;
                  break;
#endif   //#if( REMOTE_REC == ENABLE )
               }
			   address += ( U32 )sizeof( SpecialFunction );
            }  
         }
         
         return;
      }
#endif   //#if(( REMOTE_VERION == REMOTE2 )||( REMOTE_DECT == ENABLE ))
#if(( REMOTE_VERION == REMOTE1 )||( REMOTE_DECT == ENABLE ))
#if( REMOTE_DECT == ENABLE )
init_search_remote1:
#endif   //#if( REMOTE_DECT == ENABLE )
      address = RemoteCoorAddress;
//      j = RemoteTable.Number % length_remote;
//      i = RemoteTable.Number / length_remote;
      {
         RemoteCoor _MEM_TYPE_SLOW_ ptr ;
//         while( i )
         {
            for( n = 0; n < RemoteTable.Number; n++ )
            {
            	 
               //read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, 4 );
            if( RemoteVersion < 7 )
            {
               read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
               ptr.index = ( U32 )temp;
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ptr.function, sizeof( U16 ));
               address += ( U32 )13;
            }
            else
            {
               read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, 6 );
               address += ( U32 )15;
            }
			   
               switch( ptr.function )
               {
                  case REMOTE_KEY_VINC:
                  remote_index_tab[ REMOTE_VOL_UP ] = ptr.index;
                  break;
                  case REMOTE_KEY_VDEC:
                  remote_index_tab[ REMOTE_VOL_DOWN ] = ptr.index;
                  break;

                  case REMOTE_KEY_POWER:
                  remote_index_tab[ REMOTE_POWER_OFF ] = ptr.index;
                  break;

                  case REMOTE_KEY_AP4:
                  remote_index_tab[ REMOTE_AP4_MODE ] = ptr.index;
                  break;

                  case REMOTE_KEY_ENCAL:
                  remote_index_tab[ REMOTE_CAL_MODE ] = ptr.index;
                  break;

#if( REMOTE_MP3 == ENABLE )
                  case REMOTE_KEY_EXIT_MP3:
                  remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr.index;
                  break;

                  case REMOTE_KEY_PLAY_PAUSE:
                  remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr.index;
                  break;

                  case REMOTE_KEY_MP3_PREV:
                  remote_index_tab[ REMOTE_MP3_PREV ] = ptr.index;
                  break;

                  case REMOTE_KEY_MP3_NEXT:
                  remote_index_tab[ REMOTE_MP3_NEXT ] = ptr.index;
                  break;

                  case REMOTE_KEY_GO_MP3:
                  remote_index_tab[ REMOTE_MP3_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_MP3 == ENABLE )
#if( REMOTE_PASTER == ENABLE )
                  case REMOTE_KEY_PASTER:
                  remote_index_tab[ REMOTE_PASTER_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
                  case REMOTE_KEY_ENDIC:
                  remote_index_tab[ REMOTE_DIC_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
                  case REMOTE_KEY_REC:
                  remote_index_tab[ REMOTE_REC_MODE ] = ptr.index;
                  break;
                  
                  case REMOTE_KEY_REC_START:
                  remote_index_tab[ REMOTE_REC_START ] = ptr.index;
                  break;

                  case REMOTE_KEY_REC_PLAY:
                  remote_index_tab[ REMOTE_REC_PLAY ] = ptr.index;
                  break;

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr.index;
                  break;

				  case REMOTE_KEY_OK:
                  remote_index_tab[ REMOTE_OK ] = ptr.index;
                  break;

				  case  REMOTE_VOL_0:
                  remote_index_tab[  REMOTE_V_0 ] = ptr.index;
                  break;

				  case  REMOTE_VOL_1:
                  remote_index_tab[ REMOTE_V_1 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_2:
                  remote_index_tab[ REMOTE_V_2 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_3:
                  remote_index_tab[ REMOTE_V_3 ] = ptr.index;
                  break;
				  case  REMOTE_VOL_4:
                  remote_index_tab[ REMOTE_V_4 ] = ptr.index;
                  break;
#endif   //#if( REMOTE_REC == ENABLE )
               }
			    address += ( U32 )13;
            }
         }
         
      }
#endif
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   }
}

#pragma CODE = SEARCH_CODE
//play remote speech
void play_RMT_speech( U32 speech )	
{  
   if( !speech )
      return;
   //speech += RemoteBaseAddress;
   write_media_addr( speech, 1 );
}

#pragma CODE = ADDR_BAK_CODE
/*
void mp3_addr_bak_init(void)
{
	mp3_addr_bak_flag_record=0;
	mp3_addr_bak_num_record = 0;
}

void mp3_addr_bak_begin(void)
{
	mp3_addr_bak_flag_record=0xAAAA;
}

void mp3_addr_bak_over(void)
{
	mp3_addr_bak_flag_record=0;
}

void mp3_addr_bak_inc(U32 addr,U8 flag)
{

	if(mp3_addr_bak_flag_record!=0)
		{
			if(mp3_addr_bak_num_record<(Length_v-1)){
				mp3_addr_bak[mp3_addr_bak_num_record].Addr_Speech = addr;
				mp3_addr_bak[mp3_addr_bak_num_record].Flag_Speech = flag;
				mp3_addr_bak_num_record++;
				}
		}
	
}

void copy_mp3_addr_play(void)
{

	if(mp3_addr_bak_num_record!=0)
		{
		Point_Read=0;
		for(Point_Write=0;Point_Write<mp3_addr_bak_num_record;Point_Write++){
			Array_V[Point_Write].Addr_Speech = mp3_addr_bak[Point_Write].Addr_Speech;
			Array_V[Point_Write].Flag_Speech = mp3_addr_bak[Point_Write].Flag_Speech;
			}
		 mp3_addr_bak_num_record = 0;
		}
	
}
*/
#pragma CODE = REMOTE_INDEX_CODE
//search remote index
Bool search_remote_index( void )
{
#if 0

#if( DICHOTOMY == ENABLE )
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle, number;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ Temp_coor;
#undef nEnd
#define nEnd   number
   if( !RemoteTable.Number )
      return FALSE;
   nStart = 0;
   nEnd = RemoteTable.Number - 1;
   while( nStart <= nEnd )
   { 
      nMiddle = ( nStart + nEnd )/ 2;
//      Address = RemoteCoorAddress +( U32 )sizeof( RemoteCoor )*( U32 )nMiddle;
//      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( RemoteCoor ));
	  Address = RemoteCoorAddress +( U32 )13*( U32 )nMiddle;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.index, sizeof( U16)*6);
      if( Current_Index == Temp_coor.index ) 
      {
         RemoteValue = Temp_coor.function;
         Address +=  ( U32 )sizeof( REMOTE_CODE );
         play_RMT_speech( Address );
#ifdef TX_DEBUG
		SendString("\r\nRemoteValue=");
		send_hex_u16(RemoteValue);
#endif
         //bRemotePress = TRUE;
         //state_search_to_play();
         return TRUE;
      }
      else if( Current_Index < Temp_coor.index )
      {
         if( nMiddle == 0 )
         {
            return FALSE;
         }
         nEnd = nMiddle - 1;
      }
      else
      {
         nStart = nMiddle + 1;
      }
   }
   //play_RMT_speech( RemoteSpeech.SpeechNull );
//   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
      (void)act_do( News_Go.Action_NoVoice, 0 );
   return FALSE;
#undef nEnd
#endif   //#if( DICHOTOMY == ENABLE )
#else
#if( DICHOTOMY == ENABLE )
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle, number;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ Temp_coor;
   U16 _MEM_TYPE_SLOW_ temp;
#undef nEnd
#define nEnd   number
   if( !RemoteTable.Number )
      return FALSE;
   nStart = 0;
   nEnd = RemoteTable.Number - 1;
#ifdef TX_DEBUG
		SendString("\r\nversion:RemoteCoorAddress:num=");send_hex_u16(RemoteVersion);
		SendString(":");send_hex_u32(RemoteCoorAddress);
		SendString(":");send_hex_u32(RemoteTable.Number);
#endif   
   while( nStart <= nEnd )
   { 
      nMiddle = ( nStart + nEnd )/ 2;
	  
//      Address = RemoteCoorAddress +( U32 )sizeof( RemoteCoor )*( U32 )nMiddle;
//      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( RemoteCoor ));
	  //Address = RemoteCoorAddress +( U32 )13*( U32 )nMiddle;
      //read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.index, sizeof( U16)*6);
	 if( RemoteVersion < 7 )
      {
         Address = RemoteCoorAddress +( U32 )13 *( U32 )nMiddle;
#if 0//def TX_DEBUG
		SendString("\r\naddr=");send_hex_u32(Address);
#endif		 
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
         Temp_coor.index = ( U32 )temp;
         //file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.function, sizeof( U8 )*9 );
         read_data_from_nf( (Address+2), ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.function, 11);
         Address += (U32)4;
      }
      else
      {
    
         Address = RemoteCoorAddress +( U32 )15 *( U32 )nMiddle;
#if 0//def TX_DEBUG
		SendString("\r\naddr=");send_hex_u32(Address);
#endif  		 
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, 6+9 );
         Address += (U32)6;
      }
	  
      if( Current_Index == Temp_coor.index ) 
      {
         RemoteValue = Temp_coor.function;
        // Address +=  ( U32 )sizeof( REMOTE_CODE );
#if 0//def TX_DEBUG
		SendString("\r\nvoice_addr=");send_hex_u32(Address);
#endif          
         play_RMT_speech( Address );
#ifdef TX_DEBUG
		SendString("\r\nRemoteValue=");
		send_hex_u16(RemoteValue);
#endif
         //bRemotePress = TRUE;
         //state_search_to_play();
         return TRUE;
      }
      else if( Current_Index < Temp_coor.index )
      {
         if( nMiddle == 0 )
         {
            return FALSE;
         }
         nEnd = nMiddle - 1;
      }
      else
      {
         nStart = nMiddle + 1;
      }
   }
   //play_RMT_speech( RemoteSpeech.SpeechNull );
//   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
      //(void)act_do( News_Go.Action_NoVoice, 0 );
      bInvalidFlag = true;
   return FALSE;
#undef nEnd
#endif   //#if( DICHOTOMY == ENABLE )

#endif
}

#pragma CODE = REMOTE_TASK_CODE
void change_cal(void)
{
	if(RemoteValue==REMOTE_KEY_ENCAL){
		//flag_go_cal = 0x80;
		g_b_cal = TRUE;
		cal_flag = 0;
		}
	
	if(RemoteValue==REMOTE_KEY_EXCAL){
		//flag_go_cal = 0;
		g_b_cal = FALSE;
		//cal_flag = 0;
		}
}
//Scan remote
void remote_task( void )
{
   //if(RemoteBaseAddress>=size_current_ap4)return;
//   SendString("594\r\n");
  // if( search_remote_index())
   //if( bRemotePress )
   {
//      SendString("597\r\n");
      //bRmoteExitAp4 = FALSE;
      //bRemotePress = FALSE;
/*

   */
   	 change_cal();
	if(g_b_cal){
		 if((( RemoteValue >= REMOTE_KEY_0 )
	       &&( RemoteValue <= REMOTE_KEY_9 ))
	         ||(( RemoteValue>=REMOTE_KEY_PLUS)&&(RemoteValue<=REMOTE_KEY_EXCAL))
	         	||(RemoteValue == REMOTE_CAL_BACK )||(RemoteValue == REMOTE_CAL_M_CLEAR)
	         	||(RemoteValue == REMOTE_CAL_M_ADD)||(RemoteValue == REMOTE_CAL_M_SUB)
	         	||(RemoteValue == REMOTE_CAL_M_READ)
	             ||( RemoteValue == REMOTE_KEY_DOT ))           
		 {

			if(cal_flag == 0)
			{
				CalOP = REMOTE_KEY_NULL;
			    cal_state = CAL_STATE_NUM1;
			    cal_input_length = 0;
			    cal_point_length = 0;
			    cal_int_length = 0;
			    remote_bits.value &= ~0x2822U;
			    bCalClear = TRUE;
	            bCalPoint = FALSE;
	//          CalAnswer = 0;
	          CalNum1 = 0;
	          cal_flag = 1;
			  bCalMemory = TRUE;
	        //  CalNum2 = 0;
			}
			
		   if((RemoteValue==REMOTE_CAL_M_CLEAR)||(RemoteValue==REMOTE_KEY_EXCAL)){
		   		bCalMemory=TRUE;
				M_NUM_RECORD = 0;
		 		M_NUM = 0;
				M_NUM_FLAG = 0;
				return;
		   	} 

			   SDKLoadCode(REMOTE_CACU_CODE);
			  	remote_cacu();
		 	}
		 else{
		 	if(RemoteValue!=REMOTE_KEY_ENCAL){
				Point_Read = 0;
				Point_Write = 0;
				}
		 	}
		}
	else{
			//if(g_b_cal){
				//Point_Read = 0;
				//Point_Write = 0;
			//	return;
			//	}
			SDKLoadCode(REMOTE_AP4_CODE);
			remote_ap4();
			
			
		}
	 }
}


char	vtoh(U16 v)
{
	if(v<10)
		return	v + '0';
	else if(v<16)
		return	v - 10 + 'A';
	else
		return '0';
}

char *	str_hex_u16(char * str,Bool _0x,U16 val)
{
	U16	mask = 0xF000;
	U8	index = 12;
	if(_0x)
	{
		*str++ = '0';
		*str++ = 'x';
	}
	while(mask)
	{
		*str++ = vtoh((val&mask)>>index);
		mask>>=4;
		index-=4;
	}
	*str = 0;
	return	str;
}
//play number speech
void play_number_speech( U16 hex_data )
{

   _MEM_TYPE_SLOW_ U8 s[ 6 ];
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;
//   ulen = sprintf((char *)s, "%u", hex_data );
	str_hex_u16((char *)s, 0, hex_data );
   ulen = strlen((char *)s);
   temp = 0;
   if( RemoteVersion != 0x01 )
      goto play_num_speech_v2;
   while( s[ temp ] != 0 )
   {
      i = s[ temp ] - '0';
      play_RMT_speech( RemoteSpeech.Number[ i ]);
      temp++;
   }
   return;

play_num_speech_v2:
   while( s[ temp ] != 0 )
   {
      switch( ulen )
      {
         case 0x05:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         play_RMT_speech( RemoteSpeech.SpeechTenThousand );
         temp++;
         if( check_zero_number( &s[ temp ], 4 ))
            return;
         ulen--;
         break;

         case 0x04:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechThousand );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 3 ))
            return;
         ulen--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x03:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechHundred );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 2 ))
            return;
         ulen--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x02:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechTen );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 1 ))
            return;
         ulen--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x01:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         return;
      }
   }

}





Bool check_zero_number( U8 _MEM_TYPE_SLOW_ *p, U8 length )
{
   while( length )
   {
      if( *p != '0' )
         return FALSE;
      length--;
      p++;
   }
   return TRUE;
}

void clear_cal_error( void )
{
   CalNum1 = 0.0;
   CalNum2 = 0.0;
   CalOP = REMOTE_KEY_NULL;
   cal_state = CAL_STATE_NUM1;
   cal_input_length = 0;
   cal_point_length = 0;
   cal_int_length = 0;
   /*bNewCal = FALSE;
   bCalErr = FALSE;
   bRmoteExitAp4 = FALSE;
   bCalPoint = FALSE;*/
   remote_bits.value &= ~0x2822U;
   bCalClear = TRUE;
 //  bInCal = TRUE;
//   remote_bits.value |= 0x00C0;
}
#pragma CODE = REMOTE_AP4_CODE
void remote_ap4(void)
{
#ifdef TX_DEBUG
		SendString("\r\nremote_ap4!");
#endif
		if(RemoteValue==REMOTE_KEY_EXCAL){
		 cal_flag = 0;
		 return;
		 }
	      if( RemoteValue == REMOTE_KEY_VINC)
	      {
	         	   if(vol_idx < (VOL_NUM-1))
					{
					     vol_idx++;
//						 AudioMP3SetVol(VolTbl[vol_idx]);
					}
	         //bRemotePress = FALSE;
	         return;
	      }
	      if( RemoteValue == REMOTE_KEY_VDEC)
	      {
		 	   if(vol_idx > 0)
				{
				     vol_idx--;
//					 AudioMP3SetVol(VolTbl[vol_idx]);
				}
	         return;
	      }
		   if(( RemoteValue>= REMOTE_VOL_0)&&( RemoteValue<= REMOTE_VOL_4))
	      {
			 vol_idx=RemoteValue-REMOTE_VOL_0;
//			 AudioMP3SetVol(VolTbl[vol_idx]);
	         return;
	      }
	      if( RemoteValue == REMOTE_KEY_POWER )
	      {
			key_scan.key_write_point = key_scan.key_read_point = 0;
			//key_scan.key_word[key_scan.key_write_point].key_type = KBD_KEY_LONGPRESSED;
			//key_scan.key_word[key_scan.key_write_point].key_num = KEY_POWER;
			//key_scan.key_write_point++;
			kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_POWER);
	        return;
	      }
	      if( ( RemoteValue == REMOTE_KEY_AP4 ))
	      {
	         bRmoteExitAp4 = FALSE;
	         return;
	      }
/*		  
		  SDKLoadCode(ADDR_BAK_CODE);
		if( RemoteValue == REMOTE_MP3_BEGIN_RECORD)
		  {
		  	mp3_addr_bak_init();
			mp3_addr_bak_begin();
		  	return;
		  }

		if( RemoteValue == REMOTE_MP3_OVER_RECORD)
		  {
		  	mp3_addr_bak_over();
		  	return;
		  }
		if( RemoteValue == REMOTE_MP3_PLAY_RECORD)
		  {
		  	copy_mp3_addr_play();
		  	return;
		  }
*/		
#if( REMOTE_PASTER == ENABLE )
	      if( RemoteValue == REMOTE_KEY_PASTER )
	      {
	      	 Kbd_set_evt_disabled();
			 scan_oid_disable();
	         bRmoteExitAp4 = TRUE;
	         remote_to_mode = PASTER_APPLI;
	         return;
	      }
#endif   //#if( REMOTE_PASTER == ENABLE )
#if( REMOTE_MP3 == ENABLE )
	     if( RemoteValue == REMOTE_KEY_GO_MP3 )
	      {
			 bRmoteExitAp4 = TRUE;
			 Kbd_set_evt_disabled();
			 scan_oid_disable();
	         remote_to_mode = PLAYER_APPLI;
	         return;
	     }
#endif   //#if( REMOTE_MP3 == ENABLE )
#if( REMOTE_DIC == ENABLE )
	     if( RemoteValue == REMOTE_KEY_ENDIC )
	      {
			 bRmoteExitAp4 = TRUE;
			 Kbd_set_evt_disabled();
			 scan_oid_disable();
	         remote_to_mode = DIC_APPLI;
	         return;
	     }
#endif   //#if( REMOTE_MP3 == ENABLE )

	      switch( key_state )
	      {
	         case KEY_STATE_NULL:
	         if( RemoteValue == REMOTE_KEY_AP3 )
	         {
	            key_state = KEY_STATE_AP3;
	            RemoteNumber = 0;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_1 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_20 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_1 + 1;
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_21 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_99 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_21 + 21;
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_100 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_3000 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_100 + 100;
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if( RemoteValue == REMOTE_KEY_BOOK )
	         {
//	            if( Ap3_MenuPath >= BOOK_MENU_PATH )
	            {
	               key_state = KEY_STATE_BOOK;
	               RemoteNumber = 0;
	            }
	         }
	         else if(( RemoteValue >= REMOTE_KEY_P1 )
	                  &&( RemoteValue <= REMOTE_KEY_P250 ))
	         {
//	            if( Ap3_MenuPath >= PAGE_MENU_PATH )
	            {
	               RemoteNumber = RemoteValue - REMOTE_KEY_P1 + 1;
	               if( RemoteNumber <= News_Book.Num_GoTable )
	               {
	                  play_number_speech( RemoteNumber );
	                  play_RMT_speech( RemoteSpeech.SpeechPage );
	                  Current_Record.Page_num = RemoteNumber - 1;
	                  State_Search = Page_Search;
	               }
	               else
	               {
//	                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	                     //(void)act_do( News_Go.Action_NoVoice, 0 );
	                     bInvalidFlag = true;
	               }
	            }
	         }
	         else if( RemoteValue == REMOTE_KEY_PAGE )
	         {
//	            if( Ap3_MenuPath >= PAGE_MENU_PATH )
	            {
	               key_state = KEY_STATE_PAGE;
	               RemoteNumber = 0;
	            }
	         }
	         else if( RemoteValue==REMOTE_KEY_PU )
	         {
//	            if( Ap3_MenuPath >= PAGE_MENU_PATH )
	            {
	               if( Current_Record.Page_num )
	               {
	                  Current_Record.Page_num--;
	                  State_Search = Page_Search;
	               }
	            }
	         }
	         else if( RemoteValue == REMOTE_KEY_PD )
	         {
//	            if( Ap3_MenuPath >= PAGE_MENU_PATH )
	            {
	               if( Current_Record.Page_num <( News_Book.Num_GoTable - 1 ))
	               {
	                  Current_Record.Page_num++;
	                  State_Search = Page_Search;
	               }
	            }
	         }
	         else if(( RemoteValue >= REMOTE_KEY_CTL_1 )
	                  &&( RemoteValue <= REMOTE_KEY_CTL_20 ))
	         {
//	            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	            {
	               RemoteNumber = RemoteValue - REMOTE_KEY_CTL_1 + 1;
	               if( RemoteNumber <= News_Go.Num_ControlTab )
	               {
	                  //change new control mode.
	                  Temp_Control_num = Current_Record.Control_num;
	                  Current_Record.Control_num = RemoteNumber - 1;
	                  State_Search = Control_Mode_Search;
	               }
	               else
	               {
	                  //(void)act_do( News_Go.Action_NoVoice, 0 );
	                  bInvalidFlag = true;
	               }
	            }
	         }
	         break;

	         case KEY_STATE_AP3:
	         if( RemoteValue == REMOTE_KEY_AP3 )
	         {
	            RemoteNumber = 0;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_0 )
	                  &&( RemoteValue <= REMOTE_KEY_9 ))
	         {
	            if( RemoteNumber > 65500 )
	               break;
	            RemoteNumber = RemoteNumber * 10 + RemoteValue - REMOTE_KEY_0;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_1 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_20 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_1 + 1;
	            //new ap3 shall be load.
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_21 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_99 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_21 + 21;
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_AP3_100 )
	                  &&( RemoteValue <= REMOTE_KEY_AP3_3000 ))
	         {
	            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_100 + 100;
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if( RemoteValue == REMOTE_KEY_OK )
	         {
	            State_Search = Play_Search;
	            bRemoteChangeAp3 = TRUE;
	         }
	         else if( RemoteValue == REMOTE_KEY_BOOK )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_BOOK;
	         }
	         else if( RemoteValue == REMOTE_KEY_PAGE )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_PAGE;
	         }
	         else
	         {
	            key_state = KEY_STATE_NULL;
	         }
	         break;

	         case KEY_STATE_BOOK:
	         if( RemoteValue == REMOTE_KEY_BOOK )
	         {
	            RemoteNumber = 0;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_0 )
	                  &&( RemoteValue <= REMOTE_KEY_9 ))
	         {
	            if( RemoteNumber > 65500 )
	               break;
	            RemoteNumber = RemoteNumber * 10 + RemoteValue - REMOTE_KEY_0;
	         }
	         else if( RemoteValue == REMOTE_KEY_OK )
	         {
	            if( RemoteNumber )
	            {
	               //change book.
	               if( RemoteNumber <= All_Control.Num_Book )
	               {
	                 play_number_speech( RemoteNumber );	
	                  Current_Record.Book_num = RemoteNumber - 1;
	                  State_Search = Book_Search;
	               }
	               else
	               {
//	                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	                     //(void)act_do( News_Go.Action_NoVoice, 0 );
	                     bInvalidFlag = true;
	               }
	               key_state = KEY_STATE_NULL;
	            }
	            else
	            {
//	               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	                  //(void)act_do( News_Go.Action_NoVoice, 0 );
	                  bInvalidFlag = true;
	            }
	         }
	         else if( RemoteValue == REMOTE_KEY_AP3 )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_AP3;
	         }
	         else if( RemoteValue == REMOTE_KEY_PAGE )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_PAGE;
	         }
	         else
	         {
	            key_state = KEY_STATE_NULL;
	         }
	         break;

	         case KEY_STATE_PAGE:
	         if( RemoteValue == REMOTE_KEY_PAGE )
	         {
	            RemoteNumber = 0;
	         }
	         else if(( RemoteValue >= REMOTE_KEY_0 )
	                  &&( RemoteValue <= REMOTE_KEY_9 ))
	         {
	            if( RemoteNumber > 65500 )
	               break;
	            RemoteNumber = RemoteNumber * 10 + RemoteValue - REMOTE_KEY_0;
	         }
	         else if( RemoteValue == REMOTE_KEY_OK )
	         {
	            if( RemoteNumber )
	            {
	               //change page.
	               if( RemoteNumber <= News_Book.Num_GoTable )
	               {
	                  play_number_speech( RemoteNumber );
	                  play_RMT_speech( RemoteSpeech.SpeechPage );
	                  Current_Record.Page_num = RemoteNumber - 1;
	                  State_Search = Page_Search;
	               }
	               else
	               {
//	                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	                     //(void)act_do( News_Go.Action_NoVoice, 0 );
	                     bInvalidFlag = true;
	               }
	               key_state = KEY_STATE_NULL;
	            }
	            else
	            {
//	               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
	                 // (void)act_do( News_Go.Action_NoVoice, 0 );
	                 bInvalidFlag = true;
	            }
	         }
	         else if( RemoteValue == REMOTE_KEY_AP3 )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_AP3;
	         }
	         else if( RemoteValue == REMOTE_KEY_BOOK )
	         {
	            RemoteNumber = 0;
	            key_state = KEY_STATE_BOOK;
	         }
	         else
	         {
	            key_state = KEY_STATE_NULL;
	         }							
	         break;
	      }
	      //bRemotePress = FALSE;
}
//#ifdef CACU
#pragma CODE = REMOTE_CACU_CODE
//#if 0
void remote_cacu(void)
{
	unsigned int temp_1;
#ifdef TX_DEBUG
			SendString("\r\nremote_cacu!");
#endif
      if( 1 )
      {
         if(( RemoteValue == REMOTE_KEY_EXCAL )
            ||( RemoteValue == REMOTE_KEY_AP4 ))
         {
            bRmoteExitAp4 = FALSE;
        //    bInCal = FALSE;
            remote_bits.value &= ~0x0880U;
            //bRemotePress = FALSE;
             M_NUM_RECORD = 0;
			 M_NUM = 0;
			 
            return;
         }
	/*	 bCalErr  = FALSE;
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               (void)act_do( News_Go.Action_NoVoice, 0 );
            return;
         }*/

			if(RemoteValue == REMOTE_CAL_M_READ){
				temp_1 = cal_flag;
				SDKLoadCode(REMOTE_PLAY_CODE);
			    float_to_str(M_NUM);
				cal_flag = temp_1;
//				M_NUM_FLAG = 1;
//				CalNum1 = M_NUM; 
//				M_NUM_RECORD = CalNum1;
//				cal_state=CAL_STATE_NUM1;
//				if(cal_state==CAL_STATE_NUM1){
//						CalNum1 = M_NUM;
//						}
				if(cal_state==CAL_STATE_NUM2){
				   	 if(CalOP!=REMOTE_KEY_NULL){
					 	CalNum2 = M_NUM;
				   	 	}
					}
				else{
					cal_flag = 1;
					CalNum1 = M_NUM;
					cal_input_length = 1;
					cal_state = CAL_STATE_NUM1;
					}
				return;
				}
			/*
			 if((RemoteValue == REMOTE_KEY_EQUAL)&&(M_NUM_FLAG == 1)){
			 		CalNum2 = M_NUM;
					CalNum1 = M_NUM_RECORD;
				 	calculate_old();
					SDKLoadCode(REMOTE_PLAY_CODE);
		    		float_to_str(CalNum1);
					M_NUM_FLAG = 0;
					return;
			 	}*/
			  if(RemoteValue==REMOTE_CAL_M_ADD){

					   if(cal_state==CAL_STATE_NUM2){
					   	 if(CalOP!=REMOTE_KEY_NULL){
						 	calculate_old();
			      			M_NUM_RECORD = CalNum1;
					   	 	}
					   	}
			  			M_NUM += M_NUM_RECORD;
						SDKLoadCode(REMOTE_PLAY_CODE);
			    		//float_to_str(M_NUM);
			    		float_to_str(M_NUM_RECORD);
						//cal_flag= 1;
						//CalNum1 = 0;
				  		//CalNum2 = 0;
						return;
				 }
			  	  if(RemoteValue==REMOTE_CAL_M_SUB){
				  	 if(cal_state==CAL_STATE_NUM2){
					   	 if(CalOP!=REMOTE_KEY_NULL){
						 	calculate_old();
			      			M_NUM_RECORD = CalNum1;
					   	 	}
					   	}
			  			M_NUM -= M_NUM_RECORD;
						SDKLoadCode(REMOTE_PLAY_CODE);
			    		//float_to_str(M_NUM);
			    		float_to_str(M_NUM_RECORD);
						//cal_flag= 1;
						 //CalNum1 = 0;
				  		//CalNum2 = 0;
						return;
				 }
	
         switch( cal_state )
         {
            case CAL_STATE_NUM1:
            if((( RemoteValue >= REMOTE_KEY_0 )
               &&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_CAL_BACK )
                     ||( RemoteValue == REMOTE_KEY_DOT ))
            {
               if( bCalClear )
               {
                  CalNum1 = 0;
                  bCalClear = FALSE;
                  cal_flag = 1;
               }	
              
               if(RemoteValue == REMOTE_CAL_BACK)
               {
                  SDKLoadCode(REMOTE_CALBACK_CODE);
                  cal_back(1);
                  break;
               }
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }
			   SDKLoadCode(CAL_DOWITH_02_CODE);
			   cal_dowith_02();
               
                if( (cal_input_length-cal_point_length > 7)||(cal_point_length > 2 ) )
               {
 #ifdef TX_DEBUG
 				SendString("\r\n1387");
 #endif
                  cal_flag = 2;
                  return ;
               }
               break;
            }
            switch( RemoteValue )
            {
               case REMOTE_KEY_PLUS:
               case REMOTE_KEY_MINUS:
               case REMOTE_KEY_MUL:
               case REMOTE_KEY_DIV:
               if( cal_input_length == 0 )
               {
 #ifdef TX_DEBUG
 				SendString("\r\n1451");
 #endif
                  cal_flag = 2;
                  return ;
               }
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
               CalOP = RemoteValue;
               CalNum2 = 0;
               cal_state = CAL_STATE_NUM2;
               break;

               case REMOTE_KEY_EQUAL:
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
             //  calculate_old();
               CalOP = REMOTE_KEY_NULL;
               bCalClear = TRUE;
			      cal_flag = 0;
             /*  if( bCalErr )
               {
                  (void)act_do( News_Go.Action_NoVoice, 0 );
                  clear_cal_error();
                  break;
               }*/
               break;
            }
            break;

            case CAL_STATE_NUM2:
            if((( RemoteValue >= REMOTE_KEY_0 )
               &&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_CAL_BACK )
                     ||( RemoteValue == REMOTE_KEY_DOT ))
            {			   
               
                if(RemoteValue == REMOTE_CAL_BACK)
               {
                  SDKLoadCode(REMOTE_CALBACK_CODE);
                  cal_back(0);
                  break;
               }
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }

			   SDKLoadCode(CAL_DOWITH_01_CODE);
               cal_dowith_01();
               
               if((cal_input_length-cal_point_length > 7)||(cal_point_length > 2 ))
               {

 #ifdef TX_DEBUG
 				SendString("\r\n1544");
 #endif			   
                  cal_flag = 2;
                  return ;
               }
               bNewCal = TRUE;
            }
            switch( RemoteValue )
            {
               case REMOTE_KEY_PLUS:
               case REMOTE_KEY_MINUS:
               case REMOTE_KEY_MUL:
               case REMOTE_KEY_DIV:
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
               if( bNewCal )
               {
//               SDKLoadCode(REMOTE_CALCULAT_CODE);
               calculate_old();
              /*    if( bCalErr )
                  {
                     (void)( News_Go.Action_NoVoice, 0 );
                     clear_cal_error();
                     break;
                  }*/
               }
               bNewCal = FALSE;
               CalNum2 = 0;
               CalOP = RemoteValue;
               break;

               case REMOTE_KEY_EQUAL:
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
//               SDKLoadCode(REMOTE_CALCULAT_CODE);
               calculate_old();
			  // play_number_speech(CalNum1);
			  	  M_NUM_RECORD = CalNum1;
			      SDKLoadCode(REMOTE_PLAY_CODE);
			      float_to_str(CalNum1);
				  CalNum1 = 0;
				  CalNum2 = 0;
              // CalOP = REMOTE_KEY_NULL;
            //   bCalClear = TRUE;
			     // cal_flag = 0;;
            //   cal_state = CAL_STATE_NUM1;
            /*   if( bCalErr )
               {
                   (void)act_do( News_Go.Action_NoVoice, 0 );
                  clear_cal_error();
                  break;
               }*/
               break;
            }
            break;
         }
         return;
      }
}

//#pragma CODE = REMOTE_CALCULAT_CODE
//calculate old result
void calculate_old( void )
{
   switch( CalOP )
   {
      case REMOTE_KEY_PLUS:
      CalNum1 = CalNum1 + CalNum2;
      break;

      case REMOTE_KEY_MINUS:
      CalNum1 = CalNum1 - CalNum2;
      break;

      case REMOTE_KEY_MUL:
      CalNum1 = CalNum1 * CalNum2;
      break;

      case REMOTE_KEY_DIV:
      if( CalNum2 == 0 )
         CalNum1 = 0;
      else
         CalNum1 = (CalNum1*1.0) / CalNum2;
      break;
   }
}

#pragma CODE = REMOTE_PLAY_CODE

void play_float( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length )
{
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;

   if( point_length )
   {
      ulen = int_length + point_length - 1;
      for( i = 0; i < point_length; i++ )
      {
         if( s[ ulen ]!= '0' )
            break;
         s[ ulen ] = 0;
         ulen--;
      }
      if( i == point_length )
         s[ ulen ] = 0;
      int_length--;
   }
   else
   {
      int_length--;
      s[ int_length ] = 0;
   }
   
   point_length = int_length;

   if( s[ 0 ] == '-' )
   {
      play_RMT_speech( RemoteSpeech.SpeechNeg );
      temp = 1;
      int_length--;
   }
   else
      temp = 0;

   if( RemoteVersion == 0x01 )
   //if(1)
      goto play_float_v1;
   if(!(RemoteSpeech.SpeechTenThousand
   	   || RemoteSpeech.SpeechHundred 
   	   	|| RemoteSpeech.SpeechThousand
   	   	  || RemoteSpeech.SpeechTen))
   	   	 goto play_float_v1;
   
   while(( s[ temp ] != 0 )&& int_length )
   {
      switch( int_length )
      {
         case 0x07:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         play_RMT_speech( RemoteSpeech.SpeechHundred );
         temp++;
         if( check_zero_number( &s[ temp ], 6 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         break;

         case 0x06:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
            {
               play_RMT_speech( RemoteSpeech.Number[ i ]);
               play_RMT_speech( RemoteSpeech.SpeechTen );
            }
            else
            {
               if( !check_zero_number( &s[ temp + 1 ], 1 ))
                  play_RMT_speech( RemoteSpeech.Number[ i ]);
            }
         }
         temp++;
         if( check_zero_number( &s[ temp ], 5 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x05:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
               play_RMT_speech( RemoteSpeech.Number[ i ]);
         }
         play_RMT_speech( RemoteSpeech.SpeechTenThousand );
         temp++;
         if( check_zero_number( &s[ temp ], 4 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x04:
         zero_flag = FALSE;
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechThousand );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 3 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x03:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechHundred );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 2 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x02:
         i = s[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechTen );
         }
         temp++;
         if( check_zero_number( &s[ temp ], 1 ))
         {
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         if( i )
            zero_flag = FALSE;
         else
            zero_flag = TRUE;
         break;

         case 0x01:
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         int_length--;
         break;

         default:
         return;
      }
   }

   temp = point_length;

play_float_v1:
   while( s[ temp ] != 0 )
   {
      if( s[ temp ] == '.')
         play_RMT_speech( RemoteSpeech.SpeechDot );
      else
      {
         i = s[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
      }
      temp++;
   }
}


void float_to_str(double value)
{
  U8 int_len,str[14],data_flag,len,i,f_len=0;
  U32 int_data,temp;
  double f_temp;
  U32 f_data;
 // U8 t_len=7;
//  U32 tt=10;
//  U32 t=1000000;
	int_len = 0;
	len = 0;
	if(value>=0)
	{
		data_flag = 0;
      value += 0.005;
	}		
	else
	{
		data_flag = 1;
		value =0-value +0.005;
	}		
	int_data = (U32)value;
	f_temp = value - (double)int_data;
   if(int_data>=10000000)//判断计算结果是否超过7位
   {
 #ifdef TX_DEBUG
 				SendString("\r\n1874");
 #endif   
      cal_flag = 2;
      return;
   }
   else
   {
      cal_flag = 0;
   }
   f_data = (U32)(f_temp * 100);
 /*  if(f_data%10>4)
   {
   f_data =f_data/10 + 1;
   if(f_data %100 ==0)
      int_data ++;
   }
   else
      f_data /=10;*/
   f_len =2;
   /*
   while(t_len>3)
   {
      if(int_data/t)
      {
         f_data = (U32)(f_temp * tt);
         if(f_data%10>4)
         {
         f_data =f_data/10 + 1;
         if(f_data %(tt/10) ==0)
            int_data ++;
         }
         else
            f_data /=10;
       //  f_len =1;
       break;
      }
      else
      {
         f_len++;
         tt *=10;
         t  /=10;
         t_len--;
      }
   }
   if(t_len<=3)
   {
      f_data = (U32)(f_temp * 100000);
      if(f_data%10>4)
         {
         f_data =f_data/10 + 1;
         if(f_data %10000 ==0)
            int_data ++;
         }
      else
         f_data /=10;
      f_len =4;
   }*/
   
	while(len<f_len)     //小数部分
	{
		str[len++] = f_data%10 + '0';
		f_data /= 10;
	}
	str[len++]= '.';
	if(int_data==0)
	{
		str[len++] = 0+'0';
		int_len = 2;
	//	goto end;
	}
	while(int_data>0)      //整数部分
	{
		str[len++] = int_data%10 + '0';
		int_data /=10;
	}	
	if(data_flag)
		str[len++] = '-';
	int_len = len-f_len;   //整数位长度
//	end:
	for(i=0;i<(len/2);i++)  //
	{
		temp = str[i];
		str[i] = str[len-1-i];
		str[len-1-i] = temp;
	}
	str[len] =0;
/*	if((data_flag==1)&&(int_len>=10))
	{
		bCalErr = TRUE;
		return ;
	}
	else if((int_len>=9)&&(data_flag==0))
	{
		bCalErr = TRUE;
		return ;
	}*/
	play_float(str,int_len,f_len);
}


#pragma CODE = REMOTE_CALBACK01_CODE
void cal_back_01(void)
{
    if( !bCalPoint )
            CalNum1 = ( U32 )( CalNum1 / 10 );
      else
      {
         switch( cal_point_length )
         {
            case 0x00:
            bCalPoint = FALSE;
            CalNum1 = ( U32 )( CalNum1 / 10 );
            break;

            case 0x01:
            cal_point_length--;
            bCalPoint = FALSE;
            CalNum1 = ( U32 )CalNum1;
            break;

            case 0x02:
            cal_point_length--;
            CalNum1 = ( double )(( U32 )( CalNum1 * 10 ))/ 10.0;
            break;

            case 0x03:
            cal_point_length--;
            CalNum1 = ( double )(( U32 )( CalNum1 * 100 ))/ 100.0;
            break;

            case 0x04:
            cal_point_length--;
            CalNum1 = ( double )(( U32 )( CalNum1 * 1000 ))/ 1000.0;
            break;
         }
      }
}
#pragma CODE = REMOTE_CALBACK02_CODE
void cal_back_02(void)
{
 if( !bCalPoint )
         CalNum2 = ( U32 )( CalNum2 / 10 );
      else
      {
         switch( cal_point_length )
         {
            case 0x00:
            bCalPoint = FALSE;
            CalNum2 = ( U32 )( CalNum2 / 10 );
            break;

            case 0x01:
            cal_point_length--;
            bCalPoint = FALSE;
            CalNum2 = ( U32 )CalNum2;
            break;

            case 0x02:
            cal_point_length--;
            CalNum2 = ( double )(( U32 )( CalNum2 * 10 ))/ 10.0;
            break;

            case 0x03:
            cal_point_length--;
            CalNum2 = ( double )(( U32 )( CalNum2 * 100 ))/ 100.0;
            break;

            case 0x04:
            cal_point_length--;
            CalNum2 = ( double )(( U32 )( CalNum2 * 1000 ))/ 1000.0;
            break;
         }
      }
}
#pragma CODE = REMOTE_CALBACK_CODE
void cal_back(U8 back_flag)
{
   if( back_flag == 1 )
   {
   		SDKLoadCode(REMOTE_CALBACK01_CODE);
		cal_back_01();
   }
   else if(back_flag == 0)
   {
   		SDKLoadCode(REMOTE_CALBACK02_CODE);
     	cal_back_02();
   }
   if(cal_input_length)
         cal_input_length--;
   
}

#pragma CODE = CAL_DOWITH_01_CODE
void cal_dowith_01(void)
{
	if( bCalPoint )
       {
        //  if(( cal_point_length >= 4 )
        //     ||(( cal_point_length + cal_int_length )>= 7 ))
       //      break;
          cal_point_length++;
          cal_input_length++;
          switch( cal_point_length )
          {
             case 0x01:
             CalNum2 = (double)(( U32 )CalNum2 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
             break;

             case 0x02:
             CalNum2 = (double)(( U32 )( CalNum2 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
             break;

             case 0x03:
             CalNum2 = (double)(( U32 )( CalNum2 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
             break;

             case 0x04:
             CalNum2 = (double)(( U32 )( CalNum2 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
             break;
          }
       }
       else
       {
        //  if( cal_int_length >= 7 )
        //     break;
          cal_input_length++;
          cal_int_length++;
          CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
       }
	    M_NUM_RECORD = CalNum2;
}
#pragma CODE = CAL_DOWITH_02_CODE
void cal_dowith_02(void)
{
	if( bCalPoint )
       {
          //if( cal_point_length >= 4 )
      //    if(( cal_point_length >= 4 )
      //       ||(( cal_point_length + cal_int_length )>= 7 ))
     //        break;
          cal_point_length++;
          cal_input_length++;
          switch( cal_point_length )
          {
             case 0x01:
             CalNum1 = (double)(( U32 )CalNum1 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
             break;

             case 0x02:
             CalNum1 = (double)(( U32 )( CalNum1 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
             break;

             case 0x03:
             CalNum1 = (double)(( U32 )( CalNum1 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
             break;

             case 0x04:
             CalNum1 = (double)(( U32 )( CalNum1 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
             break;
          }
       }
       else
       {
         // if( cal_int_length >= 7 )
         //    break;
          cal_input_length++;
          cal_int_length++;
          CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
       }
	   M_NUM_RECORD = CalNum1;
}
#endif  //#if( AP3 == ENABLE )

#endif  //_search_remote_c_
