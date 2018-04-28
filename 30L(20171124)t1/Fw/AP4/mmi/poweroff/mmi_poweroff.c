//!
//! @file mmi_poweroff.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!				
//! @brief This file defines the MMI application status
//!
//! Contains poweroff and animation management
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: mmi_poweroff.c,v 2.3 2007/09/24 09:46:24 sguyon Exp $
//!
//! @todo
//! @bug
//!

#pragma DATA = MMI_POWEROFF_DATA
#pragma CODE = MMI_POWEROFF_CODE

#ifndef _mmi_poweroff_c_
#define _mmi_poweroff_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
//#include "conf_lcd.h"

#include "mail_cmd.h"
#include "mail_evt.h"
#include "timer_soft.h"

#include "com_appli.h"
#include "com_evt.h"
//#include "com_var.h"
#include "keypad.h"
#include "mmi_poweroff.h"

#include <stdio.h>
#include "com.h"

#include "mmgr_kernel.h"

#include "oid.h"
#include "mmi_paster.h"
#include "power.h"             // Power module definition
#include "file.h"           // for file management 
#include "mmi_player.h"
#include "player_task.h"
#include "mmi_startup.h"

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
// ID of the timer soft used by this MMI appli 

// EXEMPLE PLAY ONE FILE
#include "srv_player.h"
#include "player.h"
Bool  mmi_poweroff_playintro        ( void );
Bool  mmi_batterylow_playintro      ( void );
Bool  mmi_paster_poweroff_playintro ( void );


//_____ D E F I N I T I O N S ______________________________________________


//! Manage the application startup
//!
//! @param U8  event
//!        U16 param
//!
void poweroff_mmi_appli(U8 event, U16 param)
{
   switch( event )
   {
      case EVT_START_APPLI:
      scan_oid_disable();
      switch( LSB( param ))
      {
         case POWER_OFF:
/*
         if( paster_exit_status == PASTER_POWER_OFF )
         {
            if( !mmi_paster_poweroff_playintro() )
            {
               // Play fail then startup finish
               Mmgr_kill_this_mmi_appli();
               mail_send_command(CMD_POWER_OFF, 0);
            }
            break;
         }
*/
         // EXEMPLE PLAY ONE FILE
         if( !mmi_poweroff_playintro() )
         {
            // Play fail then startup finish
            SendString((unsigned char *)"poweroff\n");
            Mmgr_kill_this_mmi_appli();
            mail_send_command(CMD_POWER_OFF, 0);
         }
         break;

         case BATTERY_LOW:
         if( !mmi_batterylow_playintro() )
         {
            // Play fail then startup finish
            Mmgr_kill_this_mmi_appli();
            mail_send_command(CMD_POWER_OFF, 0);
         }
         break;
      }
      break;

      case EVT_APPLI_KILLED:
      case EVT_BACK_TO_TOP:
      break;

      case EVT_PLAYER:
      if( param == PLAYER_EOF )
      {
         srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //file_close();
//         srvplayer_switch_off();  // Power off player
         Mmgr_kill_this_mmi_appli();
         mail_send_command(CMD_POWER_OFF, 0);
      }
      break;

      case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;

      //default:
      //break;
   }
}

// EXEMPLE PLAY ONE FILE

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_poweroff_playintro( void )
{
   // Search file to play
   if( !search_subdir_mp3_file((U8 *)"poweroff.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_batterylow_playintro( void )
{
   // Search file to play
   if( !search_subdir_mp3_file((U8 *)"batterylow.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_paster_poweroff_playintro( void )
{
   // Search file to play
   if( !search_subdir_mp3_file((U8 *)"recclose.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}

#endif // _mmi_startup_c_

