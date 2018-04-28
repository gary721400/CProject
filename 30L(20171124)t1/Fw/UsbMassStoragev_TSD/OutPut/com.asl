+                       1  
+                       5  
+                       6  .area _$com_c$(ROM,REL) 
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
+                      58  .area _$UART_Init(UART_CODE,REL) 
+B 00 02
000000                 60  _UART_Init:: 
000000  2E 00          62  	LINK #0 
000002  58 F1          63  	PUSH R1 
000004                 65  L30: 
000004                 67  L31: 
000004                 69  L32: 
000004  48 12          70  	MOV R1,#2 
000006  AA F1 00 00    71  	MOVX _UARBR,R1 
00000A                 73  L33: 
00000A  68 F1 80 01    74  	MOV R1,#0x8001 
00000E  AA F1 00 00    75  	MOVX _UARCR,R1 
000012                 77  L34: 
000012  68 F1 00 10    78  	MOV R1,#16 
000016  3F 10          79  	IGNDS #1 
000018  B5 F1 00 00    80  	OR _PBEN,R1 
00001C                 82  L35: 
00001C                 84  L36: 
00001C                 85  L29: 
00001C  38 1F          86  	POP R1 
00001E                 87  	UNLINK_MACRO
00001E  2F 00          87  UNLINK 
+                      87  
000020                 88  .if KERNEL1_5 = 1
+                      89  	CRIT #1
+                      90  .endif
000020  C6 20          91  	RETS 
+                      93  
+                      94  .area _$SendByte(UART_CODE,REL) 
+B 00 03
000000                 95  _SendByte:: 
000000  2E 00          98  	LINK #0 
000002  58 F1          99  	PUSH R1 
000004                101  L41: 
000004                103  L42: 
000004  78 1B 00 06   104  	MOV R1,[FP+#(6)] 
000008  6B 3B 00 06   105  	MOVB [FP+#(6)],Rb3 
00000C                107  L43: 
00000C  79 3B 00 06   108  	MOVB Rb3,[FP+#(6)] 
000010  19 13         109  	MOVZE R1,Rb3 
000012  AA F1 00 00   110  	MOVX _UARTDR,R1 
000016                112  L44: 
+                     113  
000016  C7 00         114  			NOP 
000018  C7 00         115  			NOP 
+                     116  		
00001A                118  L45: 
00001A                119  L38: 
00001A                121  L46: 
00001A                122  L39: 
00001A                124  L47: 
00001A  BA F1 00 00   125  	MOVX R1,_UARSR 
00001E  54 11         126  	AND R1,#1 
000020  56 10         127  	CMP R1,#0 
000022  AC 01 00 1A   128  	AJMP IF_EQ,L38 
000026                130  L48: 
000026                132  L49: 
000026                133  L37: 
000026  38 1F         134  	POP R1 
000028                135  	UNLINK_MACRO
000028  2F 00         135  UNLINK 
+                     135  
00002A                136  .if KERNEL1_5 = 1
+                     137  	CRIT #1
+                     138  .endif
00002A  C6 20         139  	RETS 
+                     141  
+                     142  .area _$SendString(UART_CODE,REL) 
+B 00 04
000000                143  _SendString:: 
000000  2E 00         147  	LINK #0 
000002  58 F0         148  	PUSH R0 
000004  58 F1         149  	PUSH R1 
000006  58 F2         150  	PUSH R2 
000008  58 F3         151  	PUSH R3 
00000A                153  L54: 
00000A                155  L55: 
00000A                157  L56: 
00000A  AC 00 00 3E   158  	AJMP #0,L52 
00000E                159  L51: 
00000E                161  L57: 
00000E                163  L58: 
00000E                165  L59: 
00000E                166  	LOAD4$stack R0
00000E  78 0B 00 06   166  MOV R0,[FP+#(6)] 
000012  78 1B 00 08   166  MOV R1,[FP+#(6+2)] 
+                     166  
000016                167  	MOVDR R2
000016                167  .if R2=R0
+                     167  .else
000016  08 20         167  MOV R2,R0 
+                     167  .endif
000018                167  .if R3=R1
+                     167  .else
000018  08 31         167  MOV R3,R1 
+                     167  .endif
+                     167  
00001A  50 31         168  	ADD R3,#1 
00001C  51 20         169  	ADDC R2,#0 
00001E                170  	SAVE4$stack 6
00001E  6A 2B 00 06   170  MOV [FP+#(6)],R2 
000022  6A 3B 00 08   170  MOV [FP+#(6+2)],R3 
+                     170  
000026                171  	LOAD1$indirect Rb1
000026  4C 11         171  SL R1,#1 
000028  5C 01         171  SLOSB R0,#1 
00002A  BA 10 F2 E0   171  MOVX DS0, R0 
00002E  4D 11         171  SR R1,#1 
000030  29 11         171  MOVB Rb1,[R1] 
+                     171  
000032  18 01         172  	MOVSE R0,Rb1 
000034  19 01         173  	MOVZE R0,Rb1 
000036  58 F0         174  	PUSH R0 
000038                175  .if KERNEL1_5 = 1
+                     176  	CRIT #1
+                     177  .endif
000038  BD 00 00 00   178  	SCALL _SendByte 
00003C  50 F2         179  	ADD SP,#2 
00003E                181  L60: 
00003E                183  L61: 
00003E                184  L52: 
00003E                186  L62: 
00003E                187  	LOAD4$stack R0
00003E  78 0B 00 06   187  MOV R0,[FP+#(6)] 
000042  78 1B 00 08   187  MOV R1,[FP+#(6+2)] 
+                     187  
000046                188  	LOAD1$indirect Rb1
000046  4C 11         188  SL R1,#1 
000048  5C 01         188  SLOSB R0,#1 
00004A  BA 10 F2 E0   188  MOVX DS0, R0 
00004E  4D 11         188  SR R1,#1 
000050  29 11         188  MOVB Rb1,[R1] 
+                     188  
000052  18 01         189  	MOVSE R0,Rb1 
000054  56 00         190  	CMP R0,#0 
000056  AC 02 00 0E   191  	AJMP IF_NE,L51 
00005A                193  L63: 
00005A                195  L64: 
00005A                196  L50: 
00005A  38 3F         197  	POP R3 
00005C  38 2F         198  	POP R2 
00005E  38 1F         199  	POP R1 
000060  38 0F         200  	POP R0 
000062                201  	UNLINK_MACRO
000062  2F 00         201  UNLINK 
+                     201  
000064                202  .if KERNEL1_5 = 1
+                     203  	CRIT #1
+                     204  .endif
000064  C6 20         205  	RETS 
+                     207  
+                     208  .area _$send_hex_byte(UART_CODE,REL) 
+B 00 05
000000                209  _send_hex_byte:: 
000000  2E 00         212  	LINK #0 
000002  58 F0         213  	PUSH R0 
000004                215  L66: 
000004                217  L67: 
000004  78 0B 00 06   218  	MOV R0,[FP+#(6)] 
000008  6B 1B 00 06   219  	MOVB [FP+#(6)],Rb1 
00000C                221  L68: 
00000C  79 1B 00 06   222  	MOVB Rb1,[FP+#(6)] 
000010  19 01         223  	MOVZE R0,Rb1 
000012  74 F0 00 F0   224  	AND R0,#240 
000016  4B 04         225  	ASR R0,#4 
000018  19 01         226  	MOVZE R0,Rb1 
00001A  58 F0         227  	PUSH R0 
00001C                228  .if KERNEL1_5 = 1
+                     229  	CRIT #1
+                     230  .endif
00001C  BD 00 00 00   231  	SCALL _conv 
000020  50 F2         232  	ADD SP,#2 
000022  19 01         233  	MOVZE R0,Rb1 
000024  58 F0         234  	PUSH R0 
000026                235  .if KERNEL1_5 = 1
+                     236  	CRIT #1
+                     237  .endif
000026  BD 00 00 00   238  	SCALL _SendByte 
00002A  50 F2         239  	ADD SP,#2 
00002C                241  L69: 
00002C  79 1B 00 06   242  	MOVB Rb1,[FP+#(6)] 
000030  19 01         243  	MOVZE R0,Rb1 
000032  54 0F         244  	AND R0,#15 
000034  19 01         245  	MOVZE R0,Rb1 
000036  58 F0         246  	PUSH R0 
000038                247  .if KERNEL1_5 = 1
+                     248  	CRIT #1
+                     249  .endif
000038  BD 00 00 00   250  	SCALL _conv 
00003C  50 F2         251  	ADD SP,#2 
00003E  19 01         252  	MOVZE R0,Rb1 
000040  58 F0         253  	PUSH R0 
000042                254  .if KERNEL1_5 = 1
+                     255  	CRIT #1
+                     256  .endif
000042  BD 00 00 00   257  	SCALL _SendByte 
000046  50 F2         258  	ADD SP,#2 
000048                260  L70: 
000048                262  L71: 
000048                263  L65: 
000048  38 0F         264  	POP R0 
00004A                265  	UNLINK_MACRO
00004A  2F 00         265  UNLINK 
+                     265  
00004C                266  .if KERNEL1_5 = 1
+                     267  	CRIT #1
+                     268  .endif
00004C  C6 20         269  	RETS 
+                     271  
+                     272  .area _$send_hex_u8(UART_CODE,REL) 
+B 00 06
000000                273  _send_hex_u8:: 
000000  2E 00         276  	LINK #0 
000002  58 F0         277  	PUSH R0 
000004                279  L73: 
000004                281  L74: 
000004  78 0B 00 06   282  	MOV R0,[FP+#(6)] 
000008  6B 1B 00 06   283  	MOVB [FP+#(6)],Rb1 
00000C                285  L75: 
00000C  68 F0 00 30   286  	MOV R0,#48 
000010  58 F0         287  	PUSH R0 
000012                288  .if KERNEL1_5 = 1
+                     289  	CRIT #1
+                     290  .endif
000012  BD 00 00 00   291  	SCALL _SendByte 
000016  50 F2         292  	ADD SP,#2 
000018                294  L76: 
000018  68 F0 00 78   295  	MOV R0,#120 
00001C  58 F0         296  	PUSH R0 
00001E                297  .if KERNEL1_5 = 1
+                     298  	CRIT #1
+                     299  .endif
00001E  BD 00 00 00   300  	SCALL _SendByte 
000022  50 F2         301  	ADD SP,#2 
000024                303  L77: 
000024  79 1B 00 06   304  	MOVB Rb1,[FP+#(6)] 
000028  19 01         305  	MOVZE R0,Rb1 
00002A  58 F0         306  	PUSH R0 
00002C                307  .if KERNEL1_5 = 1
+                     308  	CRIT #1
+                     309  .endif
00002C  BD 00 00 00   310  	SCALL _send_hex_byte 
000030  50 F2         311  	ADD SP,#2 
000032                313  L78: 
000032                315  L79: 
000032                316  L72: 
000032  38 0F         317  	POP R0 
000034                318  	UNLINK_MACRO
000034  2F 00         318  UNLINK 
+                     318  
000036                319  .if KERNEL1_5 = 1
+                     320  	CRIT #1
+                     321  .endif
000036  C6 20         322  	RETS 
+                     324  
+                     325  .area _$send_hex_u16(UART_CODE,REL) 
+B 00 07
000000                326  _send_hex_u16:: 
000000  2E 00         329  	LINK #0 
000002  58 F0         330  	PUSH R0 
000004                332  L81: 
000004                334  L82: 
000004                336  L83: 
000004  68 F0 00 30   337  	MOV R0,#48 
000008  58 F0         338  	PUSH R0 
00000A                339  .if KERNEL1_5 = 1
+                     340  	CRIT #1
+                     341  .endif
00000A  BD 00 00 00   342  	SCALL _SendByte 
00000E  50 F2         343  	ADD SP,#2 
000010                345  L84: 
000010  68 F0 00 78   346  	MOV R0,#120 
000014  58 F0         347  	PUSH R0 
000016                348  .if KERNEL1_5 = 1
+                     349  	CRIT #1
+                     350  .endif
000016  BD 00 00 00   351  	SCALL _SendByte 
00001A  50 F2         352  	ADD SP,#2 
00001C                354  L85: 
00001C  78 0B 00 06   355  	MOV R0,[FP+#(6)] 
000020  4D 08         356  	SR R0,#8 
000022  19 01         357  	MOVZE R0,Rb1 
000024  58 F0         358  	PUSH R0 
000026                359  .if KERNEL1_5 = 1
+                     360  	CRIT #1
+                     361  .endif
000026  BD 00 00 00   362  	SCALL _send_hex_byte 
00002A  50 F2         363  	ADD SP,#2 
00002C                365  L86: 
00002C  78 0B 00 06   366  	MOV R0,[FP+#(6)] 
000030  19 01         367  	MOVZE R0,Rb1 
000032  58 F0         368  	PUSH R0 
000034                369  .if KERNEL1_5 = 1
+                     370  	CRIT #1
+                     371  .endif
000034  BD 00 00 00   372  	SCALL _send_hex_byte 
000038  50 F2         373  	ADD SP,#2 
00003A                375  L87: 
00003A                377  L88: 
00003A                378  L80: 
00003A  38 0F         379  	POP R0 
00003C                380  	UNLINK_MACRO
00003C  2F 00         380  UNLINK 
+                     380  
00003E                381  .if KERNEL1_5 = 1
+                     382  	CRIT #1
+                     383  .endif
00003E  C6 20         384  	RETS 
+                     386  
+                     387  .area _$send_hex_u32(UART_CODE,REL) 
+B 00 08
000000                388  _send_hex_u32:: 
000000  2E 00         391  	LINK #0 
000002  58 F0         392  	PUSH R0 
000004  58 F1         393  	PUSH R1 
000006                395  L90: 
000006                397  L91: 
000006                399  L92: 
000006  68 F0 00 30   400  	MOV R0,#48 
00000A  58 F0         401  	PUSH R0 
00000C                402  .if KERNEL1_5 = 1
+                     403  	CRIT #1
+                     404  .endif
00000C  BD 00 00 00   405  	SCALL _SendByte 
000010  50 F2         406  	ADD SP,#2 
000012                408  L93: 
000012  68 F0 00 78   409  	MOV R0,#120 
000016  58 F0         410  	PUSH R0 
000018                411  .if KERNEL1_5 = 1
+                     412  	CRIT #1
+                     413  .endif
000018  BD 00 00 00   414  	SCALL _SendByte 
00001C  50 F2         415  	ADD SP,#2 
00001E                417  L94: 
00001E                418  	LOAD4$stack R0
00001E  78 0B 00 06   418  MOV R0,[FP+#(6)] 
000022  78 1B 00 08   418  MOV R1,[FP+#(6+2)] 
+                     418  
000026  08 10         419  	MOV R1,R0 
000028  48 00         420  	MOV R0,#0 
00002A  4D 18         421  	SR R1,#(24-16) 
00002C  09 13         422  	MOVB Rb1,Rb3 
00002E  19 01         423  	MOVZE R0,Rb1 
000030  58 F0         424  	PUSH R0 
000032                425  .if KERNEL1_5 = 1
+                     426  	CRIT #1
+                     427  .endif
000032  BD 00 00 00   428  	SCALL _send_hex_byte 
000036  50 F2         429  	ADD SP,#2 
000038                431  L95: 
000038                432  	LOAD4$stack R0
000038  78 0B 00 06   432  MOV R0,[FP+#(6)] 
00003C  78 1B 00 08   432  MOV R1,[FP+#(6+2)] 
+                     432  
000040  08 10         433  	MOV R1,R0 
000042  48 00         434  	MOV R0,#0 
000044  09 13         435  	MOVB Rb1,Rb3 
000046  19 01         436  	MOVZE R0,Rb1 
000048  58 F0         437  	PUSH R0 
00004A                438  .if KERNEL1_5 = 1
+                     439  	CRIT #1
+                     440  .endif
00004A  BD 00 00 00   441  	SCALL _send_hex_byte 
00004E  50 F2         442  	ADD SP,#2 
000050                444  L96: 
000050                445  	LOAD4$stack R0
000050  78 0B 00 06   445  MOV R0,[FP+#(6)] 
000054  78 1B 00 08   445  MOV R1,[FP+#(6+2)] 
+                     445  
000058  4D 08         446  	SR R0,#8 
00005A  5D 18         447  	SROSB R1,#8 
00005C  09 13         448  	MOVB Rb1,Rb3 
00005E  19 01         449  	MOVZE R0,Rb1 
000060  58 F0         450  	PUSH R0 
000062                451  .if KERNEL1_5 = 1
+                     452  	CRIT #1
+                     453  .endif
000062  BD 00 00 00   454  	SCALL _send_hex_byte 
000066  50 F2         455  	ADD SP,#2 
000068                457  L97: 
000068                458  	LOAD4$stack R0
000068  78 0B 00 06   458  MOV R0,[FP+#(6)] 
00006C  78 1B 00 08   458  MOV R1,[FP+#(6+2)] 
+                     458  
000070  09 13         459  	MOVB Rb1,Rb3 
000072  19 01         460  	MOVZE R0,Rb1 
000074  58 F0         461  	PUSH R0 
000076                462  .if KERNEL1_5 = 1
+                     463  	CRIT #1
+                     464  .endif
000076  BD 00 00 00   465  	SCALL _send_hex_byte 
00007A  50 F2         466  	ADD SP,#2 
00007C                468  L98: 
00007C                470  L99: 
00007C                471  L89: 
00007C  38 1F         472  	POP R1 
00007E  38 0F         473  	POP R0 
000080                474  	UNLINK_MACRO
000080  2F 00         474  UNLINK 
+                     474  
000082                475  .if KERNEL1_5 = 1
+                     476  	CRIT #1
+                     477  .endif
000082  C6 20         478  	RETS 
+                     480  
+                     481  .area _$conv(UART_CODE,REL) 
+B 00 09
000000                482  _conv:: 
000000  2E 00         485  	LINK #0 
000002                487  L103: 
000002                489  L104: 
000002  78 0B 00 06   490  	MOV R0,[FP+#(6)] 
000006  6B 1B 00 06   491  	MOVB [FP+#(6)],Rb1 
00000A                493  L105: 
00000A  79 1B 00 06   494  	MOVB Rb1,[FP+#(6)] 
00000E  19 01         495  	MOVZE R0,Rb1 
000010  56 0A         496  	CMP R0,#10 
000012  AC 0E 00 26   497  	AJMP IF_SGE,L101 
000016                499  L106: 
000016                501  L107: 
000016                503  L108: 
000016  79 1B 00 06   504  	MOVB Rb1,[FP+#(6)] 
00001A  19 01         505  	MOVZE R0,Rb1 
00001C  70 F0 00 30   506  	ADD R0,#48 
000020  19 01         507  	MOVZE R0,Rb1 
000022  AC 00 00 34   508  	AJMP #0,L100 
000026                510  L109: 
000026                512  L110: 
000026                513  L101: 
000026                515  L111: 
000026                517  L112: 
000026                519  L113: 
000026  79 1B 00 06   520  	MOVB Rb1,[FP+#(6)] 
00002A  19 01         521  	MOVZE R0,Rb1 
00002C  52 0A         522  	SUB R0,#10 
00002E  70 F0 00 61   523  	ADD R0,#97 
000032  19 01         524  	MOVZE R0,Rb1 
000034                526  L114: 
000034                528  L115: 
000034                530  L116: 
000034                532  L117: 
000034                533  L100: 
000034                534  	UNLINK_MACRO
000034  2F 00         534  UNLINK 
+                     534  
000036                535  .if KERNEL1_5 = 1
+                     536  	CRIT #1
+                     537  .endif
000036  C6 20         538  	RETS 
+                     711  
+                     712  .area _$com_c$(ROM,REL) 
+B 00 01
000000                714  Letext: 
+                     715  
+                     716  
+                     717  
+                     718  
+                     719  
+                     720  
+                     721  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+  9 L100  000034 R
+  9 L101  000026 R
+  9 L103  000002 R
+  9 L104  000002 R
+  9 L105  00000A R
+  9 L106  000016 R
+  9 L107  000016 R
+  9 L108  000016 R
+  9 L109  000026 R
+  9 L110  000026 R
+  9 L111  000026 R
+  9 L112  000026 R
+  9 L113  000026 R
+  9 L114  000034 R
+  9 L115  000034 R
+  9 L116  000034 R
+  9 L117  000034 R
+  2 L29  00001C R
+  2 L30  000004 R
+  2 L31  000004 R
+  2 L32  000004 R
+  2 L33  00000A R
+  2 L34  000012 R
+  2 L35  00001C R
+  2 L36  00001C R
+  3 L37  000026 R
+  3 L38  00001A R
+  3 L39  00001A R
+  3 L41  000004 R
+  3 L42  000004 R
+  3 L43  00000C R
+  3 L44  000016 R
+  3 L45  00001A R
+  3 L46  00001A R
+  3 L47  00001A R
+  3 L48  000026 R
+  3 L49  000026 R
+  4 L50  00005A R
+  4 L51  00000E R
+  4 L52  00003E R
+  4 L54  00000A R
+  4 L55  00000A R
+  4 L56  00000A R
+  4 L57  00000E R
+  4 L58  00000E R
+  4 L59  00000E R
+  4 L60  00003E R
+  4 L61  00003E R
+  4 L62  00003E R
+  4 L63  00005A R
+  4 L64  00005A R
+  5 L65  000048 R
+  5 L66  000004 R
+  5 L67  000004 R
+  5 L68  00000C R
+  5 L69  00002C R
+  5 L70  000048 R
+  5 L71  000048 R
+  6 L72  000032 R
+  6 L73  000004 R
+  6 L74  000004 R
+  6 L75  00000C R
+  6 L76  000018 R
+  6 L77  000024 R
+  6 L78  000032 R
+  6 L79  000032 R
+  7 L80  00003A R
+  7 L81  000004 R
+  7 L82  000004 R
+  7 L83  000004 R
+  7 L84  000010 R
+  7 L85  00001C R
+  7 L86  00002C R
+  7 L87  00003A R
+  7 L88  00003A R
+  8 L89  00007C R
+  8 L90  000006 R
+  8 L91  000006 R
+  8 L92  000006 R
+  8 L93  000012 R
+  8 L94  00001E R
+  8 L95  000038 R
+  8 L96  000050 R
+  8 L97  000068 R
+  8 L98  00007C R
+  8 L99  00007C R
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
+    _PBEN  ****** GX
+  3 _SendByte  000000 GR
+  4 _SendString  000000 GR
+    _UARBR  ****** GX
+    _UARCR  ****** GX
+    _UARSR  ****** GX
+    _UARTDR  ****** GX
+  2 _UART_Init  000000 GR
+  9 _conv  000000 GR
+  5 _send_hex_byte  000000 GR
+  7 _send_hex_u16  000000 GR
+  8 _send_hex_u32  000000 GR
+  6 _send_hex_u8  000000 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$com_c$ size    0   flags 0
+   2 _$UART_Init size   22   flags 0
+   3 _$SendByte size   2C   flags 0
+   4 _$SendString size   66   flags 0
+   5 _$send_hex_byte size   4E   flags 0
+   6 _$send_hex_u8 size   38   flags 0
+   7 _$send_hex_u16 size   40   flags 0
+   8 _$send_hex_u32 size   84   flags 0
+   9 _$conv size   38   flags 0
