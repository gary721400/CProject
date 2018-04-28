//!
//! @file mail_cmd.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of all the commands that could be find
//! within the embedded SW.
//! This file can be easily updated by anybody except by the external customer.
//! 
//! If a command needs to be removed, the user needs to take care that 
//! the command is no more used in the embedded SW
//!
//! @version 2.4 snd3-dvk-1_9_5 $Id: mail_cmd.h,v 2.4 2007/09/24 09:44:27 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MAIL_CMD_H_
#define _MAIL_CMD_H_

#include "lib_system\mailbox\mail.h"


//! Redefinition of the task to be better interpreted
//! TASK_0  =  0x01
//! TASK_1  =  0x02
//! TASK_2  =  0x04
//! TASK_3  =  0x08
//! TASK_4  =  0x10
//! TASK_5  =  0x20
//! TASK_6  =  0x40
//! TASK_7  =  0x80
#define TASK_USB              TASK_0
#define TASK_PLAYER           TASK_1
#define TASK_POWER            TASK_2
#define TASK_EXPLORER         TASK_3
#define TASK_UPDATE           TASK_4
#define TASK_RECORDER         TASK_5
#define TASK_VIEWER           TASK_6

#define TASK_DIC              TASK_7

//! Definition of Commands
#define CMD_DUMMY                   0x00


//! @defgroup USB and Mass Storage commands
//! @{
#define CMD_USB                     ((U16)TASK_USB<<8)
#define CMD_USB_START               (CMD_USB + 1)
#define CMD_USB_STOP                (CMD_USB + 2)
#define CMD_USB_DEVICE_START        (CMD_USB + 3)
#define CMD_USB_DEVICE_STOP         (CMD_USB + 4)
#define CMD_USB_HOST_START          (CMD_USB + 5)
#define CMD_USB_HOST_STOP           (CMD_USB + 6)
#define CMD_USB_FORCE_HIGH_SPEED    (CMD_USB + 7)
//! @}


//! @defgroup Player commands
//! @{
#define CMD_PLAYER                  ((U16)TASK_PLAYER<<8)
#define CMD_PLAYER_GET_BARGRAPH     (CMD_PLAYER + 1)
//! The player command number is less than 255
//! @}


//! @defgroup Power commands
//! @{
#define CMD_POWER                   ((U16)TASK_POWER<<8)
#define CMD_POWER_OFF               (CMD_POWER + 1)
#define CMD_AUTO_POWER_OFF_DISABLE  (CMD_POWER + 2)
#define CMD_AUTO_POWER_OFF_ENABLE   (CMD_POWER + 3)
#define CMD_BATTERY_CHARGE_DISABLE  (CMD_POWER + 9)
#define CMD_BATTERY_CHARGE_ENABLE   (CMD_POWER + 10)
#define CMD_GET_VBUS_STATE          (CMD_POWER + 11)	// "0": no power from USB; "1": power from USB
#define CMD_POWER_SETTING_LOADED    (CMD_POWER + 12)
//! @}


//! @defgroup Explorer commands
//! @{
#define CMD_EXP                     ((U16)TASK_EXPLORER<<8)
#define CMD_EXP_PASTE_ENABLE        (CMD_EXP + 01)
#define CMD_EXP_PASTE_DISABLE       (CMD_EXP + 02)
//! @}

//! @defgroup UPDATE commands
//! @{
#define CMD_UPDATE                  ((U16)TASK_UPDATE<<8)
#define CMD_UPDATE_START            (CMD_UPDATE + 1)
//! @}


//! @defgroup Recorder commands
//! @{
#define CMD_RECORDER                ((U16)TASK_RECORDER<<8)
//! @}


//! @defgroup Viewer commands
//! @{
#define CMD_VIEWER                  ((U16)TASK_VIEWER<<8)
//! @}



#endif  //! _MAIL_CMD_H_ 
