//!
//! @file mmgr_kernel.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the kernel
//!
//! The mmi_manager task is running on messages posted in the mailbox by tasks.
//! These messages can be distributed to the different launched applications
//! in the processing order defined by the LIFO architecture of the stack.
//! An event can be passed from the top application towards the bottom application
//! to the destination application.
//! Each mmi application can catch an event that is destinated to it
//! or forward it to the next lowest-priority application.
//! If any application catch the event, the mmi_manager destroyes it.
//!
//! @version 1.27 snd3-dvk-1_9_5 $Id: mmgr_kernel.h,v 1.27 2007/04/03 10:42:16 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _DIC_KERNEL_H_
#define _DIC_KERNEL_H_

//#include "config.h"

//_____ M A C R O S ________________________________________________________




//_____ M A C R O S ________________________________________________________

// MMI_APPLI_STATUS defined here since always implemented
// All other label of mmi applications are to define in com_appli.h
#define MMI_APPLI_STATUS   0

// NO_MMI_APPLI is to use in the mmi application label definitions when functions are not more available
#define NO_MMI_APPLI       0xFF

//!  ID of the mmi application not stacked
//!  This value '0' must not be changed as the stack management has been designed around it
//!  ( see "mmgr_init_lifo_stack()" )
#define MMI_APPLI_NOT_DEFINED  0

//!  Generic msg for managing the stack and events
//!  (stored in the way of bit field)
#define MMGR_KERNEL_ERROR                 0x00
#define MMGR_KERNEL_OK                    0x01
#define MMGR_KERNEL_FORWARD_EVT           0x02
#define MMGR_KERNEL_KILL_ME               0x04
#define MMGR_KERNEL_FULL_SCREEN           0x08
#define MMGR_KERNEL_DO_NOT_REFRESH        0x10
#define MMGR_KERNEL_APPLI_LAUNCHED        0x20
#define MMGR_KERNEL_NO_KEY_FILTERING      0x40





//extern xdata  U32  usb_nosd_timer;

//_____ D E C L A R A T I O N S ____________________________________________

   void     mmgr_task_init(                 void                        );
   void     mmgr_task(                      void                        );


#endif  //! _MMGR_KERNEL_H_
