
#pragma DATA = FILE_DATA
#pragma CODE = FILE_CODE

#include "fs.h"
//#include "fs_patch.h"
#include "string.h"
#include "fs_com.h"
#include "fat.h"
#include "file.h"
//#include "navigation.h"
//#include "com.h"
//#include "fat_var.h"

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
char	__X CurrentShortFilePath[512];
/*
FCFILE __X *		fat_g_files[FS_NB_NAVIGATOR];
FILE_INFO __X		fat_g_navs[1];
FS_ENTRY_INFO __X	fat_g_entry[1];
FILE_INFO __X	*	fat_g_cur_nav;
FS_ENTRY_INFO	__X *	fat_g_cur_entry;
*/
FCFILE __X *	fat_g_cur_file;


//��ϣ��FILE_INFO �ṹ���Գ�Ϊһ�����ļ����ָ��һ���Ľṹ��ͨ����������Ŀ¼���ƶ���ͨ����ǰ��������ϲ㣬���²��ƶ��Ĳ���������Ŀ¼��ָ��ͬ���ļ���Ŀ¼�����FILE_INFO���㹻��������Ĺ��ܣ���������FILE_INFO�ṹ��
/*
FS_ERR  FSGotoEntry (char *spec,unsigned int index,FILE_INFO *finfo, FS_ENTRY_INFO *fentry);
//��FSGetEntry���ƵĹ��ܣ� 
//1.spec����ȫ·����
//(1)Ϊ���ļ���·�� 
//(2)���spec�����һ���ַ�Ϊ'/'��'\\'����ʾ����·����Ŀ¼��ָ���index��entry������FSGotoEntry("0:\\123\\",3,&info,&entry);info�ͽ�����123Ŀ¼������ָ���3��entry
//(3)���spec�����һ���ַ���Ϊ'/'��'\\'����ʾָ��·�����ļ���index���ԣ�����FSGotoEntry("0:\\123\\456.mp3",0,&info,&entry);info�ͽ�����123Ŀ¼������ָ��456.mp3�ļ���

FS_ERR  FSGotoEntry_u(unsigned short *spec,unsigned int index,FILE_INFO *finfo, FS_ENTRY_INFO *fentry);
//FSGotoEntry������unicode�汾��ֻ����specΪunicode�ִ���������ͬ��

FS_ERR FSNextEntry( FILE_INFO * finfo);	//finfo����ƶ�����FSGetNextEntry����
FS_ERR FSPrevEntry( FILE_INFO *  finfo);	//finfo��ǰ�ƶ���
FS_ERR FSParentEntry( FILE_INFO * finfo);	//finfo���ϲ��ƶ������finfo���ڵ�Ŀ¼���ϲ�Ŀ¼�Ļ���
FS_ERR FSSubdirEntry( FILE_INFO * finfo);	//finfo���²��ƶ������finfoָ�����Ŀ¼�Ļ��ͽ��������¼��
FS_ERR FSSeekEntry(FILE_INFO *finfo,unsigned int index);	//finfo��λ��index��λ��

FCFILE *  FSOpenEntry( FILE_INFO * finfo, unsigned int mode, FS_ERR *err);
//��FSOpen�����������ƣ���finfoָ����ļ��������ļ������

FCFILE *  FSOpenEntryForFastSeek ( FILE_INFO * finfo, unsigned int mode, unsigned int *cache_buf, unsigned int buf_size, FS_ERR *err);
//��FSOpenForFastSeek �����������ƣ���finfoָ����ļ��������ļ������

FS_ERR  FSCreateDirectoryEntry ( FILE_INFO* finfo,char *spec);
//��finfo���ڵ�Ŀ¼�´�����Ϊspec�ִ���Ŀ¼,specΪ���ļ���������·������
FS_ERR  FSCreateDirectoryEntry_u( FILE_INFO * finfo,unsigned short *spec);
//FSCreateDirectoryEntry ��unicode�汾

//�����漰���ļ��������ĺ����ӿ���ö�����ascii���ַ������ļ���������п��ܵĻ���������ṩһ��unicode���ַ��г��ļ����ӿڣ�
*/

//#pragma CODE = FILE_CODE
/* Private functions ---------------------------------------------------------*/
/*
void FSPatchInit(__X FILE_INFO *finfo,__X FS_ENTRY_INFO *fentry)
{
	int i;
	memset(CurrentShortFilePath,0,512);
	
	strcpy((char *)CurrentShortFilePath,"0:\\");
	FSGetEntry(CurrentShortFilePath, 0, finfo, fentry);
}

void	nav_string_unicode(void)
{
	g_b_unicode = TRUE;
}
void	nav_string_ascii(void)
{
	g_b_unicode = FALSE;
}


#define FS_MODE_CREATE      (0)
#define FS_MODE_READ        (1)
#define FS_MODE_READ_WRITE  (2)

#define  FOPEN_READ_ACCESS    0x01                                                                    // autorize the read access
#define  FOPEN_WRITE_ACCESS   0x02                                                                    // autorize the write access
#define  FOPEN_CLEAR_SIZE     0x04                                                                    // reset size
#define  FOPEN_CLEAR_PTR      0x08    
*/
/*
Bool  file_open( U8 fopen_mode )
{
	unsigned int	_MEM_TYPE_SLOW_	mode;
	FS_ERR	_MEM_TYPE_SLOW_	error;
	if(fopen_mode==FOPEN_MODE_R)
		mode = FS_MODE_READ;
	else if(fopen_mode==FOPEN_MODE_R_PLUS)
		mode = FS_MODE_READ_WRITE;
	else if(fopen_mode==FOPEN_MODE_W_PLUS)
		mode = FS_MODE_CREATE;
	else
		mode = FS_MODE_READ;
//	*fat_g_cur_file = (FCFILE __X *)FSOpenEntry(fat_g_cur_nav,fat_g_cur_entry,mode,&error);
	if(error)
		return	FALSE;
	return	TRUE;
}
*/
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
Bool fs_seek( FCFILE __X* pfile, unsigned long addr)
{
	FS_ERR err;
	err = FSSeek(fat_g_cur_file,addr);
	if(err==FS_NO_ERROR)return TRUE;
	else return FALSE;
}


Bool  file_seek( U32 u32_pos , U8 u8_whence )
{
	switch(u8_whence)
	{
		case	FS_SEEK_SET:
		{
			return	(FSSeek(fat_g_cur_file,u32_pos) == FS_NO_ERROR)?TRUE : FALSE;
		}
		break;

		case	FS_SEEK_CUR_RE:
		{
			if(u32_pos>fat_g_cur_file->position)
				return	FALSE;
			return	(FSSeek(fat_g_cur_file,fat_g_cur_file->position - u32_pos) == FS_NO_ERROR)?TRUE:FALSE;
		}
		break;

		case	FS_SEEK_CUR_FW:
		{
			return	(FSSeek(fat_g_cur_file,fat_g_cur_file->position + u32_pos) == FS_NO_ERROR)?TRUE:FALSE;
		}
		break;

		case	FS_SEEK_END:
		{
			if(u32_pos > fat_g_cur_file->size)
				return	FALSE;
			return	(FSSeek(fat_g_cur_file,fat_g_cur_file->size - u32_pos) == FS_NO_ERROR)?TRUE:FALSE;
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

/*
void  file_init(void)
{
	U16 iter;
	for(iter=0;iter<sizeof(fat_g_files)/sizeof(fat_g_files[0]);++iter)
		fat_g_files[iter] = 0;
	fat_g_cur_file = &fat_g_files[0];
	fat_g_cur_nav = &fat_g_navs[0];
	fat_g_cur_entry = &fat_g_entry[0];
	FSPatchInit(fat_g_cur_nav,fat_g_cur_entry);
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

Bool  nav_filelist_reset(void)
{
	return	FSSeekEntry(fat_g_cur_nav,fat_g_cur_entry,0) != FS_NO_ERROR;
}

Bool  nav_filelist_set( U16 u16_nb , Bool b_direction )
{
	++u16_nb;
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
	return	TRUE;
}

#if 1

U16   nav_filelist_get(void)
{
	return	fat_g_cur_nav->index;
}

Bool  nav_filelist_goto( U16 u16_newpos )
{
	FS_ENTRY_INFO	_MEM_TYPE_SLOW_	entry;
	return	FSSeekEntry(fat_g_cur_nav,&entry,u16_newpos) == FS_NO_ERROR;
}

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

//unicode�ַ���ת��Ϊ�����ַ���(ascii�ַ���)
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
//�����ַ���(ascii�ַ���)ת��Ϊunicode�ַ���
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

void	nav_string_unicode(void)
{
	g_b_unicode = TRUE;
}
void	nav_string_ascii(void)
{
	g_b_unicode = FALSE;
}


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


Bool  nav_filelist_findname(FS_STRING sz_name , Bool b_match_case )
{
	unsigned char	_MEM_TYPE_SLOW_	buffer[FS_MAX_NAME_BUFFER];

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
	FSPatchInit(fat_g_cur_nav,fat_g_cur_entry);
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
			if(fat_g_cur_nav->lfn[0]!=0)
			{
			//	local_to_unicode((U8 *)fat_g_cur_nav->lfn,(U16 *)buffer,sizeof(buffer));
				str_copy_u_n((U16 *)sz_name,(U16 *)fat_g_cur_nav->lfn,min(u8_size_max*2,sizeof(buffer)));
			}
			else
			{
				local_to_unicode((U8 *)fat_g_cur_nav->name,(U16 *)buffer,sizeof(buffer));
				str_copy_u_n((U16 *)sz_name,(U16 *)buffer,min(13,sizeof(buffer)));
			}
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
			if(fat_g_cur_nav->lfn[0]!=0)
			{
				str_copy_n((U8 *)sz_name,fat_g_cur_nav->lfn,u8_size_max);
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

/*
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
*/
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
*/
