#include "fc_internal.h"
#if 1
__asm{

.area FindFile(FAT_OPEN)
_FindFile::
	LINK 	#7
	PUSH	R1
	PUSH 	R2
	PUSH 	R3			;found
	PUSH	R4			;file

;//    int found=0;
	MOV 	R3, #0
;//    if( (GdrvGet_fsinfo(file->drive))->max_cluster_num < RESERVED_CLUSTER_16_BIT ) //fat 12/16
;//{
	MOV 	R4, [FP+#(6)]
	IGNDS 	#1
	MOV 	R1, [R4+#84]						;file->drive
	PUSH 	R1
	SCALL 	__GdrvGet_fsinfo
	ADD 	SP, #2
	MOV		R0, R1
	ADD		R0, #30     						;(GdrvGet_fsinfo(file->drive))->max_cluster_num
	LOAD4$near$indirect 	R0, R1, R0
	CMP$long$con 	R0, R1, 0xfff6
	MOV 	R1, #0		
	RJMP 	IF_UL, fsffL9	
;//        file->dir_cluster = 0;      // start with root directory
;//}
;//else
;//{
;//        file->dir_cluster = 2;      // start with root directory
	MOV 	R1, #2	
fsffL9:	
	IGNDS 	#2
	MOV 	[R4+#46], R3			;// R3 = 0 
	MOV 	[R4+#46+#2], R1
;//}

;// while(1)
;//	{
fsffL10:
;//  	spec = SpecToNameExt(name_ext, (unsigned char *)spec);
	LOAD4$stack 	R0, R1, 8, FP
	PUSH 	R1
	PUSH 	R0
	MOV 	R2, FP
	ADD 	R2, #(-14)
	PUSH 	R2
	SCALL 	_SpecToNameExt
	ADD 	SP, #6
	SAVE4$stack 	8, FP, R0, R1

;//     if(NULL == spec)
;//		{
	CMP$long$con 	R0, R1, 0
	RJMP 	IF_NE, fsffL13

;//      	if( 0 >= found) 
;//			{
	CMP		R3, #0
	RJMP 	IF_SG, fsffL12
	
;//          	return FS_INVALID_SPEC;
	MOV 	R0, #-5
	RJMP 	#0, fsffL7
;//			}	
;//         return found;
;//		}
	
fsffL13:
;//     if( 0 > (found = FindFileInDirectory(file, name_ext) ) )
;//		{
	PUSH	R2				;FP+#(-14)
	PUSH 	R4				;[FP+#(6)]
	SCALL 	_FindFileInDirectory
	ADD 	SP, #4
	MOV 	R3, R0			;[FP+#(-2)]
	CMP 	R0, #0
	RJMP 	IF_SL, fsffL12	
;//      	return found;
;//		}
	
;//     if('\0' ==  *spec)
;//		{
	LOAD4$stack 	R0, R1, 8, FP
	LOAD1$indirect 	Rb1, R0, R1;   Rb1=[R0,R1]
	MOVSE 	R0, Rb1
	CMP 	R0, #0
	RJMP  	IF_EQ, fsffL12	
;//            return found;
;//		}	
fsffL19:
;//     if( (file->attribute & FS_ATTR_DIRECTORY) == 0 )
;//		{
	IGNDS 	#1
	MOVB 	Rb1, [R4+#18]
	ANDB	Rb1, #16
	CMPB	Rb1, #0
	MOV 	R0, #-4	
	RJMP 	IF_EQ, fsffL7	

;//      	return FS_INVALID_DIRECTORY;
;//		}	
fsffL21:

;//     file->dir_cluster = file->first_cluster;
	LOAD4$near$indirect 	R0, R1, R4+#26
	IGNDS 	#2
	MOV 	[R4+#46], R0
	MOV 	[R4+#46+#2], R1

;//      spec = SkipSlash(spec);
	LOAD4$stack 	R0, R1, 8, FP
	PUSH 	R1
	PUSH 	R0
	SCALL	_SkipSlash
	ADD 	SP, #4
	SAVE4$stack 	8, FP, R0, R1

fsffL11:
;//	}
	RJMP 	#0, fsffL10
fsffL12:
	MOV		R0, R3	
fsffL7:
;// return FS_FILE_NOT_FOUND;

	POP		R4
	POP 	R3
	POP 	R2
	POP 	R1
	UNLINK_MACRO
	RETS
}
#else

/*
int     CheckNotFAT32( int drive )
{
    if( (GdrvGet_fsinfo(drive))->max_cluster_num < RESERVED_CLUSTER_16_BIT )
        return 1;
    return 0;        
}*/

int     FindFile(PxFCFILE file, char *spec)
{
    int found=0;
    unsigned char name_ext[NAME_SIZE+EXT_SIZE];

    if( (GdrvGet_fsinfo(file->drive))->max_cluster_num < RESERVED_CLUSTER_16_BIT ) //fat 12/16
        file->dir_cluster = 0;      // start with root directory
    else
        file->dir_cluster = 2;      // start with root directory

    while(1)
    {
        // Scan name and extension
        spec = SpecToNameExt(name_ext, (unsigned char *)spec);
        if(NULL == spec) {
            if( 0 >= found) {
                return FS_INVALID_SPEC;
            }
            return found;
        }

        // Loop it up in directory
        if( 0 > (found = FindFileInDirectory(file, name_ext) ) )
        {
            return found;
        }

        // If this is the end of the file specification, return
        if('\0' ==  *spec)
            return found;

        // Otherwise, the name and extension were a subdirectory in the path
        if( (file->attribute & FS_ATTR_DIRECTORY) == 0 )
            return FS_INVALID_DIRECTORY;

        file->dir_cluster = file->first_cluster;

        // Skip over the \ after the subdirectory
        spec = SkipSlash(spec);
    }
    return FS_FILE_NOT_FOUND;
}
#endif
