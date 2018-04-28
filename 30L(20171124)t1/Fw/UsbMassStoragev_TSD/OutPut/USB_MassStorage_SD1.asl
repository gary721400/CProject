+                       1  
+                       5  
+                       6  .area _$USB_MassStorage_SD1_c$(ROM,REL) 
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
+                      58  .area _$usd1_protect(USB_MS_CODE,REL) 
+B 00 02
000000                 60  _usd1_protect:: 
000000  2E 00          62  	LINK #0 
000002                 64  L36: 
000002                 66  L37: 
000002                 68  L38: 
000002  48 00          69  	MOV R0,#0 
000004                 71  L39: 
000004                 73  L40: 
000004                 74  L35: 
000004                 75  	UNLINK_MACRO
000004  2F 00          75  UNLINK 
+                      75  
000006                 76  .if KERNEL1_5 = 1
+                      77  	CRIT #1
+                      78  .endif
000006  C6 20          79  	RETS 
+                      81  
+                      82  .area _$usd1_write(USB_MS_CODE,REL) 
+B 00 03
000000                 83  _usd1_write:: 
000000  2E 00          88  	LINK #0 
000002  58 F1          89  	PUSH R1 
000004                 91  L42: 
000004                 93  L43: 
000004                 95  L44: 
000004                 96  .if KERNEL1_5 = 1
+                      97  	CRIT #1
+                      98  .endif
000004  BD 00 00 00    99  	SCALL _MassStorage_Write 
000008                101  L45: 
000008  78 0B 00 0A   102  	MOV R0,[FP+#(10)] 
00000C  58 F0         103  	PUSH R0 
00000E                104  	LOAD4$stack R0
00000E  78 0B 00 06   104  MOV R0,[FP+#(6)] 
000012  78 1B 00 08   104  MOV R1,[FP+#(6+2)] 
+                     104  
000016  58 F1         105  	PUSH R1 
000018  58 F0         106  	PUSH R0 
00001A                107  .if KERNEL1_5 = 1
+                     108  	CRIT #1
+                     109  .endif
00001A  BD 00 00 00   110  	SCALL _sd1_cache_write 
00001E  50 F6         111  	ADD SP,#6 
000020                113  L46: 
000020                115  L47: 
000020                116  L41: 
000020  38 1F         117  	POP R1 
000022                118  	UNLINK_MACRO
000022  2F 00         118  UNLINK 
+                     118  
000024                119  .if KERNEL1_5 = 1
+                     120  	CRIT #1
+                     121  .endif
000024  C6 20         122  	RETS 
+                     124  
+                     125  .area _$usd1_status(USB_MS_CODE,REL) 
+B 00 04
000000                126  _usd1_status:: 
000000  2E 00         128  	LINK #0 
000002                130  L51: 
000002                132  L52: 
000002                134  L53: 
000002  BA F0 00 00   135  	MOVX R0,_SD1_Mounted 
000006  56 00         136  	CMP R0,#0 
000008  AC 01 00 12   137  	AJMP IF_EQ,L49 
00000C                139  L54: 
00000C  48 01         140  	MOV R0,#1 
00000E  AC 00 00 14   141  	AJMP #0,L48 
000012                142  L49: 
000012                144  L55: 
000012  48 00         145  	MOV R0,#0 
000014                147  L56: 
000014                149  L57: 
000014                150  L48: 
000014                151  	UNLINK_MACRO
000014  2F 00         151  UNLINK 
+                     151  
000016                152  .if KERNEL1_5 = 1
+                     153  	CRIT #1
+                     154  .endif
000016  C6 20         155  	RETS 
+                     157  
+                     158  .area _$MassStorage_SD1_Register(USB_MS_CODE,REL) 
+B 00 05
000000                159  _MassStorage_SD1_Register:: 
000000  2E 00         162  	LINK #0 
000002  58 F0         163  	PUSH R0 
000004  58 F1         164  	PUSH R1 
000006                166  L62: 
000006                168  L63: 
000006                170  L64: 
000006  68 F1 00 00   171  	MOV R1,#(L61) 
00000A  68 F0 00 00   172  	MOV R0,#^(L61) 
00000E  58 F1         173  	PUSH R1 
000010  58 F0         174  	PUSH R0 
000012  68 F1 00 06   175  	MOV R1,#(L60) 
000016  68 F0 00 06   176  	MOV R0,#^(L60) 
00001A  58 F1         177  	PUSH R1 
00001C  58 F0         178  	PUSH R0 
00001E  68 F1 00 10   179  	MOV R1,#(L59) 
000022  68 F0 00 10   180  	MOV R0,#^(L59) 
000026  58 F1         181  	PUSH R1 
000028  58 F0         182  	PUSH R0 
00002A  68 F0 00 80   183  	MOV R0,#128 
00002E  58 F0         184  	PUSH R0 
000030  68 F1 00 00   185  	MOV R1,#(_usd1_status) 
000034  68 F0 00 00   186  	MOV R0,#^(_usd1_status) 
000038  58 F1         187  	PUSH R1 
00003A  58 F0         188  	PUSH R0 
00003C  68 F1 00 00   189  	MOV R1,#(_sd1_fat_size) 
000040  68 F0 00 00   190  	MOV R0,#^(_sd1_fat_size) 
000044  58 F1         191  	PUSH R1 
000046  58 F0         192  	PUSH R0 
000048  68 F1 00 00   193  	MOV R1,#(_sd1_cache_flush) 
00004C  68 F0 00 00   194  	MOV R0,#^(_sd1_cache_flush) 
000050  58 F1         195  	PUSH R1 
000052  58 F0         196  	PUSH R0 
000054  68 F1 00 00   197  	MOV R1,#(_usd1_protect) 
000058  68 F0 00 00   198  	MOV R0,#^(_usd1_protect) 
00005C  58 F1         199  	PUSH R1 
00005E  58 F0         200  	PUSH R0 
000060  68 F1 00 00   201  	MOV R1,#(_usd1_write) 
000064  68 F0 00 00   202  	MOV R0,#^(_usd1_write) 
000068  58 F1         203  	PUSH R1 
00006A  58 F0         204  	PUSH R0 
00006C  68 F1 00 00   205  	MOV R1,#(_sd1_cache_read) 
000070  68 F0 00 00   206  	MOV R0,#^(_sd1_cache_read) 
000074  58 F1         207  	PUSH R1 
000076  58 F0         208  	PUSH R0 
000078  78 0B 00 06   209  	MOV R0,[FP+#(6)] 
00007C  58 F0         210  	PUSH R0 
00007E                211  .if KERNEL1_5 = 1
+                     212  	CRIT #1
+                     213  .endif
00007E  BD 00 00 00   214  	SCALL _USBStorageRegister 
000082  70 FF 00 28   215  	ADD SP,#40 
000086                217  L65: 
000086                219  L66: 
000086                220  L58: 
000086  38 1F         221  	POP R1 
000088  38 0F         222  	POP R0 
00008A                223  	UNLINK_MACRO
00008A  2F 00         223  UNLINK 
+                     223  
00008C                224  .if KERNEL1_5 = 1
+                     225  	CRIT #1
+                     226  .endif
00008C  C6 20         227  	RETS 
+                     229  
+                     230  .area _$MassStorage_SD1_Loop(USB_MS_CODE,REL) 
+B 00 06
000000                231  _MassStorage_SD1_Loop:: 
000000  2E 00         233  	LINK #0 
000002                235  L68: 
000002                237  L69: 
000002                239  L70: 
000002                241  L71: 
000002                242  L67: 
000002                243  	UNLINK_MACRO
000002  2F 00         243  UNLINK 
+                     243  
000004                244  .if KERNEL1_5 = 1
+                     245  	CRIT #1
+                     246  .endif
000004  C6 20         247  	RETS 
+                     249  
+                     250  .area _$MassStorage_SD1_Init(USB_MS_CODE,REL) 
+B 00 07
000000                251  _MassStorage_SD1_Init:: 
000000  2E 00         253  	LINK #0 
000002  58 F0         254  	PUSH R0 
000004                256  L73: 
000004                258  L74: 
000004                260  L75: 
000004                261  .if KERNEL1_5 = 1
+                     262  	CRIT #1
+                     263  .endif
000004  BD 00 00 00   264  	SCALL _sd1_mount 
000008                266  L76: 
000008  48 03         267  	MOV R0,#3 
00000A  AA F0 00 00   268  	MOVX _SDCKCR,R0 
00000E                270  L77: 
00000E                271  .if KERNEL1_5 = 1
+                     272  	CRIT #1
+                     273  .endif
00000E  BD 00 00 00   274  	SCALL _sd1_cache8k_init 
000012                276  L78: 
000012                278  L79: 
000012                279  L72: 
000012  38 0F         280  	POP R0 
000014                281  	UNLINK_MACRO
000014  2F 00         281  UNLINK 
+                     281  
000016                282  .if KERNEL1_5 = 1
+                     283  	CRIT #1
+                     284  .endif
000016  C6 20         285  	RETS 
+                     475  
+                     476  .area _$USB_MassStorage_SD1_c$(ROM,REL) 
+B 00 01
000000                478  Letext: 
+                     479  
+                     480  
+                     481  
+                     482  
+                     483  
+                     484  
+                     485  
+                     486  
+                     487  
+                     488  
+                     489  
+                     490  .area _USB_MS_CODE$RD$USB_MassStorage_SD1_c$(USB_MS_CODE,REL) 
+B 00 08
000000                491  L61: 
+                     492  	
000000  31 2E 30 30   493  	.db 49,46,48,48,0 
000004  00
+                     494  
+                     495  .area _USB_MS_CODE$RD$USB_MassStorage_SD1_c$(USB_MS_CODE,REL) 
+B 00 08
000006                496  L60: 
+                     497  	
000006  53 44 2D 31   498  	.db 83,68,45,49,32,68,105,115,107,0 
00000A  20 44 69 73
00000E  6B 00
+                     499  
+                     500  .area _USB_MS_CODE$RD$USB_MassStorage_SD1_c$(USB_MS_CODE,REL) 
+B 00 08
000010                501  L59: 
+                     502  	
000010  4B 42 20 55   503  	.db 75,66,32,85,83,66,50,48,0 
000014  53 42 32 30
000018  00
+                     504  
+                     505  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+  2 L35  000004 R
+  2 L36  000002 R
+  2 L37  000002 R
+  2 L38  000002 R
+  2 L39  000004 R
+  2 L40  000004 R
+  3 L41  000020 R
+  3 L42  000004 R
+  3 L43  000004 R
+  3 L44  000004 R
+  3 L45  000008 R
+  3 L46  000020 R
+  3 L47  000020 R
+  4 L48  000014 R
+  4 L49  000012 R
+  4 L51  000002 R
+  4 L52  000002 R
+  4 L53  000002 R
+  4 L54  00000C R
+  4 L55  000012 R
+  4 L56  000014 R
+  4 L57  000014 R
+  5 L58  000086 R
+  8 L59  000010 R
+  8 L60  000006 R
+  8 L61  000000 R
+  5 L62  000006 R
+  5 L63  000006 R
+  5 L64  000006 R
+  5 L65  000086 R
+  5 L66  000086 R
+  6 L67  000002 R
+  6 L68  000002 R
+  6 L69  000002 R
+  6 L70  000002 R
+  6 L71  000002 R
+  7 L72  000012 R
+  7 L73  000004 R
+  7 L74  000004 R
+  7 L75  000004 R
+  7 L76  000008 R
+  7 L77  00000E R
+  7 L78  000012 R
+  7 L79  000012 R
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
+  7 _MassStorage_SD1_Init  000000 GR
+  6 _MassStorage_SD1_Loop  000000 GR
+  5 _MassStorage_SD1_Register  000000 GR
+    _MassStorage_Write  ****** GX
+    _SD1_Mounted  ****** GX
+    _SDCKCR  ****** GX
+    _USBStorageRegister  ****** GX
+    _sd1_cache8k_init  ****** GX
+    _sd1_cache_flush  ****** GX
+    _sd1_cache_read  ****** GX
+    _sd1_cache_write  ****** GX
+    _sd1_fat_size  ****** GX
+    _sd1_mount  ****** GX
+  2 _usd1_protect  000000 GR
+  4 _usd1_status  000000 GR
+  3 _usd1_write  000000 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$USB_MassStorage_SD1_c$ size    0   flags 0
+   2 _$usd1_protect size    8   flags 0
+   3 _$usd1_write size   26   flags 0
+   4 _$usd1_status size   18   flags 0
+   5 _$MassStorage_SD1_Register size   8E   flags 0
+   6 _$MassStorage_SD1_Loop size    6   flags 0
+   7 _$MassStorage_SD1_Init size   18   flags 0
+   8 _USB_MS_CODE$RD$USB_MassStorage_SD1_c$ size   1A   flags 0
