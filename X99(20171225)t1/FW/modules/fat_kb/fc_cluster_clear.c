#include "fc_internal.h"
#if 1
__asm{
.area _ClearCluster(FAT_OPEN)

_ClearCluster::

	LINK 	#4
	PUSH 	R1
	PUSH 	R2			;count = p->fsinfo->sectors_per_cluster(-4)
	PUSH 	R3			;drive(6)
	PUSH 	R4			;&gdrv[drive](-2)
	PUSH 	R5			;p->buffer
	PUSH 	R6
	PUSH 	R7
	PUSH	X0			;sector(-8)
	PUSH	X1			;sector(-8+2)

			
;//    __X struct _drive * p = &gdrv[drive];	
	MOV		R1, #30		
	MOV 	R3, [FP+#(6)]							;R3 = drive
	UMUL 	R1, R3
	MOVX 	R0, _gdrv		
	ADD 	R7, R0	
	MOV 	[FP+#(-2)], R7
	MOV		R4, R7									;R4 = [FP+#(-2)] = &gdrv[drive]
	
;//    unsigned int count = p->fsinfo->sectors_per_cluster;
	IGNDS 	#2
	MOV 	R1, [R4+#28]							;p->fsinfo
	MOV 	R2, [R1+#2]								;p->fsinfo->sectors_per_cluster
	
;//    if( 0 == (sector = FirstSectorInCluster(drive, cluster)) )//{		
	LOAD4$stack 	R0, R1, 8, FP					;cluster
	PUSH 	R1										;cluster
	PUSH 	R0										;cluster
	PUSH 	R3										;R3 = drive				
	SCALL 	_FirstSectorInCluster
	ADD 	SP, #6
	
	MOV		X0, R0									;sector
	MOV		X1, R1
	CMP$long$con 	R0, R1, 0
	RJMP 	IF_NE, fsfcc8
	
;//  return FS_INVALID_CLUSTER;	
	MOV 	R0, #-23		
	RJMP 	#0, fsfcc7
;//}
fsfcc8:
;//    FlushBuffer(drive);
	PUSH 	R3										;R3 = [FP+#(6)]	= drive
	SCALL 	_FlushBuffer
	ADD 	SP, #2

;//    p->buffer_status = EMPTY;
	MOV		R0, #0
	IGNDS	#1	
	MOV 	[R4+#26], R0
	
;//    memset(p->buffer, 0, SECTOR_SIZE);	
	MOV 	R1,	#512								;// SECTOR_SIZE
	PUSH 	R1
	PUSH 	R0	
	PUSH 	R0										;//0
	IGNDS	#1
	MOV 	R5, [R4+#20]; p->buffer
	
	PUSH 	R5
	PUSH 	R0
	SCALL 	_memset
	ADD 	SP,	#10
	RJMP 	#0, fsfcc11
fsfcc10:
;//    while( count-- > 0 )
;//       p->drv_write(sector++, p->buffer);
	PUSH 	R5										;// R5 = [ [FP+#(-2)]+20] = p->buffer
	MOV 	R0, #0	
	PUSH 	R0

	PUSH	X1
	PUSH	X0
	ADD		X1, #1									;sector++
	ADDC	X0, #0	
		
	MOV		R0, R4+#8
	LOAD4$near$indirect 	R0, R1, R0
	MOV 	Y2, #(fsfcc29)
	PUSH 	Y2
	MOV 	Y2, #^(fsfcc29)
	PUSH 	Y2
	PUSH 	R1
	PUSH 	R0
	RETS
fsfcc29:
	ADD 	SP, #8
fsfcc11:
	SUB 	R2, #1									;R2 = count--

   ;//***************
   PUSH 	R3										;R3 = [FP+#(6)]	= drive
	SCALL 	_FlushBuffer
	ADD 	SP, #2
   ;//***************
   
	CMP 	R2, #0	
	RJMP 	IF_NE, fsfcc10
	
;//    return FS_NO_ERROR;
	MOV 	R0, #0
fsfcc7:
	POP		X1
	POP		X0
	POP 	R7
	POP 	R6
	POP 	R5
	POP 	R4
	POP 	R3
	POP 	R2
	POP 	R1
	UNLINK_MACRO
	RETS
	
}
#else
#pragma CODE = FAT_OPEN

int ClearCluster(unsigned int drive, unsigned long cluster)
{
    __X struct _drive * p = &gdrv[drive];
    unsigned int count = p->fsinfo->sectors_per_cluster;
    unsigned long sector;

    if( 0 == (sector = FirstSectorInCluster(drive, cluster)) ){
        return FS_INVALID_CLUSTER;
    }
    FlushBuffer(drive);
    p->buffer_status = EMPTY;
    memset(p->buffer, 0, SECTOR_SIZE);
    while( count-- > 0 )
        p->drv_write(sector++, p->buffer);

    return FS_NO_ERROR;
}
#endif