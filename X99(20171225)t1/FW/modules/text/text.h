#ifndef _TEXT_H_
#define _TEXT_H_

#include "conf\conf_sio.h"

#if( DIC == ENABLE )

#undef _GLOBEXT_
#if (defined _text_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

#define  TEXT_Num_MaxLines          (( U16 )65530 )
//#define  TEXT_Num_Levels	           4
//#define  TEXT_Num_LinesPerLevel     16
#define  TEXT_Num_CacheLines        16
#define  TEXT_Mask_CacheIndex       (( U16 )0xFFF0 )
#define  TEXT_Mask_CacheLines	      ( ~TEXT_Mask_CacheIndex )

#define  TEXT_Error_Char            0xFFFF
#define  TEXT_Encode_ASCII          ENCODING_ASCII
#define  TEXT_Encode_UTF16BE        ENCODING_UTF16BE
#define  TEXT_Encode_UTF16LE        ENCODING_UTF16LE
#define  TEXT_Encode_Unknown        ENCODING_UNKNOWN
#define  TEXT_Offset_Invalid        (( U32 )0xFFFFFFFF )
#define  Text_check_GB1(c)          (( c )>= 0x80 )
#define  Text_check_GB2(c)	         (( c )>= 0x80 )

#define  SECTOR_MASK	               ( ~(( U32 )FS_512B_MASK ))

enum
{
   TEXT_Error_None,
   TEXT_Error_FileSystem,
   TEXT_Error_EncodeUnknown
};

typedef  void  ( *Text_output_func )( U8 );
//TEXT_Num_Levels

//TEXT_Num_LinesPerLevel
typedef struct _Text_line_info
{
   U16   line_num;
}Text_line_info;

typedef struct _Text_line_offset
{
   U16   line_num;
   U32   offset;
}Text_line_offset;

typedef  U8    CharType;

#define  Bits_get( type, value, pos, nbits )    ((((((( type )1 )<<( nbits ))- 1 )<<( pos ))&( value ))>>( pos ))
#define  Bits_get8( value, pos, nbits )	      Bits_get( U8, value, pos,nbits )
#define  Bits_get16( value, pos, nbits )	      Bits_get( U16, value, pos,nbits )
#define  Bits_get32( value, pos, nbits )	      Bits_get( U32, value, pos,nbits )

#define  Byte_get( v, pos )                     ((( U8 *)(&( v )))[ pos ])

_GLOBEXT_   U16   _MEM_TYPE_SLOW_   text_g_lines_cache_index;

_GLOBEXT_   U8    _MEM_TYPE_SLOW_   text_g_encode;
_GLOBEXT_   U16   _MEM_TYPE_SLOW_   text_g_line_width;
_GLOBEXT_   U16   _MEM_TYPE_SLOW_   text_g_lines_total_num;

_GLOBEXT_   U32   _MEM_TYPE_SLOW_   text_g_current_pos;
_GLOBEXT_   U32   _MEM_TYPE_SLOW_   text_g_start_offset;
_GLOBEXT_   U32   _MEM_TYPE_SLOW_   text_g_size;

#define  text_offset()              text_g_current_pos
#define  text_size()                text_g_size
#define  text_start_offset()        text_g_start_offset
#define  text_line_width()          text_g_line_width
#define  text_lines_total_num()     text_g_lines_total_num
#define  text_get_encode()          text_g_encode
#define  text_cache_index()         text_g_lines_cache_index


//U32   text_offset             ( void );
//U32   text_size               ( void );
//U32   text_start_offset       ( void );
//U16   text_line_width         ( void );
//U16   text_lines_total_num    ( void );
U8    text_get_char_width     ( U16 c );
//U8    text_get_encode         ( void );
U8    text_read_encode        ( void );
/*Bool*/void  text_temp_file_begin    ( void );
Bool  text_temp_file_end      ( void );
Bool  text_temp_file_open     ( void );
/*Bool*/void  text_temp_file_close    ( void );
Bool  text_temp_file_write    ( U16 line_num, U32 offset );
U32   text_temp_file_read     ( U16 line_num );
Bool  text_init               ( U32 *start_offset, U32 *size, U16 line_length );
U16   text_getc               ( void );
Bool  text_seek               ( U32 pos );
//U16   text_cache_index        ( void );
Bool  text_get_level_lines    ( Text_line_offset *start, Text_line_offset *end );
Bool  text_lines_cache_update ( U16 line_num );
U16   text_calc_line_num      ( U32 offset );
Bool  text_read_line          ( U16 line_num, CharType *line_str );
void  text_end                ( void );

#endif   //#if( DIC == ENABLE )
#endif   //_TEXT_H_
