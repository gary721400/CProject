#pragma DATA = KBFAT_XRAM
#pragma CODE = KBFAT_CODE


#include "KBDM9_DDK.h"
#include <string.h>
#include "Example_USB_MAssStorage.h"
//#include "CodeImg.h"
#include "SYSTEM.H"
#include "kbfat.h"
//#include "SDImg.h"
#include "config.h"
//#include "conf_kb.h"
#include "fs.h"
#include "fc_internal.h"

extern void	local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size);
extern int		str_comp_u(U16  * sl,U16  * sr,U8 use_case);


void	kb_fat_buffer_init(KBFatBuffer * b,U8 * p)
{
	b->buffer = p;
	b->sector_addr = 0xFFFFFFFF;
	b->is_modified = FALSE;
	b->disk_id = 0xFF;
}



Bool	kb_fat_buffer_flush(KBFatBuffer * b);

Bool	kb_fat_buffer_load(KBFatBuffer * b,U8 disk_id,U32 sector_addr)
{
	if(!kb_fat_buffer_flush(b))
		return	FALSE;
	if(disk_id!=b->disk_id||sector_addr!=b->sector_addr)
	{
		if(sdtl_read(sector_addr,b->buffer)!=0)
			return	FALSE;
		b->disk_id = disk_id;
		b->sector_addr = sector_addr;
	}
	return	TRUE;
}

void	kb_fat_buffer_set_modified(KBFatBuffer * b)
{
	b->is_modified = TRUE;
}

Bool	kb_fat_buffer_flush(KBFatBuffer * b)
{
	if(b->is_modified)
	{
		if(sdtl_write(b->sector_addr,b->buffer)!=0)
			return	FALSE;
		b->is_modified = FALSE;
	}
	return	TRUE;
}
enum
{
	Fat_Empty_Cluster
	,Fat_Valid_Cluster
	,Fat_End_Cluster
	,Fat_Invalid_Cluster
};
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

extern __X struct _drive * __X gdrv;


#define	Fat_Entry_Size	32
#define	Fat_Entry_Shift	5
#define	Fat_Entries_Per_Sec	(512/32)
#define	Fat_Entries_Sec_Shift	4
#define	Fat_Entries_Mask	(Fat_Entries_Per_Sec - 1)
Bool	kb_fat_dir_start(KBFatDir * dir,FSINFO * fsinfo,U32 start_cluster)
{
	static	U8	sector_buffer[512];
	U32	sector_addr;
	if(dir->data_buf.buffer==0)
		kb_fat_buffer_init(&dir->data_buf,sector_buffer);
	if(start_cluster>=2)
		sector_addr = fsinfo->first_dir_sector + (start_cluster-2)*fsinfo->sectors_per_cluster;
	else
		sector_addr = fsinfo->first_dir_sector;
	if(!kb_fat_buffer_load(&dir->data_buf,0,sector_addr))
		return	FALSE;
	dir->fs_info = fsinfo;
	dir->start_cluster = start_cluster;
	dir->cur_cluster = start_cluster;
	dir->entry_slot_index = 0;
	
	dir->entry_index = 0xFFFFFFFF;
	return	TRUE;
}

Bool	kb_fat_dir_entry_next(KBFatDir * dir)
{
	U32	cluster;
	U32	slot_index;
	U32	new_sector;
	int	check;
	slot_index = dir->entry_slot_index+1;
	if(slot_index%Fat_Entries_Per_Sec==0)
	{
		if(slot_index%(dir->fs_info->sectors_per_cluster<<Fat_Entries_Sec_Shift)==0)
		{
			if(kb_fat_check_cluster(dir->fs_info,dir->cur_cluster)==Fat_Valid_Cluster)
			{
				cluster = GetFATEntry(dir->data_buf.disk_id,dir->cur_cluster);
				check = kb_fat_check_cluster(dir->fs_info,cluster);
				if(check!=Fat_Valid_Cluster)
				{
					return	FALSE;
				}
			}
			else
			{
				return	FALSE;
			}
			new_sector = dir->fs_info->first_dir_sector + (cluster-2)*dir->fs_info->sectors_per_cluster;
			dir->cur_cluster = cluster;
		}
		else
			new_sector = dir->data_buf.sector_addr + 1;
		if(!kb_fat_buffer_load(&dir->data_buf,0,new_sector))
			return	FALSE;
		
	}
	dir->entry_slot_index = slot_index;
	return	TRUE;
}

U8 *	kb_fat_dir_get_entry(KBFatDir * dir)
{
	return	&(dir->data_buf.buffer[(dir->entry_slot_index&Fat_Entries_Mask)<<Fat_Entry_Shift]);
}

#define	KB_Fat_Dir_Attr_File		0x00
#define	KB_Fat_Dir_Attr_Read_Only	0x01
#define	KB_Fat_Dir_Attr_Hidden		0x02
#define	KB_Fat_Dir_Attr_System		0x04
#define	KB_Fat_Dir_Attr_Volume_ID	0x08
#define	KB_Fat_Dir_Attr_Directory	0x10
#define	KB_Fat_Dir_Attr_Long_Name	0x0F

#define	KB_Fat_Dir_Long_Entry_Last_Sign	0x40
#define	KB_Fat_Dir_Long_Entry_Index_Mask	0x3F
#define	KB_Fat_Dir_Empty_Entry_Sign	0x00
#define	KB_Fat_Dir_Erase_Entry_Sign	0xE5

#define	KB_Fat_Dir_Attr_Offset	0x0B
#define	KB_Fat_Dir_Checksum_Offset	0x0D

#define	KB_Fat_Dir_Max_Long_Name_Length	255

U8	const	kb_fat_ln_pos[] = 
{
	0x01,	0x03,	0x05,	0x07,	0x09,
	0x0E,	0x10,	0x12,	0x14,	0x16,	0x18,
	0x1C,	0x1E
};

Bool	kb_fat_dir_next(KBFatDir * dir,U16 * long_name,U8 * short_name)
{
	U8 *	ptr;
	U16	index;
	U16	u_iter;
	U16	s_iter;
	U16	u_char;
	U16	iter;
	U16	temp;
	long_name[0] = 0;
	short_name[0] = 0;
	if(dir->entry_index != 0xFFFFFFFF)
	{
		if(!kb_fat_dir_entry_next(dir))
			return	FALSE;
	}
	ptr = kb_fat_dir_get_entry(dir);
	if(!ptr)
		return	FALSE;
	while(ptr[0] == 0xE5)
	{
		if(!kb_fat_dir_entry_next(dir))
			return	FALSE;
		ptr = kb_fat_dir_get_entry(dir);
	}
	if(ptr[0] == 0)
		return	FALSE;
	if(ptr[KB_Fat_Dir_Attr_Offset] == KB_Fat_Dir_Attr_Long_Name)
	{
		u_iter= 0;
		while(ptr[KB_Fat_Dir_Attr_Offset] == KB_Fat_Dir_Attr_Long_Name)
		{
			index = ptr[0] & KB_Fat_Dir_Long_Entry_Index_Mask;
			--index;
			for(iter=0;iter<sizeof(kb_fat_ln_pos);++iter)
			{
				u_char = ptr[kb_fat_ln_pos[iter]];
				u_char += ((U16)ptr[kb_fat_ln_pos[iter]+1])<<8;
				if(u_char == 0xFFFF)
					u_char = 0;
				temp = (U16)(index*sizeof(kb_fat_ln_pos)) + iter;
				long_name[temp] = u_char;
				if(temp>u_iter)
					u_iter = temp;
			}
		//	if(ptr[0]&KB_Fat_Dir_Max_Long_Name_Length)
		//		break;
			if(!kb_fat_dir_entry_next(dir))
				return	FALSE;
			ptr = kb_fat_dir_get_entry(dir);
		}
		if(u_iter)
		{
			long_name[u_iter+1] = 0;
		}
	}
	if(ptr[0] == 0)
		return	FALSE;
	if(ptr[KB_Fat_Dir_Attr_Offset]!=KB_Fat_Dir_Attr_Long_Name)
	{
		s_iter= 0;
		for(iter=0;iter<8;++iter)
		{
			if(isspace(ptr[iter]))
				break;
			short_name[s_iter++] = ptr[iter];
		}
		if(!isspace(ptr[8]))
		{
			short_name[s_iter++] = '.';
			for(iter=8;iter<11;++iter)
			{
				if(isspace(ptr[iter]))
					break;
				short_name[s_iter++] = ptr[iter];
			}
		}
		short_name[s_iter] = 0;
		if(dir->entry_slot_index == 0xFFFFFFFF)
			dir->entry_index = 0;
		else
			dir->entry_index++;
		return	TRUE;
	}
	return	FALSE;
}
Bool	kb_fat_dir_get_attr(KBFatDir * dir,U8 * attr)
{
	U8 * ptr = kb_fat_dir_get_entry(dir);
	if(ptr == 0 )
		return	FALSE;
	*attr = ptr[KB_Fat_Dir_Attr_Offset];
	return	TRUE;
}
Bool	kb_fat_dir_set_attr(KBFatDir * dir,U8 attr)
{
	U8 * ptr = kb_fat_dir_get_entry(dir);
	if(ptr == 0 )
		return	FALSE;
	ptr[KB_Fat_Dir_Attr_Offset] = attr;
	kb_fat_buffer_set_modified(&dir->data_buf);
	return	kb_fat_buffer_flush(&dir->data_buf);
}
U32	kb_fat_dir_get_cluster(KBFatDir * dir)
{
	U32	cluster;
	U8 * ptr = kb_fat_dir_get_entry(dir);
	if(ptr == 0)
		return	0;
	cluster = ptr[26];
	cluster += ((U32)ptr[27])<<8;
	cluster += ((U32)ptr[20])<<16;
	cluster += ((U32)ptr[21])<<24;
	return	cluster;
}
Bool	kb_fat_dir_sub(KBFatDir * dir)
{
	FSINFO * fs_info;
	U32	cluster;
	U8 * ptr = kb_fat_dir_get_entry(dir);
	if(ptr == 0 )
		return	FALSE;
	fs_info = dir->fs_info;
	if(ptr[0]==0||ptr[0]==KB_Fat_Dir_Erase_Entry_Sign)
		return	FALSE;
	if(ptr[KB_Fat_Dir_Attr_Offset]!=KB_Fat_Dir_Attr_Long_Name
		&&(ptr[KB_Fat_Dir_Attr_Offset]&KB_Fat_Dir_Attr_Directory))
	{
		cluster = kb_fat_dir_get_cluster(dir);
		if(kb_fat_check_cluster(fs_info,cluster)!=Fat_Valid_Cluster)
			return	FALSE;
		return	kb_fat_dir_start(dir,fs_info,cluster);
	}
	else
		return	FALSE;
}

Bool	kb_fat_path(KBFatDir * dir,U16 * path,U16 * long_name,U8 * short_name)
{
	FSINFO * fs_info;
	U16 * left,*right;
	U16	old_char;
	U16	sn[14];
	Bool	found = FALSE;
	if(path[0]!='0'||path[1]!=':')
		return	FALSE;
	fs_info = gdrv[0].fsinfo;
	if(!kb_fat_dir_start(dir,fs_info,2))
		return	FALSE;
	left = right = &(path[3]);
	while(*left)
	{
		while(*right!='\\'&&*right!='/'&&*right)
			++right;
		old_char = *right;
		*right = 0;
		found = FALSE;
		while(kb_fat_dir_next(dir,long_name,short_name))
		{
			local_to_unicode(short_name,sn,sizeof(sn));
			if(str_comp_u((U16 *)long_name,(U16 *)left,FALSE)==0||
				str_comp_u((U16 *)sn,(U16 *)left,FALSE)==0)
			{
				if(old_char == '\\'||old_char == '/')
				{
					if(*(right+1))
					{
						if(!kb_fat_dir_sub(dir))
						{
							*right = old_char;
							return	FALSE;
						}
						found = TRUE;
						break;
					}
					else
					{
						if(kb_fat_dir_get_entry(dir)[KB_Fat_Dir_Attr_Offset]&KB_Fat_Dir_Attr_Directory)
						{
							found = TRUE;
							break;
						}
						else
						{
						}
					}
				}
				else
				{
					if(kb_fat_dir_get_entry(dir)[KB_Fat_Dir_Attr_Offset]&KB_Fat_Dir_Attr_Directory)
					{
					}
					else
					{
						found = TRUE;
						break;
					}
				}
			}
		}
		*right = old_char;
		if(found)
		{
			if(*right)
			{
				++right;
				left = right;
			}
			else
				break;
		}
		else
			return	FALSE;
	}
	return	TRUE;
}
