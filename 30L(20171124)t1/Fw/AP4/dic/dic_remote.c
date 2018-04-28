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
#pragma CODE = DIC_REMOTE_CODE
#pragma DATA = DIC_REMOTE_DATA

#ifndef _search_remote_c_
#define _search_remote_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "dic_search.h"
#include "dic_remote.h"
#include "conf_oid.h"
#include "string.h"
#include "system.h"
#include "dic_ap3.h"
#include "dic_file.h"
#include "oid.h"
#include "com.h"
#include "power.h"             // Power module definition
#include <math.h>
//#include "conf_sio.h"
#include "MP3_api.h"  

#include "conf_kb.h"
#include "keypad.h"
#include "BinaryImage.h"

//_____ M A C R O S ________________________________________________________
 
               Bool           g_b_remote_on ;
//_MEM_TYPE_SLOW_ U8     CACU_FLAG;
 _MEM_TYPE_SLOW_ REMOTE_BIT_NEW  remote_bits;
 _MEM_TYPE_SLOW_ U8     remote_to_mode;
   _MEM_TYPE_SLOW_   REMOTE_TAB           RemoteTable;
   _MEM_TYPE_SLOW_   REMOTE_SPECIAL_TAB   RemoteSpecialTable;
   _MEM_TYPE_SLOW_   U16                  RemoteNumber;
   _MEM_TYPE_SLOW_   U16                  RemoteVersion;
   _MEM_TYPE_SLOW_   REMOTE_SPEECH        RemoteSpeech;

   _MEM_TYPE_SLOW_   U8                    ADBVersion;   


   _MEM_TYPE_SLOW_   U8                   RemoteEncryptKey;
   _MEM_TYPE_SLOW_   U32                  RemoteFlag;
   
//extern   _MEM_TYPE_SLOW_   U16            remote_index_tab[ REMOTE_INDEX_MAX ];
ImageDeclareType(REMOTE_AP4_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(REMOTE_PLAY_CODE);
ImageDeclareType(REMOTE_DIC_CODE);
//_____ D E F I N I T I O N ________________________________________________
//                  U8                      gl_key;
//_MEM_TYPE_SLOW_  U8                       bRemotePress;
_MEM_TYPE_SLOW_   U8                      key_state;
_MEM_TYPE_SLOW_   U32                     RemoteCoorAddress;
//_MEM_TYPE_SLOW_  REMOTE_CODE              RemoteCode;


//_MEM_TYPE_SLOW_   U16                     remote_index_tab[ REMOTE_INDEX_MAX ];

   DicMainInfo          _MEM_TYPE_SLOW_   dic_g_main_info;
 //  DicWordTableItem     _MEM_TYPE_SLOW_   dic_g_word_table_item;
   DicWordTableItem     _MEM_TYPE_SLOW_   dic_word_table[26];
   DicWordInfoItem      _MEM_TYPE_SLOW_   dic_g_word_info_item;
   DicWordContentItem   _MEM_TYPE_SLOW_   dic_g_word_content_item;
   U32               _MEM_TYPE_SLOW_   dicremote_g_table_addr;
   U32               _MEM_TYPE_SLOW_   dicremote_g_base_addr;
   DicActTal		 _MEM_TYPE_SLOW_   dic_act_tal;
   U8				 _MEM_TYPE_SLOW_   dic_voice_played_num;


    U32               _MEM_TYPE_SLOW_   addr_noact;
//   Bool dic_play;
   #define  DIC_Num_InputLine          32

   _MEM_TYPE_SLOW_   U8  dic_g_input_word[ DIC_Num_InputLine + 1 ];
   _MEM_TYPE_SLOW_   U8  dic_g_input_cursor;
//   _MEM_TYPE_SLOW_   U8  dic_result = 0;
//   _MEM_TYPE_SLOW_   U8  dic_flag;


_MEM_TYPE_SLOW_   U16               RemoteValue;
_MEM_TYPE_SLOW_   U32  RemoteBaseAddress ;//               All_Control.Point_Remote

extern _MEM_TYPE_SLOW_  U32             actAddressBak;

//extern unsigned int cal_flag;
//extern unsigned int vol_idx;
//extern const int VolTbl[7];
//extern unsigned int prev_task;

#define  DIS_RESULT_ONLY      0x01
#define  DIS_MEMORY           0x02
#define  PLAY_RESULT          0x04
#define  GET_RESULT           0x08
#define  GET_MEMORY           0x10

Bool  search_remote_index           ( void );
//void  display_float                 ( Float16 result, U8 dis_type );
//void  clear_cal_error               ( void );

void remote_dic(void);
//Bool adb_file_init(void);

void dic_start_init(void);
Bool dic_search_init(void);
Bool dic_search_word( U8 *word );
S8	dic_string_cmp( U8 *str_left, U8 *str_right );
void dic_exit(void);
void dic_voice_play(void);
//Bool player_ap3_start( U32 start_addr, U8 flag );
//void dic_voice_to_play(void);

//extern void swap_code(U16 flag);
//***********************************************



//#the function is deal with the act;
Bool act_do( U32 address, Bool game_act_do_flag )
{
   //U16 temp;
   U16 i;
   unsigned char t=0;
   unsigned char g_buf[128];
   if( address == 0 )
      return FALSE;
//   read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act sizeof(U16));
   read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&g_buf, sizeof(U8)*5);
//   BS(All_Control.Version,0)	= g_buf[buf_offset++];
   BS(Current_Act.Type,0) = g_buf[t++];
   BS(Current_Act.Type,1) = g_buf[t++];
   BS(Current_Act.act_info.speech_info.Num_Voice,0) = g_buf[t++];
   BS(Current_Act.act_info.speech_info.Mode_Play,0) = g_buf[t++];
   BS(Current_Act.Num_Play,0) = g_buf[t++];
 //  Current_Act.Type = UIntLE2BE(Current_Act.Type);
//   Endian_Cast(Current_Act.act_info.Num_Change_Mode);
   //MSB( temp ) = Current_Act.Num_Voice;
   //LSB( temp ) = Current_Act.Mode_Play;
#ifdef TX_DEBUG
 SendString("\r\nADDR:Current_Act=");
 send_hex_u32(address);
  SendString(":");

	for(i=0;i<6;i++){
		send_hex_u8(g_buf[i]);
		SendString(":");
		}
#endif   
   switch( Current_Act.Type )
   {
   
      case Type_Stop:
      if( player_ap3_is_busy())
         player_ap3_stop();
      //break;
      return TRUE;

      case Type_Vol_Up:
      voice_play_number( address );
      case Type_V_Big:
//      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      //break;
      if(vol_idx < 6)
		{
		     vol_idx++;
			 //AudioMP3SetVol(VolTbl[vol_idx]);
		}
      return TRUE;

      case Type_Vol_Down:
      voice_play_number( address );
      case Type_V_Small:
//      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      //break;
      if(vol_idx > 0)
		{
		     vol_idx--;
			// AudioMP3SetVol(VolTbl[vol_idx]);
		}
      return TRUE;

      case Type_Play:
      case Type_Play_Title:
      case Type_play_Caption:
      if( game_act_do_flag )
         return FALSE;
      voice_play_number( address );
      //break;
      return TRUE;


	  
      case Type_play_Reread:
      case Type_play_Repeat:
      voice_play_number( address );
      return TRUE;
    


	  default:
	  return FALSE;
   }
   //return TRUE;
}



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
   
   U16 temp;
   _MEM_TYPE_SLOW_ U32 address;
   _MEM_TYPE_SLOW_ U32 *p = (U32 _MEM_TYPE_SLOW_ *)&RemoteSpeech;
   _MEM_TYPE_SLOW_ REMOTE_SPEECH_DATA speech_temp;
   _MEM_TYPE_SLOW_ U8  j, n, length_remote, speech_length = 15;
   U8 _MEM_TYPE_SLOW_ remote_coor_buf[ 512 ];

   U8 _MEM_TYPE_SLOW_ remote_flag = 0;
   
#if( REMOTE_VERION == REMOTE1 )
   //RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   length_remote = length_remote1_coordiante;
#elif( REMOTE_VERION == REMOTE2 )
   //SpecialFunction _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   length_remote = length_remote2_coordiante;
#endif

   RemoteVersion = 0x01;
///   memset( (char *)&RemoteSpeech, 0, sizeof( REMOTE_SPEECH ));
   bHaveRemote = true;

   if( !RemoteBaseAddress )
   {
      //remote_state = REMOTE_NONE;	
      bHaveRemote = FALSE;
      g_b_remote_on = FALSE;
	  return;
   }
   else
   {
      (void)file_seek((RemoteBaseAddress-4), FS_SEEK_SET);
	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&address, sizeof(U32));
	  if(0xFFFFEEEE==address){
	  	(void)file_seek((RemoteBaseAddress-5), FS_SEEK_SET);
	  	(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ADBVersion, sizeof(U8));
	  	}
	  else{
	  	 ADBVersion=0;
	  	}
	  
#if( REMOTE_DECT == ENABLE )
      (void)file_seek( 4, FS_SEEK_END );

	 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&address, sizeof(U32));
     if(0xFFFFEEEE==address)             
      {
         (void)file_seek( 10, FS_SEEK_END );
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
#ifdef TX_DEBUG
				SendString("\r\nRemoteVersion=");
				send_hex_u16(RemoteVersion);
				SendString("\r\nRemoteFlag=");
				send_hex_u32(RemoteFlag);
				SendString("\r\nRemoteEncryptKey=");
				send_hex_u8(RemoteEncryptKey);
#endif
            }
         }
      
         (void)file_seek( 5, FS_SEEK_END );
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&remote_flag, sizeof(U8));
         if( !( remote_flag & 0x01 ))
         {
            bHaveRemote = FALSE;
            g_b_remote_on = FALSE;
            return;
         }
         (void)read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));
         if( remote_flag & 0x02 )
         {
            (void)read_data_from_nf( RemoteBaseAddress + sizeof( REMOTE_TAB ), ( U8 _MEM_TYPE_SLOW_ *)&RemoteSpecialTable, sizeof( REMOTE_SPECIAL_TAB ));
            RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB ) + sizeof( REMOTE_SPECIAL_TAB );
//            goto search_init_remote;
		      return;
         }
         else
         {
            RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB );
//            goto search_init_remote;
			  return;
         }
      }
#endif   //#if( REMOTE_DECT == ENABLE )




      (void)read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));
#if( REMOTE_VERION == REMOTE2 )
      (void)read_data_from_nf( RemoteBaseAddress + sizeof( REMOTE_TAB ), ( U8 _MEM_TYPE_SLOW_ *)&RemoteSpecialTable, sizeof( REMOTE_SPECIAL_TAB ));
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB ) + sizeof( REMOTE_SPECIAL_TAB );
#elif( REMOTE_VERION == REMOTE1 )
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB );
#endif   //#if( REMOTE_VERION == REMOTE2 )
#if( REMOTE_DECT == ENABLE )
search_init_remote:
#endif   //#if( REMOTE_DECT == ENABLE )
      address = RemoteTable.SpeechAddress + RemoteBaseAddress;
/*      for( i = 0; i < speech_length; i++ )
      {
         *p = address + i * 9;
         read_data_from_nf( *p, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp, sizeof( REMOTE_SPEECH_DATA ));
         if( !speech_temp.SpeechStartAddr
            || !speech_temp.SpeechLength )
            *p = 0;
         p++;
      }
	   if( RemoteVersion >= 3 )
      {
   	   RemoteRecordStartAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+19*9;
   	   RemoteRecordEndAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+22*9;
   	   RemotePlayStartAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+23*9;
   	   RemotePlayEndAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+26*9;

         if( RemoteVersion >= 5 )
   	      RemoteRecordOkStartAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+27*9;
         if( RemoteVersion >= 6 )
   	      RemoteOKRecordStartAddr = RemoteBaseAddress+RemoteTable.SpeechAddress+28*9;
      }
	  */
      //remote_state = REMOTE_DONE;
      bHaveRemote = TRUE;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = TRUE;
      memset( remote_index_tab, 0, REMOTE_INDEX_MAX );
#if( REMOTE_DECT == ENABLE )
      if( remote_flag )
      {
         if( remote_flag & 0x02 )
         {
            length_remote = length_remote2_coordiante;
            goto init_search_remote2;
         }
         else
         {
            length_remote = length_remote1_coordiante;
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
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         return;
      }
      address = RemoteSpecialTable.SpecialFunctionAddress + RemoteBaseAddress;
//      j = RemoteSpecialTable.SpecialFunctionNumber % length_remote;
//      i = RemoteSpecialTable.SpecialFunctionNumber / length_remote;
      {
         SpecialFunction _MEM_TYPE_SLOW_ ptr;
//         while( i )
         {
            for( n = 0; n < RemoteSpecialTable.SpecialFunctionNumber; n++ )
            {

			   read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, ( U16 )sizeof( SpecialFunction ) );
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

#if 1//( REMOTE_MP3 == ENABLE )
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

#if 1//( REMOTE_PASTER == ENABLE )
                  case REMOTE_KEY_PASTER:
                  remote_index_tab[ REMOTE_PASTER_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if 1//( REMOTE_DIC == ENABLE )
                  case REMOTE_KEY_ENDIC:
                  remote_index_tab[ REMOTE_DIC_MODE ] = ptr.index;
                  break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if 1//( REMOTE_REC == ENABLE )
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

				  case REMOTE_KEY_REC_OK_OK:
                  remote_index_tab[ REMOTE_OK] = ptr.index;
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
            	 
               read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, ( U16 )4);
			   
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

#if 1//( REMOTE_MP3 == ENABLE )
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

				  case REMOTE_KEY_REC_OK_OK:
                  remote_index_tab[ REMOTE_OK] = ptr.index;
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
   return FALSE;
#undef nEnd
#endif   //#if( DICHOTOMY == ENABLE )
#else

#if( DICHOTOMY == ENABLE )
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle, number;
   U32 _MEM_TYPE_SLOW_ Address;
   U16 _MEM_TYPE_SLOW_ temp;
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
	  //Address = RemoteCoorAddress +( U32 )13*( U32 )nMiddle;
     // read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.index, sizeof( U16)*6);
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
         play_RMT_speech( Address );

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
   return FALSE;
#undef nEnd
#endif   //#if( DICHOTOMY == ENABLE )


#endif
}

void find_noact_mp3(void)
{
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle, number;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ Temp_coor;
   addr_noact=0;
   if( !RemoteTable.Number )
      return; 
   for( nStart=0;nStart<RemoteTable.Number;nStart++)
   { 
	  Address = RemoteCoorAddress +( U32 )13*( U32 )nStart;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor.index, sizeof( U16)*6);
      if( REMOTE_KEY_MINUS == Temp_coor.function ) 
      {
         Address +=  ( U32 )sizeof( REMOTE_CODE );
//         play_RMT_speech( Address );
		addr_noact=Address;
		return;
      }
   }
}

void remote_task( void )
{
  // if(RemoteBaseAddress>=size_current_ap4)return ;
/*  
   if(Current_Index==INDEX_SPACE){
   	     if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
          return;
   	     dic_g_input_word[dic_g_input_cursor++] = ' ';
         dic_g_input_word[ dic_g_input_cursor ] = 0;
		 return;
   	}
   if(Current_Index==INDEX_LIANZIFU){
   	     if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
          return;
   	     dic_g_input_word[dic_g_input_cursor++] = '-';
         dic_g_input_word[ dic_g_input_cursor ] = 0;
		 return;
   	}
 */  
   if( search_remote_index())
   {
		//remote_ap4();
//		SDKLoadCode(REMOTE_AP4_CODE);

//		if(dic_flag == 1)
//		{
//			dic_flag = 0;
//			dic_play = 0;

//		}
		remote_dic();

	}
   }
//play remote speech
void play_RMT_speech( U32 speech )	
{  
   if( !speech )
      return;
   //speech += RemoteBaseAddress;
   write_media_addr( speech, 1 );
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
}/*
char * str_result(U16 data,char *str)
{   unsigned char temp[10],i;
	unsigned char len=0;
	while(data>0)
	{
		*(str+len++)=data%10+'0';
		data /=10;
	}
	for(i=0;i<len;i++)
		temp[i]=*(str+len-1-i);
	for(i=0;i<len;i++)
		*(str+i)=temp[i];
	*(str+len)=0;
	return str;
}
*/
#if 0
//play number speech
void play_number_speech( U16 hex_data )
{

   _MEM_TYPE_SLOW_ U8 s[ 6 ];
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;
//   ulen = sprintf((char *)s, "%u", hex_data );
	str_hex_u16((char *)s, 0, hex_data );
  // str_result(hex_data,(char *)s);
   ulen = strlen((char *)s);
   temp = 0;
   if( RemoteVersion != 0x01 )
      goto play_num_speech_v2;
   if(RemoteSpeech.SpeechTenThousand
   	   || RemoteSpeech.SpeechHundred 
   	   	|| RemoteSpeech.SpeechThousand
   	   	  || RemoteSpeech.SpeechTen)
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
#endif

//#pragma CODE = REMOTE_DIC_CODE
//void dic_exit(void)
//{ // dic_result = 0;
 //  FSClose(dic_File);
//}
Bool dic_search_init(void)
{
	U8 i;
	U8 _MEM_TYPE_SLOW_     buf[32];
	for(i= 0;i< dic_g_main_info.char_num;i++ )
      {

        // FSSeek( dic_File,dic_g_main_info.word_table_addr + pos_left * 9 );
        // FSRead(dic_File,( U8 *)&buf, 9,&dic_err);
		 (void)file_seek( dic_g_main_info.word_table_addr + i* 9 , FS_SEEK_SET );
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&buf, 9);
		// if(dic_err)
        //     return FALSE;
        if(!buf[0])
			return FALSE;
         dic_word_table[i].first_char = buf[0] + 'a' - 'A';
         BS(dic_word_table[i].addr,0) = buf[1];
         BS(dic_word_table[i].addr,1) = buf[2];
         BS(dic_word_table[i].addr,2) = buf[3];
         BS(dic_word_table[i].addr,3) = buf[4];

         BS(dic_word_table[i].total_count,0) = buf[5];
         BS(dic_word_table[i].total_count,1) = buf[6];
         BS(dic_word_table[i].total_count,2) = buf[7];
         BS(dic_word_table[i].total_count,3) = buf[8];
         if( !dic_word_table[i].total_count||!dic_word_table[i].addr)
            return FALSE;
      }
	return TRUE ;
}
void dic_start_init(void)
{
	U8 i ;
	dic_g_input_cursor =0;
	for(i=0;i<DIC_Num_InputLine;i++)
         dic_g_input_word[i]= 0 ;
}
S8	dic_string_cmp( U8 *str_left, U8 *str_right )
{
   S8 cmp_result;
   while( (*str_left )&&(*str_right ))
   {
   	 if((*str_right >='A')&&(*str_right <='Z'))
	 	*str_right = *str_right -'A' + 'a';
     if(*str_left != *str_right)
         break;
     ++str_left;
     ++str_right;
   }
   cmp_result = *str_left - *str_right;
   return cmp_result;
}
Bool dic_search_word( U8 *word )
{
   U8      first_char = 0,i;
   U32     pos_left;
   U32     pos_right;
   S8      result;
   U8      buf[32];
   U8 result_count = 0;

   first_char =word[ 0 ];
  
   for(i=0;i<26;i++)
   {
   		if(first_char == dic_word_table[i].first_char)
			break;
   }
   if(i>=26)
   	return FALSE;
   pos_left = 0;
   pos_right = dic_word_table[i].total_count;
   do
   {
	  (void)file_seek(dic_word_table[i].addr +(( pos_left + pos_right )/ 2 )* 20, FS_SEEK_SET );
	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)(&dic_g_word_info_item), 20);
      result = dic_string_cmp( word, dic_g_word_info_item.word );
	   if(pos_left==pos_right)
	  	break ;
      if( result > 0 )
         pos_left = ( pos_left + pos_right )/2 + 1;
      else if( result < 0 )
      {
         pos_right = ( pos_left + pos_right )/2  ;
      }
      else
         break;
	 
   }
   while( pos_left <= pos_right);
  // if( !dic_file_seek_and_read(( U8 *)&dic_g_word_content_item, dic_g_word_info_item.content_addr, sizeof( dic_g_word_content_item )))
  //    return FALSE;
  
  if(result != 0)
      return FALSE;
  (void)file_seek( dic_g_word_info_item.content_addr , FS_SEEK_SET );
  #if 1
 	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)(&dic_act_tal),13);
  #elif
	  file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&buf, 13);
	  BS(dic_act_tal.act_type,0) = buf[0];
	  BS(dic_act_tal.act_type,1) = buf[1];
	  BS(dic_act_tal.voi_num,0) = buf[2];
	  BS(dic_act_tal.play_tpe,0) = buf[3];
	  BS(dic_act_tal.play_num,0) = buf[4];
	  BS(dic_act_tal.voi_addr,0) = buf[5];
	  BS(dic_act_tal.voi_addr,1) = buf[6];
	  BS(dic_act_tal.voi_addr,2) = buf[7];
	  BS(dic_act_tal.voi_addr,3) = buf[8];

	  BS(dic_act_tal.voi_size,0) = buf[9];
	  BS(dic_act_tal.voi_size,1) = buf[10];
	  BS(dic_act_tal.voi_size,2) = buf[11];
	  BS(dic_act_tal.voi_size,3) = buf[12];
  #endif
   return TRUE;
}

void remote_dic(void)
{
 	if(RemoteValue == REMOTE_KEY_ENDIC){
		init_array_v();
		return;
 		}
	if(RemoteValue == REMOTE_KEY_EXDIC){
		bRmoteExitAp4 = TRUE;
		if(prev_task==C_MP3){
			remote_to_mode = PLAYER_APPLI;
			}
		else{
			remote_to_mode = AP3_APPLI;
			}
		return;
 		}
    if((RemoteValue == REMOTE_KEY_AP4) )
      {
		 bRmoteExitAp4 = TRUE;
         remote_to_mode = AP3_APPLI;
         return;
     }
	if(RemoteValue == REMOTE_KEY_GO_MP3)
	//if(RemoteValue == 41)
	{
		 bRmoteExitAp4 = TRUE;
         remote_to_mode = PLAYER_APPLI;
         return;
	}
   	if(( RemoteValue >= REMOTE_KEY_A )&&( RemoteValue <= REMOTE_KEY_Z ))       
      {
         if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
          return;
         dic_g_input_word[dic_g_input_cursor++] = 'a' + ( RemoteValue - REMOTE_KEY_A);
         dic_g_input_word[ dic_g_input_cursor ] = 0;
         return ;
      }  
	if( RemoteValue == REMOTE_SPACE ){
         if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
          return;
         dic_g_input_word[dic_g_input_cursor++] = ' ';
         dic_g_input_word[ dic_g_input_cursor ] = 0;
         return ;
      }  
	if( RemoteValue == REMOTE_HENG ){
         if( dic_g_input_cursor == DIC_Num_InputLine - 1 )
          return;
         dic_g_input_word[dic_g_input_cursor++] = '-';
         dic_g_input_word[ dic_g_input_cursor ] = 0;
         return ;
      }
      if( RemoteValue == REMOTE_KEY_CANCLE )
      {      
         dic_start_init();
         return ;
      }
	  if( RemoteValue == REMOTE_KEY_BACK)
      {      
         if( dic_g_input_cursor == 0 )
         {
            dic_g_input_word[0] = 0;
             return ;
         }
         dic_g_input_cursor --;
         dic_g_input_word[dic_g_input_cursor]= 0 ;
         return ;
      }
      if(RemoteValue == REMOTE_KEY_OK)
      {
      	 init_array_v();
		 if(dic_g_input_cursor == 0){
		 	(void)act_do(actAddressBak,FALSE);
		 	 state_search_to_play();
			 return;
		 	}
#ifdef TX_DEBUG
		SendString("\r\nword=");
		SendString((char *)dic_g_input_word);
#endif			 
         if(!dic_search_word(dic_g_input_word))
         {
            while(dic_g_input_cursor)
               dic_g_input_word[dic_g_input_cursor--] = 0 ;
            dic_g_input_word[dic_g_input_cursor] = 0 ;
			//play_RMT_speech(addr_noact);
			tishi_mp3(FLAG_ERROR);
            return;
         }
         while(dic_g_input_cursor)
            dic_g_input_word[dic_g_input_cursor--] = 0 ;
         dic_g_input_word[dic_g_input_cursor] = 0 ;
#ifdef TX_DEBUG
		SendString("\r\nAVersion:addr=");
		send_hex_u8(ADBVersion);
		SendString(":");
		send_hex_u32(dic_g_word_info_item.content_addr);
#endif		 
         dic_voice_play();
		 dic_start_init();
		 (void)dic_search_init();
      }
}
void dic_voice_play(void)
{
#if 0
	act_do(dic_g_word_info_item.content_addr,FALSE);
#else  
	U32 temp_addr;
	U16 temp_len;
/*
	if(ADBVersion==1){
		(void)act_do(dic_g_word_info_item.content_addr,FALSE);
		}
	else{
		 (void)file_seek((dic_g_word_info_item.content_addr+9), FS_SEEK_SET);
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_addr,sizeof(U32));
		 write_media_addr(temp_addr,0);
		}
	*/
///*
	if(ADBVersion==0){
		write_media_addr(dic_g_word_info_item.content_addr,0);
		}
	else if(ADBVersion==1){
		(void)act_do(dic_g_word_info_item.content_addr,FALSE);
		}
	else{
		 (void)file_seek((dic_g_word_info_item.content_addr+9), FS_SEEK_SET);
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_len,sizeof(U16));
		 temp_addr = dic_g_word_info_item.content_addr+11 + temp_len;
		 (void)file_seek(temp_addr, FS_SEEK_SET);
		 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp_addr,sizeof(U32));
		 //write_media_addr(temp_addr,0);
#ifdef TX_DEBUG
		SendString("\r\naddr=");
		send_hex_u32(temp_addr);
#endif			 
		 actAddressBak = temp_addr;
		 (void)act_do(temp_addr,FALSE);
		}
//*/	
#endif  

}
/*
void dic_voice_to_play(void)
{
	if(dic_voice_played_num < dic_act_tal.play_num)	
	{
		#if 1
			player_ap3_start( (dic_g_word_info_item.content_addr+5)+ dic_voice_played_num * 9, 0 );
 		#elif
			player_ap3_start( dic_act_tal.voi_addr + dic_voice_played_num * 9, 0 );
 		#endif
		dic_voice_played_num ++ ;
	}
	else
		{
			dic_voice_played_num = 0 ;
//			dic_play = 0 ;
		}
	
}
*/



#endif  //_search_remote_c_
