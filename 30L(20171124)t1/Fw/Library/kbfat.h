#ifndef _KBFAT_H_
#define _KBFAT_H_

#include "KBDM9_DDK.h"
//#include "SYSTEM.H"
#include <string.h>
//#include "Example_USB_MAssStorage.h"
#include "compiler.h"

//#include "ADC.H"
//#include "SDImg.h"
//#include "config.h"
//#include "conf_kb.h"
#include <fs.h>
#include "fc_internal.h"


//typedef unsigned char       U8 ;


//typedef	struct	_KBFatBuffer
typedef struct
{
	U8	disk_id;
	U32	sector_addr;
	U8	is_modified;
//	FatU8	buffer[Fat_Sector_Size];
	U8 *	buffer;
}KBFatBuffer;

//typedef	struct	_KBFatDir
typedef struct
{
	FSINFO *	fs_info;
	KBFatBuffer	data_buf;
	U32	start_cluster;
	U32	cur_cluster;
	U32	entry_slot_index;
	U32	entry_index;
	
}KBFatDir;

Bool kb_fat_path(KBFatDir * dir,U16 * path,U16 * long_name,U8 * short_name);
Bool kb_fat_dir_set_attr(KBFatDir * dir,U8 attr);
Bool kb_fat_dir_get_attr(KBFatDir * dir,U8 * attr);
void show_subdir(void);
void hid_subdir(void);

#endif