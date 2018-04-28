//! @file conf_scheduler.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the scheduler
//! This file will be given to any external customer
//!
//! @version 1.16 snd3-dvk-1_9_5 $Id: conf_scheduler.h,v 1.16 2007/05/25 15:47:13 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_SCHEDULER_H_
#define _CONF_SCHEDULER_H_

//_____  I N C L U D E S ___________________________________________________

#include "conf\conf_sio.h"

//! --- Scheduler Configuration --- 
// ***** Task init *****
#define Scheduler_task_1_init    mmgr_task_init          // Init the MMI task
#define Scheduler_task_2_init    power_task_init         // Init the Power task
//#define Scheduler_task_3_init    usb_task_sch_init         // Init the USB task
#define Scheduler_task_4_init    player_task_init        // Init the Player task
//#define Scheduler_task_5_init    explorer_task_init      // Init the explorer task
//#define Scheduler_task_6_init    update_task_init        // Init the update task
#define Scheduler_task_7_init    recorder_task_init      // Init the Recorder task
//#define Scheduler_task_8_init    viewer_task_init        // Init the Viewer task

#if 0//( PASTER == ENABLE )
#define Scheduler_task_8_init    paster_task_init        // Init the Paster task
#endif  //#if( PASTER == ENABLE )


#if 0//( DIC == ENABLE )
#define Scheduler_task_10_init   dic_task_init           // Init the Dictionary task
#endif

// ***** Task definition *****
#define Scheduler_task_1         mmgr_task               // The MMI manager task
#define Scheduler_task_2         power_task              // Power task
//#define Scheduler_task_3         usb_task_sch_task       // USB task
#define Scheduler_task_4         player_task             // Player task
//#define Scheduler_task_5         explorer_task           // Explorer task
//#define Scheduler_task_6         update_task             // Update task
#define Scheduler_task_7         recorder_task           // Recorder task
//#define Scheduler_task_8         viewer_task             // Viewer task

#if 0//( PASTER == ENABLE )
#define Scheduler_task_8         paster_task             // Paster task
#endif  //#if( PASTER == ENABLE )


#if 0//( DIC == ENABLE )
#define Scheduler_task_10        dic_task                // Dictionary task
#endif   //#if( DIC == ENABLE )

// ***** Type of scheduler *****
// Possible value: SCH_TIMED, SCH_TASK, SCH_FREE
#define SCHEDULER_TYPE           SCHEDULER_FREE

#endif  //! _CONF_SCHEDULER_H_

