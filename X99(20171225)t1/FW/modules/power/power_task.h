//!
//! @file power_task.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the power task.
//!
//! @version 1.1 snd3-dvk-1_9_5 $Id: power_task.h,v 1.1 2004/10/08 11:18:29 arochemo Exp $
//!
//! @todo
//! @bug
#ifndef _POWER_TASK_H_
#define _POWER_TASK_H_

#if (defined _power_task_c_)
   #if(defined _GLOBEXT_)
      #undef _GLOBEXT_
   #endif
   #define _GLOBEXT_
#elif (!defined _GLOBEXT_)
   #define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________
#include "lib_mcu\compiler.h"


_GLOBEXT_   void     power_task_init      (void);
_GLOBEXT_   void     power_task           (void);

#endif  // _POWER_TASK_H_ 
