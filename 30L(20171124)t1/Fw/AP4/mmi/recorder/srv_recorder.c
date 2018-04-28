//!
//! @file srv_recorder.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the services for the application RECORDER
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: srv_recorder.c,v 2.0 2007/03/30 12:26:50 sguyon Exp $
//!
//! @todo
//! @bug
//!

#pragma DATA = SRV_RECORDER_DATA
#pragma CODE = SRV_RECORDER_CODE

#ifndef _srv_recorder_c_
#define _srv_recorder_c_


//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include <stdio.h>

#include "mail_evt.h"
#include "mail_cmd.h"

#include "com_appli.h"
#include "com_evt.h"
#include "com_var.h"
#include "srv_recorder.h"

//#include "ctrl_access.h"
#include "mmgr_kernel.h"
#include "recorder_task.h"
#include "recorder.h"
//#include "audio.h"

#include "power.h"             // Power module definition

//________________ P R I V A T E   D E C L A R A T I O N S ___________________

static Bool  b_record_source_mic;

//_____ D E F I N I T I O N S ______________________________________________

#if 0
//! Init service record
//!
void srvrec_init( Bool source_mic )
{
   b_record_source_mic = source_mic;
}


//! Start the record
//!
Bool srvrec_start( void )
{
   U8 u8_i;

   // Go to file record
   //if( !nav_drive_set( LUN_ID_NF_DISKMASS ) )
   if( !nav_drive_set( LUN_ID_MMC_SD ) )
      return FALSE;
   if( !nav_partition_mount() )
      return FALSE;
   
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_ascii();  // ASCII mode for name of copy file
#endif

   // Make record folder
   sprintf((char *)g_string, "record" );
   if( !nav_dir_make( g_string ) )
   {
      if( FS_ERR_FILE_EXIST != fs_g_status )
      {
         goto srvrec_start_error;
      }
   }
   if( !nav_dir_cd() )
      goto srvrec_start_error;

   // Search last record file
   for( u8_i=0; u8_i<100; u8_i++ )
   {
      sprintf( g_string, "record%03bu.wav" , u8_i );
	   if( nav_file_create( g_string ) )
         break;
      if( FS_ERR_FILE_EXIST != fs_g_status )
         goto srvrec_start_error;
   }
   if( u8_i == 100 )
      goto srvrec_start_error;
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif

   if( !b_record_source_mic )
   {
      if( !record_on( AUDIO_IN_MIC_SOURCE,   CODEC_REC_G726 ))
         return FALSE;
   }else{
      if( !record_on( AUDIO_IN_LINE_SOURCE,  CODEC_REC_G726 ))
         return FALSE;
   }      
   if( !record_start())
      return FALSE;
   recorder_task_start();    // Enable event of recorder task
   return TRUE;

srvrec_start_error:
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif
   return FALSE;
}
#endif
#if 0
//! Stop the record
//!
void srvrec_stop( void )
{
//   record_off();
//   recorder_task_stop();     // Disable event of recorder task
//***********************************************

//***********************************************
}
#endif 
#endif

