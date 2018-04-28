//!
//! @file search_remote.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search remote.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: search_remote.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _SEARCH_REMOTE_H_
#define _SEARCH_REMOTE_H_

#include "conf_oid.h"
#include "conf_sio.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________



//default remote mode
#define REMOTE1                  1
#define REMOTE2                  2
#define REMOTE_VERION            REMOTE1
//#define REMOTE_VERION            REMOTE2

#define REMOTE_DECT              ENABLE
//#define REMOTE_DECT              DISABLE

//遥控器二分法搜索方式
#define DICHOTOMY                ENABLE
//#define DICHOTOMY                DISABLE

#define REMOTE_START             0
#define REMOTE_DONE              1
#define REMOTE_NONE              2

#define KEY_STATE_NULL           0
#define KEY_STATE_AP3            1
#define KEY_STATE_BOOK           2
#define KEY_STATE_PAGE           3

typedef struct				
{
   U16   Number;
   U32   SpeechAddress;
}REMOTE_TAB;

typedef struct
{
   U16   SpecialFunctionNumber;
   U32   SpecialFunctionAddress;
}REMOTE_SPECIAL_TAB;

typedef struct
{
   U32   Number[ 10 ];
   U32   SpeechDot;
   U32   SpeechNeg;
   U32   SpeechPage;
   U32   SpeechNull;
   U32   SpeechAp3;
   U32   SpeechTen;
   U32   SpeechHundred;
   U32   SpeechThousand;
   U32   SpeechTenThousand;
}REMOTE_SPEECH;

typedef struct
{
   U32   SpeechStartAddr;
   U32   SpeechLength;
   U8    SpeechType;
}REMOTE_SPEECH_DATA;


typedef struct
{
   U16   IndexCode;
   U16   FunctionCode;
   //U32   Speech;
}REMOTE_CODE;

/* remote key value define */
#define REMOTE_KEY_NULL       0
#define REMOTE_KEY_OK         1
#define REMOTE_KEY_AP3        2
#define REMOTE_KEY_BOOK       3
#define REMOTE_KEY_PAGE       4
#define REMOTE_KEY_VINC       5
#define REMOTE_KEY_VDEC       6
#define REMOTE_KEY_POWER      7

#define REMOTE_KEY_0          8
#define REMOTE_KEY_1          9
#define REMOTE_KEY_2          10
#define REMOTE_KEY_3          11
#define REMOTE_KEY_4          12
#define REMOTE_KEY_5          13
#define REMOTE_KEY_6          14
#define REMOTE_KEY_7          15
#define REMOTE_KEY_8          16
#define REMOTE_KEY_9          17

#define REMOTE_KEY_CTL_1      18
#define REMOTE_KEY_CTL_20     37

#define REMOTE_KEY_AP3_1      38
#define REMOTE_KEY_AP3_20     57
#define REMOTE_KEY_ENCAL      58
#define REMOTE_KEY_ENDIC      59
#define REMOTE_KEY_PLUS       60
#define REMOTE_KEY_MINUS      61
#define REMOTE_KEY_MUL        62
#define REMOTE_KEY_DIV        63
#define REMOTE_KEY_EQUAL      64
#define REMOTE_KEY_EXCAL      65

#define REMOTE_KEY_A          66
#define REMOTE_KEY_Z          91
#define REMOTE_KEY_CANCLE     92
#define REMOTE_KEY_EXDIC      93
#define REMOTE_KEY_PU         94
#define REMOTE_KEY_PD         95
#define REMOTE_KEY_AP3_21     96
#define REMOTE_KEY_AP3_99     174

#define REMOTE_KEY_GO_MP3     175
#define REMOTE_KEY_EXIT_MP3   176
#define REMOTE_KEY_PLAY_PAUSE 177
#define REMOTE_KEY_MP3_PREV   178
#define REMOTE_KEY_MP3_NEXT   179

#define REMOTE_KEY_BACK       182
#define REMOTE_KEY_DOT        183
#define REMOTE_KEY_P1         184
#define REMOTE_KEY_P250       433

#define REMOTE_KEY_AP3_100    434
#define REMOTE_KEY_AP3_3000   3334

#define REMOTE_KEY_REC        3335
#define REMOTE_KEY_PASTER     3336
#define REMOTE_KEY_AP4        3337
#define REMOTE_KEY_REC_START  3338
#define REMOTE_KEY_REC_PLAY   3339
#define REMOTE_KEY_REC_STOP   3340
#define REMOTE_CAL_BACK       3341
#define REMOTE_CHANGE_SPEED   3342

#define REMOTE_LEFT_PARENTHESIS        3343
#define REMOTE_RIGHT_PARENTHESIS       3344
#define REMOTE_CAL_M_ADD               3345
#define REMOTE_CAL_M_SUB               3346
#define REMOTE_CAL_M_READ              3347
#define REMOTE_CAL_M_CLEAR             3348

#define REMOTE_VOL_0             		3350
#define REMOTE_VOL_1             		3351
#define REMOTE_VOL_2             		3352
#define REMOTE_VOL_3             		3353
#define REMOTE_VOL_4             		3354

#define REMOTE_LEFT_PARENTHESIS        3343
#define REMOTE_RIGHT_PARENTHESIS       3344
#define REMOTE_CAL_M_ADD               3345
#define REMOTE_CAL_M_SUB               3346
#define REMOTE_CAL_M_READ              3347
#define REMOTE_CAL_M_CLEAR             3348

#define REMOTE_MP3_BEGIN_RECORD		   3375
#define REMOTE_MP3_OVER_RECORD		   3376
#define REMOTE_MP3_PLAY_RECORD		   3377

#define RM_ENTER              0x7B
#define RM_CANCLE             0x7C
#define RM_EXI                0x7D
#define RM_BACK               0x7E
#define RM_DOT                0x7F

//#define NINE_BYTE_SPEECH_FLAG    0x01
//#define SPECIAL_TABLE_FLAG       0x02
//#define ENCRYPT_SPEECH_FLAG      0x04


typedef struct
{
   U32  index;//U16   index;//
   U16   function;
   U32   speech_address;
   U32   speech_length;
   U8    speech_type;
}RemoteCoor;

typedef struct
{
   U32   index;//U16   index;//
   U16   function;
}SpecialFunction;

//_GLOBEXT_ _MEM_TYPE_SLOW_  U32            RemoteBaseAddress;
//#define  RemoteBaseAddress                All_Control.Point_Remote


extern                 Bool           g_b_remote_on;

#define  length_remote2_coordiante         128
#define  length_remote1_coordiante         39

extern   _MEM_TYPE_SLOW_   REMOTE_TAB           RemoteTable;
extern   _MEM_TYPE_SLOW_   REMOTE_SPECIAL_TAB   RemoteSpecialTable;

extern   _MEM_TYPE_SLOW_   U16                  RemoteNumber;

extern   _MEM_TYPE_SLOW_   U16                  RemoteVersion;
extern   _MEM_TYPE_SLOW_   REMOTE_SPEECH        RemoteSpeech;
extern   _MEM_TYPE_SLOW_   U32  				RemoteBaseAddress ;

extern   _MEM_TYPE_SLOW_   U8                   RemoteEncryptKey;
extern   _MEM_TYPE_SLOW_   U32                  RemoteFlag;


extern _MEM_TYPE_SLOW_ unsigned int cal_flag;

//extern _MEM_TYPE_SLOW_   U8                flag_go_cal;

extern   _MEM_TYPE_SLOW_   U32  RemoteRecordStartAddr ;
extern  _MEM_TYPE_SLOW_   U32  RemoteRecordEndAddr ;
extern  _MEM_TYPE_SLOW_   U32  RemotePlayStartAddr ;
extern  _MEM_TYPE_SLOW_   U32  RemotePlayEndAddr ;


#define NINE_BYTE_SPEECH_FLAG    0x01u
#define SPECIAL_TABLE_FLAG       0x02u
#define ENCRYPT_SPEECH_FLAG      0x04u

#if 0
typedef union
{
   struct
   {
      U16   bHaveRemote          :1;//
      U16   bCalErr              :1;//
      U16   bInDictionary        :1;//
      U16   bInEbookMode         :1;//
      
      U16   bCalNagative         :1;
      U16   bNewCal              :1;
      U16   bCalClear            :1;
      U16   bInCal               :1;
      
      U16   bRemoteEnterCal      :1;
      U16   bRemoteChangeAp3     :1;
      U16   bCalMemory           :1;
      U16   bRemoteExitAp4       :1;

      U16   bKeyChangeControl    :1;
      U16   bCalPoint            :1;
      U16   bRemoteChoiceAp3     :1;
      
   }bits;
   U16      value;
}REMOTE_BIT_NEW;
#else
typedef union
{
   struct
   {
      unsigned   bHaveRemote          :1;//
      unsigned   bCalErr              :1;//
      unsigned   bInDictionary        :1;//
      unsigned   bInEbookMode         :1;//
      
      unsigned   bCalNagative         :1;
      unsigned   bNewCal              :1;
      unsigned   bCalClear            :1;
      unsigned   bInCal               :1;
      
      unsigned   bRemoteEnterCal      :1;
      unsigned   bRemoteChangeAp3     :1;
      unsigned   bCalMemory           :1;
      unsigned   bRemoteExitAp4       :1;

      unsigned   bKeyChangeControl    :1;
      unsigned   bCalPoint            :1;
      unsigned   bRemoteChoiceAp3     :1;
      
   }bits;
   U16      value;
}REMOTE_BIT_NEW;
#endif



extern _MEM_TYPE_SLOW_ REMOTE_BIT_NEW remote_bits;

#define  bHaveRemote             remote_bits.bits.bHaveRemote
#define  bCalErr                 remote_bits.bits.bCalErr
#define  bInDictionary           remote_bits.bits.bInDictionary
#define  bInEbookMode            remote_bits.bits.bInEbookMode
#define  bCalNagative            remote_bits.bits.bCalNagative
#define  bNewCal                 remote_bits.bits.bNewCal
#define  bCalClear               remote_bits.bits.bCalClear
#define  bInCal                  remote_bits.bits.bInCal
#define  bRemoteEnterCal         remote_bits.bits.bRemoteEnterCal
#define  bRemoteChangeAp3        remote_bits.bits.bRemoteChangeAp3
#define  bCalMemory              remote_bits.bits.bCalMemory
#define  bRmoteExitAp4           remote_bits.bits.bRemoteExitAp4
#define  bKeyChangeControl       remote_bits.bits.bKeyChangeControl
#define  bCalPoint               remote_bits.bits.bCalPoint
#define  bRemoteChoiceAp3        remote_bits.bits.bRemoteChoiceAp3

//#define  bCalOver		         remote_bits.bits.bInDictionary

extern _MEM_TYPE_SLOW_ U8     remote_to_mode;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#define  REMOTE_INDEX_MAX                 32
#define  REMOTE_VOL_UP                    0
#define  REMOTE_VOL_DOWN                  ( REMOTE_VOL_UP + 1 )
#define  REMOTE_POWER_OFF                 ( REMOTE_VOL_UP + 2 )
#define  REMOTE_EXIT_MP3                  ( REMOTE_VOL_UP + 3 )
#define  REMOTE_MP3_PLAY_PAUSE            ( REMOTE_VOL_UP + 4 )
#define  REMOTE_MP3_PREV                  ( REMOTE_VOL_UP + 5 )
#define  REMOTE_MP3_NEXT                  ( REMOTE_VOL_UP + 6 )
#define  REMOTE_AP4_MODE                  ( REMOTE_VOL_UP + 7 )
#define  REMOTE_MP3_MODE                  ( REMOTE_VOL_UP + 8 )
#define  REMOTE_REC_MODE                  ( REMOTE_VOL_UP + 9 )
#define  REMOTE_PASTER_MODE               ( REMOTE_VOL_UP + 10 )
#define  REMOTE_DIC_MODE                  ( REMOTE_VOL_UP + 11 )
#define  REMOTE_CAL_MODE                  ( REMOTE_VOL_UP + 12 )
#define  REMOTE_REC_START                 ( REMOTE_VOL_UP + 13 )
#define  REMOTE_REC_PLAY                  ( REMOTE_VOL_UP + 14 )
#define  REMOTE_REC_STOP                  ( REMOTE_VOL_UP + 15 )
#define  REMOTE_OK                  	  ( REMOTE_VOL_UP + 16 )
#define  REMOTE_V_0                  	  ( REMOTE_VOL_UP + 17 )
#define  REMOTE_V_1                  	  ( REMOTE_VOL_UP + 18 )
#define  REMOTE_V_2                  	  ( REMOTE_VOL_UP + 19 )
#define  REMOTE_V_3                  	  ( REMOTE_VOL_UP + 20 )
#define  REMOTE_V_4                  	  ( REMOTE_VOL_UP + 21 )
#define  REMOTE_V_5                  	  ( REMOTE_VOL_UP + 22 )
#define  REMOTE_V_6                  	  ( REMOTE_VOL_UP + 23 )
#define  REMOTE_V_7                  	  ( REMOTE_VOL_UP + 24 )
#define  REMOTE_V_8                  	  ( REMOTE_VOL_UP + 25 )
#define  REMOTE_V_9                  	  ( REMOTE_VOL_UP + 26 )

#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

void  remote_task                   ( void );
Bool  do_change_ap3                 ( void );
void  init_remote                   ( void );
void  remote_task_init              ( void );
Bool  check_zero_number             ( U8 _MEM_TYPE_SLOW_ *p, U8 length );
void  play_RMT_speech               ( U32 speech );

Bool search_remote_index( void );
void remote_cacu(void);
void calculate_old( void );
void clear_cal_error( void );
void display_float( Float16 result, U8 dis_type );
void play_float( U8 _MEM_TYPE_SLOW_ *s, U8 int_length, U8 point_length );


#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
