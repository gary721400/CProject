//!
//! @file usb_task_sch.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the action sending by all USB driver
//!
//! @version 2.3 snd3-refd1-1_9_5 $Id: usb_task_sch.h,v 2.3 2007/07/24 15:27:39 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _USB_TASK_SCH_H_
#define _USB_TASK_SCH_H_

          // USB main routines for sheduler
void  usb_task_sch_init                ( void );
void  usb_task_sch_start               ( void );
void  usb_task_sch_stop                ( void );
void  usb_task_sch_task                ( void );
                                      
          // USB DEVICE events        
void  usb_task_sch_device_vbus_on      ( void );
void  usb_task_sch_device_vbus_off     ( void );
void  usb_task_sch_device_enable       ( void );
void  usb_task_sch_device_enum_high    ( void );
void  usb_task_sch_device_enum_full    ( void );
void  usb_task_sch_device_disable      ( void );
void  usb_task_sch_suspend             ( void );
void  usb_task_sch_wakeup              ( void );
void  usb_task_sch_resume              ( void );
void  usb_task_sch_reset               ( void );
                                      
         // USB DEVICE mass storage events
void  usb_task_sch_ms_started          ( void );
void  usb_task_sch_ms_read             ( void );
void  usb_task_sch_ms_write            ( void );

         // USB DEVICE HID events
void  usb_task_sch_hid_send            ( void );

         // USB DEVICE CDC events
void  usb_task_sch_cdc_send            ( void );
void  usb_task_sch_cdc_receiv          ( void );
                                      
          // USB HOST events          
void  usb_task_sch_host_connection     ( void );
void  usb_task_sch_host_disconnection  ( void );
void  usb_task_sch_host_limitation_hw  ( void );
void  usb_task_sch_host_limitation_sw  ( void );
                                      
          // USB HOST mass storage events
void  usb_task_sch_host_ms_change      ( void );
                                      
          // USB HOST hub events      
void  usb_task_sch_host_hub_change     ( void );

          // USB HOST hid events      
void  usb_task_sch_host_hid_change     ( void );
          // USB HOST hid mouse events
void  usb_task_sch_host_hid_mouse_btn1 ( void );
void  usb_task_sch_host_hid_mouse_btn2 ( void );
void  usb_task_sch_host_hid_mouse_btn3 ( void );
void  usb_task_sch_host_hid_mouse_move ( S8 s8_x, S8 s8_y );
          // USB HOST hid keyboard events
void  usb_task_sch_host_hid_kb_key     ( U8 u8_key );

          // USB HOST cdc events      
void  usb_task_sch_host_cdc_change     ( void );

#endif  // _USB_TASK_SCH_H_
