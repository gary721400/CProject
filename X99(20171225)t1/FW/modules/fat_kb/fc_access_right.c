#include "fc_internal.h"
#if 1
__asm {

.area CheckWriteAccess(FAT_OPEN)
_CheckWriteAccess::	
	MOV 	R0,[SP+#(4)]
	IGNDS 	#1
	MOVB 	Rb1,[R0+#18]
	BOR 	R0.2, R0.0
	JB		R0.2, $0
	MOV		R0, #0
	RJMP	$1
$0:	
	MOV     R0, #-1
$1:	
	RETS
}	
#else
int     CheckWriteAccess(PxFCFILE file)
{
    if( file->attribute & (FS_ATTR_READONLY + FS_ATTR_SYSTEM) ) {
        return -1;
    }

    return 0;
}
#endif