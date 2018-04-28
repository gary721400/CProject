//!
//! @file paster.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster event.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: paster.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef	_PASTER_H_
#define	_PASTER_H_

//_____  I N C L U D E S ___________________________________________________
#include "conf\conf_sio.h"
#include "modules\cue\cue.h"

#if( PASTER == ENABLE )

#undef _GLOBEXT_
#if (defined _paster_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

enum
{
   PASTER_STATE_IDLE,
   PASTER_STATE_START,
   PASTER_STATE_CHECK,

   PASTER_STATE_CUE_START,
   PASTER_STATE_CUE_PLAY,

   PASTER_STATE_SOUND_START,
   PASTER_STATE_SOUND_WAIT,

   //PASTER_STATE_RECORD_OID,
   PASTER_STATE_RECORD_START,
   PASTER_STATE_RECORD_START_READY,
   PASTER_STATE_RECORD,
   PASTER_STATE_RECORD_PAUSE,
   PASTER_STATE_RECORD_RESUME,
   PASTER_STATE_RECORD_STOP,

   //PASTER_STATE_PLAY_OID,
   PASTER_STATE_PLAY_START,
   PASTER_STATE_PLAY_START_READY,
   PASTER_STATE_PLAY,
   PASTER_STATE_PLAY_PAUSE,
   PASTER_STATE_PLAY_RESUME,
   PASTER_STATE_PLAY_STOP,

   PASTER_STATE_COPY_SRC_OID,
   PASTER_STATE_COPY_DEST_OID,
   PASTER_STATE_COPY_START,
   PASTER_STATE_COPY,
   PASTER_STATE_COPY_END,

   PASTER_STATE_ERROR,
   PASTER_STATE_ERROR_WAIT,
   PASTER_STATE_END,

   PASTER_STATE_NULL
};

enum
{
   CMD_PASTER_NONE,
   CMD_PASTER_START,
	
   CMD_PASTER_OID,
   CMD_PASTER_KEY_PRESSED,

   CMD_PASTER_RECORD,
   CMD_PASTER_PLAY,
   CMD_PASTER_PAUSE,
   CMD_PASTER_STOP,
   CMD_PASTER_COPY,

   CMD_PASTER_VOL_UP,
   CMD_PASTER_VOL_DOWN,

   CMD_PASTER_BACK,

   CMD_PASTER_RECORD_EOF,
   CMD_PASTER_PLAY_EOF,

   CMD_PASTER_POWER_LOW,
   CMD_PASTER_POWER_OFF,
	
   CMD_PASTER_END
};

enum
{
   PASTER_ERROR_DISK_FAILED          = CUE_INDEX_ERROR_DISK_FAILED,
   PASTER_ERROR_DISK_IS_FULL         = CUE_INDEX_ERROR_DISK_IS_FULL,
   PASTER_ERROR_INVALID_OID          = CUE_INDEX_ERROR_INVALID_OID,
   PASTER_ERROR_FIND_FILE_FAILED     = CUE_INDEX_ERROR_FIND_FILE_FAILED,
   PASTER_ERROR_CREATE_FILE_FAILED   = CUE_INDEX_ERROR_CREATE_FILE_FAILED,
   PASTER_ERROR_RECORD_START_FAILED  = CUE_INDEX_ERROR_RECORD_START_FAILED,
   PASTER_ERROR_PLAY_START_FAILED    = CUE_INDEX_ERROR_PLAY_START_FAILED,
   PASTER_ERROR_COPY_SRC_FILE        = CUE_INDEX_ERROR_COPY_SRC_FILE,
   PASTER_ERROR_COPY_DEST_FILE       = CUE_INDEX_ERROR_COPY_DEST_FILE,
   PASTER_ERROR_COPY_START_FAILED    = CUE_INDEX_ERROR_COPY_START_FAILED,
   PASTER_ERROR_COPY_ABORT           = CUE_INDEX_ERROR_COPY_ABORT
};

#define	PASTER_TASK_ID	      3
#define	JOIN16( h, l )	      (((( U16 )( h ))<< 8 )|(( U16 )( l )))


// ID of the timer soft used by this MMI appli
_GLOBEXT_ _MEM_TYPE_SLOW_  U8    s_paster_display_id;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8    mmipaster_play_id_timerdisplay;

_GLOBEXT_   U8    _MEM_TYPE_SLOW_   g_paster_state;

_GLOBEXT_   U8    _MEM_TYPE_SLOW_   s_paster_cue;

#define  PASTER_RECORD_TIME  TIMER_MS(500) 


Bool  paster_init_disk     ( void );
void  paster_send_cmd      ( U8 id, U16 param );
void  paster_evt           ( void );
void  paster_evt_init      ( void );
void  paster_file_getinfos ( void );
void  paster_reset_cmd     ( void );


#endif  //#if( PASTER == ENABLE )

#endif  // _PASTER_H_ 
