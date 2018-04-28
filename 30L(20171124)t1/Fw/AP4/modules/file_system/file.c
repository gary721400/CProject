
#pragma DATA = FILE_DATA
#pragma CODE = FILE_CODE

#include <fs.h>
//#include "fs_patch.h"
#include "string.h"
#include "fs_com.h"
#include "fat.h"
#include "file.h"
#include "navigation.h"
#include "com.h"
//#include "fat_var.h"
#include "fc_internal.h"
//#include "search.h"
#include "globalvariable.h"
#define		MAX_LUN	1

//_____ D E C L A R A T I O N S ____________________________________________

//**** Global file system variables

//! Variables to select string format (initialised in nav_reset())
                     Bool                 g_b_unicode;

//! Variables to enable/disable the disk check before each action on disk
//_GLOBEXT_                     Bool                 g_b_no_check_disk;
#if 0
//! \name Variables initialised in drive_mount() and used in other module "navigation" and "file"
//! @{
   _MEM_TYPE_SLOW_   Fs_management        fs_g_nav;
   _MEM_TYPE_FAST_   Fs_management_fast   fs_g_nav_fast;
   _MEM_TYPE_SLOW_   Fs_management_entry  fs_g_nav_entry;
//! @}

//! Variable frequently used by many fonction (optimization no parameter in fonction)
   _MEM_TYPE_SLOW_   Fs_segment           fs_g_seg;

#if( FS_USE_CLUSTER_LIST_OPTIMIZATION == TRUE )
   _MEM_TYPE_SLOW_   Fs_segment           fs_g_offset;
#endif   //#if( FS_USE_CLUSTER_LIST_OPTIMIZATION == TRUE )

//! To optimize speed in fonctions: fat_getfreespace, fat_cluster_list, fat_cluster_val, fat_checkcluster
   _MEM_TYPE_FAST_   Fs_cluster           fs_g_cluster;

#if (FS_LEVEL_FEATURES > FSFEATURE_READ)
//! \name Variable used to update the second FAT zone
//! @{
   _MEM_TYPE_SLOW_   U16                  fs_g_u16_first_mod_fat; //!< Offset (unit 512B) in fat of the first sector (unit 512B)
   _MEM_TYPE_SLOW_   U16                  fs_g_u16_last_mod_fat;  //!< Offset (unit 512B) in fat of the last sector (unit 512B)
//! @}
#endif  // FS_LEVEL_FEATURES

//! \name Variable used to manage the cache
//! @{
   _MEM_TYPE_SLOW_   U8                   fs_g_sector[ FS_CACHE_SIZE ];


   _MEM_TYPE_SLOW_   Fs_sector_cache      fs_g_sectorcache;
//typedef U8  _MEM_TYPE_SLOW_   * PTR_CACHE;
//!}@

//! Global variable to optimize code
   _MEM_TYPE_SLOW_   U32                  fs_gu32_addrsector;  //!< Store the address of futur cache (unit 512B)
#endif
char	__X CurrentShortFilePath[128];

FCFILE __X *		fat_g_files;
FILE_INFO __X		fat_g_navs[1];
FS_ENTRY_INFO __X	fat_g_entry[1];
FILE_INFO __X	*	fat_g_cur_nav;
FS_ENTRY_INFO	__X *	fat_g_cur_entry;
FCFILE __X *	fat_g_cur_file;

//=================================================
//extern __X ClusterListItem cluster_buf[16];

//我希望FILE_INFO 结构可以成为一个像文件浏览指针一样的结构，通过它可以在目录中移动，通过向前，向后，向上层，向下层移动的操作可以在目录中指向不同的文件或目录，如果FILE_INFO不足够完成这样的功能，可以扩充FILE_INFO结构；
/*
FS_ERR  FSGotoEntry (char *spec,unsigned int index,FILE_INFO *finfo, FS_ENTRY_INFO *fentry);
//和FSGetEntry类似的功能； 
//1.spec参数全路径名
//(1)为长文件名路径 
//(2)如果spec串最后一个字符为'/'或'\\'，表示进入路径的目录并指向第index个entry，比如FSGotoEntry("0:\\123\\",3,&info,&entry);info就进入了123目录，并且指向第3个entry
//(3)如果spec中最后一个字符不为'/'和'\\'，表示指向路径的文件，index忽略，比如FSGotoEntry("0:\\123\\456.mp3",0,&info,&entry);info就进入了123目录，并且指向456.mp3文件；

FS_ERR  FSGotoEntry_u(unsigned short *spec,unsigned int index,FILE_INFO *finfo, FS_ENTRY_INFO *fentry);
//FSGotoEntry函数的unicode版本，只不过spec为unicode字串，其它相同；

FS_ERR FSNextEntry( FILE_INFO * finfo);	//finfo向后移动，和FSGetNextEntry类似
FS_ERR FSPrevEntry( FILE_INFO *  finfo);	//finfo向前移动，
FS_ERR FSParentEntry( FILE_INFO * finfo);	//finfo向上层移动，如果finfo所在的目录有上层目录的话；
FS_ERR FSSubdirEntry( FILE_INFO * finfo);	//finfo向下层移动，如果finfo指向的是目录的话就进入这个子录；
FS_ERR FSSeekEntry(FILE_INFO *finfo,unsigned int index);	//finfo定位到index的位置

FCFILE *  FSOpenEntry( FILE_INFO * finfo, unsigned int mode, FS_ERR *err);
//和FSOpen函数功能类似，打开finfo指向的文件，返回文件句柄；

FCFILE *  FSOpenEntryForFastSeek ( FILE_INFO * finfo, unsigned int mode, unsigned int *cache_buf, unsigned int buf_size, FS_ERR *err);
//和FSOpenForFastSeek 函数功能类似，打开finfo指向的文件，返回文件句柄；

FS_ERR  FSCreateDirectoryEntry ( FILE_INFO* finfo,char *spec);
//在finfo所在的目录下创建名为spec字串的目录,spec为长文件名，不是路径名；
FS_ERR  FSCreateDirectoryEntry_u( FILE_INFO * finfo,unsigned short *spec);
//FSCreateDirectoryEntry 的unicode版本

//凡是涉及到文件名操作的函数接口最好都是用ascii码字符串长文件名，如果有可能的话，最好再提供一份unicode码字符中长文件名接口；
*/

#pragma CODE = FILE_CODE
#if 0
Bool  file_ispresent(void)
{
	return	(*fat_g_cur_file)!=0;
}
#endif
/*
#define FS_MODE_CREATE      (0)
#define FS_MODE_READ        (1)
#define FS_MODE_READ_WRITE  (2)

#define  FOPEN_READ_ACCESS    0x01                                                                    // autorize the read access
#define  FOPEN_WRITE_ACCESS   0x02                                                                    // autorize the write access
#define  FOPEN_CLEAR_SIZE     0x04                                                                    // reset size
#define  FOPEN_CLEAR_PTR      0x08    
*/
Bool  file_open( U8 fopen_mode )
{
	unsigned int	_MEM_TYPE_SLOW_	mode;
	FS_ERR	_MEM_TYPE_SLOW_	error;
/*	
	if(fopen_mode==FOPEN_MODE_R)
		mode = FS_MODE_READ;
	else if(fopen_mode==FOPEN_MODE_R_PLUS)
		mode = FS_MODE_READ_WRITE;
	else if(fopen_mode==FOPEN_MODE_W_PLUS)
		mode = FS_MODE_CREATE;
	else
*/		
	mode = FS_MODE_READ;
//	*fat_g_cur_file = (FCFILE __X *)FSOpenEntry(fat_g_cur_nav,fat_g_cur_entry,mode,&error);
	fat_g_cur_file = (FCFILE __X *)_FSOpen((char *)CurrentShortFilePath, mode, &error);
	//fat_g_cur_file = (FCFILE __X **)&fat_g_files;
	if(error)
		return	FALSE;
	return	TRUE;
}

U16   file_read_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size )
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	return	(U16)FSRead(fat_g_cur_file,(void __X *)buffer,u16_buf_size, &error);
}

#if 0
Bool  file_set_eof(void)
{
	return	FSResize(*fat_g_cur_file,(*fat_g_cur_file)->position)!= FS_NO_ERROR;
}



U16   file_write_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size )
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	unsigned int	_MEM_TYPE_SLOW_	write_count;
	return	(U16)FSWrite(*fat_g_cur_file,(void __X *)buffer, u16_buf_size,&error);
}


U32   file_getpos(void)
{
	return	(*fat_g_cur_file)->position;
}

U32   file_getsize(void)
{
	return	(*fat_g_cur_file)->size;
}
#endif

Bool  file_seek( U32 u32_pos , U8 u8_whence )
{
	switch(u8_whence)
	{
		case	FS_SEEK_SET:
		{
			return	(fs_seek( fat_g_cur_file,u32_pos));
		}
		break;

		case	FS_SEEK_CUR_RE:
		{
			if(u32_pos>(fat_g_cur_file)->position)
				return	FALSE;
			return	(fs_seek( fat_g_cur_file, fat_g_cur_file->position - u32_pos));
		}
		break;

		case	FS_SEEK_CUR_FW:
		{
			return	(fs_seek(fat_g_cur_file, fat_g_cur_file->position + u32_pos) );
		}
		break;

		case	FS_SEEK_END:
		{
			if(u32_pos > fat_g_cur_file->size)
				return	FALSE;
			return	(fs_seek(fat_g_cur_file, fat_g_cur_file->size - u32_pos));
		}
		break;
	}
	return	FALSE;
}
#if 0
U16		file_getc(void)
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	if(g_b_unicode)
	{
		U16	_MEM_TYPE_SLOW_	result = 0xFFFF;
		if(FSRead(*fat_g_cur_file,&result,sizeof(result),&error)!=sizeof(result))
			return	0xFFFF;
		if(error!=FS_NO_ERROR)
			return	0xFFFF;
		return	result;
	}
	else
	{
		U8	_MEM_TYPE_SLOW_	result;
		if(FSRead(*fat_g_cur_file,&result,sizeof(result),&error)!=sizeof(result))
			return	0xFFFF;
		if(error!=FS_NO_ERROR)
			return	0xFFFF;
		return	result;
	}
}


Bool	file_putc(U16 u16_byte)
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	if(g_b_unicode)
	{
		if(FSWrite(*fat_g_cur_file,&u16_byte,sizeof(u16_byte),&error)!=sizeof(u16_byte))
			return	FALSE;
		if(error!=FS_NO_ERROR)
			return	FALSE;
	}
	else
	{
		U8	_MEM_TYPE_SLOW_	result;
		result = (U8)u16_byte;
		if(FSWrite(*fat_g_cur_file,&result,sizeof(result),&error)!=sizeof(result))
			return	FALSE;
		if(error!=FS_NO_ERROR)
			return	FALSE;
	}
	return	TRUE;
}


Bool    file_eof(void)
{
	return	(*fat_g_cur_file)->position<(*fat_g_cur_file)->size;
}

void  file_flush(void)
{
	FSFlush(*fat_g_cur_file);
}

#endif

void  file_close(void)
{
	if(fat_g_cur_file == 0)
		return;
	FSClose(fat_g_cur_file);
	fat_g_cur_file = 0;
}

#if 0
void  nav_reset(void)
{
	FS_ENTRY_INFO	_MEM_TYPE_SLOW_	entry;
	FSSeekEntry(fat_g_cur_nav,&entry,0);
}
#endif

/*
Bool  nav_select( U8 u8_idnav )
{
	if(u8_idnav >= FS_NB_NAVIGATOR)
		return	FALSE;
	fat_g_cur_file = &fat_g_files[u8_idnav];
	return	TRUE;
}
*/
#if 0
U8    nav_drive_nb(void)
{
	return	MAX_LUN;
}

Bool  nav_drive_set( U8 u8_number )
{
	return	TRUE;
}

U8    nav_drive_get(void)
{
	return	(*fat_g_cur_file)->drive;
}
#endif
int	kb_fat_check_cluster(FSINFO * fs_info,U32 cluster)
{
	cluster &= MASK_CLUSTER_32_BIT;
	if( cluster == 0)
		return	Fat_Empty_Cluster;
	if( cluster >= fs_info->last_cluster_mark )
		return	Fat_End_Cluster;
	if( cluster < 2 || cluster > fs_info->max_cluster_num )
		return	Fat_Invalid_Cluster;
	return Fat_Valid_Cluster;
}



FS_ERR FSBufSeek( FCFILE __X* pfile,U32 new_position )
{
   U32/* i, */cluster, new_cluster;
   FS_ERR err = FS_NO_ERROR;
   __X FSINFO *fsinfo;
	FCFILE file;
	U32 clus_offset;
	U32 cur_offset;
//	FCFILE *pfile;
	ClusterListItem *cluster_list;
//	U8 list_num = sizeof( cluster_buf )/ sizeof( ClusterListItem );
	int i, j = -1;

	//test_timer = timer_get_time();
//   pfile = *fat_g_cur_file;
   cluster_list = cluster_buf;

   if( 0 == pfile )
      return FS_OPENFILE_FULL;
   file = *pfile;
   fsinfo = gdrv[ file.drive ].fsinfo;  
   if( 0 == fsinfo->total_sectors )
      return FS_FORMAT_ERROR;

   SetFileSystemBusy( file.drive );

   if( 0xFFFFFFFFUL == new_position )
   {
      new_position = file.size;
   }
   else if( new_position > file.size )
   {
      err = FS_SEEK_OUT_OF_RANGE;
      goto _LEnd_FSSeek;
   }
   else if( new_position == file.position )
   {
      goto _LEnd_FSSeek;
   }

   gdrv[ file.drive ].buffer_status = EMPTY;
   clus_offset = ( new_position /( fsinfo->sectors_per_cluster )/ SECTOR_SIZE );
   //last_index = -1;
   //for( i = 0; i < list_num; ++i )
   cur_offset = 0;
   for( i = 0; i < NUM_CLUSTR; ++i )
   {
#if 1
      //if( cluster_list[ i ].disk_id == 0xFFFF )
      //   continue;

      /*if(( pfile->drive == cluster_list[ i ].disk_id )
         &&( pfile->first_cluster == cluster_list[ i ].start_cluster ))
      {
         if( cluster_list[ i ].offset_by_clus >= cur_offset )
         {
            cur_offset = cluster_list[ i ].offset_by_clus;
            j = i;
         }
         
         if(( clus_offset >= cluster_list[ i ].offset_by_clus )
            &&( clus_offset <( cluster_list[ i ].offset_by_clus + cluster_list[ i ].seg_len )))
         {
            cluster = cluster_list[ i ].cluster + clus_offset - cluster_list[ i ].offset_by_clus;
            goto fs_buf_seek_end;
            //break;
         }
      }*/
      if(( pfile->drive == cluster_list->disk_id )
         &&( pfile->first_cluster == cluster_list->start_cluster ))
      {
         if( cluster_list->offset_by_clus >= cur_offset )
         {
            cur_offset = cluster_list->offset_by_clus;
            j = i;
         }
         
         if(( clus_offset >= cluster_list->offset_by_clus )
            &&( clus_offset <( cluster_list->offset_by_clus + cluster_list->seg_len )))
         {
            cluster = cluster_list->cluster + clus_offset - cluster_list->offset_by_clus;
            goto fs_buf_seek_end;
            //break;
         }
      }
      cluster_list++;
#else
      if(( pfile->drive == cluster_list[ i ].disk_id )
         &&( pfile->first_cluster == cluster_list[ i ].start_cluster )
            &&( clus_offset >= cluster_list[ i ].offset_by_clus )
               &&( clus_offset <( cluster_list[ i ].offset_by_clus + cluster_list[ i ].seg_len )))
      {
         //
         cluster = cluster_list[ i ].cluster + clus_offset - cluster_list[ i ].offset_by_clus;
         goto fs_buf_seek_end;
         //break;
      }
      else if( cluster_list[ i ].disk_id == 0xFFFF )
      {
         break;
      }
#endif
   }
   //if( cluster_list[ i ].disk_id == 0xFFFF )
   {
#if 0
      //
      if( i == 0 )
      {
         cur_offset = 0;
         cluster = file.first_cluster;
      }
      else
      {
         ClusterListItem *p = &cluster_list[ i - 1 ];
         cur_offset = p->offset_by_clus + p->seg_len - 1;
         cluster = p->cluster + p->seg_len - 1;
      }
#else
      cur_offset = 0;
      cluster = file.first_cluster;
      if( j != -1 )
      {
   //      ClusterListItem *p = &cluster_buf[ j ];//&cluster_list[ j ];//
   		 ClusterListItem *p = &cluster_list[ j ];//
         cur_offset = p->offset_by_clus + p->seg_len - 1;
         cluster = p->cluster + p->seg_len - 1;
      }
#endif
      
      while( cur_offset != clus_offset )
      {
         new_cluster = GetFATEntry( file.drive, cluster );
         if( INVALID_CLUSTER == new_cluster )
         {
            err = FS_DRIVE_ERROR;
            goto _LEnd_FSSeek;
         }
         else if( kb_fat_check_cluster( fsinfo, new_cluster )!= Fat_Valid_Cluster )
         {
            err = FS_SEEK_OUT_OF_RANGE;
            goto _LEnd_FSSeek;
         }
         ++cur_offset;
         cluster = new_cluster;
      }
   }

fs_buf_seek_end:
   file.cluster = cluster;
   file.position = new_position;
   //test_timer = timer_get_time() - test_timer;
_LEnd_FSSeek:
   *pfile = file;
   ClearFileSystemBusy( file.drive );
   return err;
}


extern __X   FCFILE   *ADPCM_File; 
Bool fs_seek( FCFILE __X* pfile, U32 addr )
{
   FS_ERR err;
//   err = FSBufSeek( addr );

   if( pfile == fat_g_cur_file )
   {
      err = FSBufSeek( fat_g_cur_file, addr );
   }
   else if( pfile == ADPCM_File )
   {
      err = FSBufSeek( ADPCM_File,  addr );
   }
   
   if( err == FS_NO_ERROR )
      return TRUE;
   return FALSE;
}

#pragma CODE = FILE_EXTERN_CODE
/*
void  file_init(void)
{
	U16 iter;
	for(iter=0;iter<sizeof(fat_g_files)/sizeof(fat_g_files[0]);++iter)
		fat_g_files[iter] = 0;
	fat_g_cur_file = &fat_g_files[0];
	fat_g_cur_nav = &fat_g_navs[0];
	fat_g_cur_entry = &fat_g_entry[0];
	( void )FSPatchInit(fat_g_cur_nav,fat_g_cur_entry);
}

FS_ERR FSNearValidEntry( FILE_INFO __X* finfo,unsigned int index)
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	if(FSSeekEntry(finfo,fat_g_cur_entry,index)==FS_END_DIRECTORY)
		return	FS_END_DIRECTORY;
	for(;;)
	{
		if((finfo->attribute&FS_ATTR_LFN_ENTRY)!=FS_ATTR_LFN_ENTRY&&
			(finfo->attribute&FS_ATTR_VOLUME)!=FS_ATTR_VOLUME)
			return	FS_NO_ERROR;
		error = FSNextEntry(finfo,fat_g_cur_entry);
#if 0		
		SendString("\r\nfat_g_cur_nav->name == ");
		SendString((char *)fat_g_cur_nav->name);
		SendString("\r\nfat_g_cur_nav->lfn == ");
		SendString((char *)fat_g_cur_nav->lfn);
		SendString("\r\nCurrentShortFilePath == ");
		SendString(CurrentShortFilePath);
#endif		
		if(error==FS_END_DIRECTORY)
			return	error;
		if(error==FS_NO_ERROR)
			return	error;
	}
}

FS_ERR FSNextValidEntry( FILE_INFO __X* finfo)
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	for(;;)
	{
		error = FSNextEntry(finfo,fat_g_cur_entry);
#if 0		
		SendString("\r\nfat_g_cur_nav->name == ");
		SendString((char *)fat_g_cur_nav->name);
		SendString("\r\nfat_g_cur_nav->lfn == ");
		SendString((char *)fat_g_cur_nav->lfn);
		SendString("\r\nCurrentShortFilePath == ");
		SendString(CurrentShortFilePath);
#endif		
		if(error==FS_END_DIRECTORY)
			return	error;
		if(error==FS_NO_ERROR)
			return	error;
	}
}

#if 0
FS_ERR FSPrevValidEntry( FILE_INFO __X*  finfo)
{
	FS_ERR	_MEM_TYPE_SLOW_	error;
	for(;;)
	{
		error = FSPrevEntry(finfo,fat_g_cur_entry);
#if 0		
		SendString("\r\nfat_g_cur_nav->name == ");
		SendString((char *)fat_g_cur_nav->name);
		SendString("\r\nfat_g_cur_nav->lfn == ");
		SendString((char *)fat_g_cur_nav->lfn);
		SendString("\r\nCurrentShortFilePath == ");
		SendString(CurrentShortFilePath);
#endif		
		if(error==FS_END_DIRECTORY)
			return	error;
		if(error==FS_NO_ERROR)
			return	error;
	}
}
#endif


Bool  nav_filelist_reset(void)
{
	return	FSSeekEntry(fat_g_cur_nav,fat_g_cur_entry,0) != FS_NO_ERROR;
}

Bool  nav_filelist_set( U16 u16_nb , Bool b_direction )
{
	++u16_nb;
#if 1
	while(u16_nb)
	{
		if(FSNextValidEntry(fat_g_cur_nav)!=FS_NO_ERROR)
			return	FALSE;
		--u16_nb;
	}
#else
	if(b_direction)
	{
		while(u16_nb)
		{
			if(FSNextValidEntry(fat_g_cur_nav)!=FS_NO_ERROR)
				return	FALSE;
			--u16_nb;
		}
	}
	else
	{
		while(u16_nb)
		{
			if(FSPrevValidEntry(fat_g_cur_nav)!=FS_NO_ERROR)
				return	FALSE;
			--u16_nb;
		}
	}
#endif	
	return	TRUE;
}

#if 1

U16   nav_filelist_get(void)
{
	return	fat_g_cur_nav->index;
}

#if 0
Bool  nav_filelist_goto( U16 u16_newpos )
{
	FS_ENTRY_INFO	_MEM_TYPE_SLOW_	entry;
	return	FSSeekEntry(fat_g_cur_nav,&entry,u16_newpos) == FS_NO_ERROR;
}
#endif
*/
/*
int	str_comp_u(U16  * sl,U16  * sr,U8 use_case)
{
	if(use_case)
	{
		while(*sl&&*sl==*sr)
		{
			++sl;
			++sr;
		}
		return	*sl - *sr;
	}
	else
	{
		while(*sl&&tolower(*sl)==tolower(*sr))
		{
			++sl;
			++sr;
		}
		return	tolower(*sl)-tolower(*sr);
	}
}

U8 *	str_copy(U8 * dst,U8 * src)
{
	U8 *	old_ptr = dst;
	while(*dst++=*src++);
	return	old_ptr;
}

U16 *	str_copy_u(U16 * dst,U16 * src)
{
	U16 *	old_ptr = dst;
	while(*dst++=*src++);
	return	old_ptr;
}

U16 *	str_copy_u_n(U16 * dst,U16 * src,U16 num)
{
	U16 *	old_ptr = dst;
	while(num)
	{
		*dst++ = *src++;
		if(*src == 0)
			break;
		--num;
	}
	if(num)
		*dst = 0;
	return	old_ptr;
}

//unicode字符串转化为本地字符串(ascii字符串)
void	unicode_to_local(U16 * unicode_str,U8 * local_str,U16 buffer_size)
{
	while(*unicode_str)
	{
		if(*unicode_str<256)
		{
			*local_str++=*unicode_str++;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
		}
		else
		{
			*local_str++=*unicode_str;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			*local_str++=((*unicode_str)&0xFF)>>8;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			++unicode_str;
		}
	}
	*local_str = 0;
}
//本地字符串(ascii字符串)转化为unicode字符串
void	local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size)
{
	while(*local_str)
	{
		*unicode_str++=*local_str++;
		buffer_size -= 2;
		if(buffer_size<=3)
		{
			*unicode_str = 0;
			return;
		}
	}
	*unicode_str = 0;
}
#endif
*/
void	nav_string_unicode(void)
{
	g_b_unicode = TRUE;
}
void	nav_string_ascii(void)
{
	g_b_unicode = FALSE;
}

/*
int	str_comp_a(U8 * sl,U8 * sr,U8 use_case)
{
	if(use_case)
	{
		while(*sl&&*sl==*sr)
		{
			++sl;
			++sr;
		}
		return	*sl - *sr;
	}
	else
	{
		while(*sl&&tolower(*sl)==tolower(*sr))
		{
			++sl;
			++sr;
		}
		return	tolower(*sl)-tolower(*sr);
	}
}

U8 *	str_copy_n(U8 * dst,U8 * src,U16 num)
{
	U8 *	old_ptr = dst;
	while(num)
	{
		*dst++ = *src++;
		if(*src == 0)
			break;
		--num;
	}
	if(num)
		*dst = 0;
	return	old_ptr;
}
*/
/*
Bool  nav_filelist_findname(FS_STRING sz_name , Bool b_match_case )
{
	unsigned char	_MEM_TYPE_SLOW_	buffer[FS_MAX_NAME_BUFFER];
//	if(FSSeekEntry(fat_g_cur_nav,fat_g_cur_entry,0)==FS_END_DIRECTORY)
//		return	FALSE;
	if(FSNearValidEntry(fat_g_cur_nav,0)==FS_END_DIRECTORY)
		return	FALSE;		
	if(g_b_unicode)
	{
#if	1
		do
		{
		//	local_to_unicode((U8 *)fat_g_cur_nav->lfn,(U16 *)buffer,sizeof(buffer));
			if(str_comp_u((U16 *)sz_name,(U16 *)fat_g_cur_nav->lfn,b_match_case) == 0)
				return	TRUE;
			unicode_to_local((U16 *)sz_name, buffer,sizeof(buffer));
			if(str_comp_a((U8 *)buffer,(U8 *)fat_g_cur_nav->name,b_match_case) == 0)
				return	TRUE;
		}
		while(FSNextValidEntry(fat_g_cur_nav)==FS_NO_ERROR);
#endif		
	}
	else
	{
		do
		{
			if(str_comp_a((U8 *)sz_name,(U8 *)fat_g_cur_nav->lfn,b_match_case) == 0)
				return	TRUE;
			if(str_comp_a((U8 *)sz_name,(U8 *)fat_g_cur_nav->name,b_match_case) == 0)
				return	TRUE;
		}
		while(FSNextValidEntry(fat_g_cur_nav)==FS_NO_ERROR);
	}
	return	FALSE;
}

Bool  nav_dir_root(void)
{
//	return	FSGotoEntry("0:\\",0,fat_g_cur_nav,fat_g_cur_entry) == FS_NO_ERROR;
//	return	FSGetEntry("0:\\",0,fat_g_cur_nav,fat_g_cur_entry) == FS_NO_ERROR;
	( void )FSPatchInit(fat_g_cur_nav,fat_g_cur_entry);
	return	TRUE;
}
Bool  nav_dir_cd(void)
{
	return	FSSubdirEntry(fat_g_cur_nav,fat_g_cur_entry) == FS_NO_ERROR;
}


Bool  nav_file_name( FS_STRING sz_name , U8 u8_size_max , Bool b_mode , Bool b_match_case  )
{
	unsigned char	_MEM_TYPE_SLOW_	buffer[FS_MAX_NAME_BUFFER];
	if(g_b_unicode)
	{
#if 1
		if(b_mode == FS_NAME_GET)
		{
#if 0
			local_to_unicode((U8 *)fat_g_cur_nav->name,(U16 *)buffer,sizeof(buffer));
			str_copy_u_n((U16 *)sz_name,(U16 *)buffer,min(13,sizeof(buffer)));
#else
			if(*(U16 *)fat_g_cur_nav->lfn)
			{
			//	local_to_unicode((U8 *)fat_g_cur_nav->lfn,(U16 *)buffer,sizeof(buffer));
				str_copy_u_n((U16 *)sz_name,(U16 *)fat_g_cur_nav->lfn,min(u8_size_max*2,sizeof(buffer)));
			}
			else
			{
				local_to_unicode((U8 *)fat_g_cur_nav->name,(U16 *)buffer,sizeof(buffer));
				str_copy_u_n((U16 *)sz_name,(U16 *)buffer,min(13,sizeof(buffer)));
			}
#endif			
			return	TRUE;
		}
		else
		{
		//	local_to_unicode((U8 *)fat_g_cur_nav->lfn,(U16 *)buffer,sizeof(buffer));
			if(str_comp_u((U16 *)sz_name,(U16 *)fat_g_cur_nav->lfn,b_match_case))
				return	TRUE;
			local_to_unicode((U8 *)fat_g_cur_nav->name,(U16 *)buffer,sizeof(buffer));
			if(str_comp_u((U16 *)sz_name,(U16 *)buffer,b_match_case))
				return	TRUE;
		}
#else
		return	FALSE;
#endif
	}
	else
	{
		if(b_mode == FS_NAME_GET)
		{
			if(*(U16 *)fat_g_cur_nav->lfn)
			{
			//	str_copy_n((U8 *)sz_name,fat_g_cur_nav->lfn,u8_size_max);
				unicode_to_local((U16 *)fat_g_cur_nav->lfn,(U8 *)sz_name,sizeof(fat_g_cur_nav->lfn));
			}
			else
			{
				str_copy_n((U8 *)sz_name,fat_g_cur_nav->name,u8_size_max);
			}
			return	TRUE;
		}
		else
		{
			if(str_comp_a((U8 *)sz_name,(U8 *)fat_g_cur_nav->lfn,b_match_case))
				return	TRUE;
			if(str_comp_a((U8 *)sz_name,(U8 *)fat_g_cur_nav->name,b_match_case))
				return	TRUE;
		}
	}
	return	FALSE;
}
*/

#if 0
Bool  nav_dir_make( FS_STRING sz_name  )
{
	char	__X	temp[256];
	FS_ERR	error;
//	SendString((char*)CurrentShortFilePath);
	strcpy((char *)temp,(char *)CurrentShortFilePath);
	strcat((char *)temp,(char *)"\\");
	strcat((char *)temp,(char *)sz_name);
//	SendString((char*)"\r\n");
//	SendString(( char*)CurrentShortFilePath);
//	SendString(( char*)"\r\n");
//	SendString(( char*)temp);
	error = FSDirectory((char *)temp);
	if(error == FS_DIRECTORY_CONFLICT)
		fs_g_status = FS_ERR_FILE_EXIST;
	return	(error == FS_NO_ERROR);
}
//Bool  nav_file_name( FS_STRING sz_name , U8 u8_size_max , Bool b_mode , Bool b_match_case  );
#endif
/*
Bool  nav_file_isdir(void)
{
	return	((fat_g_cur_nav->attribute&FS_ATTR_LFN_ENTRY)!=FS_ATTR_LFN_ENTRY)&&(fat_g_cur_nav->attribute&FS_ATTR_DIRECTORY);
}
Bool  nav_file_checkext( FS_STRING sz_filterext )
{
	unsigned int	_MEM_TYPE_SLOW_	dot_index;
	for(dot_index=0;dot_index<SHORT_NAME_SIZE;++dot_index)
		if(fat_g_cur_nav->name[dot_index] == '.')
			break;
	if(dot_index == SHORT_NAME_SIZE)
		return	FALSE;
	
	return	str_comp_a((U8 *)(&fat_g_cur_nav->name[dot_index+1]),(U8 *)sz_filterext,FALSE) == 0;
}

*/
#if 0
Bool  nav_file_del( Bool b_only_empty )
{
	extern	char	__X	CurrentShortFilePath[512];
	char	__X	temp[256];
	FS_ERR	__X	error;
	FCFILE	__X *	__X  file;
	strcpy((char *)temp,(char *)CurrentShortFilePath);
	strcat((char *)temp,(char *)"\\");
	strcat((char *)temp,(char *)fat_g_cur_nav->name);
	return	FSDelete(temp)== FS_NO_ERROR;
}
#endif
#pragma CODE = FS_CREATE_BUF_CODE
Bool FSCreateBuf( FCFILE *pfile )
{
   //U32 test_timer;
   U32 cluster, new_cluster;
   int list_index, i;
   FSINFO *fsinfo;
//   FCFILE *pfile;
   ClusterListItem *cluster_list;
//   U8 list_num;

   Bool clear_cluster_list = FALSE;

   //test_timer = timer_get_time();

//   pfile = *fat_g_cur_file;
//   list_num = sizeof( cluster_buf )/ sizeof( ClusterListItem );
   cluster_list = cluster_buf;
   fsinfo = gdrv[ pfile->drive ].fsinfo;  
   if( fsinfo == NULL )
      return FALSE;
   if( !b_clusterlist )
   {
      for( list_index = 0; list_index < NUM_CLUSTR; ++list_index )
      {
         cluster_list[ list_index ].disk_id = 0xFFFF;
         cluster_list[ list_index ].start_cluster = fsinfo->last_cluster_mark;
      }
      b_clusterlist = TRUE;
   }
   else
   {
      for( list_index = 0; list_index < NUM_CLUSTR; ++list_index )
      {
         if(( pfile->drive == cluster_list[ list_index ].disk_id )
            &&( pfile->first_cluster == cluster_list[ list_index ].start_cluster ))
            return TRUE;
      }
      for( list_index = 0; list_index < NUM_CLUSTR; ++list_index )
      {
         if( cluster_list[ list_index ].disk_id == 0xFFFF )
            goto null_cluster_list;
      }
   }
   
   list_index = 0;
null_cluster_list:
   i = 0;
   if( cluster_list[ list_index ].disk_id != 0xFFFF )
      clear_cluster_list = TRUE;
   cluster_list[ list_index ].disk_id = pfile->drive;
   cluster_list[ list_index ].start_cluster = pfile->first_cluster;
   cluster_list[ list_index ].cluster = pfile->first_cluster;
   cluster_list[ list_index ].seg_len = 1;
   cluster_list[ list_index ].offset_by_clus = 0;
   cluster = pfile->first_cluster;
   while( 1 )
   {
      new_cluster = GetFATEntry( pfile->drive, cluster );
      if( new_cluster ==( cluster + 1 ))
      {
         ++cluster_list[ list_index ].seg_len;
      }
      else
      {
         if( kb_fat_check_cluster( fsinfo, new_cluster )!= Fat_Valid_Cluster )
         {
            break;
         }
         if( i >=( NUM_CLUSTR - 1 ))
            break;
         if( list_index ==( NUM_CLUSTR - 1 ))
         {
            if( cluster_list[ 0 ].disk_id != 0xFFFF )
               clear_cluster_list = TRUE;
            cluster_list[ 0 ].disk_id = pfile->drive;
            cluster_list[ 0 ].start_cluster = pfile->first_cluster;
            cluster_list[ 0 ].cluster = new_cluster;
            cluster_list[ 0 ].seg_len = 1;
            cluster_list[ 0 ].offset_by_clus = cluster_list[ list_index ].offset_by_clus + cluster_list[ list_index ].seg_len;
            
            list_index = -1;
         }
         else
         {
            if( cluster_list[ list_index + 1 ].disk_id != 0xFFFF )
               clear_cluster_list = TRUE;
            cluster_list[ list_index + 1 ].disk_id = pfile->drive;
            cluster_list[ list_index + 1 ].start_cluster = pfile->first_cluster;
            cluster_list[ list_index + 1 ].cluster = new_cluster;
            cluster_list[ list_index + 1 ].seg_len = 1;
            cluster_list[ list_index + 1 ].offset_by_clus = cluster_list[ list_index ].offset_by_clus + cluster_list[ list_index ].seg_len;
         }
         ++list_index;
         ++i;
      }
      cluster = new_cluster;
   }
   if( clear_cluster_list )
   {
      /*++list_index;
      if( list_index >= FS_NB_CACHE_CLUSLIST )
         list_index = 0;*/
      for( i = 0; i < NUM_CLUSTR; ++i )
      {
         ++list_index;
         if( list_index >= NUM_CLUSTR )
            list_index = 0;
         if( cluster_list[ list_index ].disk_id != 0xFFFF )
         {
            if( cluster_list[ list_index ].offset_by_clus == 0 )
               break;
         }
         cluster_list[ list_index ].disk_id = 0xFFFF;
         /*list_index++;
         if( list_index >= FS_NB_CACHE_CLUSLIST )
            list_index = 0;*/
      }
   }
   return TRUE;
}

/*
int	FSCreateBuf(void)
{
	U32	test_timer;
	U32	cluster,new_cluster;
	int	iter,list_index;
	FSINFO * fsinfo;
	FCFILE* pfile;ClusterListItem  * cluster_list;U8 list_num;
	//test_timer = timer_get_time();
	pfile = *fat_g_cur_file;
	list_num = sizeof(cluster_buf)/sizeof(ClusterListItem);
	cluster_list = cluster_buf;
	fsinfo              = gdrv[pfile->drive].fsinfo;  
	if(fsinfo == NULL)
		return	FALSE;
	for(list_index = 0;list_index<list_num;++list_index)
	{
		cluster_list[list_index].disk_id = 0xFFFF;
		cluster_list[list_index].start_cluster = fsinfo->last_cluster_mark;
	}
	list_index = 0;
	cluster_list[list_index].disk_id = pfile->drive;
	cluster_list[list_index].start_cluster = pfile->first_cluster;
	cluster_list[list_index].cluster = pfile->first_cluster;
	cluster_list[list_index].seg_len = 1;
	cluster_list[list_index].offset_by_clus = 0;
	cluster =pfile->first_cluster;
	for(;;)
	{
		new_cluster = GetFATEntry(pfile->drive, cluster);
		if(new_cluster == cluster + 1)
		{
			++cluster_list[list_index].seg_len;
		}
		else
		{
			if(kb_fat_check_cluster(fsinfo,new_cluster)!=Fat_Valid_Cluster)
			{
				break;
			}
			if(list_index == list_num-1)
			{
				break;
			}
			cluster_list[list_index+1].disk_id = pfile->drive;
			cluster_list[list_index+1].start_cluster = pfile->first_cluster;
			cluster_list[list_index+1].cluster = new_cluster;
			cluster_list[list_index+1].seg_len = 1;
			cluster_list[list_index+1].offset_by_clus = cluster_list[list_index].offset_by_clus+cluster_list[list_index].seg_len;
			++list_index;
		}
		cluster = new_cluster;
	}
//	test_timer = timer_get_time() - test_timer;
//	list_index = test_timer;
//	list_index = timer_get_time();
//	list_index = timer_get_time() - list_index;
	return	TRUE;
}
*/

