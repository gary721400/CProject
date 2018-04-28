+                       1  
+                       5  
+                       6  .area _$SYSTEM_C$(ROM,REL) 
+B 00 01
000000                  7  Ltext0: 
+                      41  .include "kbccmacro.asm" 
000000                 42   .define _sdtl_mount	 _sd1_mount        
000000                 43   .define _sdtl_resv_size	 _sd1_res_size     
000000                 44   .define _sdtl_get_size	 _sd1_fat_size     
000000                 45   .define _sdtl_resv_read	 _sd1_res_read     
000000                 46   .define _sdtl_resv_write	 _sd1_res_write    
000000                 47   .define _sdtl_read	 _sd1_read         
000000                 48   .define _sdtl_write	 _sd1_write        
000000                 49   .define _sdtl_cache_init	 _sd1_cache2k_init 
000000                 50   .define _sdtl_cache_read	 _sd1_cache_read   
000000                 51   .define _sdtl_cache_write _sd1_cache_write  
000000                 52   .define _sdtl_cache_flush _sd1_cache_flush  
000000                 53   .define _sdtl_allocate    _sd1_allocate     
000000                 54   .define _spif_res_size	 _spif_resv_size   
000000                 55   .define _spif_res_read	 _spif_resv_read   
000000                 56   .define _spif_res_write	 _spif_resv_write  
+                      57  
+                      58  .area _SYSTEM_CODE$RD$VolTbl(SYSTEM_CODE,REL) 
+                      59  .even 
+B 00 02
000000                 60  _VolTbl:: 
000000  00 07          61  	.dw 7  
000002  00 0D          62  	.dw 13  
000004  00 14          63  	.dw 20  
000006  00 1C          64  	.dw 28  
000008  00 1F          65  	.dw 31  
+                      68  
+                      69  .area _SYSTEM_CODE$RD$SpeakerTbl(SYSTEM_CODE,REL) 
+                      70  .even 
+B 00 03
000000                 71  _SpeakerTbl:: 
000000  00 04          72  	.dw 4  
000002  00 05          73  	.dw 5  
000004  00 06          74  	.dw 6  
000006  00 08          75  	.dw 8  
000008  00 0A          76  	.dw 10  
+                      78  
+                      79  .area _$system_reset(SYSTEM_CODE,REL) 
+B 00 04
000000                 81  _system_reset:: 
000000  2E 00          83  	LINK #0 
000002  58 F0          84  	PUSH R0 
000004  58 F1          85  	PUSH R1 
000006                 87  L49: 
000006                 89  L50: 
000006                 91  L51: 
000006  68 F1 00 22    92  	MOV R1,#(L45) 
00000A  68 F0 00 22    93  	MOV R0,#^(L45) 
00000E  58 F1          94  	PUSH R1 
000010  58 F0          95  	PUSH R0 
000012                 96  .if KERNEL1_5 = 1
+                      97  	CRIT #1
+                      98  .endif
000012  BD 00 00 00    99  	SCALL _SendString 
000016  50 F4         100  	ADD SP,#4 
000018                102  L52: 
+                     103  	
000018  F6 57         104  	BCLR 0x57.6 
00001A                106  L53: 
+                     107  	
00001A  E7 57         108  	BSET 0x57.7 
00001C                110  L54: 
+                     111  	
00001C  F5 57         112  	BCLR 0x57.5 
00001E                114  L55: 
00001E  68 F0 BF FF   115  	MOV R0,#0xbfff 
000022  AA F0 00 00   116  	MOVX _RTCWDT,R0 
000026                118  L56: 
000026                119  L46: 
000026                121  L57: 
000026                123  L58: 
000026                125  L59: 
000026                127  L60: 
000026                128  L47: 
000026                130  L61: 
000026  AC 00 00 26   131  	AJMP #0,L46 
00002A                133  L62: 
00002A                135  L63: 
00002A  38 1F         136  	POP R1 
00002C  38 0F         137  	POP R0 
00002E                138  	UNLINK_MACRO
00002E  2F 00         138  UNLINK 
+                     138  
000030                139  .if KERNEL1_5 = 1
+                     140  	CRIT #1
+                     141  .endif
000030  C6 20         142  	RETS 
+                     144  
+                     145  .area _$Exception(SYSTEM_CODE,REL) 
+B 00 05
000000                146  _Exception:: 
000000  2E 00         148  	LINK #0 
000002                149  	SAVE$ISR
000002  58 F0         149  PUSH R0 
000004  1F 09         149  PUSH RC 
000006  1F 0A         149  PUSH  SRC 
000008  68 09 00 00   149  MOV RC, #0 
00000C  68 0A 00 00   149  MOV SRC, #0 
000010  1F 01         149  PUSH  PSR 
000012  1F 00         149  PUSH  SOCR 
000014  1F 0C         149  PUSH  RE 
000016  1F 0B         149  PUSH  RS 
+                     149                   
000018  1F 13         149  PUSH XM0 
00001A  1F 19         149  PUSH YM0 
00001C  1F 02         149  PUSH SB 
00001E  1F 10         149  PUSH DS0 
000020  1F 11         149  PUSH DS1 
000022  58 FE         149  PUSH Y2 
000024  58 F8         149  PUSH X0 
000026  68 13 00 00   149  MOV  XM0, #0 
00002A  68 19 00 00   149  MOV  YM0, #0 
00002E  58 F2         149  PUSH R2 
000030  58 F3         149  PUSH R3 
000032  BA F0 F2 DC   149  MOVX  R0,ES0 
000036  58 F0         149  PUSH  R0 
000038  BA F0 F2 DE   149  MOVX  R0,ES1 
00003C  58 F0         149  PUSH  R0 
00003E  BD 00 00 00   149  SCALL _ClrCirqAndAdjustCpri 
+                     149          
+                     149  
000042  58 F0         150  	PUSH R0 
000044                152  L65: 
000044                154  L66: 
000044                156  L67: 
000044                157  .if KERNEL1_5 = 1
+                     158  	CRIT #1
+                     159  .endif
000044  BD 00 00 00   160  	SCALL _system_reset 
000048                162  L68: 
000048                164  L69: 
000048                165  L64: 
000048  38 0F         166  	POP R0 
00004A                167  	RESTORE$ISR
00004A  38 0F         167  POP   R0 
00004C  BA EF F2 E0   167  MOVX  ES1,R0 
000050  38 0F         167  POP   R0 
000052  BA EE F2 E0   167  MOVX  ES0,R0 
000056  38 3F         167  POP  R3 
000058  38 2F         167  POP  R2 
00005A  38 8F         167  POP  X0 
00005C  38 EF         167  POP Y2 
00005E  1F 91         167  POP DS1 
000060  1F 90         167  POP DS0 
000062  1F 82         167  POP SB 
000064  1F 99         167  POP YM0 
000066  1F 93         167  POP XM0 
+                     167          
000068  1F 8B         167  POP   RS 
00006A  1F 8C         167  POP   RE 
00006C  1F 80         167  POP   SOCR 
+                     167     
00006E  1F 81         167  POP   PSR 
000070  1F 8A         167  POP   SRC 
000072  1F 89         167  POP RC 
000074  38 0F         167  POP R0 
+                     167  
000076                168  	UNLINK_MACRO
000076  2F 00         168  UNLINK 
+                     168  
000078                169  .if KERNEL1_5 = 1
+                     170  	CRIT #1
+                     171  .endif
000078  C6 10         172  	RETI 
+                     174  
+                     175  .area _$timer_get_time(SYSTEM_CODE,REL) 
+B 00 06
000000                176  _timer_get_time:: 
000000  2E 02         178  	LINK #2 
000002                180  L71: 
000002                182  L72: 
000002                185  L73: 
000002                186  	LOAD4$memory R0
000002  68 10 00 00   186  MOV DS0,#@(_g_timer_tick) 
000006  88 F0 00 00   186  MOV R0,DS0:(_g_timer_tick) 
00000A  88 F1 00 02   186  MOV R1,DS0:(_g_timer_tick+2) 
+                     186  
00000E                187  	SAVE4$stack -4
00000E  6A 0B FF FC   187  MOV [FP+#(-4)],R0 
000012  6A 1B FF FE   187  MOV [FP+#(-4+2)],R1 
+                     187  
000016                189  L74: 
000016                190  	LOAD4$stack R0
000016  78 0B FF FC   190  MOV R0,[FP+#(-4)] 
00001A  78 1B FF FE   190  MOV R1,[FP+#(-4+2)] 
+                     190  
00001E                192  L75: 
00001E                194  L76: 
00001E                195  L70: 
00001E                196  	UNLINK_MACRO
00001E  2F 00         196  UNLINK 
+                     196  
000020                197  .if KERNEL1_5 = 1
+                     198  	CRIT #1
+                     199  .endif
000020  C6 20         200  	RETS 
+                     202  
+                     203  .area _$timer_check_timeout(SYSTEM_CODE,REL) 
+B 00 07
000000                204  _timer_check_timeout:: 
000000  2E 00         207  	LINK #0 
000002  58 F1         208  	PUSH R1 
000004  58 F2         209  	PUSH R2 
000006  58 F3         210  	PUSH R3 
000008                212  L80: 
000008                214  L81: 
000008                216  L82: 
000008                217  .if KERNEL1_5 = 1
+                     218  	CRIT #1
+                     219  .endif
000008  BD 00 00 00   220  	SCALL _timer_get_time 
00000C                221  	LOAD4$stack R2
00000C  78 2B 00 06   221  MOV R2,[FP+#(6)] 
000010  78 3B 00 08   221  MOV R3,[FP+#(6+2)] 
+                     221  
000014                222  	CMP$long R0
000014  12 13         222  SUB R1,R3 
000016  13 02         222  SUBC R0,R2 
+                     222  
000018  AC 05 00 22   223  	AJMP IF_UL,L78 
00001C                225  L83: 
00001C                227  L84: 
00001C                229  L85: 
00001C  48 01         230  	MOV R0,#1 
00001E  AC 00 00 24   231  	AJMP #0,L77 
000022                233  L86: 
000022                235  L87: 
000022                236  L78: 
000022                238  L88: 
000022                240  L89: 
000022                242  L90: 
000022  48 00         243  	MOV R0,#0 
000024                245  L91: 
000024                247  L92: 
000024                249  L93: 
000024                251  L94: 
000024                252  L77: 
000024  38 3F         253  	POP R3 
000026  38 2F         254  	POP R2 
000028  38 1F         255  	POP R1 
00002A                256  	UNLINK_MACRO
00002A  2F 00         256  UNLINK 
+                     256  
00002C                257  .if KERNEL1_5 = 1
+                     258  	CRIT #1
+                     259  .endif
00002C  C6 20         260  	RETS 
+                     262  
+                     263  .area _$TIMER0_ISR(SYSTEM_CODE,REL) 
+B 00 08
000000                264  _TIMER0_ISR:: 
000000  2E 00         266  	LINK #0 
000002                267  	SAVE$ISR
000002  58 F0         267  PUSH R0 
000004  1F 09         267  PUSH RC 
000006  1F 0A         267  PUSH  SRC 
000008  68 09 00 00   267  MOV RC, #0 
00000C  68 0A 00 00   267  MOV SRC, #0 
000010  1F 01         267  PUSH  PSR 
000012  1F 00         267  PUSH  SOCR 
000014  1F 0C         267  PUSH  RE 
000016  1F 0B         267  PUSH  RS 
+                     267                   
000018  1F 13         267  PUSH XM0 
00001A  1F 19         267  PUSH YM0 
00001C  1F 02         267  PUSH SB 
00001E  1F 10         267  PUSH DS0 
000020  1F 11         267  PUSH DS1 
000022  58 FE         267  PUSH Y2 
000024  58 F8         267  PUSH X0 
000026  68 13 00 00   267  MOV  XM0, #0 
00002A  68 19 00 00   267  MOV  YM0, #0 
00002E  58 F2         267  PUSH R2 
000030  58 F3         267  PUSH R3 
000032  BA F0 F2 DC   267  MOVX  R0,ES0 
000036  58 F0         267  PUSH  R0 
000038  BA F0 F2 DE   267  MOVX  R0,ES1 
00003C  58 F0         267  PUSH  R0 
00003E  BD 00 00 00   267  SCALL _ClrCirqAndAdjustCpri 
+                     267          
+                     267  
000042  58 F0         268  	PUSH R0 
000044  58 F1         269  	PUSH R1 
000046                271  L96: 
000046                273  L97: 
000046                275  L98: 
000046  48 11         276  	MOV R1,#1 
000048  48 00         277  	MOV R0,#0 
00004A  68 10 00 00   278  	MOV DS0,#@(_g_timer_tick) 
00004E  B0 F1 00 02   279  	ADD DS0:(_g_timer_tick+2),R1 
000052  B1 F0 00 00   280  	ADDC DS0:(_g_timer_tick),R0 
000056                282  L99: 
000056                284  L100: 
000056                285  L95: 
000056  38 1F         286  	POP R1 
000058  38 0F         287  	POP R0 
00005A                288  	RESTORE$ISR
00005A  38 0F         288  POP   R0 
00005C  BA EF F2 E0   288  MOVX  ES1,R0 
000060  38 0F         288  POP   R0 
000062  BA EE F2 E0   288  MOVX  ES0,R0 
000066  38 3F         288  POP  R3 
000068  38 2F         288  POP  R2 
00006A  38 8F         288  POP  X0 
00006C  38 EF         288  POP Y2 
00006E  1F 91         288  POP DS1 
000070  1F 90         288  POP DS0 
000072  1F 82         288  POP SB 
000074  1F 99         288  POP YM0 
000076  1F 93         288  POP XM0 
+                     288          
000078  1F 8B         288  POP   RS 
00007A  1F 8C         288  POP   RE 
00007C  1F 80         288  POP   SOCR 
+                     288     
00007E  1F 81         288  POP   PSR 
000080  1F 8A         288  POP   SRC 
000082  1F 89         288  POP RC 
000084  38 0F         288  POP R0 
+                     288  
000086                289  	UNLINK_MACRO
000086  2F 00         289  UNLINK 
+                     289  
000088                290  .if KERNEL1_5 = 1
+                     291  	CRIT #1
+                     292  .endif
000088  C6 10         293  	RETI 
+                     295  
+                     296  .area _$adc_get(SYSTEM_CODE,REL) 
+B 00 09
000000                297  _adc_get:: 
000000  2E 01         299  	LINK #1 
000002  58 F0         300  	PUSH R0 
000004  58 F1         301  	PUSH R1 
000006                303  L112: 
000006                305  L113: 
000006                308  L114: 
000006                309  	LOAD4$memory R0
000006  68 10 00 00   309  MOV DS0,#@(_adc_timer) 
00000A  88 F0 00 00   309  MOV R0,DS0:(_adc_timer) 
00000E  88 F1 00 02   309  MOV R1,DS0:(_adc_timer+2) 
+                     309  
000012  58 F1         310  	PUSH R1 
000014  58 F0         311  	PUSH R0 
000016                312  .if KERNEL1_5 = 1
+                     313  	CRIT #1
+                     314  .endif
000016  BD 00 00 00   315  	SCALL _timer_check_timeout 
00001A  50 F4         316  	ADD SP,#4 
00001C  19 01         317  	MOVZE R0,Rb1 
00001E  56 00         318  	CMP R0,#0 
000020  AC 02 00 28   319  	AJMP IF_NE,L102 
000024                321  L115: 
000024                323  L116: 
000024                325  L117: 
000024  AC 00 00 FE   326  	AJMP #0,L101 
000028                328  L118: 
000028                330  L119: 
000028                331  L102: 
000028                333  L120: 
000028                334  .if KERNEL1_5 = 1
+                     335  	CRIT #1
+                     336  .endif
000028  BD 00 00 00   337  	SCALL _timer_get_time 
00002C  50 1A         338  	ADD R1,#10 
00002E  51 00         339  	ADDC R0,#0 
000030  50 11         340  	ADD R1,#1 
000032  51 00         341  	ADDC R0,#0 
000034                342  	SAVE4$memory _adc_timer
000034  68 10 00 00   342  MOV DS0,#@(_adc_timer) 
000038                342  .if KERNEL1_E = 1
+                     342  NOP
+                     342  .endif
000038  A8 F0 00 00   342  MOV DS0:(_adc_timer),R0 
00003C  A8 F1 00 02   342  MOV DS0:(_adc_timer+2),R1 
+                     342  
000040                344  L121: 
000040  68 10 00 00   345  	MOV DS0,#@_adc_channel 
000044  99 F0 00 00   346  	MOVZE R0,DS0:_adc_channel 
000048  58 F0         347  	PUSH R0 
00004A                348  .if KERNEL1_5 = 1
+                     349  	CRIT #1
+                     350  .endif
00004A  BD 00 00 00   351  	SCALL _Get_ADCValue 
00004E  50 F2         352  	ADD SP,#2 
000050  6A 0B FF FE   353  	MOV [FP+#(-2)],R0 
000054                355  L122: 
000054  68 10 00 00   356  	MOV DS0,#@_adc_channel 
000058  99 F0 00 00   357  	MOVZE R0,DS0:_adc_channel 
00005C  56 00         358  	CMP R0,#0 
00005E  AC 02 00 7C   359  	AJMP IF_NE,L104 
000062                361  L123: 
000062                363  L124: 
000062                365  L125: 
000062  78 0B FF FE   366  	MOV R0,[FP+#(-2)] 
000066  68 10 00 00   367  	MOV DS0,#@_adc0_value 
00006A                368  .if KERNEL1_E = 1
+                     369  	NOP
+                     370  .endif
00006A  A8 F0 00 00   371  	MOV DS0:_adc0_value,R0 
00006E                373  L126: 
00006E  49 12         374  	MOVB Rb1,#2 
000070  68 10 00 00   375  	MOV DS0,#@_adc_channel 
000074                376  .if KERNEL1_E = 1
+                     377  	NOP
+                     378  .endif
000074  A9 F1 00 00   379  	MOVB DS0:_adc_channel,Rb1 
000078                381  L127: 
000078  AC 00 00 FE   382  	AJMP #0,L101 
00007C                384  L128: 
00007C                386  L129: 
00007C                387  L104: 
00007C                389  L130: 
00007C  68 10 00 00   390  	MOV DS0,#@_adc_channel 
000080  99 F0 00 00   391  	MOVZE R0,DS0:_adc_channel 
000084  56 02         392  	CMP R0,#2 
000086  AC 02 00 A4   393  	AJMP IF_NE,L106 
00008A                395  L131: 
00008A                397  L132: 
00008A                399  L133: 
00008A  78 0B FF FE   400  	MOV R0,[FP+#(-2)] 
00008E  68 10 00 00   401  	MOV DS0,#@_adc2_value 
000092                402  .if KERNEL1_E = 1
+                     403  	NOP
+                     404  .endif
000092  A8 F0 00 00   405  	MOV DS0:_adc2_value,R0 
000096                407  L134: 
000096  49 13         408  	MOVB Rb1,#3 
000098  68 10 00 00   409  	MOV DS0,#@_adc_channel 
00009C                410  .if KERNEL1_E = 1
+                     411  	NOP
+                     412  .endif
00009C  A9 F1 00 00   413  	MOVB DS0:_adc_channel,Rb1 
0000A0                415  L135: 
0000A0  AC 00 00 FE   416  	AJMP #0,L101 
0000A4                418  L136: 
0000A4                420  L137: 
0000A4                421  L106: 
0000A4                423  L138: 
0000A4  68 10 00 00   424  	MOV DS0,#@_adc_channel 
0000A8  99 F0 00 00   425  	MOVZE R0,DS0:_adc_channel 
0000AC  56 03         426  	CMP R0,#3 
0000AE  AC 02 00 CC   427  	AJMP IF_NE,L108 
0000B2                429  L139: 
0000B2                431  L140: 
0000B2                433  L141: 
0000B2  78 0B FF FE   434  	MOV R0,[FP+#(-2)] 
0000B6  68 10 00 00   435  	MOV DS0,#@_adc3_value 
0000BA                436  .if KERNEL1_E = 1
+                     437  	NOP
+                     438  .endif
0000BA  A8 F0 00 00   439  	MOV DS0:_adc3_value,R0 
0000BE                441  L142: 
0000BE  49 17         442  	MOVB Rb1,#7 
0000C0  68 10 00 00   443  	MOV DS0,#@_adc_channel 
0000C4                444  .if KERNEL1_E = 1
+                     445  	NOP
+                     446  .endif
0000C4  A9 F1 00 00   447  	MOVB DS0:_adc_channel,Rb1 
0000C8                449  L143: 
0000C8  AC 00 00 FE   450  	AJMP #0,L101 
0000CC                452  L144: 
0000CC                454  L145: 
0000CC                455  L108: 
0000CC                457  L146: 
0000CC  68 10 00 00   458  	MOV DS0,#@_adc_channel 
0000D0  99 F0 00 00   459  	MOVZE R0,DS0:_adc_channel 
0000D4  56 07         460  	CMP R0,#7 
0000D6  AC 02 00 F4   461  	AJMP IF_NE,L110 
0000DA                463  L147: 
0000DA                465  L148: 
0000DA                467  L149: 
0000DA  78 0B FF FE   468  	MOV R0,[FP+#(-2)] 
0000DE  68 10 00 00   469  	MOV DS0,#@_adc7_value 
0000E2                470  .if KERNEL1_E = 1
+                     471  	NOP
+                     472  .endif
0000E2  A8 F0 00 00   473  	MOV DS0:_adc7_value,R0 
0000E6                475  L150: 
0000E6  49 10         476  	MOVB Rb1,#0 
0000E8  68 10 00 00   477  	MOV DS0,#@_adc_channel 
0000EC                478  .if KERNEL1_E = 1
+                     479  	NOP
+                     480  .endif
0000EC  A9 F1 00 00   481  	MOVB DS0:_adc_channel,Rb1 
0000F0                483  L151: 
0000F0  AC 00 00 FE   484  	AJMP #0,L101 
0000F4                486  L152: 
0000F4                488  L153: 
0000F4                489  L110: 
0000F4                491  L154: 
0000F4  49 17         492  	MOVB Rb1,#7 
0000F6  68 10 00 00   493  	MOV DS0,#@_adc_channel 
0000FA                494  .if KERNEL1_E = 1
+                     495  	NOP
+                     496  .endif
0000FA  A9 F1 00 00   497  	MOVB DS0:_adc_channel,Rb1 
0000FE                499  L155: 
0000FE                501  L156: 
0000FE                502  L101: 
0000FE  38 1F         503  	POP R1 
000100  38 0F         504  	POP R0 
000102                505  	UNLINK_MACRO
000102  2F 00         505  UNLINK 
+                     505  
000104                506  .if KERNEL1_5 = 1
+                     507  	CRIT #1
+                     508  .endif
000104  C6 20         509  	RETS 
+                     511  
+                     512  .area _$op_time_dowith(SYSTEM_CODE,REL) 
+B 00 0A
000000                513  _op_time_dowith:: 
000000  2E 02         515  	LINK #2 
000002  58 F0         516  	PUSH R0 
000004  58 F1         517  	PUSH R1 
000006                519  L160: 
000006                521  L161: 
000006                523  L162: 
000006  BA F0 00 00   524  	MOVX R0,_op_close_timer 
00000A  BA F1 00 02   525  	MOVX R1,_op_close_timer+2 
00000E                526  	SAVE4$stack -4
00000E  6A 0B FF FC   526  MOV [FP+#(-4)],R0 
000012  6A 1B FF FE   526  MOV [FP+#(-4+2)],R1 
+                     526  
000016                527  	CMP$long$con R0
000016  52 10         527  SUB R1,#(0 & 0xFFFF) 
000018  53 00         527  SUBC R0,#(0 >> 16) 
+                     527  
00001A  AC 01 00 46   528  	AJMP IF_EQ,L158 
00001E                529  	LOAD4$stack R0
00001E  78 0B FF FC   529  MOV R0,[FP+#(-4)] 
000022  78 1B FF FE   529  MOV R1,[FP+#(-4+2)] 
+                     529  
000026  58 F1         530  	PUSH R1 
000028  58 F0         531  	PUSH R0 
00002A                532  .if KERNEL1_5 = 1
+                     533  	CRIT #1
+                     534  .endif
00002A  BD 00 00 00   535  	SCALL _timer_check_timeout 
00002E  50 F4         536  	ADD SP,#4 
000030  19 01         537  	MOVZE R0,Rb1 
000032  56 00         538  	CMP R0,#0 
000034  AC 01 00 46   539  	AJMP IF_EQ,L158 
000038                541  L163: 
000038                543  L164: 
000038                545  L165: 
000038  48 10         546  	MOV R1,#0 
00003A  48 00         547  	MOV R0,#0 
00003C  AA F0 00 00   548  	MOVX _op_close_timer,R0 
000040  AA F1 00 02   549  	MOVX _op_close_timer+2,R1 
000044                551  L166: 
+                     552  	
000044  F2 4E         553  	BCLR 0x4e.2 
000046                555  L167: 
000046                557  L168: 
000046                558  L158: 
000046                560  L169: 
000046                562  L170: 
000046                563  L157: 
000046  38 1F         564  	POP R1 
000048  38 0F         565  	POP R0 
00004A                566  	UNLINK_MACRO
00004A  2F 00         566  UNLINK 
+                     566  
00004C                567  .if KERNEL1_5 = 1
+                     568  	CRIT #1
+                     569  .endif
00004C  C6 20         570  	RETS 
+                     572  
+                     573  .area _$clk_time_dowith(SYSTEM_CODE,REL) 
+B 00 0B
000000                574  _clk_time_dowith:: 
000000  2E 02         576  	LINK #2 
000002  58 F0         577  	PUSH R0 
000004  58 F1         578  	PUSH R1 
000006                580  L174: 
000006                582  L175: 
000006                584  L176: 
000006                585  	LOAD4$memory R0
000006  68 10 00 00   585  MOV DS0,#@(_g_system_clk_timer) 
00000A  88 F0 00 00   585  MOV R0,DS0:(_g_system_clk_timer) 
00000E  88 F1 00 02   585  MOV R1,DS0:(_g_system_clk_timer+2) 
+                     585  
000012                586  	SAVE4$stack -4
000012  6A 0B FF FC   586  MOV [FP+#(-4)],R0 
000016  6A 1B FF FE   586  MOV [FP+#(-4+2)],R1 
+                     586  
00001A                587  	CMP$long$con R0
00001A  52 10         587  SUB R1,#(0 & 0xFFFF) 
00001C  53 00         587  SUBC R0,#(0 >> 16) 
+                     587  
00001E  AC 01 00 50   588  	AJMP IF_EQ,L172 
000022                589  	LOAD4$stack R0
000022  78 0B FF FC   589  MOV R0,[FP+#(-4)] 
000026  78 1B FF FE   589  MOV R1,[FP+#(-4+2)] 
+                     589  
00002A  58 F1         590  	PUSH R1 
00002C  58 F0         591  	PUSH R0 
00002E                592  .if KERNEL1_5 = 1
+                     593  	CRIT #1
+                     594  .endif
00002E  BD 00 00 00   595  	SCALL _timer_check_timeout 
000032  50 F4         596  	ADD SP,#4 
000034  19 01         597  	MOVZE R0,Rb1 
000036  56 00         598  	CMP R0,#0 
000038  AC 01 00 50   599  	AJMP IF_EQ,L172 
00003C                601  L177: 
00003C                603  L178: 
00003C                605  L179: 
00003C  48 10         606  	MOV R1,#0 
00003E  48 00         607  	MOV R0,#0 
000040                608  	SAVE4$memory _g_system_clk_timer
000040  68 10 00 00   608  MOV DS0,#@(_g_system_clk_timer) 
000044                608  .if KERNEL1_E = 1
+                     608  NOP
+                     608  .endif
000044  A8 F0 00 00   608  MOV DS0:(_g_system_clk_timer),R0 
000048  A8 F1 00 02   608  MOV DS0:(_g_system_clk_timer+2),R1 
+                     608  
00004C                610  L180: 
00004C                611  .if KERNEL1_5 = 1
+                     612  	CRIT #1
+                     613  .endif
00004C  BD 00 00 00   614  	SCALL _SysClkDowngrade 
000050                616  L181: 
000050                618  L182: 
000050                619  L172: 
000050                621  L183: 
000050                623  L184: 
000050                624  L171: 
000050  38 1F         625  	POP R1 
000052  38 0F         626  	POP R0 
000054                627  	UNLINK_MACRO
000054  2F 00         627  UNLINK 
+                     627  
000056                628  .if KERNEL1_5 = 1
+                     629  	CRIT #1
+                     630  .endif
000056  C6 20         631  	RETS 
+                     633  
+                     634  .area _$vol_dec(SYSTEM_CODE,REL) 
+B 00 0C
000000                635  _vol_dec:: 
000000  2E 00         637  	LINK #0 
000002                639  L186: 
000002                641  L187: 
000002                643  L188: 
000002                645  L189: 
000002                647  L190: 
000002                648  L185: 
000002                649  	UNLINK_MACRO
000002  2F 00         649  UNLINK 
+                     649  
000004                650  .if KERNEL1_5 = 1
+                     651  	CRIT #1
+                     652  .endif
000004  C6 20         653  	RETS 
+                     655  
+                     656  .area _$vol_inc(SYSTEM_CODE,REL) 
+B 00 0D
000000                657  _vol_inc:: 
000000  2E 00         659  	LINK #0 
000002                661  L192: 
000002                663  L193: 
000002                665  L194: 
000002                667  L195: 
000002                669  L196: 
000002                670  L191: 
000002                671  	UNLINK_MACRO
000002  2F 00         671  UNLINK 
+                     671  
000004                672  .if KERNEL1_5 = 1
+                     673  	CRIT #1
+                     674  .endif
000004  C6 20         675  	RETS 
+                     677  
+                     678  .area _$judge_op(SYSTEM_CODE,REL) 
+B 00 0E
000000                679  _judge_op:: 
000000  2E 00         681  	LINK #0 
000002  58 F0         682  	PUSH R0 
000004  58 F1         683  	PUSH R1 
000006                685  L200: 
000006                687  L201: 
000006                689  L202: 
+                     690  	
000006  7C 59 F0 40   691  	BMOV R0.0,0x59.4 
00000A  54 01         692  	AND R0,#1 
00000C  68 10 00 00   693  	MOV DS0,#@(_gflag) 
000010  88 F1 00 00   694  	MOV R1,DS0:(_gflag) 
000014  4D 16         695  	SR R1,#6 
000016  54 11         696  	AND R1,#1 
000018  16 01         697  	CMP R0,R1 
00001A  AC 01 00 28   698  	AJMP IF_EQ,L198 
00001E                700  L203: 
00001E                702  L204: 
00001E                704  L205: 
00001E  48 15         705  	MOV R1,#5 
000020  68 10 00 00   706  	MOV DS0,#@_vol_idx_bak 
000024                707  .if KERNEL1_E = 1
+                     708  	NOP
+                     709  .endif
000024  A8 F1 00 00   710  	MOV DS0:_vol_idx_bak,R1 
000028                712  L206: 
000028                714  L207: 
000028                715  L198: 
000028                717  L208: 
000028                719  L209: 
000028                720  L197: 
000028  38 1F         721  	POP R1 
00002A  38 0F         722  	POP R0 
00002C                723  	UNLINK_MACRO
00002C  2F 00         723  UNLINK 
+                     723  
00002E                724  .if KERNEL1_5 = 1
+                     725  	CRIT #1
+                     726  .endif
00002E  C6 20         727  	RETS 
+                     729  
+                     730  .area _$inc_volume(SYSTEM_CODE,REL) 
+B 00 0F
000000                731  _inc_volume:: 
000000  2E 00         734  	LINK #0 
000002  58 F0         735  	PUSH R0 
000004  58 F1         736  	PUSH R1 
000006                738  L215: 
000006                740  L216: 
000006  78 1B 00 06   741  	MOV R1,[FP+#(6)] 
00000A  6B 3B 00 06   742  	MOVB [FP+#(6)],Rb3 
00000E                744  L217: 
+                     745  	
00000E  7C 59 F1 40   746  	BMOV R1.0,0x59.4 
000012  54 11         747  	AND R1,#1 
000014  68 10 00 00   748  	MOV DS0,#@(_gflag) 
000018  88 F0 00 00   749  	MOV R0,DS0:(_gflag) 
00001C  74 F0 FF BF   750  	AND R0,#0xffbf 
000020  54 11         751  	AND R1,#1 
000022  4C 16         752  	SL R1,#6 
000024  74 F1 00 40   753  	AND R1,#64 
000028  15 01         754  	OR R0,R1 
00002A  68 10 00 00   755  	MOV DS0,#@_gflag 
00002E                756  .if KERNEL1_E = 1
+                     757  	NOP
+                     758  .endif
00002E  A8 F0 00 00   759  	MOV DS0:_gflag,R0 
000032                761  L218: 
000032  68 10 00 00   762  	MOV DS0,#@(_gflag) 
000036  88 F0 00 00   763  	MOV R0,DS0:(_gflag) 
00003A  74 F0 00 40   764  	AND R0,#64 
00003E  56 00         765  	CMP R0,#0 
000040  AC 02 00 54   766  	AJMP IF_NE,L211 
000044                768  L219: 
000044                770  L220: 
000044                772  L221: 
+                     773  	
000044  F2 4E         774  	BCLR 0x4e.2 
000046                776  L222: 
000046  48 05         777  	MOV R0,#5 
000048  58 F0         778  	PUSH R0 
00004A                779  .if KERNEL1_5 = 1
+                     780  	CRIT #1
+                     781  .endif
00004A  BD 00 00 00   782  	SCALL _DAC_Speaker_Volume 
00004E  50 F2         783  	ADD SP,#2 
000050                785  L223: 
000050                787  L224: 
000050  AC 00 00 7C   788  	AJMP #0,L212 
000054                789  L211: 
000054                791  L225: 
000054                793  L226: 
000054                795  L227: 
+                     796  	
000054  E2 4E         797  	BSET 0x4e.2 
000056                799  L228: 
000056  68 10 00 00   800  	MOV DS0,#@_flag_wav_play 
00005A  99 F0 00 00   801  	MOVZE R0,DS0:_flag_wav_play 
00005E  56 00         802  	CMP R0,#0 
000060  AC 01 00 72   803  	AJMP IF_EQ,L213 
000064                805  L229: 
000064                807  L230: 
000064                809  L231: 
000064  48 0C         810  	MOV R0,#12 
000066  58 F0         811  	PUSH R0 
000068                812  .if KERNEL1_5 = 1
+                     813  	CRIT #1
+                     814  .endif
000068  BD 00 00 00   815  	SCALL _DAC_Speaker_Volume 
00006C  50 F2         816  	ADD SP,#2 
00006E                818  L232: 
00006E                820  L233: 
00006E  AC 00 00 7C   821  	AJMP #0,L214 
000072                822  L213: 
000072                824  L234: 
000072                826  L235: 
000072                828  L236: 
000072  48 08         829  	MOV R0,#8 
000074  58 F0         830  	PUSH R0 
000076                831  .if KERNEL1_5 = 1
+                     832  	CRIT #1
+                     833  .endif
000076  BD 00 00 00   834  	SCALL _DAC_Speaker_Volume 
00007A  50 F2         835  	ADD SP,#2 
00007C                837  L237: 
00007C                839  L238: 
00007C                840  L214: 
00007C                842  L239: 
00007C                844  L240: 
00007C                845  L212: 
00007C                847  L241: 
00007C                849  L242: 
00007C                850  L210: 
00007C  38 1F         851  	POP R1 
00007E  38 0F         852  	POP R0 
000080                853  	UNLINK_MACRO
000080  2F 00         853  UNLINK 
+                     853  
000082                854  .if KERNEL1_5 = 1
+                     855  	CRIT #1
+                     856  .endif
000082  C6 20         857  	RETS 
+                     859  
+                     860  .area _$SpeakDetect(SYSTEM_CODE,REL) 
+B 00 10
000000                861  _SpeakDetect:: 
000000  2E 00         864  	LINK #0 
000002  58 F0         865  	PUSH R0 
000004  58 F1         866  	PUSH R1 
000006  58 F2         867  	PUSH R2 
000008  58 F3         868  	PUSH R3 
00000A                870  L248: 
00000A                872  L249: 
00000A  78 0B 00 06   873  	MOV R0,[FP+#(6)] 
00000E  6B 1B 00 06   874  	MOVB [FP+#(6)],Rb1 
000012                876  L250: 
000012                877  .if KERNEL1_5 = 1
+                     878  	CRIT #1
+                     879  .endif
000012  BD 00 00 00   880  	SCALL _judge_op 
000016                882  L251: 
000016  68 10 00 00   883  	MOV DS0,#@(_vol_idx) 
00001A  88 F0 00 00   884  	MOV R0,DS0:(_vol_idx) 
00001E  68 10 00 00   885  	MOV DS0,#@(_vol_idx_bak) 
000022  96 F0 00 00   886  	CMP R0,DS0:(_vol_idx_bak) 
000026  AC 01 00 A4   887  	AJMP IF_EQ,L244 
00002A                889  L252: 
00002A                891  L253: 
00002A                893  L254: 
00002A  68 10 00 00   894  	MOV DS0,#@(_vol_idx) 
00002E  88 F0 00 00   895  	MOV R0,DS0:(_vol_idx) 
000032  68 10 00 00   896  	MOV DS0,#@_vol_idx_bak 
000036                897  .if KERNEL1_E = 1
+                     898  	NOP
+                     899  .endif
000036  A8 F0 00 00   900  	MOV DS0:_vol_idx_bak,R0 
00003A                902  L255: 
00003A                903  .if KERNEL1_5 = 1
+                     904  	CRIT #1
+                     905  .endif
00003A  BD 00 00 00   906  	SCALL _DAC_Disable_Speaker 
00003E                908  L256: 
00003E                909  .if KERNEL1_5 = 1
+                     910  	CRIT #1
+                     911  .endif
00003E  BD 00 00 00   912  	SCALL _DAC_Enable_HeadPhone 
000042                914  L257: 
000042  68 10 00 00   915  	MOV DS0,#@(_sys_flag) 
000046  88 F0 00 00   916  	MOV R0,DS0:(_sys_flag) 
00004A  74 F0 40 00   917  	AND R0,#16384 
00004E  56 00         918  	CMP R0,#0 
000050  AC 01 00 68   919  	AJMP IF_EQ,L246 
000054                921  L258: 
000054                923  L259: 
000054                925  L260: 
000054  68 10 00 00   926  	MOV DS0,#@(_VolTbl) 
000058  88 F0 00 00   927  	MOV R0,DS0:(_VolTbl) 
00005C  58 F0         928  	PUSH R0 
00005E                929  .if KERNEL1_5 = 1
+                     930  	CRIT #1
+                     931  .endif
00005E  BD 00 00 00   932  	SCALL _DAC_HeadPhone_Volume 
000062  50 F2         933  	ADD SP,#2 
000064                935  L261: 
000064                937  L262: 
000064  AC 00 00 96   938  	AJMP #0,L247 
000068                939  L246: 
000068                941  L263: 
000068                943  L264: 
000068                945  L265: 
000068  68 10 00 00   946  	MOV DS0,#@_vol_idx 
00006C  88 F1 00 00   947  	MOV R1,DS0:_vol_idx 
000070  48 00         948  	MOV R0,#0 
000072  4C 11         949  	SL R1,#1 
000074  5C 01         950  	SLOSB R0,#1 
000076  68 F3 00 00   951  	MOV R3,#(_VolTbl) 
00007A  68 F2 00 00   952  	MOV R2,#^(_VolTbl) 
00007E  10 13         953  	ADD R1,R3 
000080  11 02         954  	ADDC R0,R2 
000082                955  	LOAD2$indirect R0
000082  4C 11         955  SL R1,#1 
000084  5C 01         955  SLOSB R0,#1 
000086  BA 10 F2 E0   955  MOVX DS0, R0 
00008A  4D 11         955  SR R1,#1 
00008C  28 01         955  MOV R0,[R1] 
+                     955  
00008E  58 F0         956  	PUSH R0 
000090                957  .if KERNEL1_5 = 1
+                     958  	CRIT #1
+                     959  .endif
000090  BD 00 00 00   960  	SCALL _DAC_HeadPhone_Volume 
000094  50 F2         961  	ADD SP,#2 
000096                963  L266: 
000096                965  L267: 
000096                966  L247: 
000096                968  L268: 
000096  79 1B 00 06   969  	MOVB Rb1,[FP+#(6)] 
00009A  19 01         970  	MOVZE R0,Rb1 
00009C  58 F0         971  	PUSH R0 
00009E                972  .if KERNEL1_5 = 1
+                     973  	CRIT #1
+                     974  .endif
00009E  BD 00 00 00   975  	SCALL _inc_volume 
0000A2  50 F2         976  	ADD SP,#2 
0000A4                978  L269: 
0000A4                980  L270: 
0000A4                981  L244: 
0000A4                983  L271: 
0000A4                985  L272: 
0000A4                986  L243: 
0000A4  38 3F         987  	POP R3 
0000A6  38 2F         988  	POP R2 
0000A8  38 1F         989  	POP R1 
0000AA  38 0F         990  	POP R0 
0000AC                991  	UNLINK_MACRO
0000AC  2F 00         991  UNLINK 
+                     991  
0000AE                992  .if KERNEL1_5 = 1
+                     993  	CRIT #1
+                     994  .endif
0000AE  C6 20         995  	RETS 
+                     997  
+                     998  .area _$CheckUSB(SYSTEM_CODE,REL) 
+B 00 11
000000                999  _CheckUSB:: 
000000  2E 00        1001  	LINK #0 
000002  58 F1        1002  	PUSH R1 
000004               1004  L279: 
000004               1006  L280: 
000004               1008  L281: 
000004  68 10 00 00  1009  	MOV DS0,#@(_gflag) 
000008  88 F0 00 00  1010  	MOV R0,DS0:(_gflag) 
00000C  74 F0 80 00  1011  	AND R0,#0x8000 
000010  56 00        1012  	CMP R0,#0 
000012  AC 01 00 1C  1013  	AJMP IF_EQ,L274 
000016               1015  L282: 
000016  48 00        1016  	MOV R0,#0 
000018  AC 00 00 5C  1017  	AJMP #0,L273 
00001C               1018  L274: 
00001C               1020  L283: 
00001C  68 10 00 00  1021  	MOV DS0,#@(_adc2_value) 
000020  88 F0 00 00  1022  	MOV R0,DS0:(_adc2_value) 
000024  76 F0 00 64  1023  	CMP R0,#100 
000028  AC 0C 00 5A  1024  	AJMP IF_ULE,L276 
00002C               1026  L284: 
00002C               1028  L285: 
00002C               1030  L286: 
00002C  48 00        1031  	MOV R0,#0 
00002E  58 F0        1032  	PUSH R0 
000030               1033  .if KERNEL1_5 = 1
+                    1034  	CRIT #1
+                    1035  .endif
000030  BD 00 00 00  1036  	SCALL _clkup_time 
000034  50 F2        1037  	ADD SP,#2 
000036               1039  L287: 
000036  68 F1 00 16  1040  	MOV R1,#(L278) 
00003A  68 F0 00 16  1041  	MOV R0,#^(L278) 
00003E  58 F1        1042  	PUSH R1 
000040  58 F0        1043  	PUSH R0 
000042               1044  .if KERNEL1_5 = 1
+                    1045  	CRIT #1
+                    1046  .endif
000042  BD 00 00 00  1047  	SCALL _SendString 
000046  50 F4        1048  	ADD SP,#4 
000048               1050  L288: 
000048  48 10        1051  	MOV R1,#0 
00004A  48 00        1052  	MOV R0,#0 
00004C  AA F0 00 00  1053  	MOVX _sys_led_timer,R0 
000050  AA F1 00 02  1054  	MOVX _sys_led_timer+2,R1 
000054               1056  L289: 
000054  48 01        1057  	MOV R0,#1 
000056  AC 00 00 5C  1058  	AJMP #0,L273 
00005A               1060  L290: 
00005A               1062  L291: 
00005A               1063  L276: 
00005A               1065  L292: 
00005A  48 00        1066  	MOV R0,#0 
00005C               1068  L293: 
00005C               1070  L294: 
00005C               1071  L273: 
00005C  38 1F        1072  	POP R1 
00005E               1073  	UNLINK_MACRO
00005E  2F 00        1073  UNLINK 
+                    1073  
000060               1074  .if KERNEL1_5 = 1
+                    1075  	CRIT #1
+                    1076  .endif
000060  C6 20        1077  	RETS 
+                    1079  
+                    1080  .area _$power_forbid_autopoweroff(SYSTEM_CODE,REL) 
+B 00 12
000000               1081  _power_forbid_autopoweroff:: 
000000  2E 00        1083  	LINK #0 
000002  58 F0        1084  	PUSH R0 
000004               1086  L296: 
000004               1088  L297: 
000004               1090  L298: 
000004  49 11        1091  	MOVB Rb1,#1 
000006  AB F1 00 00  1092  	MOVBX _g_b_autopoweroff_forbidden,Rb1 
00000A               1094  L299: 
00000A               1096  L300: 
00000A               1097  L295: 
00000A  38 0F        1098  	POP R0 
00000C               1099  	UNLINK_MACRO
00000C  2F 00        1099  UNLINK 
+                    1099  
00000E               1100  .if KERNEL1_5 = 1
+                    1101  	CRIT #1
+                    1102  .endif
00000E  C6 20        1103  	RETS 
+                    1105  
+                    1106  .area _$power_authorize_autopoweroff(SYSTEM_CODE,REL) 
+B 00 13
000000               1107  _power_authorize_autopoweroff:: 
000000  2E 00        1109  	LINK #0 
000002  58 F0        1110  	PUSH R0 
000004  58 F1        1111  	PUSH R1 
000006               1113  L302: 
000006               1115  L303: 
000006               1117  L304: 
000006  49 10        1118  	MOVB Rb1,#0 
000008  AB F1 00 00  1119  	MOVBX _g_b_autopoweroff_forbidden,Rb1 
00000C               1121  L305: 
00000C               1122  .if KERNEL1_5 = 1
+                    1123  	CRIT #1
+                    1124  .endif
00000C  BD 00 00 00  1125  	SCALL _timer_get_time 
000010  70 F1 93 E0  1126  	ADD R1,#(0x000493e0 & 0xFFFF) 
000014  51 04        1127  	ADDC R0,#(0x000493e0 >> 16) 
000016  50 11        1128  	ADD R1,#1 
000018  51 00        1129  	ADDC R0,#0 
00001A  AA F0 00 00  1130  	MOVX _g_autopoweroff_timer,R0 
00001E  AA F1 00 02  1131  	MOVX _g_autopoweroff_timer+2,R1 
000022               1133  L306: 
000022  68 F0 FD FF  1134  	MOV R0,#0xfdff 
000026  68 10 00 00  1135  	MOV DS0,#@(_systerm_flag) 
00002A  B4 F0 00 00  1136  	AND DS0:(_systerm_flag),R0 
00002E               1138  L307: 
00002E  68 F0 FF EF  1139  	MOV R0,#0xffef 
000032  68 10 00 00  1140  	MOV DS0,#@(_systerm_flag) 
000036  B4 F0 00 00  1141  	AND DS0:(_systerm_flag),R0 
00003A               1143  L308: 
00003A               1145  L309: 
00003A               1146  L301: 
00003A  38 1F        1147  	POP R1 
00003C  38 0F        1148  	POP R0 
00003E               1149  	UNLINK_MACRO
00003E  2F 00        1149  UNLINK 
+                    1149  
000040               1150  .if KERNEL1_5 = 1
+                    1151  	CRIT #1
+                    1152  .endif
000040  C6 20        1153  	RETS 
+                    1154  
000042               1155  _Delay_ms:: 
000042  2E 00        1156      LINK #0 
000044  58 F0        1157      PUSH R0 
000046  58 F1        1158      PUSH R1 
+                    1159  
000048  78 0B 00 06  1160      MOV R0,[FP+#(6)] 
+                    1161  $Loop2: 
00004C  68 F1 2E E0  1162      MOV R1,#12000  
+                    1163  $Loop: 
000050  4E 10        1164      CMPD1 R1,#0 
000052  AC 02 00 50  1165      AJMP IF_NE,$Loop 
000056  4E 00        1166      CMPD1 R0,#0 
000058  AC 02 00 4C  1167      AJMP IF_NE,$Loop2 
00005C  38 1F        1168      POP R1 
00005E  38 0F        1169      POP R0 
000060               1170      UNLINK_MACRO
000060  2F 00        1170  UNLINK 
+                    1170  
000062  C6 20        1171      RETS 
+                    1172  
+                    1173  
+                    1174  .area _$SysClkUpgrade(SYSTEM_CODE,REL) 
+B 00 14
000000               1176  _SysClkUpgrade:: 
000000  2E 00        1178  	LINK #0 
000002  58 F0        1179  	PUSH R0 
000004  58 F1        1180  	PUSH R1 
000006               1182  L314: 
000006               1184  L315: 
000006               1186  L316: 
000006  68 10 00 00  1187  	MOV DS0,#@(_gflag) 
00000A  88 F0 00 00  1188  	MOV R0,DS0:(_gflag) 
00000E  74 F0 04 00  1189  	AND R0,#1024 
000012  56 00        1190  	CMP R0,#0 
000014  AC 01 00 54  1191  	AJMP IF_EQ,L311 
000018               1193  L317: 
000018               1195  L318: 
000018               1197  L319: 
000018  48 09        1198  	MOV R0,#9 
00001A  58 F0        1199  	PUSH R0 
00001C               1200  .if KERNEL1_5 = 1
+                    1201  	CRIT #1
+                    1202  .endif
00001C  BD 00 00 00  1203  	SCALL _SystemClock 
000020  50 F2        1204  	ADD SP,#2 
000022               1206  L320: 
000022  48 03        1207  	MOV R0,#3 
000024  58 F0        1208  	PUSH R0 
000026               1209  .if KERNEL1_5 = 1
+                    1210  	CRIT #1
+                    1211  .endif
000026  BD 00 00 00  1212  	SCALL _SystemClock 
00002A  50 F2        1213  	ADD SP,#2 
00002C               1215  L321: 
00002C  48 00        1216  	MOV R0,#0 
00002E  58 F0        1217  	PUSH R0 
000030               1218  .if KERNEL1_5 = 1
+                    1219  	CRIT #1
+                    1220  .endif
000030  BD 00 00 00  1221  	SCALL _SPIF_Set_Clock 
000034  50 F2        1222  	ADD SP,#2 
000036               1224  L322: 
000036  68 F0 FB FF  1225  	MOV R0,#0xfbff 
00003A  68 10 00 00  1226  	MOV DS0,#@(_gflag) 
00003E  B4 F0 00 00  1227  	AND DS0:(_gflag),R0 
000042               1229  L323: 
000042  68 F1 00 0C  1230  	MOV R1,#(L313) 
000046  68 F0 00 0C  1231  	MOV R0,#^(L313) 
00004A  58 F1        1232  	PUSH R1 
00004C  58 F0        1233  	PUSH R0 
00004E               1234  .if KERNEL1_5 = 1
+                    1235  	CRIT #1
+                    1236  .endif
00004E  BD 00 00 00  1237  	SCALL _SendString 
000052  50 F4        1238  	ADD SP,#4 
000054               1240  L324: 
000054               1242  L325: 
000054               1243  L311: 
000054               1245  L326: 
000054               1247  L327: 
000054               1248  L310: 
000054  38 1F        1249  	POP R1 
000056  38 0F        1250  	POP R0 
000058               1251  	UNLINK_MACRO
000058  2F 00        1251  UNLINK 
+                    1251  
00005A               1252  .if KERNEL1_5 = 1
+                    1253  	CRIT #1
+                    1254  .endif
00005A  C6 20        1255  	RETS 
+                    1257  
+                    1258  .area _$SysClkDowngrade(SYSTEM_CODE,REL) 
+B 00 15
000000               1259  _SysClkDowngrade:: 
000000  2E 00        1261  	LINK #0 
000002  58 F0        1262  	PUSH R0 
000004  58 F1        1263  	PUSH R1 
000006               1265  L332: 
000006               1267  L333: 
000006               1269  L334: 
000006  68 10 00 00  1270  	MOV DS0,#@(_gflag) 
00000A  88 F0 00 00  1271  	MOV R0,DS0:(_gflag) 
00000E  74 F0 04 00  1272  	AND R0,#1024 
000012  56 00        1273  	CMP R0,#0 
000014  AC 02 00 48  1274  	AJMP IF_NE,L329 
000018               1276  L335: 
000018               1278  L336: 
000018               1280  L337: 
000018  68 F1 00 00  1281  	MOV R1,#(L331) 
00001C  68 F0 00 00  1282  	MOV R0,#^(L331) 
000020  58 F1        1283  	PUSH R1 
000022  58 F0        1284  	PUSH R0 
000024               1285  .if KERNEL1_5 = 1
+                    1286  	CRIT #1
+                    1287  .endif
000024  BD 00 00 00  1288  	SCALL _SendString 
000028  50 F4        1289  	ADD SP,#4 
00002A               1291  L338: 
00002A  48 08        1292  	MOV R0,#8 
00002C  58 F0        1293  	PUSH R0 
00002E               1294  .if KERNEL1_5 = 1
+                    1295  	CRIT #1
+                    1296  .endif
00002E  BD 00 00 00  1297  	SCALL _SPIF_Set_Clock 
000032  50 F2        1298  	ADD SP,#2 
000034               1300  L339: 
+                    1301  
000034  FC 40        1302           BCLR MODCON.12 
000036  BF 00 00 2D  1303           REP #45 
00003A  C7 00        1304     		NOP 
+                    1305        
00003C               1307  L340: 
00003C  68 F0 04 00  1308  	MOV R0,#1024 
000040  68 10 00 00  1309  	MOV DS0,#@(_gflag) 
000044  B5 F0 00 00  1310  	OR DS0:(_gflag),R0 
000048               1312  L341: 
000048               1314  L342: 
000048               1315  L329: 
000048               1317  L343: 
000048               1319  L344: 
000048               1320  L328: 
000048  38 1F        1321  	POP R1 
00004A  38 0F        1322  	POP R0 
00004C               1323  	UNLINK_MACRO
00004C  2F 00        1323  UNLINK 
+                    1323  
00004E               1324  .if KERNEL1_5 = 1
+                    1325  	CRIT #1
+                    1326  .endif
00004E  C6 20        1327  	RETS 
+                    1329  
+                    1330  .area _$clkup_time(SYSTEM_CODE,REL) 
+B 00 16
000000               1331  _clkup_time:: 
000000  2E 00        1334  	LINK #0 
000002  58 F0        1335  	PUSH R0 
000004  58 F1        1336  	PUSH R1 
000006               1338  L348: 
000006               1340  L349: 
000006  78 0B 00 06  1341  	MOV R0,[FP+#(6)] 
00000A  6B 1B 00 06  1342  	MOVB [FP+#(6)],Rb1 
00000E               1344  L350: 
00000E               1345  .if KERNEL1_5 = 1
+                    1346  	CRIT #1
+                    1347  .endif
00000E  BD 00 00 00  1348  	SCALL _SysClkUpgrade 
000012               1350  L351: 
000012  79 1B 00 06  1351  	MOVB Rb1,[FP+#(6)] 
000016  19 01        1352  	MOVZE R0,Rb1 
000018  56 00        1353  	CMP R0,#0 
00001A  AC 01 00 3C  1354  	AJMP IF_EQ,L346 
00001E               1356  L352: 
00001E               1358  L353: 
00001E               1360  L354: 
00001E               1361  .if KERNEL1_5 = 1
+                    1362  	CRIT #1
+                    1363  .endif
00001E  BD 00 00 00  1364  	SCALL _timer_get_time 
000022  70 F1 07 D0  1365  	ADD R1,#2000 
000026  51 00        1366  	ADDC R0,#0 
000028  50 11        1367  	ADD R1,#1 
00002A  51 00        1368  	ADDC R0,#0 
00002C               1369  	SAVE4$memory _g_system_clk_timer
00002C  68 10 00 00  1369  MOV DS0,#@(_g_system_clk_timer) 
000030               1369  .if KERNEL1_E = 1
+                    1369  NOP
+                    1369  .endif
000030  A8 F0 00 00  1369  MOV DS0:(_g_system_clk_timer),R0 
000034  A8 F1 00 02  1369  MOV DS0:(_g_system_clk_timer+2),R1 
+                    1369  
000038               1371  L355: 
000038               1373  L356: 
000038  AC 00 00 4C  1374  	AJMP #0,L347 
00003C               1375  L346: 
00003C               1377  L357: 
00003C               1379  L358: 
00003C               1381  L359: 
00003C  48 10        1382  	MOV R1,#0 
00003E  48 00        1383  	MOV R0,#0 
000040               1384  	SAVE4$memory _g_system_clk_timer
000040  68 10 00 00  1384  MOV DS0,#@(_g_system_clk_timer) 
000044               1384  .if KERNEL1_E = 1
+                    1384  NOP
+                    1384  .endif
000044  A8 F0 00 00  1384  MOV DS0:(_g_system_clk_timer),R0 
000048  A8 F1 00 02  1384  MOV DS0:(_g_system_clk_timer+2),R1 
+                    1384  
00004C               1386  L360: 
00004C               1388  L361: 
00004C               1389  L347: 
00004C               1391  L362: 
00004C               1393  L363: 
00004C               1394  L345: 
00004C  38 1F        1395  	POP R1 
00004E  38 0F        1396  	POP R0 
000050               1397  	UNLINK_MACRO
000050  2F 00        1397  UNLINK 
+                    1397  
000052               1398  .if KERNEL1_5 = 1
+                    1399  	CRIT #1
+                    1400  .endif
000052  C6 20        1401  	RETS 
+                    1403  
+                    1404  .area _$time3_init(SYSTEM_TIMER3_INIT_CODE,REL) 
+B 00 17
000000               1405  _time3_init:: 
000000  2E 00        1407  	LINK #0 
000002  58 F0        1408  	PUSH R0 
000004  58 F1        1409  	PUSH R1 
000006               1411  L365: 
000006               1413  L366: 
000006               1415  L367: 
000006  68 F1 00 00  1416  	MOV R1,#(_TIMER0_ISR) 
00000A  68 F0 00 00  1417  	MOV R0,#^(_TIMER0_ISR) 
00000E  58 F1        1418  	PUSH R1 
000010  58 F0        1419  	PUSH R0 
000012  68 F0 00 16  1420  	MOV R0,#22 
000016  58 F0        1421  	PUSH R0 
000018               1422  .if KERNEL1_5 = 1
+                    1423  	CRIT #1
+                    1424  .endif
000018  BD 00 00 00  1425  	SCALL _SetVector 
00001C  50 F6        1426  	ADD SP,#6 
00001E               1428  L368: 
00001E  68 F0 D0 05  1429  	MOV R0,#0xd005 
000022  AA F0 00 00  1430  	MOVX _TMR3CR,R0 
000026               1432  L369: 
000026  68 F0 02 EE  1433  	MOV R0,#750 
00002A  AA F0 00 00  1434  	MOVX _TM3PRD,R0 
00002E               1436  L370: 
00002E  48 01        1437  	MOV R0,#1 
000030  AA F0 00 00  1438  	MOVX _TM3ICR,R0 
000034               1440  L371: 
000034  48 10        1441  	MOV R1,#0 
000036  48 00        1442  	MOV R0,#0 
000038               1443  	SAVE4$memory _g_timer_tick
000038  68 10 00 00  1443  MOV DS0,#@(_g_timer_tick) 
00003C               1443  .if KERNEL1_E = 1
+                    1443  NOP
+                    1443  .endif
00003C  A8 F0 00 00  1443  MOV DS0:(_g_timer_tick),R0 
000040  A8 F1 00 02  1443  MOV DS0:(_g_timer_tick+2),R1 
+                    1443  
000044               1445  L372: 
000044               1447  L373: 
000044               1448  L364: 
000044  38 1F        1449  	POP R1 
000046  38 0F        1450  	POP R0 
000048               1451  	UNLINK_MACRO
000048  2F 00        1451  UNLINK 
+                    1451  
00004A               1452  .if KERNEL1_5 = 1
+                    1453  	CRIT #1
+                    1454  .endif
00004A  C6 20        1455  	RETS 
+                    1457  
+                    1458  .area _$System_Timer3_Init(SYSTEM_TIMER3_INIT_CODE,REL) 
+B 00 18
000000               1459  _System_Timer3_Init:: 
000000  2E 00        1461  	LINK #0 
000002  58 F0        1462  	PUSH R0 
000004  58 F1        1463  	PUSH R1 
000006               1465  L375: 
000006               1467  L376: 
000006               1469  L377: 
000006               1470  .if KERNEL1_5 = 1
+                    1471  	CRIT #1
+                    1472  .endif
000006  BD 00 00 00  1473  	SCALL _time3_init 
00000A               1475  L378: 
+                    1476  	
00000A  7C 59 F0 40  1477  	BMOV R0.0,0x59.4 
00000E  54 01        1478  	AND R0,#1 
000010  68 10 00 00  1479  	MOV DS0,#@(_gflag) 
000014  88 F1 00 00  1480  	MOV R1,DS0:(_gflag) 
000018  74 F1 FF BF  1481  	AND R1,#0xffbf 
00001C  54 01        1482  	AND R0,#1 
00001E  4C 06        1483  	SL R0,#6 
000020  74 F0 00 40  1484  	AND R0,#64 
000024  15 10        1485  	OR R1,R0 
000026  68 10 00 00  1486  	MOV DS0,#@_gflag 
00002A               1487  .if KERNEL1_E = 1
+                    1488  	NOP
+                    1489  .endif
00002A  A8 F1 00 00  1490  	MOV DS0:_gflag,R1 
00002E               1492  L379: 
00002E  49 30        1493  	MOVB Rb3,#0 
000030  68 10 00 00  1494  	MOV DS0,#@_battery_count 
000034               1495  .if KERNEL1_E = 1
+                    1496  	NOP
+                    1497  .endif
000034  A9 F3 00 00  1498  	MOVB DS0:_battery_count,Rb3 
000038               1500  L380: 
000038  49 30        1501  	MOVB Rb3,#0 
00003A  68 10 00 00  1502  	MOV DS0,#@_adc_channel 
00003E               1503  .if KERNEL1_E = 1
+                    1504  	NOP
+                    1505  .endif
00003E  A9 F3 00 00  1506  	MOVB DS0:_adc_channel,Rb3 
000042               1508  L381: 
000042  48 11        1509  	MOV R1,#1 
000044  48 00        1510  	MOV R0,#0 
000046               1511  	SAVE4$memory _adc_timer
000046  68 10 00 00  1511  MOV DS0,#@(_adc_timer) 
00004A               1511  .if KERNEL1_E = 1
+                    1511  NOP
+                    1511  .endif
00004A  A8 F0 00 00  1511  MOV DS0:(_adc_timer),R0 
00004E  A8 F1 00 02  1511  MOV DS0:(_adc_timer+2),R1 
+                    1511  
000052               1513  L382: 
000052  48 10        1514  	MOV R1,#0 
000054  48 00        1515  	MOV R0,#0 
000056  AA F0 00 00  1516  	MOVX _sys_led_timer,R0 
00005A  AA F1 00 02  1517  	MOVX _sys_led_timer+2,R1 
00005E               1519  L383: 
00005E               1520  .if KERNEL1_5 = 1
+                    1521  	CRIT #1
+                    1522  .endif
00005E  BD 00 00 00  1523  	SCALL _timer_get_time 
000062  70 F1 00 32  1524  	ADD R1,#50 
000066  51 00        1525  	ADDC R0,#0 
000068  50 11        1526  	ADD R1,#1 
00006A  51 00        1527  	ADDC R0,#0 
00006C  AA F0 00 00  1528  	MOVX _sys_led_timer,R0 
000070  AA F1 00 02  1529  	MOVX _sys_led_timer+2,R1 
000074               1531  L384: 
000074  49 10        1532  	MOVB Rb1,#0 
000076  68 10 00 00  1533  	MOV DS0,#@_flag_tishi 
00007A               1534  .if KERNEL1_E = 1
+                    1535  	NOP
+                    1536  .endif
00007A  A9 F1 00 00  1537  	MOVB DS0:_flag_tishi,Rb1 
00007E               1539  L385: 
00007E  48 10        1540  	MOV R1,#0 
000080  48 00        1541  	MOV R0,#0 
000082  AA F0 00 00  1542  	MOVX _op_close_timer,R0 
000086  AA F1 00 02  1543  	MOVX _op_close_timer+2,R1 
00008A               1545  L386: 
00008A  48 10        1546  	MOV R1,#0 
00008C  48 00        1547  	MOV R0,#0 
00008E  AA F0 00 00  1548  	MOVX _rec_led_timer,R0 
000092  AA F1 00 02  1549  	MOVX _rec_led_timer+2,R1 
000096               1551  L387: 
000096               1552  .if KERNEL1_5 = 1
+                    1553  	CRIT #1
+                    1554  .endif
000096  BD 00 00 00  1555  	SCALL _timer_get_time 
00009A  70 F1 00 19  1556  	ADD R1,#25 
00009E  51 00        1557  	ADDC R0,#0 
0000A0  50 11        1558  	ADD R1,#1 
0000A2  51 00        1559  	ADDC R0,#0 
0000A4  AA F0 00 00  1560  	MOVX _rec_led_timer,R0 
0000A8  AA F1 00 02  1561  	MOVX _rec_led_timer+2,R1 
0000AC               1563  L388: 
0000AC  49 11        1564  	MOVB Rb1,#1 
0000AE  68 10 00 00  1565  	MOV DS0,#@_flag_tishi 
0000B2               1566  .if KERNEL1_E = 1
+                    1567  	NOP
+                    1568  .endif
0000B2  A9 F1 00 00  1569  	MOVB DS0:_flag_tishi,Rb1 
0000B6               1571  L389: 
0000B6  48 00        1572  	MOV R0,#0 
0000B8  68 10 00 00  1573  	MOV DS0,#@_sleep_mode_count 
0000BC               1574  .if KERNEL1_E = 1
+                    1575  	NOP
+                    1576  .endif
0000BC  A8 F0 00 00  1577  	MOV DS0:_sleep_mode_count,R0 
0000C0               1579  L390: 
0000C0  48 10        1580  	MOV R1,#0 
0000C2  48 00        1581  	MOV R0,#0 
0000C4               1582  	SAVE4$memory _sleep_mode_timer
0000C4  68 10 00 00  1582  MOV DS0,#@(_sleep_mode_timer) 
0000C8               1582  .if KERNEL1_E = 1
+                    1582  NOP
+                    1582  .endif
0000C8  A8 F0 00 00  1582  MOV DS0:(_sleep_mode_timer),R0 
0000CC  A8 F1 00 02  1582  MOV DS0:(_sleep_mode_timer+2),R1 
+                    1582  
0000D0               1584  L391: 
0000D0               1586  L392: 
0000D0               1587  L374: 
0000D0  38 1F        1588  	POP R1 
0000D2  38 0F        1589  	POP R0 
0000D4               1590  	UNLINK_MACRO
0000D4  2F 00        1590  UNLINK 
+                    1590  
0000D6               1591  .if KERNEL1_5 = 1
+                    1592  	CRIT #1
+                    1593  .endif
0000D6  C6 20        1594  	RETS 
+                    1596  
+                    1597  .area _$BatteryDetect(BATTERY_CODE,REL) 
+B 00 19
000000               1598  _BatteryDetect:: 
000000  2E 02        1600  	LINK #2 
000002  58 F1        1601  	PUSH R1 
000004  58 F2        1602  	PUSH R2 
000006  58 F3        1603  	PUSH R3 
000008               1605  L401: 
000008               1607  L402: 
000008               1611  L403: 
000008  68 10 00 00  1612  	MOV DS0,#@(_adc2_value) 
00000C  88 F0 00 00  1613  	MOV R0,DS0:(_adc2_value) 
000010  76 F0 00 64  1614  	CMP R0,#100 
000014  AC 0C 00 1E  1615  	AJMP IF_ULE,L394 
000018               1617  L404: 
000018  48 00        1618  	MOV R0,#0 
00001A  AC 00 00 DC  1619  	AJMP #0,L393 
00001E               1620  L394: 
00001E               1622  L405: 
00001E  68 F1 00 64  1623  	MOV R1,#100 
000022  48 00        1624  	MOV R0,#0 
000024  68 10 00 00  1625  	MOV DS0,#@_adc0_value 
000028  88 F3 00 00  1626  	MOV R3,DS0:_adc0_value 
00002C  48 20        1627  	MOV R2,#0 
00002E               1628  	MUL$ulong R0
00002E  58 F3        1628  PUSH R3 
000030  58 F2        1628  PUSH R2 
000032  58 F1        1628  PUSH R1 
000034  58 F0        1628  PUSH R0 
000036               1628  .if KERNEL1_5 = 1
+                    1628  CRIT #1
+                    1628  .endif
000036  BD 00 00 00  1628  SCALL __MULulong 
00003A  50 F8        1628  ADD SP,#8 
+                    1628  
00003C  68 10 00 00  1629  	MOV DS0,#@_adc7_value 
000040  88 F3 00 00  1630  	MOV R3,DS0:_adc7_value 
000044  48 20        1631  	MOV R2,#0 
000046               1632  	DIV$ulong R0
000046  58 F3        1632  PUSH R3 
000048  58 F2        1632  PUSH R2 
00004A  58 F1        1632  PUSH R1 
00004C  58 F0        1632  PUSH R0 
00004E               1632  .if KERNEL1_5 = 1
+                    1632  CRIT #1
+                    1632  .endif
00004E  BD 00 00 00  1632  SCALL __DIVulong 
000052  50 F8        1632  ADD SP,#8 
+                    1632  
000054  08 01        1633  	MOV R0,R1 
000056  6A 0B FF FE  1634  	MOV [FP+#(-2)],R0 
00005A               1636  L406: 
00005A  78 0B FF FE  1637  	MOV R0,[FP+#(-2)] 
00005E  76 F0 00 59  1638  	CMP R0,#89 
000062  AC 06 00 C4  1639  	AJMP IF_UGE,L396 
000066               1641  L407: 
000066               1643  L408: 
000066               1645  L409: 
000066  68 10 00 00  1646  	MOV DS0,#@_battery_count 
00006A  99 F0 00 00  1647  	MOVZE R0,DS0:_battery_count 
00006E  50 01        1648  	ADD R0,#1 
000070  68 10 00 00  1649  	MOV DS0,#@_battery_count 
000074               1650  .if KERNEL1_E = 1
+                    1651  	NOP
+                    1652  .endif
000074  A9 F1 00 00  1653  	MOVB DS0:_battery_count,Rb1 
000078               1655  L410: 
000078  68 10 00 00  1656  	MOV DS0,#@_battery_count 
00007C  99 F0 00 00  1657  	MOVZE R0,DS0:_battery_count 
000080  76 F0 00 14  1658  	CMP R0,#20 
000084  AC 09 00 DA  1659  	AJMP IF_SL,L397 
000088               1661  L411: 
000088               1663  L412: 
000088               1665  L413: 
000088  68 F1 00 00  1666  	MOV R1,#(L400) 
00008C  68 F0 00 00  1667  	MOV R0,#^(L400) 
000090  58 F1        1668  	PUSH R1 
000092  58 F0        1669  	PUSH R0 
000094               1670  .if KERNEL1_5 = 1
+                    1671  	CRIT #1
+                    1672  .endif
000094  BD 00 00 00  1673  	SCALL _SendString 
000098  50 F4        1674  	ADD SP,#4 
00009A               1676  L414: 
00009A  48 00        1677  	MOV R0,#0 
00009C  58 F0        1678  	PUSH R0 
00009E               1679  .if KERNEL1_5 = 1
+                    1680  	CRIT #1
+                    1681  .endif
00009E  BD 00 00 00  1682  	SCALL _clkup_time 
0000A2  50 F2        1683  	ADD SP,#2 
0000A4               1685  L415: 
+                    1686  	
0000A4  E6 57        1687  	BSET 0x57.6 
0000A6               1689  L416: 
+                    1690  	
0000A6  F7 57        1691  	BCLR 0x57.7 
0000A8               1693  L417: 
0000A8  48 10        1694  	MOV R1,#0 
0000AA  48 00        1695  	MOV R0,#0 
0000AC  AA F0 00 00  1696  	MOVX _sys_led_timer,R0 
0000B0  AA F1 00 02  1697  	MOVX _sys_led_timer+2,R1 
0000B4               1699  L418: 
0000B4  48 04        1700  	MOV R0,#4 
0000B6  68 10 00 00  1701  	MOV DS0,#@(_gflag) 
0000BA  B5 F0 00 00  1702  	OR DS0:(_gflag),R0 
0000BE               1704  L419: 
0000BE  48 01        1705  	MOV R0,#1 
0000C0  AC 00 00 DC  1706  	AJMP #0,L393 
0000C4               1708  L420: 
0000C4               1710  L421: 
0000C4               1712  L422: 
0000C4               1714  L423: 
0000C4               1715  L396: 
0000C4               1717  L424: 
0000C4               1719  L425: 
0000C4               1721  L426: 
0000C4  49 10        1722  	MOVB Rb1,#0 
0000C6  68 10 00 00  1723  	MOV DS0,#@_battery_count 
0000CA               1724  .if KERNEL1_E = 1
+                    1725  	NOP
+                    1726  .endif
0000CA  A9 F1 00 00  1727  	MOVB DS0:_battery_count,Rb1 
0000CE               1729  L427: 
0000CE  68 F0 FF FB  1730  	MOV R0,#0xfffb 
0000D2  68 10 00 00  1731  	MOV DS0,#@(_gflag) 
0000D6  B4 F0 00 00  1732  	AND DS0:(_gflag),R0 
0000DA               1734  L428: 
0000DA               1736  L429: 
0000DA               1737  L397: 
0000DA               1739  L430: 
0000DA  48 00        1740  	MOV R0,#0 
0000DC               1742  L431: 
0000DC               1744  L432: 
0000DC               1745  L393: 
0000DC  38 3F        1746  	POP R3 
0000DE  38 2F        1747  	POP R2 
0000E0  38 1F        1748  	POP R1 
0000E2               1749  	UNLINK_MACRO
0000E2  2F 00        1749  UNLINK 
+                    1749  
0000E4               1750  .if KERNEL1_5 = 1
+                    1751  	CRIT #1
+                    1752  .endif
0000E4  C6 20        1753  	RETS 
+                    1978  
+                    1979  .area _$SYSTEM_C$(ROM,REL) 
+B 00 01
000000               1981  Letext: 
+                    1982  
+                    1983  .area _SYSTEM_DATA$RD$flag_tishi(SYSTEM_DATA,REL,dat) 
+B 00 1A
000000               1984  _flag_tishi:: 
000000               1985  	.ds 1 
+                    1987  
+                    1988  .area _SYSTEM_DATA$RD$sleep_mode_count(SYSTEM_DATA,REL,dat) 
+                    1989  .even 
+B 00 1B
000000               1990  _sleep_mode_count:: 
000000               1991  	.ds 2 
+                    1993  
+                    1994  .area _SYSTEM_DATA$RD$speaker_vol(SYSTEM_DATA,REL,dat) 
+                    1995  .even 
+B 00 1C
000000               1996  _speaker_vol:: 
000000               1997  	.ds 2 
+                    1999  
+                    2000  .area _SYSTEM_DATA$RD$adc_timer(SYSTEM_DATA,REL,dat) 
+                    2001  .even 
+B 00 1D
000000               2002  _adc_timer:: 
000000               2003  	.ds 4 
+                    2005  
+                    2006  .area _SYSTEM_DATA$RD$adc_channel(SYSTEM_DATA,REL,dat) 
+B 00 1E
000000               2007  _adc_channel:: 
000000               2008  	.ds 1 
+                    2010  
+                    2011  .area _SYSTEM_DATA$RD$led_timer(SYSTEM_DATA,REL,dat) 
+                    2012  .even 
+B 00 1F
000000               2013  _led_timer:: 
000000               2014  	.ds 4 
+                    2016  
+                    2017  
+                    2018  
+                    2019  .area _XRAM$RD$g_autopoweroff_timer(XRAM,REL,dat) 
+                    2020  .even 
+B 00 20
000000               2021  _g_autopoweroff_timer:: 
000000               2022  	.ds 4 
+                    2024  
+                    2025  .area _XRAM$RD$g_b_poweroff_activated(XRAM,REL,dat) 
+B 00 21
000000               2026  _g_b_poweroff_activated:: 
000000               2027  	.ds 1 
+                    2029  
+                    2030  .area _XRAM$RD$g_b_autopoweroff_forbidden(XRAM,REL,dat) 
+B 00 22
000000               2031  _g_b_autopoweroff_forbidden:: 
000000               2032  	.ds 1 
+                    2034  
+                    2035  .area _XRAM$RD$Buf(XRAM,REL,dat) 
+B 00 23
000000               2036  _Buf:: 
000000               2037  	.ds 512 
+                    2040  
+                    2041  .area _XRAM$RD$Length(XRAM,REL,dat) 
+                    2042  .even 
+B 00 24
000000               2043  _Length:: 
000000               2044  	.ds 4 
+                    2046  
+                    2047  .area _XRAM$RD$Start_Sector(XRAM,REL,dat) 
+                    2048  .even 
+B 00 25
000000               2049  _Start_Sector:: 
000000               2050  	.ds 4 
+                    2052  
+                    2053  .area _SYSTEM_DATA$RD$gOidXOR_Key(SYSTEM_DATA,REL,dat) 
+B 00 26
000000               2054  _gOidXOR_Key:: 
000000               2055  	.ds 1 
+                    2057  
+                    2058  .area _SYSTEM_DATA$RD$play_file_size(SYSTEM_DATA,REL,dat) 
+                    2059  .even 
+B 00 27
000000               2060  _play_file_size:: 
000000               2061  	.ds 4 
+                    2063  
+                    2064  .area _SYSTEM_DATA$RD$mp3_begin_add(SYSTEM_DATA,REL,dat) 
+                    2065  .even 
+B 00 28
000000               2066  _mp3_begin_add:: 
000000               2067  	.ds 4 
+                    2069  
+                    2070  .area _SYSTEM_DATA$RD$ADPCM_File(SYSTEM_DATA,REL,dat) 
+                    2071  .even 
+B 00 29
000000               2072  _ADPCM_File:: 
000000               2073  	.ds 2 
+                    2075  
+                    2076  .area _SYSTEM_DATA$RD$flag_creat(SYSTEM_DATA,REL,dat) 
+B 00 2A
000000               2077  _flag_creat:: 
000000               2078  	.ds 1 
+                    2080  
+                    2081  .area _SYSTEM_DATA$RD$OID_PLAY(SYSTEM_DATA,REL,dat) 
+B 00 2B
000000               2082  _OID_PLAY:: 
000000               2083  	.ds 1 
+                    2085  
+                    2086  .area _SYSTEM_DATA$RD$low_num(SYSTEM_DATA,REL,dat) 
+                    2087  .even 
+B 00 2C
000000               2088  _low_num:: 
000000               2089  	.ds 2 
+                    2091  
+                    2092  .area _SYSTEM_DATA$RD$heigh_num(SYSTEM_DATA,REL,dat) 
+                    2093  .even 
+B 00 2D
000000               2094  _heigh_num:: 
000000               2095  	.ds 2 
+                    2097  
+                    2098  .area _SYSTEM_DATA$RD$system_check_timer(SYSTEM_DATA,REL,dat) 
+                    2099  .even 
+B 00 2E
000000               2100  _system_check_timer:: 
000000               2101  	.ds 4 
+                    2103  
+                    2104  .area _SYSTEM_DATA$RD$DAC_Out(SYSTEM_DATA,REL,dat) 
+                    2105  .even 
+B 00 2F
000000               2106  _DAC_Out:: 
000000               2107  	.ds 2 
+                    2109  
+                    2110  .area _SYSTEM_DATA$RD$flag_wav_play(SYSTEM_DATA,REL,dat) 
+B 00 30
000000               2111  _flag_wav_play:: 
000000               2112  	.ds 1 
+                    2114  
+                    2115  .area _SYSTEM_DATA$RD$battery_count(SYSTEM_DATA,REL,dat) 
+B 00 31
000000               2116  _battery_count:: 
000000               2117  	.ds 1 
+                    2119  
+                    2120  .area _SYSTEM_DATA$RD$vol_idx_bak(SYSTEM_DATA,REL,dat) 
+                    2121  .even 
+B 00 32
000000               2122  _vol_idx_bak:: 
000000               2123  	.ds 2 
+                    2125  
+                    2126  .area _SYSTEM_DATA$RD$g_timer_tick(SYSTEM_DATA,REL,dat) 
+                    2127  .even 
+B 00 33
000000               2128  _g_timer_tick:: 
000000               2129  	.ds 4 
+                    2131  
+                    2132  .area _SYSTEM_DATA$RD$g_system_clk_timer(SYSTEM_DATA,REL,dat) 
+                    2133  .even 
+B 00 34
000000               2134  _g_system_clk_timer:: 
000000               2135  	.ds 4 
+                    2137  
+                    2138  .area _SYSTEM_DATA$RD$sleep_mode_timer(SYSTEM_DATA,REL,dat) 
+                    2139  .even 
+B 00 35
000000               2140  _sleep_mode_timer:: 
000000               2141  	.ds 4 
+                    2143  
+                    2144  .area _SYSTEM_DATA$RD$qieqing_timer(SYSTEM_DATA,REL,dat) 
+                    2145  .even 
+B 00 36
000000               2146  _qieqing_timer:: 
000000               2147  	.ds 4 
+                    2149  
+                    2150  .area _XRAM$RD$op_close_timer(XRAM,REL,dat) 
+                    2151  .even 
+B 00 37
000000               2152  _op_close_timer:: 
000000               2153  	.ds 4 
+                    2155  
+                    2156  .area _XRAM$RD$sys_led_timer(XRAM,REL,dat) 
+                    2157  .even 
+B 00 38
000000               2158  _sys_led_timer:: 
000000               2159  	.ds 4 
+                    2161  
+                    2162  .area _XRAM$RD$rec_led_timer(XRAM,REL,dat) 
+                    2163  .even 
+B 00 39
000000               2164  _rec_led_timer:: 
000000               2165  	.ds 4 
+                    2167  
+                    2168  
+                    2169  
+                    2170  
+                    2171  
+                    2172  
+                    2173  
+                    2174  
+                    2175  
+                    2176  
+                    2177  
+                    2178  
+                    2179  
+                    2180  
+                    2181  
+                    2182  
+                    2183  
+                    2184  
+                    2185  
+                    2186  
+                    2187  .area _BATTERY_CODE$RD$SYSTEM_C$(BATTERY_CODE,REL) 
+B 00 3A
000000               2188  L400: 
000000  0D           2189  	.db 13 
000001  0A           2190  	.db 10 
+                    2191  	
000002  6C 6F 77 20  2192  	.db 108,111,119,32,98,97,116,116,101,114,121,33,0 
000006  62 61 74 74
00000A  65 72 79 21
00000E  00
+                    2193  
+                    2194  .area _SYSTEM_CODE$RD$SYSTEM_C$(SYSTEM_CODE,REL) 
+B 00 3B
000000               2195  L331: 
000000  0D           2196  	.db 13 
000001  0A           2197  	.db 10 
+                    2198  	
000002  73 79 73 20  2199  	.db 115,121,115,32,108,111,119,33,0 
000006  6C 6F 77 21
00000A  00
+                    2200  
+                    2201  .area _SYSTEM_CODE$RD$SYSTEM_C$(SYSTEM_CODE,REL) 
+B 00 3B
00000C               2202  L313: 
00000C  0D           2203  	.db 13 
00000D  0A           2204  	.db 10 
+                    2205  	
00000E  73 79 73 20  2206  	.db 115,121,115,32,117,112,33,0 
000012  75 70 21 00
+                    2207  
+                    2208  .area _SYSTEM_CODE$RD$SYSTEM_C$(SYSTEM_CODE,REL) 
+B 00 3B
000016               2209  L278: 
000016  0D           2210  	.db 13 
000017  0A           2211  	.db 10 
+                    2212  	
000018  66 69 6E 64  2213  	.db 102,105,110,100,32,117,115,98,33,0 
00001C  20 75 73 62
000020  21 00
+                    2214  
+                    2215  .area _SYSTEM_CODE$RD$SYSTEM_C$(SYSTEM_CODE,REL) 
+B 00 3B
000022               2216  L45: 
000022  0D           2217  	.db 13 
000023  0A           2218  	.db 10 
+                    2219  	
000024  73 79 73 74  2220  	.db 115,121,115,116,101,109,32,114,101,115,101,116,33,0 
000028  65 6D 20 72
00002C  65 73 65 74
000030  21 00
+                    2221  
+                    2222  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+  8 L100  000056 R
+  9 L101  0000FE R
+  9 L102  000028 R
+  9 L104  00007C R
+  9 L106  0000A4 R
+  9 L108  0000CC R
+  9 L110  0000F4 R
+  9 L112  000006 R
+  9 L113  000006 R
+  9 L114  000006 R
+  9 L115  000024 R
+  9 L116  000024 R
+  9 L117  000024 R
+  9 L118  000028 R
+  9 L119  000028 R
+  9 L120  000028 R
+  9 L121  000040 R
+  9 L122  000054 R
+  9 L123  000062 R
+  9 L124  000062 R
+  9 L125  000062 R
+  9 L126  00006E R
+  9 L127  000078 R
+  9 L128  00007C R
+  9 L129  00007C R
+  9 L130  00007C R
+  9 L131  00008A R
+  9 L132  00008A R
+  9 L133  00008A R
+  9 L134  000096 R
+  9 L135  0000A0 R
+  9 L136  0000A4 R
+  9 L137  0000A4 R
+  9 L138  0000A4 R
+  9 L139  0000B2 R
+  9 L140  0000B2 R
+  9 L141  0000B2 R
+  9 L142  0000BE R
+  9 L143  0000C8 R
+  9 L144  0000CC R
+  9 L145  0000CC R
+  9 L146  0000CC R
+  9 L147  0000DA R
+  9 L148  0000DA R
+  9 L149  0000DA R
+  9 L150  0000E6 R
+  9 L151  0000F0 R
+  9 L152  0000F4 R
+  9 L153  0000F4 R
+  9 L154  0000F4 R
+  9 L155  0000FE R
+  9 L156  0000FE R
+  A L157  000046 R
+  A L158  000046 R
+  A L160  000006 R
+  A L161  000006 R
+  A L162  000006 R
+  A L163  000038 R
+  A L164  000038 R
+  A L165  000038 R
+  A L166  000044 R
+  A L167  000046 R
+  A L168  000046 R
+  A L169  000046 R
+  A L170  000046 R
+  B L171  000050 R
+  B L172  000050 R
+  B L174  000006 R
+  B L175  000006 R
+  B L176  000006 R
+  B L177  00003C R
+  B L178  00003C R
+  B L179  00003C R
+  B L180  00004C R
+  B L181  000050 R
+  B L182  000050 R
+  B L183  000050 R
+  B L184  000050 R
+  C L185  000002 R
+  C L186  000002 R
+  C L187  000002 R
+  C L188  000002 R
+  C L189  000002 R
+  C L190  000002 R
+  D L191  000002 R
+  D L192  000002 R
+  D L193  000002 R
+  D L194  000002 R
+  D L195  000002 R
+  D L196  000002 R
+  E L197  000028 R
+  E L198  000028 R
+  E L200  000006 R
+  E L201  000006 R
+  E L202  000006 R
+  E L203  00001E R
+  E L204  00001E R
+  E L205  00001E R
+  E L206  000028 R
+  E L207  000028 R
+  E L208  000028 R
+  E L209  000028 R
+  F L210  00007C R
+  F L211  000054 R
+  F L212  00007C R
+  F L213  000072 R
+  F L214  00007C R
+  F L215  000006 R
+  F L216  000006 R
+  F L217  00000E R
+  F L218  000032 R
+  F L219  000044 R
+  F L220  000044 R
+  F L221  000044 R
+  F L222  000046 R
+  F L223  000050 R
+  F L224  000050 R
+  F L225  000054 R
+  F L226  000054 R
+  F L227  000054 R
+  F L228  000056 R
+  F L229  000064 R
+  F L230  000064 R
+  F L231  000064 R
+  F L232  00006E R
+  F L233  00006E R
+  F L234  000072 R
+  F L235  000072 R
+  F L236  000072 R
+  F L237  00007C R
+  F L238  00007C R
+  F L239  00007C R
+  F L240  00007C R
+  F L241  00007C R
+  F L242  00007C R
+ 10 L243  0000A4 R
+ 10 L244  0000A4 R
+ 10 L246  000068 R
+ 10 L247  000096 R
+ 10 L248  00000A R
+ 10 L249  00000A R
+ 10 L250  000012 R
+ 10 L251  000016 R
+ 10 L252  00002A R
+ 10 L253  00002A R
+ 10 L254  00002A R
+ 10 L255  00003A R
+ 10 L256  00003E R
+ 10 L257  000042 R
+ 10 L258  000054 R
+ 10 L259  000054 R
+ 10 L260  000054 R
+ 10 L261  000064 R
+ 10 L262  000064 R
+ 10 L263  000068 R
+ 10 L264  000068 R
+ 10 L265  000068 R
+ 10 L266  000096 R
+ 10 L267  000096 R
+ 10 L268  000096 R
+ 10 L269  0000A4 R
+ 10 L270  0000A4 R
+ 10 L271  0000A4 R
+ 10 L272  0000A4 R
+ 11 L273  00005C R
+ 11 L274  00001C R
+ 11 L276  00005A R
+ 3B L278  000016 R
+ 11 L279  000004 R
+ 11 L280  000004 R
+ 11 L281  000004 R
+ 11 L282  000016 R
+ 11 L283  00001C R
+ 11 L284  00002C R
+ 11 L285  00002C R
+ 11 L286  00002C R
+ 11 L287  000036 R
+ 11 L288  000048 R
+ 11 L289  000054 R
+ 11 L290  00005A R
+ 11 L291  00005A R
+ 11 L292  00005A R
+ 11 L293  00005C R
+ 11 L294  00005C R
+ 12 L295  00000A R
+ 12 L296  000004 R
+ 12 L297  000004 R
+ 12 L298  000004 R
+ 12 L299  00000A R
+ 12 L300  00000A R
+ 13 L301  00003A R
+ 13 L302  000006 R
+ 13 L303  000006 R
+ 13 L304  000006 R
+ 13 L305  00000C R
+ 13 L306  000022 R
+ 13 L307  00002E R
+ 13 L308  00003A R
+ 13 L309  00003A R
+ 14 L310  000054 R
+ 14 L311  000054 R
+ 3B L313  00000C R
+ 14 L314  000006 R
+ 14 L315  000006 R
+ 14 L316  000006 R
+ 14 L317  000018 R
+ 14 L318  000018 R
+ 14 L319  000018 R
+ 14 L320  000022 R
+ 14 L321  00002C R
+ 14 L322  000036 R
+ 14 L323  000042 R
+ 14 L324  000054 R
+ 14 L325  000054 R
+ 14 L326  000054 R
+ 14 L327  000054 R
+ 15 L328  000048 R
+ 15 L329  000048 R
+ 3B L331  000000 R
+ 15 L332  000006 R
+ 15 L333  000006 R
+ 15 L334  000006 R
+ 15 L335  000018 R
+ 15 L336  000018 R
+ 15 L337  000018 R
+ 15 L338  00002A R
+ 15 L339  000034 R
+ 15 L340  00003C R
+ 15 L341  000048 R
+ 15 L342  000048 R
+ 15 L343  000048 R
+ 15 L344  000048 R
+ 16 L345  00004C R
+ 16 L346  00003C R
+ 16 L347  00004C R
+ 16 L348  000006 R
+ 16 L349  000006 R
+ 16 L350  00000E R
+ 16 L351  000012 R
+ 16 L352  00001E R
+ 16 L353  00001E R
+ 16 L354  00001E R
+ 16 L355  000038 R
+ 16 L356  000038 R
+ 16 L357  00003C R
+ 16 L358  00003C R
+ 16 L359  00003C R
+ 16 L360  00004C R
+ 16 L361  00004C R
+ 16 L362  00004C R
+ 16 L363  00004C R
+ 17 L364  000044 R
+ 17 L365  000006 R
+ 17 L366  000006 R
+ 17 L367  000006 R
+ 17 L368  00001E R
+ 17 L369  000026 R
+ 17 L370  00002E R
+ 17 L371  000034 R
+ 17 L372  000044 R
+ 17 L373  000044 R
+ 18 L374  0000D0 R
+ 18 L375  000006 R
+ 18 L376  000006 R
+ 18 L377  000006 R
+ 18 L378  00000A R
+ 18 L379  00002E R
+ 18 L380  000038 R
+ 18 L381  000042 R
+ 18 L382  000052 R
+ 18 L383  00005E R
+ 18 L384  000074 R
+ 18 L385  00007E R
+ 18 L386  00008A R
+ 18 L387  000096 R
+ 18 L388  0000AC R
+ 18 L389  0000B6 R
+ 18 L390  0000C0 R
+ 18 L391  0000D0 R
+ 18 L392  0000D0 R
+ 19 L393  0000DC R
+ 19 L394  00001E R
+ 19 L396  0000C4 R
+ 19 L397  0000DA R
+ 3A L400  000000 R
+ 19 L401  000008 R
+ 19 L402  000008 R
+ 19 L403  000008 R
+ 19 L404  000018 R
+ 19 L405  00001E R
+ 19 L406  00005A R
+ 19 L407  000066 R
+ 19 L408  000066 R
+ 19 L409  000066 R
+ 19 L410  000078 R
+ 19 L411  000088 R
+ 19 L412  000088 R
+ 19 L413  000088 R
+ 19 L414  00009A R
+ 19 L415  0000A4 R
+ 19 L416  0000A6 R
+ 19 L417  0000A8 R
+ 19 L418  0000B4 R
+ 19 L419  0000BE R
+ 19 L420  0000C4 R
+ 19 L421  0000C4 R
+ 19 L422  0000C4 R
+ 19 L423  0000C4 R
+ 19 L424  0000C4 R
+ 19 L425  0000C4 R
+ 19 L426  0000C4 R
+ 19 L427  0000CE R
+ 19 L428  0000DA R
+ 19 L429  0000DA R
+ 19 L430  0000DA R
+ 19 L431  0000DC R
+ 19 L432  0000DC R
+ 3B L45  000022 R
+  4 L46  000026 R
+  4 L47  000026 R
+  4 L49  000006 R
+  4 L50  000006 R
+  4 L51  000006 R
+  4 L52  000018 R
+  4 L53  00001A R
+  4 L54  00001C R
+  4 L55  00001E R
+  4 L56  000026 R
+  4 L57  000026 R
+  4 L58  000026 R
+  4 L59  000026 R
+  4 L60  000026 R
+  4 L61  000026 R
+  4 L62  00002A R
+  4 L63  00002A R
+  5 L64  000048 R
+  5 L65  000044 R
+  5 L66  000044 R
+  5 L67  000044 R
+  5 L68  000048 R
+  5 L69  000048 R
+  6 L70  00001E R
+  6 L71  000002 R
+  6 L72  000002 R
+  6 L73  000002 R
+  6 L74  000016 R
+  6 L75  00001E R
+  6 L76  00001E R
+  7 L77  000024 R
+  7 L78  000022 R
+  7 L80  000008 R
+  7 L81  000008 R
+  7 L82  000008 R
+  7 L83  00001C R
+  7 L84  00001C R
+  7 L85  00001C R
+  7 L86  000022 R
+  7 L87  000022 R
+  7 L88  000022 R
+  7 L89  000022 R
+  7 L90  000022 R
+  7 L91  000024 R
+  7 L92  000024 R
+  7 L93  000024 R
+  7 L94  000024 R
+  8 L95  000056 R
+  8 L96  000046 R
+  8 L97  000046 R
+  8 L98  000046 R
+  8 L99  000056 R
+    LIB_PRE_PRPCESS =000000 
+  1 Letext  000000 R
+  1 Ltext0  000000 R
+    R0 =0007D0 
+    R1 =0007D1 
+    R2 =0007D2 
+    R3 =0007D3 
+    R4 =0007D4 
+    R5 =0007D5 
+    R6 =0007D6 
+    R7 =0007D7 
+    Rb0 =0003E8 
+    Rb1 =0003E9 
+    Rb10 =0003F2 
+    Rb11 =0003F3 
+    Rb12 =0003F4 
+    Rb13 =0003F5 
+    Rb14 =0003F6 
+    Rb15 =0003F7 
+    Rb2 =0003EA 
+    Rb3 =0003EB 
+    Rb4 =0003EC 
+    Rb5 =0003ED 
+    Rb6 =0003EE 
+    Rb7 =0003EF 
+    Rb8 =0003F0 
+    Rb9 =0003F1 
+    SPIBOOT_KBDM9 v1.06  000001 
+ 29 _ADPCM_File  000000 GR
+ 19 _BatteryDetect  000000 GR
+ 23 _Buf  000000 GR
+ 11 _CheckUSB  000000 GR
+    _ClrCirqAndAdjustCpri  ****** GX
+    _DAC_Disable_Speaker  ****** GX
+    _DAC_Enable_HeadPhone  ****** GX
+    _DAC_HeadPhone_Volume  ****** GX
+ 2F _DAC_Out  000000 GR
+    _DAC_Speaker_Volume  ****** GX
+ 13 _Delay_ms  000042 GR
+  5 _Exception  000000 GR
+    _Get_ADCValue  ****** GX
+ 24 _Length  000000 GR
+ 2B _OID_PLAY  000000 GR
+    _RTCWDT  ****** GX
+    _SPIF_Set_Clock  ****** GX
+    _SendString  ****** GX
+    _SetVector  ****** GX
+ 10 _SpeakDetect  000000 GR
+  3 _SpeakerTbl  000000 GR
+ 25 _Start_Sector  000000 GR
+ 15 _SysClkDowngrade  000000 GR
+ 14 _SysClkUpgrade  000000 GR
+    _SystemClock  ****** GX
+ 18 _System_Timer3_Init  000000 GR
+  8 _TIMER0_ISR  000000 GR
+    _TM3ICR  ****** GX
+    _TM3PRD  ****** GX
+    _TMR3CR  ****** GX
+  2 _VolTbl  000000 GR
+    __DIVulong  ****** GX
+    __MULulong  ****** GX
+    _adc0_value  ****** GX
+    _adc2_value  ****** GX
+    _adc3_value  ****** GX
+    _adc7_value  ****** GX
+ 1E _adc_channel  000000 GR
+  9 _adc_get  000000 GR
+ 1D _adc_timer  000000 GR
+ 31 _battery_count  000000 GR
+  B _clk_time_dowith  000000 GR
+ 16 _clkup_time  000000 GR
+ 2A _flag_creat  000000 GR
+ 1A _flag_tishi  000000 GR
+ 30 _flag_wav_play  000000 GR
+ 26 _gOidXOR_Key  000000 GR
+ 20 _g_autopoweroff_timer  000000 GR
+ 22 _g_b_autopoweroff_forbidden  000000 GR
+ 21 _g_b_poweroff_activated  000000 GR
+ 34 _g_system_clk_timer  000000 GR
+ 33 _g_timer_tick  000000 GR
+    _gflag  ****** GX
+ 2D _heigh_num  000000 GR
+  F _inc_volume  000000 GR
+  E _judge_op  000000 GR
+ 1F _led_timer  000000 GR
+ 2C _low_num  000000 GR
+ 28 _mp3_begin_add  000000 GR
+ 37 _op_close_timer  000000 GR
+  A _op_time_dowith  000000 GR
+ 27 _play_file_size  000000 GR
+ 13 _power_authorize_autopoweroff  000000 GR
+ 12 _power_forbid_autopoweroff  000000 GR
+ 36 _qieqing_timer  000000 GR
+ 39 _rec_led_timer  000000 GR
+ 1B _sleep_mode_count  000000 GR
+ 35 _sleep_mode_timer  000000 GR
+ 1C _speaker_vol  000000 GR
+    _sys_flag  ****** GX
+ 38 _sys_led_timer  000000 GR
+ 2E _system_check_timer  000000 GR
+  4 _system_reset  000000 GR
+    _systerm_flag  ****** GX
+ 17 _time3_init  000000 GR
+  7 _timer_check_timeout  000000 GR
+  6 _timer_get_time  000000 GR
+  C _vol_dec  000000 GR
+    _vol_idx  ****** GX
+ 32 _vol_idx_bak  000000 GR
+  D _vol_inc  000000 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$SYSTEM_C$ size    0   flags 0
+   2 _SYSTEM_CODE$RD$VolTbl size    A   flags 0
+   3 _SYSTEM_CODE$RD$SpeakerTbl size    A   flags 0
+   4 _$system_reset size   32   flags 0
+   5 _$Exception size   7A   flags 0
+   6 _$timer_get_time size   22   flags 0
+   7 _$timer_check_timeout size   2E   flags 0
+   8 _$TIMER0_ISR size   8A   flags 0
+   9 _$adc_get size  106   flags 0
+   A _$op_time_dowith size   4E   flags 0
+   B _$clk_time_dowith size   58   flags 0
+   C _$vol_dec size    6   flags 0
+   D _$vol_inc size    6   flags 0
+   E _$judge_op size   30   flags 0
+   F _$inc_volume size   84   flags 0
+  10 _$SpeakDetect size   B0   flags 0
+  11 _$CheckUSB size   62   flags 0
+  12 _$power_forbid_autopoweroff size   10   flags 0
+  13 _$power_authorize_autopoweroff size   64   flags 0
+  14 _$SysClkUpgrade size   5C   flags 0
+  15 _$SysClkDowngrade size   50   flags 0
+  16 _$clkup_time size   54   flags 0
+  17 _$time3_init size   4C   flags 0
+  18 _$System_Timer3_Init size   D8   flags 0
+  19 _$BatteryDetect size   E6   flags 0
+  1A _SYSTEM_DATA$RD$flag_tishi size    2   flags 0
+  1B _SYSTEM_DATA$RD$sleep_mode_count size    2   flags 0
+  1C _SYSTEM_DATA$RD$speaker_vol size    2   flags 0
+  1D _SYSTEM_DATA$RD$adc_timer size    4   flags 0
+  1E _SYSTEM_DATA$RD$adc_channel size    2   flags 0
+  1F _SYSTEM_DATA$RD$led_timer size    4   flags 0
+  20 _XRAM$RD$g_autopoweroff_timer size    4   flags 0
+  21 _XRAM$RD$g_b_poweroff_activated size    2   flags 0
+  22 _XRAM$RD$g_b_autopoweroff_forbidden size    2   flags 0
+  23 _XRAM$RD$Buf size  200   flags 0
+  24 _XRAM$RD$Length size    4   flags 0
+  25 _XRAM$RD$Start_Sector size    4   flags 0
+  26 _SYSTEM_DATA$RD$gOidXOR_Key size    2   flags 0
+  27 _SYSTEM_DATA$RD$play_file_size size    4   flags 0
+  28 _SYSTEM_DATA$RD$mp3_begin_add size    4   flags 0
+  29 _SYSTEM_DATA$RD$ADPCM_File size    2   flags 0
+  2A _SYSTEM_DATA$RD$flag_creat size    2   flags 0
+  2B _SYSTEM_DATA$RD$OID_PLAY size    2   flags 0
+  2C _SYSTEM_DATA$RD$low_num size    2   flags 0
+  2D _SYSTEM_DATA$RD$heigh_num size    2   flags 0
+  2E _SYSTEM_DATA$RD$system_check_timer size    4   flags 0
+  2F _SYSTEM_DATA$RD$DAC_Out size    2   flags 0
+  30 _SYSTEM_DATA$RD$flag_wav_play size    2   flags 0
+  31 _SYSTEM_DATA$RD$battery_count size    2   flags 0
+  32 _SYSTEM_DATA$RD$vol_idx_bak size    2   flags 0
+  33 _SYSTEM_DATA$RD$g_timer_tick size    4   flags 0
+  34 _SYSTEM_DATA$RD$g_system_clk_timer size    4   flags 0
+  35 _SYSTEM_DATA$RD$sleep_mode_timer size    4   flags 0
+  36 _SYSTEM_DATA$RD$qieqing_timer size    4   flags 0
+  37 _XRAM$RD$op_close_timer size    4   flags 0
+  38 _XRAM$RD$sys_led_timer size    4   flags 0
+  39 _XRAM$RD$rec_led_timer size    4   flags 0
+  3A _BATTERY_CODE$RD$SYSTEM_C$ size   10   flags 0
+  3B _SYSTEM_CODE$RD$SYSTEM_C$ size   32   flags 0
