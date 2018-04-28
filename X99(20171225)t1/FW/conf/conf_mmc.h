//! @file conf_mmc.h,v
//!
//! Copyright (c) 2006
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the MMC
//! This file will be given to any external customer
//!
//! @version 1.2 snd3-dvk-1_9_5 $Id: conf_mmc.h,v 1.2 2006/10/20 07:15:06 coger Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_MMC_H_
#define _CONF_MMC_H_


//! --- Lock/unlock command management configuration ---
//! Values: - ENABLE: secured functions implemented
//!         - DISABLE: secured funtions not implemented
#define MMC_CARD_SECU_FUNC  (DISABLE)

//! --- Wide bus configuration ---
//! Values: - ENABLE: allows 4-bit data bus width
//!         - DISABLE: force 1-bit data bus width
#define SD_4_BIT            (ENABLE)


#endif  //! _CONF_MMC_H_


