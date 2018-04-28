#ifndef _UNICODE_STRING_H_
#define _UNICODE_STRING_H_

#include	"lib_system/unicode/unicode.h"
#include	"mmi/shared/com_var.h"
#include "lib_system/utilities/string.h"
#include "conf\conf_sio.h"

#if( DIC == ENABLE )

#define  STRING_SIZE_ASCII    G_STRING_SIZE
#define  STRING_SIZE_UNICODE  G_STRING_SIZE2

/*
extern   UnicodeBuffer     string_g_ascii[];
extern   UnicodeBuffer     string_g_unicode[];
*/

#define  string_g_ascii       g_string
#define  string_g_unicode     g_string2

void  string_ascii_to_unicode    ( UnicodeBuffer *dest, U8 _MEM_TYPE_SLOW_ *source );
U8    *string_get_unicode        ( U8 *ascii_str );
U8    *string_get_unicode_buffer ( U8 *ascii_str );

#define  String_ptr_ascii_buffer()     string_g_ascii
#define  String_ptr_unicode_buffer()   string_g_unicode
#define  String_unicode_addr( buffer ) (( U8 _MEM_TYPE_SLOW_ *)Unicode_get_addr_string( buffer ))

#endif   //#if( DIC == ENABLE )
#endif   //_UNICODE_STRING_H_
