//! @file conf_access.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the control access
//! This file will be given to any external customer
//!
//! @version 2.2 snd3-dvk-1_9_5 $Id: conf_access.h,v 2.2 2007/07/24 11:30:11 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_ACCESS_H_
#define _CONF_ACCESS_H_

//#include "conf\conf_kbd.h"

// Active the Logical Unit
//#define  MEM_NF               ENABLED
#define  MEM_NF               DISABLED

//#define  MEM_MMC              DISABLED
#define  MEM_MMC              ENABLED
#define  MEM_USB              DISABLED // only in mode USB HOST


// Specific option control access
#define  GLOBAL_WR_PROTECT    DISABLED // To manage a global write protection

#endif  //! _CONF_ACCESS_H_

