+                       1  
+                       5  
+                       6  .area _$USB_MassStorage_config_c$(ROM,REL) 
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
+                      58  .area _$__usbtimer(USB_MS_CODE,REL) 
+B 00 02
000000                 60  ___usbtimer:: 
000000  2E 00          62  	LINK #0 
000002                 63  	SAVE$ISR
000002  58 F0          63  PUSH R0 
000004  1F 09          63  PUSH RC 
000006  1F 0A          63  PUSH  SRC 
000008  68 09 00 00    63  MOV RC, #0 
00000C  68 0A 00 00    63  MOV SRC, #0 
000010  1F 01          63  PUSH  PSR 
000012  1F 00          63  PUSH  SOCR 
000014  1F 0C          63  PUSH  RE 
000016  1F 0B          63  PUSH  RS 
+                      63                   
000018  1F 13          63  PUSH XM0 
00001A  1F 19          63  PUSH YM0 
00001C  1F 02          63  PUSH SB 
00001E  1F 10          63  PUSH DS0 
000020  1F 11          63  PUSH DS1 
000022  58 FE          63  PUSH Y2 
000024  58 F8          63  PUSH X0 
000026  68 13 00 00    63  MOV  XM0, #0 
00002A  68 19 00 00    63  MOV  YM0, #0 
00002E  58 F2          63  PUSH R2 
000030  58 F3          63  PUSH R3 
000032  BA F0 F2 DC    63  MOVX  R0,ES0 
000036  58 F0          63  PUSH  R0 
000038  BA F0 F2 DE    63  MOVX  R0,ES1 
00003C  58 F0          63  PUSH  R0 
00003E  BD 00 00 00    63  SCALL _ClrCirqAndAdjustCpri 
+                      63          
+                      63  
000042  58 F0          64  	PUSH R0 
000044                 66  L36: 
000044                 68  L37: 
000044                 70  L38: 
000044  72 9F 40 00    71   SUB TMR2CR , #0x4000  
000048                 73  L39: 
000048                 75  L40: 
000048                 76  L35: 
000048  38 0F          77  	POP R0 
00004A                 78  	RESTORE$ISR
00004A  38 0F          78  POP   R0 
00004C  BA EF F2 E0    78  MOVX  ES1,R0 
000050  38 0F          78  POP   R0 
000052  BA EE F2 E0    78  MOVX  ES0,R0 
000056  38 3F          78  POP  R3 
000058  38 2F          78  POP  R2 
00005A  38 8F          78  POP  X0 
00005C  38 EF          78  POP Y2 
00005E  1F 91          78  POP DS1 
000060  1F 90          78  POP DS0 
000062  1F 82          78  POP SB 
000064  1F 99          78  POP YM0 
000066  1F 93          78  POP XM0 
+                      78          
000068  1F 8B          78  POP   RS 
00006A  1F 8C          78  POP   RE 
00006C  1F 80          78  POP   SOCR 
+                      78     
00006E  1F 81          78  POP   PSR 
000070  1F 8A          78  POP   SRC 
000072  1F 89          78  POP RC 
000074  38 0F          78  POP R0 
+                      78  
000076                 79  	UNLINK_MACRO
000076  2F 00          79  UNLINK 
+                      79  
000078                 80  .if KERNEL1_5 = 1
+                      81  	CRIT #1
+                      82  .endif
000078  C6 10          83  	RETI 
+                      85  
+                      86  .area _$USBWriteCommit(USB_MS_CODE,REL) 
+B 00 03
000000                 87  _USBWriteCommit:: 
000000  2E 01          89  	LINK #1 
000002  58 F0          90  	PUSH R0 
000004  58 F1          91  	PUSH R1 
000006  58 F2          92  	PUSH R2 
000008  58 F3          93  	PUSH R3 
00000A                 95  L47: 
00000A                 97  L48: 
00000A                100  L49: 
00000A  48 11         101  	MOV R1,#1 
00000C  6A 1B FF FE   102  	MOV [FP+#(-2)],R1 
000010                103  L42: 
000010                105  L50: 
000010  78 1B FF FE   106  	MOV R1,[FP+#(-2)] 
000014  52 11         107  	SUB R1,#1 
000016  6A 1B FF FE   108  	MOV [FP+#(-2)],R1 
00001A  68 F3 00 26   109  	MOV R3,#38 
00001E  48 20         110  	MOV R2,#0 
000020                111  	MOVSE42 R0
000020                111  .if R1=R1
+                     111  .else
+                     111  MOV R1,R1
+                     111  .endif
000020  68 F0 FF FF   111  MOV R0,#0xFFFF 
000024  56 10         111  CMP R1,#0 
000026  D9 01         111  RJMP IF_SL,(.+4)   
000028  48 00         111  MOV R0,#0 
+                     111  
+                     111  
00002A                112  	MUL$long R2
00002A  58 F1         112  PUSH R1 
00002C  58 F0         112  PUSH R0 
00002E  58 F3         112  PUSH R3 
000030  58 F2         112  PUSH R2 
000032                112  .if KERNEL1_5 = 1
+                     112  CRIT #1
+                     112  .endif
000032  BD 00 00 00   112  SCALL __MULlong 
000036  50 F8         112  ADD SP,#8 
+                     112  
000038  68 F3 00 14   113  	MOV R3,#(_usb_stor+20) 
00003C  68 F2 00 14   114  	MOV R2,#^(_usb_stor+20) 
000040  10 13         115  	ADD R1,R3 
000042  11 02         116  	ADDC R0,R2 
000044                117  	LOAD4$indirect R0
000044                117  .if R0=R1
+                     117  +++ compiler error ++++
+                     117  .else
000044  4C 11         117  SL R1,#1 
000046  5C 01         117  SLOSB R0,#1 
000048  BA 10 F2 E0   117  MOVX DS0,R0 
00004C  4D 11         117  SR R1,#1 
00004E  38 01         117  MOV R0,[R1++] 
000050  28 11         117  MOV R1,[R1] 
+                     117  .endif
+                     117  
000052  68 FE 00 64   118  	MOV Y2,#(L51) 
000056  58 FE         119  	PUSH Y2 
000058  68 FE 00 64   120  	MOV Y2,#^(L51) 
00005C  58 FE         121  	PUSH Y2 
00005E  58 F1         122  	PUSH R1 
000060  58 F0         123  	PUSH R0 
000062                124  .if KERNEL1_5 = 1
+                     125  	CRIT #1
+                     126  .endif
000062  C6 20         127  	RETS 
000064                128  L51: 
000064                129  L43: 
000064                131  L52: 
000064                133  L53: 
000064  78 0B FF FE   134  	MOV R0,[FP+#(-2)] 
000068  56 00         135  	CMP R0,#0 
00006A  AC 02 00 10   136  	AJMP IF_NE,L42 
00006E                138  L54: 
00006E  FE 9F         139   BCLR TMR2CR .14  
000070                141  L55: 
000070                143  L56: 
000070                144  L41: 
000070  38 3F         145  	POP R3 
000072  38 2F         146  	POP R2 
000074  38 1F         147  	POP R1 
000076  38 0F         148  	POP R0 
000078                149  	UNLINK_MACRO
000078  2F 00         149  UNLINK 
+                     149  
00007A                150  .if KERNEL1_5 = 1
+                     151  	CRIT #1
+                     152  .endif
00007A  C6 20         153  	RETS 
+                     155  
+                     156  .area _$MassStorage_Write(USB_MS_CODE,REL) 
+B 00 04
000000                157  _MassStorage_Write:: 
000000  2E 00         159  	LINK #0 
000002                161  L58: 
000002                163  L59: 
000002                165  L60: 
000002  58 F0 BA F0   166   PUSH R0  MOVX R0, TMR2CR   MOV  TMR2CR , #0  BSET R0.15  MOVX TMR2CR , R0  POP R0  
000006  F2 3E 68 9F
00000A  00 00 EF F0
00000E  BA 9F F2 E0
000012  38 0F
000014                168  L61: 
000014                170  L62: 
000014                171  L57: 
000014                172  	UNLINK_MACRO
000014  2F 00         172  UNLINK 
+                     172  
000016                173  .if KERNEL1_5 = 1
+                     174  	CRIT #1
+                     175  .endif
000016  C6 20         176  	RETS 
+                     178  
+                     179  .area _$MassStorage_Registers(USB_MS_CODE,REL) 
+B 00 05
000000                180  _MassStorage_Registers:: 
000000  2E 01         182  	LINK #1 
000002  58 F0         183  	PUSH R0 
000004  58 F1         184  	PUSH R1 
000006                186  L64: 
000006                188  L65: 
000006                191  L66: 
000006  48 00         192  	MOV R0,#0 
000008  6A 0B FF FE   193  	MOV [FP+#(-2)],R0 
00000C                195  L67: 
00000C                196  .if KERNEL1_5 = 1
+                     197  	CRIT #1
+                     198  .endif
00000C  BD 00 00 00   199  	SCALL _USBCoreInit 
000010                201  L68: 
000010  48 06         202  	MOV R0,#6 
000012  58 F0         203  	PUSH R0 
000014  48 01         204  	MOV R0,#1 
000016  58 F0         205  	PUSH R0 
000018                206  .if KERNEL1_5 = 1
+                     207  	CRIT #1
+                     208  .endif
000018  BD 00 00 00   209  	SCALL _USBStorageInit 
00001C  50 F4         210  	ADD SP,#4 
00001E                212  L69: 
00001E  78 0B FF FE   213  	MOV R0,[FP+#(-2)] 
000022  08 10         214  	MOV R1,R0 
000024  50 11         215  	ADD R1,#1 
000026  6A 1B FF FE   216  	MOV [FP+#(-2)],R1 
00002A  58 F0         217  	PUSH R0 
00002C                218  .if KERNEL1_5 = 1
+                     219  	CRIT #1
+                     220  .endif
00002C  BD 00 00 00   221  	SCALL _MassStorage_SD1_Register 
000030  50 F2         222  	ADD SP,#2 
000032                224  L70: 
000032                226  L71: 
000032                227  L63: 
000032  38 1F         228  	POP R1 
000034  38 0F         229  	POP R0 
000036                230  	UNLINK_MACRO
000036  2F 00         230  UNLINK 
+                     230  
000038                231  .if KERNEL1_5 = 1
+                     232  	CRIT #1
+                     233  .endif
000038  C6 20         234  	RETS 
+                     236  
+                     237  .area _$MassStorage_Loop(USB_MS_CODE,REL) 
+B 00 06
000000                238  _MassStorage_Loop:: 
000000  2E 01         240  	LINK #1 
000002  58 F1         241  	PUSH R1 
000004                243  L86: 
000004                245  L87: 
000004                248  L88: 
000004                249  .if KERNEL1_5 = 1
+                     250  	CRIT #1
+                     251  .endif
000004  BD 00 00 00   252  	SCALL _MassStorage_SD1_Loop 
000008                254  L89: 
000008  BA F0 00 00   255  	MOVX R0,_TMR2CR 
00000C  74 F0 40 00   256  	AND R0,#16384 
000010  56 00         257  	CMP R0,#0 
000012  AC 01 00 1A   258  	AJMP IF_EQ,L73 
000016                260  L90: 
000016                261  .if KERNEL1_5 = 1
+                     262  	CRIT #1
+                     263  .endif
000016  BD 00 00 00   264  	SCALL _USBWriteCommit 
00001A                265  L73: 
00001A                267  L91: 
00001A                268  .if KERNEL1_5 = 1
+                     269  	CRIT #1
+                     270  .endif
00001A  BD 00 00 00   271  	SCALL _USBRemoveCheck 
00001E  56 00         272  	CMP R0,#0 
000020  AC 01 00 2A   273  	AJMP IF_EQ,L75 
000024                275  L92: 
000024  48 01         276  	MOV R0,#1 
000026  AC 00 00 D0   277  	AJMP #0,L72 
00002A                278  L75: 
00002A                280  L93: 
00002A                281  .if KERNEL1_5 = 1
+                     282  	CRIT #1
+                     283  .endif
00002A  BD 00 00 00   284  	SCALL _USBCoreLoop 
00002E  6A 0B FF FE   285  	MOV [FP+#(-2)],R0 
000032                287  L94: 
000032  68 10 00 00   288  	MOV DS0,#@_b_usb_detection 
000036  99 F0 00 00   289  	MOVZE R0,DS0:_b_usb_detection 
00003A  56 00         290  	CMP R0,#0 
00003C  AC 01 00 CE   291  	AJMP IF_EQ,L77 
000040                293  L95: 
000040                295  L96: 
000040                297  L97: 
000040  78 0B FF FE   298  	MOV R0,[FP+#(-2)] 
000044  68 10 00 00   299  	MOV DS0,#@(_usb_core_status) 
000048  96 F0 00 00   300  	CMP R0,DS0:(_usb_core_status) 
00004C  AC 01 00 68   301  	AJMP IF_EQ,L79 
000050                303  L98: 
000050                305  L99: 
000050                307  L100: 
000050  BB F1 00 00   308  	MOVBX Rb1,_usb_count 
000054  19 01         309  	MOVZE R0,Rb1 
000056  50 01         310  	ADD R0,#1 
000058  AB F1 00 00   311  	MOVBX _usb_count,Rb1 
00005C                313  L101: 
00005C  78 0B FF FE   314  	MOV R0,[FP+#(-2)] 
000060  68 10 00 00   315  	MOV DS0,#@_usb_core_status 
000064                316  .if KERNEL1_E = 1
+                     317  	NOP
+                     318  .endif
000064  A8 F0 00 00   319  	MOV DS0:_usb_core_status,R0 
000068                321  L102: 
000068                323  L103: 
000068                324  L79: 
000068                326  L104: 
000068  BA F0 00 00   327  	MOVX R0,_usb_nosd_timer 
00006C  BA F1 00 02   328  	MOVX R1,_usb_nosd_timer+2 
000070  58 F1         329  	PUSH R1 
000072  58 F0         330  	PUSH R0 
000074                331  .if KERNEL1_5 = 1
+                     332  	CRIT #1
+                     333  .endif
000074  BD 00 00 00   334  	SCALL _timer_check_timeout 
000078  50 F4         335  	ADD SP,#4 
00007A  19 01         336  	MOVZE R0,Rb1 
00007C  56 00         337  	CMP R0,#0 
00007E  AC 01 00 CE   338  	AJMP IF_EQ,L81 
000082                340  L105: 
000082                342  L106: 
000082                344  L107: 
000082  48 10         345  	MOV R1,#0 
000084  48 00         346  	MOV R0,#0 
000086  AA F0 00 00   347  	MOVX _usb_nosd_timer,R0 
00008A  AA F1 00 02   348  	MOVX _usb_nosd_timer+2,R1 
00008E                350  L108: 
00008E  68 F1 00 00   351  	MOV R1,#(L83) 
000092  68 F0 00 00   352  	MOV R0,#^(L83) 
000096  58 F1         353  	PUSH R1 
000098  58 F0         354  	PUSH R0 
00009A                355  .if KERNEL1_5 = 1
+                     356  	CRIT #1
+                     357  .endif
00009A  BD 00 00 00   358  	SCALL _SendString 
00009E  50 F4         359  	ADD SP,#4 
0000A0                361  L109: 
0000A0  BB F1 00 00   362  	MOVBX Rb1,_usb_count 
0000A4  19 01         363  	MOVZE R0,Rb1 
0000A6  58 F0         364  	PUSH R0 
0000A8                365  .if KERNEL1_5 = 1
+                     366  	CRIT #1
+                     367  .endif
0000A8  BD 00 00 00   368  	SCALL _send_hex_u8 
0000AC  50 F2         369  	ADD SP,#2 
0000AE                371  L110: 
0000AE  BB F1 00 00   372  	MOVBX Rb1,_usb_count 
0000B2  19 01         373  	MOVZE R0,Rb1 
0000B4  56 02         374  	CMP R0,#2 
0000B6  AC 0E 00 C4   375  	AJMP IF_SGE,L84 
0000BA                377  L111: 
0000BA                379  L112: 
0000BA                381  L113: 
0000BA  49 11         382  	MOVB Rb1,#1 
0000BC  68 10 00 00   383  	MOV DS0,#@_adpate_flag 
0000C0                384  .if KERNEL1_E = 1
+                     385  	NOP
+                     386  .endif
0000C0  A9 F1 00 00   387  	MOVB DS0:_adpate_flag,Rb1 
0000C4                389  L114: 
0000C4                391  L115: 
0000C4                392  L84: 
0000C4                394  L116: 
0000C4  49 10         395  	MOVB Rb1,#0 
0000C6  68 10 00 00   396  	MOV DS0,#@_b_usb_detection 
0000CA                397  .if KERNEL1_E = 1
+                     398  	NOP
+                     399  .endif
0000CA  A9 F1 00 00   400  	MOVB DS0:_b_usb_detection,Rb1 
0000CE                402  L117: 
0000CE                404  L118: 
0000CE                405  L81: 
0000CE                407  L119: 
0000CE                409  L120: 
0000CE                410  L77: 
0000CE                412  L121: 
0000CE  48 00         413  	MOV R0,#0 
0000D0                415  L122: 
0000D0                417  L123: 
0000D0                418  L72: 
0000D0  38 1F         419  	POP R1 
0000D2                420  	UNLINK_MACRO
0000D2  2F 00         420  UNLINK 
+                     420  
0000D4                421  .if KERNEL1_5 = 1
+                     422  	CRIT #1
+                     423  .endif
0000D4  C6 20         424  	RETS 
+                     426  
+                     427  .area _$MassStorage_Exit(USB_MS_CODE,REL) 
+B 00 07
000000                428  _MassStorage_Exit:: 
000000  2E 00         430  	LINK #0 
000002                432  L125: 
000002                434  L126: 
000002                436  L127: 
000002                437  .if KERNEL1_5 = 1
+                     438  	CRIT #1
+                     439  .endif
000002  BD 00 00 00   440  	SCALL _USBCoreUninit 
000006                442  L128: 
000006                443  .if KERNEL1_5 = 1
+                     444  	CRIT #1
+                     445  .endif
000006  BD 00 00 00   446  	SCALL _USBStorageUninit 
00000A                448  L129: 
00000A                449  .if KERNEL1_5 = 1
+                     450  	CRIT #1
+                     451  .endif
00000A  BD 00 00 00   452  	SCALL _USBWriteCommit 
00000E                454  L130: 
00000E                456  L131: 
00000E                457  L124: 
00000E                458  	UNLINK_MACRO
00000E  2F 00         458  UNLINK 
+                     458  
000010                459  .if KERNEL1_5 = 1
+                     460  	CRIT #1
+                     461  .endif
000010  C6 20         462  	RETS 
+                     464  
+                     465  .area _$MassStorage_Init(USB_MS_CODE,REL) 
+B 00 08
000000                466  _MassStorage_Init:: 
000000  2E 01         468  	LINK #1 
000002  58 F0         469  	PUSH R0 
000004  58 F1         470  	PUSH R1 
000006                472  L133: 
000006                474  L134: 
000006                477  L135: 
000006  68 F1 00 00   478  	MOV R1,#(___usbtimer) 
00000A  68 F0 00 00   479  	MOV R0,#^(___usbtimer) 
00000E  58 F1         480  	PUSH R1 
000010  58 F0         481  	PUSH R0 
000012  68 F0 00 14   482  	MOV R0,#20 
000016  58 F0         483  	PUSH R0 
000018                484  .if KERNEL1_5 = 1
+                     485  	CRIT #1
+                     486  .endif
000018  BD 00 00 00   487  	SCALL _SetVector 
00001C  50 F6         488  	ADD SP,#6 
00001E                490  L136: 
00001E  68 F0 24 9F   491  	MOV R0,#9375 
000022  AA F0 00 00   492  	MOVX _TM2PRD,R0 
000026                494  L137: 
000026  68 F0 10 07   495  	MOV R0,#4103 
00002A  AA F0 00 00   496  	MOVX _TMR2CR,R0 
00002E                498  L138: 
00002E  48 01         499  	MOV R0,#1 
000030  AA F0 00 00   500  	MOVX _TM2ICR,R0 
000034                502  L139: 
000034                503  .if KERNEL1_5 = 1
+                     504  	CRIT #1
+                     505  .endif
000034  BD 00 00 00   506  	SCALL _MassStorage_SD1_Init 
000038                508  L140: 
000038                510  L141: 
000038                511  L132: 
000038  38 1F         512  	POP R1 
00003A  38 0F         513  	POP R0 
00003C                514  	UNLINK_MACRO
00003C  2F 00         514  UNLINK 
+                     514  
00003E                515  .if KERNEL1_5 = 1
+                     516  	CRIT #1
+                     517  .endif
00003E  C6 20         518  	RETS 
+                     708  
+                     709  .area _$USB_MassStorage_config_c$(ROM,REL) 
+B 00 01
000000                711  Letext: 
+                     712  
+                     713  
+                     714  
+                     715  
+                     716  
+                     717  
+                     718  
+                     719  .area _USB_MS_XRAM$RD$usb_stor(USB_MS_XRAM,REL,dat) 
+                     720  .even 
+B 00 09
000000                721  _usb_stor:: 
000000                722  	.ds 38 
+                     725  
+                     726  
+                     727  
+                     728  
+                     729  
+                     730  
+                     731  
+                     732  
+                     733  
+                     734  
+                     735  
+                     736  
+                     737  
+                     738  
+                     739  
+                     740  
+                     741  .area _USB_MS_CODE$RD$USB_MassStorage_config_c$(USB_MS_CODE,REL) 
+B 00 0A
000000                742  L83: 
000000  0D            743  	.db 13 
000001  0A            744  	.db 10 
+                     745  	
000002  75 73 62 5F   746  	.db 117,115,98,95,99,111,117,110,116,61,0 
000006  63 6F 75 6E
00000A  74 3D 00
+                     747  
+                     748  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+  6 L100  000050 R
+  6 L101  00005C R
+  6 L102  000068 R
+  6 L103  000068 R
+  6 L104  000068 R
+  6 L105  000082 R
+  6 L106  000082 R
+  6 L107  000082 R
+  6 L108  00008E R
+  6 L109  0000A0 R
+  6 L110  0000AE R
+  6 L111  0000BA R
+  6 L112  0000BA R
+  6 L113  0000BA R
+  6 L114  0000C4 R
+  6 L115  0000C4 R
+  6 L116  0000C4 R
+  6 L117  0000CE R
+  6 L118  0000CE R
+  6 L119  0000CE R
+  6 L120  0000CE R
+  6 L121  0000CE R
+  6 L122  0000D0 R
+  6 L123  0000D0 R
+  7 L124  00000E R
+  7 L125  000002 R
+  7 L126  000002 R
+  7 L127  000002 R
+  7 L128  000006 R
+  7 L129  00000A R
+  7 L130  00000E R
+  7 L131  00000E R
+  8 L132  000038 R
+  8 L133  000006 R
+  8 L134  000006 R
+  8 L135  000006 R
+  8 L136  00001E R
+  8 L137  000026 R
+  8 L138  00002E R
+  8 L139  000034 R
+  8 L140  000038 R
+  8 L141  000038 R
+  2 L35  000048 R
+  2 L36  000044 R
+  2 L37  000044 R
+  2 L38  000044 R
+  2 L39  000048 R
+  2 L40  000048 R
+  3 L41  000070 R
+  3 L42  000010 R
+  3 L43  000064 R
+  3 L47  00000A R
+  3 L48  00000A R
+  3 L49  00000A R
+  3 L50  000010 R
+  3 L51  000064 R
+  3 L52  000064 R
+  3 L53  000064 R
+  3 L54  00006E R
+  3 L55  000070 R
+  3 L56  000070 R
+  4 L57  000014 R
+  4 L58  000002 R
+  4 L59  000002 R
+  4 L60  000002 R
+  4 L61  000014 R
+  4 L62  000014 R
+  5 L63  000032 R
+  5 L64  000006 R
+  5 L65  000006 R
+  5 L66  000006 R
+  5 L67  00000C R
+  5 L68  000010 R
+  5 L69  00001E R
+  5 L70  000032 R
+  5 L71  000032 R
+  6 L72  0000D0 R
+  6 L73  00001A R
+  6 L75  00002A R
+  6 L77  0000CE R
+  6 L79  000068 R
+  6 L81  0000CE R
+  A L83  000000 R
+  6 L84  0000C4 R
+  6 L86  000004 R
+  6 L87  000004 R
+  6 L88  000004 R
+  6 L89  000008 R
+  6 L90  000016 R
+  6 L91  00001A R
+  6 L92  000024 R
+  6 L93  00002A R
+  6 L94  000032 R
+  6 L95  000040 R
+  6 L96  000040 R
+  6 L97  000040 R
+  6 L98  000050 R
+  6 L99  000050 R
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
+    _ClrCirqAndAdjustCpri  ****** GX
+  7 _MassStorage_Exit  000000 GR
+  8 _MassStorage_Init  000000 GR
+  6 _MassStorage_Loop  000000 GR
+  5 _MassStorage_Registers  000000 GR
+    _MassStorage_SD1_Init  ****** GX
+    _MassStorage_SD1_Loop  ****** GX
+    _MassStorage_SD1_Register  ****** GX
+  4 _MassStorage_Write  000000 GR
+    _SendString  ****** GX
+    _SetVector  ****** GX
+    _TM2ICR  ****** GX
+    _TM2PRD  ****** GX
+    _TMR2CR  ****** GX
+    _USBCoreInit  ****** GX
+    _USBCoreLoop  ****** GX
+    _USBCoreUninit  ****** GX
+    _USBRemoveCheck  ****** GX
+    _USBStorageInit  ****** GX
+    _USBStorageUninit  ****** GX
+  3 _USBWriteCommit  000000 GR
+    __MULlong  ****** GX
+  2 ___usbtimer  000000 GR
+    _adpate_flag  ****** GX
+    _b_usb_detection  ****** GX
+    _send_hex_u8  ****** GX
+    _timer_check_timeout  ****** GX
+    _usb_core_status  ****** GX
+    _usb_count  ****** GX
+    _usb_nosd_timer  ****** GX
+  9 _usb_stor  000000 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$USB_MassStorage_config_c$ size    0   flags 0
+   2 _$__usbtimer size   7A   flags 0
+   3 _$USBWriteCommit size   7C   flags 0
+   4 _$MassStorage_Write size   18   flags 0
+   5 _$MassStorage_Registers size   3A   flags 0
+   6 _$MassStorage_Loop size   D6   flags 0
+   7 _$MassStorage_Exit size   12   flags 0
+   8 _$MassStorage_Init size   40   flags 0
+   9 _USB_MS_XRAM$RD$usb_stor size   26   flags 0
+   A _USB_MS_CODE$RD$USB_MassStorage_config_c$ size    E   flags 0
