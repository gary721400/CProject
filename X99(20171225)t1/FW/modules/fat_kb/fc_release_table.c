#include "fc_internal.h"

#if 1
__asm{
.area ReleaseFATEntries(FAT_OPEN)
_ReleaseFATEntries::
	LINK #0
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH r4
	PUSH R5
	PUSH r6
	PUSH R7
    ;
	MOV     R5,     [FP+#(6)]
	IGNDS   #1
	MOV     R4,     [R5+#84]
    ;10
	PUSH    R4
	SCALL   __GdrvGet_fsinfo
	ADD     SP,     #2
	IGNDS   #4
	MOV     R6,     [R1+#34]
	MOV     R7,     [R1+#36]
    ;12
	MOV     R2,     [R5+#(26)]
	MOV     R3,     [R5+#(28)]
	MOV     R0,     R2
	MOV     R1,     R3
$step1:
    ;14
	CMP     R2,     #0
	RJMP    IF_NE,  $step2
	CMP     R3,     #0    
	RJMP    IF_NE,  $step2
	RJMP    $end
$step2:
    ;15
	CMP$long R0,R1,R6,R7
	RJMP    IF_UGE, $end
    
	PUSH    R3
	PUSH    R2
	PUSH    R4
	SCALL   _GetFATEntry
	ADD     SP,     #6
    ;18
	MOV     R5,     #0
	PUSH    R5
	PUSH    R5
	PUSH    R3
	PUSH    R2
	PUSH    R4
	SCALL   _SetFATEntry
	ADD     SP,     #10
    ;24
    MOV     R3,     R1
    MOV     R2,     R0
    RJMP    $step1
    
$end:
	MOV     R5,     [FP+#(6)]
	IGNDS   #3
	MOVB    Rb3,    [R5+#19]
	OR      R1,     #128
	MOVB    [R5+#19],Rb3
	;
	POP R7
	POP R6
    pop R5
    pop R4
	POP R3
	POP R2
	POP R1
	POP R0
	UNLINK_MACRO
	RETS
}
#else
void    ReleaseFATEntries(PxFCFILE file)
{
    unsigned long j, k;
    unsigned int drive = file->drive;
    unsigned long last_mark = (GdrvGet_fsinfo(drive))->last_cluster_mark;
    
    j = file->first_cluster;

    while( j != EMPTY_CLUSTER && j < last_mark ) {
        k = GetFATEntry(drive, j);
        SetFATEntry(drive, j, EMPTY_CLUSTER);
        j = k;
    }
    file->mode |= WRITTEN;
}
#endif