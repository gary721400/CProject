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
#ifndef _srv_recorder_c_
#define _srv_recorder_c_


//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include <stdio.h>

#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "srv_recorder.h"

#include "modules\control_access\ctrl_access.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "modules\recorder\recorder_task.h"
#include "modules\recorder\recorder.h"
#include "modules\audio\audio.h"

#include "modules\power\power.h"             // Power module definition
#include "modules\player\player_task.h"
#include "lib_mcu\timer\timer_drv.h"

#pragma DATA = SRV_RECORDER_DATA
#pragma CODE = SRV_RECORDER_CODE

ImageDeclareType( RECORDER_TEMP_CODE00 );
ImageDeclareType( RECORDER_TEMP_CODE01 );
ImageDeclareType( RECORDER_TEMP_CODE02 );
ImageDeclareType( RECORDER_TEMP_CODE03 );
ImageDeclareType( RECORDER_TEMP_CODE04 );
ImageDeclareType( RECORDER_TEMP_CODE05 );

//________________ P R I V A T E   D E C L A R A T I O N S ___________________

//static Bool  b_record_source_mic;

//_____ D E F I N I T I O N S ______________________________________________

//U8    _MEM_TYPE_SLOW_   last_record_number = 1;

RECORD_VALUE   _MEM_TYPE_SLOW_   record_result /*= RECORD_OK*/;


void  record_end_set_led   ( void );

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
   sprintf( g_string, "Recorder" );
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
   //for( u8_i=1; u8_i<100; u8_i++ )
   for( u8_i = last_record_number; u8_i < 100; u8_i++ )
   {
      sprintf( g_string, "rec%02bu.wav" , u8_i );
	   if( nav_file_create( g_string ) )
         break;
      if( FS_ERR_FILE_EXIST != fs_g_status )
         goto srvrec_start_error;
   }
   last_record_number = u8_i;
   if( u8_i >= 100 )
   {
      record_result = RECORD_FILE_FULL;
      goto srvrec_start_error;
   }

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif

   if( !record_on( WAVE_FORMAT_IMA_ADPCM ))
      return FALSE;
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


#pragma CODE = RECORDER_CODE

//! Stop the record
//!
void srvrec_stop( void )
{
   //__asm("BCLR MICICR.0");
   SDKLoadCode( RECORDER_TEMP_CODE03 );

   ( void )record_off();
   //recorder_task_stop();     // Disable event of recorder task
   //power_authorize_autopoweroff();

   SDKLoadCode( RECORDER_TEMP_CODE05 );
   record_end_set_led();
}


#pragma CODE = RECORDER_TEMP_CODE05

void record_end_set_led( void )
{
   g_b_set_led = FALSE;
   if( green_led_is_timer_stopped())
   {
      green_led_on();
      green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
   }
   if( red_led_is_timer_stopped())
   {
      if( g_b_battery_low )
      {
         red_led_on();
         green_led_off();
         green_led_stop_timer();
      }
      else
      {
#if 0
         if( Bat_is_charge_on())
            red_led_on();
         else
#endif
            red_led_off();
      }
      red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
   }
}

#endif

