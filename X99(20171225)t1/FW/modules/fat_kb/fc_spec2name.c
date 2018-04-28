#include "fc_internal.h"

__asm{
.area _SpecToNameExt(FAT_OPEN)
.define _NAME_SIZE      8
.define _EXT_SIZE		3

.define     i       R6
.define     c       R7 
.define     end     R1
.define     count   R5

_SpecToNameExt::
	LINK    #0
	PUSH    R3
	PUSH    R4
	PUSH    count
	PUSH    i
	MOVX    R1,     ES1
	PUSH    R7
	PUSH    R1
	
	;initial
	;; name_ext = R3
    ;; spec     = ES1:R4	    
	MOV     R1,         [FP+#8]
	MOVX    ES1,        R1
	MOV     i,          #0					;i
	MOV     R4,         [FP+#10]
	MOV     R3,         [FP+#6]
	MOVX    DS0,        i
	MOV     R7,         i
	MOV     end,        #46
	MOV     count,      #_NAME_SIZE
	
	EX0B    Rb15,       [PTR1++]      
	CMP     c,          #(0xe5)
	RJMP    IF_NE,      $_floop

	MOVB    Rb1,        #5
    RJMP    $_todst
$_floop:
    
	CMP     c,          #0  			;
	AJMP   IF_EQ,      $_fillspace
	CMP     c,          #92				;'\\'
	AJMP   IF_EQ,      $_fillspace
	CMP     c,          end  			;'.'
	AJMP   IF_EQ,      $_fillspace

	PUSH    c
	SCALL   _CheckFileCharacter
	ADD     SP,         #2
	CMP     R0,         #0
	AJMP    IF_NE,      $_fail_end
    
	CMP     i,          count
	RJMP    IF_UGE,     $_incrloop
	
	
	PUSH    c
	SCALL   _toupper
	ADD     SP,         #2

$_todst:	
	MOVB    [R3++],     Rb1
	ADD     i,          #1
	JNB     R7.7,       $_incrloop
	EX0B    Rb1,        [PTR1++] 
	 
	MOVB    [R3++],     Rb1
    ADD     i,          #1
$_incrloop:	    
    EX0B    Rb15,      [PTR1++]    
    RJMP    $_floop  

$_step2:

    CMP     i,          #(_NAME_SIZE+_EXT_SIZE)
    RJMP    IF_UGE,      $_true_end
	ADD     count,      #(_EXT_SIZE)
	CMP     c,          #46  			;'.'
	RJMP    IF_NE,      $_fillspace
	MOV     end,        #0           
	EX0B    Rb15,      [PTR1++]  
	RJMP    $_floop


$_fail_end:
	MOV     R0,         #0
	MOV     R1,         #0
	RJMP    $_end

$_true_end:
    ;spec
    MOV     R1,         R4
    MOVX    R0,         ES1
	SUB     R1,         #1
	SUBC    R0,         #0

$_end:
    POP     R4
	POP     R7
	POP     i
    MOVX    ES1,        R4
    POP     count
	POP     R4
	POP     R3
	UNLINK
    RETS
$_fillspace:
	CMP     i,          #0
	RJMP    IF_EQ,      $_fail_end
$l001:    	
	CMP     i,          count
	RJMP    IF_UGE,     $_step2
    MOVB    Rb1,        #0x20
    MOVB    [R3++],     Rb1
    ADD     i,          #1
    RJMP    $l001
}
 
