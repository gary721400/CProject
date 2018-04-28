//!
//! @file usb_task_sch.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the usb_task_sch.
//!
//! @version 2.5 snd3-dvk-1_9_5 $Id: usb_task_sch.c,v 2.5 2007/07/24 15:27:39 sguyon Exp $
//!
//! @todo
//! @bug


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
//#include "conf_usb.h"

// Include for mail box
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"

#include "modules\usb\usb_task.h"
#include "modules\power\power.h"             // Power module definition

//#include "lib_mcu\usb\usb_drv.h"
//#include "lib_mcu\int\int_drv.h"
//#include "lib_mcu\clock\clock_drv.h"

#include "usb_task_sch.h"
//#include "usb_action.h"
//#include "header.h"
#include "sio\com.h"
#include "..\..\conf\conf_oid.h"
//#include "modules\file_system\navigation.h"
//#include "intrins.h"
#include "drivers\oid\oid.h"
#include "lib_mcu\timer\timer_drv.h"
//#include "lib_mcu\mmc\mmc_drv.h"
//#include "drivers\wireless\master_protocol.h"
//#include "lib_mem\mmc\mmc_sd.h"
#include "mmi\mstorage\mmi_storage.h"
#include "USB_MAssStorage.h"
#include <KBDM9_DDK.H>

#pragma DATA = USB_TASK_SCH_DATA
#pragma CODE = USB_TASK_SCH_CODE

//ImageDeclareType( TASK_INIT_CODE );

ImageDeclareType( USB_STORAGE_TEMP_CODE00 );
ImageDeclareType( USB_STORAGE_TEMP_CODE01 );

//_____ M A C R O S ________________________________________________________

                  Bool           g_b_usb_on = FALSE;
//extern            Bool           b_card_ready;
//extern            Bool           gfSysUSBMP3Mode;
//extern            Bool           fCmdProcess;
//extern   xdata    USB_Variables  USB;

//extern   _MEM_TYPE_MEDSLOW_   U32            usb_charge_time;

//_____ D E F I N I T I O N ________________________________________________


void usb_task_sch_add_evt( U8 u8_evt );


#pragma CODE = TASK_INIT_CODE

//! Initialization of usb task
//!
void usb_task_sch_init( void )
{
}


#pragma CODE = USB_TASK_SCH_CODE

//! This fonction manage the event of usb_task
//!
void usb_task_sch_task( void )
{
   _MEM_TYPE_MALLOC_MAIL_ Msg Cmd;
   if( Power_is_poweroff_activated())
   {
      return;
   }

   // Look usb command
   if( MAILBOX_NO_MSG != mail_get_command( TASK_USB, &Cmd ))
   {
      // Here, manage the command
      switch( Cmd.id )
      {
         /*case CMD_USB_START:
         mail_send_event( EVT_USB_CTRL_POWERED, 0 );
         break;*/
         
         case CMD_USB_DEVICE_START:
         //WTST = 1;// Program Wait States ,for testing Ex_ROM
         g_b_usb_on = TRUE;
         //gfSysUSBMP3Mode = TRUE;
         power_forbid_autopoweroff();
         SDKLoadCode( USB_STORAGE_TEMP_CODE00 );
         usb_task_init();
         break;

         case CMD_USB_DEVICE_STOP:
         //g_b_usb_on = FALSE;
         // un-init USB
         SDKLoadCode( USB_STORAGE_TEMP_CODE01 );
         MassStorage_Exit();

         //SDKLoadCode( TASK_INIT_CODE );
         //timer_init_timer();
         power_authorize_autopoweroff();

#if 0//( WIRELESS == ENABLE )
         if( !g_b_wireless_flag )
            wireless_task_init();
#endif   //#if( WIRELESS == ENABLE )
         break;
      }
   }

   if( g_b_usb_on )
   {
      // Look player event
      usb_task();
   }
}


