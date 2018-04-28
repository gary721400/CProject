#ifndef _unicode_string_c_
#define _unicode_string_c_

#include "config.h"
#include "unicode_string.h"

#if( DIC == ENABLE )

/*
UnicodeBuffer     string_g_ascii[ STRING_SIZE_ASCII ];
UnicodeBuffer     string_g_unicode[ STRING_SIZE_UNICODE ];
*/
/*
void string_ascii_to_unicode( UnicodeBuffer *dest, U8 _MEM_TYPE_SLOW_ *source )
{
   ( dest )[ 6 ] = 0x00;
   ( dest )[ 7 ] = 0x00;    
   unicode_append_ascii_string_to_buffer( dest, source );
}
*/


U8 *string_get_unicode( U8 *ascii_str )
{
   Unicode_init_buffer( string_g_unicode, STRING_SIZE_UNICODE );
   string_ascii_to_unicode( string_g_unicode, ascii_str );
   return (( U8 _MEM_TYPE_SLOW_ *)Unicode_get_addr_string( string_g_unicode ));
}

#if 0
U8 *string_get_unicode_buffer( U8 *ascii_str )
{
   string_ascii_to_unicode( string_g_unicode, ascii_str );
   return string_g_unicode;
}
#endif
#endif   //#if( DIC == ENABLE )
#endif   //_unicode_string_c_