//! @file string.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the definitions of the string utilities
//!
//! @version 1.2 snd3-dvk-1_9_5 $Id: string.h,v 1.2 2006/05/24 09:29:08 bminier Exp $
//!
//! @todo
//! @bug
#ifndef _STRING_H_
#define _STRING_H_


#undef _GLOBEXT_
#if (defined _string_c_)
   #define _GLOBEXT_
#else
   #define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "lib_system/unicode/unicode.h"
#include "mmi\shared\com_var.h"
#include "compiler.h"


_GLOBEXT_ U8      string_strlen        (U8*);

//void string_copy(U8 _MEM_TYPE_SLOW_ *src_str, U8 _MEM_TYPE_SLOW_ *dest_str, U8 _MEM_TYPE_SLOW_ size);
Bool string_comp(U8 _MEM_TYPE_SLOW_ *src_str, U8 _MEM_TYPE_SLOW_ *dest_str);

//***********************************************
//#define g_buffer_ascii                g_string
//#define g_buffer_unicode              g_string2
#define g_buffer_ascii                g_string1
#define g_buffer_unicode              g_string

void  string_ascii_to_unicode ( UnicodeBuffer *dest, U8 _MEM_TYPE_SLOW_ *source );

#define	string_unicode( ptr )               (( U8 _MEM_TYPE_SLOW_ *)Unicode_get_addr_string( ptr ))
#define	string_get_ascii_buffer()           g_buffer_ascii

U8  *string_unicode_ptr     ( U8 _MEM_TYPE_SLOW_ *source );
U8  *string_unicode_buffer  ( U8 _MEM_TYPE_SLOW_ *source );
//***********************************************

#endif  //! _STRING_H_

