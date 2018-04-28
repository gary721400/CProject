//!
//! @file string.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of some string utilities
//!
//! @version 1.4 snd3-dvk-1_9_5 $Id: string.c,v 1.4 2006/05/24 15:03:11 bminier Exp $
//!
//! @todo
//! @bug
//!
#ifndef _string_c_
#define _string_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "string.h"

#include "conf\conf_sio.h"


//_____ D E F I N I T I O N S ______________________________________________


//***********************************************
void string_ascii_to_unicode( UnicodeBuffer *dest, U8 _MEM_TYPE_SLOW_ *source )
{
  //( dest )[ 6 ] = 0x00;
  //( dest )[ 7 ] = 0x00;
  Unicode_set_buffer_empty( dest );
  unicode_append_ascii_string_to_buffer( dest, source );
}


#if( PASTER == ENABLE )
//U16 *string_unicode_ptr( U8 _MEM_TYPE_SLOW_ *source )
U8 *string_unicode_ptr( U8 _MEM_TYPE_SLOW_ *source )
{
  string_ascii_to_unicode( g_buffer_unicode, source );
  return string_unicode( g_buffer_unicode );
}
#endif   //#if( PASTER == ENABLE )

#if 0
U8 *string_unicode_buffer( U8 _MEM_TYPE_SLOW_ *source )
{
  string_ascii_to_unicode( g_buffer_unicode, source );
  return g_buffer_unicode;
}
#endif

//***********************************************

#endif // _string_c_
