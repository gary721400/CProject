.include "DAC_Flag.inc"
.include "CODEC.inc"

.area PCMv2MuteUnmuteCode2(SPACE_IRAM)
;void DAC_Mute2(void);
_DAC_Mute2::

 SCALL _DAC_Soft_Mute2

 BSET	DAC_StatusMute
 
 RETS




_DAC_Soft_Mute2::
 PUSH R0
 PUSH R1

 PUSH DS1

 MOV DS1, #CODEC_DS1

 MOV R0, #CR1
 MOV R1, [R0]
 JB R1.5, $23                  ; if DAC_MUTE was already 1, do nothing
 BSET R1.5                     ;CR1:DAC_MUTE = 1
 MOV [R0], R1
 
 MOV R0, #IFR
$2:
 MOV R1, [R0]
 JNB R1.0, $2                      ; wait GAIN_DOWN_DONE flag = 1 

 BSET R1.0
 MOV [R0], R1                  ; clear GAIN_DOWN_DONE flag
$23:
 BSET	DAC_StatusMute
 
 POP DS1
 POP R1
 POP R0
 RETS
 

