//!
//! @file usb_task.c,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file manages the USB task either device/host or both.
//!
//! The USB task selects the correct USB task (usb_device task or usb_host task
//! to be executed depending on the current mode available.
//!
//! According to USB_DEVICE_FEATURE and USB_HOST_FEATURE value (located in conf_usb.h file)
//! The usb_task can be configured to support USB DEVICE mode or USB Host mode or both
//! for a dual role device application.
//!
//! This module also contains the general USB interrupt subroutine. This subroutine is used
//! to detect asynchronous USB events.
//!
//! Note:
//!   - The usb_task belongs to the scheduler, the usb_device_task and usb_host do not, they are called
//!     from the general usb_task
//!   - See conf_usb.h file for more details about the configuration of this module
//!
//!
//! @version 2.2 snd3-dvk-1_9_5 $Id: usb_task.c,v 2.2 2007/06/25 09:02:48 sguyon Exp $
//!
//! @todo
//! @bug

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "..\..\conf\conf_oid.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail.h"
#include "lib_mcu\timer\timer_drv.h"
#include "mmi\kbd\keypad.h"

//#include "memory.h"
#include "stdio.h"
//#include "intrins.h"
#include "sio\com.h"
#include "modules\power\power.h"

//#include "modules\file_system\navigation.h"
#include "drivers\oid\oid.h"
#include "lib_system\mailbox\mail_evt.h"
//#include "lib_mcu\mmc\mmc_drv.h"
//#include "lib_mem\mmc\mmc_sd.h"
#include "mmi\mstorage\mmi_storage.h"
#include "modules\mmi_manager\mmgr_kernel.h"
//#include "modules\search\search.h"

#include "USB_MAssStorage.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "adc.h"

#pragma DATA = USB_TASK_DATA
//#pragma CODE = USB_TASK_CODE
#pragma CODE = USB_STORAGE_CODE

//_____ M A C R O S ________________________________________________________

#define USB_MODE_UNDEFINED            0x00
#define USB_MODE_HOST                 0x01
#define USB_MODE_DEVICE               0x02


//_____ D E F I N I T I O N S ______________________________________________

//! Store the current mode (idle/device/host)
//volatile U8 g_usb_mode;

//! Check the current mode and eventualy start the new mode (UID change, VBUS change, ...)
//! It is call in USB task
//void usb_task_check_mode   ( void );


//_____ D E C L A R A T I O N S ____________________________________________
_MEM_TYPE_SLOW_   U32   usb_nosd_timer;

_MEM_TYPE_SLOW_   U8    usb_count;
                  Bool  b_usb_detection = FALSE;

//U8    _MEM_TYPE_SLOW_   usb_power  = 0;

//extern   Bool  gfSysUSBMP3Mode;  // global flg of system, 1 -> USB mode , 0 ->  MP3 mode
//extern   Bool  fUSBBulkInOut;
extern   Bool  g_b_usb_on;
/*
         _MEM_TYPE_SLOW_   U16            oid_count;
         _MEM_TYPE_SLOW_   U32            Timer_count;
                           Bool           Usb_connect = FALSE;

extern   _MEM_TYPE_SLOW_   U8             g_u8_card_type;
extern   _MEM_TYPE_SLOW_   U32            g_u32_card_rca;
extern   _MEM_TYPE_SLOW_   U32            g_u32_card_size;
extern                     Bool           b_card_ready;
extern   xdata             USB_Variables  USB;
*/

//extern   void  USBSysClkSetUp( void );
//extern   void  ram_xdata_fill( void );


//USB.UsbResetType = 0; before the main(), STARTUP.A51

#pragma CODE = USB_STORAGE_TEMP_CODE00

//! @brief Initializes the USB processus
//!
//! Depending on the mode supported (HOST/DEVICE/DUAL_ROLE) the function
//! calls the coresponding usb mode initialization function
//!
void usb_task_init( void )
{
   //Variable Declare
   //int lun;

   //__asm{ BCLR USBCON.14 };
   //__asm{ BSET USBCON.14 };
   
   //USBStoragePoweroff();
   //USB MassStorage Initial
   MassStorage_Init();

   USBSetDebounceCount( 0x18000 );

   /*if( !USBAttachCheck())
   {
      // check attached or not
      // do your things, for example: mp3 play
      U16 param;

      KEY_ID( param ) =  KEY_POWER;
      KEY_STATUS( param ) = KBD_KEY_LONGPRESSED;
      SendString((U8 *)"usb not attached\r\n");
      //g_b_usb_on = FALSE;
      mail_send_event( EVT_KEY_PRESSED, param );
      return;
   }*/

   // USB Storage Initial
   //lun = 0;

   MassStorage_Registers();

   usb_nosd_timer = Timer_set_timer( TIMER_MS( 4000 ));
   
   Usb_connect = TRUE;
   usb_core_status = 0;
   usb_count = 0;
   b_usb_detection = TRUE;

   __asm{ BSET USBCON.14 };
}


#pragma CODE = USB_STORAGE_CODE

//! @brief Entry point of the USB mamnagement
//!
//! Depending on the USB mode supported (HOST/DEVICE/DUAL_ROLE) the function
//! calls the coresponding usb management function.
//!
void usb_task( void )
{
   if( Usb_connect )
   {
      if( MassStorage_Loop() == USB_DISCONN )
      {
         U16 param;

         //KEY_ID( param ) =  KEY_POWER;
         KEY_ID( param ) =  KEY_MODE;
         KEY_STATUS( param ) = KBD_KEY_LONGPRESSED;
         SendString((U8 *)"usb disconnect\r\n");
         //g_b_usb_on = FALSE;
         mail_send_event( EVT_KEY_PRESSED, param );
      }
   }
   if( !usb_key_is_enable())
   {
      if( Timer_is_timeout( usb_nosd_timer ))
      {
         red_led_flash();
         green_led_off();
         green_led_stop_timer();
         usb_nosd_timer = Timer_set_timer( TIMER_MS( 300 ));
      }
   }
}



