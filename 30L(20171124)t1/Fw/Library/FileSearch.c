//--------------------------------------------------------------------------------------------------
//#define FILESEARCH_CPP
//--------------------------------------------------------------------------------------------------
#include <fs.h>
#include "string.h"
#include "FileSearch.h"
//#include "_FileSearch.h"
//#include "compiler.h"
#include "com.h"
//--------------------------------------------------------------------------------------------------
#pragma DATA = FILESEARCH_DATA
#pragma CODE = FILESEARCH_CODE
//--------------------------------------------------------------------------------------------------
char	__X	g_full_path[128];
FILE_INFO __X	fat_g_info;
unsigned int current_find_num;
//==================================================




 
//get ap3 file name's header
U32 file_name_lead( void )
{
#if 0
#ifdef UNCOIDE_FORMAT
   U16 *p = ( U16 *)( g_string );
#else
   U8 *p = ( U8 *)( g_string );
#endif
#else
	U8 *p = ( U8 *)( g_full_path );
#endif
   _MEM_TYPE_SLOW_ U32 iNameLead;

   //bRemoteChoiceAp3 = FALSE;
   iNameLead = 0;
   if( !((*p >= '0' )&&( *p <= '9')))
   {
      //bRemoteChoiceAp3 = TRUE;
      return 0xFFFFFFFF;
   }
   while((*p >= '0' )&&( *p <= '9'))
   {
      iNameLead = iNameLead * 10 + *p - '0';
      p++;
   }
   //SendString((U8 *)"\r\nnamelead=");
   //send_hex_u16(iNameLead);
   return iNameLead;
}


char * get_ext(char* str)
{
	int	index;
	int	last;
	last = index = 0;
	while(str[last])
	{
		if(str[last] == '.')
			index = last;
		++last;
	}
	if(index == 0)
		index = last;
	else
		++index;
	return	&str[index];
}
//============================================================
int	file_get_ext_count(char * dir_path,char * ext)
{
	FILE_INFO	__X	finfo;
	FS_ENTRY_INFO	__X	fentry;
	FS_ERR    err;
	int	count = 0;
	unsigned int i;
#if 0//def TX_DEBUG
	SendString("\r\nfile_get_ext_count");
#endif	
	memset((char *)&finfo,0,sizeof(finfo));
#if 0	
	err = FSGetEntry(dir_path,0,&finfo,&fentry);
#else
    for( i = 0; ; ++i ) {
        err = FSGetEntry(dir_path,i,&finfo,&fentry);
        if( err == FS_NO_ERROR )
            break;
        if( err == FS_FILE_NOT_FOUND )
            continue;
        return 0;
    }
#endif	
#if 0//def TX_DEBUG
	SendString("\r\n42_err=");
	send_hex_u8(err);
	SendString("\r\n");SendString((char *)dir_path);
	SendString("\r\n");SendString((char *)ext);
#endif
	//if(err!=FS_NO_ERROR)
	//	return	0;
	do
	{
		if(( finfo.attribute & FS_ATTR_DIRECTORY )!= FS_ATTR_DIRECTORY )
		{
#ifdef TX_DEBUG
	SendString("\r\nfinfo.namer=");
	SendString((char *)finfo.name);
#endif		
			if(strcmp(ext,get_ext((char *)finfo.name))==0){
#if 0//def TX_DEBUG
	SendString("\r\nfinfo.namer=");
	SendString((char *)finfo.name);
#endif
				if((finfo.size != 0)||(finfo.size != 0xFFFFFFFF)){
						++count;
					}
				
				}
		}
	}
	while(FSGetNextEntry(&finfo,&fentry)==FS_NO_ERROR);
	return	count;
}

//--------------------------------------------------------------------------------
#if 0
Bool file_get_path(char * dir_path,char * ext,int index)
{
	FILE_INFO	__X	finfo;
	FS_ENTRY_INFO	__X	fentry;
	FS_ERR    err;
	++index;
	g_full_path[0] = 0;
	err = FSGetEntry(dir_path,0,&finfo,&fentry);
#ifdef TX_DEBUG
	SendString("\r\n70_err:index=");
	send_hex_u8(err);
	SendString(":");
	send_hex_u16(index);
#endif	
	if(err!=FS_NO_ERROR)
		return	FALSE;


	
	while(index)
	{
		if(!(finfo.attribute&FS_ATTR_DIRECTORY))
		{

		 if(strcmp(ext,get_ext((char *)finfo.name))==0)
			{
				//--index;
#ifdef TX_DEBUG
	SendString("\r\nfinfo.namer=");
	SendString((char *)finfo.name);
#endif
				if((finfo.size != 0)||(finfo.size != 0xFFFFFFFF)){
					--index;
					}
			}
		}
		if(index == 0)
		{
			strcpy(g_full_path,dir_path);
			strcat(g_full_path,"\\");
			strcat(g_full_path,(char *)finfo.name);

			memcpy((char *)fat_g_info.name,(char *)finfo.name,sizeof(FILE_INFO));
			//strcpy((char *)fat_g_info.name,(char *)finfo.name);
			//strcpy((char *)fat_g_info.attribute,(char *)finfo.name);
			return	TRUE;
		}
		if(FSGetNextEntry(&finfo,&fentry)!=FS_NO_ERROR)
			break;
	}
#ifdef TX_DEBUG
	SendString("\r\nnofind!");
#endif	
	return	FALSE;
}
#else
Bool file_get_path(char * dir_path,char * ext,int index)
{
	FILE_INFO	__X	finfo;
	FS_ENTRY_INFO	__X	fentry;
	FS_ERR    err;
    int temp_index = index + 1;
    unsigned int i;
	//++index;
	memset((char *)&finfo,0,sizeof(finfo));
	g_full_path[0] = 0;
#if 0	
	err = FSGetEntry(dir_path,0,&finfo,&fentry);
	if(err!=FS_NO_ERROR)
		return	FALSE;
#else
   for( i = 0; ; ++i ) {
        err = FSGetEntry(dir_path,i,&finfo,&fentry);
        if( err == FS_NO_ERROR )
            break;
        if( err == FS_FILE_NOT_FOUND )
            continue;
        return false;
    }
#endif	
#if 0//def TX_DEBUG
	SendString("\r\n70_err:index=");
	send_hex_u8(err);
	SendString(":");
	send_hex_u16(temp_index);
#endif	

	while(temp_index)
	{
#if 0//def TX_DEBUG
	SendString("\r\nfinfo=");
	SendString((char *)finfo.name);
	SendString(":");
	send_hex_u8(finfo.attribute);
	SendString(":");
	send_hex_u32(finfo.size);
	SendString(":");
	send_hex_u8(finfo.index);
	SendString(":");
	SendString((char *)finfo.name);
#endif	
		if(( finfo.attribute & FS_ATTR_DIRECTORY )!= FS_ATTR_DIRECTORY )
		{

		 if(strcmp(ext,get_ext((char *)finfo.name))==0)
			{
				--temp_index;
#if 0//def TX_DEBUG
	SendString("\r\nfinfo.namer=");
	SendString((char *)finfo.name);
#endif
				//if((finfo.size != 0)||(finfo.size != 0xFFFFFFFF)){
				//	--index;
				//	}
			}
		}
		if(temp_index == 0)
		{
			strcpy(g_full_path,dir_path);
			strcat(g_full_path,"\\");
			strcat(g_full_path,(char *)finfo.name);

			memcpy((char *)fat_g_info.name,(char *)finfo.name,sizeof(FILE_INFO));
			//strcpy((char *)fat_g_info.name,(char *)finfo.name);
			//strcpy((char *)fat_g_info.attribute,(char *)finfo.name);
			return	TRUE;
		}
		memset((char *)&finfo,0,sizeof(finfo));
		if(FSGetNextEntry(&finfo,&fentry)!=FS_NO_ERROR)
			break;
	}
#ifdef TX_DEBUG
	SendString("\r\nnofind!");
#endif	
	return	FALSE;
}
#endif



//--------------------------------------------------------------------------------

Bool search_path_file(char * dir_path,char * ext,long need_num)
{
	FILE_INFO	__X	finfo;
	//FILE_INFO __X fat_g_info;
	FS_ENTRY_INFO	__X	fentry;
	FS_ERR    err;
	unsigned int	i,count = 0;
	unsigned long	temp_data = 0;
	memset((char *)&finfo,0,sizeof(finfo));
#if 0	
	err = FSGetEntry(dir_path,0,&finfo,&fentry);
#if 0//def TX_DEBUG
	SendString("\r\n42_err=");
	send_hex_u8(err);
#endif	
	if(err!=FS_NO_ERROR)
		return	FALSE;
#else
     for( i = 0; ; ++i ) {
        err = FSGetEntry(dir_path,i,&finfo,&fentry);
        if( err == FS_NO_ERROR )
            break;
        if( err == FS_FILE_NOT_FOUND )
            continue;
        return false;
    }
#endif		
	do
	{
		if(( finfo.attribute & FS_ATTR_DIRECTORY )!= FS_ATTR_DIRECTORY )
		{
			if(strcmp(ext,get_ext((char *)finfo.name))==0){
				
				++count;
				
				strcpy(( char *)( g_full_path ),(char *)finfo.name);
#if 0//def TX_DEBUG
	SendString("\r\nname=");
	SendString((char *)finfo.name);
#endif				
				temp_data = file_name_lead();
				if( temp_data == need_num){
					current_find_num = count-1;
					SendString("\n\rd=");
					send_hex_u16(count);
					return TRUE;
					}
				}
				
		}
		memset((char *)&finfo,0,sizeof(finfo));
	}
	while(FSGetNextEntry(&finfo,&fentry)==FS_NO_ERROR);
	return	FALSE;
}



