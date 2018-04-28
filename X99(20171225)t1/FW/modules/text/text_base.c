#ifndef _text_base_c_
#define _text_base_c_

#include "config.h"
#include "text_base.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
//#include "unicode_string.h"

#pragma DATA = TEXT_BASE_DATA
#pragma CODE = TEXT_BASE_CODE

#if( DIC == ENABLE )

U8 _MEM_TYPE_SLOW_   textbase_g_type = TEXTBASE_Type_None;

#if 1

#pragma CODE = TEXT_BASE_TEMP_CODE00

Bool textbase_init( U8 type, void *param )
{
   switch( type )
   {
      //case TEXTBASE_Type_TextFile:
      case TEXTBASE_Type_SmallText:
      /*if( param != NULL )
      {
         if( !nav_filelist_findname( string_get_unicode(( U8 *)param ), FALSE ))
            return FALSE;
      }
      if( Fat_file_isnot_open )
      {
         if( !file_open( FOPEN_MODE_R ))
            return FALSE;
      }*/

      textbase_g_type = type;
      return TRUE;
   }
   return FALSE;
}


#pragma CODE = TEXT_BASE_CODE

/*Bool*/void textbase_end( void )
{
#if 0
   switch( textbase_g_type )
   {
      /*case TEXTBASE_Type_TextFile:
      file_close();
      break;*/
      
      case TEXTBASE_Type_SmallText:
      break;
   }
#endif
   textbase_g_type = TEXTBASE_Type_None;
   //return TRUE;
}
#endif

Bool textbase_seek( U32 pos )
{
   switch( textbase_g_type )
	{
      //case TEXTBASE_Type_TextFile:
      case TEXTBASE_Type_SmallText:
      return file_seek( pos, FS_SEEK_SET );
	}
	return FALSE;
}


U16  textbase_getc( void )
{
   U8 temp_buf[ 1 ];
   switch( textbase_g_type )
   {
      //case TEXTBASE_Type_TextFile:
      case TEXTBASE_Type_SmallText:
      if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)temp_buf, 1 )== 1 )
         return ( U16 )temp_buf[ 0 ];
      break;
   }
	return 0xFFFF;
}

#if 0
U32 textbase_get_pos( void )
{
   switch( textbase_g_type )
   {
      case TEXTBASE_Type_TextFile:
      case TEXTBASE_Type_SmallText:
      return fs_g_nav_entry.u32_pos_in_file;
	}
	return 0;
}


U8	textbase_get_type( void )
{
   return textbase_g_type;
}


Bool textbase_is_init( void )
{
   return textbase_g_type != TEXTBASE_Type_None;
}
#endif
#endif   //#if( DIC == ENABLE )
#endif   //_text_base_c_
