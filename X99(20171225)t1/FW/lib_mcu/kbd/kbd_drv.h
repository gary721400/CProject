//! @file kbd_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains definition for the low level keybpad driver
//!
//! @version 1.13 snd3-dvk-1_9_5 $Id: kbd_drv.h,v 1.13 2005/08/12 06:40:56 ffosse Exp $
//!
//! @todo
//! @bug
//!
#ifndef _KBD_DRV_H_
#define _KBD_DRV_H_

//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________


//! Keypad input interrupt enable.
//! @param 0000XXXXb, X: 1 means enable
#define Kbd_int_enable(m)              (KBCON &= ~(m))
//! Keypad input interrupt disable.
//! @param 0000XXXXb, X: 1 means disable
#define Kbd_int_disable(m)             (KBCON |= (m))

//! Keypad set high level input trigger.
//! @param 0000XXXXb, X: 1 means high level
#define Kbd_trig_high_level(l)         (KBCON |= (l) << 4)
//! Keypad set low level input trigger.
//! @param 0000XXXXb, X: 1 means low level
#define Kbd_trig_low_level(l)          (KBCON &= ~((l) << 4))

//! Keypad exit power-down enable.
#define Kbd_enable_pd_exit()           (KBSTA |= MSK_KPDE)
//! Keypad exit power-down disable.
#define Kbd_disable_pd_exit()          (KBSTA &= ~MSK_KPDE)

//! Keypad power-on key usage enable.
#define Kbd_enable_power_on_key()      (KBSTA |= MSK_KDCPE)
//! Keypad power-on key usage disable.
#define Kbd_disable_power_on_key()     (KBSTA &= ~MSK_KDCPE)

//! Keypad return status register.
//! reading KBSTA erase status KINF flags
//! @return YYYYXXXXb, X: 1 means a 0 to 1 or 1 to 0 is triggered
//! reading KBSTA erase status flags
#define Kbd_get_status()               (KBSTA)

//! Keypad return input status.
//! reading KBSTA erase status KINF flags
//! @return 0000XXXXb, X: 1 means a 0 to 1 or 1 to 0 is triggered
#define Kbd_get_event()                (KBSTA & MSK_KINF)

//! Keypad return power-on key pressed status.
//! reading KBSTA erase status KINF flags
//! @return TRUE:  power-on key pressed
//!         FALSE: power_on key released
#define Kbd_is_power_key_pressed()     ((KBSTA & MSK_KDCPL) == 0)

//! Keypad return power-on key released status.
//! reading KBSTA erase status KINF flags
//! @return TRUE:  power-on key released
//!         FALSE: power_on key pressed
#define Kbd_is_power_key_released()    ((KBSTA & MSK_KDCPL) == MSK_KDCPL)


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________


#endif  // _KBD_DRV_H_

