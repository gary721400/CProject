#include "fc_internal.h"
#if 1
__asm {
.area CheckFileCharacter(FAT_OPEN)
_CheckFileCharacter::
    link 	#0
	PUSH 	R1
	PUSH	R3
	MOVX	R1, ES0
	PUSH	R1
	IGNDS	#1
    MOV		ES0, #^(_fat_ch_table)
    MOV     R3, #(_fat_ch_table)
	MOV     R1,     [FP+#(6)]
	CMP     R1,     #5
	RJMP    IF_NE,  $L001
	MOV     R0,     #0
	RJMP    Lexit
	
$L001:	
	CMP     R1,     #32
	RJMP    IF_UGE, $L002
$L009:	
	MOV     R0,     #-1
	RJMP    Lexit
	
	
$L002:
	
	
	
	MOV		R0, #16
$L004:	
	EX0B	Rb2, [PTR0++]
	CMPB	Rb2, Rb3
	AJMP	IF_EQ, $L009
	CMPD1	R0, #1
	AJMP	IF_UG, $L004
Lexit:
	POP		R1
	MOVX	ES0, R1
	POP		R3
	POP 	R1
	unlink
	RETS
}
#else
int CheckFileCharacter(unsigned int c)
{
    int i;
    if( c == 0x05 ){
        return 0;
    }
    if( c < 0x20 ){
        return -1;
    }
    for( i=0; i<16;++i){
        if( c == fat_ch_table[0] )
            return -1;
    }
    return 0;
}
#endif
