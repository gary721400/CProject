//!
//! @file player.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 1.40 snd3-dvk-1_9_5 $Id: player.h,v 1.40 2007/09/24 09:41:01 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _PLAYER_H_
#define _PLAYER_H_

//_____ I N C L U D E S ____________________________________________________

//#include "modules\file_system\fs_com.h"
#include "mmi\player\mmi_player.h"
#include "mp3_internal_include.h"

//_____ D E F I N I T I O N ________________________________________________
#undef _GLOBEXT_
#if (defined _player_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

#if 1
#define  PCM_Buf_1      (( __X int *)PCM_PlayBuf_L1_ )
#define  PCM_Buf_2      (( __X int *)PCM_PlayBuf_L2_ )
#define  ADPCM_Buf      (( __X int *)PCM_PlayBuf_R1_ )
#endif

_GLOBEXT_   _MEM_TYPE_MEDSLOW_   ENCRYPT_DATA   ap4_audio_var;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32            Key_encrypt_mp3;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32            media_paly_all_time;
// Streams Control
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U8             p_g_Id_stream;  // ID of stream running, by default no stream
// End streams Control

_GLOBEXT_                        Bool           g_b_volume_on;



_GLOBEXT_   unsigned long  Frame_Count;
_GLOBEXT_   unsigned int   ADPCM_Buffer_Size;

_GLOBEXT_   __X   FCFILE   *ADPCM_File; 

typedef enum
{
   PLAY_BEGIN_TIME = 0
,  PLAY_CURRENT_TIME   	
,  PLAY_ALL_TIME
};

//! @defgroup Player_status Player status type
//! These definitions are all the possible status values returned by the player
//! @{
typedef  U8                   Player_status;    //!< type of player status
#define  PLAYER_ERR_STREAM_ONGOING     FAIL+0   //!< A stream is on going
#define  PLAYER_ERR_NO_STREAM_ONGOING  FAIL+1   //!< No stream is on going
#define  PLAYER_ERR_LOAD_CODEC         FAIL+2   //!< Error during the codec loading
#define  PLAYER_ERR_NO_CODEC_FOUND     FAIL+3   //!< No codec found for this file
#define  PLAYER_ERR_ACCESS_FILE        FAIL+4   //!< Error during the access file
//! @}

#define  PLAYER_NO_EVENT             0  // There aren't event
#define  PLAYER_CMD_OK               1  // The last command is executed
#define  PLAYER_CMD_FAIL             2  // The last command is FAIL
#define  PLAYER_EOF                  3  // End of file, the player is stopped
#define  PLAYER_ERROR_SYNCH          4  // The synchro is lost
#define  PLAYER_ERROR_UNDERRUN       5  // Error underrun
#define  PLAYER_FILE_NOT_SUPPORTED   7  // Error file not supported
#define  PLAYER_BOF                  8  // Beginning of file, the player is stopped
#define  PLAYER_START_AB             9  // Beginning of repeat AB
#define  PLAYER_EOF_AB              10  // End of repeat AB
#define  PLAYER_BARGRAPH            11  // Bargraph conversion

//***********************************************
// add by wangwei
#define	AP3_FLAG_FILE_HEAD_INVALID    0
#define	AP3_FLAG_FILE_HEAD_START      1
#define	AP3_FLAG_FILE_HEAD_END        2
#define	AP3_FILE_HEAD_SIZE            (( U16 )512 )
//***********************************************
//_____ D E C L A R A T I O N ______________________________________________

//_GLOBEXT_ _MEM_TYPE_MEDSLOW_ Player_status   player_g_status;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U8              player_g_nav_id;  // Navigator selected to run the stream

#define  player_off()      player_stop_stream()

//! Functions of player
Bool player_on             ( void );
//Bool player_off            ( void );
Bool player_play_file      ( char *path );
Bool player_play_stream    ( U8 u8_id_src , U8 u8_codec, U16 u16_nb_sector );
Bool player_stop           ( void );
Bool player_restart_play   ( void );
Bool player_pause          ( void );
Bool player_fast_foward    ( void );
Bool player_fast_rwd       ( void );
Bool player_set_marker_A   ( void );
Bool player_set_marker_B   ( void );
Bool player_stop_repeat_AB ( void );  
void player_set_speed      ( U8 idx_speed );

//! Functions of line in
Bool  player_linein_on      ( void );
Bool  player_linein_off     ( void );

//! Fonctions for all mode (player, recorder, line in )
Bool  player_set_volume     ( U16 u16_volume );
U16   player_get_time       ( void );
void  player_get_bargraph   ( U8 _MEM_TYPE_SLOW_ * );
U16   player_get_timetotal  ( void );
U8    player_channel_type     ( void );
U8    player_sampling_freq    ( void );
U16   player_bitrate          ( void );
U8    player_get_codec_type   ( void );

U8    player_evt              ( void );

void  player_stop_stream      ( void );

#endif  // _PLAYER_H_ 
