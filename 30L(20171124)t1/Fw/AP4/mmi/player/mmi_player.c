//!
//! @file mmi_player.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 2.7 snd3-dvk-1_9_5 $Id: mmi_player.c,v 2.7 2007/10/15 09:59:28 sguyon Exp $
//!
//! @todo
//! @bug
//!
#pragma DATA = MMI_PLAYER_DATA
#pragma CODE = MMI_PLAYER_CODE

#ifndef _mmi_player_c_
#define _mmi_player_c_

//_____  I N C L U D E S ___________________________________________________
#include "compiler.h"

#include <stdio.h>
#include "power.h"
#include "explorer.h"
#include "file.h"           // for file management 
#include "com.h"
#include "player_task.h"
#include "power_task.h"
#include "mail_evt.h"
#include "conf_oid.h"
#include "mail.h"
#include "mail_evt.h"
#include "mail_cmd.h"
#include "com_appli.h"
#include "com_evt.h"
#include "com_var.h"
#include "keypad.h"
#include "mmi_player.h"
#include "srv_player.h"

#include "mmgr_kernel.h"
#include "player.h"
//#include "ctrl_access.h"
#include "fat.h"
#include "mmi_ap3.h"
#include "oid.h"
#include "string_1.h"
#include "mmi_startup.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "search_remote.h"
#include "timer_drv.h"

//________________ D E C L A R A T I O N S ___________________


//** DECLARATION TO MANAGE THE BEHAVIOR

// Last state of player (PLAY or STOP)

Bool  mmiplayer_b_restartplay;

const U8	 player_dir[]= "Mp3";


//_____ D E F I N I T I O N S ______________________________________________
 _MEM_TYPE_SLOW_ U32 file_size_count;
 _MEM_TYPE_SLOW_ U32 play_file_size;

extern   _MEM_TYPE_SLOW_ U8 				current_api;
typedef enum
{
   MP3_PLAYER_MODE
,  MP3_PLAYER_NO_FILE
,  MP3_PLAYER_ERROR
}PLAYER_STATE;
//_MEM_TYPE_SLOW_   U8    mp3_player_state;

//** ROUTINES TO MANAGE THE BEHAVIOR
void mmi_player_manage_evt             ( U8 event );
void mmi_player_change                 ( Bool b_direction, Bool b_force_repeat, Bool b_resume_current );
void mmi_player_switchoff              ( Bool b_poweroff );
Bool  mmi_player_playintro             ( void );
Bool  mmi_playerno_file_playintro      ( void );

//_____ D E F I N I T I O N S ______________________________________________
//! Manage of event and behavior
//!
//! @param U8  event
//!        U16 param
//!
void player_mmi_appli( U8 event, U16 param )
{
   switch( event )
   {
      case EVT_PLAYER:
      if(( mp3_player_state == MP3_PLAYER_MODE )
         ||( mp3_player_state == MP3_PLAYER_NO_FILE ))
      {
         if( param == PLAYER_EOF )
         {
            power_authorize_autopoweroff();
            if( mp3_player_state == MP3_PLAYER_MODE )
            {
               mp3_player_state = MP3_PLAYER_ERROR;
               srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               srvplayer_switch_off();  // Power off player
               mail_send_event( EVT_START_PLAYER_APPLI, 0 );
            }
            else
            {
               mp3_player_state = MP3_PLAYER_ERROR;
               if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
                  ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
               {
                  if( power_is_vbus_present())
                  {
                     g_select_appli = IDLE_APPLI;
                     mmi_player_switchoff( FALSE );
                  }
                  else
                  {
                     g_power_off_mode = POWER_OFF;
                     mmi_player_switchoff( TRUE );
                  }
               }
               else
               {
#if( PASTER_MODE == ENABLE )
                  g_select_appli = PASTER_APPLI;
#else
#if( DIC == DISABLE )
                  g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
                  g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //#if( PASTER_MODE == ENABLE )
                  mmi_player_switchoff( FALSE );
               }
            }
         }
      }
      else
      {
         mp3_player_state = MP3_PLAYER_ERROR;
         mmi_player_manage_evt( param );
      }
      break;
      
      case EVT_START_APPLI:
      SendString((unsigned char *)"player\r\n");
//	  led_2_off();
	  current_api = MMI_APPLI_PLAYER;
//	  led_2_start_timer(LED_2_PLAY_TIME);
      mmgr_set_appli_full_screen();    
#if( OID == ENABLE )
      //b_oid_init_flag = FALSE;
      if( !b_oid_init_flag )
      {
         reinit_oid_pen();
      }
#endif   //#if( OID == ENABLE )

      scan_oid_disable();
      mmi_player_kbd_disable();
      mmi_file_flag &= ~MP3_NO_FILE_FLAG;
#if( PASTER_MODE == DISABLE )
      mmi_file_flag |= APF_NO_FILE_FLAG;
#endif   //#if( PASTER_MODE == DISABLE )
#if( DIC == DISABLE )
      mmi_file_flag |= ADB_NO_FILE_FLAG;
#endif   //#if( DIC == DISABLE )
//***********************************************

      mp3_player_state = MP3_PLAYER_ERROR;
      if( !mmi_player_playintro())
      {
         mail_send_event( EVT_START_PLAYER_APPLI, 0 );
      }
      else
         mp3_player_state = MP3_PLAYER_MODE;
      break;

      case EVT_START_PLAYER_APPLI:
//***********************************************
#if( OID == ENABLE )
      bPenLeave = TRUE;
#endif   //#if( OID == ENABLE )
      scan_oid_enable();
      mmi_player_kbd_enable();
/*	  
	 	temp=nav_drive_set( LUN_ID_MMC_SD );
	  send_hex_u8(temp);
	  SendString("\r\n");
	  temp=nav_dir_root();
	  send_hex_u8(temp);
	  SendString("\r\n");
*/	  
      if( nav_drive_set( LUN_ID_MMC_SD )&& nav_dir_root())
      {
         unicode_init_buffer_enc( g_string, G_STRING_SIZE, ENCODING_UTF16BE );
         string_ascii_to_unicode((U8 _MEM_TYPE_SLOW_ *)g_string,(unsigned char *)player_dir );
         if( !nav_filelist_findname( Unicode_get_addr_string( g_string ), FALSE ))
         {
            if( !nav_dir_make( Unicode_get_addr_string( g_string )))
            {
               if( FS_ERR_FILE_EXIST != fs_g_status )
               {
                  mail_send_event( EVT_PLAYER_NO_FILE, 0 );
                  break;
               }
            }
         }
         if( !nav_dir_cd())
         {
            mail_send_event( EVT_PLAYER_NO_FILE, 0 );
            break;
         }
      }
      else
      {
         SendString((unsigned char *)"player disk error\r\n");
         mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         break;
      }
//***********************************************
      // Open explorer
      //exp_init( ext_filemusic );
      if( !exp_open( 0, 0 ))
      {
         mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         return;
      }
      SendString((unsigned char *)"play mp3 file\r\n");
      // Init display
      power_forbid_autopoweroff();
      srvplayer_g_arg.repeat = NORMAL;//REPEAT_ALL;//
      AudioMP3SetEQ(EQ_NOMAL);
//      srvplayer_g_arg.repeat = REPEAT_ALL;//REPEAT_ALL;//
      // Launch play
      mmiplayer_b_restartplay = TRUE;     // automatic start play
      srvplayer_switch_on();
      mmi_player_change( FS_FIND_NEXT, TRUE, TRUE );
      
#if( OID == ENABLE )
      bPenLeave = TRUE;
#endif   //#if( OID == ENABLE )
      break;

      case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;

      case EVT_TIMER:
      break;

      case EVT_KEY_PRESSED:
      if( mp3_player_state != MP3_PLAYER_ERROR )
      {
         if(( KBD_KEY_PRESSED | KBD_KEY_LONGPRESSED | KBD_KEY_REPEAT | KBD_KEY_REPEAT_RELEASED )
            & KEY_STATUS( param ))
         {
            break;
         }
         if( KBD_KEY_RELEASED & KEY_STATUS( param ))
         {
#if 0         
            if(( KEY_ID( param )!= KEY_VOL )
               /*&&( KEY_ID( param )!= KEY_DOWN )*/)
#else
		if(( KEY_ID( param )!= KEY_VOL_DEC)||( KEY_ID( param )!= KEY_VOL_INC))
#endif
			break;
         }
      }
      else
      {
#if 0      
         if( mmi_player_kbd_is_disable())
         	{
         	}
		 else
		 	{
		 	 if(( KEY_ID( param )== KEY_VOL_DEC)&&KBD_KEY_RELEASED == KEY_STATUS( param ))
				{
				 srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
				 break;
				}
			if(( KEY_ID( param )== KEY_VOL_INC)&&KBD_KEY_RELEASED == KEY_STATUS( param ))
				{
				 srvplayer_volume_change( SRVPLAYER_VOL_UP );
				 break;
				}
			 
		 	}   
#endif		 
      }
	 
      if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
#if( PASTER_MODE == ENABLE )	
#if 0
            if( power_is_vbus_present())
            {
               mail_send_event( EVT_USB_POWERED, 0 );
               break;
            }
#endif			
            g_power_off_mode = POWER_OFF;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_select_appli = IDLE_APPLI;
            mmi_player_switchoff( TRUE );
#endif //#if( PASTER_MODE == ENABLE )
            break;

			case KEY_VOL_DEC:
			
			mmi_player_change( FS_FIND_PREV, TRUE, FALSE );
			break;
			case KEY_VOL_INC:
			mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
			break;
#if 0			
            case KEY_VOL:
            if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
            {
               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
            }
            else
            {
               if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
               {
                  mmiplayer_b_restartplay = TRUE;
               }
               else
               {
                  mmiplayer_b_restartplay = TRUE;
                  mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
               }
            }
            break;

            case KEY_DOWN:
            if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
            {
               mmiplayer_b_restartplay = TRUE;
            }
            mmi_player_change( FS_FIND_PREV, TRUE, FALSE );
            break;
#endif

/*
			case KEY_MEN:
#if(PASTER_MODE == ENABLE)				
			g_select_appli = PASTER_APPLI;
			mmi_player_switchoff( FALSE );
#endif		//#if(PASTER_MODE == ENABLE)
#if(PASTER_MODE == DISABLE)
//            Search_Record.news_sys.flag_anjian = 1;
//			Search_Record.news_sys.poweroff_flag = 1;
            g_select_appli = POWER_OFF_APPLI;
            g_power_off_mode = POWER_OFF;
            mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
            Mmgr_kill_this_mmi_appli();
#endif		//#if(PASTER_MODE == DISABLE)
			break;

*/
	
            case KEY_MODE:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#if( PASTER_MODE == ENABLE )
            g_select_appli = PASTER_APPLI;
#else
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#if( DIC == DISABLE )
            bRemoteEnterCal = FALSE;
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //#if( PASTER_MODE == ENABLE )
            mmi_player_switchoff( FALSE );
            break;
         }
      }
//	  SendString("369\r\n");
//	  send_hex_u16();
      if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
			break;
			case KEY_MODE:
            switch( srvplayer_g_player_state )
            {
               case MMI_PLAYER_STATE_PLAY :
               srvplayer_pause();
               power_authorize_autopoweroff();
               break;

               case MMI_PLAYER_STATE_PAUSE :
               srvplayer_restart_play();
               power_forbid_autopoweroff();
               break;
               
               case MMI_PLAYER_STATE_STOP :
               if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
               {
                  mmiplayer_b_restartplay = TRUE;
               }
               else
               {
                  mmiplayer_b_restartplay = TRUE;
                  mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
               }
               break;
            }
            break;
			
            case KEY_VOL_DEC:
//			SendString("403\r\n");
		 	srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
		    break;
			case KEY_VOL_INC:
//			SendString("406\r\n");
		 	srvplayer_volume_change( SRVPLAYER_VOL_UP );
		    break;
#if 0			
            case KEY_VOL:
            if( g_b_volume_change )
               srvplayer_volume_change( SRVPLAYER_VOL_UP );
            else
               srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            break;

            case KEY_DOWN:
            srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            break;
#endif
         }
      }
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_player_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_player_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_select_appli = IDLE_APPLI;
      mmi_player_switchoff( TRUE );
      break;

      
//***********************************************
#if 0
      case EVT_USB_POWERED:
      mmi_player_switchoff( FALSE );
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_select_appli = MSTORAGE_APPLI;
//      mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      break;
#endif
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      if( mp3_player_state != MP3_PLAYER_ERROR )
         break;
      switch( param )
      {
         case REMOTE_VOL_UP:
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         break;

         case REMOTE_VOL_DOWN:
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         break;

         case REMOTE_POWER_OFF:
         if( power_is_vbus_present())
            break;
         g_power_off_mode = POWER_OFF;
         g_b_remote_on = FALSE;
         mmi_player_switchoff( TRUE );
         break;

#if( REMOTE_MP3 == ENABLE )
         case REMOTE_EXIT_MP3:
         //bRemoteExitMp3 = TRUE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;

         case REMOTE_MP3_PLAY_PAUSE:
         switch( srvplayer_g_player_state )
         {
            case MMI_PLAYER_STATE_PLAY :
            srvplayer_pause();
            power_authorize_autopoweroff();
            break;

            case MMI_PLAYER_STATE_PAUSE :
            srvplayer_restart_play();
            power_forbid_autopoweroff();
            break;
               
            case MMI_PLAYER_STATE_STOP :
            if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
            {
               mmiplayer_b_restartplay = TRUE;
            }
            else
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
            }
            break;
         }
         break;

         case REMOTE_MP3_PREV:
         if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
         {
            mmiplayer_b_restartplay = TRUE;
         }
         mmi_player_change( FS_FIND_PREV, TRUE, FALSE );
         break;

         case REMOTE_MP3_NEXT:
         if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
         {
            mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
         }
         else
         {
            if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
            {
               mmiplayer_b_restartplay = TRUE;
            }
            else
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
            }
         }
         break;
#endif   //#if( REMOTE_MP3 == ENABLE )

         case REMOTE_CAL_MODE:
         bRemoteEnterCal = TRUE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         bRemoteEnterCal = FALSE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;

#if( REMOTE_REC == ENABLE )
         case REMOTE_REC_MODE:
         break;
#endif   //#if( REMOTE_REC == ENABLE )

         /*case REMOTE_MP3_MODE:
         case REMOTE_REC_START:
         case REMOTE_REC_PLAY:
         case REMOTE_REC_STOP:
         break;*/

#if( REMOTE_PASTER == ENABLE )
         case REMOTE_PASTER_MODE:
         g_select_appli = PASTER_APPLI;
         mmi_player_switchoff( FALSE );
         break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
         case REMOTE_DIC_MODE:
         g_select_appli = DIC_APPLI;
         mmi_player_switchoff( FALSE );
         break;
#endif   //#if( REMOTE_DIC == ENABLE )
      }
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      case EVT_PLAYER_NO_FILE:
      //SendString("PLAYER_NO_FILE\r\n");
      scan_oid_disable();
      mmi_player_kbd_disable();
      mp3_player_state = MP3_PLAYER_ERROR;
      mmi_file_flag |= MP3_NO_FILE_FLAG;
      if( !mmi_playerno_file_playintro())
      {
         if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
            ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         {
            if( power_is_vbus_present())
            {
               g_select_appli = IDLE_APPLI;
               mmi_player_switchoff( FALSE );
            }
            else
            {
               g_power_off_mode = POWER_OFF;
               mmi_player_switchoff( TRUE );
            }
         }
         else
         {
#if( PASTER_MODE == ENABLE )
            g_select_appli = PASTER_APPLI;
#else
#if( DIC == DISABLE )
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //#if( PASTER_MODE == ENABLE )
            mmi_player_switchoff( FALSE );
         }
      }
      else
      {
         mp3_player_state = MP3_PLAYER_NO_FILE;
      }
      break;
//***********************************************

      case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;

      default:
      Mmgr_krn_forward_current_event();
      break;
   }
}


//! Process the player events
//!
void mmi_player_manage_evt( U8 event )
{
   switch( event )
   {
      case PLAYER_EOF:                    // End of file, the player is stopped
      case PLAYER_BOF:                    // Beginning of file, the player is stopped
      // End of play then reset state of player
      srvplayer_g_player_state            = MMI_PLAYER_STATE_STOP;
      mmi_player_change( (PLAYER_EOF==event)? FS_FIND_NEXT : FS_FIND_PREV , FALSE, FALSE );
      break;
   }
}


//! Manage the status of the file(s) and play
//!
void mmi_player_change( Bool b_direction, Bool b_force_repeat, Bool b_resume_current )
{
   U8 u8_stateplay;
   U8 u8_loop = 0;
   
   srvplayer_stop();       // in case of

   if( b_resume_current )
      goto mmi_player_change_playfile;

   while(1)
   {
      //*** Go to next file
      // When you are on repeat single mode you play the same file
      if((REPEAT_SINGLE == srvplayer_g_arg.repeat)
      || (SINGLE        == srvplayer_g_arg.repeat))
      {
         u8_loop++;
         goto mmi_player_change_playfile;
      }
         
      // Mov in explorer API
      switch( exp_mov(b_direction) )
      {
         case EXP_MOV_OK_LOOP:
         u8_loop++;
         case EXP_MOV_OK:
         break;
   
         // Open loop error or empty
         case EXP_MOV_EMPTY:
         //mmi_player_switchoff(FALSE);
         //return;
         case EXP_MOV_DISKNOPRESENT:
         //mmi_player_switchoff(FALSE);
         //return;
         case EXP_MOV_DISKERROR:
         /*g_select_appli = IDLE_APPLI;
         mmi_player_switchoff(FALSE);*/
         mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         return;
      }
mmi_player_change_playfile:
      if( u8_loop )
      {
         if((REPEAT_SINGLE != srvplayer_g_arg.repeat )
         && (REPEAT_ALL    != srvplayer_g_arg.repeat )
         && (!b_force_repeat) )
         {
            mmiplayer_b_restartplay = FALSE;  // Stop play
            power_authorize_autopoweroff();
         }
         if( 1 < u8_loop )
         {
            // Loop empty or all fail
            /*g_select_appli = IDLE_APPLI;
            mmi_player_switchoff(FALSE);*/
            mail_send_event( EVT_PLAYER_NO_FILE, 0 );
            return;
         }
      }

      srvplayer_file_getinfos();
//***********************************************
      //add by xuanyu, because cannot codec if the file size is 0
      if( fs_g_nav_entry.u32_size < 1024 )
         continue;
//***********************************************
      if( mmiplayer_b_restartplay )
      {
         // Restart play
         u8_stateplay = srvplayer_play( b_resume_current );
         b_resume_current = FALSE;
         if( SRVPLAYER_PLAY_OK != u8_stateplay )
         {
            continue;   // Error jump to other file
         }
      }
      break;   // File change
   }
}


//! @brief Turn off the player MMI
//!
void mmi_player_switchoff( Bool b_poweroff )
{
   srvplayer_stop();       // in case of
   srvplayer_switch_off();

   Mmgr_kill_this_mmi_appli();
   power_authorize_autopoweroff();
   if( b_poweroff )
   {
      // Send command to power OFF player
      //mail_send_command(CMD_POWER_OFF, 0);
      g_select_appli = POWER_OFF_APPLI;
   }
}


//***********************************************


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_player_playintro( void )
{
   // Search file to play
   if( !search_subdir_mp3_file((U8 *)"mp3player.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_playerno_file_playintro( void )
{
   // Search file to play
   if( !search_subdir_mp3_file((U8 *)"nomp3file.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}

//***********************************************

#endif // _mmi_player_c_


