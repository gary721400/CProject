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
//#include "sssmp3_regs.h"
#include "modules\power\power.h"             // Power module definition
//#include <intrins.h>
#include <math.h>
#include "conf\conf_lcd.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_sio.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

#include "mmi\shared\com_var.h"
#include "mmi\shared\com_appli.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = SEARCH_REMOTE_DATA
#pragma CODE = SEARCH_REMOTE_CODE

//ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

//ImageDeclareType( MMI_AP4_CONTROL_TEMP_CODE00 );

ImageDeclareType( CAL_NUM1_CODE00 );
ImageDeclareType( CAL_NUM1_CODE01 );
ImageDeclareType( CAL_NUM1_CODE );
ImageDeclareType( CAL_NUM2_CODE00 );
ImageDeclareType( CAL_NUM2_CODE01 );
ImageDeclareType( CAL_NUM2_CODE );
ImageDeclareType( CAL_RESULT_CODE00 );
ImageDeclareType( CAL_RESULT_CODE01 );
ImageDeclareType( FLOAT_DISPLAY_CODE00 );
ImageDeclareType( FLOAT_DISPLAY_CODE01 );
ImageDeclareType( FLOAT_DISPLAY_CODE02 );
ImageDeclareType( FLOAT_DISPLAY_CODE03 );

ImageDeclareType( SEARCH_REMOTE_CALDO_CODE );
ImageDeclareType( SEARCH_REMOTE_CALEXIT_CODE );
ImageDeclareType( SEARCH_CAL_NUM1_OP_CODE );
ImageDeclareType( SEARCH_CAL_NUM2_OP_CODE );

ImageDeclareType( SEARCH_REMOTE_COV_CODE );
ImageDeclareType( SEARCH_REMOTE_KEY_NULL_CODE );
ImageDeclareType( SEARCH_REMOTE_KEY_AP4_CODE );
ImageDeclareType( SEARCH_REMOTE_KEY_BOOK_CODE );
ImageDeclareType( SEARCH_REMOTE_KEY_PAGE_CODE );
ImageDeclareType( SEARCH_REMOTE_TEMP_CODE03 );
ImageDeclareType( SEARCH_REMOTE_SPEC_CODE );

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________
//                  U8                      gl_key;
//_MEM_TYPE_SLOW_  U8                       bRemotePress;
_MEM_TYPE_SLOW_   U8                      key_state;
_MEM_TYPE_SLOW_   U32                     RemoteCoorAddress;
_MEM_TYPE_SLOW_   U32                     bakRemoteAddr;
//_MEM_TYPE_SLOW_  REMOTE_CODE              RemoteCode;

#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
_MEM_TYPE_SLOW_   U16                     remote_index_tab[ REMOTE_INDEX_MAX ];
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

#define  MAX_INT_LENGTH             8//10    //max:10  //lcd max 8
#define  MAX_POINT_LENGTH           5     //max:5
#define  MAX_VAILD_LENGTH           ( MAX_INT_LENGTH + MAX_POINT_LENGTH )
#define  MAX_RESULT_LENGTH          8     //lcd max 9

#define  CAL_STATE_NUM1             0
#define  CAL_STATE_NUM2             1

_MEM_TYPE_SLOW_   U16               RemoteValue;
_MEM_TYPE_SLOW_   double            CalNum1;
_MEM_TYPE_SLOW_   double            CalNum2;
_MEM_TYPE_SLOW_   double            cal_memory_num;
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

_MEM_TYPE_SLOW_   U8                   cal_result_str[ 26 ];
_MEM_TYPE_SLOW_   U8                   int_length;
_MEM_TYPE_SLOW_   U8                   point_length;
_MEM_TYPE_SLOW_   Float16              int_data;
_MEM_TYPE_SLOW_   Float16              point_data;
_MEM_TYPE_SLOW_   U32                  point;

_MEM_TYPE_SLOW_   REMOTE_TAB           RemoteTable;
_MEM_TYPE_SLOW_   REMOTE_SPECIAL_TAB   RemoteSpecialTable;

_MEM_TYPE_SLOW_   U16                  RemoteNumber;

_MEM_TYPE_SLOW_   U16                  RemoteVersion;
_MEM_TYPE_SLOW_   REMOTE_SPEECH        RemoteSpeech;

_MEM_TYPE_SLOW_   U8                   RemoteEncryptKey;
_MEM_TYPE_SLOW_   U32                  RemoteFlag;

_MEM_TYPE_SLOW_   U32                  Ap4_Record_Address;
_MEM_TYPE_SLOW_   U32                  Ap4_Record_2_Address;
_MEM_TYPE_SLOW_   U32                  Ap4_Play_Address;
_MEM_TYPE_SLOW_   U32                  Ap4_Record_Stop_Address;
_MEM_TYPE_SLOW_   U32                  Ap4_Play_Stop_Address;
_MEM_TYPE_SLOW_   U32                  rereadAddress;
_MEM_TYPE_SLOW_   U32                  normalAddress;
_MEM_TYPE_SLOW_   REMOTE_BIT_NEW       remote_bits /*= { 0x00 }*/;

_MEM_TYPE_SLOW_   U8                   remote_to_mode;

//---------------------extern------------------------------
extern _MEM_TYPE_MEDSLOW_   U16               virtualKeyState;
//_____ D E C L A R A T I O N ______________________________________________

void  play_number_speech            ( U16 hex_data );
void  calculate_old                 ( void );
void  play_float                    ( /*U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length*/void );
void  display_float                 ( Float16 result, U8 dis_type );
void  clear_cal_error               ( void );
U32   search_one_remote_index       ( U16 one_number );
U8    _chkfloat_                    ( Float16 num );
char  *convert_U32                  ( char *p, U32 i );
char  *convert_S32                  ( char *p, S32 i );
void  cal_num1_deal                 ( void );
void  cal_num2_deal                 ( void );
void  remote_key_null_deal          ( void );
void  remote_key_book_deal          ( void );
void  remote_key_ap4_deal           ( void );
void  remote_key_page_deal          ( void );
void  cal_act_do                    ( void );
void  display_operation             ( void );
void  cal_num1_op_deal              ( void );
void  cal_num2_op_deal              ( void );
void  display_float_deal1           ( void );
void  display_float_deal2           ( void );
void  display_float_deal3           ( void );
void  display_float_deal4           ( U8 dis_type );
void  cal_num1_back_deal            ( void );
void  cal_num1_normal_deal          ( void );
void  cal_num2_back_deal            ( void );
void  cal_num2_normal_deal          ( void );
Bool  remote_spec_deal              ( void );


#if 1
#pragma CODE = SEARCH_REMOTE_TEMP_CODE00

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
   _MEM_TYPE_SLOW_ U32 *p = ( _MEM_TYPE_SLOW_ U32 *)&RemoteSpeech;
   _MEM_TYPE_SLOW_ REMOTE_SPEECH_DATA speech_temp;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   _MEM_TYPE_SLOW_ U8  /*j, n, length_remote, */speech_length = 15;
   //U8 _MEM_TYPE_SLOW_ remote_coor_buf[ 512 ];
#if( REMOTE_DECT == ENABLE )
   U8 _MEM_TYPE_SLOW_ remote_flag = 0;
#endif   //#if( REMOTE_DECT == ENABLE )
#if( REMOTE_VERION == REMOTE1 )
   //RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   //length_remote = length_remote1_coordiante;
#elif( REMOTE_VERION == REMOTE2 )
   //SpecialFunction _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote_coordiante ];
   //length_remote = length_remote2_coordiante;
#endif
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

   Ap4_Record_Address = 0;
   Ap4_Play_Address = 0;
   Ap4_Record_2_Address = 0;
   Ap4_Record_Stop_Address = 0;
   Ap4_Play_Stop_Address = 0;
   rereadAddress = 0;
   normalAddress = 0;
   //Ap4_Record_OK_Address = 0;
   //Ap4_Record_END_Address = 0;
   
   RemoteVersion = 0x01;
   memset( &RemoteSpeech, 0, sizeof( REMOTE_SPEECH ));
   if( !RemoteBaseAddress )
   {
      //remote_state = REMOTE_NONE;	
      bHaveRemote = FALSE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   }
   else
   {
#if( REMOTE_DECT == ENABLE )
      ( void )file_seek( 4, FS_SEEK_END );
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&address, sizeof( U32 ));
      if( address == 0xFFFFEEEE )
      {
         U16 temp;
         ( void )file_seek( 10, FS_SEEK_END );
         file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
         if( !MSB( temp )
            &&( LSB( temp )>= 0x02 ))
         {
            RemoteVersion = temp;
            speech_length = 19;
            if( RemoteVersion >= 4 )
            {
               ( void )file_seek( 11, FS_SEEK_END );
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&RemoteEncryptKey, sizeof( U8 ));
               ( void )file_seek( 8, FS_SEEK_END );
               file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&RemoteFlag, sizeof( U32 ));
            }
         }
      
         ( void )file_seek( 5, FS_SEEK_END );
         file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&remote_flag, sizeof( U8 ));
         if( !( remote_flag & NINE_BYTE_SPEECH_FLAG ))
         {
            bHaveRemote = FALSE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            return;
         }
         read_data_from_nf( RemoteBaseAddress, ( U8 _MEM_TYPE_SLOW_ *)&RemoteTable, sizeof( REMOTE_TAB ));

         if( RemoteVersion >= 3 )
         {
            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 19 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Address, sizeof( U32 ));
            if( Ap4_Record_Address )
            {
               Ap4_Record_Address = RemoteBaseAddress + RemoteTable.SpeechAddress + 19 * 9;
            }
            
            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 22 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Stop_Address, sizeof( U32 ));
            if( Ap4_Record_Stop_Address )
            {
               Ap4_Record_Stop_Address = RemoteBaseAddress + RemoteTable.SpeechAddress + 22 * 9;
            }
            
            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 23 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&Ap4_Play_Address, sizeof( U32 ));
            if( Ap4_Play_Address )
            {
               Ap4_Play_Address = RemoteBaseAddress + RemoteTable.SpeechAddress + 23 * 9;
            }

            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 26 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&Ap4_Play_Stop_Address, sizeof( U32 ));
            if( Ap4_Play_Stop_Address )
            {
               Ap4_Play_Stop_Address = RemoteBaseAddress + RemoteTable.SpeechAddress + 26 * 9;
            }

            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 13 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&rereadAddress, sizeof( U32 ));
            if( rereadAddress )
            {
               rereadAddress = RemoteBaseAddress + RemoteTable.SpeechAddress + 13 * 9;
            }

            read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 12 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&normalAddress, sizeof( U32 ));
            if( normalAddress )
            {
               normalAddress = RemoteBaseAddress + RemoteTable.SpeechAddress + 12 * 9;
            }
            /*
            if( RemoteVersion >= 0x05 )
            {
               read_data_from_nf(( RemoteBaseAddress + RemoteTable.SpeechAddress + 27 * 9 ), ( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_2_Address, sizeof( U32 ));
               if( Ap4_Record_2_Address )
               {
                  Ap4_Record_2_Address = RemoteBaseAddress + RemoteTable.SpeechAddress + 27 * 9;
               }
            }*/
         }

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
         *p = address + ( U32 )i * 9;
         //read_data_from_nf( *p, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp, sizeof( REMOTE_SPEECH_DATA ));
         read_data_from_nf( *p, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp, 9 );
         //read_data_from_nf( *p + 8, ( U8 _MEM_TYPE_SLOW_ *)&speech_temp.SpeechType, sizeof( U8 ));
         if( !speech_temp.SpeechStartAddr 
            || !speech_temp.SpeechLength )
            *p = 0;
         p++;
      }
      //remote_state = REMOTE_DONE;
      bHaveRemote = TRUE;

#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = TRUE;
      memset( remote_index_tab, 0, sizeof( remote_index_tab[ REMOTE_INDEX_MAX ]));
#if( REMOTE_DECT == ENABLE )
      if( remote_flag )
      {
         if( remote_flag & SPECIAL_TABLE_FLAG )
         {
            //length_remote = length_remote2_coordiante;
            goto init_search_remote2;
         }
         else
         {
            //length_remote = length_remote1_coordiante;
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
      {
         SpecialFunction _MEM_TYPE_SLOW_ ptr;
         for( i = 0; i < RemoteSpecialTable.SpecialFunctionNumber; i++ )
         {
            read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, sizeof( SpecialFunction ));
            {
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
#endif   //#if( REMOTE_REC == ENABLE )

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr.index;
                  break;
                  
                  case REMOTE_RECORD_OK:
                  remote_index_tab[ REMOTE_REC_RECORD_OK ] = ptr.index;
                  break;

                  case REMOTE_VOLUME0:
                  case REMOTE_VOLUME1:
                  case REMOTE_VOLUME2:
                  case REMOTE_VOLUME3:
                  case REMOTE_VOLUME4:
                  case REMOTE_VOLUME5:
                  case REMOTE_VOLUME6:
                  case REMOTE_VOLUME7:
                  case REMOTE_VOLUME8:
                  case REMOTE_VOLUME9:
                  case REMOTE_VOLUME10:
                  case REMOTE_VOLUME11:
                  case REMOTE_VOLUME12:
                  case REMOTE_VOLUME13:
                  case REMOTE_VOLUME14:
                  remote_index_tab[ ptr.function - REMOTE_VOLUME0 + REMOTE_VOL0 ] = ptr.index;
                  break;

                  case REMOTE_NORMAL_PLAY_REPEAT:
                  remote_index_tab[ REMOTE_SPEECH_REPEAT ] = ptr.index;
                  break;
               }
            }
            address += ( U32 )sizeof( SpecialFunction );
         }
         
         /*if( RemoteVersion >= 0x03 )
         {
            Ap4_Record_OK_Address = search_one_remote_index( remote_index_tab[ REMOTE_REC_RECORD_OK ]);
            Ap4_Record_END_Address = search_one_remote_index( remote_index_tab[ REMOTE_REC_STOP ]);
         }*/
         return;
      }
#endif   //#if(( REMOTE_VERION == REMOTE2 )||( REMOTE_DECT == ENABLE ))
#if(( REMOTE_VERION == REMOTE1 )||( REMOTE_DECT == ENABLE ))
#if( REMOTE_DECT == ENABLE )
init_search_remote1:
#endif   //#if( REMOTE_DECT == ENABLE )
      address = RemoteCoorAddress;
      //j = RemoteTable.Number % length_remote;
      //i = RemoteTable.Number / length_remote;
      {
         RemoteCoor _MEM_TYPE_SLOW_ ptr;
         for( i = 0; i < RemoteTable.Number; i++ )
         {
            read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&ptr, 4 );
            
            {
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
#endif   //#if( REMOTE_REC == ENABLE )

                  case REMOTE_KEY_REC_STOP:
                  remote_index_tab[ REMOTE_REC_STOP ] = ptr.index;
                  break;

                  case REMOTE_RECORD_OK:
                  remote_index_tab[ REMOTE_REC_RECORD_OK ] = ptr.index;
                  break;

                  case REMOTE_VOLUME0:
                  case REMOTE_VOLUME1:
                  case REMOTE_VOLUME2:
                  case REMOTE_VOLUME3:
                  case REMOTE_VOLUME4:
                  case REMOTE_VOLUME5:
                  case REMOTE_VOLUME6:
                  case REMOTE_VOLUME7:
                  case REMOTE_VOLUME8:
                  case REMOTE_VOLUME9:
                  case REMOTE_VOLUME10:
                  case REMOTE_VOLUME11:
                  case REMOTE_VOLUME12:
                  case REMOTE_VOLUME13:
                  case REMOTE_VOLUME14:
                  remote_index_tab[ ptr.function - REMOTE_VOLUME0 + REMOTE_VOL0 ] = ptr.index;
                  break;

                  case REMOTE_NORMAL_PLAY_REPEAT:
                  remote_index_tab[ REMOTE_SPEECH_REPEAT ] = ptr.index;
                  break;
                  case REMOTE_ENTER_REREAD:
                  
                  break;
               }
            }
            address += ( U32 )13;
         }
      }
      /*if( RemoteVersion >= 0x03 )
      {
         Ap4_Record_OK_Address = search_one_remote_index( remote_index_tab[ REMOTE_REC_RECORD_OK ]);
         Ap4_Record_END_Address = search_one_remote_index( remote_index_tab[ REMOTE_REC_STOP ]);
      }*/
#endif
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   }
}

#if 0
//search remote index
U32 search_one_remote_index( U16 one_number )
{
#if( DICHOTOMY == ENABLE )
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle, number;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ Temp_coor;
#undef nEnd
#define nEnd   number
   if( !RemoteTable.Number )
      return 0;
   nStart = 0;
   nEnd = RemoteTable.Number - 1;
   while( nStart <= nEnd )
   { 
      nMiddle = ( nStart + nEnd )/ 2;
      Address = RemoteCoorAddress +( U32 )sizeof( RemoteCoor )*( U32 )nMiddle;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( RemoteCoor ));
      if( one_number == Temp_coor.index ) 
      {
         return ( Address + 4 );
      }
      else if( one_number < Temp_coor.index )
      {
         if( nMiddle == 0 )
         {
            return 0;
         }
         nEnd = nMiddle - 1;
      }
      else
      {
         nStart = nMiddle + 1;
      }
   }
   return 0;
#undef nEnd

#elif( DICHOTOMY == DISABLE )
   U8  _MEM_TYPE_SLOW_ j, n;
   U16 _MEM_TYPE_SLOW_ i;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry[ length_remote1_coordiante ];
   if( !RemoteTable.Number )
      return 0;
   i = RemoteTable.Number / length_remote1_coordiante;
   j = RemoteTable.Number % length_remote1_coordiante;
   Address = RemoteCoorAddress;
   /*if( i == 0 )
   {
      read_data_from_nf( Address, ( U8 *)&remote_coor_arry, ( U16 )sizeof( RemoteCoor )*( U16 )j );
      for( n = 0; n < j; n++ )
      {
         if( one_number == remote_coor_arry[ n ].index )
         {
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
            if( one_number == remote_coor_arry[ n ].index )
            {
               Address = Address + ( U32 )n * 13 + 4;
               return Address;
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
            Address = Address + ( U32 )n * 13 + 4;
            return Address;
         }
      }
   //}
   return 0;

#endif   //#if( DICHOTOMY == ENABLE )
}
#endif


#pragma CODE = SEARCH_REMOTE_TEMP_CODE01

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
   bakRemoteAddr = 0;
   nStart = 0;
   nEnd = RemoteTable.Number - 1;
   while( nStart <= nEnd )
   { 
      //nMiddle = ( nStart + nEnd )/ 2;
      nMiddle = ( nStart + nEnd )>> 1;
      Address = RemoteCoorAddress +( U32 )13 *( U32 )nMiddle;
      read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, 4 );
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
                              ||(( RemoteValue >= REMOTE_LEFT_PARENTHESIS )&&( RemoteValue <= REMOTE_CAL_M_CLEAR ))
                                 ||(( RemoteValue >= REMOTE_VOLUME0 )&&( RemoteValue <= REMOTE_VOLUME14 )))
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
         bakRemoteAddr = Address;
         g_b_enable_flush = TRUE;
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
   //if( Ap3_MenuPath >= CONTROL_MENU_PATH )
     g_b_enable_flush = TRUE;
      ( void )act_do( News_Go.Action_NoVoice, 0 );
   return FALSE;
#undef nEnd

#elif( DICHOTOMY == DISABLE )
   U8  _MEM_TYPE_SLOW_ j, n;
   U16 _MEM_TYPE_SLOW_ i;
   U32 _MEM_TYPE_SLOW_ Address;
   RemoteCoor _MEM_TYPE_SLOW_ remote_coor_arry;
   if( !RemoteTable.Number )
      return FALSE;
   //i = RemoteTable.Number / length_remote1_coordiante;
   //j = RemoteTable.Number % length_remote1_coordiante;
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
      //while( i )
      for( i = 0; i < RemoteTable.Number; i++ )
      {
         read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&remote_coor_arry, 4 );
         //for( n = 0; n < length_remote1_coordiante; n++ )
         {
            if( Current_Index == remote_coor_arry.index )
            {
               RemoteValue = remote_coor_arry.function;
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
                                    ||(( RemoteValue >= REMOTE_LEFT_PARENTHESIS )&&( RemoteValue <= REMOTE_CAL_M_CLEAR ))
                                       ||(( RemoteValue >= REMOTE_VOLUME0 )&&( RemoteValue <= REMOTE_VOLUME14 )))
                  {
                     Address += ( U32 )sizeof( REMOTE_CODE );
                     play_RMT_speech( Address );
                  }
               }
               else
               {
                  Address += ( U32 )sizeof( REMOTE_CODE );
                  play_RMT_speech( Address );
               }
               return TRUE;
            }
         }
         Address += ( U32 )13;
      }
   //}
   //play_RMT_speech( RemoteSpeech.SpeechNull );
   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
      act_do( News_Go.Action_NoVoice, 0 );
   //state_search_to_play();
   return FALSE;
#endif   //#if( DICHOTOMY == ENABLE )
}


#pragma CODE = SEARCH_REMOTE_TEMP_CODE02

//Scan remote
void remote_task( void )
{
   //if( search_remote_index())
   //if( bRemotePress )
   {
      //bRmoteExitAp4 = FALSE;
      //bRemotePress = FALSE;
      SDKLoadCode( SEARCH_REMOTE_SPEC_CODE );
      if( remote_spec_deal())
         return;
#if 0
#if 0
      if(( RemoteValue >= REMOTE_KEY_A )&&( RemoteValue <= REMOTE_KEY_Z ))
      {
         if( bGameClicked && !bInCal )
         {
            CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_A )+ 'a';
            gl_key_press = TRUE;
         }
         return;
      }
      if( RemoteValue == REMOTE_KEY_CANCLE )
      {
         if( bGameClicked && !bInCal )
         {
            CurrentKey = RM_CANCLE;
            gl_key_press = TRUE;
            return;
         }
      }
      if( RemoteValue == REMOTE_KEY_BACK )
      {
         if( bGameClicked && !bInCal )
         {
            CurrentKey = RM_BACK;
            gl_key_press = TRUE;
         }
         return;
      }
      if( RemoteValue == REMOTE_KEY_DOT )
      {
         if( bGameClicked && !bInCal )
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
         mmi_ap3_display_volume();
         //bRemotePress = FALSE;
         return;
      }
      if( RemoteValue == REMOTE_KEY_VINC )
      {
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_ap3_display_volume();
         //bRemotePress = FALSE;
         return;
      }
      if( RemoteValue == REMOTE_KEY_POWER )
      {
         if( !power_is_vbus_present())
            mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
         //bSpeechEnd = FALSE;
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
            //g_b_remote_on = TRUE;
            //bInDictionary = TRUE;
            /*bRmoteExitAp4 = TRUE;
            bInEbookMode = TRUE;*/
            remote_bits.value |= 0x1010u;//0x0808u;
            remote_to_mode = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
            return;
         }
         if( RemoteValue == REMOTE_KEY_EXDIC )
         {
            /*bRmoteExitAp4 = FALSE;
            bInEbookMode = FALSE;*/
#if( DIC == ENABLE )
            remote_bits.value &= ~0x1010u;//0x0808u;
#endif   //#if( DIC == ENABLE )
            return;
         }
      }
#endif   //#if( REMOTE_DIC == ENABLE )
#if( REMOTE_REC == ENABLE )
      if( RemoteValue == REMOTE_KEY_REC )
      {
         //bInCal = FALSE;
         //g_b_dic_remote_on = TRUE;
         //g_b_remote_on = TRUE;
         bRmoteExitAp4 = TRUE;
         remote_to_mode = RECORDER_APPLI;
         return;
      }
#endif   //#if( REMOTE_REC == ENABLE )
#if( REMOTE_PASTER == ENABLE )
      if( RemoteValue == REMOTE_KEY_PASTER )
      {
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
         //g_b_remote_on = TRUE;
         bRmoteExitAp4 = TRUE;
         remote_to_mode = PLAYER_APPLI;
         return;
      }
#endif   //#if( REMOTE_MP3 == ENABLE )
#endif
#if 1
      if( RemoteValue == REMOTE_KEY_ENCAL )
      {
         //enter calculation state
         SDKLoadCode( SEARCH_REMOTE_CALDO_CODE );
         clear_cal_error();
         //bRemotePress = FALSE;
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
/*		 
         lcd_clear_part_of_screen( 32, 0, 87, 8 );
         lcd_goto_xy( 34, 0 );
         Ih_set_curr_font( IDC_FONT_08 );
*/		 
		 lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
         lcd_goto_xy( 0, 8 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts(( U8 *)"Calculator" );
         SDKLoadCode( CAL_RESULT_CODE00 );
         display_float( CalNum1, DIS_RESULT_ONLY );
         return;
      }
      if( bInCal )
      {
         if(( RemoteValue == REMOTE_KEY_EXCAL )
            ||( RemoteValue == REMOTE_KEY_AP4 ))
         {
            /*bRmoteExitAp4 = FALSE;
            bInCal = FALSE;*/
            //remote_bits.value &= ~0x0110u;//0x0880;
            //bRemotePress = FALSE;
            SDKLoadCode( SEARCH_REMOTE_CALEXIT_CODE );
            cal_exit_deal();
            return;
         }
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               ( void )act_do( News_Go.Action_NoVoice, 0 );
            return;
         }
         SDKLoadCode( SEARCH_REMOTE_CALDO_CODE );
         SDKLoadCode( CAL_RESULT_CODE00 );
         cal_act_do();
#if 0
         switch( cal_state )
         {
            case CAL_STATE_NUM1:
            if((( RemoteValue >= REMOTE_KEY_0 )
               &&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_CAL_BACK )
                     ||( RemoteValue == REMOTE_KEY_DOT ))
            {
               SDKLoadCode( CAL_NUM1_CODE );
               cal_num1_deal();
#if 0
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
                     {
                        CalNum1 = ( U32 )( CalNum1 / 10 );
                        if( cal_int_length )
                           cal_int_length--;
                     }
                     else
                     {
                        switch( cal_point_length )
                        {
                           case 0x00:
                           bCalPoint = FALSE;
                           CalNum1 = ( U32 )( CalNum1 / 10 );
                           if( cal_int_length )
                              cal_int_length--;
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
                           
                           case 0x05:
                           cal_point_length--;
                           CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 ))/ 10000.0;
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
               if( cal_input_length >= MAX_VAILD_LENGTH )
                  break;
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }
               if( bCalPoint )
               {
                  //if( cal_point_length >= 4 )
                  if(( cal_point_length >= MAX_POINT_LENGTH )
                     ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
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

                     case 0x05:
                     //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
                     CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
                     break;
                  }
               }
               else
               {
                  if( cal_int_length >= MAX_VAILD_LENGTH )
                     break;
                  cal_input_length++;
                  cal_int_length++;
                  CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
               }
#endif
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
               goto cal_equal;
#if 0
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
#endif
               break;
            }
            break;

            case CAL_STATE_NUM2:
            if((( RemoteValue >= REMOTE_KEY_0 )
               &&( RemoteValue <= REMOTE_KEY_9 ))
                  ||( RemoteValue == REMOTE_CAL_BACK )
                     ||( RemoteValue == REMOTE_KEY_DOT ))
            {
               SDKLoadCode( CAL_NUM2_CODE );
               cal_num2_deal();
#if 0
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

                           case 0x05:
                           cal_point_length--;
                           CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 ))/ 10000.0;
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
                     else
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
                        remote_bits.value &= ~0x4484u;//0x2822;
                        /*bCalClear = TRUE;
                        bInCal = TRUE;*/
                        remote_bits.value |= 0x0300u;//0x00C0;
                        display_float( CalNum1, DIS_RESULT_ONLY );
                        break;
                     }
                  }
                  if( cal_input_length )
                     cal_input_length--;
                  bNewCal = TRUE;
                  break;
               }
               //if( cal_input_length >= 11 )
               if( cal_input_length >= MAX_VAILD_LENGTH )
                  break;
               if( RemoteValue == REMOTE_KEY_DOT )
               {
                  bCalPoint = TRUE;
                  break;
               }
               if( bCalPoint )
               {
                  //if( cal_point_length >= 4 )
                  if(( cal_point_length >= MAX_POINT_LENGTH )
                     ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
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

                     case 0x05:
                     //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
                     CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
                     break;
                  }
               }
               else
               {
                  if( cal_int_length >= MAX_VAILD_LENGTH )
                     break;
                  cal_input_length++;
                  cal_int_length++;
                  CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
               }
               bNewCal = TRUE;
#endif
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
               if( bNewCal )
               {
                  SDKLoadCode( CAL_RESULT_CODE );
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
cal_equal:
               SDKLoadCode( CAL_RESULT_CODE );
               calculate_old();
               cal_input_length = 0;
               cal_point_length = 0;
               cal_int_length = 0;
               bCalPoint = FALSE;
               
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
#endif
         return;
      }
#endif
      SDKLoadCode( SEARCH_REMOTE_TEMP_CODE03 );
      switch( key_state )
      {
         case KEY_STATE_NULL:
         SDKLoadCode( SEARCH_REMOTE_KEY_NULL_CODE );
         remote_key_null_deal();
#if 0
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
            if( bGameClicked && !bInCal )
            {
               CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_0 )+ '0';
               gl_key_press = TRUE;
            }
         }
         else if( RemoteValue == REMOTE_KEY_OK )
         {
            if( bGameClicked && !bInCal )
            {
               CurrentKey = RM_ENTER;
               gl_key_press = TRUE;
               State_Search = Wait_Remote_Do_Game;
            }
         }*/
#endif
         break;

         case KEY_STATE_AP3:
         SDKLoadCode( SEARCH_REMOTE_KEY_AP4_CODE );
         remote_key_ap4_deal();
#if 0
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
#endif
         break;

         case KEY_STATE_BOOK:
         SDKLoadCode( SEARCH_REMOTE_KEY_BOOK_CODE );
         remote_key_book_deal();
#if 0
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
#endif
         break;

         case KEY_STATE_PAGE:
         SDKLoadCode( SEARCH_REMOTE_KEY_PAGE_CODE );
         remote_key_page_deal();
#if 0
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
#endif
         break;
      }
      //bRemotePress = FALSE;
   }
}


#pragma CODE = SEARCH_REMOTE_SPEC_CODE

Bool remote_spec_deal( void )
{
#if 1
#if 1
   if(( RemoteValue >= REMOTE_KEY_A )&&( RemoteValue <= REMOTE_KEY_Z ))
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_A )+ 'a';
         gl_key_press = TRUE;
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_KEY_CANCLE )
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = RM_CANCLE;
         gl_key_press = TRUE;
         return TRUE;
      }
   }
   if( RemoteValue == REMOTE_KEY_BACK )
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = RM_BACK;
         gl_key_press = TRUE;
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_KEY_DOT )
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = RM_DOT;
         gl_key_press = TRUE;
         return TRUE;
      }
      if( !bInCal )
         return TRUE;
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
      mmi_ap3_display_volume();
      //bRemotePress = FALSE;
      return TRUE;
   }
   if( RemoteValue == REMOTE_KEY_VINC )
   {
      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      mmi_ap3_display_volume();
      //bRemotePress = FALSE;
      return TRUE;
   }
   if(( RemoteValue >= REMOTE_VOLUME0 )
      &&( RemoteValue <= REMOTE_VOLUME14 ))
   {
      if(( RemoteValue - REMOTE_VOLUME0 )>( VOL_LEVEL - 1 ))
         return TRUE;
      g_volume = RemoteValue - REMOTE_VOLUME0;
      srvplayer_volume_send();
      mmi_ap3_display_volume();
      //bRemotePress = FALSE;
      return TRUE;
   }
   if( RemoteValue == REMOTE_KEY_POWER )
   {
      if( !power_is_vbus_present())
         mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
      //bSpeechEnd = FALSE;
      //bRemotePress = FALSE;
      return TRUE;
   }
   if( !bInCal &&( RemoteValue == REMOTE_KEY_AP4 ))
   {
      bRmoteExitAp4 = FALSE;
      return TRUE;
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
         //g_b_remote_on = TRUE;
         //bInDictionary = TRUE;
         /*bRmoteExitAp4 = TRUE;
         bInEbookMode = TRUE;*/
         remote_bits.value |= 0x1010u;//0x0808u;
         remote_to_mode = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
         return TRUE;
      }
      if( RemoteValue == REMOTE_KEY_EXDIC )
      {
         /*bRmoteExitAp4 = FALSE;
         bInEbookMode = FALSE;*/
#if( DIC == ENABLE )
         remote_bits.value &= ~0x1010u;//0x0808u;
#endif   //#if( DIC == ENABLE )
         return TRUE;
      }
   }
#endif   //#if( REMOTE_DIC == ENABLE )
#if( REMOTE_REC == ENABLE )
   if( RemoteValue == REMOTE_KEY_REC )
   {
      //bInCal = FALSE;
      //g_b_dic_remote_on = TRUE;
      //g_b_remote_on = TRUE;
      bRmoteExitAp4 = TRUE;
      remote_to_mode = RECORDER_APPLI;
      recorder_mode = 0;
      return TRUE;
   }
#endif   //#if( REMOTE_REC == ENABLE )
#if( REMOTE_PASTER == ENABLE )
   if( RemoteValue == REMOTE_KEY_PASTER )
   {
      //g_b_remote_on = TRUE;
      bRmoteExitAp4 = TRUE;
      remote_to_mode = PASTER_APPLI;
      return TRUE;
   }
#endif   //#if( REMOTE_PASTER == ENABLE )
#if( REMOTE_MP3 == ENABLE )
   if( RemoteValue == REMOTE_KEY_GO_MP3 )
   {
      //bInCal = FALSE;
      //g_b_remote_on = TRUE;
      bRmoteExitAp4 = TRUE;
      remote_to_mode = PLAYER_APPLI;
      return TRUE;
   }
#endif   //#if( REMOTE_MP3 == ENABLE )
   if( RemoteValue == REMOTE_CHANGE_MODE )
   {
      if( !bInCal )
      {
         if(( Ap3_MenuPath >= CONTROL_MENU_PATH )
            && News_Go.Num_ControlTab )
         {
            Temp_Control_num = Current_Record.Control_num;
            if( ++Current_Record.Control_num >= News_Go.Num_ControlTab )
               Current_Record.Control_num = 0;
            State_Search = Control_Mode_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
         }
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_GAME_PREV )
   {
      if( News_Control.Type_Control == GAME_TYPE )
      {
         bSkipPrevious = TRUE;
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_GAME_NEXT )
   {
      if( News_Control.Type_Control == GAME_TYPE )
      {
         bSkipNext = TRUE;
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_GAME_EXIT )
   {
      if( bInGame &&( News_Control.Type_Control == GAME_TYPE ))
      {
         bTipPlay = TRUE;
         bExitGameAction = TRUE;
      }
      return TRUE;
   }
   
   if( RemoteValue == REMOTE_ENTER_REREAD )
   {
      if( !bInCal )
      {
         ap4_read_status = MMI_AP3_RERAED_STATUS;
         //virtualKeyState = GOTO_REREAD;
         
         
        if( Point_Read == Point_Write )
            mail_send_event( EVT_SEARCH_REREADMODE, 0 );
      }
      return TRUE;
   }
   if( RemoteValue == REMOTE_EXIT_REREAD )
   {
      if( !bInCal )
      {
         if( Point_Read == Point_Write )
            mail_send_event( EVT_SEARCH_REREADMODE, 1 );
         else
            ap4_read_status = MMI_AP3_SIMPLE_STATUS;
      }
      return TRUE;
   }
  // */
#endif
   return FALSE;
}


#pragma CODE = SEARCH_REMOTE_TEMP_CODE03

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
   //ulen = sprintf( s, "%u", hex_data );
   memset( s, 0, 6 );
   SDKLoadCode( SEARCH_REMOTE_COV_CODE );
   convert_U16(( char *)s, hex_data );
   ulen = strlen(( char *)s );
   temp = 0;
   if( RemoteVersion != 0x01 )
      goto play_num_speech_v2;
   if( RemoteSpeech.SpeechTen || RemoteSpeech.SpeechHundred || RemoteSpeech.SpeechThousand || RemoteSpeech.SpeechTenThousand )
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


#pragma CODE = SEARCH_REMOTE_CODE

//play remote speech
void play_RMT_speech( U32 speech )	
{  
   state_search_to_play();
   
   if( !speech )
      return;
   //speech += RemoteBaseAddress;
   
   write_media_addr( speech, 1 );
}


#pragma CODE = SEARCH_REMOTE_TEMP_CODE02

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

#pragma CODE = SEARCH_REMOTE_CALDO_CODE

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
      //if( !CalNum2 )
      if( _chkfloat_( CalNum2 ))
         //CalNum1 = 0.0;
         CalNum1 = 1000000000.0;
      else
         CalNum1 = CalNum1 / CalNum2;
      break;
   }
}


#pragma CODE = CAL_RESULT_CODE00

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
   //_MEM_TYPE_SLOW_ U8 s[ 26 ], s1[ 8 ], int_length, point_length;
   //_MEM_TYPE_SLOW_ U8 s1[ 8 ];
   //_MEM_TYPE_SLOW_ Float16 int_data, point_data;
   //_MEM_TYPE_SLOW_ U32 point;

   //int_length = sprintf( s, "%.4f", result );
   //point_data = modf( result, &int_data );
   int_data = ( Float16 )(( S32 )result );
   point_data = result - int_data;
   SDKLoadCode( FLOAT_DISPLAY_CODE00 );
   display_float_deal1();
#if 0
   memset( cal_result_str, 0, 26 );
   if(( point_data < 0 )&&(( S32 )int_data == 0 ))
   {
      cal_result_str[ 0 ] = '-';
      convert_S32(( char *)&cal_result_str[ 1 ], ( S32 )int_data );
   }
   else
      convert_S32(( char *)cal_result_str, ( S32 )int_data );
   strcat(( char *)cal_result_str, "." );
   int_length = strlen(( char *)cal_result_str );
#endif
   
   //bCalNagative = FALSE;
   if( cal_result_str[ 0 ] == '-' )
   {
      SDKLoadCode( FLOAT_DISPLAY_CODE01 );
      display_float_deal2();
#if 0
      if( int_length <= 12 )
      {
         //bCalNagative = TRUE;
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/
            
         //point_data = modf( result, &int_data );
display_float_int1:
         point = ( U32 )( -int_data );
         //int_length = sprintf( s + 1, "%lu.", point );
         memset( &cal_result_str[ 1 ], 0, 25 );
         convert_U32(( char *)&cal_result_str[ 1 ], point );
         strcat(( char *)cal_result_str, "." );
         int_length = strlen(( char *)cal_result_str );
         point = ( U32 )(( -point_data )* 100000 + 0.5 );
         if( point >= 100000 )
         {
            point_data = ( Float16 )( point - 100000 )/ -100000.0;
            int_data += -1.0;
            goto display_float_int1;
         }
         else
            point_data = ( Float16 )point / -100000.0;
         //point_length = sprintf( s + int_length, "%04lu", point );
         memset( s1, 0, 8 );
         convert_U32(( char *)s1, point );
         point_length = strlen(( char *)s1 );
         for( ; point_length < 5; point_length++ )
            strcat(( char *)cal_result_str, "0" );
         strcat(( char *)cal_result_str, ( char *)s1 );
#if 0
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
#endif
         /*if( dis_type & DIS_RESULT_ONLY )
         {
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_goto_xy( 128 -( int_length + point_length )* 8, 16 );
            lcd_puts( s );
         }
         if( dis_type &( DIS_MEMORY | GET_MEMORY ))
         {
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 0, 8 );
            lcd_puts(( U8 *)"M" );
            cal_memory_num = int_data + point_data;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length, point_length );
         }*/
      }
      else
      {
         bCalErr = TRUE;

         /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_goto_xy( 0, 16 );
         lcd_puts(( U8 *)"Error" );*/
      }
#endif
   }
   else
   {
      SDKLoadCode( FLOAT_DISPLAY_CODE02 );
      display_float_deal3();
#if 0
      if( int_length <= 11 )
      {
         //length = sprintf( s, "%13.4f", result );
         //lcd_goto_xy( 24, 16 );
         /*lcd_goto_xy( 128 - length * 8, 16 );
         lcd_puts( s );*/

         //point_data = modf( result, &int_data );
display_float_int2:
         point = ( U32 )int_data;
         //int_length = sprintf( s, "%lu.", point );
         memset( cal_result_str, 0, 26 );
         convert_U32(( char *)cal_result_str, ( U32 )point );
         strcat(( char *)cal_result_str, "." );
         int_length = strlen(( char *)cal_result_str );
         point = ( U32 )( point_data * 100000 + 0.5 );
         if( point >= 100000 )
         {
            point_data = ( Float16 )( point - 100000 )/ 100000.0;
            //point = ( U32 )int_data + 1;
            int_data += 1.0;
            goto display_float_int2;
         }
         else
            point_data = ( Float16 )point / 100000.0;
         //point_length = sprintf( s + int_length, "%04lu", point );
         memset( s1, 0, 8 );
         convert_U32(( char *)s1, point );
         point_length = strlen(( char *)s1 );
         for( ; point_length < 5; point_length++ )
            strcat(( char *)cal_result_str, "0" );
         strcat(( char *)cal_result_str, ( char *)s1 );
#if 0
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
#endif
         /*if( dis_type & DIS_RESULT_ONLY )
         {
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_goto_xy( 128 -( int_length + point_length )* 8, 16 );
            lcd_puts( s );
         }
         if( dis_type &( DIS_MEMORY | GET_MEMORY ))
         {
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 0, 8 );
            lcd_puts(( U8 *)"M" );
            cal_memory_num = int_data + point_data;
         }
         if( dis_type & GET_RESULT )
         {
            CalNum1 = int_data + point_data;
         }
         if( dis_type & PLAY_RESULT )
         {
            play_float( s, int_length, point_length );
         }*/
      }
      else
      {
         bCalErr = TRUE;

         /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_goto_xy( 0, 16 );
         lcd_puts(( U8 *)"Error" );*/
      }
#endif
   }

   SDKLoadCode( FLOAT_DISPLAY_CODE03 );
   display_float_deal4( dis_type );
#if 0
   if( !bCalErr )
   {
      if( dis_type & DIS_RESULT_ONLY )
      {
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_goto_xy( 128 -( int_length + point_length )* 8, 16 );
         lcd_puts( cal_result_str );
      }
      if( dis_type &( DIS_MEMORY | GET_MEMORY ))
      {
         Ih_set_curr_font( IDC_FONT_08 );
         lcd_goto_xy( 0, 8 );
         lcd_puts(( U8 *)"M" );
         cal_memory_num = int_data + point_data;
      }
      if( dis_type & GET_RESULT )
      {
         CalNum1 = int_data + point_data;
      }
      /*if( dis_type & PLAY_RESULT )
      {
         play_float( s, int_length, point_length );
      }*/
   }
   else
   {
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 16, LCD_WIDTH, 16 );
      Ih_set_curr_font( IDC_FONT_16 );
      lcd_goto_xy( 0, 16 );
      lcd_puts(( U8 *)"Error" );
   }
#endif
}


#pragma CODE = CAL_RESULT_CODE01

//play float number
void play_float( /*U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length*/void )
{
   _MEM_TYPE_SLOW_ U8 i, ulen, temp;
   Bool zero_flag = FALSE;

   if( point_length )
   {
      ulen = int_length + point_length - 1;
      for( i = 0; i < point_length; i++ )
      {
         if( cal_result_str[ ulen ]!= '0' )
            break;
         cal_result_str[ ulen ] = 0;
         ulen--;
      }
      if( i == point_length )
         cal_result_str[ ulen ] = 0;
      int_length--;
   }
   else
   {
      int_length--;
      cal_result_str[ int_length ] = 0;
   }
   
   point_length = int_length;

   if( cal_result_str[ 0 ] == '-' )
   {
      play_RMT_speech( RemoteSpeech.SpeechNeg );
      temp = 1;
      int_length--;
   }
   else
      temp = 0;

   if( RemoteVersion == 0x01 )
      goto play_float_v1;
   if( !( RemoteSpeech.SpeechTen || RemoteSpeech.SpeechHundred || RemoteSpeech.SpeechThousand || RemoteSpeech.SpeechTenThousand ))
      goto play_float_v1;
   
   while(( cal_result_str[ temp ] != 0 )&& int_length )
   {
      switch( int_length )
      {
         case 0x08:
         i = cal_result_str[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         play_RMT_speech( RemoteSpeech.SpeechThousand );
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 7 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
         break;

         case 0x07:
#if 1
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
            {
               play_RMT_speech( RemoteSpeech.Number[ i ]);
               play_RMT_speech( RemoteSpeech.SpeechHundred );
            }
            else
            {
               if( !check_zero_number( &cal_result_str[ temp + 1 ], 2 ))
                  play_RMT_speech( RemoteSpeech.Number[ i ]);
            }
         }
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 6 ))
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
#else
         i = cal_result_str[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         play_RMT_speech( RemoteSpeech.SpeechHundred );
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 6 ))
         {
            play_RMT_speech( RemoteSpeech.SpeechTenThousand );
            int_length = 0;
            break;
            //return;
         }
         int_length--;
#endif
         break;

         case 0x06:
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
            {
               play_RMT_speech( RemoteSpeech.Number[ i ]);
               play_RMT_speech( RemoteSpeech.SpeechTen );
            }
            else
            {
               if( !check_zero_number( &cal_result_str[ temp + 1 ], 1 ))
                  play_RMT_speech( RemoteSpeech.Number[ i ]);
            }
         }
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 5 ))
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
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            if( i )
               play_RMT_speech( RemoteSpeech.Number[ i ]);
         }
         play_RMT_speech( RemoteSpeech.SpeechTenThousand );
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 4 ))
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
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechThousand );
         }
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 3 ))
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
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechHundred );
         }
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 2 ))
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
         i = cal_result_str[ temp ] - '0';
         if( !zero_flag || i )
         {
            play_RMT_speech( RemoteSpeech.Number[ i ]);
            if( i )
               play_RMT_speech( RemoteSpeech.SpeechTen );
         }
         temp++;
         if( check_zero_number( &cal_result_str[ temp ], 1 ))
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
         i = cal_result_str[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
         int_length--;
         break;

         //default:
         case 0x00:
         return;

         default:
         goto play_float_v1;
      }
   }

   temp = point_length;

play_float_v1:
   while( cal_result_str[ temp ] != 0 )
   {
      if( cal_result_str[ temp ] == '.')
         play_RMT_speech( RemoteSpeech.SpeechDot );
      else
      {
         i = cal_result_str[ temp ] - '0';
         play_RMT_speech( RemoteSpeech.Number[ i ]);
      }
      temp++;
   }
}


#pragma CODE = SEARCH_REMOTE_CALDO_CODE

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
   remote_bits.value &= ~0x4414u;//0x2822u;
   /*bCalClear = TRUE;
   bInCal = TRUE;*/
   remote_bits.value |= 0x0300u;//0x00C0u;

//   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_part_of_screen( 32, 0, 87, 8 );
}


#pragma CODE = CAL_NUM1_CODE

void cal_num1_deal( void )
{
   if( bCalClear )
   {
      CalNum1 = 0.0;
      bCalClear = FALSE;
   }
   if( RemoteValue == REMOTE_CAL_BACK )
   {
      SDKLoadCode( CAL_NUM1_CODE00 );
      cal_num1_back_deal();
#if 0
      //if( CalNum1 )
      if( _chkfloat_( CalNum1 )== 0 )
      {
         if( !bCalPoint )
         {
            CalNum1 = ( U32 )( CalNum1 / 10 );
            if( cal_int_length )
               cal_int_length--;
         }
         else
         {
            switch( cal_point_length )
            {
               case 0x00:
               bCalPoint = FALSE;
               CalNum1 = ( U32 )( CalNum1 / 10 );
               if( cal_int_length )
                  cal_int_length--;
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

               case 0x05:
               cal_point_length--;
               CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 ))/ 10000.0;
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
      display_float( CalNum1, DIS_RESULT_ONLY );
      if( cal_input_length )
         cal_input_length--;
#endif
      return;
   }

   SDKLoadCode( CAL_NUM1_CODE01 );
   cal_num1_normal_deal();
#if 0
   //if( cal_input_length >= 11 )
   if( cal_input_length >= MAX_VAILD_LENGTH )
      return;
   if( RemoteValue == REMOTE_KEY_DOT )
   {
      bCalPoint = TRUE;
      return;
   }
   if( bCalPoint )
   {
      //if( cal_point_length >= 4 )
      if(( cal_point_length >= MAX_POINT_LENGTH )
         ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
         return;
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

         case 0x05:
         //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
         CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
         break;
      }
   }
   else
   {
      if( cal_int_length >= MAX_INT_LENGTH )
         return;
      cal_input_length++;
      cal_int_length++;
      CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
   }
   display_float( CalNum1, DIS_RESULT_ONLY );
#endif
}


#pragma CODE = CAL_NUM1_CODE00

void cal_num1_back_deal( void )
{
   //if( CalNum1 )
   if( _chkfloat_( CalNum1 )== 0 )
   {
      if( !bCalPoint )
      {
         CalNum1 = ( U32 )( CalNum1 / 10 );
         if( cal_int_length )
            cal_int_length--;
      }
      else
      {
         switch( cal_point_length )
         {
            case 0x00:
            bCalPoint = FALSE;
            CalNum1 = ( U32 )( CalNum1 / 10 );
            if( cal_int_length )
               cal_int_length--;
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

            case 0x05:
            cal_point_length--;
            CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 ))/ 10000.0;
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
   display_float( CalNum1, DIS_RESULT_ONLY );
   if( cal_input_length )
      cal_input_length--;
}


#pragma CODE = CAL_NUM1_CODE01

void cal_num1_normal_deal( void )
{
   //if( cal_input_length >= 11 )
   if( cal_input_length >= MAX_VAILD_LENGTH )
      return;
   if( RemoteValue == REMOTE_KEY_DOT )
   {
      bCalPoint = TRUE;
      return;
   }
   if( bCalPoint )
   {
      //if( cal_point_length >= 4 )
      if(( cal_point_length >= MAX_POINT_LENGTH )
         ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
         return;
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

         case 0x05:
         //CalNum1 = CalNum1 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
         CalNum1 = ( Float16 )(( U32 )( CalNum1 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
         break;
      }
   }
   else
   {
      if( cal_int_length >= MAX_INT_LENGTH )
         return;
      cal_input_length++;
      cal_int_length++;
      CalNum1 = CalNum1 * 10 + RemoteValue - REMOTE_KEY_0;
   }
   display_float( CalNum1, DIS_RESULT_ONLY );
}


#pragma CODE = CAL_NUM2_CODE

void cal_num2_deal( void )
{
   if( bCalMemory )
   {
      CalNum2 = 0.0;
      bCalMemory = FALSE;
   }
   if( RemoteValue == REMOTE_CAL_BACK )
   {
      SDKLoadCode( CAL_NUM2_CODE00 );
      cal_num2_back_deal();
#if 0
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

               case 0x05:
               cal_point_length--;
               CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 ))/ 10000.0;
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
         else
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
            remote_bits.value &= ~0x4484u;//0x2822;
            /*bCalClear = TRUE;
            bInCal = TRUE;*/
            remote_bits.value |= 0x0300u;//0x00C0;
            display_float( CalNum1, DIS_RESULT_ONLY );
            return;
         }
      }
      if( cal_input_length )
         cal_input_length--;
      bNewCal = TRUE;
      display_float( CalNum2, DIS_RESULT_ONLY );
      display_operation();
#endif
      return;
   }
   SDKLoadCode( CAL_NUM2_CODE01 );
   cal_num2_normal_deal();
#if 0
   //if( cal_input_length >= 11 )
   if( cal_input_length >= MAX_VAILD_LENGTH )
      return;
   if( RemoteValue == REMOTE_KEY_DOT )
   {
      bCalPoint = TRUE;
      return;
   }
   if( bCalPoint )
   {
      //if( cal_point_length >= 4 )
      if(( cal_point_length >= MAX_POINT_LENGTH )
         ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
         return;
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

         case 0x05:
         //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
         CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
         break;
      }
   }
   else
   {
      if( cal_int_length >= MAX_INT_LENGTH )
         return;
      cal_input_length++;
      cal_int_length++;
      CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
   }
   bNewCal = TRUE;
   display_float( CalNum2, DIS_RESULT_ONLY );
   display_operation();
#endif
}


#pragma CODE = CAL_NUM2_CODE00

void cal_num2_back_deal( void )
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

            case 0x05:
            cal_point_length--;
            CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 ))/ 10000.0;
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
      else
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
         remote_bits.value &= ~0x4484u;//0x2822;
         /*bCalClear = TRUE;
         bInCal = TRUE;*/
         remote_bits.value |= 0x0300u;//0x00C0;
         display_float( CalNum1, DIS_RESULT_ONLY );
         return;
      }
   }
   if( cal_input_length )
      cal_input_length--;
   bNewCal = TRUE;
   display_float( CalNum2, DIS_RESULT_ONLY );
   display_operation();
}


#pragma CODE = CAL_NUM2_CODE01

void cal_num2_normal_deal( void )
{
   //if( cal_input_length >= 11 )
   if( cal_input_length >= MAX_VAILD_LENGTH )
      return;
   if( RemoteValue == REMOTE_KEY_DOT )
   {
      bCalPoint = TRUE;
      return;
   }
   if( bCalPoint )
   {
      //if( cal_point_length >= 4 )
      if(( cal_point_length >= MAX_POINT_LENGTH )
         ||(( cal_point_length + cal_int_length )>= MAX_VAILD_LENGTH ))
         return;
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

         case 0x05:
         //CalNum2 = CalNum2 +( Float16 )( RemoteValue - REMOTE_KEY_0 )/ 100000.0;
         CalNum2 = ( Float16 )(( U32 )( CalNum2 * 10000 )* 10 +( RemoteValue - REMOTE_KEY_0 ))/ 100000.0;
         break;
      }
   }
   else
   {
      if( cal_int_length >= MAX_INT_LENGTH )
         return;
      cal_input_length++;
      cal_int_length++;
      CalNum2 = CalNum2 * 10 + RemoteValue - REMOTE_KEY_0;
   }
   bNewCal = TRUE;
   display_float( CalNum2, DIS_RESULT_ONLY );
   display_operation();
}

#endif

/*
char *convert_S16( char *p, int i )
{
   if( i / 10 )
      p = convert_S16( p, i / 10 );
   else if( i < 0 )
      *p++ = '-';
   return *( short *)p = ( i % 10 < 0 ? -1 *( i % 10 ): i % 10 )+ '0', p + 1;
}
*/


#pragma CODE = SEARCH_REMOTE_CALDO_CODE
//#pragma CODE = CAL_RESULT_CODE

U8 _chkfloat_( Float16 num )
{
   U32 temp_num;
   
   //if( !cal_input_length )
   //   return 1;
   temp_num = ( U32 )num;
   if( temp_num )
      return 0;
   //if( !cal_point_length )
   //   return 1;
   temp_num = ( U32 )( num * 100000 );
   if( !temp_num )
      return 1;
   return 0;
}


#pragma CODE = SEARCH_REMOTE_COV_CODE

char *convert_U16( char *p, U16 i )
{
   if( i / 10 )
      p = convert_U16( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#pragma CODE = CAL_RESULT_CODE00

char *convert_U32( char *p, U32 i )
{
   if( i / 10 )
      p = convert_U32( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#pragma CODE = FLOAT_DISPLAY_CODE00

char *convert_S32( char *p, S32 i )
{
   if( i / 10 )
      p = convert_S32( p, i / 10 );
   else if( i < 0 )
      *p++ = '-';
   return *( char *)p = ( i % 10 < 0 ? -1 *( i % 10 ): i % 10 )+ '0', p + 1;
}


#pragma CODE = SEARCH_REMOTE_CALEXIT_CODE

void cal_exit_deal( void )
{
   remote_bits.value &= ~0x0110u;//0x0880;
   /*if( News_Control.Type_Control == GAME_TYPE )
   {
      print_dic_screen();
      print_lettle_screen( lettle_count );
      return;
   }*/
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
     Ih_set_curr_font( IDC_FONT_16 );
	 lcd_goto_xy( 6, 8 );
     lcd_puts( g_string );
 //  lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
/*   
   lcd_goto_xy( 52, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"READ" );
   switch( ap3_mode )
   {
      case SINGLE_PLAY_MODE:
      lcd_draw_picture( 98-6, 0, IDP_AP4_SINGLE );
      break;

      case SEQUENCE_PLAY_MODE:
      lcd_draw_picture( 98-6, 0, IDP_AP4_SEQUENCE );
      break;
   }
   if( Ap3_MenuPath >= CONTROL_MENU_PATH )
   {
      SDKLoadCode( MMI_AP4_CONTROL_TEMP_CODE00 );
      display_control_caption();
   }
*/   
/*   
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 15, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );
*/   
   mmi_ap3_display_icon(FALSE);
}


#pragma CODE = SEARCH_REMOTE_CALDO_CODE

void cal_act_do( void )
{
   switch( cal_state )
   {
      case CAL_STATE_NUM1:
      if((( RemoteValue >= REMOTE_KEY_0 )
         &&( RemoteValue <= REMOTE_KEY_9 ))
            ||( RemoteValue == REMOTE_CAL_BACK )
               ||( RemoteValue == REMOTE_KEY_DOT ))
      {
         SDKLoadCode( CAL_NUM1_CODE );
         cal_num1_deal();
         break;
      }
      SDKLoadCode( SEARCH_CAL_NUM1_OP_CODE );
      cal_num1_op_deal();
#if 0
      switch( RemoteValue )
      {
         case REMOTE_CAL_M_ADD:
         cal_memory_num += CalNum1;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_SUB:
         cal_memory_num -= CalNum1;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_READ:
         display_float( cal_memory_num, DIS_RESULT_ONLY );
         CalNum1 = cal_memory_num;
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_CLEAR:
         cal_memory_num = 0.0;
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;
         
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
         display_float( CalNum2, DIS_RESULT_ONLY );
         display_operation();
         break;

         case REMOTE_KEY_EQUAL:
         //goto cal_equal;
         SDKLoadCode( CAL_RESULT_CODE );
         calculate_old();
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;

         CalOP = REMOTE_KEY_NULL;
         bCalClear = TRUE;
         cal_state = CAL_STATE_NUM1;
         //display_float( CalNum1 );
         display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               ( void )act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         //play_float( CalNum1 );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts(( U8 *)"=" );
         break;
      }
#endif
      break;

      case CAL_STATE_NUM2:
      if((( RemoteValue >= REMOTE_KEY_0 )
         &&( RemoteValue <= REMOTE_KEY_9 ))
            ||( RemoteValue == REMOTE_CAL_BACK )
               ||( RemoteValue == REMOTE_KEY_DOT ))
      {
         SDKLoadCode( CAL_NUM2_CODE );
         cal_num2_deal();
         break;
      }
      SDKLoadCode( SEARCH_CAL_NUM2_OP_CODE );
      cal_num2_op_deal();
#if 0
      switch( RemoteValue )
      {
         case REMOTE_CAL_M_ADD:
         cal_memory_num += CalNum2;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_SUB:
         cal_memory_num -= CalNum2;
         display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         if( _chkfloat_( cal_memory_num )== 1 )
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_READ:
         display_float( cal_memory_num, DIS_RESULT_ONLY );
         CalNum2 = cal_memory_num;
         bCalMemory = TRUE;
         break;

         case REMOTE_CAL_M_CLEAR:
         cal_memory_num = 0.0;
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
         bCalMemory = TRUE;
         break;
         
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
            SDKLoadCode( CAL_RESULT_CODE );
            calculate_old();
            //display_float( CalNum1 );
            display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
            if( bCalErr )
            {
               if( Ap3_MenuPath >= CONTROL_MENU_PATH )
                  ( void )act_do( News_Go.Action_NoVoice, 0 );
               clear_cal_error();
               break;
            }
         }
         bNewCal = FALSE;
         CalNum2 = 0.0;
         CalOP = RemoteValue;
         display_operation();
         break;

         case REMOTE_KEY_EQUAL:
cal_equal:
         SDKLoadCode( CAL_RESULT_CODE );
         calculate_old();
         cal_input_length = 0;
         cal_point_length = 0;
         cal_int_length = 0;
         bCalPoint = FALSE;

         CalOP = REMOTE_KEY_NULL;
         bCalClear = TRUE;
         cal_state = CAL_STATE_NUM1;
         //display_float( CalNum1 );
         display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               ( void )act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
         //play_float( CalNum1 );
         lcd_goto_xy( 0, 16 );
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_puts(( U8 *)"=" );
         break;
      }
#endif
      break;
   }
   //bRemotePress = FALSE;
}


void display_operation( void )
{
   lcd_goto_xy( 0, 26 );
   Ih_set_curr_font( IDC_FONT_16 );
   switch( CalOP )
   {
      case REMOTE_KEY_PLUS:
      lcd_puts(( U8 *)"+" );
      break;

      case REMOTE_KEY_MINUS:
      lcd_puts(( U8 *)"-" );
      break;

      case REMOTE_KEY_MUL:
      lcd_puts(( U8 *)"*" );
      break;

      case REMOTE_KEY_DIV:
      lcd_puts(( U8 *)"/" );
      break;

   }
}


#pragma CODE = FLOAT_DISPLAY_CODE00

void display_float_deal1( void )
{
   memset( cal_result_str, 0, 26 );
   if(( point_data < 0 )&&(( S32 )int_data == 0 ))
   {
      cal_result_str[ 0 ] = '-';
      convert_S32(( char *)&cal_result_str[ 1 ], ( S32 )int_data );
   }
   else
      convert_S32(( char *)cal_result_str, ( S32 )int_data );
   strcat(( char *)cal_result_str, "." );
   int_length = strlen(( char *)cal_result_str );
}


#pragma CODE = FLOAT_DISPLAY_CODE01

void display_float_deal2( void )
{
   _MEM_TYPE_SLOW_ U8 s1[ 8 ];
   
   if( int_length <=( MAX_RESULT_LENGTH + 2 ))
   {
display_float_int1:
      point = ( U32 )( -int_data );
      //int_length = sprintf( s + 1, "%lu.", point );
      memset( &cal_result_str[ 1 ], 0, 25 );
      convert_U32(( char *)&cal_result_str[ 1 ], point );
      strcat(( char *)cal_result_str, "." );
      int_length = strlen(( char *)cal_result_str );
      point = ( U32 )(( -point_data )* 100000 + 0.5 );
      if( point >= 100000 )
      {
         point_data = ( Float16 )( point - 100000 )/ -100000.0;
         int_data += -1.0;
         goto display_float_int1;
      }
      else
         point_data = ( Float16 )point / -100000.0;
      //point_length = sprintf( s + int_length, "%04lu", point );
      memset( s1, 0, 8 );
      convert_U32(( char *)s1, point );
      point_length = strlen(( char *)s1 );
      for( ; point_length < 5; point_length++ )
         strcat(( char *)cal_result_str, "0" );
      strcat(( char *)cal_result_str, ( char *)s1 );
   }
   else
   {
      bCalErr = TRUE;
   }
}


#pragma CODE = FLOAT_DISPLAY_CODE02

void display_float_deal3( void )
{
   _MEM_TYPE_SLOW_ U8 s1[ 8 ];
   
   if( int_length <=( MAX_RESULT_LENGTH + 1 ))
   {
display_float_int2:
      point = ( U32 )int_data;
      //int_length = sprintf( s, "%lu.", point );
      memset( cal_result_str, 0, 26 );
      convert_U32(( char *)cal_result_str, ( U32 )point );
      strcat(( char *)cal_result_str, "." );
      int_length = strlen(( char *)cal_result_str );
      point = ( U32 )( point_data * 100000 + 0.5 );
      if( point >= 100000 )
      {
         point_data = ( Float16 )( point - 100000 )/ 100000.0;
         //point = ( U32 )int_data + 1;
         int_data += 1.0;
         goto display_float_int2;
      }
      else
         point_data = ( Float16 )point / 100000.0;
      //point_length = sprintf( s + int_length, "%04lu", point );
      memset( s1, 0, 8 );
      convert_U32(( char *)s1, point );
      point_length = strlen(( char *)s1 );
      for( ; point_length < 5; point_length++ )
         strcat(( char *)cal_result_str, "0" );
      strcat(( char *)cal_result_str, ( char *)s1 );
   }
   else
   {
      bCalErr = TRUE;
   }
}


#pragma CODE = FLOAT_DISPLAY_CODE03

void display_float_deal4( U8 dis_type )
{
	Ih_set_curr_font( IDC_FONT_16 );
   if( !bCalErr )
   {
      if( dis_type & DIS_RESULT_ONLY )
      {
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
         lcd_goto_xy( 128 -( int_length + point_length )* 8, 24 );
         lcd_puts( cal_result_str );
      }
      if( dis_type &( DIS_MEMORY | GET_MEMORY ))
      {
         lcd_goto_xy( 0, 24 );
         lcd_puts(( U8 *)"M" );
         cal_memory_num = int_data + point_data;
      }
      if( dis_type & GET_RESULT )
      {
         CalNum1 = int_data + point_data;
      }
      /*if( dis_type & PLAY_RESULT )
      {
         play_float( s, int_length, point_length );
      }*/
   }
   else
   {
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
      lcd_goto_xy( 0, 24 );
      lcd_puts(( U8 *)"Error" );
   }
}


#pragma CODE = SEARCH_REMOTE_KEY_NULL_CODE

void remote_key_null_deal( void )
{
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
               ( void )act_do( News_Go.Action_NoVoice, 0 );
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
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         }
      }
   }
   
   else if(( RemoteValue >= REMOTE_KEY_0 )
      &&( RemoteValue <= REMOTE_KEY_9 ))
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = ( U8 )( RemoteValue - REMOTE_KEY_0 )+ '0';
         gl_key_press = TRUE;
      }
   }
   else if( RemoteValue == REMOTE_KEY_OK )
   {
      if( bGameClicked && !bInCal )
      {
         CurrentKey = RM_ENTER;
         gl_key_press = TRUE;
         State_Search = Wait_Remote_Do_Game;
      }
   }
}


#pragma CODE = SEARCH_REMOTE_KEY_AP4_CODE

void remote_key_ap4_deal( void )
{
   if( RemoteValue == REMOTE_KEY_AP3 )
   {
      RemoteNumber = 0;
   }
   else if(( RemoteValue >= REMOTE_KEY_0 )
      &&( RemoteValue <= REMOTE_KEY_9 ))
   {
      if( RemoteNumber > 65500 )
         return;
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
}


#pragma CODE = SEARCH_REMOTE_KEY_BOOK_CODE

void remote_key_book_deal( void )
{
   if( RemoteValue == REMOTE_KEY_BOOK )
   {
      RemoteNumber = 0;
   }
   else if(( RemoteValue >= REMOTE_KEY_0 )
      &&( RemoteValue <= REMOTE_KEY_9 ))
   {
      if( RemoteNumber > 65500 )
         return;
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
               ( void )act_do( News_Go.Action_NoVoice, 0 );
         }
         key_state = KEY_STATE_NULL;
      }
      else
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
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
}


#pragma CODE = SEARCH_REMOTE_KEY_PAGE_CODE

void remote_key_page_deal( void )
{
   if( RemoteValue == REMOTE_KEY_PAGE )
   {
      RemoteNumber = 0;
   }
   else if(( RemoteValue >= REMOTE_KEY_0 )
      &&( RemoteValue <= REMOTE_KEY_9 ))
   {
      if( RemoteNumber > 65500 )
         return;
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
               ( void )act_do( News_Go.Action_NoVoice, 0 );
         }
         key_state = KEY_STATE_NULL;
      }
      else
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
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
}


#pragma CODE = SEARCH_CAL_NUM1_OP_CODE

void cal_num1_op_deal( void )
{
   switch( RemoteValue )
   {
      case REMOTE_CAL_M_ADD:
      cal_memory_num += CalNum1;
      goto cal_mem_next;
      /*display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      if( _chkfloat_( cal_memory_num )== 1 )
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 8 );
      bCalMemory = TRUE;
      break;*/

      case REMOTE_CAL_M_SUB:
      cal_memory_num -= CalNum1;
cal_mem_next:
      display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      if( _chkfloat_( cal_memory_num )== 1 )
      {
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16);
      }
      bCalMemory = TRUE;
      break;

      case REMOTE_CAL_M_READ:
      display_float( cal_memory_num, DIS_RESULT_ONLY );
      CalNum1 = cal_memory_num;
      bCalMemory = TRUE;
      break;

      case REMOTE_CAL_M_CLEAR:
      cal_memory_num = 0.0;
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
      bCalMemory = TRUE;
      break;

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
      display_float( CalNum2, DIS_RESULT_ONLY );
      display_operation();
      break;

      case REMOTE_KEY_EQUAL:
      //goto cal_equal;
      //SDKLoadCode( CAL_RESULT_CODE );
      calculate_old();
      cal_input_length = 0;
      cal_point_length = 0;
      cal_int_length = 0;
      bCalPoint = FALSE;

      CalOP = REMOTE_KEY_NULL;
      bCalClear = TRUE;
      cal_state = CAL_STATE_NUM1;
      //display_float( CalNum1 );
      display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
      SDKLoadCode( CAL_RESULT_CODE01 );
      play_float();
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      //play_float( CalNum1 );
      lcd_goto_xy( 0, 24 );
      Ih_set_curr_font( IDC_FONT_16 );
      lcd_puts(( U8 *)"=" );
      break;
   }
}


#pragma CODE = SEARCH_CAL_NUM2_OP_CODE

void cal_num2_op_deal( void )
{
   switch( RemoteValue )
   {
      case REMOTE_CAL_M_ADD:
      cal_memory_num += CalNum2;
      display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
           ( void )act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      if( _chkfloat_( cal_memory_num )== 1 )
      {
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
      }
      bCalMemory = TRUE;
      break;

      case REMOTE_CAL_M_SUB:
      cal_memory_num -= CalNum2;
      display_float( cal_memory_num, DIS_MEMORY | GET_MEMORY );
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      if( _chkfloat_( cal_memory_num )== 1 )
      {
         SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
      }
      bCalMemory = TRUE;
      break;

      case REMOTE_CAL_M_READ:
      display_float( cal_memory_num, DIS_RESULT_ONLY );
      CalNum2 = cal_memory_num;
      bCalMemory = TRUE;
      break;

      case REMOTE_CAL_M_CLEAR:
      cal_memory_num = 0.0;
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 16 );
      bCalMemory = TRUE;
      break;

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
         display_float( CalNum1, DIS_RESULT_ONLY | GET_RESULT );
         if( bCalErr )
         {
            if( Ap3_MenuPath >= CONTROL_MENU_PATH )
               ( void )act_do( News_Go.Action_NoVoice, 0 );
            clear_cal_error();
            break;
         }
      }
      bNewCal = FALSE;
      CalNum2 = 0.0;
      CalOP = RemoteValue;
      display_operation();
      break;

      case REMOTE_KEY_EQUAL:
      calculate_old();
      cal_input_length = 0;
      cal_point_length = 0;
      cal_int_length = 0;
      bCalPoint = FALSE;

      CalOP = REMOTE_KEY_NULL;
      bCalClear = TRUE;
      cal_state = CAL_STATE_NUM1;
      //display_float( CalNum1 );
      display_float( CalNum1, DIS_RESULT_ONLY | PLAY_RESULT | GET_RESULT );
      SDKLoadCode( CAL_RESULT_CODE01 );
      play_float();
      if( bCalErr )
      {
         if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
         clear_cal_error();
         break;
      }
      //play_float( CalNum1 );
      lcd_goto_xy( 0, 24 );
      Ih_set_curr_font( IDC_FONT_16 );
      lcd_puts(( U8 *)"=" );
      break;
   }
}

#endif

#endif  //#if( AP3 == ENABLE )

#endif  //_search_remote_c_
