#ifndef _text_c_
#define _text_c_

#include "config.h"

#include <stdio.h>

#include "mmi/shared/com_var.h"
//-------------Unicode处理部分-------------
//#include "unicode_string.h"
//-------------C标准库的字符处理-------------
#include <ctype.h>
//-------------文件系统-------------
//#include "modules\file_system\fat.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
//-------------调试工具-------------

#include "text.h"
#include "text_base.h"
#include "modules/text/text_viewer.h"
#include "com.h"

#pragma DATA = TEXT_DATA
#pragma CODE = TEXT_CODE

ImageDeclareType( TEXT_TEMP_CODE02 );

#if( DIC == ENABLE )

//U8    _MEM_TYPE_SLOW_   text_g_temp_file[] = "temp.tmp";
//U8    _MEM_TYPE_SLOW_   text_g_old_nav_id;
U16   _MEM_TYPE_SLOW_   text_g_lines_cache_index;
U32   _MEM_TYPE_SLOW_   text_g_lines_cache[ TEXT_Num_CacheLines + 1 ];
U16   _MEM_TYPE_SLOW_   text_g_lines_nums[ TEXT_Num_CacheLines + 1 ];
Text_line_offset  _MEM_TYPE_SLOW_   text_g_last_line;

U16   _MEM_TYPE_SLOW_   text_g_total_lines_num;

U8    _MEM_TYPE_SLOW_   text_g_encode /*= TEXT_Encode_ASCII*/;
U16   _MEM_TYPE_SLOW_   text_g_line_width;
U16   _MEM_TYPE_SLOW_   text_g_lines_total_num;

U32   _MEM_TYPE_SLOW_   text_g_current_pos;
U32   _MEM_TYPE_SLOW_   text_g_start_offset;
U32   _MEM_TYPE_SLOW_   text_g_size;

//U8    _MEM_TYPE_SLOW_   text_g_last_error = TEXT_Error_None;

//#define	Dic_check_GB1(c)	((c) >= 0xB0 && (c)<= 0xF7)
//#define	Dic_check_GB2(c)	((c) >= 0xA1 && (c)<= 0xFE)

#if 0
/*Bool*/void text_temp_file_begin( void )
{
   switch( textbase_get_type())
   {
      case TEXTBASE_Type_TextFile:
      text_g_old_nav_id = nav_get();
      //return nav_select( FS_NAV_ID_USER );
      nav_select( FS_NAV_ID_USER );
      break;
      
      case TEXTBASE_Type_SmallText:
      //return TRUE;
      break;
      
   }
   //return FALSE;
}


Bool text_temp_file_end( void )
{
   switch( textbase_get_type())
   {
      //case TEXTBASE_Type_TextFile:
      //return nav_select( text_g_old_nav_id );

		case TEXTBASE_Type_SmallText:
      return TRUE;
	}
   return FALSE;
}


Bool text_temp_file_open( void )
{
   U32 _MEM_TYPE_SLOW_ sector_to_ext;
   Fs_file_segment _MEM_TYPE_SLOW_ segment;
   //U8 _MEM_TYPE_SLOW_ retry = 0;
	
   if( textbase_get_type()!= TEXTBASE_Type_TextFile )
      return TRUE;
   segment.u8_lun = fs_g_nav.u8_lun;
   text_temp_file_begin();
//__retry:
   if( !nav_drive_set( segment.u8_lun ))
      goto __FileErrorEnd;
   if( !nav_dir_root())
      goto __FileErrorEnd;
   if( !nav_filelist_findname( string_get_unicode( text_g_temp_file ), FALSE ))
   {
      if( !nav_file_create( string_get_unicode( text_g_temp_file )))
      {
#if 0
         if( retry == 0 )
         {
            segment.u8_lun = LUN_ID_NF_DISKMASS;
            retry++;
            goto __retry;
         }
         else
#endif
            goto __FileErrorEnd;
      }
   }
   if( !file_open( FOPEN_MODE_R_PLUS ))
      goto __FileErrorEnd;
   sector_to_ext = ((( U32 )TEXT_Num_MaxLines )* 4 + FS_512B - 1 )/ FS_512B;
   do
   {
      segment.u8_lun = 0xFF;
      segment.u16_size = sector_to_ext;
      if( !file_write( &segment ))
      {
         file_close();
         text_temp_file_end();
         goto __FileErrorEnd;
      }
      sector_to_ext -= segment.u16_size;
   }
   while( sector_to_ext > 0 );
   text_g_last_error = TEXT_Error_None;
   return text_temp_file_end();
__FileErrorEnd:
   text_g_last_error = TEXT_Error_FileSystem;
   return FALSE;
}


/*Bool*/void text_temp_file_close( void )
{
   if( textbase_get_type()!= TEXTBASE_Type_TextFile )
      return;
      //return TRUE;
   //text_temp_file_begin();
   //file_close();
   //text_temp_file_end();
   //return text_temp_file_end();
}
#endif

Bool text_temp_file_write( U16 line_num, U32 line_offset )
{
   switch( textbase_get_type())
   {
      /*case TEXTBASE_Type_TextFile:
      if( !file_seek((( U32 )line_num )* 4, FS_SEEK_SET ))
         goto __FileErrorEnd;
      if( !file_putc( Byte_get( line_offset, 0 )))
         goto __FileErrorEnd;
      if( !file_putc(Byte_get( line_offset, 1 )))
         goto	__FileErrorEnd;
      if( !file_putc(Byte_get( line_offset, 2 )))
         goto __FileErrorEnd;
      if( !file_putc(Byte_get( line_offset, 3 )))
         goto __FileErrorEnd;
      text_g_last_error = TEXT_Error_None;
      return TRUE;*/

      case TEXTBASE_Type_SmallText:
      /*if( line_num < TEXT_Num_CacheLines )
      {
         //text_g_lines_cache[ line_num % TEXT_Num_CacheLines ] = line_offset;
      }*/
      return TRUE;
   }
   return FALSE;
/*__FileErrorEnd:
   text_g_last_error = TEXT_Error_FileSystem;
   return FALSE;*/
}


U32 text_temp_file_read( U16 line_num )
{
   U32 result;
   switch( textbase_get_type())
   {
      /*case TEXTBASE_Type_TextFile:
      if( !file_seek((( U32 )line_num )* 4, FS_SEEK_SET ))
         goto __FileErrorEnd;
      Byte_get( result, 0 ) = ( U8 )file_getc();
      Byte_get( result, 1 ) = ( U8 )file_getc();
      Byte_get( result, 2 ) = ( U8 )file_getc();
      Byte_get( result, 3 ) = ( U8 )file_getc();
      text_g_last_error = TEXT_Error_None;
      return result;*/

      case TEXTBASE_Type_SmallText:
      return text_g_lines_cache[ line_num % TEXT_Num_CacheLines ];
   }
   return FALSE;
/*__FileErrorEnd:
   text_g_last_error = TEXT_Error_FileSystem;
   return TEXT_Offset_Invalid;*/
}


//void text_output( U8 process );
#pragma CODE = TEXT_TEMP_CODE00

Bool text_init( U32 * start_offset, U32 *size, U16 line_width )
{
   U8	_MEM_TYPE_SLOW_ iter;
   Text_line_offset start;
   text_g_start_offset = *start_offset;
   text_g_size = *size;
   text_g_line_width = line_width;
//  SendString((U8 *)"\r\ntext_g_line_width = ");
//   send_hex_u16(text_g_line_width);
   text_g_lines_total_num = 0;
   text_g_current_pos = 0;
   text_g_last_line.line_num = 0;
   text_g_last_line.offset = 0;
   text_g_lines_cache_index = TEXT_Num_MaxLines;
   text_g_total_lines_num = 0;
   for( iter = 0; iter < TEXT_Num_CacheLines; ++iter )
   {
      text_g_lines_nums[ iter ] = TEXT_Num_MaxLines;
      text_g_lines_cache[ iter ] = TEXT_Offset_Invalid;
   }
   //if( !text_temp_file_open())
   //   return FALSE;
   text_g_encode = text_read_encode();

   start.line_num = 0;
   if( text_g_encode == TEXT_Encode_ASCII )
      start.offset = 0;
   else
      start.offset = 2;
   if( textbase_get_type()== TEXTBASE_Type_SmallText )
      text_g_last_line.line_num = TEXT_Num_MaxLines;
   else
      text_g_last_line.line_num = TEXT_Num_CacheLines;
   text_g_last_line.offset = TEXT_Offset_Invalid;
   SDKLoadCode( TEXT_TEMP_CODE02 );
   if( !text_get_level_lines( &start, &text_g_last_line ))
      return FALSE;
   
   text_g_total_lines_num = text_g_last_line.line_num;
   SendString((U8 *)"\r\nline_num = ");
   send_hex_u16(text_g_total_lines_num);
 
   iter = text_g_total_lines_num%2;
   if(iter){
   	text_g_total_lines_num = text_g_total_lines_num/2 + 1;
   	}
   else{
   	text_g_total_lines_num = text_g_total_lines_num/2;
   	}
   
   if( !text_lines_cache_update( 0 ))
      return FALSE;
   return TRUE;
}


U8	text_read_encode( void )
{
   U32 old_offset;
   U8 mark[ 2 ];
   old_offset = text_offset();
   if( !text_seek( 0 ))
      return TEXT_Encode_Unknown;
   mark[ 0 ] = ( U8 )text_getc();
   mark[ 1 ] = ( U8 )text_getc();
   if( !text_seek( old_offset ))
      return TEXT_Encode_Unknown;
   //text_g_last_error = TEXT_Error_None;
   if(( mark[ 0 ] == 0xFF )&&( mark[ 1 ]== 0xFE ))
      return TEXT_Encode_UTF16LE;
   else if(( mark[ 0 ] == 0xFE )&&( mark[ 1 ] == 0xFF ))
      return TEXT_Encode_UTF16BE;
   return TEXT_Encode_ASCII;
}

#if 0
U8	text_get_encode( void )
{
   return text_g_encode;
}
#endif


#pragma CODE = TEXT_CODE

U16 text_getc( void )
{
   if( text_g_current_pos >= text_g_size )
      return TEXT_Error_Char;
   text_g_current_pos++;
   return textbase_getc();
}


U16 text_getc_utf16( void )
{
   U16 result, v;
   if( text_g_encode == TEXT_Encode_UTF16BE )
   {
      v = text_getc();
      if( v == TEXT_Error_Char )
         return TEXT_Error_Char;
      MSB( result ) = ( U8 )v;
      v = text_getc();
      if( v == TEXT_Error_Char )
         return TEXT_Error_Char;
      LSB( result ) = ( U8 )v;
   }
   else
   {
      v = text_getc();
      if( v == TEXT_Error_Char )
         return TEXT_Error_Char;
      LSB( result ) = ( U8 )v;
      v = text_getc();
      if( v == TEXT_Error_Char )
         return TEXT_Error_Char;
      MSB( result ) = ( U8 )v;
   }
   return result;
}


Bool text_seek( U32 pos )
{
   if( pos >= text_g_size )
      pos = text_g_size;
   text_g_current_pos = pos;
   if( !textbase_seek( text_g_start_offset + text_g_current_pos ))
      goto __FileErrorEnd;
   //text_g_last_error = TEXT_Error_None;
   return TRUE;
__FileErrorEnd:
   //text_g_last_error = TEXT_Error_FileSystem;
   return FALSE;
}

#if 0
U32 text_offset( void )
{
   return text_g_current_pos;
}


U32 text_size( void )
{
   return text_g_size;
}


U16 text_line_width( void )
{
   return text_g_line_width;
}


U16 text_lines_total_num( void )
{
   return text_g_lines_total_num;
}


U32 text_start_offset( void )
{
   return text_g_start_offset;
}


U16 text_cache_index( void )
{
/*
   U16   _MEM_TYPE_SLOW_   v = 0;
	U8    _MEM_TYPE_SLOW_   iter;
   for( iter = TEXT_Num_Levels - 1; iter != level; --iter )
      v |= ( text_g_lines_cache_index[ iter ])<<( iter * 4 );
   return v;
*/
   return text_g_lines_cache_index;
}

/*
void text_output( U8 process )
{
   output_process( process );
}*/
#endif

U8	text_get_char_width( U16 c )
{
   if( c < 0x07F )
      return 8;
   else if(( c >= 0x080 )&&( c <= 0x3FF ))
      return 14;
   else
      return 16;
}


#pragma CODE = TEXT_TEMP_CODE02

Bool text_get_level_lines( Text_line_offset *start, Text_line_offset *end )
{
   U16   _MEM_TYPE_SLOW_   iter = 0;
   U16   _MEM_TYPE_SLOW_   cur_line;
   U8    _MEM_TYPE_SLOW_   cache_line;
   U8    _MEM_TYPE_SLOW_   char_width;
   U8    _MEM_TYPE_SLOW_   cur_line_width;
   U16                     v;

   if( start->offset == TEXT_Offset_Invalid )
   {
      end->line_num = start->line_num;
      end->offset = TEXT_Offset_Invalid;
      return TRUE;
   }
   for( iter = 0; iter <= TEXT_Num_CacheLines; ++iter )
   {
      text_g_lines_nums[ iter ] = TEXT_Num_MaxLines;
      text_g_lines_cache[ iter ] = TEXT_Offset_Invalid;
   }
   text_g_lines_cache_index = TEXT_Num_MaxLines;
   if( !text_seek( start->offset ))
   {
      end->line_num = 0;
      //end->offset = 0;
      return FALSE;
   }
   cur_line = 0;
   cache_line = 0;
   //text_output( 0 );
   while(( cur_line < end->line_num )&&( text_offset()< end->offset ))
   {
      text_g_lines_nums[ cache_line ] = cur_line;
      text_g_lines_cache[ cache_line ] = text_offset();

      /*if(( cur_line & 0x007F )== 0 )
      {
         if( end->line_num != TEXT_Num_MaxLines )
            text_output(( U32 )( cur_line - start->line_num )* 100 /( end->line_num - start->line_num + 1 ));
         else
            text_output(( U32 )( text_offset()- start->offset)* 100 /( end->offset - start->offset + 1 ));
      }*/

      if( text_g_encode == TEXT_Encode_ASCII )
      {
         cur_line_width = text_g_line_width;

         while( cur_line_width > 0 )
         {
            v = text_getc();
            if( v == TEXT_Error_Char )
               break;
            if( Text_check_GB1( v ))
            {
               char_width = text_get_char_width( text_getc()|( v << 8 ));
			   char_width = char_width/2;
               if( char_width > cur_line_width )
               {
                  if( !text_seek( text_offset()- 2 ))
                     return FALSE;
                  break;
               }
            }
            else
            {
               char_width = text_get_char_width( v );
			   char_width = char_width/2;
               if( v == '\n' )
                  break;
               if( char_width > cur_line_width )
               {
                  if( !text_seek( text_offset()- 1 ))
                     return FALSE;
                  break;
               }
            }
            cur_line_width -= char_width;
         }
      }
      else
      {
         cur_line_width = text_g_line_width;
         while( cur_line_width > 0 )
         {
            v = text_getc_utf16();
            if( v == TEXT_Error_Char )
               break;
            if( v ==( U16 )'\n' )
               break;
            char_width = text_get_char_width( v );
			char_width = char_width/2;
            if( char_width > cur_line_width )
            {
               if( !text_seek (text_offset()- 2 ))
                  return FALSE;
               break;
            }
            cur_line_width -= char_width;
         }
      }
      if( v == TEXT_Error_Char ) //遇到文本结束符号需要特殊处理
      {
         if( cur_line_width != text_g_line_width )
         {
            cur_line++;
            cache_line++;
         }
         text_g_lines_total_num = cur_line;
         for( iter = cache_line; iter < TEXT_Num_CacheLines; ++iter )
         {
            text_g_lines_nums[ iter ] = cur_line;
            text_g_lines_cache[ iter ] = TEXT_Offset_Invalid;
         }
         break;
      }
      else
      {
         cache_line++;
         cur_line++;
         if( cache_line == TEXT_Num_CacheLines )
         {
            //text_temp_file_begin();
            for( iter = 0; iter < TEXT_Num_CacheLines; ++iter )
            {
               if( !text_temp_file_write( text_g_lines_nums[ iter ], text_g_lines_cache[ iter ]))
                  return FALSE;
            }
            //text_temp_file_end();
            cache_line = 0;
         }
      }
   }
   if( cache_line > 0 )
   {
      //text_temp_file_begin();
      for( iter = 0; iter < cache_line; ++iter )
      {
      	  SendString((U8 *)"\r\n line:post=");
		  send_hex_u16(text_g_lines_nums[ iter ]);
		  SendString((U8 *)":");
		  send_hex_u32(text_g_lines_cache[ iter ]);
         if( !text_temp_file_write( text_g_lines_nums[ iter ], text_g_lines_cache[ iter ]))
            return FALSE;
      }
      //text_temp_file_end();
      cache_line = 0;
   }
   //text_output( 100 );
   end->line_num = cur_line;
   if( v == TEXT_Error_Char )
      end->offset = TEXT_Offset_Invalid;
   else
      end->offset = text_offset();
   return TRUE;
}


Bool text_lines_cache_update( U16 line_num )
{
   U8	iter;
   Text_line_offset end_line;

   line_num &= TEXT_Mask_CacheIndex;
   if( line_num == text_g_lines_cache_index )
      return TRUE;
   if( line_num ==( TEXT_Num_MaxLines & TEXT_Mask_CacheIndex ))
      goto __flag;

   if( textbase_get_type() == TEXTBASE_Type_SmallText )
   {
      text_g_last_line.line_num = 0;
      if( text_g_encode == TEXT_Encode_UTF16LE || text_g_encode == TEXT_Encode_UTF16BE )
         text_g_last_line.offset = 2;
      else
         text_g_last_line.offset = 0;
   }

   if( line_num + TEXT_Num_CacheLines > text_g_last_line.line_num )
   {
      end_line.line_num = line_num + TEXT_Num_CacheLines;
      end_line.offset = TEXT_Offset_Invalid;
      if( !text_get_level_lines( &text_g_last_line, &end_line ))
         return FALSE;
      text_g_last_line = end_line;
   }
__flag:
   //text_temp_file_begin();
   for( iter = 0; iter < TEXT_Num_CacheLines; ++iter )
   {
      if(( text_g_last_line.offset == TEXT_Offset_Invalid )&&( line_num + iter >= text_g_last_line.line_num ))
      {
         while( iter < TEXT_Num_CacheLines )
         text_g_lines_cache[ iter++ ] = TEXT_Offset_Invalid;
         break;
      }
      else
         text_g_lines_cache[ iter ] = text_temp_file_read( line_num+iter );
   }
   //text_temp_file_end();

   if( textbase_get_type() == TEXTBASE_Type_SmallText )
   {
      text_g_last_line.line_num = 0;
      if( text_g_encode == TEXT_Encode_UTF16LE || text_g_encode == TEXT_Encode_UTF16BE )
         text_g_last_line.offset = 2;
      else
         text_g_last_line.offset = 0;
   }

   text_g_lines_cache_index = line_num;
   return TRUE;
}

#if 0
U16 text_calc_line_num( U32 offset )
{
   Text_line_offset end_line;
   if( offset >= text_g_last_line.offset )
   {
      end_line.line_num = TEXT_Num_MaxLines;
      end_line.offset = offset;
      if( !text_get_level_lines( &text_g_last_line, &end_line ))
         return TEXT_Num_MaxLines;
      text_g_last_line = end_line;
   }
   else
   {
      if( text_g_last_line.offset == TEXT_Offset_Invalid )
      {
         end_line.line_num = offset * text_g_last_line.line_num / text_g_size;
      }
      else
      {
         end_line.line_num = offset * text_g_last_line.line_num / text_g_last_line.offset;
      }
   }
   if( !text_lines_cache_update( end_line.line_num ))
      return TEXT_Num_MaxLines;
   return end_line.line_num;
}
#endif


#pragma CODE = TEXT_TEMP_CODE01

Bool text_read_line( U16 line_num, CharType * line_str )
{
   U16 _MEM_TYPE_SLOW_ cur_line_width;
   U8  _MEM_TYPE_SLOW_ char_width;
	U8	nc;
	U8	level_index;
	U16 v;
   level_index = Bits_get16( line_num, 0, 4 );
   if(( line_num >= TEXT_Num_MaxLines )||( text_g_lines_cache[ level_index ]== TEXT_Offset_Invalid ))
	{
      line_str[ 0 ]= '\0';
      line_str[ 1 ]= '\0';
      return TRUE;
   }

   if( !text_seek( text_g_lines_cache[ level_index ]))
   {
      line_str[ 0 ]= '\0';
      line_str[ 1 ]= '\0';
      return FALSE;
   }
   if( text_g_encode == TEXT_Encode_ASCII )
   {
      nc = 0;
      cur_line_width = text_g_line_width;
      while( cur_line_width > 0 )
      {
         v = text_getc();
         if( v == TEXT_Error_Char )
            break;
         if( Text_check_GB1( v ))
         {
            line_str[ nc++ ] = ( U8 )v;
            line_str[ nc++ ] = ( U8 )text_getc();
            char_width = text_get_char_width(( line_str[ nc - 1 ])|(( line_str[ nc - 2 ])<< 8 ));
            if( char_width > cur_line_width )
            {
               line_str[ nc - 2 ]= '\0';
               line_str[ nc - 1 ]= '\0';
               if( !text_seek( text_offset()- 2 ))
                  return FALSE;
               break;
            }
         }
         else
         {
            char_width = text_get_char_width( v );
            if( v == '\n')
               break;
            if( char_width > cur_line_width )
            {
               if( !text_seek( text_offset()- 1 ))
                  return FALSE;
               break;
            }
            line_str[ nc++ ] = ( U8 )v;
         }
		 char_width = char_width/2;
         cur_line_width -= char_width;
      }
      line_str[ nc ]= '\0';
   }
   else
   {
      nc = 0;
      cur_line_width = text_g_line_width;
      while( cur_line_width > 0 )
      {
         v = text_getc_utf16();
         if( v == TEXT_Error_Char )
            break;
         if( v == ( U16 )'\n' )
            break;
         char_width = text_get_char_width( v );
         if( char_width > cur_line_width )
         {
            if( !text_seek( text_offset()- 2 ))
               return FALSE;
            break;
         }
         if( text_g_encode == TEXT_Encode_UTF16BE )
         {
            line_str[ nc++ ] = MSB( v );
            line_str[ nc++ ] = LSB( v );
         }
         else
         {
            line_str[ nc++ ] = LSB( v );
            line_str[ nc++ ] = MSB( v );
         }
		 char_width = char_width/2;
         cur_line_width -= char_width;
      }
      line_str[ nc++ ]= '\0';
      line_str[ nc++ ]= '\0';
   }
   return TRUE;
}

#if 0
void text_end( void )
{
   text_temp_file_close();
}
#endif
#endif   //#if( DIC == ENABLE )
#endif   //_text_c_
