;--------------------------------------------------------------------------------------------------
;#pragma DATA = FILESEARCH_DATA
;#pragma CODE = FILESEARCH_CODE
;--------------------------------------------------------------------------------------------------
.area _FILESEARCH_CODE_(FILESEARCH_CODE,REL)
;--------------------------------------------------------------------------------------------------
_Mmemachar::
	LINK	#0
	PUSH	R1
	PUSH	R2
	PUSH	X0
	PUSH   DS0
	PUSH   DS1

	MOV	DS0,	#0
	MOV	DS1,	#1
	MOV	R0,	#0
	MOV	R2,	[FP+#(8)]
	MOV	X0,	[FP+#(6)]
	MOV	R1,	[FP+#(10)]
$loop:	
    CMPB	rb3,	[X0++]
	RJMP	IF_EQ,	$exit
	IGNDS   #1
	CMPI1	R0,	0xF2E4			; _R2 0xF2E4
	RJMP	IF_SGE,	$exit
	RJMP	$loop

$exit:
    POP DS1
    POP DS0	
    POP	X0
	POP	R2
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mmemschar::
	LINK	#0
	PUSH	R1
	PUSH	X0
	PUSH    DS0
	PUSH    DS1

	MOV	    DS0,	#0
	MOV	    DS1,	#1
	MOV	    R0,	[FP+#(8)]
	SUB	    R0,	#1
	RJMP	IF_SL,	$exit
	MOV	    X0,	[FP+#(6)]
	ADD	    X0,	R0
	MOV	    R1,	[FP+#(10)]
$loop:	
    CMPB	rb3,	[X0]
	RJMP	IF_EQ,	$exit
	SUB	    X0,	#1
	CMPD1	R0,	#0
	RJMP	IF_SL,	$exit
	RJMP	$loop

$exit:
	POP DS1
	POP DS0
    POP	X0
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mstrlen::
	LINK	#0
	PUSH	R1
	PUSH	R3

	MOV	   R0,	#0
	MOV	   R3,	[FP+#(6)]
	MOVX	ES0,	R3			; ES0 + NOP + EX0
	MOV	   R3,	[FP+#(8)]
$loop:
    EX0B	rb2,	[PTR0++]
	CMPB	rb2,	#0
	RJMP	IF_EQ,	$exit
	CMPI1	R0,	#512
	RJMP	IF_UGE,	$exit
	RJMP	$loop

$exit:
	POP	R3
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mstrcpy::
	LINK	#0
	PUSH	R1
	PUSH	R3
	PUSH	R4

	MOV	R0,	#0
	MOV	R3,	[FP+#(6)]
	MOV	R4,	[FP+#(10)]
	MOVX	ES0,	R3			; ES0 + NOP + EX0
	MOVX	ES1,	R4			; ES1 + NOP + EX0
	MOV	R3,	[FP+#(8)]
	MOV	R4,	[FP+#(12)]
$loop:	
    EX0B	rb2,	[PTR1++]
	EX0B	[PTR0++],rb2
	CMPB	rb2,	#0
	RJMP	IF_EQ,	$exit
	CMPI1	R0,	#512
	RJMP	IF_UGE,	$exit
	RJMP	$loop

$exit:	
    POP	R4
	POP	R3
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mstrwcpy::
	LINK	#0
	PUSH	R1
	PUSH	R3
	PUSH	R4

	MOV	R0,	#0
	MOV	R3,	[FP+#(6)]
	MOV	R4,	[FP+#(10)]
	MOVX	ES0,	R3			; ES0 + NOP + EX0
	MOVX	ES1,	R4			; ES1 + NOP + EX0
	MOV	R3,	[FP+#(8)]
	MOV	R4,	[FP+#(12)]
$loop:	EX0B	rb2,	[PTR1++]
	EX0B	[PTR0++],rb2
	EX0B	rb3,	[PTR1++]
	EX0B	[PTR0++],rb3
	CMP	R1,	#0
	RJMP	IF_EQ,	$exit
	CMPI2	R0,	#512
	RJMP	IF_UGE,	$exit
	RJMP	$loop

$exit:	POP	R4
	POP	R3
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mmemcpy::
	LINK	#0
	PUSH	R3
	PUSH	R4

	MOV	R0,	[FP+#(14)]
	SUB	R0,	#1
	RJMP	IF_SL,	$exit
	MOV	R3,	[FP+#(6)]
	MOV	R4,	[FP+#(10)]
	MOVX	ES0,	R3			; ES0 + NOP + EX0
	MOVX	ES1,	R4			; ES1 + NOP + EX0
	MOV	R3,	[FP+#(8)]
	MOV	R4,	[FP+#(12)]
	REPB R0,$loop
	EX0B	[PTR0++],[PTR1++]
	NOP
$loop:
	ADD	R0,	#1
$exit:	POP	R4
	POP	R3
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_Mmemcmp::
	LINK	#0
	PUSH	R1
	PUSH	R2
	PUSH	R3
	PUSH	R4

	MOV	R0,	#0
	MOV	R2,	[FP+#(14)]
	CMP	R2,	#0
	RJMP	IF_EQ,	$exit
	MOV	R3,	[FP+#(6)]
	MOV	R4,	[FP+#(10)]
	MOVX	ES0,	R3			; ES0 + NOP + EX0
	MOVX	ES1,	R4			; ES1 + NOP + EX0
	MOV	R3,	[FP+#(8)]
	MOV	R4,	[FP+#(12)]
$loop:	EX0B	rb2,	[PTR0++]
	EX0B	rb3,	[PTR1++]
	CMPB	rb2,	rb3
	RJMP	IF_NE,	$exit
	CMPD1	R2,	#1
	RJMP	IF_UG,	$loop
	MOV	R0,	#1

$exit:	POP	R4
	POP	R3
	POP	R2
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
;--------------------------------------------------------------------------------------------------
_MstrExt3::
	LINK	#0
	PUSH	R1
	PUSH	X0
	PUSH   DS0
	PUSH   DS1

	MOV	DS0,	#0
	MOV	DS1,	#1
	MOV	X0,	#_cExt3
	MOVB	rb2,	[X0]
	CMPB	rb2,	#0
	MOV	R0,	#0
	RJMP	IF_EQ,	$exit

	MOV	R1,	#0
	MOV	X0,	[FP+#(6)]
$lend:	CMPB	rb0,	[X0++]
	RJMP	IF_EQ,	$okend
	CMPI1	R1,	#512
	RJMP	IF_UGE,	$err
	RJMP	$lend

$okend:	MOV	R1,	#0
	MOVB	rb4,	#'.
$loop:	SUB	X0,	#1
	CMPB	rb4,	[X0]
	RJMP	IF_EQ,	$ok
	CMPI1	R1,	#3		; .xx
	RJMP	IF_UG,	$err
	RJMP	$loop

$ok:	MOVB	rb2,	[X0++]
	MOVB	rb3,	[X0++]
	CMPB	rb0,	[X0]
	RJMP	IF_EQ,	$fix1
	MOVB	rb4,	[X0++]
	CMPB	rb0,	[X0]
	RJMP	IF_EQ,	$fix2
	MOVB	rb5,	[X0++]
	RJMP	$fix3
$fix1:	MOVB	rb4,	rb0
$fix2:	MOVB	rb5,	rb0
$fix3:	OR	R1,	#0x2020
	OR	R2,	#0x2020

	PUSH	R3
	PUSH	R4
	MOV	X0,	#_cExt3
$cmpe:	MOVB	rb6,	[X0++]
	CMPB	rb6,	#'.
	RJMP	IF_NE,	$chk
	MOVB	rb7,	[X0++]
	MOVB	rb8,	[X0++]
	MOVB	rb9,	[X0++]
	SUB	R3,	R1
	SUBC	R4,	R2
	RJMP	IF_EQ,	$get
	ADD	R0,	#1
	RJMP	$cmpe
$get:	POP	R4
	POP	R3

$exit:
    POP DS1
    POP DS0	
    POP	X0
	POP	R1
	UNLINK
	;CRIT	#1
	RETS
$next:	AND	R0,	#0xF0
	ADD	R0,	#0x10
	RJMP	$cmpe
$chk:	CMPB	rb6,	#',
	RJMP	IF_EQ,	$next
	CMPB	rb6,	#'*
	RJMP	IF_EQ,	$get
	MOV	R0,	#-1
	RJMP	$get
$err:	MOV	R0,	#-1
	RJMP	$exit
