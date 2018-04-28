//! @file setting.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the setting interface.
//!
//! @version 1.7 snd3-dvk-1_9_5 $Id: setting.h,v 1.7 2007/10/05 13:43:11 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _SETTING_H_
#define _SETTING_H_

#include "conf\conf_update.h"
#include "modules\file_system\fat.h"

Bool setting_save    (U8 *p_struct);
Bool setting_load    (U8 *p_struct);

// Exemple about zone settings, zone system, zone data user, FileSystem management ...
Bool setting_update  ( void );
Bool load_data_via_picture_zone  ( void );

#endif  // _SETTING_H_
