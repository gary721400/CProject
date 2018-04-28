//!
//! @file cue.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the cue.
//!
//! @version 1.20 snd3b-dvk-1_0_33 $Id: cue.h,v 1.20 2006/12/21 16:02:30 coger Exp $
//!
//! @todo
//! @bug

#ifndef _CUE_H_
#define	_CUE_H_

//_____  I N C L U D E S ___________________________________________________

//#include "lib_system\utilities\string.h"

#include "conf\conf_sio.h"
#include "modules/apf/apf.h"

#undef _GLOBEXT_
#if (defined _cue_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ M A C R O S ________________________________________________________
#if( PASTER == ENABLE )

#define CUE_MAX_NUM     10

enum
{
   CUE_INDEX_BACK,                        // 0
   CUE_INDEX_END,                         // 1

   CUE_INDEX_CHECK,                       // 2

   CUE_INDEX_RECORD,                      // 3
   CUE_INDEX_RECORD_RESUME,               // 4
   CUE_INDEX_RECORD_PAUSE,                // 5
   CUE_INDEX_RECORD_END,                  // 6

   CUE_INDEX_PLAY,                        // 7
   CUE_INDEX_PLAY_RESUME,                 // 8
   CUE_INDEX_PLAY_PAUSE,                  // 9
   CUE_INDEX_PLAY_END,                    // 10

   CUE_INDEX_COPY,                        // 11
   CUE_INDEX_COPY_SRC,                    // 12
   CUE_INDEX_COPY_DEST,                   // 13
   CUE_INDEX_COPY_START,                  // 14
   CUE_INDEX_COPY_END,                    // 15

   CUE_INDEX_ERROR_DISK_FAILED,           // 16
   CUE_INDEX_ERROR_DISK_IS_FULL,          // 17
   CUE_INDEX_ERROR_INVALID_OID,           // 18
   CUE_INDEX_ERROR_FIND_FILE_FAILED,      // 19
   CUE_INDEX_ERROR_CREATE_FILE_FAILED,    // 20
   CUE_INDEX_ERROR_RECORD_START_FAILED,   // 21
   CUE_INDEX_ERROR_PLAY_START_FAILED,     // 22
   CUE_INDEX_ERROR_COPY_SRC_FILE,         // 23
   CUE_INDEX_ERROR_COPY_DEST_FILE,        // 24
   CUE_INDEX_ERROR_COPY_START_FAILED,     // 25
   CUE_INDEX_ERROR_COPY_ABORT,            // 26

   CUE_INDEX_POWER_OFF,                   // 27
   CUE_INDEX_POWER_LOW,                   // 28
   CUE_INDEX_PASTER_START,                // 29
   CUE_INDEX_RECORD_READY,                // 30

   CUE_INDEX_PASTER_POWER_ON,             // 31
   CUE_INDEX_MODE_AP3,                    // 32
   CUE_INDEX_MODE_MP3,                    // 33

   CUE_INDEX_VOLUME,                      // 34
  
   CUE_INDEX_NUM                          // 35
};

//_____ D E F I N I T I O N S ______________________________________________
#define	FS_NAV_ID_CUE             FS_NAV_ID_USER


#define	POS_OID_START       0
#define	POS_DIVISION_INDEX  2
#define	POS_CUE_NUM         4

#define	POS_CUE_STRUCT      8
#define	SIZE_CUE_STRUCT     8

#if 0
extern Bool g_b_cue_file_is_load;
extern Bool g_b_cue_file_is_open;
extern U16 _MEM_TYPE_SLOW_ g_u16_cue_division;

Bool cue_file_open();
void cue_file_close();
Bool cue_file_load();
#define	cue_file_is_load()      g_b_cue_file_is_load
#define	cue_file_is_open()      g_b_cue_file_is_open
#define	cue_file_division()     g_u16_cue_division

#define	cue_is_oid_index_record( index )    (( index )> 0 &&( index )<= cue_file_division())
#define	cue_is_oid_index_play( index )      (( index )> cue_file_division()&&( index )<= 2 * cue_file_division())
#endif

_GLOBEXT_  struct APF_CUE_INFO   _MEM_TYPE_SLOW_   g_cue_info;

_GLOBEXT_   U16   _MEM_TYPE_SLOW_   g_u16_oid_start;
_GLOBEXT_   U16   _MEM_TYPE_SLOW_   g_u16_oid_division;

#define  cue_get_oid_index_start()     g_u16_oid_start
#define  cue_get_oid_index_division()  g_u16_oid_division

//U16 *cue_index_to_unicode( U16 index );
U8 *cue_index_to_unicode( U16 index );

//U32 cue_get_offset(U8 index);
Bool cue_init                   ( void );
Bool cue_play                   ( U8 index );
void cue_off                    ( void );
Bool cue_is_busy                ( void );
//U16  cue_get_oid_index_start    ( void );
//U16  cue_get_oid_index_division ( void );

#endif  //#if( PASTER == ENABLE )

#endif  //_CUE_H_
