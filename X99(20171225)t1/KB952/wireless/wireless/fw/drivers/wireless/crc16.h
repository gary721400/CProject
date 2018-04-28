//! @file port.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the timers 0, 1 and watchdog driver definition
//!
//! @version 1.8 snd3-refd1-1_9_5 $Id: crc16.h,v 1.8 2007/01/26 15:58:51 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CRC16_H_
#define _CRC16_H_

#undef _GLOBEXT_
#if (defined _crc16_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________


//_____ O P T I O N S ______________________________________________________


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

U16   cal_crc16   ( U8 *ptr, U8 length );

#endif  // _PORT_H_
