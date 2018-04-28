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
#ifndef _MMGR_KERNEL_H_
#define _MMGR_KERNEL_H_

#include "config.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmgr_kernel_c_)
   #define _GLOBEXT_ extern
#else                                        
   #define _GLOBEXT_ extern
#endif


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


//!  Init the global used to pass a message to the kernel
#define Mmgr_krn_init_msg_to_kernel()     (mmgr_g_msg_to_kernel=MMGR_KERNEL_ERROR)

//!  Macro that will kill a MMI application in the MMI
#define Mmgr_kill_this_mmi_appli()        Mmgr_kill_this_mmi_appli_with_ret_val(0)

//!  Macro that will kill a MMI application with parameter to forward
#define Mmgr_kill_this_mmi_appli_with_ret_val(val)    \
{                                                     \
   mmgr_g_msg_to_kernel |= MMGR_KERNEL_KILL_ME;       \
   g_ret_value_killed_appli = val;                    \
}

//!  Test if the current MMI application is to kill
#define Mmgr_krn_is_this_mmi_appli_to_kill()    (0 != (mmgr_g_msg_to_kernel & MMGR_KERNEL_KILL_ME))

//!  Forward the current event
#define Mmgr_krn_forward_current_event()     (mmgr_g_msg_to_kernel |= MMGR_KERNEL_FORWARD_EVT)

//!  Test if the current event is to make follow to the following below MMI application
#define Mmgr_krn_is_forward_current_event()  (0 != (mmgr_g_msg_to_kernel & MMGR_KERNEL_FORWARD_EVT))

//!  Test if the current MMI application is at the top
#define Mmgr_is_this_appli_at_the_top()                        \
         (                                                     \
            (mmgr_g_idx_curr_appli == mmgr_g_idx_top_appli)    \
         )

//!  Inform that the refreshing of screen is not be done following the killing of the appli
#define Mmgr_do_not_refresh_screen_after_killing()    (mmgr_g_msg_to_kernel |= MMGR_KERNEL_DO_NOT_REFRESH)
//!  Test if the refreshing of screen is not to do
#define Mmgr_is_refresh_screen_not_to_do()            (0 != (mmgr_g_msg_to_kernel & MMGR_KERNEL_DO_NOT_REFRESH))

//!  Inform that a request has be done to launch a new appli
#define Mmgr_new_appli_is_stacked()             (mmgr_g_msg_to_kernel |= MMGR_KERNEL_APPLI_LAUNCHED)
//!  Test if a request has be done to launch a new appli
#define Mmgr_is_new_appli_stacked()             (0 != (mmgr_g_msg_to_kernel & MMGR_KERNEL_APPLI_LAUNCHED))

//!  Prevent MMI manager from filtering key events
#define Mmgr_do_not_filter_key_pressed()        (mmgr_g_msg_to_kernel |= MMGR_KERNEL_NO_KEY_FILTERING)
//!  Allow MMI manager to filter key events
#define Mmgr_is_key_filtering_not_to_do()       (0 != (mmgr_g_msg_to_kernel & MMGR_KERNEL_NO_KEY_FILTERING))

//!  Set the variable at the ID value "no appli"
//!    Used after that its mmi application has been killed
#define Mmgr_set_id_appli_not_defined( id_process )     (id_process = MMI_APPLI_NOT_DEFINED)


//_____ D E C L A R A T I O N S ____________________________________________

typedef struct 
{
   U8 id_process;  //! Unique id used to manage internally the mmi application stack
   U8 id_module;   //! Id of MMI applications defined by the user in "com_appli.h"
   U8 features;    //! Features of the mmi appli stored in the way of bit field
}Mmi_appli;


//! Global array used to point all of the MMI applications.
_GLOBEXT_ _MEM_TYPE_SLOW_ Mmi_appli mmgr_ga_lifo_stack[MMGR_KERNEL_STACK_SIZE];

//! Global U8 used by the macros above making possible the MMI appli to let a messages to the kernel
_GLOBEXT_ _MEM_TYPE_SLOW_ U8 mmgr_g_msg_to_kernel;

//! Keep the index value on the application lifo stack of the mmi application currently processed.
_GLOBEXT_ _MEM_TYPE_SLOW_ U8 mmgr_g_idx_curr_appli;

//! Keep the index value of the application lifo stack slot where the top mmi application has been pushed.
_GLOBEXT_ _MEM_TYPE_SLOW_ U8 mmgr_g_idx_top_appli;

//! Retuned value while suiciding mmi application to forward through the event EVT_APPLI_KILLED
_GLOBEXT_ _MEM_TYPE_SLOW_ U8 g_ret_value_killed_appli;


//_GLOBEXT_ _MEM_TYPE_SLOW_  U32  usb_nosd_timer;


//---------------------------------
ImageDeclareType(KEYPAD_DRV_V2_CODE);
ImageDeclareType(KEYPAD_DRV_V1_CODE);


//_____ D E C L A R A T I O N S ____________________________________________

_GLOBEXT_   void     mmgr_task_init(                 void                        );
_GLOBEXT_   void     mmgr_task(                      void                        );
_GLOBEXT_   U8       mmgr_activate_mmi_appli(        U8 id_appli, U16 param);
_GLOBEXT_   Bool     mmgr_is_appli_at_the_top(       U8 id_process         );
_GLOBEXT_   U8       mmgr_get_process_id_curr_appli( void                        );
_GLOBEXT_   U8       mmgr_get_module_id_appli(       S8 idx                      );
_GLOBEXT_   void     mmgr_set_appli_full_screen(     void                        );
_GLOBEXT_   Bool     mmgr_is_top_appli_full_screen(  void                        );


#endif  //! _MMGR_KERNEL_H_
