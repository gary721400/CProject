#ifndef _navext_c_
#define _navext_c_

#include "config.h"
#include "navext.h"
//-------------文件系统-------------
#include "modules\file_system\fat.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
//-------------字符串部分-------------
#include "unicode_string.h"

#if( DIC == ENABLE )

//U8    _MEM_TYPE_SLOW_   *navext_g_ext = "";
Bool                    navext_g_dir = FALSE;

extern   U8    _MEM_TYPE_SLOW_   dic_g_ext[];
/*U8 *navext_file_set_ext( U8 _MEM_TYPE_SLOW_ *ext )
{
   U8 *old_ptr;
   old_ptr = navext_g_ext;
   navext_g_ext = ext;
   return old_ptr;
}
*/

void navext_file_use_dir( Bool v )
{
   navext_g_dir = v;
}

#if 0
Bool navext_file_prev( void )
{
   Bool is_dir;
   file_close();
   is_dir = ( fs_g_nav.b_mode_nav == FS_DIR );
   if( !is_dir )
   {
      while( nav_filelist_set( 0, FS_FIND_PREV ))
      {
         if( nav_file_checkext( navext_g_ext ))
            return TRUE;
      }
      if( navext_g_dir )
      {
         return nav_filelist_last( FS_DIR );
      }
   }
   if( navext_g_dir )
   {
      return nav_filelist_set( 0, FS_FIND_PREV );
   }
   return FALSE;
}


Bool navext_file_next( void )
{
   Bool is_dir;
   file_close();
   is_dir = ( fs_g_nav.b_mode_nav == FS_DIR );

   if( is_dir )
   {
      if( navext_g_dir )
      {
         if( nav_filelist_set( 0, FS_FIND_NEXT ))
         {
            if( fs_g_nav.b_mode_nav == FS_DIR )
               return TRUE;
         }
      }
      if( nav_filelist_first( FS_FILE ))
      {
         if( nav_file_checkext( navext_g_ext ))
            return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( nav_file_checkext( navext_g_ext ))
         return TRUE;
   }
   return FALSE;
}
#endif

Bool navext_file_first( void )
{
   file_close();

   if( navext_g_dir )
   {
      if( nav_filelist_first( FS_DIR ))
         return TRUE;
   }

   if( nav_filelist_first( FS_FILE ))
   {
      do
      {
         //if( nav_file_checkext( navext_g_ext ))
         if( nav_file_checkext( dic_g_ext ))
            return TRUE;
      }
      while( nav_filelist_set( 0, FS_FIND_NEXT ));
   }
   return FALSE;
}


Bool navext_file_last( void )
{
   file_close();

   if( nav_filelist_last( FS_FILE ))
   {
      do
      {
         //if( nav_file_checkext( navext_g_ext ))
         if( nav_file_checkext( dic_g_ext ))
            return TRUE;
      }
      while( nav_filelist_set( 0, FS_FIND_PREV ));
   }

   if( navext_g_dir )
   {
      if( nav_filelist_last( FS_DIR ))
         return TRUE;
   }
   return FALSE;
}

#if 0
Bool navext_file_dir( Bool type )
{
   if( type )
   {
      if( !nav_dir_cd())
         return FALSE;
   }
   else
   {
      if( !nav_dir_gotoparent())
         return FALSE;
   }
   return navext_file_first();
}
#endif

#if 0//( LCD_PRESENT == ENABLE )
U8 *navext_file_get_name( void )
{
   unicode_init_buffer_enc( String_ptr_unicode_buffer(), STRING_SIZE_UNICODE, ENCODING_UTF16BE );
   if( !nav_file_name( string_unicode(String_ptr_unicode_buffer()), Unicode_get_size_buffer( String_ptr_unicode_buffer()), FS_NAME_GET, FALSE ))
   {
      return "";
   }
   return String_ptr_unicode_buffer();
}
#endif   //#if( LCD_PRESENT == ENABLE )
#endif   //#if( DIC == ENABLE )
#endif   //_navext_c_