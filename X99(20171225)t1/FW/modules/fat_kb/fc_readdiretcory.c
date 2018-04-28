#include "fc_internal.h"

#if 1
__asm{
	
.area ReadDirectoryEntry(FAT_OPEN)

_LoadW:
	ADD 	R1, R3
	PUSH 	R1
	SCALL 	_BYTES_TO_WORD
	ADD 	SP,#2
	IGNDS 	#1
	MOV 	[R2++],R0
	RET
_LoadDW:
	ADD		R1, R3
	PUSH 	R1
	SCALL 	_BYTES_TO_DWORD
	ADD 	SP,#2
	IGNDS 	#2
	MOV 	[R2++],R0
	MOV 	[R2],R1	
	RET
_ReadDirectoryEntry::

	LINK 	#3
	PUSH 	R1
	PUSH 	R2
	PUSH 	R3
	
;	PxDIRECTORY d = FindDirectory(file);
	MOV 	R2,[FP+#(6)]	;R2 = file
	PUSH 	R2
	SCALL 	__FindDirectory
	ADD 	SP,#2
	MOV		R3, R1
	MOV 	[FP+#(-2)],R3	;R3 = d

;    if( !d )
;        return FS_INVALID_CLUSTER;
	CMP 	R3,#0
	RJMP 	IF_NE,fsrdd8
	MOV 	R0,#-23
	RJMP 	#0,fsrdd7

;    if( INVALID_ADDRESS == d )
;        return FS_DRIVE_ERROR;	
fsrdd8:
	CMP 	R3,#1
	RJMP 	IF_NE,fsrdd10
	MOV 	R0,#-1
	RJMP 	#0,fsrdd7
	
;	file->attribute = d->attribute;	
fsrdd10:
	IGNDS 	#2
	MOVB 	Rb1,[R3+#11]
	MOVB 	[R2+#18],Rb1

;	*(__X unsigned long *)(&(file->date_n_time)) = BYTES_TO_DWORD((__X unsigned char *)&d->date_n_time);

	ADD 	R2, #20
	MOV		R1, #22
	ACALL  _LoadDW

;	file->size = BYTES_TO_DWORD((__X unsigned char *)&d->size);
	ADD		R2, #42-(20+2)
	MOV		R1, #28
	ACALL  _LoadDW
		
;	*((__X unsigned int*)(&(file->first_cluster)))  = BYTES_TO_WORD((__X unsigned char *)&d->first_cluster_high);	

	ADD 	R2, #26-(42+2)
	MOV		R1, #20
	ACALL 	_LoadW
	
;	*((__X unsigned int*)(&(file->first_cluster))+1)= BYTES_TO_WORD((__X unsigned char *)&d->first_cluster);
	MOV		R1, #26
	ACALL 	_LoadW


;	NameExtToSpec(file->spec, d->name_ext);
	PUSH	R3
	MOV 	R2, [FP+#(6)]	;R2 = file
	ADD 	R2, #4
	PUSH 	R2
	SCALL 	_NameExtToSpec
	ADD 	SP,#4
	MOV 	R0,#0
fsrdd7:
	POP 	R3
	POP 	R2
	POP 	R1
	UNLINK
	RETS	
}	
#else
#ifdef RDDIR1
int     ReadDirectoryEntry(PxFCFILE file)
#else
int     ReadDirectoryEntry(PxFCFILE file, __X char *lfn)
#endif
{
    PxDIRECTORY d = FindDirectory(file);
    unsigned int date, time;

    if( !d )
        return FS_INVALID_CLUSTER;
    if( INVALID_ADDRESS == d )
        return FS_DRIVE_ERROR;

    file->attribute = d->attribute;

    *(__X unsigned long *)(&(file->date_n_time)) = BYTES_TO_DWORD((__X unsigned char *)&d->date_n_time);

    *((__X unsigned int*)(&(file->first_cluster)))  = BYTES_TO_WORD((__X unsigned char *)&d->first_cluster_high);
    *((__X unsigned int*)(&(file->first_cluster))+1)= BYTES_TO_WORD((__X unsigned char *)&d->first_cluster);

    file->size = BYTES_TO_DWORD((__X unsigned char *)&d->size);

    {
        NameExtToSpec(file->spec, d->name_ext);
#ifndef RDDIR1        
        {
            if(INVALID_CLUSTER != file->lfn_cluster) {
    			RetrieveLFN(file, lfn);
    		} else {
                (__X unsigned int*)lfn[0] = 0;
            }
        }
#endif        
    }

    return 0;
}
#endif