//!
//! @file mmgr_kernel.c,v
//!
//! Copyright (c) 2004 Atmel.
//!mailbox
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the mmi_manager
//!
//! The mmi_manager is the task that manage the executing of MMI applications.
//! The kernel of the mmi_manager is organized around an LIFO stack which stores all applications
//! being launching and running at the same time.
//! The last application lauched (or pushed) is placed at the top of the stack and has in consequence the focus
//! in the interaction human - machine.
//!
//! @version 1.61 snd3-dvk-1_9_5 $Id: mmgr_kernel.c,v 1.61 2007/07/26 14:41:50 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmgr_kernel_c_
#define _mmgr_kernel_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "lib_system\auto_disp\auto_disp.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_var.h"
#include "modules\power\power.h"
#include "mmi\kbd\keypad.h"

#include "conf\conf_lcd.h"
#include "conf\conf_mmi.h"
//#include "lib_mem\mmc\mmc_sd.h"
#include "conf\conf_oid.h"
#include "drivers\oid\oid.h"
//#include "modules\search\search_task.h"
#include "sio\com.h"
#include "lib_mcu\timer\timer_drv.h"
//#include "intrins.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "lib_mcu\int\int_drv.h"                // Interrupt definitions
//#include "modules\player_ap3\player_ap3.h"
//#include "modules\search\search_game.h"
//#include "mmi\ap3\mmi_ap3.h"
#include "modules\power\power.h"             // Power module definition
//#include "mmi\player\mmi_player.h"
#include "mmi\mstorage\mmi_storage.h"


#include "conf\conf_access.h"
#include "conf\conf_sio.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "USB_MAssStorage.h"

#include "lib_mcu\lcd\lcd_drv.h"
#include "drivers\lcd\lcd_SSD1303.h"
#include "mmi\images\tab_pict.h"

#pragma DATA = MMGR_KERNEL_DATA
#pragma CODE = MMGR_KERNEL_CODE

//_____ D E C L A R A T I O N ______________________________________________
#if 0//( AP3 == ENABLE )
extern   Bool  g_b_search_on;
#endif
extern   Bool  b_card_state;
//extern   Bool  g_b_usb_on;

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

//static _MEM_TYPE_SLOW_ U8 s_false = FALSE;


//! Store the future index pointing the top application.
//! This variable computes the future index according to the new applications pushed during the task process.
//! Indeed, these new applications can't be directly processed since they haven't started yet.
//! This variable prevents application stack overflow.
//! Each event "EVT_SART_APPLI" makes it possible to retrieve the pushed applications to launch.
_MEM_TYPE_SLOW_   U8          s_future_idx_top_appli;

//! Global array used to point all of the MMI applications.
_MEM_TYPE_SLOW_   Mmi_appli   mmgr_ga_lifo_stack[ MMGR_KERNEL_STACK_SIZE ];

//! Global U8 used by the macros above making possible the MMI appli to let a messages to the kernel
_MEM_TYPE_SLOW_   U8          mmgr_g_msg_to_kernel;

//! Keep the index value on the application lifo stack of the mmi application currently processed.
_MEM_TYPE_SLOW_   U8          mmgr_g_idx_curr_appli;

//! Keep the index value of the application lifo stack slot where the top mmi application has been pushed.
_MEM_TYPE_SLOW_   U8          mmgr_g_idx_top_appli;

//! Retuned value while suiciding mmi application to forward through the event EVT_APPLI_KILLED
_MEM_TYPE_SLOW_   U8          g_ret_value_killed_appli;

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

void  mmgr_init_lifo_stack ( void );
U8    mmgr_push_mmi_appli  (  U8   );
U8    mmgr_pop_mmi_appli   (   U8   );

//extern   U8    _MEM_TYPE_SLOW_   s_b_vbus_present;

//_____ D E F I N I T I O N S ______________________________________________

//! @brief The mmi_manager task in charge of managing the entire MMI.
//!
//! The mmi_manager task is running on messages posted in the mailbox by tasks.
//! These messages can be distributed to the different launched applications
//! in the processing order defined by the LIFO architecture of the stack.
//! An event can be passed from the top application towards the bottom application
//! to the destination application.
//! Each mmi application can catch an event that is destinated to it
//! or forward it to the next lowest-priority application.
//! If any application catch the event, the mmi_manager destroyes it.
//! The first MMI application launched, placed at the bottom of the application stack,
//! is called : mmi_idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none
void mmgr_task(void)
{
   // Boolean "b_evt_back_to_top_stacked" to prevent consecutive posted events "EVT_BACK_TO_TOP"
   // when several applications are killed at the same time
   // This avoids multiple refeshing of a same screen.
   static Bool b_evt_back_to_top_stacked = FALSE;
   _MEM_TYPE_SLOW_ Msg evt;

   // Keypad process
   kbd_process();



#if 0//( OID == ENABLE )
#if 1//(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))
   // Scan oid pen
   if( b_oid_init_flag )
   {
      //if( g_b_oid_enable )
      {
         //Int_disable();
         scan_oid();
         //Int_enable();
      }
   }
#endif   //#if(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))
#endif   //#if( OID == ENABLE )

#if 0//( WIRELESS == ENABLE )
   if( g_b_wireless_flag )
   {
      sio_task();
      wireless_task();
   }
#endif   //#if( WIRELESS == ENABLE )

#if 0//( AP3 == ENABLE )
   search_task();
#endif   //#if( AP3 == ENABLE )

   // Timers software process
   ts_process();

   // Get the eventual event present in the mailbox.
   do
   {
      if( mail_get_event(&evt) == PASS )
      {
         // An event was present in the mailbox.
         // This message is treated below by the mmi manager kernel
         _MEM_TYPE_SLOW_ Mmi_appli * ptr_lifo_stack;
         _MEM_TYPE_SLOW_ U8 i;

         // Event is tested first by the automatic display module.
         // If caught by this last, search the next event from the mailbox.
         if( TRUE == ad_scan_evt(evt) )   { continue; }

         if( LSB(evt.id)==EVT_KEY_PRESSED )
         {
#if (BACKLIGHT == ENABLE)
            if( Lcd_backlight_is_enable() )
            {
               Lcd_rearm_backlight_on();
            }
#endif
#if (POWER_OFF_AUTO == ENABLE)
            power_rearm_autopoweroff_delay();
#endif
#if 0//( AP3 == ENABLE )
            if( g_b_search_on )
            {
               if( !player_ap3_is_busy())
               {
                  GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
                  GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
               }
            }
#endif   //#if( AP3 == ENABLE )
         }

         // Init the global used to pass a message to the kernel
         Mmgr_krn_init_msg_to_kernel();


         // Test if a new application is to launch
         if( LSB(evt.id)==EVT_START_APPLI )
         {
            // Yes, so the stack pointer points on the new top application to launch
            mmgr_g_idx_top_appli++;
         }

         // i has the index of the top application by default
         i = mmgr_g_idx_top_appli;


         // The automatic screen refreshing is launched by the pop of the event "EVT_BACK_TO_TOP".
         // The first step of the automatic screen refreshing is to be positionned
         // on the mmi appli that will give the full-screen background.
         // So, the following code detects the mmi appli that has the feature "MMGR_KERNEL_FULL_SCREEN"
         if( EVT_BACK_TO_TOP == LSB(evt.id) )
         {
            b_evt_back_to_top_stacked = FALSE;      // "EVT_BACK_TO_TOP" is going to be treated
            while( (i>0) && (0 == (mmgr_ga_lifo_stack[i].features & MMGR_KERNEL_FULL_SCREEN)) )
            {
               i--;
            }
         }
         // Effective exit from the power-off mode, kill all non-full screen MMI being at the top
         else if( EVT_POWER_ON == (LSB(evt.id)) )
         {
            for( ; i>=0; i-- )
            {
               ptr_lifo_stack = &mmgr_ga_lifo_stack[i];
               if( 0 != (ptr_lifo_stack->features & MMGR_KERNEL_FULL_SCREEN) )   { break; }
               // Kill the current mmi application
               ( void )mmgr_pop_mmi_appli( ptr_lifo_stack->id_process );
            }
            // This Power event, like the others (see just following), is routed directly to the appli MMI_STATUS
            i = mmgr_g_idx_top_appli;
         }
         // All Power events are routed directly to the first launched appli MMI_STATUS
         else if( EVT_POWER == (LSB(evt.id)&0xf0) )                                          { i=0; }
         // Kbd events are routed by force to MMI_STATUS when in power-off mode
         else if( (EVT_KEY_PRESSED == (LSB(evt.id))) && (Power_is_poweroff_activated()) )    { i=0; }

         // Main loop in which events are treated
         for( ; i>=0; i-- )
         {
            ptr_lifo_stack = &mmgr_ga_lifo_stack[i];

            Mmgr_krn_init_msg_to_kernel();

            if( NO_MMI_APPLI != ptr_lifo_stack->id_module )
            {
               // Save the index of the current mmi application processed
               mmgr_g_idx_curr_appli =  i;

               // Init some display parameters of the MMI applis
               Lcd_init_display_options();

               // And launch the MMI Appli with the event in parameter
               call_mmi_appli(ptr_lifo_stack->id_module, LSB(evt.id), evt.param);
            }

            if( Mmgr_krn_is_this_mmi_appli_to_kill() )
            {
               // Save the the id of the current mmi application
               // before its removing in the lifo stack by the following function "mmgr_pop_mmi_appli"
               _MEM_TYPE_SLOW_ U8 id_process_killed = ptr_lifo_stack->id_process;
               // Kill the current mmi application
               ( void )mmgr_pop_mmi_appli( id_process_killed );
               // Disable keypad events to avoid spurious ones in the new top appli
               if( !Mmgr_is_key_filtering_not_to_do() )
                  { kbd_disable_event(FALSE); }

               if( 0 != i )
               {
                  // Check if the appli just killed was at the top
                  if( (i == mmgr_g_idx_top_appli+1          )
                  &&  (!Mmgr_is_refresh_screen_not_to_do()  )
                  &&  (!Mmgr_is_new_appli_stacked()         )
                  &&  (FALSE == b_evt_back_to_top_stacked   ))
                  {
                     // Yes it is, so we need to prepare for the application placed
                     // above an event to say that it is time to go back to the top
                     // This event will be catched by the mmi manager a scheduler round after.
                     mail_send_event( EVT_BACK_TO_TOP, 0 );
                     b_evt_back_to_top_stacked = TRUE;      // "EVT_BACK_TO_TOP" has just been stacked
                  }

                  // Forward the specific event EVT_APPLI_KILLED (without posting) to the mmi application
                  // that will become the one at the top by the looping with "continue"
                  evt.id         =  EVT_APPLI_KILLED;
                  MSB(evt.param) =  id_process_killed;
                  LSB(evt.param) =  g_ret_value_killed_appli;

                  // This event enables to the next appli,
                  // that is usually the one which has activated the one just killed,
                  // to execute the associated code relative to the suicide of that appli
                  continue;
               }
            }

            // In case of the event "EVT_BACK_TO_TOP",
            // the automatic screen refreshing of mmi applis requires a second step
            // that is to be redraw each mmi appli graphics.
            if( EVT_BACK_TO_TOP == LSB(evt.id) )
            {
               if( i == mmgr_g_idx_top_appli )
               {
                  break;  // The graphic of the top mmi appli has just been drawn. So exit right now.
               }
               else
               {
                  i += 2; // To be positionned on the mmi appli being just above this current one
                  continue;
               }
            }

            // The mmi appli has given back the hand.
            // Do I need to forward the event to the following appli?
            if( !Mmgr_krn_is_forward_current_event() )
            {
               // No event to forward since the current mmi appli has catched it
               break;
            }
            else if( 0 == i )
            {
               // Means that the idle needs to forward to itself an event.
               // To continue inside our loop, we just modify i
               i += 1;
            }
         }

         // Macro effective only if screen building in Qundau, not in LCD
         Lcd_transfert_screen_to_lcd();
      }
   
      // Process the objects of the automatic display module from events caught above.
      ad_process();

   } while( mail_evt_used_room_get() >= (MAILBOX_EVT_SIZE_MAX/2) );

#if ( BACKLIGHT==ENABLE )
   Lcd_shut_backlight_off();
#endif

#if 0//( MEM_MMC != DISABLED )
   // Check event about memory
   //if( g_select_appli != MSTORAGE_APPLI )
   //if( !g_b_usb_on )
   {
      if( b_card_state != Mmc_card_present())
      {
         //mail_send_event( EVT_EXP_MEMORY_MMCSD_CHANGE ,0 );
         //USB.LunStatus[ USB.CurrentLun ] = STATUS_MEDIA_NO_PRESENT; //@1026
         //SDVar.CardCapacity = 0;   
         //Port3_SDIFSave();  // Mason@1018
         SendString((U8 *)"remove\r\n");
         RESET_SYS();
      }
   }
#endif

#if 0//( EARPHONE_DET == ENABLE )
   if( g_b_earphone_present != earphone_is_present())
   {
      g_b_earphone_present = earphone_is_present();
      if( g_b_earphone_present )
      {
         op_disable();
         SendString((U8 *)"op disable\r\n");
      }
      else
      {
         //extern Bool g_b_volume_on;
         //extern _MEM_TYPE_SLOW_ U8 g_u8_player_volume_l;
         //if( g_b_volume_on && g_u8_player_volume_l )
         {
            op_enable();
            u32_op_delay_off_time = 0;
            SendString((U8 *)"op enable\r\n");
         }
      }
   }
   if( u32_op_delay_off_time && Timer_is_timeout( u32_op_delay_off_time ))
   {
      u32_op_delay_off_time = 0;
      op_disable();
   }
#endif   //#if( EARPHONE_DET == ENABLE )
   /*if( g_b_mmc_sd_write )
   {
      g_b_mmc_sd_write = FALSE;
      mail_send_event( EVT_USB_DEVICE_RECEIV, 0 );
   }*/
}


void __interrupt pb_sd_isr( void )
{
   if( sd_det_pif())
   {
      //clr_sd_det_pif();
      //if( b_card_state == Mmc_card_present())
       //  return;
      PBICR = 0x00;
      //sd_det_disable();
      //RESET_SYS();
      RTCWDT = 0xBFFFu;//reset system
   }
   PBPIF = 0;
}
#if 0
void __interrupt rtc_isr( void )
{
  b_flag_rtc = TRUE;
  RTCHDT = 0x0000;
  RTCMST = 0;
  RTCTBR = 0x0010;
 __asm{
  	bset RTCMST.15
  	bset RTCICR.0
    bset RTCTBR.15
    }
}
#endif
#pragma CODE = TASK_INIT_CODE
#if 0
void init_rtc_timer(void)
{

 SendString((U8 *)"\r\ninit_rtc_timer");
//  __asm{bset MODCON.13}
//  MODCON |= 0x20;
	//__asm{ BSET MODCON.13 }
//  g_setting_list.struct_setting.min = 0;	
  b_flag_rtc = FALSE;
// return;
 if(!b_rtc_set_flag){
	 b_rtc_set_flag = TRUE;
	 RTCHDT = 0x0000;
	 // RTCMST = g_setting_list.struct_setting.min<<4;
	 RTCMST = 0;
	 RTCTBR = 0x0010;
	}

  SetVector( RTC_INT_TRAP , rtc_isr );
//  g_setting_list.struct_setting.min ++;
//  RTCTBR = g_setting_list.struct_setting.min<<4;

  __asm{
  	bset MODCON.13
  	bset RTCMST.15
  	bset RTCICR.0
    bset RTCTBR.15
    }
/*  
  SendString((U8 *)"\r\nset time=");
  send_hex_u8((0x1F&RTCHDT));
  SendString((U8 *)"/");
  send_hex_u8((0x1F&(RTCMST>>6)));
*/  
}
#endif
//!
//! @brief Initialisation of the mmi_manager
//!
//! Set to 0 the general LIFO stack
//! And activate the first MMI application : the idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none
void mmgr_task_init( void )
{
   __X FS_ERR err;

   FSInit( 1 );
#if USE_SD
#if 0
   if( FS_mount( &SD_Drive )== FS_DRIVE_ERROR )
   {
      RESET_SYS();
   }
#else
   usb_key_enable();
   err = FS_mount( &SD_Drive );
   switch( err )
   {
      /*case FS_DRIVE_ERROR:
      RESET_SYS();
      break;*/

      case FS_NO_ERROR:
      case FS_FORMAT_ERROR:
      break;

      default:
      usb_key_disable();
      break;
   }
#endif
   SD1_fast_mode();
#endif

#if 0//USE_NF
   if( FS_mount( &NF_Drive )== FS_DRIVE_ERROR )
   { 
      RESET_SYS();
   }
#endif

   // Init the LIFO
   mmgr_init_lifo_stack();

   timer_init_timer();
   
   // Init the software timers
   ts_init();

   // Init the keypad
   kbd_init_process();
#if( EARPHONE_DET == ENABLE )
   g_b_earphone_present = earphone_is_present();
   //if( g_b_earphone_present )
      op_disable();
   //else
      //op_enable();
#endif   //#if( EARPHONE_DET == ENABLE )

   //green_led_on();
   green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
   //ap3_poweron_state = MMI_AP3_NORMAL;//MMI_AP3_POWERON;

   sd_det_disable();
	_bitSET( SD_DET_DIR );
  	_bitSET( SD_DET_PH );
	PBPIF = 0x00;
   b_card_state = Mmc_card_present();
   sd_det_enable();
   if( !b_card_state )
      sd_det_rise();
   else
      sd_det_fall();
   PBICR = 0x31;
   SetVector( PORT_B_INT_TRAP, pb_sd_isr );
#if 0//( OID == ENABLE )
	//³õÊ¼»¯OID pen
	b_oid_init_flag = FALSE;
#endif  //#if( OID == ENABLE )

#if 0//( AP3 == ENABLE )
   search_task_init();
#endif   //#if( AP3 == ENABLE )

#if 0//( WIRELESS == ENABLE )
   g_b_wireless_flag = FALSE;
   if( !power_is_vbus_present())
      wireless_task_init();
#endif   //#if( WIRELESS == ENABLE )
   
   // Init the automatic display
   ad_init();


if(!g_b_V1Hard_Flag){
		SDKLoadCode(KEYPAD_DRV_V2_CODE);
	 	( void )kbd_V2_decode();
	  	}
	  	else{
			SDKLoadCode(KEYPAD_DRV_V1_CODE);
			( void )kbd_V1_decode();
			}

//	init_rtc_timer();
   // Launch and activate the MMI_Status application
   ( void )mmgr_activate_mmi_appli(MMI_APPLI_STATUS, 0);  
}

//!
//! @brief Initialisation of the LIFO Stack
//!
//! Set to 0 the general LIFO stack
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none
void mmgr_init_lifo_stack( void )
{
   Mmi_appli _MEM_TYPE_SLOW_ * ptr_lifo_stack = &mmgr_ga_lifo_stack[MMGR_KERNEL_STACK_SIZE -1];
   _MEM_TYPE_SLOW_ U8         i              = MMGR_KERNEL_STACK_SIZE;

   do
   { // Range of the id value : from 1 to MMGR_KERNEL_SIZE_MAX; 0 is kept for MMI_APPLI_UNKNOWN
      ptr_lifo_stack->id_process  =  i;
      ptr_lifo_stack->id_module   =  NO_MMI_APPLI;
      ptr_lifo_stack->features    =  0;
      ptr_lifo_stack--;
   }
   while( --i != 0 );

   mmgr_g_idx_top_appli    = (U8)(0 -1);  // -1 because of the incrementation at the time of EVT_START_APPLI
   s_future_idx_top_appli  = (U8)(0 -1);  // -1 because of the incrementation in push_mmi_appli()
}


#pragma CODE = MMGR_KERNEL_CODE

//!
//! @brief PushMmiAppli
//!
//! Push the MMI application inside the stack of application
//! If no more place on the stack we return an error : KERNEL_STACK_FULL
//! The stack has the behaviour of a LIFO stack.
//! Keep into memory the adress of the new application put at the top of the
//! Stack.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id_module: ID of the application to push
//!
//! @return U8:
//!    0x00     -->   If an error (id not defined or application stack full)
//!    Id       -->   The process id of the application just been stacked
U8 mmgr_push_mmi_appli( U8 id_module )
{
   if( NO_MMI_APPLI == id_module )
      { return( MMI_APPLI_NOT_DEFINED ); }

   if( (MMGR_KERNEL_STACK_SIZE-1) == s_future_idx_top_appli )
      { return( MMI_APPLI_NOT_DEFINED ); }  // Stack is full, so exit right now.

   s_future_idx_top_appli++;
   mmgr_ga_lifo_stack[s_future_idx_top_appli].id_module = id_module;

   return( mmgr_ga_lifo_stack[s_future_idx_top_appli].id_process );
}

//!
//! @brief PopMmiAppli
//!
//! Pop the MMI application from the Lifo stack
//! Just remove the application from the stack
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8 id_process : The stacking ID of the application to remove
//!
//! @return U8:
//!    MMGR_KERNEL_OK           -->   Ok, the mmi application has been removed
//!    MMGR_KERNEL_ERROR        -->   Some problems has occured
U8 mmgr_pop_mmi_appli( U8 id_process )
{
   U8 i;

   if( id_process <= 0x01 )
   { // The id value equals to 1 is always the one of the appli IDLE.
     // The id value equals to 0 is reserved for MMI_APPLI_NOT_DEFINED value
     // As the appli IDLE must always stay alive, exit from here right now
      return( MMGR_KERNEL_ERROR );
   }

   // Search  in the stack the position of the appli to pop
   i = s_future_idx_top_appli;
   while (mmgr_ga_lifo_stack[i].id_process != id_process)
   {
      if( 0 == i-- )
      {
         // Do not continue since the bottom of the stack has been reached and this id is unknown
         return( MMGR_KERNEL_ERROR );
      }
   }

   // Slide all mmi applications from the one that is to delete
   for( ; i<s_future_idx_top_appli; i++ )
   {
      mmgr_ga_lifo_stack[i] = mmgr_ga_lifo_stack[i+1];
   }

   // And set the last slot as not mmi application defined because of the sliding
   mmgr_ga_lifo_stack[i].id_process  = id_process;
   mmgr_ga_lifo_stack[i].id_module   = NO_MMI_APPLI;
   mmgr_ga_lifo_stack[i].features    =  0;

   // One application in less in the stack
   mmgr_g_idx_top_appli--;
   s_future_idx_top_appli--;

   // Notify all was succeed
   return( MMGR_KERNEL_OK );
}

//!
//! @brief ActivateMmiAppli
//!
//! Push the MMI application inside the application stack
//! Activating an application means thats a new "instanciation" will be created
//! so we need to post the start event : EVT_START_APPLI to be able to catch
//! the first entry point of the selected application
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  ptr_appli: Id of the application to push
//!        U16 param    : Optionnal parameter
//!
//! @return U8:
//!    0x00     -->   If an error
//!    Id       -->   The id of the application launched
U8 mmgr_activate_mmi_appli( U8 id_module, U16 param )
{

   _MEM_TYPE_SLOW_ U8 process_id   =  mmgr_push_mmi_appli( id_module );

   if( MMI_APPLI_NOT_DEFINED != process_id )
   {
      // The pushing of a new application in the stack has succeed
      // The code of that application will be executed
      // with the help of the start event posted as follows
      mail_send_event( EVT_START_APPLI, param );
      Mmgr_new_appli_is_stacked();
   }
   return process_id;
}


//!
//! @brief IsAppliAtTheTop
//!
//! Test if the MMI application, identified by its id, is at the top in the application stack
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8  id_process: id of the MMI application to test
//!
//! @return U8:
//!    TRUE     -->   The MMI application is at the top (has the focus)
//!    FALSE    -->   The MMI application is not at the top
Bool mmgr_is_appli_at_the_top( U8 id_process )
{
   if( mmgr_ga_lifo_stack[mmgr_g_idx_top_appli].id_process == id_process )
      { return( TRUE );  }
   else
      { return( FALSE ); }
}

//!
//! @brief GetProcessIdCurrAppli
//!
//! Get the process id of the current MMI application
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return U8:
//!    ID       -->   process id of the current application
U8 mmgr_get_process_id_curr_appli( void )
{
   return( mmgr_ga_lifo_stack[mmgr_g_idx_curr_appli].id_process );
}


//!
//! @brief GetModuleIdAppli
//!
//! Get the module id of a MMI application with a index as parameter
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  idx   (-1) -> appli just below
//!               ( 0) -> current appli, not useful!!!
//!               (+1) -> appli just above
//!
//! @return U8:
//!    ID       -->   module id of an application
U8 mmgr_get_module_id_appli( S8 idx )
{
   return( mmgr_ga_lifo_stack[mmgr_g_idx_curr_appli+idx].id_module );
}

//!
//! @brief SetAppliFullScreen
//!
//! Set the current appli being full screen. 
//! This information saved in the mmi kernel is useful for the screen refresh
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void mmgr_set_appli_full_screen( void )
{
   mmgr_ga_lifo_stack[mmgr_g_idx_curr_appli].features |= MMGR_KERNEL_FULL_SCREEN;
}


//!
//! @brief MmgrIsTopAppliFullScreen
//!
//! Test if  the top MMI application is full screen or not
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return Bool:
Bool mmgr_is_top_appli_full_screen( void )
{
   return( (mmgr_ga_lifo_stack[mmgr_g_idx_top_appli].features & MMGR_KERNEL_FULL_SCREEN) );
}

#endif // _mmgr_kernel_c_
