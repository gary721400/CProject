.include "CODEC.inc"
.include "DAC_Flag.inc"

.area PCMv2_InitCode(DAC_TEMP_CODE00)




;**************************************************************************************
_DAC_SetupISR::
	LINK	#0
	PUSH	R0
	PUSH	R1
	PUSH	R2
	
	MOV		R0 , #0x4039
	MOV		R1 , #^_DAC_ISR_L
	OR		R1 , #0xAD00
	MOV		R2 , #_DAC_ISR_L
	CRIT #3
	MOVX	IRAMA , R0		                      
	MOVX	IRAMDH , R1		                     
	MOVX	IRAMDL , R2		       
	
	MOV		R0 , #0x4034
	MOV		R1 , #^_DAC_ISR_R
	OR		R1 , #0xAD00
	MOV		R2 , #_DAC_ISR_R
	CRIT #3
	MOVX	IRAMA , R0		                            
	MOVX	IRAMDH , R1		                     
	MOVX	IRAMDL , R2		        
	
	POP		R2
	POP		R1
	POP		R0
	UNLINK
	RETS


_DAC_CallBackFunction2Check::
	MOVX	R0 , _DAC_FillBufferFunc2
	IGNDS	#1
	ADD		R0 , _DAC_FillBufferFunc2+2
	BMOVN	DAC_CallBack2_Enable , PSR.0
	RETS


_DAC_Reg_Init::
 PUSH R0
 PUSH R1
; PUSH DS0
 PUSH DS1

; MOV DS0, #0
 MOV DS1, #CODEC_DS1

 MOV R0, #AICR
 MOV R1, #0
 MOV [R0++], R1                      ; AICR = 0x00
 
; MOV R1, [R0]                        ; CR1
; AND R1, #0x0084                     ; SB_MICBIAS, BYPASS, keeps unchanged
; OR R1, #0x0022                      ; MONO = 0, DAC_MUTE = 1, HP_DIS = 0, DACSEL = 0, BTL_MUTE = 1
; MOV [R0++], R1
 
 MOV R0, #CR2
 MOV R1, [R0]                        ; CR2
 OR R1, #0x0078                      ; ADC_HPF, INSEL keep unchanged
 MOV [R0++], R1
 
 MOV R1, #0                          ;CCR1 = 0x00  
 MOV [R0++], R1
 
 MOV R1, [R0]                        ; CCR2
 AND R1, #0x000F                     ; DFREQ[3:0] = 0
 MOV [R0++], R1
 
; MOV R1, [R0]                        ; PMR1
; OR R1, #0x00E7                      ; SB_ADC, SB_LIN keep unchanged
; MOV [R0++], R1
 
 MOV R0, #PMR2
 MOV R1, [R0]                        ; PMR2
 OR R1, #0x0030                      ; LEGOD:RLGOD = 11b , 左右聲道的gain 都由 GODL 控制, 
 MOV [R0++], R1
 
 MOV R0, #ICR                        ; ICR = 0x3F
 MOV R1, #0x3F
 MOV [R0++], R1 

 MOV R1, #0x3F
 MOV [R0++], R1                      ; clear IFR 
 
 MOV R1, #0xAA                       ; -15dB
 MOV [R0++], R1                      ; CGR1 = 0x00
 
 MOV R1, #0xC4                       ; CGR2 = 0xC4, LRGOB:RLGOB = 11b, GOBL[4:0]=0x04 (0dB)
 MOV [R0++], R1
 
 MOV R1, #0x04                       ; CGR3 = 0x04
 MOV [R0], R1            
 
 MOV R0, #CGR8                       ; CGR8 = 0xCA
 MOV R1, #0xD7                       ; LRGO:RLGO = 11b, GOL[4:0] = 0x17 (-17.5dB)
 MOV [R0++], R1
 
 MOV R1, #0x0A                       ; CGR9 
 MOV [R0++], R1                      ; GOR[4:0] = 0x0A (-1.5dB) 

; MOV DACLD, #0
; MOV DACRD, #0
   
 POP DS1
; POP DS0
 POP R1
 POP R0
 RETS
 
;-------------------------------------------------------------------------------------
; turn on the DAC from power-down-mode or sleep mode 

_DAC_Power_On::
 PUSH R0
 PUSH R1
 PUSH R2
 PUSH DS1
 
 MOV DS1, #CODEC_DS1
 
 MOV R0, #PMR2
 MOV R1, [R0]  
 JNB R1.1, not_power_down             
; in power down mode
 BCLR R1.1                        ; PMR2:SB = 0 
 MOV [R0], R1
 
;wait the time Tsbyu = 500ms
 MOV R2, #500
wait_Tsbyu:
 ACALL DAC_Delay_1ms  
 CMPD1 R2, #0
 RJMP IF_NN, wait_Tsbyu 
 
not_power_down:                   ; PMR2:SB = 0, not in power down mode
 JNB R1.0, not_sleep_mode
 BCLR R1.0                        ; clear PMR2:SD_SLEEP
 MOV [R0], R1
;wait the time Tshd_dac = 200ms ??;
 
 MOV R2, #200
wait_Tshd:
 ACALL DAC_Delay_1ms
 CMPD1 R2, #0   
 RJMP IF_NN, wait_Tshd
 
not_sleep_mode:
; enable DAC
 MOV R0, #PMR1
 MOV R1, [R0]
 BCLR R1.7                        ; SB_DAC = 0
 MOV [R0], R1
;
 MOV R2, #10
wait_DAC:
 ACALL DAC_Delay_1ms
 CMPD1 R2, #0   
 RJMP IF_NN, wait_DAC 
 
 MOV R0, #CR1
 MOV R1, [R0]
 BSET R1.3                        ; DACSEL = 1
 MOV [R0], R1
   
 MOV R0, #PMR1
 MOV R1, [R0] 
 BCLR R1.5                        ; SB_MIX = 0
 MOV [R0], R1
   
 POP DS1
 POP R2
 POP R1
 POP R0
 RETS 