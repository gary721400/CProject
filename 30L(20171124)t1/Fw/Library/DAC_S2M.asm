.include "CODEC.inc"

.area PCMv2S2MCode(DAC_COMMON_CODE)
;void DAC_Stereo_to_Mono(unsigned Option);  // Option:  0 -> disable,  1 --> enable
_DAC_Stereo_to_Mono::
 LINK #0
 PUSH R0
 PUSH R1
 PUSH R2
 PUSH DS1

 MOV DS1, #CODEC_DS1

 MOV R0, #CR1
 MOV R1, [R0]
 
 MOV R2, [FP+#6]
 BMOV R1.6, R2.0
 MOV [R0], R1 
 
 POP DS1
 POP R2
 POP R1
 POP R0
 UNLINK 
 RETS

