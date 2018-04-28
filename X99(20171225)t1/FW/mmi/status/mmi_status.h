//!
//! @file mmi_status.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file declares the MMI application STATUS
//!
//! Is the base application of the MMI.
//! Must be lauched first by the kernel.
//!
//! @version 1.6 snd3-dvk-1_9_5 $Id: mmi_status.h,v 1.6 2006/10/04 17:11:20 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_STATUS_H_
#define _MMI_STATUS_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmi_status_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

_GLOBEXT_ _MEM_TYPE_SLOW_  U8       g_startup_id_appli
#if 0//def _mmi_status_c_
   = MMI_APPLI_NOT_DEFINED
#endif
;


#define  find_path_a( a, b )     find_path_u( atou( a ), b )


_GLOBEXT_   char  _MEM_TYPE_SLOW_   tisiFileName[ 512 ];
_GLOBEXT_   char  _MEM_TYPE_SLOW_   tisiFileShortpath[ 128 ];
//_____ D E F I N I T I O N S ______________________________________________
_GLOBEXT_   void  status_display             (void);
_GLOBEXT_   void  status_inhib_display       (void);
_GLOBEXT_   void  status_authorize_display   (void);

_GLOBEXT_   U16   *atou                      ( U8 *astr );
_GLOBEXT_   Bool  find_path_u                ( U16 *path_str, U8 *short_path );


#endif  //! _MMI_STATUS_H_
