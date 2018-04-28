/*
 * FS_patch.c
 *
 * Description: 
 *
 * Author: Nick Lee <linin@kingb.com.tw>
 * Copyright(c)2010 King Billion electronics Co., Ltd.<http://www.kingb.com.tw/>
 * All rights reserved.
 *
 * Modifications:
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fs.h>
#include "fs_com.h"
#include "FS_patch.h"


#pragma CODE = FS_PATCH_CODE
#pragma DATA = FS_PATCH_DATA

/* Private define ------------------------------------------------------------*/
#define  _FS_PATH_LEVEL_   (8)

// uni '\' => 0x005C (Oblique Line)
#define  OBLIQUE_LINE      0x005C
#define  CLUSTER_BUF_SIZE  6144
#define  SKIPS_CLUSTER     1024

/* Private macro -------------------------------------------------------------*/
//cache_size = MAX_FILE_SIZE/(32768*gdrv[drive].fsinfo->sectors_per_cluster)
/* Private function prototypes -----------------------------------------------*/
int   *unistrncpy    ( int* s, const int *ct, int n );
int   *unistrcpy     ( int* s, const int *ct );
int   unistrlen      ( const int *cs );
int   *unistrchar    ( const int *cs, int uni );
int   *unistrupper   ( int *s );
int   CheckToken     ( int token );
int   CheckIsLFN     ( int *s, int len );
void  Int2Char       ( int *intStr, char *charStr, int len );

/* Private variables ---------------------------------------------------------*/
struct
{
   unsigned int   idx;
   int            str_pos;
}iPathIndexArray[ _FS_PATH_LEVEL_ ];

unsigned int   iPathIndexLevel;
__X   char     CurrentShortFilePath[ 256 ];

#if( FAT_CLUSTER_BUF == DISABLE )
__Y   char     cluster_buf[ CLUSTER_BUF_SIZE ];
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )

#pragma CODE = NAVIGATION_TEMP_CODE00

/* Private functions ---------------------------------------------------------*/
Bool FSPatchInit( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry )
{
   int i;
   FS_ERR fserr;

   for( i = 0; i < _FS_PATH_LEVEL_; i++ )
   {
      iPathIndexArray[ i ].idx = 0;
      iPathIndexArray[ i ].str_pos = 0;
   }
   iPathIndexLevel = 0;
   memset( CurrentShortFilePath, 0, sizeof( CurrentShortFilePath ));

   strcpy(( char *)CurrentShortFilePath, "0:\\" );
   fserr = FSGetEntry( CurrentShortFilePath, 0, finfo, fentry );
   switch( fserr )
   {
      case FS_FORMAT_ERROR:
      case FS_DRIVE_ERROR:
      return FALSE;

      case FS_FILE_NOT_FOUND:
      for( i = 1; ; ++i )
      {
         fserr = FSGetEntry( CurrentShortFilePath, i, finfo, fentry );
         if( fserr == FS_NO_ERROR )
         {
            iPathIndexArray[ 0 ].idx = finfo->index;
            return TRUE;
         }
         else if( fserr != FS_FILE_NOT_FOUND )
            return FALSE;
      }
      //return TRUE;

      default:
      return TRUE;
   }
   //return ( fserr == FS_NO_ERROR );
}


#if 0
FS_ERR FSGotoEntry_u( unsigned short *spec,
                        unsigned int index,
                        __X FILE_INFO *finfo,
                        __X FS_ENTRY_INFO *entry_info )
{	
   int unilen;
   int GetNextTokenFlag;
   int StrLen;
   int *pos1;
   int *pos2;
   int *pLongFilePath;
   int unistring[ 128 ];
   FS_ERR fserr;

   pLongFilePath = ( int *)spec;
   CurrentShortFilePath[ 0 ] = ( char )( spec[ 0 ] & 0x00FF );
   CurrentShortFilePath[ 1 ] = ( char )( spec[ 1 ] & 0x00FF );
   CurrentShortFilePath[ 2 ] = '\0';
   if( !( CurrentShortFilePath[ 0 ] >= '0'
            && CurrentShortFilePath[ 0 ] <= '9'
               && CurrentShortFilePath[ 1 ] == ':' ))
      return FS_INVALID_SPEC;
   fserr = FSGetEntry( CurrentShortFilePath, 0, finfo, entry_info );
   strcat(( char *)CurrentShortFilePath, "\\" );
   iPathIndexLevel = 0;
   iPathIndexArray[ 0 ].str_pos = ( int )strlen(( char *)CurrentShortFilePath );
   GetNextTokenFlag = 1;
   while( 1 )
   {
      //若是搜尋到其中某層目錄的最後都還沒找到對應的entry，即表示長檔名路徑不正確。
      if( fserr == FS_END_DIRECTORY )
         break;

      if(( strcmp(( char *)".", ( char *)finfo->name ))
            &&( strcmp(( char *)"..", ( char*)finfo->name )))  // not is "." or ".."  
      {
         if(( finfo->attribute &( FS_ATTR_DIRECTORY | FS_ATTR_ARCHIVE ))
            &&( fserr == FS_NO_ERROR ))
         {
            if( GetNextTokenFlag )
            {
               // Get folder or file name
               pos1 = unistrchar( pLongFilePath, OBLIQUE_LINE );
               pos2 = unistrchar( pos1 + 1, OBLIQUE_LINE );
               if( pos2 )
                  unistrncpy( unistring, pos1 + 1, pos2 - pos1 - 1 );
               else
                  unistrcpy( unistring, pos1 + 1 );
               StrLen = unistrlen( unistring );
               unistrupper( unistring );  // 將字串中英文字母轉換成大寫
               if( !CheckIsLFN( unistring, StrLen ))  // 判斷是否沒有LFN Entry
                  Int2Char( unistring, ( char *)unistring, StrLen );    // 若沒有LFN Entry則轉成ASCII
               GetNextTokenFlag = 0;
            }

            unistrupper(( int *)finfo->lfn );   //英文字母轉換成大寫

            //比較是否相等，Unicode部份英文字母統一先轉換成大寫後再比較
            //短檔名部份依照FAT命名規則，英文字母固定是大寫字母
            if( !memcmp(( char *)finfo->name, unistring, StrLen )
               || !memcmp(( char *)finfo->lfn, unistring, ( StrLen << 1 )))
            {
               GetNextTokenFlag = 1;
               pLongFilePath = pos2;
               iPathIndexArray[ iPathIndexLevel ].idx = finfo->index;

               // 若屬性為目錄(FS_ATTR_DIRECTORY)，則進入該目錄繼續找
               // 反之若屬性為檔案(FS_ATTR_ARCHIVE)，則表示找到並取得正確的短檔名路徑
               if( finfo->attribute & FS_ATTR_DIRECTORY )
               {
                  strcat( CurrentShortFilePath, ( char*)finfo->name );  //接續到路徑字串後面
                  iPathIndexArray[ iPathIndexLevel ].str_pos = ( int )strlen( CurrentShortFilePath );
                  if( pLongFilePath == 0 )
                     return FSGetEntry( CurrentShortFilePath, index, finfo, entry_info );
                  else
                     FSGetEntry( CurrentShortFilePath, 0, finfo, entry_info );
                  strcat( CurrentShortFilePath, "\\" );
                  iPathIndexLevel++;
                  continue;
               }
               else
                  return fserr;
            }
         }
      }
      fserr = FSGetNextEntry( finfo, entry_info ); // 繼續往下搜尋
   }
   // 傳進來的長檔名路徑有誤
   CurrentShortFilePath[ 0 ] = 0;   
   return FS_INVALID_SPEC;
}

												   
FS_ERR FSGotoEntry( char *spec,
                     unsigned int index,
                     __X FILE_INFO *finfo,
                     __X FS_ENTRY_INFO *entry_info )
{
   int i;
   unsigned short uni_spec[ 256 ];

   for( i = 0; i < strlen(( char *)spec ); i++ )
   {
      if( spec[ i ] > 0x7F )
         return FS_PARAMETER_ERROR;
      uni_spec[ i ] = spec[ i ];
   }
   uni_spec[ i ] = 0;
   return FSGotoEntry_u( uni_spec, index, finfo, entry_info );
}
#endif

#pragma CODE = NAVIGATION_CODE

FS_ERR FSNextEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry )
{
   FS_ERR fserr;

   fserr = FSGetNextEntry( finfo, fentry );
   iPathIndexArray[ iPathIndexLevel ].idx = finfo->index;
   return fserr;
}


FS_ERR FSPrevEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry )
{
   int index;
   FS_ERR fserr;

   index = iPathIndexArray[ iPathIndexLevel ].idx;
RETRY:
   if( index > 0 )
      index--;
   else
      return FS_END_DIRECTORY;
   fserr = FSGetEntry( CurrentShortFilePath, index, finfo, fentry );
   if( fserr == FS_FILE_NOT_FOUND )
      goto RETRY;
   iPathIndexArray[ iPathIndexLevel ].idx = index;
   return fserr;
}

#if 0
FS_ERR FSParentEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry )
{
   if( iPathIndexLevel == 0 )
      return FS_INVALID_DIRECTORY;
   iPathIndexLevel--;
   CurrentShortFilePath[ iPathIndexArray[ iPathIndexLevel ].str_pos ] = '\0';
   iPathIndexArray[ iPathIndexLevel ].idx = 0;
   return FSGetEntry( CurrentShortFilePath, 0, finfo, fentry );
}
#endif

#pragma CODE = NAVIGATION_TEMP_CODE01

FS_ERR FSSubdirEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry )
{
   FS_ERR fserr;

   fserr = FSGetEntry( CurrentShortFilePath, iPathIndexArray[ iPathIndexLevel ].idx, finfo, fentry );
   if( fserr != FS_NO_ERROR )
      return fserr;
   if( finfo->attribute & FS_ATTR_DIRECTORY
      && strcmp(( char *)".", ( __X char *)finfo->name )
      && strcmp( "..", ( __X char *)finfo->name ))
   {
      if( iPathIndexLevel > 0 )
         strcat(( char *)CurrentShortFilePath, "\\" );
      strcat( CurrentShortFilePath, ( __X char *)finfo->name );
      iPathIndexLevel++;
      iPathIndexArray[ iPathIndexLevel ].idx = 0;
      iPathIndexArray[ iPathIndexLevel ].str_pos = ( int )strlen(( char *)CurrentShortFilePath );
      return FSGetEntry( CurrentShortFilePath, 0, finfo, fentry );
   }
   return FS_INVALID_DIRECTORY;
}


#pragma CODE = FILE_TEMP_CODE00

FCFILE *FSOpenEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int mode, __X FS_ERR *err )
{
   char ShortFilePath[ 512 ];

   U32 len;

   *err = FSGetEntry( CurrentShortFilePath, iPathIndexArray[ iPathIndexLevel ].idx, finfo, fentry );
   if( *err != FS_NO_ERROR )
      return 0;
   if( finfo->attribute & FS_ATTR_ARCHIVE )
//   if(( finfo->attribute & FS_ATTR_LFN_ENTRY ) != FS_ATTR_LFN_ENTRY
//      &&( finfo->attribute & FS_ATTR_DIRECTORY ) != FS_ATTR_DIRECTORY
//      &&( finfo->attribute & FS_ATTR_VOLUME ) != FS_ATTR_VOLUME )
   {
      strcpy(( char *)ShortFilePath, ( char *)CurrentShortFilePath );
      if( iPathIndexLevel > 0 )
         strcat(( char *)ShortFilePath, "\\" );
      strcat( ShortFilePath, ( __X char *)finfo->name );

      /*len = FSGetFileSize( ShortFilePath, err );
      //if( len <( 210UL * 1024 * 1024 ))
      if( len <( 50UL * 1024 * 1024 ))
      {
         return FSOpen( ShortFilePath, mode, err );
      }
      else*/
#if( FAT_CLUSTER_BUF == DISABLE )
      {
//         return FSOpenForFastSeek( ShortFilePath, mode, &cluster_buf, CLUSTER_BUF_SIZE, err );
         return FSOpenForFastSeekCustom( ShortFilePath, mode, &cluster_buf, CLUSTER_BUF_SIZE, SKIPS_CLUSTER, err );
      }
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )
#if( FAT_CLUSTER_BUF == ENABLE )
      return FSOpen( ShortFilePath, mode, err );
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )
   }
   *err = FS_INVALID_SPEC;
   return 0;
}

#if 0
FCFILE *FSOpenEntryForFastSeek( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int mode, unsigned int *cache_buf, unsigned int buf_size, __X FS_ERR *err )
{
   char ShortFilePath[ 512 ];

   *err = FSGetEntry( CurrentShortFilePath, iPathIndexArray[ iPathIndexLevel ].idx, finfo, fentry );
   if( *err != FS_NO_ERROR )
      return 0;
   if( finfo->attribute & FS_ATTR_ARCHIVE )
//   if(( finfo->attribute & FS_ATTR_LFN_ENTRY ) != FS_ATTR_LFN_ENTRY
//      &&( finfo->attribute & FS_ATTR_DIRECTORY ) != FS_ATTR_DIRECTORY
//      &&( finfo->attribute & FS_ATTR_VOLUME ) != FS_ATTR_VOLUME )
   {
      strcpy(( char *)ShortFilePath, ( char *)CurrentShortFilePath );
      if( iPathIndexLevel > 0 )
         strcat(( char *)ShortFilePath, "\\" );
      strcat( ShortFilePath, ( __X char *)finfo->name );
      return FSOpenForFastSeek( ShortFilePath, mode, cache_buf, buf_size, err );
   }
   *err = FS_INVALID_SPEC;
   return 0;
}
#endif

#pragma CODE = NAVIGATION_CODE

FS_ERR FSSeekEntry( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int index )
{
   FS_ERR	error;
   error = FSGetEntry( CurrentShortFilePath, index, finfo, fentry );
   return	error;
}
/* End of file ---------------------------------------------------------------*/
