//! @file conf_clock.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the keyboard
//! This file will be given to any external customer
//!
//! @version 1.1 snd3-dvk-1_9_5 $Id: conf_clock.h,v 1.1 2005/01/03 15:01:46 ffosse Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_CLOCK_H_
#define _CONF_CLOCK_H_

//! --- Oscillator Frequency Configuration ---
//! Values: From 12000 up to 26000 : value in KHz
#define FOSC               12000

//! --- Oscillator Type Configuration ---
//! Values: CLK_CRYSTAL    a crystal is connected on X1/X2 pins
//!         CLK_GENERATOR  an external oscillator drives X1 input
#define CLK_DRIVE          CLK_CRYSTAL

#endif  // _CONF_CLOCK_H_

