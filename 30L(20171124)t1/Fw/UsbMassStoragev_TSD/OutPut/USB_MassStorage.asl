+                       1  
+                       5  
+                       6  .area _$USB_MassStorage_c$(ROM,REL) 
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
+                      58  .area _USB_MS_XRAM$RD$lidian_flag(USB_MS_XRAM,REL,dat) 
+B 00 02
000000                 59  _lidian_flag:: 
000000                 60  	.ds (_1$_lidian_flag - _0$_lidian_flag) 
+                      61  .area _SGV_INIT_CODE(USB_MS_CODE,REL) 
+                      62  .even 
000000                 63  .sgvsym _lidian_flag 
000000                 64  	INIT$global3 _lidian_flag
+B 00 03
000000  68 F0 00 00    64  MOV R0,#^_lidian_flag 
000004  68 F1 00 00    64  MOV R1,#_lidian_flag 
000008  68 F2 00 00    64  MOV R2,#^ _0$_lidian_flag 
00000C  68 F3 00 00    64  MOV R3,# _0$_lidian_flag 
000010  48 41          64  MOV R4,#( _1$_lidian_flag -  _0$_lidian_flag) 
000012                 64  .if KERNEL1_5 = 1
+                      64  CRIT #1
+                      64  .endif
000012  BD 00 00 00    64  SCALL  USB_MS_CODE__INIT$copy 
+                      64  
+                      65  .area _$lidian_flag(USB_MS_CODE,REL) 
+B 00 04
000000                 66  _0$_lidian_flag: 
000000  00             67  	.db 0x0  
000001                 68  _1$_lidian_flag: 
+                      70  
+                      71  .area _$key_down(USB_MS_CODE,REL) 
+B 00 05
000000                 73  _key_down:: 
000000  2E 04          75  	LINK #4 
000002  58 F1          76  	PUSH R1 
000004  58 F2          77  	PUSH R2 
000006  58 F3          78  	PUSH R3 
000008                 80  L42: 
000008                 82  L43: 
000008                 86  L44: 
000008  48 10          87  	MOV R1,#0 
00000A  6A 1B FF FC    88  	MOV [FP+#(-4)],R1 
00000E                 90  L45: 
00000E  48 10          91  	MOV R1,#0 
000010  6A 1B FF FE    92  	MOV [FP+#(-2)],R1 
000014                 94  L46: 
000014  BA F0 00 00    95  	MOVX R0,_rec_led_timer 
000018  BA F1 00 02    96  	MOVX R1,_rec_led_timer+2 
00001C                 97  	SAVE4$stack -8
00001C  6A 0B FF F8    97  MOV [FP+#(-8)],R0 
000020  6A 1B FF FA    97  MOV [FP+#(-8+2)],R1 
+                      97  
000024                 98  	CMP$long$con R0
000024  52 10          98  SUB R1,#(0 & 0xFFFF) 
000026  53 00          98  SUBC R0,#(0 >> 16) 
+                      98  
000028  AC 01 00 B8    99  	AJMP IF_EQ,L36 
00002C                100  	LOAD4$stack R0
00002C  78 0B FF F8   100  MOV R0,[FP+#(-8)] 
000030  78 1B FF FA   100  MOV R1,[FP+#(-8+2)] 
+                     100  
000034  58 F1         101  	PUSH R1 
000036  58 F0         102  	PUSH R0 
000038                103  .if KERNEL1_5 = 1
+                     104  	CRIT #1
+                     105  .endif
000038  BD 00 00 00   106  	SCALL _timer_check_timeout 
00003C  50 F4         107  	ADD SP,#4 
00003E  19 01         108  	MOVZE R0,Rb1 
000040  56 00         109  	CMP R0,#0 
000042  AC 01 00 B8   110  	AJMP IF_EQ,L36 
000046                112  L47: 
000046                114  L48: 
000046                116  L49: 
000046                117  .if KERNEL1_5 = 1
+                     118  	CRIT #1
+                     119  .endif
000046  BD 00 00 00   120  	SCALL _timer_get_time 
00004A  70 F1 00 FA   121  	ADD R1,#250 
00004E  51 00         122  	ADDC R0,#0 
000050  50 11         123  	ADD R1,#1 
000052  51 00         124  	ADDC R0,#0 
000054  AA F0 00 00   125  	MOVX _rec_led_timer,R0 
000058  AA F1 00 02   126  	MOVX _rec_led_timer+2,R1 
00005C                128  L50: 
00005C  48 03         129  	MOV R0,#3 
00005E  58 F0         130  	PUSH R0 
000060                131  .if KERNEL1_5 = 1
+                     132  	CRIT #1
+                     133  .endif
000060  BD 00 00 00   134  	SCALL _Get_ADCValue 
000064  50 F2         135  	ADD SP,#2 
000066  68 10 00 00   136  	MOV DS0,#@_adc3_value 
00006A                137  .if KERNEL1_E = 1
+                     138  	NOP
+                     139  .endif
00006A  A8 F0 00 00   140  	MOV DS0:_adc3_value,R0 
00006E                142  L51: 
+                     143  	
00006E  7C 46 F0 50   144  	BMOV R0.0,0x46.5 
000072  54 01         145  	AND R0,#1 
000074  56 00         146  	CMP R0,#0 
000076  AC 01 00 A8   147  	AJMP IF_EQ,L38 
00007A                149  L52: 
00007A                151  L53: 
00007A                153  L54: 
00007A                154  	LOAD4$memory R0
00007A  68 10 00 00   154  MOV DS0,#@(_time_count) 
00007E  88 F0 00 00   154  MOV R0,DS0:(_time_count) 
000082  88 F1 00 02   154  MOV R1,DS0:(_time_count+2) 
+                     154  
000086                155  	MOVDR R2
000086                155  .if R2=R0
+                     155  .else
000086  08 20         155  MOV R2,R0 
+                     155  .endif
000088                155  .if R3=R1
+                     155  .else
000088  08 31         155  MOV R3,R1 
+                     155  .endif
+                     155  
00008A  50 31         156  	ADD R3,#1 
00008C  51 20         157  	ADDC R2,#0 
00008E                158  	SAVE4$memory _time_count
00008E  68 10 00 00   158  MOV DS0,#@(_time_count) 
000092                158  .if KERNEL1_E = 1
+                     158  NOP
+                     158  .endif
000092  A8 F2 00 00   158  MOV DS0:(_time_count),R2 
000096  A8 F3 00 02   158  MOV DS0:(_time_count+2),R3 
+                     158  
00009A                159  	CMP$long$con R0
00009A  52 12         159  SUB R1,#(2 & 0xFFFF) 
00009C  53 00         159  SUBC R0,#(2 >> 16) 
+                     159  
00009E  AC 0C 00 B8   160  	AJMP IF_ULE,L39 
0000A2                162  L55: 
0000A2                164  L56: 
0000A2                166  L57: 
0000A2  48 01         167  	MOV R0,#1 
0000A4  AC 00 00 BA   168  	AJMP #0,L35 
0000A8                170  L58: 
0000A8                172  L59: 
0000A8                174  L60: 
0000A8                176  L61: 
0000A8                177  L38: 
0000A8                179  L62: 
0000A8                181  L63: 
0000A8                183  L64: 
0000A8  48 10         184  	MOV R1,#0 
0000AA  48 00         185  	MOV R0,#0 
0000AC                186  	SAVE4$memory _time_count
0000AC  68 10 00 00   186  MOV DS0,#@(_time_count) 
0000B0                186  .if KERNEL1_E = 1
+                     186  NOP
+                     186  .endif
0000B0  A8 F0 00 00   186  MOV DS0:(_time_count),R0 
0000B4  A8 F1 00 02   186  MOV DS0:(_time_count+2),R1 
+                     186  
0000B8                188  L65: 
0000B8                190  L66: 
0000B8                191  L39: 
0000B8                193  L67: 
0000B8                195  L68: 
0000B8                196  L36: 
0000B8                198  L69: 
0000B8  48 00         199  	MOV R0,#0 
0000BA                201  L70: 
0000BA                203  L71: 
0000BA                204  L35: 
0000BA  38 3F         205  	POP R3 
0000BC  38 2F         206  	POP R2 
0000BE  38 1F         207  	POP R1 
0000C0                208  	UNLINK_MACRO
0000C0  2F 00         208  UNLINK 
+                     208  
0000C2                209  .if KERNEL1_5 = 1
+                     210  	CRIT #1
+                     211  .endif
0000C2  C6 20         212  	RETS 
+                     214  
+                     215  .area _TEXT$main(USB_MS_CODE,REL) 
+B 00 06
000000                216  _main:: 
000000  2E 03         218  	LINK #3 
000002  58 F0         219  	PUSH R0 
000004  58 F1         220  	PUSH R1 
000006                222  L104: 
000006                224  L105: 
000006                228  L106: 
000006                229  .if KERNEL1_5 = 1
+                     230  	CRIT #1
+                     231  .endif
000006  BD 00 00 00   232  	SCALL _IP_INIT 
00000A                234  L107: 
00000A                235  .if KERNEL1_5 = 1
+                     236  	CRIT #1
+                     237  .endif
00000A  BD 00 00 00   238  	SCALL _ADC_init 
00000E                240  L108: 
00000E  48 03         241  	MOV R0,#3 
000010  58 F0         242  	PUSH R0 
000012                243  .if KERNEL1_5 = 1
+                     244  	CRIT #1
+                     245  .endif
000012  BD 00 00 00   246  	SCALL _SystemClock 
000016  50 F2         247  	ADD SP,#2 
000018                249  L109: 
+                     250  	
000018  F2 4E         251  	BCLR 0x4e.2 
00001A                253  L110: 
+                     254  	
00001A  F5 57         255  	BCLR 0x57.5 
00001C                257  L111: 
+                     258  	
00001C  F6 57         259  	BCLR 0x57.6 
00001E                261  L112: 
+                     262  	
00001E  F7 57         263  	BCLR 0x57.7 
000020                265  L113: 
000020                266  .if KERNEL1_5 = 1
+                     267  	CRIT #1
+                     268  .endif
000020  BD 00 00 00   269  	SCALL _System_Timer3_Init 
000024                271  L114: 
000024                272  .if KERNEL1_5 = 1
+                     273  	CRIT #1
+                     274  .endif
000024  BD 00 00 00   275  	SCALL _UART_Init 
000028                277  L115: 
000028  68 F1 00 28   278  	MOV R1,#(L73) 
00002C  68 F0 00 28   279  	MOV R0,#^(L73) 
000030  58 F1         280  	PUSH R1 
000032  58 F0         281  	PUSH R0 
000034                282  .if KERNEL1_5 = 1
+                     283  	CRIT #1
+                     284  .endif
000034  BD 00 00 00   285  	SCALL _SendString 
000038  50 F4         286  	ADD SP,#4 
00003A                288  L116: 
00003A                289  .if KERNEL1_5 = 1
+                     290  	CRIT #1
+                     291  .endif
00003A  BD 00 00 00   292  	SCALL _MassStorage_Init 
00003E                294  L117: 
00003E                295  .if KERNEL1_5 = 1
+                     296  	CRIT #1
+                     297  .endif
00003E  BD 00 00 00   298  	SCALL _USBStoragePoweroff 
000042                300  L118: 
000042  48 01         301  	MOV R0,#(0x00018000 >> 16) 
000044  68 F1 80 00   302  	MOV R1,#(0x00018000 & 0xFFFF) 
000048  58 F1         303  	PUSH R1 
00004A  58 F0         304  	PUSH R0 
00004C                305  .if KERNEL1_5 = 1
+                     306  	CRIT #1
+                     307  .endif
00004C  BD 00 00 00   308  	SCALL _USBSetDebounceCount 
000050  50 F4         309  	ADD SP,#4 
000052                311  L119: 
000052  48 10         312  	MOV R1,#0 
000054  48 00         313  	MOV R0,#0 
000056                314  	SAVE4$stack -6
000056  6A 0B FF FA   314  MOV [FP+#(-6)],R0 
00005A  6A 1B FF FC   314  MOV [FP+#(-6+2)],R1 
+                     314  
00005E  AC 00 00 8C   315  	AJMP #0,L77 
000062                316  L74: 
000062                318  L120: 
000062                320  L121: 
000062                322  L122: 
000062                323  .if KERNEL1_5 = 1
+                     324  	CRIT #1
+                     325  .endif
000062  BD 00 00 00   326  	SCALL _USBAttachCheck 
000066  6A 0B FF FE   327  	MOV [FP+#(-2)],R0 
00006A                329  L123: 
00006A  78 0B FF FE   330  	MOV R0,[FP+#(-2)] 
00006E  56 00         331  	CMP R0,#0 
000070  AC 01 00 78   332  	AJMP IF_EQ,L78 
000074                334  L124: 
000074  AC 00 00 9E   335  	AJMP #0,L76 
000078                336  L78: 
000078                338  L125: 
000078                340  L126: 
000078                341  L75: 
000078                343  L127: 
000078                344  	LOAD4$stack R0
000078  78 0B FF FA   344  MOV R0,[FP+#(-6)] 
00007C  78 1B FF FC   344  MOV R1,[FP+#(-6+2)] 
+                     344  
000080  50 11         345  	ADD R1,#1 
000082  51 00         346  	ADDC R0,#0 
000084                347  	SAVE4$stack -6
000084  6A 0B FF FA   347  MOV [FP+#(-6)],R0 
000088  6A 1B FF FC   347  MOV [FP+#(-6+2)],R1 
+                     347  
00008C                348  L77: 
00008C                350  L128: 
00008C                351  	LOAD4$stack R0
00008C  78 0B FF FA   351  MOV R0,[FP+#(-6)] 
000090  78 1B FF FC   351  MOV R1,[FP+#(-6+2)] 
+                     351  
000094                352  	CMP$long$con R0
000094  52 10         352  SUB R1,#(0x00100000 & 0xFFFF) 
000096  73 F0 00 10   352  SUBC R0,#(0x00100000 >> 16) 
+                     352  
00009A  AC 0C 00 62   353  	AJMP IF_ULE,L74 
00009E                354  L76: 
00009E                356  L129: 
00009E  78 0B FF FE   357  	MOV R0,[FP+#(-2)] 
0000A2  56 00         358  	CMP R0,#0 
0000A4  AC 02 00 BE   359  	AJMP IF_NE,L80 
0000A8                361  L130: 
0000A8                363  L131: 
0000A8                365  L132: 
0000A8  68 F1 00 1A   366  	MOV R1,#(L82) 
0000AC  68 F0 00 1A   367  	MOV R0,#^(L82) 
0000B0  58 F1         368  	PUSH R1 
0000B2  58 F0         369  	PUSH R0 
0000B4                370  .if KERNEL1_5 = 1
+                     371  	CRIT #1
+                     372  .endif
0000B4  BD 00 00 00   373  	SCALL _SendString 
0000B8  50 F4         374  	ADD SP,#4 
0000BA                376  L133: 
0000BA  AC 00 01 64   377  	AJMP #0,L83 
0000BE                379  L134: 
0000BE                381  L135: 
0000BE                382  L80: 
0000BE                384  L136: 
0000BE                385  .if KERNEL1_5 = 1
+                     386  	CRIT #1
+                     387  .endif
0000BE  BD 00 00 00   388  	SCALL _timer_get_time 
0000C2  70 F1 07 D0   389  	ADD R1,#2000 
0000C6  51 00         390  	ADDC R0,#0 
0000C8  50 11         391  	ADD R1,#1 
0000CA  51 00         392  	ADDC R0,#0 
0000CC  AA F0 00 00   393  	MOVX _usb_nosd_timer,R0 
0000D0  AA F1 00 02   394  	MOVX _usb_nosd_timer+2,R1 
0000D4                396  L137: 
0000D4  48 00         397  	MOV R0,#0 
0000D6  68 10 00 00   398  	MOV DS0,#@_usb_core_status 
0000DA                399  .if KERNEL1_E = 1
+                     400  	NOP
+                     401  .endif
0000DA  A8 F0 00 00   402  	MOV DS0:_usb_core_status,R0 
0000DE                404  L138: 
0000DE  49 10         405  	MOVB Rb1,#0 
0000E0  AB F1 00 00   406  	MOVBX _usb_count,Rb1 
0000E4                408  L139: 
0000E4  49 11         409  	MOVB Rb1,#1 
0000E6  68 10 00 00   410  	MOV DS0,#@_b_usb_detection 
0000EA                411  .if KERNEL1_E = 1
+                     412  	NOP
+                     413  .endif
0000EA  A9 F1 00 00   414  	MOVB DS0:_b_usb_detection,Rb1 
0000EE                416  L140: 
0000EE  49 10         417  	MOVB Rb1,#0 
0000F0  68 10 00 00   418  	MOV DS0,#@_adpate_flag 
0000F4                419  .if KERNEL1_E = 1
+                     420  	NOP
+                     421  .endif
0000F4  A9 F1 00 00   422  	MOVB DS0:_adpate_flag,Rb1 
0000F8                424  L141: 
0000F8                425  .if KERNEL1_5 = 1
+                     426  	CRIT #1
+                     427  .endif
0000F8  BD 00 00 00   428  	SCALL _MassStorage_Registers 
0000FC                430  L142: 
0000FC  AC 00 01 30   431  	AJMP #0,L85 
000100                432  L84: 
000100                434  L143: 
000100                436  L144: 
000100                438  L145: 
000100                439  .if KERNEL1_5 = 1
+                     440  	CRIT #1
+                     441  .endif
000100  BD 00 00 00   442  	SCALL _MassStorage_Loop 
000104  56 01         443  	CMP R0,#1 
000106  AC 02 01 0E   444  	AJMP IF_NE,L87 
00010A                446  L146: 
00010A  AC 00 01 34   447  	AJMP #0,L86 
00010E                448  L87: 
00010E                450  L147: 
00010E  68 10 00 00   451  	MOV DS0,#@_adpate_flag 
000112  99 F0 00 00   452  	MOVZE R0,DS0:_adpate_flag 
000116  56 00         453  	CMP R0,#0 
000118  AC 01 01 20   454  	AJMP IF_EQ,L89 
00011C                456  L148: 
00011C  AC 00 01 64   457  	AJMP #0,L83 
000120                458  L89: 
000120                460  L149: 
000120                461  .if KERNEL1_5 = 1
+                     462  	CRIT #1
+                     463  .endif
000120  BD 00 00 00   464  	SCALL _key_down 
000124  19 01         465  	MOVZE R0,Rb1 
000126  56 00         466  	CMP R0,#0 
000128  AC 01 01 30   467  	AJMP IF_EQ,L91 
00012C                469  L150: 
00012C                471  L151: 
00012C                473  L152: 
00012C  AC 00 01 34   474  	AJMP #0,L86 
000130                476  L153: 
000130                478  L154: 
000130                479  L91: 
000130                481  L155: 
000130                483  L156: 
000130                484  L85: 
000130                486  L157: 
000130  AC 00 01 00   487  	AJMP #0,L84 
000134                488  L86: 
000134                490  L158: 
000134                491  .if KERNEL1_5 = 1
+                     492  	CRIT #1
+                     493  .endif
000134  BD 00 00 00   494  	SCALL _MassStorage_Exit 
000138                495  L93: 
000138                497  L159: 
000138  68 F0 80 00   498  	MOV R0,#0x8000 
00013C  68 10 00 00   499  	MOV DS0,#@(_gflag) 
000140  B5 F0 00 00   500  	OR DS0:(_gflag),R0 
000144                502  L160: 
000144  68 F1 00 0E   503  	MOV R1,#(L94) 
000148  68 F0 00 0E   504  	MOV R0,#^(L94) 
00014C  58 F1         505  	PUSH R1 
00014E  58 F0         506  	PUSH R0 
000150                507  .if KERNEL1_5 = 1
+                     508  	CRIT #1
+                     509  .endif
000150  BD 00 00 00   510  	SCALL _SendString 
000154  50 F4         511  	ADD SP,#4 
000156                513  L161: 
000156  48 01         514  	MOV R0,#1 
000158  58 F0         515  	PUSH R0 
00015A                516  .if KERNEL1_5 = 1
+                     517  	CRIT #1
+                     518  .endif
00015A  BD 00 00 00   519  	SCALL _CommonLoadCode 
00015E  50 F2         520  	ADD SP,#2 
000160                522  L162: 
000160                523  L95: 
000160                525  L163: 
000160                526  L96: 
000160                528  L164: 
000160  AC 00 01 60   529  	AJMP #0,L95 
000164                530  L83: 
000164                532  L165: 
000164  68 F1 00 00   533  	MOV R1,#(L98) 
000168  68 F0 00 00   534  	MOV R0,#^(L98) 
00016C  58 F1         535  	PUSH R1 
00016E  58 F0         536  	PUSH R0 
000170                537  .if KERNEL1_5 = 1
+                     538  	CRIT #1
+                     539  .endif
000170  BD 00 00 00   540  	SCALL _SendString 
000174  50 F4         541  	ADD SP,#4 
000176                543  L166: 
000176  AC 00 01 8A   544  	AJMP #0,L100 
00017A                545  L99: 
00017A                547  L167: 
00017A                549  L168: 
00017A                551  L169: 
00017A                552  .if KERNEL1_5 = 1
+                     553  	CRIT #1
+                     554  .endif
00017A  BD 00 00 00   555  	SCALL _key_down 
00017E  19 01         556  	MOVZE R0,Rb1 
000180  56 00         557  	CMP R0,#0 
000182  AC 01 01 8A   558  	AJMP IF_EQ,L102 
000186                560  L170: 
000186  AC 00 01 38   561  	AJMP #0,L93 
00018A                562  L102: 
00018A                564  L171: 
00018A                566  L172: 
00018A                567  L100: 
00018A                569  L173: 
00018A  AC 00 01 7A   570  	AJMP #0,L99 
00018E                572  L174: 
00018E                574  L175: 
00018E  38 1F         575  	POP R1 
000190  38 0F         576  	POP R0 
000192                577  	UNLINK_MACRO
000192  2F 00         577  UNLINK 
+                     577  
000194                578  .if KERNEL1_5 = 1
+                     579  	CRIT #1
+                     580  .endif
000194  C6 20         581  	RETS 
+                     771  
+                     772  .area _$USB_MassStorage_c$(ROM,REL) 
+B 00 01
000000                774  Letext: 
+                     775  
+                     776  .area _USB_MS_XRAM$RD$usb_core_status(USB_MS_XRAM,REL,dat) 
+                     777  .even 
+B 00 07
000000                778  _usb_core_status:: 
000000                779  	.ds 2 
+                     781  
+                     782  .area _USB_MS_XRAM$RD$b_usb_detection(USB_MS_XRAM,REL,dat) 
+B 00 08
000000                783  _b_usb_detection:: 
000000                784  	.ds 1 
+                     786  
+                     787  .area _XRAM$RD$usb_count(XRAM,REL,dat) 
+B 00 09
000000                788  _usb_count:: 
000000                789  	.ds 1 
+                     791  
+                     792  .area _XRAM$RD$usb_nosd_timer(XRAM,REL,dat) 
+                     793  .even 
+B 00 0A
000000                794  _usb_nosd_timer:: 
000000                795  	.ds 4 
+                     797  
+                     798  .area _USB_MS_XRAM$RD$adpate_flag(USB_MS_XRAM,REL,dat) 
+B 00 0B
000000                799  _adpate_flag:: 
000000                800  	.ds 1 
+                     802  
+                     803  .area _USB_MS_XRAM$RD$vid_data(USB_MS_XRAM,REL,dat) 
+                     804  .even 
+B 00 0C
000000                805  _vid_data:: 
000000                806  	.ds 2 
+                     808  
+                     809  .area _USB_MS_XRAM$RD$flag_chongdian(USB_MS_XRAM,REL,dat) 
+                     810  .even 
+B 00 0D
000000                811  _flag_chongdian:: 
000000                812  	.ds 2 
+                     814  
+                     815  .area _USB_MS_XRAM$RD$usb_attach_time(USB_MS_XRAM,REL,dat) 
+                     816  .even 
+B 00 0E
000000                817  _usb_attach_time:: 
000000                818  	.ds 4 
+                     820  
+                     821  .area _USB_MS_XRAM$RD$time_count(USB_MS_XRAM,REL,dat) 
+                     822  .even 
+B 00 0F
000000                823  _time_count:: 
000000                824  	.ds 4 
+                     826  
+                     827  .area _USB_MS_XRAM$RD$g_exit_usb(USB_MS_XRAM,REL,dat) 
+                     828  .even 
+B 00 10
000000                829  _g_exit_usb:: 
000000                830  	.ds 2 
+                     832  
+                     833  
+                     834  
+                     835  
+                     836  
+                     837  
+                     838  
+                     839  
+                     840  
+                     841  
+                     842  
+                     843  
+                     844  
+                     845  
+                     846  
+                     847  
+                     848  
+                     849  
+                     850  
+                     851  
+                     852  
+                     853  .area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL) 
+B 00 11
000000                854  L98: 
000000  0D            855  	.db 13 
000001  0A            856  	.db 10 
+                     857  	
000002  75 73 62 20   858  	.db 117,115,98,32,97,100,112,97,116,101,33,0 
000006  61 64 70 61
00000A  74 65 21 00
+                     859  
+                     860  .area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL) 
+B 00 11
00000E                861  L94: 
00000E  0D            862  	.db 13 
00000F  0A            863  	.db 10 
+                     864  	
000010  75 73 62 20   865  	.db 117,115,98,32,101,120,105,116,33,0 
000014  65 78 69 74
000018  21 00
+                     866  
+                     867  .area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL) 
+B 00 11
00001A                868  L82: 
00001A  0D            869  	.db 13 
00001B  0A            870  	.db 10 
+                     871  	
00001C  75 73 62 20   872  	.db 117,115,98,32,97,100,112,101,97,116,33,0 
000020  61 64 70 65
000024  61 74 21 00
+                     873  
+                     874  .area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL) 
+B 00 11
000028                875  L73: 
000028  0D            876  	.db 13 
000029  0A            877  	.db 10 
+                     878  	
00002A  4B 42 20 75   879  	.db 75,66,32,117,115,98,32,98,101,103,105,110,33,0 
00002E  73 62 20 62
000032  65 67 69 6E
000036  21 00
+                     880  
+                     881  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+  6 L100  00018A R
+  6 L102  00018A R
+  6 L104  000006 R
+  6 L105  000006 R
+  6 L106  000006 R
+  6 L107  00000A R
+  6 L108  00000E R
+  6 L109  000018 R
+  6 L110  00001A R
+  6 L111  00001C R
+  6 L112  00001E R
+  6 L113  000020 R
+  6 L114  000024 R
+  6 L115  000028 R
+  6 L116  00003A R
+  6 L117  00003E R
+  6 L118  000042 R
+  6 L119  000052 R
+  6 L120  000062 R
+  6 L121  000062 R
+  6 L122  000062 R
+  6 L123  00006A R
+  6 L124  000074 R
+  6 L125  000078 R
+  6 L126  000078 R
+  6 L127  000078 R
+  6 L128  00008C R
+  6 L129  00009E R
+  6 L130  0000A8 R
+  6 L131  0000A8 R
+  6 L132  0000A8 R
+  6 L133  0000BA R
+  6 L134  0000BE R
+  6 L135  0000BE R
+  6 L136  0000BE R
+  6 L137  0000D4 R
+  6 L138  0000DE R
+  6 L139  0000E4 R
+  6 L140  0000EE R
+  6 L141  0000F8 R
+  6 L142  0000FC R
+  6 L143  000100 R
+  6 L144  000100 R
+  6 L145  000100 R
+  6 L146  00010A R
+  6 L147  00010E R
+  6 L148  00011C R
+  6 L149  000120 R
+  6 L150  00012C R
+  6 L151  00012C R
+  6 L152  00012C R
+  6 L153  000130 R
+  6 L154  000130 R
+  6 L155  000130 R
+  6 L156  000130 R
+  6 L157  000130 R
+  6 L158  000134 R
+  6 L159  000138 R
+  6 L160  000144 R
+  6 L161  000156 R
+  6 L162  000160 R
+  6 L163  000160 R
+  6 L164  000160 R
+  6 L165  000164 R
+  6 L166  000176 R
+  6 L167  00017A R
+  6 L168  00017A R
+  6 L169  00017A R
+  6 L170  000186 R
+  6 L171  00018A R
+  6 L172  00018A R
+  6 L173  00018A R
+  6 L174  00018E R
+  6 L175  00018E R
+  5 L35  0000BA R
+  5 L36  0000B8 R
+  5 L38  0000A8 R
+  5 L39  0000B8 R
+  5 L42  000008 R
+  5 L43  000008 R
+  5 L44  000008 R
+  5 L45  00000E R
+  5 L46  000014 R
+  5 L47  000046 R
+  5 L48  000046 R
+  5 L49  000046 R
+  5 L50  00005C R
+  5 L51  00006E R
+  5 L52  00007A R
+  5 L53  00007A R
+  5 L54  00007A R
+  5 L55  0000A2 R
+  5 L56  0000A2 R
+  5 L57  0000A2 R
+  5 L58  0000A8 R
+  5 L59  0000A8 R
+  5 L60  0000A8 R
+  5 L61  0000A8 R
+  5 L62  0000A8 R
+  5 L63  0000A8 R
+  5 L64  0000A8 R
+  5 L65  0000B8 R
+  5 L66  0000B8 R
+  5 L67  0000B8 R
+  5 L68  0000B8 R
+  5 L69  0000B8 R
+  5 L70  0000BA R
+  5 L71  0000BA R
+ 11 L73  000028 R
+  6 L74  000062 R
+  6 L75  000078 R
+  6 L76  00009E R
+  6 L77  00008C R
+  6 L78  000078 R
+  6 L80  0000BE R
+ 11 L82  00001A R
+  6 L83  000164 R
+  6 L84  000100 R
+  6 L85  000130 R
+  6 L86  000134 R
+  6 L87  00010E R
+  6 L89  000120 R
+  6 L91  000130 R
+  6 L93  000138 R
+ 11 L94  00000E R
+  6 L95  000160 R
+  6 L96  000160 R
+ 11 L98  000000 R
+  6 L99  00017A R
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
+    USB_MS_CODE__INIT$copy  ****** GX
+  4 _0$_lidian_flag  000000 R
+  4 _1$_lidian_flag  000001 R
+    _ADC_init  ****** GX
+    _CommonLoadCode  ****** GX
+    _Get_ADCValue  ****** GX
+    _IP_INIT  ****** GX
+    _MassStorage_Exit  ****** GX
+    _MassStorage_Init  ****** GX
+    _MassStorage_Loop  ****** GX
+    _MassStorage_Registers  ****** GX
+    _SendString  ****** GX
+    _SystemClock  ****** GX
+    _System_Timer3_Init  ****** GX
+    _UART_Init  ****** GX
+    _USBAttachCheck  ****** GX
+    _USBSetDebounceCount  ****** GX
+    _USBStoragePoweroff  ****** GX
+    _adc3_value  ****** GX
+  B _adpate_flag  000000 GR
+  8 _b_usb_detection  000000 GR
+  D _flag_chongdian  000000 GR
+ 10 _g_exit_usb  000000 GR
+    _gflag  ****** GX
+  5 _key_down  000000 GR
+  2 _lidian_flag  000000 GR
+  6 _main  000000 GR
+    _rec_led_timer  ****** GX
+  F _time_count  000000 GR
+    _timer_check_timeout  ****** GX
+    _timer_get_time  ****** GX
+  E _usb_attach_time  000000 GR
+  7 _usb_core_status  000000 GR
+  9 _usb_count  000000 GR
+  A _usb_nosd_timer  000000 GR
+  C _vid_data  000000 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$USB_MassStorage_c$ size    0   flags 0
+   2 _USB_MS_XRAM$RD$lidian_flag size    2   flags 0
+   3 _SGV_INIT_CODE size   16   flags 0
+   4 _$lidian_flag size    2   flags 0
+   5 _$key_down size   C4   flags 0
+   6 _TEXT$main size  196   flags 0
+   7 _USB_MS_XRAM$RD$usb_core_status size    2   flags 0
+   8 _USB_MS_XRAM$RD$b_usb_detection size    2   flags 0
+   9 _XRAM$RD$usb_count size    2   flags 0
+   A _XRAM$RD$usb_nosd_timer size    4   flags 0
+   B _USB_MS_XRAM$RD$adpate_flag size    2   flags 0
+   C _USB_MS_XRAM$RD$vid_data size    2   flags 0
+   D _USB_MS_XRAM$RD$flag_chongdian size    2   flags 0
+   E _USB_MS_XRAM$RD$usb_attach_time size    4   flags 0
+   F _USB_MS_XRAM$RD$time_count size    4   flags 0
+  10 _USB_MS_XRAM$RD$g_exit_usb size    2   flags 0
+  11 _USB_MS_CODE$RD$USB_MassStorage_c$ size   38   flags 0
