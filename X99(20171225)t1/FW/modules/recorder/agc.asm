.include "CODEC.inc"

.area AGCRAM(AGC_XRAM)

;X_In_Ptr:		.dsw		1							; input buffer, must be in the XRAM
X_Buf_Size:	.dsw		1

Count:			.dsw		1							; frame count
ZCO_TH:			.dsw		1
Zero_Cnt:		.dsw		1

;Min_Zero:   .dsw    1
;Max_Zero:   .dsw    1
;Frame_No_H:   .dsw    1 
;Frame_No_L:   .dsw    1
;Frame_H:      .dsw  1 
;Frame_L:      .dsw  1
Silence_Count: .dsw 1

Power_High:	.dsw		1 
Power_Low:	.dsw		1
;Min_Power_H:  .dsw  1
;Min_Power_L:  .dsw  1
;
;Max_Power_H:  .dsw  1
;Max_Power_L:  .dsw  1 


First:      .dsw    1
Sensitive:	.dsw    1
Shift_bits: .dsw    1
_AGC_Flag::		.dsw		1							; b0 = 1, add 4dB
                                    ; b1 = 1, sub 4dB

.area AGCInit(AGC_Init_Code)

;-----------------------------------------
; void AGC_Init(unsigned int Frame_Size)

_AGC_Init::
	LINK #0
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH DS0
	PUSH SRC
	
	MOV DS0, #0
  
	MOV R0, [FP+#6]
	MOVX X_Buf_Size, R0

	MOV R0, #0
	MOV R1, #Count
	MOV R2, #First
	SUB R2, R1
	SR R2, #1 
	SUB R2, #1
	REP R2
	MOV [R1++], R0

  MOV R0, #0x0004            ; set b2 =1, AGC enable
  MOVX _AGC_Flag, R0
  		
;	MOVX ARL, X_Buf_Size
;	MOV R0, #3
;	UDIV R0
;	MOVX ZCO_TH, ARL										; zero cross threshold for silence

  MOV R0, #36
  MOVX ZCO_TH, R0
  	
	MOV R0, #16
	MOVX Sensitive, R0

	SR R0, #2
	MOVX Silence_Count, R0
		
	MOV R0, #4
	MOVX Shift_bits, R0
			
	POP SRC
	POP DS0
	POP R2
	POP R1
	POP R0
	UNLINK
	RETS

_AGC_Sensitive::
		LINK #0
		PUSH R0
		PUSH R5
		
		MOV R0, [FP+#6]
		EDU R0
		NEGB Rb10
		ADDB Rb10, #15
		MOVSE R0, Rb10
		MOVX Shift_bits, R0 
		
		MOV R5, #1
    SL R5, Rb1
		MOVX Sensitive, R5
		
		SR R5, #2
		CMP R5, #0
		RJMP IF_NE, $55
		MOV R5, #1
$55:
		MOVX Silence_Count, R5
		
		POP R5
		POP R0
		UNLINK
		RETS 



.area AGCCode(AGC_Code)	
;-------------------------------
; void AGC(__X int * In_Buffer)	
;---------------------------------
_AGC::	
	LINK #0
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH X0
	PUSH DS0
	PUSH DS1
	
	PUSH RC
	PUSH RS
	PUSH RE
	
	MOV DS0, #0
	MOV DS1, #0
	MOV X0, [FP+#6]	
	BMOV DS1.0, X0.15
	;BMOV DS0.0, X0.15
	BSET X0.15
;	MOVX X_In_Ptr, X0
	  	
  CLRMAC

	MOV R2, #0
	MOV R4, #0	
	
	MOVX R3, X_Buf_Size
	SUB R3, #1
	
	REPB R3, $0	
	MOV R0, [X0++]						; get input data
	MOV R1, [X0]
	BXOR R1.15, R0.15					; calculate zero crossing
	BMOV R2.0, R1.15
	ADD R4, R2

;  MULS.ss.s R0, R0					; calculate average power
;  ADD DS0:Power_Low, ARL
;  ADDC DS0:Power_High, ARH
  ABS R0
  MOV R1, #0
  ADD DS0:Power_Low, R0
  ADDC DS0:Power_High, R1  

$0:  	
	ADD DS0:Zero_Cnt, R4
	
	MOVX R0, Count 
	ADD R0, #1
	CMP R0, DS0:Sensitive

	AJMP IF_NE, Exit_AGC
;
;	; using the average zero crossibg to check whether it is a silence
;
	MOVX ARL, Zero_Cnt
	UDIV R0
	CMP ARL, DS0:ZCO_TH
	AJMP	IF_UG, Not_Silence
 
  MOV R1, #1
  SUB DS0:Silence_Count, R1
  RJMP IF_NZ, $44 
	SCALL Dec_Gain
	MOVX R1, Sensitive
	SR R1, #2 
	RJMP IF_NZ, $43
	MOV R1, #1
$43:
	MOVX Silence_Count, R1
$44:	
	MOV R0, #0
	MOVX	Zero_Cnt, R0						
  MOVX DS0:Power_High, R0
  MOVX DS0:Power_Low, R0
  AJMP Exit_AGC

  ; adjust gain of ADC according the average power of 16 frame
Not_Silence:  
  MOVX R1, Power_High
  MOVX R2, Power_Low
  MOVX R5, Shift_bits
  SR	R1, Rb11
  SROSB R2, Rb11
;  CMP R1, #0x03
;  AJMP IF_ULE, IS_Silence
  CMP	R1, #0x16
  AJMP IF_UGE, Over_Upper
  CMP R1, #0x0C
  AJMP IF_ULE, Too_Low
	MOV R0, #0										; no need to adjust
	MOVX	Zero_Cnt, R0
  MOVX DS0:Power_High, R0
  MOVX DS0:Power_Low, R0		  
  AJMP Exit_AGC

IS_Silence:  
Over_Upper:
	SCALL Dec_Gain
	MOV R0, #0
	MOVX	Zero_Cnt, R0
  MOVX DS0:Power_High, R0
  MOVX DS0:Power_Low, R0			
	AJMP Exit_AGC

Too_Low:
	SCALL Inc_Gain
	MOV R0, #0
	MOVX	Zero_Cnt, R0
  MOVX DS0:Power_High, R0
  MOVX DS0:Power_Low, R0				
	AJMP Exit_AGC

Exit_AGC:
	MOVX Count, R0
	
	POP RE
	POP RS
	POP RC
	
	POP DS1
	POP DS0
	POP X0
	POP R7
	POP R6
	POP R5
	POP	R4
	POP R3
	POP R2
	POP R1
	POP R0
	UNLINK
	RETS
	
	
Dec_Gain:
	PUSH R1
	PUSH R2
	PUSH DS1
	
	MOVX R1, _AGC_Flag
	BCLR R1.0
	BSET R1.1
	MOVX _AGC_Flag, R1

  MOV DS1, #CODEC_DS1 	
  
  MOV R2, #CGR10
  MOV R1, [R2]
  SR R1, #4
  RJMP IF_Z, Is_0dB
;  SUB R1, #1                   ; decrease 1.5dB at a time  
  SUB R1, #2                   ; decrease 3dB at a time
  CMP R1, #0
  RJMP IF_SGE, $66
  MOV R1, #0
$66:
  SL R1, #4
  MOV [R2], R1
  
Is_0dB:	

  POP DS1
	POP R2
	POP R1
	RETS

Inc_Gain:
	PUSH R1
	PUSH R2
  PUSH DS1
    	
	MOVX R1, _AGC_Flag
	BSET R1.0
	BCLR R1.1
	MOVX _AGC_Flag, R1

  MOV DS1, #CODEC_DS1
  
  MOV R2, #CGR10
  MOV R1, [R2]
  SR R1, #4
  CMP R1, #15
  RJMP IF_EQ, Is_22dB
  ADD R1, #1                 ; add 1.5dB
;  ADD R1, #2                ; add 3dB
  CMP R1, #15
  RJMP IF_SLE, $57
  MOV R1, #15
$57:
  SL R1, #4
  MOV [R2], R1  
Is_22dB:	

  POP DS1
	POP R2
	POP	R1
	RETS