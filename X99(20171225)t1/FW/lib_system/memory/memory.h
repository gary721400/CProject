//! @file memory.h,v
//!
//! Copyright (c) 2003-2004
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief General definition of library memory
//!
//! @version 1.5 snd3-dvk-1_9_5 $Id: memory.h,v 1.5 2007/02/28 13:14:04 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef _MEMORY_H_
#define _MEMORY_H_

//_____ M A C R O S ________________________________________________________
//_____ D E F I N I T I O N S ______________________________________________
//_____ D E C L A R A T I O N S ____________________________________________

//_____ F U N C T I O N S __________________________________________________

#include <string.h>
#define  memcpy_code2ram( dest , src, size )    memcpy( dest , src , size ) 
#define  memcpy_ram2ram( dest , src, size )     memcpy( dest , src , size ) 
#define  memcmp_code2ram( src1 , src2, size )   memcmp( src1 , src2, size )
#define  memcmp_ram2ram( src1 , src2, size )    memcmp( src1 , src2, size )
#define  asm_code2xdata( src , dest , size )    memcpy( dest , src , size ) 
#define  asm_xdata2xdata( src , dest , size )   memcpy( dest , src , size ) 
#define  asm_data2xdata( src , dest , size )    memcpy( dest , src , size ) 
#define  asm_clearxdata( dest , size )          memset( dest , 0, size )


#endif  // _MEMORY_H_

