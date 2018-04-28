U8 *	str_cat(U8 * dst,U8 * src)
{
	U8 * old_ptr = dst;
	while(*dst)
		++dst;
	while(*src)
	{
		*dst++=*src++;
	}
	*dst = 0;
	return	old_ptr;
}

char * get_ext(char * str,int str_size)
{
	int	index;
	int	last;
	last = index = 0;
	while(last<str_size)
	{
		if(str[last] == '\0')
			break;
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

int	file_get_ext_count(char * dir_path,char * ext)
{
	FILE_INFO	__X	finfo;
	FS_ENTRY_INFO	__X	fentry;
	int	count = 0;
	
	if(FSGetEntry(dir_path,0,&finfo,&fentry)!=FS_NO_ERROR)
		return	0;
	do
	{
		if((finfo.attribute!=FS_ATTR_LFN_ENTRY)&&!(finfo.attribute&FS_ATTR_DIRECTORY))
		{
			if(str_comp_a((U8 *)ext,(U8 *)get_ext(finfo.lfn),FALSE)==0)
				++count;
			else if(str_comp_a((U8 *)ext,(U8 *)get_ext(finfo.name),FALSE)==0)
				++count;
		}
	}
	while(FSGetNextEntry(&finfo,&fentry)==FS_NO_ERROR);
	return	count;
}
char	__X	g_full_path[256];
char *	file_get_path(char * dir_path,char * ext,int index)
{
	FILE_INFO	__X	finfo;
	FS_ENTRY_INFO	__X	fentry;
	U8 * str;
	++index;
	g_full_path[0] = 0;
	if(FSGetEntry(dir_path,0,&finfo,&fentry)!=FS_NO_ERROR)
		return	0;
	while(index)
	{
		if((finfo.attribute!=FS_ATTR_LFN_ENTRY)&&!(finfo.attribute&FS_ATTR_DIRECTORY))
		{
			if(str_comp_a((U8 *)ext,(U8 *)get_ext(finfo.lfn),FALSE)==0)
			{
				--index;
				str = (U8 *)finfo.lfn;
			}
			else if(str_comp_a((U8 *)ext,(U8 *)get_ext(finfo.name),FALSE)==0)
			{
				--index;
				str = (U8 *)finfo.name;
			}
		}
		if(index == 0)
		{
			str_copy_n((U8 *)g_full_path,dir_path,sizeof(g_full_path));
			str_cat((U8 *)g_full_path,"\\");
			str_cat((U8 *)g_full_path,str);
			return	g_full_path;
		}
	}
	while(FSGetNextEntry(&finfo,&fentry)==FS_NO_ERROR);
	return	g_full_path;
}
