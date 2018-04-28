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


#ifndef _search_remote_c_
#define _search_remote_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "search.h"
#include "search_game.h"
#include "search_remote.h"
#include "conf\conf_oid.h"
//#include "modules\debug\debug_sio.h"
#include "string.h"
#include "lib_mcu\timer\timer_drv.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "mmi\ap3\mmi_ap3.h"
#include "mmi\player\srv_player.h"
#include "mmi\shared\com_evt.h"
#include "modules\file_system\file.h"
#include "drivers\oid\oid.h"
#include "sssmp3_regs.h"
#include "modules\power\power.h"             // Power module definition
#include <intrins.h>
#include <math.h>

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________
//                  U8                      gl_key;
//_MEM_TYPE_SLOW_  U8                       bRemotePress;
_MEM_TYPE_SLOW_   U8                      key_state;
_MEM_TYPE_SLOW_   U32                     RemoteCoorAddress;
//_MEM_TYPE_SLOW_  REMOTE_CODE              RemoteCode;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
_MEM_TYPE_SLOW_   U16                     remote_index_tab[ REMOTE_INDEX_MAX ];
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

#define  CAL_STATE_NUM1             0
#define  CAL_STATE_NUM2             1

_MEM_TYPE_SLOW_   U16               RemoteValue;
_MEM_TYPE_SLOW_   Float16           CalNum1;
_MEM_TYPE_SLOW_   Float16           CalNum2;
_MEM_TYPE_SLOW_   U16               CalOP;
_MEM_TYPE_SLOW_   U8                cal_state;
_MEM_TYPE_SLOW_   U8                cal_input_length;
_MEM_TYPE_SLOW_   U8                cal_point_length;
_MEM_TYPE_SLOW_   U8                cal_int_length;

#define  DIS_RESULT_ONLY      0x01
#define  DIS_MEMORY           0x02
#define  PLAY_RESULT          0x04
#define  GET_RESULT           0x08
#define  GET_MEMORY           0x10

//_____ D E C L A R A T I O N ______________________________________________

void  play_number_speech            ( U16 hex_data );
void  calculate_old                 ( void );
void  play_float                    ( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length );
Bool  search_remote_index           ( void );
void  display_float                 ( Float16 result, U8 dis_type );
void  clear_cal_error               ( void );

//***********************************************
//remote task initial
void remote_task_init( void )
{
   //bRemotePress = FALSE;
   key_state = KEY_STATE_NULL;
   //remote_state = REMOTE_START;
}


void init_remote( void )
{
   _MEM_TYPE_SLOW_ U16 i;
   _MEM_TYPE_SLOW_ U32 address;
   _MEM_TYPE_SLOW_ U32 *p = &RemoteSpeech;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   _MEM_TYPE_SLOW_ U8  j, n, length_remote, speech_length = 15;
   U8 _MEM_TYPE_SLOW_ remote_coor_buf[ 512 ];
#if( REMOTE_DECT == ENABLE )
   U8 _MEM_TYPE_SLOW_ remote_flag = 0;
#endif   //#if( REMOTE_DECT == ENABLE )
#if( REMOTE_VERION == REMOTE1 )
   //RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   length_remote = length_remote1_coordiante;
#elif( REMOTE_VERION == REMOTE2 )
   //SpecialFunction _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   length_remote = length_remote2_coordiante;
#endif
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   RemoteVersion = 0x01;
   memset( &RemoteSpeech, 0, sizeof( REMOTE_SPEECH ));
   if( !RemoteBaseAddress )
   {
      //remote_state = REMOTE_NONE;	
      bHaveRemote = FALSE;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   }
   else
   {
#if( REMOTE_DECT == ENABLE )
      file_seek( 4, FS_SEEK_END );
      if(( 0xFF == ( U8 )file_getc())
         &&( 0xFF == ( U8 )file_getc())
            &&( 0xEE == ( U8 )file_getc())
               &&( 0xEE == ( U8 )file_getc()))
      {
         U16 temp;
         file_seek( 10, FS_SEEK_END );
         MSB( temp ) = ( U8 )file_getc();
         LSB( temp ) = ( U8 )file_getc();
         if( !MSB( temp )
            &&( LSB( temp )>= 0x02 ))
         {
            RemoteVersion = temp;
            speech_length = 19;
            if( RemoteVersion >= 4 )
            {
               file_seek( 11, FS_SEEK_END );
               RemoteEncryptKey = ( U8 )file_getc();
               file_seek( 8, FS_SEEK_END );
               MSB0( RemoteFlag ) = ( U8 )file_getc();
               MSB1( RemoteFlag ) = ( U8 )file_getc();
               MSB2( RemoteFlag ) = ( U8 )file_getc();
               MSB3( RemoteFlag ) = ( U8 )file_getc();
            }
         }
      
         file_seek( 5, FS_SEEK_END );
         remote_flag = ( U8 )file_getc();
         if( !( remote_flag & 0x01 ))
         {
            bHaveRemote = FALSE;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            return;
         }
         read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));
         if( remote_flag & 0x02 )
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
#endif   //#if( REMOTE_DECT == ENABLE )

      read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));
#if( REMOTE_VERION == REMOTE2 )
      read_data_from_nf( RemoteBaseAddress + sizeof( REMOTE_TAB ), ( U8 _MEM_TYPE_SLOW_ *)&RemoteSpecialTable, sizeof( REMOTE_SPECIAL_TAB ));
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB ) + sizeof( REMOTE_SPECIAL_TAB );
#elif( REMOTE_VERION == REMOTE1 )
      RemoteCoorAddress = RemoteBaseAddress + sizeof( REMOTE_TAB );
#endif   //#if( REMOTE_VERION == REMOTE2 )
#if( REMOTE_DECT == ENABLE )
search_init_remote:
#endif   //#if( REMOTE_DECT == ENABLE )
      address = RemoteTable.SpeechAddress + RemoteBaseAddress;
      for( i = 0; i < speech_length; i++ )
      {
         *p = address + i * 9;
         p++;
      }
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
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         return;
      }
      address = RemoteSpecialTable.SpecialFunctionAddress + RemoteBaseAddress;
      j = RemoteSpecialTable.SpecialFunctionNumber % length_remote;
      i = RemoteSpecialTable.SpecialFunctionNumber / length_remote;
      {
         SpecialFunction _MEM_TYPE_SLOW_ *ptr = &remote_coor_buf;
         while( i )
         {
            read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&remote_coor_buf, ( U16 )sizeof( SpecialFunction )*( U16 )length_remote );
            for( n = 0; n < length_remote; n++ )
            {
               switch( ptr[ n ].function )
               {
                  case REMOTE_KEY_VINC:
                  remote_index_tab[ REMOTE_VOL_UP ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_VDEC:
                  remote_index_tab[ REMOTE_VOL_DOWN ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_POWER:
                  remote_index_tab[ REMOTE_POWER_OFF ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_AP4:
                  remote_index_tab[ REMOTE_AP4_MODE ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_ENCAL:
                  remote_index_tab[ REMOTE_CAL_MODE ] = ptr[ n ].index;
                  break;

#if( REMOTE_MP3 == ENABLE )
                  case REMOTE_KEY_EXIT_MP3:
                  remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_PLAY_PAUSE:
                  remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_MP3_PREV:
                  remote_index_tab[ REMOTE_MP3_PREV ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_MP3_NEXT:
                  remote_index_tab[ REMOTE_MP3_NEXT ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_GO_MP3:
                  remote_index_tab[ REMOTE_MP3_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
                  case REMOTE_KEY_PASTER:
                  remote_index_tab[ REMOTE_PASTER_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
                  case REMOTE_KEY_ENDIC:
                  remote_index_tab[ REMOTE_DIC_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
                  case REMOTE_KEY_REC:
                  remote_index_tab[ REMOTE_REC_MODE ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_REC_START:
                  remote_index_tab[ REMOTE_REC_START ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_REC_PLAY:
                  remote_index_tab[ REMOTE_REC_PLAY ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_REC == ENABLE )
               }
            }
            i--;
            address += ( U32 )sizeof( SpecialFunction )* length_remote;
         }
      
         read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&remote_coor_buf, ( U16 )sizeof( SpecialFunction )*( U16 )j );
         for( n = 0; n < j; n++ )
         {
            switch( ptr[ n ].function )
            {
               case REMOTE_KEY_VINC:
               remote_index_tab[ REMOTE_VOL_UP ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_VDEC:
               remote_index_tab[ REMOTE_VOL_DOWN ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_POWER:
               remote_index_tab[ REMOTE_POWER_OFF ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_AP4:
               remote_index_tab[ REMOTE_AP4_MODE ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_ENCAL:
               remote_index_tab[ REMOTE_CAL_MODE ] = ptr[ n ].index;
               break;

#if( REMOTE_MP3 == ENABLE )
               case REMOTE_KEY_EXIT_MP3:
               remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_PLAY_PAUSE:
               remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_MP3_PREV:
               remote_index_tab[ REMOTE_MP3_PREV ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_MP3_NEXT:
               remote_index_tab[ REMOTE_MP3_NEXT ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_GO_MP3:
               remote_index_tab[ REMOTE_MP3_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
               case REMOTE_KEY_PASTER:
               remote_index_tab[ REMOTE_PASTER_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
               case REMOTE_KEY_ENDIC:
               remote_index_tab[ REMOTE_DIC_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
               case REMOTE_KEY_REC:
               remote_index_tab[ REMOTE_REC_MODE ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_REC_START:
               remote_index_tab[ REMOTE_REC_START ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_REC_PLAY:
               remote_index_tab[ REMOTE_REC_PLAY ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_REC_STOP:
               remote_index_tab[ REMOTE_REC_STOP ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_REC == ENABLE )
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
      j = RemoteTable.Number % length_remote;
      i = RemoteTable.Number / length_remote;
      {
         RemoteCoor _MEM_TYPE_SLOW_ *ptr = &remote_coor_buf;
         while( i )
         {
            read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&remote_coor_buf, ( U16 )sizeof( RemoteCoor )*( U16 )length_remote );
            for( n = 0; n < length_remote; n++ )
            {
               switch( ptr[ n ].function )
               {
                  case REMOTE_KEY_VINC:
                  remote_index_tab[ REMOTE_VOL_UP ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_VDEC:
                  remote_index_tab[ REMOTE_VOL_DOWN ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_POWER:
                  remote_index_tab[ REMOTE_POWER_OFF ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_AP4:
                  remote_index_tab[ REMOTE_AP4_MODE ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_ENCAL:
                  remote_index_tab[ REMOTE_CAL_MODE ] = ptr[ n ].index;
                  break;

#if( REMOTE_MP3 == ENABLE )
                  case REMOTE_KEY_EXIT_MP3:
                  remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_PLAY_PAUSE:
                  remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_MP3_PREV:
                  remote_index_tab[ REMOTE_MP3_PREV ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_MP3_NEXT:
                  remote_index_tab[ REMOTE_MP3_NEXT ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_GO_MP3:
                  remote_index_tab[ REMOTE_MP3_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
                  case REMOTE_KEY_PASTER:
                  remote_index_tab[ REMOTE_PASTER_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
                  case REMOTE_KEY_ENDIC:
                  remote_index_tab[ REMOTE_DIC_MODE ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
                  case REMOTE_KEY_REC:
                  remote_index_tab[ REMOTE_REC_MODE ] = ptr[ n ].index;
                  break;
                  
                  case REMOTE_KEY_REC_START:
                  remote_index_tab[ REMOTE_REC_START ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_REC_PLAY:
                  remote_index_tab[ REMOTE_REC_PLAY ] = ptr[ n ].index;
                  break;

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr[ n ].index;
                  break;
#endif   //#if( REMOTE_REC == ENABLE )
               }
            }
            i--;
            address += ( U32 )sizeof( RemoteCoor )* length_remote;
         }
         read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&remote_coor_buf, ( U16 )sizeof( RemoteCoor )*( U16 )j );
         for( n = 0; n < j; n++ )
         {
            switch( ptr[ n ].function )
            {
               case REMOTE_KEY_VINC:
               remote_index_tab[ REMOTE_VOL_UP ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_VDEC:
               remote_index_tab[ REMOTE_VOL_DOWN ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_POWER:
               remote_index_tab[ REMOTE_POWER_OFF ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_AP4:
               remote_index_tab[ REMOTE_AP4_MODE ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_ENCAL:
               remote_index_tab[ REMOTE_CAL_MODE ] = ptr[ n ].index;
               break;

#if( REMOTE_MP3 == ENABLE )
               case REMOTE_KEY_EXIT_MP3:
               remote_index_tab[ REMOTE_EXIT_MP3 ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_PLAY_PAUSE:
               remote_index_tab[ REMOTE_MP3_PLAY_PAUSE ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_MP3_PREV:
               remote_index_tab[ REMOTE_MP3_PREV ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_MP3_NEXT:
               remote_index_tab[ REMOTE_MP3_NEXT ] = ptr[ n ].index;
               break;
               
               case REMOTE_KEY_GO_MP3:
               remote_index_tab[ REMOTE_MP3_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
               case REMOTE_KEY_PASTER:
               remote_index_tab[ REMOTE_PASTER_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
               case REMOTE_KEY_ENDIC:
               remote_index_tab[ REMOTE_DIC_MODE ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_DIC == ENABLE )

#if( REMOTE_REC == ENABLE )
               case REMOTE_KEY_REC:
               remote_index_tab[ REMOTE_REC_MODE ] = ptr[ n ].index;
               break;
               
               case REMOTE_KEY_REC_START:
               remote_index_tab[ REMOTE_REC_START ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_REC_PLAY:
               remote_index_tab[ REMOTE_REC_PLAY ] = ptr[ n ].index;
               break;

               case REMOTE_KEY_REC_STOP:
               remote_index_tab[ REMOTE_REC_STOP ] = ptr[ n ].index;
               break;
#endif   //#if( REMOTE_REC == ENABLE )
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
      Address = RemoteCoorAddress +( U32 )sizeof( RemoteCoor )*( U32 )nMiddle;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( RemoteCoor ));
      if( Current_Index == Temp_coor.index ) 
      {
         RemoteValue = Temp_coor.function;
         if( bInCal )
         {
            if((( RemoteValue >= REMOTE_KEY_0 )&&( RemoteValue <= REMOTE_KEY_9 ))
               ||( RemoteValue == REMOTE_KEY_GO_MP3 )
                  ||( RemoteValue == REMOTE_CAL_BACK )||( RemoteValue == REMOTE_KEY_DOT )
                     ||(( RemoteValue >= REMOTE_KEY_ENCAL )&&( RemoteValue <= REMOTE_KEY_EXCAL ))
                        ||(( RemoteValue >= REMOTE_KEY_VINC )&&( RemoteValue <= REMOTE_KEY_POWER ))
                           ||(( RemoteValue >= REMOTE_KEY_REC )&&( RemoteValue <= REMOTE_KEY_AP4 ))
                              ||(( RemoteValue >= REMOTE_LEFT_PARENTHESIS )&&( RemoteValue <= REMOTE_CAL_M_CLEAR )))
            {
               Address +=  ( U32 )sizeof( REMOTE_CODE );
               play_RMT_speech( Address );
            }
         }
         else
         {
            Address +=  ( U32 )sizeof( REMOTE_CODE );
            play_RMT_speech( Address );
         }
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
   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
      act_do( News_Go.Action_NoVoice, 0 );
   return FALSE;
#undef nEnd

#elif( DICHOTOMY == DISABLE )
   U8  _MEM_TYPE_SLOW_ j, n;
   U16 _MEM_TYPE_SLOW_ i;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote1_coordiante ];
   if( !RemoteTable.Number )
      return FALSE;
   i = RemoteTable.Number / length_remote1_coordiante;
   j = RemoteTable.Number % length_remote1_coordiante;
   Address = RemoteCoorAddress;
   /*if( i == 0 )
   {
      read_data_from_nf( Address, ( U8 *)&remote_coor_arry, ( U16 )sizeof( RemoteCoor )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         if( Current_Index == remote_coor_arry[ n ].index )
         {
            RemoteValue = remote_coor_arry[ n ].function;
            Address +=  ( U32 )n *( U32 )sizeof( RemoteCoor ) + ( U32 )sizeof( REMOTE_CODE );
            play_RMT_speech( Address );
            bRemotePress = TRUE;
            state_search_to_play();
            return;
         }
      }
   }
   else*/
   //{
      while( i )
      {
         read_data_from_nf( Address, ( U8 *)&remote_coor_arry, ( U16 )sizeof( RemoteCoor )*( U16 )length_remote1_coordiante );
         for( n = 0; n < length_remote1_coordiante; n++ )
         {
            if( Current_Index == remote_coor_arry[ n ].index )
            {
               RemoteValue = remote_coor_arry[ n ].function;
               //bRemotePress = TRUE;
               //state_search_to_play();
               if( bInCal )
               {
                  if((( RemoteValue >= REMOTE_KEY_0 )&&( RemoteValue <= REMOTE_KEY_9 ))
                     ||( RemoteValue == REMOTE_KEY_GO_MP3 )
                        ||( RemoteValue == REMOTE_CAL_BACK )||( RemoteValue == REMOTE_KEY_DOT )
                           ||(( RemoteValue >= REMOTE_KEY_ENCAL )&&( RemoteValue <= REMOTE_KEY_EXCAL ))
                              ||(( RemoteValue >= REMOTE_KEY_VINC )&&( RemoteValue <= REMOTE_KEY_POWER ))
                                 ||(( RemoteValue >= REMOTE_KEY_REC )&&( RemoteValue <= REMOTE_KEY_AP4 ))
                                    ||(( RemoteValue >= REMOTE_LEFT_PARENTHESIS )&&( RemoteValue <= REMOTE_CAL_M_CLEAR )))
                  {
                     Address +=  ( U32 )n *( U32 )sizeof( RemoteCoor ) + ( U32 )sizeof( REMOTE_CODE );
                     play_RMT_speech( Address );
                  }
               }
               else
               {
                  Address +=  ( U32 )n *( U32 )sizeof( RemoteCoor ) + ( U32 )sizeof( REMOTE_CODE );
                  play_RMT_speech( Address );
               }
               return TRUE;
            }
         }
         i--;
         Address += ( U32 )sizeof( RemoteCoor )* length_remote1_coordiante;
      }
      //if( j == 0 )
         //return;
      read_data_from_nf( Address, ( U8 *)&remote_coor_arry, ( U16 )sizeof( RemoteCoor )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         if( Current_Index == remote_coor_arry[ n ].index )
         {
            RemoteValue = remote_coor_arry[ n ].function;
            //bRemotePress = TRUE;
            //state_search_to_play();
            if( bInCal )
            {
               if((( RemoteValue >= REMOTE_KEY_0 )&&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_KEY_GO_MP3 )
                     ||( RemoteValue == REMOTE_CAL_BACK )||( RemoteValue == REMOTE_KEY_DOT )
                        ||(( RemoteValue >= REMOTE_KEY_ENCAL )&&( RemoteValue <= REMOTE_KEY_EXCAL ))
                           ||(( RemoteValue >= REMOTE_KEY_VINC )&&( RemoteValue <= REMOTE_KEY_POWER ))
                              ||(( RemoteValue >= REMOTE_KEY_REC )&&( RemoteValue <= REMOTE_KEY_AP4 ))
                                 ||(( RemoteValue >= REMOTE_LEFT_PARENTHESIS )&&( RemoteValue <= REMOTE_CAL_M_CLEAR )))
               {
                  Address +=  ( U32 )n *( U32 )sizeof( RemoteCoor ) + ( U32 )sizeof( REMOTE_CODE );
                  play_RMT_speech( Address );
               }
            }
            else
            {
               Address +=  ( U32 )n *( U32 )sizeof( RemoteCoor ) + ( U32 )sizeof( REMOTE_CODE );
               play_RMT_speech( Address );
            }
            return TRUE;
         }
      }
   //}
   //play_RMT_speech( RemoteSpeech.SpeechNull );
   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
      act_do( News_Go.Action_NoVoice, 0 );
   //state_search_to_play();
   return FALSE;
#endif   //#if( DICHOTOMY == ENABLE )
}


//Scan remote
void remote_task( void )
{
   if( search_remote_index())
   //if( bRemotePress )
   {
      //bRmoteExitAp4 = FALSE;
      //bRemotePress = FALSE;
#if 0
      if(( RemoteValue >= REMOTE_KEY_A )&&( RemoteValue <= REMOTE_KEY_Z ))
      {
         if( bGameClicked )
         {
            CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_A )+ 'a';
            gl_key_press = TRUE;
         }
         return;
      }
      if( RemoteValue == REMOTE_KEY_CANCLE )
      {
         if( bGameClicked )
         {
            CurrentKey = RM_CANCLE;
            gl_key_press = TRUE;
            return;
         }
      }
      if( RemoteValue == REMOTE_KEY_BACK )
      {
         if( bGameClicked )
         {
            CurrentKey = RM_BACK;
            gl_key_press = TRUE;
         }
         return;
      }
      if( RemoteValue == REMOTE_KEY_DOT )
      {
         if( bGameClicked )
         {
            CurrentKey = RM_DOT;
            gl_key_press = TRUE;
            return;
         }
         if( !bInCal )
            return;
      }
      /*if( bInDictionary )
      {
         if( RemoteValue == REMOTE_KEY_OK )
         {
            //CurrentKey = RM_ENTER;
            gl_key_press = TRUE;
         }
         bRemotePress = FALSE;
         return;
      }*/
#endif
      if( RemoteValue == REMOTE_KEY_VDEC )
      {
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         //bRemotePress = FALSE;
         return;
      }
      if( RemoteValue == REMOTE_KEY_VINC )
      {
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         //bRemotePress = FALSE;
         return;
      }
      if( RemoteValue == REMOTE_KEY_POWER )
      {
         if( !power_is_vbus_present())
            mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
         bSpeechEnd = FALSE;
         //bRemotePress = FALSE;
         return;
      }
      if( !bInCal &&( RemoteValue == REMOTE_KEY_AP4 ))
      {
         bRmoteExitAp4 = FALSE;
         return;
      }
#if( REMOTE_DIC == ENABLE )
      //if( bInEbookMode )
      {
         if( RemoteValue == REMOTE_KEY_ENDIC )
         {
            //bInCal = FALSE;
            //enter_dic();
            //bRemotePress = FALSE;
#if( DIC == ENABLE )
            fat_clusterlist_save();
            //g_b_remote_on = TRUE;
            //bInDictionary = TRUE;
            /*bRmoteExitAp4 = TRUE;
            bInEbookMode = TRUE;*/
            remote_bits.value |= 0x0808;
            remote_to_mode = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
            return;
         }
         if( RemoteValue == REMOTE_KEY_EXDIC )
         {
            /*bRmoteExitAp4 = FALSE;
            bInEbookMode = FALSE;*/
#if( DIC == ENABLE )
            remote_bits.value &= ~0x0808;
#endif   //#if( DIC == ENABLE )
            return;
         }
      }
#endif   //#if( REMOTE_DIC == ENABLE )
#if( REMOTE_REC == ENABLE )
      if( RemoteValue == REMOTE_KEY_REC )
      {
         return;
      }
#endif   //#if( REMOTE_REC == ENABLE )
#if( REMOTE_PASTER == ENABLE )
      if( RemoteValue == REMOTE_KEY_PASTER )
      {
         fat_clusterlist_save();
         //g_b_remote_on = TRUE;
         bRmoteExitAp4 = TRUE;
         remote_to_mode = PASTER_APPLI;
         return;
      }
#endif   //#if( REMOTE_PASTER == ENABLE )
#if( REMOTE_MP3 == ENABLE )
      if( RemoteValue == REMOTE_KEY_GO_MP3 )
      {
         //bInCal = FALSE;
         fat_clusterlist_save();
         //g_b_remote_on = TRUE;
         bRmoteExitAp4 = TRUE;
         remote_to_mode = PLAYER_APPLI;
         return;
      }
#endif   //#if( REMOTE_MP3 == ENABLE )
#if 1
      if( RemoteValue == REMOTE_KEY_ENCAL )
      {
         //enter calculation state
         clear_cal_error();
         //bRemotePress = FALSE;
         return;
      }
      if( bInCal )
      {
         if(( RemoteValue == REMOTE_KEY_EXCAL )
            ||( RemoteValue == REMOTE_KEY_AP4 ))
         {
            /*bRmoteExitAp4 = FALSE;
            bInCal = FALSE;*/
            remote_bits.value &= ~0x0880;
            //bRemotePress = FALSE;
            return;
         }
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
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
                  CalNum1 = 0.0;
                  bCalClear = FALSE;
               }
               if( RemoteValue == REMOTE_CAL_BACK )
               {
                  //if( CalNum1 )
                  if( _chkfloat_( CalNum1 )== 0 )
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
                           CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10 ))/ 10.0;
                           break;

                           case 0x03:
                           cal_point_length--;
                           CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100 ))/ 100.0;
                           break;

                           case 0x04:
                           cal_point_length--;
                           CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000 ))/ 1000.0;
                           break;
                        }
                     }
                  }
                  else
                  {
                     if( bCalPoint )
                     {
                        cal_point_length = 0;
                        bCalPoint = FALSE;
                     }
                  }
                  if( cal_input_length )
                     cal_input_length--;
                  break;
               }
               //if( cal_input_length >= 11 )
               if( cal_input_length >= 7 )
                  break;
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }
               if( bCalPoint )
               {
                  //if( cal_point_length >= 4 )
                  if(( cal_point_length >= 4 )
                     ||(( cal_point_length + cal_int_length )>= 7 ))
                     break;
                  cal_point_length++;
                  cal_input_length++;
                  switch( cal_point_length )
                  {
                     case 0x01:
                     //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10.0;
                     CalNum1 = (( U32 )CalNum1 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
                     break;

                     case 0x02:
                     //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100.0;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
                     break;

                     case 0x03:
                     //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 1000.0;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
                     break;

                     case 0x04:
                     //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10000.0;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
                     break;
                  }
               }
               else
               {
                  if( cal_int_length >= 7 )
                     break;
                  cal_input_length++;
                  cal_int_length++;
                  CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
               }
               break;
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
               CalOP = RemoteValue;
               CalNum2 = 0.0;
               cal_state = CAL_STATE_NUM2;
               break;

               case REMOTE_KEY_EQUAL:
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
               calculate_old();
               CalOP = REMOTE_KEY_NULL;
               bCalClear = TRUE;
               //display_float( CalNum1 );
               display_float( CalNum1, PLAY_RESULT | GET_RESULT );
               if( bCalErr )
               {
                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                     act_do( News_Go.Action_NoVoice, 0 );
                  clear_cal_error();
                  break;
               }
               //play_float( CalNum1 );
               break;
            }
            break;

            case CAL_STATE_NUM2:
            if((( RemoteValue >= REMOTE_KEY_0 )
               &&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_CAL_BACK )
                     ||( RemoteValue == REMOTE_KEY_DOT ))
            {
               if( RemoteValue == REMOTE_CAL_BACK )
               {
                  //if( CalNum2 )
                  if( _chkfloat_( CalNum2 )== 0 )
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
                           CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10 ))/ 10.0;
                           break;

                           case 0x03:
                           cal_point_length--;
                           CalNum2 = ( Float16 )(( U32 )( CalNum2 * 100 ))/ 100.0;
                           break;

                           case 0x04:
                           cal_point_length--;
                           CalNum2 = ( Float16 )(( U32 )( CalNum2 * 1000 ))/ 1000.0;
                           break;
                        }
                     }
                  }
                  else
                  {
                     if( bCalPoint )
                     {
                        cal_point_length = 0;
                        bCalPoint = FALSE;
                     }
                  }
                  if( cal_input_length )
                     cal_input_length--;
                  bNewCal = TRUE;
                  break;
               }
               //if( cal_input_length >= 11 )
               if( cal_input_length >= 7 )
                  break;
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }
               if( bCalPoint )
               {
                  //if( cal_point_length >= 4 )
                  if(( cal_point_length >= 4 )
                     ||(( cal_point_length + cal_int_length )>= 7 ))
                     break;
                  cal_point_length++;
                  cal_input_length++;
                  switch( cal_point_length )
                  {
                     case 0x01:
                     //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10.0;
                     CalNum2 = ( Float16 )(( U32 )CalNum2 * 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10.0;
                     break;

                     case 0x02:
                     //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100.0;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100.0;
                     break;

                     case 0x03:
                     //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 1000.0;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 100 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 1000.0;
                     break;

                     case 0x04:
                     //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 10000.0;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 1000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 10000.0;
                     break;
                  }
               }
               else
               {
                  if( cal_int_length >= 7 )
                     break;
                  cal_input_length++;
                  cal_int_length++;
                  CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
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
                  calculate_old();
                  //display_float( CalNum1 );
                  display_float( CalNum1, GET_RESULT );
                  if( bCalErr )
                  {
                     if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                        act_do( News_Go.Action_NoVoice, 0 );
                     clear_cal_error();
                     break;
                  }
               }
               bNewCal = FALSE;
               CalNum2 = 0.0;
               CalOP = RemoteValue;
               break;

               case REMOTE_KEY_EQUAL:
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
               calculate_old();
               CalOP = REMOTE_KEY_NULL;
               bCalClear = TRUE;
               cal_state = CAL_STATE_NUM1;
               //display_float( CalNum1 );
               display_float( CalNum1, PLAY_RESULT | GET_RESULT );
               if( bCalErr )
               {
                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                     act_do( News_Go.Action_NoVoice, 0 );
                  clear_cal_error();
                  break;
               }
               //play_float( CalNum1 );
               break;
            }
            break;
         }
         //bRemotePress = FALSE;
         return;
      }
#endif
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
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if(( RemoteValue >= REMOTE_KEY_AP3_21 )
                  &&( RemoteValue <= REMOTE_KEY_AP3_99 ))
         {
            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_21 + 21;
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if(( RemoteValue >= REMOTE_KEY_AP3_100 )
                  &&( RemoteValue <= REMOTE_KEY_AP3_3000 ))
         {
            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_100 + 100;
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if( RemoteValue == REMOTE_KEY_BOOK )
         {
            if( Ap3_MenuPath >= BOOK_MENU_PATH )
            {
               key_state = KEY_STATE_BOOK;
               RemoteNumber = 0;
            }
         }
         else if(( RemoteValue >= REMOTE_KEY_P1 )
                  &&( RemoteValue <= REMOTE_KEY_P250 ))
         {
            if( Ap3_MenuPath >= PAGE_MENU_PATH )
            {
               RemoteNumber = RemoteValue - REMOTE_KEY_P1 + 1;
               if( RemoteNumber <= News_Book.Num_GoTable )
               {
                  play_number_speech( RemoteNumber );
                  play_RMT_speech( RemoteSpeech.SpeechPage );
                  Current_Record.Page_num = RemoteNumber - 1;
                  State_Search = Page_Search;
                  ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
               }
               else
               {
                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                     act_do( News_Go.Action_NoVoice, 0 );
               }
            }
         }
         else if( RemoteValue == REMOTE_KEY_PAGE )
         {
            if( Ap3_MenuPath >= PAGE_MENU_PATH )
            {
               key_state = KEY_STATE_PAGE;
               RemoteNumber = 0;
            }
         }
         else if( RemoteValue==REMOTE_KEY_PU )
         {
            if( Ap3_MenuPath >= PAGE_MENU_PATH )
            {
               if( Current_Record.Page_num )
               {
                  Current_Record.Page_num--;
                  State_Search = Page_Search;
                  ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
               }
            }
         }
         else if( RemoteValue == REMOTE_KEY_PD )
         {
            if( Ap3_MenuPath >= PAGE_MENU_PATH )
            {
               if( Current_Record.Page_num <( News_Book.Num_GoTable - 1 ))
               {
                  Current_Record.Page_num++;
                  State_Search = Page_Search;
                  ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
               }
            }
         }
         else if(( RemoteValue >= REMOTE_KEY_CTL_1 )
                  &&( RemoteValue <= REMOTE_KEY_CTL_20 ))
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            {
               RemoteNumber = RemoteValue - REMOTE_KEY_CTL_1 + 1;
               if( RemoteNumber <= News_Go.Num_ControlTab )
               {
                  //change new control mode.
                  Temp_Control_num = Current_Record.Control_num;
                  Current_Record.Control_num = RemoteNumber - 1;
                  State_Search = Control_Mode_Search;
                  ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
               }
               else
               {
                  act_do( News_Go.Action_NoVoice, 0 );
               }
            }
         }
         /*else if(( RemoteValue >= REMOTE_KEY_0 )
                  &&( RemoteValue <= REMOTE_KEY_9 ))
         {
            if( bGameClicked )
            {
               CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_0 )+ '0';
               gl_key_press = TRUE;
            }
         }
         else if( RemoteValue == REMOTE_KEY_OK )
         {
            if( bGameClicked )
            {
               CurrentKey = RM_ENTER;
               gl_key_press = TRUE;
               State_Search = Wait_Remote_Do_Game;
            }
         }*/
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
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if(( RemoteValue >= REMOTE_KEY_AP3_21 )
                  &&( RemoteValue <= REMOTE_KEY_AP3_99 ))
         {
            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_21 + 21;
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if(( RemoteValue >= REMOTE_KEY_AP3_100 )
                  &&( RemoteValue <= REMOTE_KEY_AP3_3000 ))
         {
            RemoteNumber = RemoteValue - REMOTE_KEY_AP3_100 + 100;
            //do_change_ap3();
            bRemoteChangeAp3 = TRUE;
         }
         else if( RemoteValue == REMOTE_KEY_OK )
         {
            //do_change_ap3();
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
                  ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
               }
               else
               {
                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                     act_do( News_Go.Action_NoVoice, 0 );
               }
               key_state = KEY_STATE_NULL;
            }
            else
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
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
            if( RemoteNumber &&( Ap3_MenuPath >= PAGE_MENU_PATH ))
            {
               //change page.
               if( RemoteNumber <= News_Book.Num_GoTable )
               {
                  play_number_speech( RemoteNumber );
                  play_RMT_speech( RemoteSpeech.SpeechPage );
                  Current_Record.Page_num = RemoteNumber - 1;
                  State_Search = Page_Search;
                  ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
               }
               else
               {
                  if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                     act_do( News_Go.Action_NoVoice, 0 );
               }
               key_state = KEY_STATE_NULL;
            }
            else
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  act_do( News_Go.Action_NoVoice, 0 );
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
}


//play number speech
void play_number_speech( U16 hex_data )
{
#if 0
   _MEM_TYPE_SLOW_ U8 bcd_data;
   _MEM_TYPE_SLOW_ U16 temp;
   
   temp = hex_data % 10000;
   bcd_data = hex_data / 10000;
   play_RMT_speech( RemoteSpeech.Number[ bcd_data ]);
   bcd_data = temp / 1000;
   temp %= 1000;
   play_RMT_speech( RemoteSpeech.Number[ bcd_data ]);
   bcd_data = temp / 100;
   temp %= 100;
   play_RMT_speech( RemoteSpeech.Number[ bcd_data ]);
   bcd_data = temp / 10;
   play_RMT_speech( RemoteSpeech.Number[ bcd_data ]);
   bcd_data = temp % 10;
   play_RMT_speech( RemoteSpeech.Number[ bcd_data ]);
#else
   _MEM_TYPE_SLOW_ U8 s[ 6 ];
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;
   ulen = sprintf( s, "%u", hex_data );
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
#endif
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

#if 1
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
      if( !CalNum2 )
         //CalNum1 = 0.0;
         CalNum1 = 10000000.0;
      else
         CalNum1 = CalNum1 / CalNum2;
      break;
   }
}


//display float number
/*void display_float( Float16 result )
{
   _MEM_TYPE_SLOW_ U8 s[ 26 ], length;
   length = sprintf( s, "%.4f", result );
   if( s[ 0 ] == '-' )
   {
      if( length > 13 )
         bCalErr = TRUE;
   }
   else
   {
      if( length > 12 )
         bCalErr = TRUE;
   }
}*/
//display float number
void display_float( Float16 result, U8 dis_type )
{
   _MEM_TYPE_SLOW_ U8 s[ 26 ], int_length, point_length;
   _MEM_TYPE_SLOW_ Float16 int_data, point_data;
   _MEM_TYPE_SLOW_ U32 point;

   int_length = sprintf( s, "%.4f", result );
   //bCalNagative = FALSE;
   if( s[ 0 ] == '-' )
   {
      if( int_length <= 13 )
      {
         //bCalNagative = TRUE;
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/
            
         point_data = modf( result, &int_data );
display_float_int1:
         point = ( U32 )( -int_data );
         int_length = sprintf( s + 1, "%lu.", point );
         switch( int_length )
         {
            case 0x02:
            case 0x03:
            case 0x04:
            point = ( U32 )(( -point_data )* 10000 + 0.5 );
            if( point >= 10000 )
            {
               point_data = ( Float16 )( point - 10000 )/ -10000.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -10000.0;
            point_length = sprintf( s + 1 + int_length, "%04lu", point );
            break;

            case 0x05:
            point = ( U32 )(( -point_data )* 1000 + 0.5 );
            if( point >= 1000 )
            {
               point_data = ( Float16 )( point - 1000 )/ -1000.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -1000.0;
            point_length = sprintf( s + 1 + int_length, "%03lu", point );
            break;

            case 0x06:
            point = ( U32 )(( -point_data )* 100 + 0.5 );
            if( point >= 100 )
            {
               point_data = ( Float16 )( point - 100 )/ -100.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -100.0;
            point_length = sprintf( s + 1 + int_length, "%02lu", point );
            break;

            case 0x07:
            point = ( U32 )(( -point_data )* 10 + 0.5 );
            if( point >= 10 )
            {
               point_data = ( Float16 )( point - 10 )/ -10.0;
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = ( Float16 )point / -10.0;
            point_length = sprintf( s + 1 + int_length, "%01lu", point );
            break;

            case 0x08:
            point = ( U32 )(( -point_data )+ 0.5 );
            if( point >= 1 )
            {
               point_data = -( Float16 )( point - 1 );
               int_data += -1.0;
               goto display_float_int1;
            }
            else
               point_data = -( Float16 )point;
            //length += sprintf( s + 1 + length, "%00lu", point )+ 1;
            point_length = 0;
            break;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length + 1, point_length );
         }
      }
      else
      {
         bCalErr = TRUE;
      }
   }
   else
   {
      if( int_length <= 12 )
      {
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/

         point_data = modf( result, &int_data );
         point = ( U32 )int_data;
display_float_int2:
         int_length = sprintf( s, "%lu.", point );
         switch( int_length )
         {
            case 0x02:
            case 0x03:
            case 0x04:
            point = ( U32 )( point_data * 10000 + 0.5 );
            if( point >= 10000 )
            {
               point_data = ( Float16 )( point - 10000 )/ 10000.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 10000.0;
            point_length = sprintf( s + int_length, "%04lu", point );
            break;

            case 0x05:
            point = ( U32 )( point_data * 1000 + 0.5 );
            if( point >= 1000 )
            {
               point_data = ( Float16 )( point - 1000 )/ 1000.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 1000.0;
            point_length = sprintf( s + int_length, "%03lu", point );
            break;

            case 0x06:
            point = ( U32 )( point_data * 100 + 0.5 );
            if( point >= 100 )
            {
               point_data = ( Float16 )( point - 100 )/ 100.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 100.0;
            point_length = sprintf( s + int_length, "%02lu", point );
            break;

            case 0x07:
            point = ( U32 )( point_data * 10 + 0.5 );
            if( point >= 10 )
            {
               point_data = ( Float16 )( point - 10 )/ 10.0;
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point / 10.0;
            point_length = sprintf( s + int_length, "%01lu", point );
            break;

            case 0x08:
            point = ( U32 )( point_data + 0.5 );
            if( point >= 1 )
            {
               point_data = ( Float16 )( point - 1 );
               point = ( U32 )int_data + 1;
               goto display_float_int2;
            }
            else
               point_data = ( Float16 )point;
            point_length = 0;
            break;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length, point_length );
         }
      }
      else
      {
         bCalErr = TRUE;
      }
   }
}


//play float number
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
   remote_bits.value &= ~0x2822;
   /*bCalClear = TRUE;
   bInCal = TRUE;*/
   remote_bits.value |= 0x00C0;
}
#endif

#endif  //#if( AP3 == ENABLE )

#endif  //_search_remote_c_
