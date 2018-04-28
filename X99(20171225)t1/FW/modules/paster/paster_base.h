//!
//! @file paster_base.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster base.
//!
//! @version 1.20 snd3b-dvk-1_0_33 $Id: paster_base.h,v 1.20 2006/12/21 16:02:30 coger Exp $
//!
//! @todo
//! @bug

#ifndef	_PASTER_BASE_H_
#define	_PASTER_BASE_H_

//_____  I N C L U D E S ___________________________________________________

#include "conf\conf_sio.h"
#include "modules/oid_cmd/oid_cmd.h"
#include "modules\file_system\fs_com.h"


#undef _GLOBEXT_
#if (defined _paster_base_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ M A C R O S ________________________________________________________
//#define  paster_unicode_buffer()       g_buffer_unicode
//#define  paster_ascii_buffer()         g_buffer_ascii

_GLOBEXT_   const    U8    g_paster_dir[]
#if 0//def _paster_base_c_
= "Rec"
#endif
;
/*
_GLOBEXT_ Fs_index _MEM_TYPE_SLOW_ g_paster_dir_index
#ifdef _paster_base_c_
= { 0xFF, 0, 0 }
#endif
;
*/

_GLOBEXT_   char  /*_MEM_TYPE_SLOW_*/   paster_file[ 13 ];

// Definitions of zone module IDs
//    Module IDs of the codec zone
#define  CODEC_PLAY_FIRST  (0)
#define  CODEC_MP3         (0)
#define  CODEC_WMA         (1)
#define  CODEC_WAV         (2)
#define  CODEC_MTV         (3)
#define  CODEC_MP2         (4)
#define  CODEC_WMA_2       (5)
#define  CODEC_PLAY_MAX    (6)

#define  CODEC_REC_FIRST   (10)
#define  CODEC_REC_G726    (10)
#define  CODEC_REC_MAX     (11)

#define  CODEC_VIEW_FIRST  (20)
#define  CODEC_JPG         (20)
#define  CODEC_BMP         (21)
#define  CODEC_VIEW_MAX    (22)

#define  CODEC_ADC         (30)

#define  CODEC_CIPHER      (50)

//    Module IDs of the picture zone
#define  PICTURE_DEFAULT   (0)

//    Module IDs of the font zone
#define  FONT_NORMAL       (0)
#define  FONT_TBL_GB2312   (20)
#define  FONT_TBL_B5       (21)


#if( PASTER == ENABLE )

#define	PASTER_MAX_NUM	          10000
#define	PASTER_INDEX_LENGTH	    5

#define	PASTER_RECORD_START	    0
#define	PASTER_RECORD_APPEND     1

#define	PASTER_INDEX_TO_DIR      0
#define	PASTER_INDEX_TO_FILE     1

enum
{
   PASTER_OID_CMD_RECORD = OID_CMD_RECORD,
   PASTER_OID_CMD_PLAY   = OID_CMD_PLAY,
   PASTER_OID_CMD_PAUSE  = OID_CMD_PAUSE,
   PASTER_OID_CMD_STOP   = OID_CMD_STOP,
   PASTER_OID_CMD_COPY   = OID_CMD_COPY,
   PASTER_OID_CMD_BACK   = OID_CMD_BACK,

   PASTER_OID_CMD_NUM    = OID_CMD_NUM,
   PASTER_OID_CMD_NONE   = OID_CMD_NONE
};

//_____ D E F I N I T I O N S ______________________________________________

#define  paster_trans_oid_cmd( oid_value )            oid_cmd_trans(oid_value)
//#define  paster_index_to_dirname( index, dirname )    paster_index_to_string( index, dirname )

//#define  paster_record_stop()       srvrec_stop()
#define  paster_play_stop()         player_ap3_stop()

U8    *paster_index_to_string   ( U16 index, U8 *str );
U8    *paster_index_to_filename ( U16 index, U8 *filename, U8 codec );
//U16 *paster_index_to_unicode  ( U16 index, Bool is_file );
U8    *paster_index_to_unicode  ( U16 index, Bool is_file, U8 type );

Bool  paster_record_find        ( U16 index );
Bool  paster_record_start       ( void );
void  paster_record_stop        ( void );
void  paster_record_file_end    ( void );

Bool  paster_play_find          ( U16 index );
Bool  paster_play_start         ( void );
Bool  paster_play_is_busy       ( void );
//void  paster_play_stop          ( void );
void  paster_play_off           ( void );

Bool  paster_back_to_top_dir    ( void );
Bool  paster_index_file_find    ( U16 index, Bool create_path );

//U8   paster_trans_oid_cmd     ( U16 oid_value );

#endif  //#if( PASTER == ENABLE )

#endif  //_PASTER_BASE_H_
