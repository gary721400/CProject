//! @file config.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the system configuration definition
//!
//! @version 2.6 snd3-dvk-1_9_5 $Id: config.h,v 2.6 2007/11/22 17:09:09 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define KEIL
//#define SND3B

//_____ I N C L U D E S ____________________________________________________

#include "compiler.h"                // Compiler definitions
//#include "lib_mcu\mcu_drv.h"                 // Component declaration
//#include "conf_oid.h"

//_____ M A C R O S ________________________________________________________

//#define ATMEL_WARNING    // Define it to display ATMEL warnings

// This flag autorize the quick startup
//#define START_QUICK_MODE
#define TX_DEBUG
//

// Work package version
//#define SYS_LAB                  "POPULAR"





// USB
#define USB_NO_INTERRUPT (DISABLE)

#define _MEM_TYPE_MALLOC_MAIL_   _MEM_TYPE_SLOW_


#define ISP_VERSION              0x05

// To check if update file is compliant with this design (during update task)
// !!! Warning !! device type 0x00000000 is reserved. Please do not use this device type number
//#define DEVICE_TYPE_MSW       0x1234
//#define DEVICE_TYPE_LSW       0x5678
//#define DEVICE_TYPE           ((((U32)DEVICE_TYPE_MSW)<<16) + (U32)DEVICE_TYPE_LSW)  

#define LUN_ID_MMC_SD 			 0
//! Periode of the tick in ms
//! Here the tick is set to 2ms
#define TICK_PERIOD              2


//! Number of timers software available
//#define NB_TIMER_SOFT            10

//! Number of graphical objects available in auto display
//! Warning : each object requires a timer soft for running.
#define NB_AD_OBJ                5


//! Number of MMI applications being able to be processed at the same time
//#define MMGR_KERNEL_STACK_SIZE   4


// keypad configuration
// --------------------
//! Define if keypad is used for power-down mode exit
//! Values: - TRUE:  power-down mode exit by keypad enabled
//!         - FALSE: power-down mode exit by keypad disabled
#define KBD_POWER_DOWN_EXIT      FALSE


//***********************************************

#define	SIZE_OF_ITEM( a )             ( sizeof( a )/ sizeof(( a )[ 0 ]))
//***********************************************
//_____ I N C L U D E S ____________________________________________________

//! INCLUDE PATH and compile options (don't touch)
//#define LIB_MEM         "memory.h"
//#define LIB_CTRLACCESS  "ctrl_access.h"

//#include "option.h"


  
#endif  //! _CONFIG_H_

