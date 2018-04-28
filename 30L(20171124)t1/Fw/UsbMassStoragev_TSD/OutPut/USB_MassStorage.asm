; Generated by KBCC
.stabs "KBCC",0x3c,0,0,0
.stabs "E:\work\kb\code\80VT\80VT(20160918)t1\Fw\UsbMassStoragev_TSD",0x64,0,3,Ltext0
.stabs "E:\work\kb\code\80VT\80VT(20160918)t1\Fw\UsbMassStoragev_TSD\Src\USB_MassStorage.c",0x64,0,3,Ltext0

.area _$USB_MassStorage_c$(ROM,REL)
Ltext0:
.stabs "int:t1=r1;-32768;32767;",128,0,0,0
.stabs "char:t2=r2;-128;127;",128,0,0,0
.stabs "double:t3=r1;8;0;",128,0,0,0
.stabs "float:t4=r1;4;0;",128,0,0,0
.stabs "long double:t5=r1;8;0;",128,0,0,0
.stabs "long int:t6=r1;-2147483648;2147483647;",128,0,0,0
.stabs "long long int:t7=r1;-2147483648;2147483647;",128,0,0,0
.stabs "short:t8=r1;-32768;32767;",128,0,0,0
.stabs "signed char:t9=r1;-128;127;",128,0,0,0
.stabs "unsigned char:t10=r1;0;255;",128,0,0,0
.stabs "unsigned long:t11=r1;0;4294967295;",128,0,0,0
.stabs "unsigned long long:t12=r1;0;4294967295;",128,0,0,0
.stabs "unsigned short:t13=r1;0;65535;",128,0,0,0
.stabs "unsigned int:t14=r1;0;65535;",128,0,0,0
.stabs "void:t15=15",128,0,0,0
.stabs "short __fixed:t16=r1;2;0;",128,0,0,0
.stabs "__fixed:t17=r1;2;0;",128,0,0,0
.stabs "long __fixed:t18=r1;4;0;",128,0,0,0
.stabs "unsigned short __fixed:t19=r1;2;0;",128,0,0,0
.stabs "unsigned __fixed:t20=r1;2;0;",128,0,0,0
.stabs "unsigned long __fixed:t21=r1;4;0;",128,0,0,0
.stabs "__sat short __fixed:t22=r1;2;0;",128,0,0,0
.stabs "__sat __fixed:t23=r1;2;0;",128,0,0,0
.stabs "__sat long __fixed:t24=r1;4;0;",128,0,0,0
.stabs "__sat unsigned short __fixed:t25=r1;2;0;",128,0,0,0
.stabs "__sat unsigned __fixed:t26=r1;2;0;",128,0,0,0
.stabs "__sat unsigned long __fixed:t27=r1;4;0;",128,0,0,0
.stabs "short __accum:t28=r1;4;0;",128,0,0,0
.stabs "__accum:t29=r1;4;0;",128,0,0,0
.stabs "long __accum:t30=r1;6;0;",128,0,0,0
.stabs "unsigned short __accum:t31=r1;4;0;",128,0,0,0
.stabs "unsigned __accum:t32=r1;4;0;",128,0,0,0
.stabs "unsigned long __accum:t33=r1;6;0;",128,0,0,0
.include "kbccmacro.asm"
 .define _sdtl_mount	 _sd1_mount        
 .define _sdtl_resv_size	 _sd1_res_size     
 .define _sdtl_get_size	 _sd1_fat_size     
 .define _sdtl_resv_read	 _sd1_res_read     
 .define _sdtl_resv_write	 _sd1_res_write    
 .define _sdtl_read	 _sd1_read         
 .define _sdtl_write	 _sd1_write        
 .define _sdtl_cache_init	 _sd1_cache2k_init 
 .define _sdtl_cache_read	 _sd1_cache_read   
 .define _sdtl_cache_write _sd1_cache_write  
 .define _sdtl_cache_flush _sd1_cache_flush  
 .define _sdtl_allocate    _sd1_allocate     
 .define _spif_res_size	 _spif_resv_size   
 .define _spif_res_read	 _spif_resv_read   
 .define _spif_res_write	 _spif_resv_write  

.area _USB_MS_XRAM$RD$lidian_flag(USB_MS_XRAM,REL,dat)
_lidian_flag::
	.ds (_1$_lidian_flag - _0$_lidian_flag)
.area _SGV_INIT_CODE(USB_MS_CODE,REL)
.even
.sgvsym _lidian_flag
	INIT$global3 _lidian_flag, _0$_lidian_flag, _1$_lidian_flag, USB_MS_CODE__INIT$copy
.area _$lidian_flag(USB_MS_CODE,REL)
_0$_lidian_flag:
	.db 0x0 ; 0
_1$_lidian_flag:
.stabs "lidian_flag:G10",32,0,4,0

.area _$key_down(USB_MS_CODE,REL)
.stabs "key_down:F10",36,0,4,_key_down
_key_down::
.cfundef "E:\work\kb\code\80VT\80VT(20160918)t1\Fw\UsbMassStoragev_TSD\Src\USB_MassStorage.c","key_down",27
	LINK #4
	PUSH R1
	PUSH R2
	PUSH R3
.stabn 0xc0,0,0,L42-_key_down
L42:
.stabn 0x44,0,28,L43-_key_down
L43:
.stabs "count:14",128,0,0,-2
.stabs "adc_value:14",128,0,0,-4
.stabn 0x44,0,31,L44-_key_down
L44:
	MOV R1,#0
	MOV [FP+#(-4)],R1
.stabn 0x44,0,32,L45-_key_down
L45:
	MOV R1,#0
	MOV [FP+#(-2)],R1
.stabn 0x44,0,37,L46-_key_down
L46:
	MOVX R0,_rec_led_timer
	MOVX R1,_rec_led_timer+2
	SAVE4$stack -8,FP,R0,R1
	CMP$long$con R0,R1,0
	AJMP IF_EQ,L36
	LOAD4$stack R0,R1,-8,FP
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _timer_check_timeout
	ADD SP,#4
	MOVZE R0,Rb1
	CMP R0,#0
	AJMP IF_EQ,L36
.stabn 0xc0,0,1,L47-_key_down
L47:
.stabn 0x44,0,37,L48-_key_down
L48:
.stabn 0x44,0,38,L49-_key_down
L49:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _timer_get_time
	ADD R1,#250
	ADDC R0,#0
	ADD R1,#1
	ADDC R0,#0
	MOVX _rec_led_timer,R0
	MOVX _rec_led_timer+2,R1
.stabn 0x44,0,39,L50-_key_down
L50:
	MOV R0,#3
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _Get_ADCValue
	ADD SP,#2
	MOV DS0,#@_adc3_value
.if KERNEL1_E = 1
	NOP
.endif
	MOV DS0:_adc3_value,R0
.stabn 0x44,0,89,L51-_key_down
L51:
	; intrinsic function: __bit_get
	BMOV R0.0,0x46.5
	AND R0,#1
	CMP R0,#0
	AJMP IF_EQ,L38
.stabn 0xc0,0,2,L52-_key_down
L52:
.stabn 0x44,0,90,L53-_key_down
L53:
.stabn 0x44,0,91,L54-_key_down
L54:
	LOAD4$memory R0,R1,_time_count
	MOVDR R2,R3,R0,R1
	ADD R3,#1
	ADDC R2,#0
	SAVE4$memory _time_count,R2,R3; _time_count=R2,R3
	CMP$long$con R0,R1,2
	AJMP IF_ULE,L39
.stabn 0xc0,0,3,L55-_key_down
L55:
.stabn 0x44,0,91,L56-_key_down
L56:
.stabn 0x44,0,92,L57-_key_down
L57:
	MOV R0,#1
	AJMP #0,L35
.stabn 0xe0,0,3,L58-_key_down
L58:
.stabn 0x44,0,93,L59-_key_down
L59:
.stabn 0xe0,0,2,L60-_key_down
L60:
.stabn 0x44,0,94,L61-_key_down
L61:
L38:
.stabn 0xc0,0,2,L62-_key_down
L62:
.stabn 0x44,0,95,L63-_key_down
L63:
.stabn 0x44,0,96,L64-_key_down
L64:
	MOV R1,#0
	MOV R0,#0
	SAVE4$memory _time_count,R0,R1; _time_count=R0,R1
.stabn 0xe0,0,2,L65-_key_down
L65:
.stabn 0x44,0,97,L66-_key_down
L66:
L39:
.stabn 0xe0,0,1,L67-_key_down
L67:
.stabn 0x44,0,98,L68-_key_down
L68:
L36:
.stabn 0x44,0,103,L69-_key_down
L69:
	MOV R0,#0
.stabn 0xe0,0,0,L70-_key_down
L70:
.stabn 0x44,0,104,L71-_key_down
L71:
L35:
	POP R3
	POP R2
	POP R1
	UNLINK_MACRO
.if KERNEL1_5 = 1
	CRIT #1
.endif
	RETS
.stabs "main:F15",36,0,4,_main

.area _TEXT$main(USB_MS_CODE,REL)
_main::
.cfundef "E:\work\kb\code\80VT\80VT(20160918)t1\Fw\UsbMassStoragev_TSD\Src\USB_MassStorage.c","main",107
	LINK #3
	PUSH R0
	PUSH R1
.stabn 0xc0,0,0,L104-_main
L104:
.stabn 0x44,0,108,L105-_main
L105:
.stabs "flag:14",128,0,0,-2
.stabs "i:11",128,0,0,-6
.stabn 0x44,0,111,L106-_main
L106:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _IP_INIT
.stabn 0x44,0,114,L107-_main
L107:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _ADC_init
.stabn 0x44,0,115,L108-_main
L108:
	MOV R0,#3
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _SystemClock
	ADD SP,#2
.stabn 0x44,0,116,L109-_main
L109:
	; intrinsic function: __bitCLR
	BCLR 0x4e.2
.stabn 0x44,0,117,L110-_main
L110:
	; intrinsic function: __bitCLR
	BCLR 0x57.5
.stabn 0x44,0,118,L111-_main
L111:
	; intrinsic function: __bitCLR
	BCLR 0x57.6
.stabn 0x44,0,120,L112-_main
L112:
	; intrinsic function: __bitCLR
	BCLR 0x57.7
.stabn 0x44,0,125,L113-_main
L113:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _System_Timer3_Init
.stabn 0x44,0,128,L114-_main
L114:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _UART_Init
.stabn 0x44,0,129,L115-_main
L115:
	MOV R1,#(L73)
	MOV R0,#^(L73)
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _SendString
	ADD SP,#4
.stabn 0x44,0,132,L116-_main
L116:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _MassStorage_Init
.stabn 0x44,0,134,L117-_main
L117:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _USBStoragePoweroff
.stabn 0x44,0,135,L118-_main
L118:
	MOV R0,#(0x00018000 >> 16)
	MOV R1,#(0x00018000 & 0xFFFF)
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _USBSetDebounceCount
	ADD SP,#4
.stabn 0x44,0,142,L119-_main
L119:
	MOV R1,#0
	MOV R0,#0
	SAVE4$stack -6,FP,R0,R1
	AJMP #0,L77
L74:
.stabn 0xc0,0,1,L120-_main
L120:
.stabn 0x44,0,143,L121-_main
L121:
.stabn 0x44,0,144,L122-_main
L122:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _USBAttachCheck
	MOV [FP+#(-2)],R0
.stabn 0x44,0,147,L123-_main
L123:
	MOV R0,[FP+#(-2)]
	CMP R0,#0
	AJMP IF_EQ,L78
.stabn 0x44,0,147,L124-_main
L124:
	AJMP #0,L76
L78:
.stabn 0xe0,0,1,L125-_main
L125:
.stabn 0x44,0,148,L126-_main
L126:
L75:
.stabn 0x44,0,142,L127-_main
L127:
	LOAD4$stack R0,R1,-6,FP
	ADD R1,#1
	ADDC R0,#0
	SAVE4$stack -6,FP,R0,R1
L77:
.stabn 0x44,0,142,L128-_main
L128:
	LOAD4$stack R0,R1,-6,FP
	CMP$long$con R0,R1,0x00100000
	AJMP IF_ULE,L74
L76:
.stabn 0x44,0,149,L129-_main
L129:
	MOV R0,[FP+#(-2)]
	CMP R0,#0
	AJMP IF_NE,L80
.stabn 0xc0,0,1,L130-_main
L130:
.stabn 0x44,0,150,L131-_main
L131:
.stabn 0x44,0,152,L132-_main
L132:
	MOV R1,#(L82)
	MOV R0,#^(L82)
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _SendString
	ADD SP,#4
.stabn 0x44,0,154,L133-_main
L133:
	AJMP #0,L83
.stabn 0xe0,0,1,L134-_main
L134:
.stabn 0x44,0,155,L135-_main
L135:
L80:
.stabn 0x44,0,157,L136-_main
L136:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _timer_get_time
	ADD R1,#2000
	ADDC R0,#0
	ADD R1,#1
	ADDC R0,#0
	MOVX _usb_nosd_timer,R0
	MOVX _usb_nosd_timer+2,R1
.stabn 0x44,0,160,L137-_main
L137:
	MOV R0,#0
	MOV DS0,#@_usb_core_status
.if KERNEL1_E = 1
	NOP
.endif
	MOV DS0:_usb_core_status,R0
.stabn 0x44,0,161,L138-_main
L138:
	MOVB Rb1,#0
	MOVBX _usb_count,Rb1
.stabn 0x44,0,162,L139-_main
L139:
	MOVB Rb1,#1
	MOV DS0,#@_b_usb_detection
.if KERNEL1_E = 1
	NOP
.endif
	MOVB DS0:_b_usb_detection,Rb1
.stabn 0x44,0,163,L140-_main
L140:
	MOVB Rb1,#0
	MOV DS0,#@_adpate_flag
.if KERNEL1_E = 1
	NOP
.endif
	MOVB DS0:_adpate_flag,Rb1
.stabn 0x44,0,164,L141-_main
L141:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _MassStorage_Registers
.stabn 0x44,0,167,L142-_main
L142:
	AJMP #0,L85
L84:
.stabn 0xc0,0,1,L143-_main
L143:
.stabn 0x44,0,168,L144-_main
L144:
.stabn 0x44,0,169,L145-_main
L145:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _MassStorage_Loop
	CMP R0,#1
	AJMP IF_NE,L87
.stabn 0x44,0,170,L146-_main
L146:
	AJMP #0,L86
L87:
.stabn 0x44,0,171,L147-_main
L147:
	MOV DS0,#@_adpate_flag
	MOVZE R0,DS0:_adpate_flag
	CMP R0,#0
	AJMP IF_EQ,L89
.stabn 0x44,0,171,L148-_main
L148:
	AJMP #0,L83
L89:
.stabn 0x44,0,172,L149-_main
L149:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _key_down
	MOVZE R0,Rb1
	CMP R0,#0
	AJMP IF_EQ,L91
.stabn 0xc0,0,2,L150-_main
L150:
.stabn 0x44,0,172,L151-_main
L151:
.stabn 0x44,0,173,L152-_main
L152:
	AJMP #0,L86
.stabn 0xe0,0,2,L153-_main
L153:
.stabn 0x44,0,174,L154-_main
L154:
L91:
.stabn 0xe0,0,1,L155-_main
L155:
.stabn 0x44,0,175,L156-_main
L156:
L85:
.stabn 0x44,0,167,L157-_main
L157:
	AJMP #0,L84
L86:
.stabn 0x44,0,176,L158-_main
L158:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _MassStorage_Exit
L93:
.stabn 0x44,0,178,L159-_main
L159:
	MOV R0,#0x8000
	MOV DS0,#@(_gflag)
	OR DS0:(_gflag),R0
.stabn 0x44,0,180,L160-_main
L160:
	MOV R1,#(L94)
	MOV R0,#^(L94)
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _SendString
	ADD SP,#4
.stabn 0x44,0,182,L161-_main
L161:
	MOV R0,#1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _CommonLoadCode
	ADD SP,#2
.stabn 0x44,0,183,L162-_main
L162:
L95:
.stabn 0x44,0,183,L163-_main
L163:
L96:
.stabn 0x44,0,183,L164-_main
L164:
	AJMP #0,L95
L83:
.stabn 0x44,0,186,L165-_main
L165:
	MOV R1,#(L98)
	MOV R0,#^(L98)
	PUSH R1
	PUSH R0
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _SendString
	ADD SP,#4
.stabn 0x44,0,188,L166-_main
L166:
	AJMP #0,L100
L99:
.stabn 0xc0,0,1,L167-_main
L167:
.stabn 0x44,0,188,L168-_main
L168:
.stabn 0x44,0,189,L169-_main
L169:
.if KERNEL1_5 = 1
	CRIT #1
.endif
	SCALL _key_down
	MOVZE R0,Rb1
	CMP R0,#0
	AJMP IF_EQ,L102
.stabn 0x44,0,189,L170-_main
L170:
	AJMP #0,L93
L102:
.stabn 0xe0,0,1,L171-_main
L171:
.stabn 0x44,0,190,L172-_main
L172:
L100:
.stabn 0x44,0,188,L173-_main
L173:
	AJMP #0,L99
.stabn 0xe0,0,0,L174-_main
L174:
.stabn 0x44,0,191,L175-_main
L175:
	POP R1
	POP R0
	UNLINK_MACRO
.if KERNEL1_5 = 1
	CRIT #1
.endif
	RETS
.stabs "34:T34=eHEADER_PHONE:0,SPEAKER:1,;",128,0,0,0
.stabs "33:T35=s2bRev0:14,0,1;bRev1:14,1,1;bRev2:14,2,1;bRev3:14,3,1;\\",128,0,0,0
.stabs "bRev4:14,4,1;bRev5:14,5,1;bRev6:14,6,1;bRev7:14,7,1;\\",128,0,0,0
.stabs "bRev8:14,8,1;bRev9:14,9,1;bRev10:14,10,1;bRev11:14,11,1;\\",128,0,0,0
.stabs "bRev12:14,12,1;bRev13:14,13,1;bRev14:14,14,1;bRev15:14,15,1;;",128,0,0,0
.stabs "32:T36=u2bits:35,0,16;value:13,0,16;;",128,0,0,0
.stabs "_ClusterListItem:T37=s18disk_id:13,0,16;start_cluster:11,16,32;\\",128,0,0,0
.stabs "offset_by_clus:11,48,32;cluster:11,80,32;seg_len:11,112,32;;",128,0,0,0
.stabs "31:T38=s8Ap3_num:13,0,16;Book_num:13,16,16;Page_num:13,32,16;\\",128,0,0,0
.stabs "Control_num:13,48,16;;",128,0,0,0
.stabs "30:T39=s4Cur_num:13,0,16;Max_num:13,16,16;;",128,0,0,0
.stabs "29:T40=s6Sd_num:13,0,16;Book_num:13,16,16;Book_id:13,32,16;;",128,0,0,0
.stabs "28:T41=u2w:13,0,16;b:42=ar1;0;1;10,0,16;;",128,0,0,0
.stabs "27:T43=u4dw:11,0,32;l:11,0,32;w:44=ar1;0;1;13,0,32;b:45=ar1;0;3;10,0,32;;",128,0,0,0
.stabs "_date_n_time:T46=s4year:14,0,7;month:14,7,4;day:14,11,5;\\",128,0,0,0
.stabs "hour:14,16,5;minute:14,21,6;second:14,27,5;;",128,0,0,0
.stabs "_fcb:T47=s86next:48=*47,0,16;used:1,16,16;spec:49=ar1;0;13;10,32,112;\\",128,0,0,0
.stabs "attribute:10,144,8;mode:10,152,8;date_n_time:46,160,32;sector_in_cluster:14,192,16;\\",128,0,0,0
.stabs "first_cluster:11,208,32;last_cluster:11,240,32;cluster:11,272,32;position:11,304,32;\\",128,0,0,0
.stabs "size:11,336,32;dir_cluster:11,368,32;dir_index:14,400,16;dir_first_cluster:11,416,32;\\",128,0,0,0
.stabs "fmode:14,448,16;possible_cluster:11,464,32;entry_index:14,496,16;lfn_cluster:11,512,32;\\",128,0,0,0
.stabs "old_final_cluster:11,544,32;new_start_cluster:11,576,32;pcluster_nodes:50=*14,608,32;\\",128,0,0,0
.stabs "node_count:14,640,16;skips_clusters:14,656,16;drive:1,672,16;;",128,0,0,0
.stabs "_fsinfo:T51=s134bytes_per_sector:14,0,16;sectors_per_cluster:14,16,16;\\",128,0,0,0
.stabs "num_of_fats:14,32,16;dir_entries:14,48,16;total_sectors:11,64,32;\\",128,0,0,0
.stabs "sectors_per_fat:11,96,32;media_id:10,128,8;dummy:10,136,8;first_fat_sector:11,144,32;\\",128,0,0,0
.stabs "first_dir_sector:11,176,32;first_dat_sector:11,208,32;max_cluster_num:11,240,32;\\",128,0,0,0
.stabs "last_cluster_mark:11,272,32;fs_semaphore:50,304,32;internal_slot:47,336,688;\\",128,0,0,0
.stabs "entry_ext:52=ar1;0;5;2,1024,48;;",128,0,0,0
.stabs "_drive:T53=s30drv_clear:54=*55=f1,0,32;drv_read:56=*57=f1,32,32;\\",128,0,0,0
.stabs "drv_write:58=*59=f1,64,32;drv_size:60=*61=f11,96,32;drv_flush:62=*63=f1,128,32;\\",128,0,0,0
.stabs "buffer:64=*10,160,16;sector_in_buffer:11,176,32;buffer_status:14,208,16;\\",128,0,0,0
.stabs "fsinfo:65=*51,224,16;;",128,0,0,0
.stabs "26:T67=s34Drv:53,0,240;pMountFunc:68=*69=f66=eFS_NO_ERROR:0,FS_DRIVE_ERROR:-1,\\",128,0,0,0
.stabs "FS_FORMAT_ERROR:-2,FS_SYSTEM_ERROR:-3,FS_INVALID_DIRECTORY:-4,FS_INVALID_SPEC:-5,\\",128,0,0,0
.stabs "FS_INVALID_MODE:-6,FS_FILE_NOT_FOUND:-7,FS_DIRECTORY_CONFLICT:-8,FS_DIRECTORY_FULL:-9,\\",128,0,0,0
.stabs "FS_OPENFILE_FULL:-10,FS_VOLUME_FULL:-11,FS_ACCESS_DENIED:-12,FS_SEEK_OUT_OF_RANGE:-13,\\",128,0,0,0
.stabs "FS_RENAMING_ERROR:-14,FS_UNRECOVERABLE_FILE:-15,FS_END_DIRECTORY:-16,FS_TRASH_ERROR:-17,\\",128,0,0,0
.stabs "FS_PARAMETER_ERROR:-18,FS_FILE_INCONSISTENT:-19,FS_WRITE_OUT_RANGE:-20,FS_TABLE_INCONSISTENT:-21,\\",128,0,0,0
.stabs "FS_DRIVE_NOT_READY:-22,FS_INVALID_CLUSTER:-23,;,240,32;;",128,0,0,0
.stabs "_fs_entry_info:T70=s2drive:1,0,16;;",128,0,0,0
.stabs "_fat_cache:T71=s12next:72=*71,0,32;drive:1,32,16;index:1,48,16;\\",128,0,0,0
.stabs "state:1,64,16;data:42,80,16;;",128,0,0,0
.stabs "_fc_trash_info:T73=s12first_cluster:11,0,32;cluster:11,32,32;index:1,64,16;\\",128,0,0,0
.stabs "secidx:1,80,16;;",128,0,0,0
.stabs "_file_info:T74=s280name:75=ar1;0;12;10,0,104;attribute:10,104,8;\\",128,0,0,0
.stabs "date_n_time:46,112,32;size:11,144,32;index:14,176,16;lfn:76=ar1;0;255;10,192,2048;;",128,0,0,0
.stabs "buffer_status:T77=eEMPTY:0,CLEAN:1,DIRTY:2,;",128,0,0,0
.stabs "24:T78=eSDKLOADCODE_NO_ERR:0,SDKLOADCODE_PARAM_ERR:-1,SDKLOADCODE_DATA_ERR:-5,;",128,0,0,0
.stabs "23:T79=eKBLBS_PARSE_CONTENT_OK:1,KBLBS_PARSE_CONTENT_LAST_FRAME:0,KBLBS_PARSE_CONTENT_BITSTREAM_NOT_ENOUGH:-1,\\",128,0,0,0
.stabs "KBLBS_PARSE_CONTENT_ERR_FORMAT:-2,;",128,0,0,0
.stabs "22:T80=s16Version:14,0,16;Mode:14,16,16;Filter:14,32,16;Length:14,48,16;\\",128,0,0,0
.stabs "Timber:14,64,16;Pitch:14,80,16;Speed:14,96,16;HeadLength:14,112,16;;",128,0,0,0
.stabs "21:T81=eKBLBS_PARSE_HEAD_LAST_HEAD:0,KBLBS_PARSE_HEAD_FURTHERMORE_HEAD:1,\\",128,0,0,0
.stabs "KBLBS_PARSE_HEAD_ERR_VERSION:-1,KBLBS_PARSE_HEAD_ERR_MODE:-2,;",128,0,0,0
.stabs "20:T82=s10IMA_SR:14,0,16;IMA_ADPCM_BufSize:14,16,16;IMA_PCM_BufSize:14,32,16;\\",128,0,0,0
.stabs "IMA_Frame_No:11,48,32;;",128,0,0,0
.stabs "19:T83=eSMP_8K:1,SMP_16K:2,SMP_32K:3,SMP_11P025K:4,SMP_22P05K:5,SMP_44P1K:6,\\",128,0,0,0
.stabs "SMP_12K:7,SMP_24K:8,SMP_48K:9,;",128,0,0,0
.stabs "18:T84=eHead_Phone_Out:0,Speaker_Out:1,;",128,0,0,0
.stabs "17:T85=eNo_Mixed_To_DAC:0,Mixed_To_DAC:1,;",128,0,0,0
.stabs "16:T86=eMIC_In:0,Line_In:1,;",128,0,0,0
.stabs "15:T87=eMIC_Gain_0dB:0,MIC_Gain_20dB:1,;",128,0,0,0
.stabs "14:T88=eAUADC_Gain_0dB:0,AUADC_Gain_1P5dB:1,AUADC_Gain_3dB:2,AUADC_Gain_4P5dB:3,\\",128,0,0,0
.stabs "AUADC_Gain_6dB:4,AUADC_Gain_7P5dB:5,AUADC_Gain_9dB:6,AUADC_Gain_10P5dB:7,\\",128,0,0,0
.stabs "AUADC_Gain_12dB:8,AUADC_Gain_13P5dB:9,AUADC_Gain_15dB:10,AUADC_Gain_16P5dB:11,\\",128,0,0,0
.stabs "AUADC_Gain_18dB:12,AUADC_Gain_19P5dB:13,AUADC_Gain_21dB:14,AUADC_Gain_22P5dB:15,;",128,0,0,0
.stabs "13:T89=eAUADC_SAMPLE_RATE_96K:0,AUADC_SAMPLE_RATE_48K:1,AUADC_SAMPLE_RATE_44P1K:2,\\",128,0,0,0
.stabs "AUADC_SAMPLE_RATE_32K:3,AUADC_SAMPLE_RATE_24K:4,AUADC_SAMPLE_RATE_22P05K:5,\\",128,0,0,0
.stabs "AUADC_SAMPLE_RATE_16K:6,AUADC_SAMPLE_RATE_12K:7,AUADC_SAMPLE_RATE_11P025K:8,\\",128,0,0,0
.stabs "AUADC_SAMPLE_RATE_9P6K:9,AUADC_SAMPLE_RATE_8K:10,;",128,0,0,0
.stabs "12:T90=eAUADC_NO_ERR:0,AUADC_SampleRate_SETUP_ERR:-1,AUADC_DTU_SETUP_ERR:-2,\\",128,0,0,0
.stabs "AUADC_OUT_BUF_SETUP_ERR:-3,AUADC_BUF_SIZE_SETUP_ERR:-4,AUADC_CALL_BACK_SETUP_ERR:-5,\\",128,0,0,0
.stabs "AUADC_PARAMETER_SETUP_ERR:-6,AUADC_NO_INIT_ERR:-7,;",128,0,0,0
.stabs "11:T91=eDAC_SAMPLE_RATE_96K:0,DAC_SAMPLE_RATE_48K:1,DAC_SAMPLE_RATE_44P1K:2,\\",128,0,0,0
.stabs "DAC_SAMPLE_RATE_32K:3,DAC_SAMPLE_RATE_24K:4,DAC_SAMPLE_RATE_22P05K:5,DAC_SAMPLE_RATE_16K:6,\\",128,0,0,0
.stabs "DAC_SAMPLE_RATE_12K:7,DAC_SAMPLE_RATE_11P025K:8,DAC_SAMPLE_RATE_9P6K:9,DAC_SAMPLE_RATE_8K:10,;",128,0,0,0
.stabs "10:T92=eDAC_NO_ERR:0,DAC_SAMPLE_RATE_ERR:-1,DAC_NO_SET_FRAME_SIZE:-2,DAC_DTU_SETUP_ERR:-3,\\",128,0,0,0
.stabs "DAC_FRAME_SIZE_ERR:-4,DAC_NO_INIT:-5,DAC_PLL_SETUP_ERR:-6,DAC_SOCR_BIT15_DISABLE:-7,;",128,0,0,0
.stabs "9:T93=eUSB_UPLOAD:16,USB_DOWNLOAD:32,;",128,0,0,0
.stabs "8:T94=eREMOVABLE:128,FIXED:0,;",128,0,0,0
.stabs "7:T95=eDIRECT_ACCESS:0,CDROM:1280,;",128,0,0,0
.stabs "__STORAGE:T96=s38stor_status:97=*98=f1,0,32;stor_size:99=*100=f11,32,32;\\",128,0,0,0
.stabs "stor_read:101=*102=f1,64,32;stor_write:103=*104=f1,96,32;stor_check_wp:105=*106=f1,128,32;\\",128,0,0,0
.stabs "stor_flush:107=*108=f1,160,32;device_type:14,192,16;device_vendor:109=*2,208,32;\\",128,0,0,0
.stabs "device_identify:109,240,32;device_revision:109,272,32;;",128,0,0,0
.stabs "6:T110=eSPI_30M:0,SPI_20M:1,SPI_15M:2,SPI_12M:3,SPI_10M:4,SPI_7P5M:5,SPI_6M:6,\\",128,0,0,0
.stabs "SPI_4M:7,SPI_3M:8,SPI_2M:9,SPI_1M:10,SPI_750K:11,SPI_600K:12,SPI_400K:13,SPI_200K:14,\\",128,0,0,0
.stabs "SPI_100K:15,;",128,0,0,0
.stabs "5:T111=eSPIF_No_Err:0,Do_Not_Support:-1,Parameter_Error:-2,SPIF_No_Space:-3,\\",128,0,0,0
.stabs "Over_RES_Size:-4,Over_FAT_Size:-5,;",128,0,0,0
.stabs "4:T112=eSDTL_No_Err:0,Data_CRC_Err:-1,RSP_CRC_Err:-2,CMD_RSP_Timeout:-3,PRG_Timeout:-4,\\",128,0,0,0
.stabs "SD_Not_Ready_for_Data:-5,Write_Data_Err:-6,Erase_Addr_Err:-7,Erase_Block_No_Err:-8,\\",128,0,0,0
.stabs "Write_To_Protect_ERR:-9,Unusable_Card:-10,No_Card_Inserted:-11,SD_Over_RES_Size:-12,\\",128,0,0,0
.stabs "SD_Over_FAT_Size:-13,Unknown_Err:-14,;",128,0,0,0
.stabs "3:T113=eFEQ_60M:3,FEQ_48M:4,FEQ_40M:5,FEQ_34P28M:6,FEQ_30M:7,FEQ_26P66M:8,\\",128,0,0,0
.stabs "FEQ_24M:9,FEQ_20M:11,FEQ_16M:14,FEQ_15M:15,FEQ_12M:19,FEQ_10M:23,FEQ_9P6M:24,FEQ_8M:29,\\",128,0,0,0
.stabs "FEQ_7P5M:31,;",128,0,0,0
.stabs "2:T114=eDTU0:0,DTU1:1,DTU2:2,DTU3:3,;",128,0,0,0
.stabs "1:T115=eSET_FLASH_BUSY_TRAP:64,CLEAR_FLASH_BUSY_TRAP:65,NFC_DELAY_CALLBACK_TRAP:66,\\",128,0,0,0
.stabs "SD_DELAY_CALLBACK_TRAP:67,STORAGE_SET_BUSY:68,STORAGE_CLR_BUSY:69,Set_File_System_Busy:71,\\",128,0,0,0
.stabs "Clr_File_System_Busy:72,Is_CWrite_Protect:73,FS_Callback:74,GET_DATE_NTIME:75,\\",128,0,0,0
.stabs "I_Change_Drive:76,STACK_OVERFLOW_TRAP:4,STACK_UNDERFLOW_TRAP:8,EXCEPTION_TRAP:12,\\",128,0,0,0
.stabs "TIMER0_INT_TRAP:16,TIMER1_INT_TRAP:18,TIMER2_INT_TRAP:20,TIMER3_INT_TRAP:22,\\",128,0,0,0
.stabs "TIMER4_INT_TRAP:24,I2C_INT_TRAP:30,ADC_INT_TRAP:32,PORT_B_INT_TRAP:34,PORT_C_INT_TRAP:36,\\",128,0,0,0
.stabs "RTC_INT_TRAP:38,CSI_INT_TRAP:40,SPI2_INT_TRAP:42,SPI1_INT_TRAP:44,P80_INT_TRAP:53,\\",128,0,0,0
.stabs "UART_INT_TRAP:54,UART_TX_INT_TRAP:55,UART_RX_INT_TRAP:56,I2SL_INT_TRAP:58,I2SR_INT_TRAP:59,;",128,0,0,0
.stabs "_bitref:t14",128,0,0,0
.stabs "GREV_BIT_NEW:t36",128,0,0,0
.stabs "ClusterListItem:t37",128,0,0,0
.stabs "Record:t38",128,0,0,0
.stabs "BOOKNEWS:t39",128,0,0,0
.stabs "LASTBOOK:t40",128,0,0,0
.stabs "DWord:t11",128,0,0,0
.stabs "Word:t14",128,0,0,0
.stabs "Int32:t6",128,0,0,0
.stabs "Int16:t1",128,0,0,0
.stabs "Int8:t2",128,0,0,0
.stabs "Uint32:t11",128,0,0,0
.stabs "Uint16:t14",128,0,0,0
.stabs "Byte:t10",128,0,0,0
.stabs "Uchar:t10",128,0,0,0
.stabs "Uint8:t10",128,0,0,0
.stabs "WORD:t14",128,0,0,0
.stabs "ULONG:t11",128,0,0,0
.stabs "USHORT:t14",128,0,0,0
.stabs "Dword:t11",128,0,0,0
.stabs "UCHAR:t10",128,0,0,0
.stabs "FP64:t3",128,0,0,0
.stabs "FP32:t4",128,0,0,0
.stabs "INT32S:t6",128,0,0,0
.stabs "INT32U:t11",128,0,0,0
.stabs "INT16S:t1",128,0,0,0
.stabs "INT16U:t14",128,0,0,0
.stabs "INT8S:t9",128,0,0,0
.stabs "INT8U:t10",128,0,0,0
.stabs "BOOLEAN:t10",128,0,0,0
.stabs "Union16:t41",128,0,0,0
.stabs "Union32:t43",128,0,0,0
.stabs "Status_bool:t10",128,0,0,0
.stabs "Status:t10",128,0,0,0
.stabs "Bool:t10",128,0,0,0
.stabs "S32:t6",128,0,0,0
.stabs "S16:t8",128,0,0,0
.stabs "S8:t9",128,0,0,0
.stabs "U32:t11",128,0,0,0
.stabs "U16:t13",128,0,0,0
.stabs "U8:t10",128,0,0,0
.stabs "Float16:t4",128,0,0,0
.stabs ":t116=*117=f15",128,4,0,0
.stabs "tStorageBusyCallBackFunc:t116",128,0,0,0
.stabs "tFSDrvHandle:t67",128,0,0,0
.stabs "FS_ENTRY_INFO:t70",128,0,0,0
.stabs "FAT_CACHE:t71",128,0,0,0
.stabs "FSINFO:t51",128,0,0,0
.stabs "FCFILE:t47",128,0,0,0
.stabs "FILE_INFO:t74",128,0,0,0
.stabs "FS_ERR:t66",128,0,0,0
.stabs "wchar_t:t13",128,0,0,0
.stabs "size_t:t11",128,0,0,0
.stabs "ptrdiff_t:t1",128,0,0,0
.stabs "tSDKLoadCodeStatus:t78",128,0,0,0
.stabs "tKBLBSParseContentStatus:t79",128,0,0,0
.stabs "tKBLBSHeadInfo:t80",128,0,0,0
.stabs "tKBLBSParseHeadStatus:t81",128,0,0,0
.stabs "tIMA_INFO:t82",128,0,0,0
.stabs "tMp3SampleRateID:t83",128,0,0,0
.stabs "tMixer_Out:t84",128,0,0,0
.stabs "tMixer_Option:t85",128,0,0,0
.stabs ":t118=*119=f15",128,4,0,0
.stabs "tAUADC_BufNotify:t118",128,0,0,0
.stabs "tAUADC_In_Path:t86",128,0,0,0
.stabs "tAUADC_MIC_Gain:t87",128,0,0,0
.stabs "tAUADC_Input_Gain:t88",128,0,0,0
.stabs "tAUADC_SmapleRate:t89",128,0,0,0
.stabs "tAUADC_Err:t90",128,0,0,0
.stabs "tDAC_SampleRate:t91",128,0,0,0
.stabs "tDAC_Error:t92",128,0,0,0
.stabs "_STORAGE:t96",128,0,0,0
.stabs ":t120=*121=f15",128,4,0,0
.stabs "tSPIFBusyCallBackFunc:t120",128,0,0,0
.stabs "tSPI_Clock:t110",128,0,0,0
.stabs "SPIF_ERR:t111",128,0,0,0
.stabs "SDTL_ERR:t112",128,0,0,0
.stabs ":t122=*123=f15",128,4,0,0
.stabs "PFWAKEUP:t122",128,0,0,0
.stabs "tFEQ:t113",128,0,0,0
.stabs "tKBDM9_DTUNumber:t114",128,0,0,0
.stabs "tKBDM9_TrapNo:t115",128,0,0,0

.area _$USB_MassStorage_c$(ROM,REL)
.stabs "", 100, 0, 0,Letext
Letext:

.area _USB_MS_XRAM$RD$usb_core_status(USB_MS_XRAM,REL,dat)
.even
_usb_core_status::
	.ds 2
.stabs "usb_core_status:G1",32,0,4,0

.area _USB_MS_XRAM$RD$b_usb_detection(USB_MS_XRAM,REL,dat)
_b_usb_detection::
	.ds 1
.stabs "b_usb_detection:G10",32,0,4,0

.area _XRAM$RD$usb_count(XRAM,REL,dat)
_usb_count::
	.ds 1
.stabs "usb_count:G10",32,0,0,0

.area _XRAM$RD$usb_nosd_timer(XRAM,REL,dat)
.even
_usb_nosd_timer::
	.ds 4
.stabs "usb_nosd_timer:G11",32,0,0,0

.area _USB_MS_XRAM$RD$adpate_flag(USB_MS_XRAM,REL,dat)
_adpate_flag::
	.ds 1
.stabs "adpate_flag:G10",32,0,4,0

.area _USB_MS_XRAM$RD$vid_data(USB_MS_XRAM,REL,dat)
.even
_vid_data::
	.ds 2
.stabs "vid_data:G14",32,0,4,0

.area _USB_MS_XRAM$RD$flag_chongdian(USB_MS_XRAM,REL,dat)
.even
_flag_chongdian::
	.ds 2
.stabs "flag_chongdian:G14",32,0,4,0

.area _USB_MS_XRAM$RD$usb_attach_time(USB_MS_XRAM,REL,dat)
.even
_usb_attach_time::
	.ds 4
.stabs "usb_attach_time:G11",32,0,4,0

.area _USB_MS_XRAM$RD$time_count(USB_MS_XRAM,REL,dat)
.even
_time_count::
	.ds 4
.stabs "time_count:G11",32,0,4,0

.area _USB_MS_XRAM$RD$g_exit_usb(USB_MS_XRAM,REL,dat)
.even
_g_exit_usb::
	.ds 2
.stabs "g_exit_usb:G14",32,0,4,0
; EXTERN _Get_ADCValue
; EXTERN _ADC_init
; EXTERN _MassStorage_Exit
; EXTERN _MassStorage_Loop
; EXTERN _MassStorage_Registers
; EXTERN _MassStorage_Init
; EXTERN _System_Timer3_Init
; EXTERN _timer_get_time
; EXTERN _timer_check_timeout
; EXTERN _rec_led_timer
; EXTERN _adc3_value
; EXTERN _gflag
; EXTERN _SendString
; EXTERN _UART_Init
; EXTERN _CommonLoadCode
; EXTERN _USBStoragePoweroff
; EXTERN _USBAttachCheck
; EXTERN _USBSetDebounceCount
; EXTERN _IP_INIT
; EXTERN _SystemClock

.area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL)
L98:
	.db 13
	.db 10
	;.asciz /usb adpate!/
	.db 117,115,98,32,97,100,112,97,116,101,33,0

.area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL)
L94:
	.db 13
	.db 10
	;.asciz /usb exit!/
	.db 117,115,98,32,101,120,105,116,33,0

.area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL)
L82:
	.db 13
	.db 10
	;.asciz /usb adpeat!/
	.db 117,115,98,32,97,100,112,101,97,116,33,0

.area _USB_MS_CODE$RD$USB_MassStorage_c$(USB_MS_CODE,REL)
L73:
	.db 13
	.db 10
	;.asciz /KB usb begin!/
	.db 75,66,32,117,115,98,32,98,101,103,105,110,33,0

; THE END
