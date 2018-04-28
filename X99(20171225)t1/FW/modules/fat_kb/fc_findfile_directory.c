#include "fc_internal.h"
// ¤j©ó 0 : file found
// others : error, err error table
#if 1
__asm{
.area FindFileInDirectory(FAT_OPEN)
_FindFileInDirectory::

	LINK 	#52
	PUSH 	R1
	PUSH 	R2		;file.dir_index			[FP+#(-38)]  
	PUSH 	R3		;empty_index			[FP+#(-90)]
	PUSH	R4		;fsinfo					[FP+#(-104)]
	PUSH	R5		;d						[FP+#(-2)]
	PUSH	R6		
	PUSH	R7		
	PUSH	X0		;file.dir_cluster		[FP+#(-42)]	           
	PUSH	X1		;file.dir_cluster		[FP+#(-42)+2]                             
	PUSH	X2		;num_of_dir_entries		[FP+#(-92)] 
	PUSH	Y0		;file 					FP+#(-88)
	PUSH	Y1		;x 						[FP+#(-102)]
	PUSH	Y2		;x 						[FP+#(-102)+2]

;//	FCFILE file = *pfile;
	MOV 	R0, FP
	ADD 	R0, #(-88)
	MOV 	Y0, R0
	MOV 	R1, [FP+#(6)]
	MOV$nearWords 	R0, R1, 43	;  43 words at [R1]

;//	PxFSINFO fsinfo = GdrvGet_fsinfo(file.drive);
	MOV 	R0, [FP+#(-4)]						;file.drive	
	PUSH 	R0
	SCALL	 __GdrvGet_fsinfo
	ADD 	SP, #2
	MOV 	R4, R1								;//	R4 = fsinfo(-104)
	
;//	PxDIRECTORY     d;
;//	unsigned long empty_cluster;
;//unsigned int  empty_index = NO_DIRECTORY_INDEX;
	MOV		R3, #0xFFFF							; R3 = empty_index

;//	if( file.dir_cluster == 0 && fsinfo->max_cluster_num < RESERVED_CLUSTER_16_BIT )
;//	{
	LOAD4$stack 	R0, R1, -42, FP
	MOV		X0, R0								;X0,X1 = file.dir_cluster
	MOV		X1, R1
	CMP$long$con 	R0, R1, 0
	RJMP 	IF_NE, fsfff9
	MOV		R0, R4								;// R4 = [FP+#(-104)]
	ADD		R0, #30
	LOAD4$near$indirect 	R0, R1, R0			; R0 = fsinfo->max_cluster_num
	CMP$long$con 	R0, R1, 0xfff6
	RJMP 	IF_UGE, fsfff9

;//		num_of_dir_entries = (unsigned int)(fsinfo->first_dat_sector - fsinfo->first_dir_sector)
;//							 * ENTRIES_PER_SECTOR;

	LOAD4$near$indirect 	R6, R7, R4+#26	;R6,R7 = fsinfo->first_dat_sector
	MOV		R0, R4							;// R4 = [FP+#(-104)]
	ADD		R0, #22	
	LOAD4$near$indirect 	R0, R1, R0		;  fsinfo->first_dir_sector
	SUB 	R7, R1
	SUBC 	R6, R0	
	RJMP 	#0, fsfff9_1	
;//	}
;//	else
;//	{	
fsfff9:
;//		num_of_dir_entries = (unsigned int)fsinfo->sectors_per_cluster * ENTRIES_PER_SECTOR ;
	IGNDS 	#1	
	MOV 	R7, [R4+#2]		
;//}
fsfff9_1:
	SL 		R7, #4									; * ENTRIES_PER_SECTOR
	MOV		X2, R7									; X2 = num_of_dir_entries

;//	file.dir_first_cluster = file.dir_cluster;
	SAVE4$stack 	-36, FP, X0, X1

;//	file.lfn_cluster = INVALID_CLUSTER;
	SCALL	Give_lfn_cluster
	
;//	while(1) 
;//	{
fsffff15:
;//  	for(file.dir_index = 0; file.dir_index < num_of_dir_entries; ++file.dir_index) 
;//		{
	MOV 	R2, #0			;;//file.dir_index = 0;	;// R2 = [FP+#(-38)]
	MOV		[FP+#(-38)], R2
	RJMP 	#0, fsffff21
	
fsffff18:
;//      	RESET_WATCH_DOG();  // Clear Watch Dog
	BCLR 	RTCWDT.14

;//			if( !(d = FindDirectory(&file)) )
;//			{
	PUSH 	Y0					;Y0 = FP+#(-88) = &file
	SCALL 	__FindDirectory
	ADD 	SP, #2
	MOV 	R5, R1				;//R5 = [FP+#(-2)] = d
	CMP 	R1, #0	
	RJMP 	IF_NE, fsffff25
	
;//           	return FS_INVALID_CLUSTER;
	MOV 	R0, #-23
	AJMP 	#0, fsffff7
;//			}	
fsffff25:
;//         if( d == INVALID_ADDRESS )
;//			{
	CMP 	R5, #1				;R5 = [FP+#(-2)]
	AJMP 	IF_EQ, fsffff7_1	
	
;//          	return FS_DRIVE_ERROR;
;//			}	
;//         if( (d->name_ext[0] == DELETED_FILE || d->name_ext[0] == END_DIRECTORY)                
;//				&& empty_index == NO_DIRECTORY_INDEX )
;//			{
	IGNDS 	#1
	MOVB 	Rb13, [R5]							;//d->name_ext[0]
	CMPB 	Rb13, #229							;//d->name_ext[0] == DELETED_FILE
	RJMP 	IF_EQ, fdfffffsfff31
	CMPB 	Rb13, #0							;//d->name_ext[0] == END_DIRECTORY
	RJMP 	IF_NE, fsffff29
fdfffffsfff31:				
	MOV		R1, R3								;//R3 = empty_index(-90)
	MOV 	R0, #0
	CMP$long$con 	R0, R1, 0xffff
	RJMP 	IF_NE, fsffff29
	
;//           	empty_cluster = file.dir_cluster;
	SAVE4$stack 	-98, FP, X0, X1
	
;//             empty_index   = file.dir_index;
	MOV		R3, R2								;//R3 = empty_index(-90), R2 = file.dir_index(-38)

;//             continue;
	RJMP 	#0, fsffff19
;//			}
fsffff29:
;//         if( d->name_ext[0] == END_DIRECTORY )
;//			{
	CMPB 	Rb13, #0							;//d->name_ext[0]	
	RJMP 	IF_EQ, fsffff20	
;//                break;
;//			}	
;//         if( d->name_ext[0] == DELETED_FILE )
;//			{
	CMPB 	Rb13, #229							;//d->name_ext[0]
	RJMP 	IF_EQ, fsffff19	
	
;//                continue;
;//			}	
;//			if( (d->attribute & LFN_ATTR) == LFN_ATTR ) 
;//			{
	IGNDS	 #1
	MOVB 	Rb13, [R5+#11]						;d->attribute
	MOVB	Rb12, Rb13
	ANDB	Rb13, #15
	CMPB 	Rb13, #15	
	RJMP 	IF_NE, fsfff38
	
;//          	if( INVALID_CLUSTER  == file.lfn_cluster )
;//				{
	LOAD4$stack 	R0, R1, -24, FP
	CMP$long$con 	R0, R1, 0xffffffff
	RJMP 	IF_NE, fsffff19

;//             	file.lfn_cluster = file.dir_cluster;
	SAVE4$stack 	-24, FP, X0, X1
	
;//					continue;
	RJMP 	#0, fsffff19
;//				}
;//			}
fsfff38:
;//			if((memcmp(d->name_ext, name_ext, NAME_SIZE+EXT_SIZE) == 0 
;//				&&(d->attribute & FS_ATTR_VOLUME) == 0) )
;//			{
	MOV 	R1, #11
	MOV 	R0, #0
	PUSH 	R1
	PUSH 	R0	
	MOV 	R1, [FP+#(8)]		
	PUSH 	R1
	PUSH 	R0		
	PUSH 	R5										;d->name_ext		
	PUSH 	R0	
	SCALL 	_memcmp
	ADD 	SP, #12
	CMP 	R0, #0
	RJMP 	IF_NE, fsfff45
						
	ANDB 	Rb12, #8								;//Rb12 = [R5+#11] ;d->attribute
	CMPB 	Rb12, #0		
	RJMP 	IF_NE, fsfff45

;//          	if( ReadDirectoryEntry(&file) )
;//				{
	PUSH 	Y0										;Y0 = FP+#(-88)
	SCALL 	_ReadDirectoryEntry
	ADD 	SP, #2
	CMP 	R0, #0
	RJMP 	IF_NE, fsffff7_1
	
;//		         	return -1;
;//				}
;//             err = FS_NO_ERROR;
	MOV R6,#0
;//             goto _Lend;
	RJMP 	#0, fsfff49
;//			}	
fsfff45:
;//			file.lfn_cluster = INVALID_CLUSTER;
	SCALL	Give_lfn_cluster
fsffff19:
	ADD 	R2, #1				;//++file.dir_index
	MOV 	[FP+#(-38)], R2		;//R2 = [FP+#(-38)]	
fsffff21:						;//file.dir_index < num_of_dir_entries; 		
	CMP 	R2, X2				;//R2 = file.dir_index(-38)	 , X2 = num_of_dir_entries(-92)
	RJMP 	IF_UL, fsffff18
;//		}	
fsffff20:
;//     if( file.dir_index < num_of_dir_entries || 0 == file.dir_cluster)
;//		{
	CMP 	R2, X2				;//R2 = file.dir_index(-38)	 , X2 = num_of_dir_entries(-92)	
	RJMP 	IF_UL, fsffff17		
	MOV		R0, X0				;X0,X1 = file.dir_cluster
	MOV		R1, X1
	CMP$long$con 	R0, R1, 0
	RJMP 	IF_EQ, fsffff17
;//            break;
;//		}	
;//     if(INVALID_CLUSTER == (x = GetFATEntry(file.drive, file.dir_cluster))) 
;//		{
	PUSH 	X1										;file.dir_cluster
	PUSH 	X0
	MOV 	R0, [FP+#(-4)]							;file.drive
	PUSH 	R0
	SCALL 	_GetFATEntry
	ADD 	SP, #6

	MOV		Y1, R0
	MOV		Y2, R1
	CMP$long$con 	R0, R1, 0xffffffff
	RJMP 	IF_EQ, fsffff7_1	
;//      	return -1;
;//		}	
;//     if( x >= fsinfo->last_cluster_mark )
;//		{
	MOV		R0, R4											;R4 = [FP+#(-104)]
	ADD		R0, #34
	LOAD4$near$indirect 	R0, R1, R0						;fsinfo->last_cluster_mark
	
	CMP$long	R0, R1, Y1, Y2
	RJMP	IF_ULE, fsffff17				
	
;//      	break;
;//		}	
;//     file.dir_cluster = x;
	MOV		X0, Y1
	MOV		X1, Y2
	SAVE4$stack 	-42, FP, X0, X1
	AJMP 	#0, fsffff15
;//	}	
fsffff17:
;//	file.dir_index = empty_index;
	MOV 	[FP+#(-38)], R3						;//R3 = empty_index(-90)
	MOV		R2, R3
;// if(NO_DIRECTORY_INDEX != file.dir_index) 
;//	{
	MOV		R1, R2								;R2 = file.dir_index
	MOV 	R0, #0
	CMP$long$con 	R0, R1, 0xffff
	RJMP 	IF_EQ, fsfff64
	
;// 	file.dir_cluster = empty_cluster;
	LOAD4$stack 	X0, X1, -98, FP
	SAVE4$stack 	-42, FP, X0, X1
;//	}
fsfff64:
;//	NameExtToSpec(file.spec, name_ext);
	MOV 	R0, [FP+#(8)]
	PUSH 	R0
	MOV 	R0, FP
	ADD 	R0, #(-84)
	PUSH 	R0
	SCALL 	_NameExtToSpec
	ADD 	SP, #4
	
;// err = FS_FILE_NOT_FOUND;
	MOV 	R6, #-7	
fsfff49:
;//_Lend:
;// *pfile = file;
	MOV 	R0, [FP+#(6)]
	MOV 	R1, Y0									;Y0 = FP+#(-88)
	MOV$nearWords 	R0, R1, 43;  43 words at [R1]   
	
;//	return err; 
	MOV		R0, R6
;//	}
fsffff7:
	POP		Y2
	POP		Y1
	POP		Y0
	POP		X2
	POP		X1
	POP		X0
	POP		R7
	POP		R6
	POP		R5
	POP		R4
	POP 	R3
	POP 	R2
	POP 	R1
	UNLINK_MACRO
	RETS
fsffff7_1:
	MOV		R0, #-1
	RJMP	#0, fsffff7	

Give_lfn_cluster:
;//	file.lfn_cluster = INVALID_CLUSTER;
	MOV 	R0, #(0xffffffff >> 16)
	MOV 	R1, #(0xffffffff & 0xFFFF)
	SAVE4$stack 	-24, FP, R0, R1
	RETS
}
#else
int     FindFileInDirectory(PxFCFILE pfile, __X unsigned char *name_ext)
{
    FCFILE file = *pfile;
    PxFSINFO fsinfo = GdrvGet_fsinfo(file.drive);
    PxDIRECTORY     d;
    unsigned long empty_cluster;
	unsigned int  empty_index = NO_DIRECTORY_INDEX;
    unsigned long x;
    unsigned int num_of_dir_entries;
    FS_ERR err;
    

	if( file.dir_cluster == 0 && fsinfo->max_cluster_num < RESERVED_CLUSTER_16_BIT ){
		num_of_dir_entries = (unsigned int)(fsinfo->first_dat_sector - fsinfo->first_dir_sector) * ENTRIES_PER_SECTOR;
	}
	else{
		num_of_dir_entries = (unsigned int)fsinfo->sectors_per_cluster * ENTRIES_PER_SECTOR ;
	}

    file.dir_first_cluster = file.dir_cluster;
    file.lfn_cluster = INVALID_CLUSTER;

    while(1) {
        for(file.dir_index = 0; file.dir_index < num_of_dir_entries; ++file.dir_index) {
            RESET_WATCH_DOG();  // Clear Watch Dog

            if( !(d = FindDirectory(&file)) )
                return FS_INVALID_CLUSTER;
            if( d == INVALID_ADDRESS )
                return FS_DRIVE_ERROR;

            if(    (d->name_ext[0] == DELETED_FILE || d->name_ext[0] == END_DIRECTORY)
                && empty_index == NO_DIRECTORY_INDEX )
            {
                empty_cluster = file.dir_cluster;
                empty_index   = file.dir_index;
                continue;
            }

            if( d->name_ext[0] == END_DIRECTORY )
                break;
            if( d->name_ext[0] == DELETED_FILE )
                continue;
			//long file name entry
			if( (d->attribute & LFN_ATTR) == LFN_ATTR ) {
                if( INVALID_CLUSTER  == file.lfn_cluster )
                    file.lfn_cluster = file.dir_cluster;
				continue;
			}

            if( //name_ext == NULL && d->attribute & FS_ATTR_VOLUME  ||
                //name_ext != NULL &&
                (memcmp(d->name_ext, name_ext, NAME_SIZE+EXT_SIZE) == 0 &&
                (d->attribute & FS_ATTR_VOLUME) == 0) )
            {
                if( ReadDirectoryEntry(&file) )
                    return -1;
                err = FS_NO_ERROR;
                goto _Lend;
            }
            file.lfn_cluster = INVALID_CLUSTER;
        }

        if( file.dir_index < num_of_dir_entries || 0 == file.dir_cluster)
            break;

        if(INVALID_CLUSTER == (x = GetFATEntry(file.drive, file.dir_cluster))) {
            return -1;
        }

        if( x >= fsinfo->last_cluster_mark )
            break;

        file.dir_cluster = x;
    }

    file.dir_index = empty_index;

    if(NO_DIRECTORY_INDEX != file.dir_index) {
        file.dir_cluster = empty_cluster;
    }

    {
        NameExtToSpec(file.spec, name_ext);
    }
    
    err = FS_FILE_NOT_FOUND;
_Lend:
    *pfile = file;
    return err;    
}
#endif
