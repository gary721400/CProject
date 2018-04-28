#include "fc_internal.h"
#if 1
__asm{
.area UpdateDirectoryEntry(FAT_OPEN,REL)
_UpdateDirectoryEntry::
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
    ;7
	MOV     R2,     [SP+#(4+8)]
	
	PUSH    R2
	SCALL   __FindDirectory
	ADD     SP,     #2
	MOV 	R3, R1
    ;15
	ADD     R2,     #4
	MOV     R0,     [SP+#(4+10)]
	SCALL   DOTDOTK

    ;18
    ADD     R2,     #14
	IGNDS   #2
	MOVB    Rb1,    [R2] ;#18
	MOVB    [R3+#11],Rb1
	
    ;27
	MOV     R0,     #0
	ADD     R3,     #12
	IGNDS   #3
	MOV     [R3],   R0
	MOV     [R3+#6],R0
	
    ;29
	MOV     R0,     [R2+#66]
	PUSH    R0
	SCALL   _SetDirty
	ADD     SP,     #2
	
    ;23
	ADD     R2,     #8
	ADD     R3,     #8
	ACALL   Lcallcnv2 ;R1,20->22

    ;20
	SUB     R2,     #6
	ACALL   Lcallcnv4 ;R1,22->24->26
	
    ;22
	ADD     R2,     #8
	ACALL   Lcallcnv2 ;R1,26->28
	
    ;25
	ADD     R2,     #14
    ACALL   Lcallcnv4 ;R1,28->30->32
    
    ;end
    POP R3
	POP R2
	POP R1
	POP R0
	RETS

Lcallcnv2:
	PUSH    R2
	SCALL   _BYTES_TO_WORD
	ADD     SP,     #2
	IGNDS   #1
	MOV     [R3++],R0
    RET	
Lcallcnv4:
	PUSH    R2
	SCALL   _BYTES_TO_DWORD
	ADD     SP,     #2
	IGNDS   #2
	MOV     [R3++],R0
	MOV     [R3++],R1
    RET	
DOTDOTK:    
}
#ifdef DOTDOT	
__asm{
    CMP     R0,     #0
    RJMP    IF_EQ,  $1
    MOV     R0,     #(NAME_SIZE + EXT_SIZE)
    PUSH    R0
    MOV     R0,     #0
    PUSH    R0
	PUSH    R2
	PUSH    R0
	PUSH    R3
	PUSH    R0
	SCALL   _memcpy
	ADD     SP,     #12
	RETS
$1:
    MOV     R0,     #0
	PUSH    R2
	PUSH    R0
	PUSH    R3
	SCALL   _SpecToNameExt
	ADD     SP,     #6
	RETS	
}	
#else
__asm{
    MOV     R0,     #0
	PUSH    R2
	PUSH    R0
	PUSH    R3
	SCALL   _SpecToNameExt
	ADD     SP,     #6
	RETS
}
#endif
    

#else
#if 1

void    UpdateDirectoryEntry(PxFCFILE file)
{
    PxDIRECTORY d = FindDirectory(file);

#ifdef DOTDOT //called by fc_updatdotdot.c
    //if('.' == file->spec[0]) {
        memcpy(d->name_ext, file->spec, NAME_SIZE + EXT_SIZE);
#else        
    //} else {
        SpecToNameExt(d->name_ext, file->spec);
    //}
#endif
    d->attribute = file->attribute;

    *(__X unsigned long*)&(d->date_n_time) = BYTES_TO_DWORD(((__X unsigned char *)&(file->date_n_time))  );

    d->first_cluster = BYTES_TO_WORD(((__X unsigned char *)&(file->first_cluster))+2);
	d->first_cluster_high = BYTES_TO_WORD((__X unsigned char *)&(file->first_cluster));

    d->size = BYTES_TO_DWORD((__X unsigned char *)&file->size);

    d->reserved = d->reserved1 = 0;

    SetDirty(file->drive);
}
#endif
#endif
