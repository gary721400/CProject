;
; filename		i2c_lib.asm
;
; for KBDM9 series
.define	i2c_id		pcid
.define	i2c_od		pcod
.define	i2c_clk		r0.1
.define	i2c_clk_en	pcen.1
.define	i2c_clk_dir	pcdir.1
.define	i2c_clk_od	pcod.1
.define	i2c_clk_ph	pcph.1
.define	i2c_sda		r0.2
.define	i2c_sda_en	pcen.2
.define	i2c_sda_dir	pcdir.2
.define	i2c_sda_od	pcod.2
.define	i2c_sda_ph	pcph.2
.define	i2c_addr		0x42
;
;
; _i2c_write_byte
;	_i2c_write_byte2
; _i2c_read_byte
;	_i2c_read_byte2
;	
;i2c_start		i2c_stop		i2c_clock_delay
;
;===============================
	.area	__i2c_code(CIS_CODE)
;===========================
_initial_i2c::
	link	#0
	push	r0
	
	bclr	i2c_sda_en
	bclr	i2c_clk_en
	bset	i2c_sda_od
	bset	i2c_clk_od
	bclr	i2c_sda_dir
	bclr	i2c_clk_dir
	bset	i2c_sda_ph
	bset	i2c_clk_ph
	
	pop	r0
	unlink
	rets
;===========================
_i2c_write_byte::
	link	#0
	push	r0

	scall	_i2c_start

	mov	r0,#i2c_addr
	push	r0		;control code
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=

	mov	r0,[fp+#6]
	push	r0		;addr
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=
	
	mov	r0,[fp+#8]
	push	r0		;data
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=
	mov	r0,#0
$100:
	scall	_i2c_stop

	unlink
	rets	
;---------------------------------------------
_i2c_write_byte2::
	link	#0
;	push	r0		;write data & return status
	push	r1		;bit count
	push	r2		;temp data
	push	sb
	
	mov	r2,[fp+#6]
	mov	r1,#8
$10:
;---------------------------------------------------
	sl	r2,#1		; bit write
	movx	r0,i2c_od		;
	bmov	i2c_sda,r2.8	;
	movx	i2c_od,r0		;
	scall	_i2c_clock_delay	;
	bset	i2c_clk_od	;
	scall	_i2c_clock_delay	;
	bclr	i2c_clk_od	;
;---------------------------------------------------

	sub	r1,#1
	ajmp	IF_NZ,$10
	
	bset	i2c_sda_dir
	scall	_i2c_clock_delay
	bset	i2c_clk_od
	movx	r0,i2c_id
	bmov	r2.0,i2c_sda
	scall	_i2c_clock_delay
	bclr	i2c_sda_od
	bclr	i2c_clk_od
	bclr	i2c_sda_dir
	
	mov	r0,#0
	jnb	r2.0,$100		;for check ack ok ?
	mov	r0,#0xffff
$100:
	scall	_i2c_clock_delay
	scall	_i2c_clock_delay
	
	pop	sb
	pop	r2
	pop	r1
;	pop	r0
	unlink	
	rets
;---------------------------------------		
;========================


;===========================
_i2c_read_byte::
	link	#0
;	push	r0

	scall	_i2c_start

	mov	r0,#i2c_addr
	push	r0		;control code
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=

	mov	r0,[fp+#6]
	push	r0		;addr
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=
	
	scall	_i2c_re_start

	mov	r0,#i2c_addr+1
	push	r0		;control code read
	scall	_i2c_write_byte2
	add	sp,#2
	cmp	r0,#0x8000		;write fail
	ajmp	IF_NC,$100		;>=

	scall	_i2c_read_byte2
$100:
	scall	_i2c_stop

	unlink
	rets	
;---------------------------------------------	
_i2c_read_byte2::
	link	#0
;	push	r0		;read data or status
	push	r1		;bit count
	push	r2		;temp data
	push	sb
	
	mov	r2,#0
	mov	r1,#8
	bset	i2c_sda_dir
$10:
;-------------------------------------------------------------
	scall	_i2c_clock_delay		;read bit
	bset	i2c_clk_od		;
					;
	movx	r0,i2c_id			;
	sl	r2,#1			;
	bmov	r2.0,i2c_sda		;
					;
	scall	_i2c_clock_delay		;
	bclr	i2c_clk_od		;
;	scall	_i2c_clock_delay		;
;---------------------------------------------------------------	
	sub	r1,#1
	ajmp	IF_NZ,$10

	bset	i2c_sda_od		;nack
	bclr	i2c_sda_dir		;end of byte read
	scall	_i2c_clock_delay		;
	bset	i2c_clk_od		;
	scall	_i2c_clock_delay		;
	bclr	i2c_clk_od		;
	bclr	i2c_sda_od
	
	mov	r0,r2
	
	pop	sb
	pop	r2
	pop	r1
;	pop	r0
	unlink	
	rets
;;===========================
		

;===========================
	
;==========================

_i2c_re_start::
	
_i2c_start::
	bset	i2c_sda_od
	scall	_i2c_clock_delay
	bset	i2c_clk_od
	scall	_i2c_clock_delay
	bclr	i2c_sda_od
	scall	_i2c_clock_delay
	bclr	i2c_clk_od
	scall	_i2c_clock_delay
	scall	_i2c_clock_delay
	scall	_i2c_clock_delay
	rets

_i2c_stop::
	link	#0
	bclr	i2c_sda_od
	bclr	i2c_clk_od
	scall	_i2c_clock_delay
	bset	i2c_clk_od
	scall	_i2c_clock_delay
	bset	i2c_sda_od
	scall	_i2c_clock_delay
	unlink
	rets

;-------------------------------------------
_i2c_clock_delay:
	link	#0
	push	r0
	mov	r0,#150
$10:	sub	r0,#1
	ajmp	IF_NZ,$10
	pop	r0
	unlink
	rets



; filename : 668_lib.asm
; KBDM9 series : CIS 668 library
; 
;
;	uint	i2c_read_byte(uint reg_addr);
;	uint	i2c_write_byte(uint reg_addr, uint reg_data);
;	uint	write_cis_reg(void);		//reg_buffer to CIS
;	uint	set_cis_view_area(x1,y1,x2,y2);
;	uint	cvt_cis_view_area(void);
;	uint	initial_cis_driver(void);
;	void	close_cis_driver(void);
;	void	clear_cis_line_buffer(void);
;	uint	initial_cis_reg(void);		//write CIS default register
;
; CIS Vsync	PB13
; CIS Pclk	PC9
; CIS Hsync	PC10
;
;
; 20091013	debug : _initial_cis_driver 時, set csrdba --> set cswdba 
;		debug : _start_cis_driver, clear _cis_flag
;		_cis_flag.1=1 時, 才可參考 _cis_y_count
;		debug : i2c R/W : mov pben,#0x0300 --> bset pben.8  pben.9
;		modify I2C time out delay 300 --> 2000 (for 80M cpuclk)
;20091015	modify stat_cis_driver check Vsync time out 100ms --> 4 sec
;
;===============================
.include	"668_lib.inc"
;
;=============================
;=============================
	.area	__t_cis_ram(CIS_DATA)
_cis_y_count::	.dsw	1		;line count, _cis_flag.1=1 時, 才可參考此register
_cis_x_byte::	.dsw	1		  ;line data count
_cis_y_byte::	.dsw	1
;_cis_x1::		.dsw	1
;_cis_y1::		.dsw	1
;_cis_x2::		.dsw	1
;_cis_y2::		.dsw	1
_cis_flag::	.dsw	1		;bit 0  new frame
					;bit 1  new line
					;bit 2  firat_interrupt
_cis_buffer_point::	.dsw	1		;new line data buffer point
_cis_line_buffer0::	.dsw	ini_cis_x_byte/2
_cis_line_buffer1::	.dsw	ini_cis_x_byte/2
_cis_frame_count:: .dsw	1

;----------------------------------------
;	.area	__t_cis_iram(CIS_IRAM_DATA)
;_cis_line_buffer1::	.dsw	7200	;(120*120/2)+1

;==============================
	.area	__t_i2c_code(CIS_CODE)
;_i2c_write_byte::
;	link	#0
;;	push	r0
;
;	bset	pben.8			;pb8 : i2c_sck
;	bset	pben.9			;pb9 : i2c_sda
;;	bset	i2cicr.0
;;	bset	socr.15
;
;	mov	i2csar,#0x0042
;;				;master mode, write eeprom
;$10:		
;;	mov	i2ccr,#0xbc00	;100k
;;	mov	i2ccr,#0xbc02	;200k
;;	mov	i2ccr,#0xbc04	;400k
;;	mov	i2ccr,#0xbc06	;800k
;	mov	i2ccr,#0x8000	;all interrupt disable
;;	mov	i2ccr,#0xa000	;tx complete interrupt only
;;	mov	i2ccr,#0x9000	;tx register empty interrupt inly
;;	mov	i2ccr,#0x8800	;rx register full interrupt only
;;	mov	i2ccr,#0x8400	;receive NAK interrupt only
;	
;	mov	r0,[fp+#6]	;reg_addr
;	movx	i2ctdr,r0
;	bset	i2ccr.0		;kick
;	nop
;	nop
;	nop
;	mov	r0,#2000
;$12:
;	sub	r0,#1
;	ajmp	IF_NZ,$14
;	mov	r0,#0xffff	
;	ajmp	$100		;time out
;$14:	
;	jnb	i2ccr.9,$12	;check tx register empty
;	bclr	i2ccr.9
;
;	mov	r0,[fp+#8]	;reg_data
;	movx	i2ctdr,r0
;	mov	r0,#2000
;$16:
;	sub	r0,#1
;	ajmp	IF_NZ,$18
;	mov	r0,#0xffff	
;	ajmp	$100		;time out
;$18:	
;	nop
;	jnb	i2ccr.9,$16	;check tx register empty
;	bclr	i2ccr.9
;	mov	r0,#0
;
;;	bset	i2ccr.5		;NACK.... 寫入 EEPROM 時, NACK 是沒有作用的
;;	movx	i2ctdr,r0
;;$16:
;;	nop
;;	jnb	i2ccr.9,$16	;check tx register empty
;;	bclr	i2ccr.9
;$100:
;	bset	i2ccr.4		;stop bit
;	scall	delay
;	mov	i2ccr,#0
;	
;	unlink
;	rets
;;---------------------------------------------
;; uint	i2c_read_byte(uint reg_addr);
;;
;_i2c_read_byte::			;read EEPROM
;	link	#0
;;	push	r0
;
;	bset	pben.8		;pb8 : i2c_sck
;	bset	pben.9		;pb9 : i2c_sda
;;	bset	i2cicr.0
;;	bset	socr.15
;
;	mov	i2csar,#0x0042
;
;;	mov	i2ccr,#0xbc00	;all interrupt enable, check portB.3 pin xor ?? times
;	mov	i2ccr,#0x8000	;all interrupt disable
;;	mov	i2ccr,#0xa000	;tx complete interrupt only
;;	mov	i2ccr,#0x9000	;tx register empty interrupt inly
;;	mov	i2ccr,#0x8800	;rx register full interrupt only
;;	mov	i2ccr,#0x8400	;receive NAK interrupt only
;	
;	mov	r0,[fp+#6]	;reg_addr
;	movx	i2ctdr,r0
;	bset	i2ccr.0		;kick
;	nop
;	mov	r0,#2000
;$22:
;	sub	r0,#1
;	ajmp	IF_NZ,$23
;	mov	r0,#0xffff
;	ajmp	$100		;time out
;$23:
;	nop
;	jnb	i2ccr.9,$22	;tdref
;	bclr	i2ccr.9
;	
;	bset	i2ccr.4		;stop bit
;	scall	delay
;	mov	i2csar,#0x0043
;	bset	i2ccr.5		;NACK, 必須放在 i2crdr data 讀出來之前先設定
;	bset	i2ccr.0		;kick
;	
;	nop
;	nop
;	nop
;;	bset	i2ccr.3		;re-start bit
;;	
;;	nop
;;	nop
;;	nop
;;	nop
;;	nop
;;	nop
;;	nop
;;	nop
;;	nop			;48M時, 至少須有 6個 nop,   20090813
;;	
;;	bclr	i2ccr.3
;	mov	r0,#2000
;$24:
;	sub	r0,#1
;	ajmp	IF_NZ,$25
;	mov	r0,#0xffff
;	ajmp	$100
;$25:
;	jnb	i2ccr.8,$24	;rdrff : check rx register full
;	bclr	i2ccr.8
;	nop
;	nop
;	nop
;;	bset	i2ccr.4		;stop bit
;	mov	i2ccr,#0x8010	;stop
;	nop
;	nop
;	nop
;;	bset	i2ccr.5		;NACK, 必須放在 i2crdr data 讀出來之前先設定
;	movx	r0,i2crdr
;	nop
;	nop
;;	mov	r0,#2000
;;$26:
;;	sub	r0,#1
;;	ajmp	IF_NZ,$27
;;	mov	r0,#0xffff
;;	ajmp	$100
;;$27:
;;	nop
;;	jnb	i2ccr.8,$26	;rdrff
;;	bclr	i2ccr.8
;;	bset	i2ccr.4		;stop bit
;;	movx	r0,i2crdr		;最後一筆資料的讀出,需在設定stop bit 之後
;$100:	
;	scall	delay
;	mov	i2ccr,#0
;	unlink
;	rets
;	
;delay:
;	link	#0
;	push	r0
;	mov	r0,#500
;$10:	sub	r0,#1
;	ajmp	IF_NZ,$10
;	pop	r0
;	unlink
;	rets	
		
;---------------------------------------------------------------------------
;-------------------------------------------------------------
;	uint	read_cis(uint);	//	 error_flag, frame_flag, line_frag, line_count, buffer point
;				//	    0x8000      0x0             0x1          0x2	03
;				//return	 -1, -2,  -3    0/1		0/1	0~287	addr
;	
_read_cis::
	link	#0
;	push	r0
	push	r1
	push	r2
;	push	r3
	
	mov	r2,#0
	movx	r1,_cis_flag
	mov	r0,[fp+#6]
	cmp	r0,#cis_frame_flag
	ajmp	IF_EQ,$10
	cmp	r0,#cis_line_flag
	ajmp	IF_EQ,$20
	cmp	r0,#cis_line_count
	ajmp	IF_EQ,$30
	cmp	r0,#cis_buffer_point
	ajmp	IF_EQ,$40
	cmp	r0,#cis_frame_count
	ajmp	IF_EQ,$4
	cmp	r0,#cis_error_flag
	ajmp	IF_EQ,$50
	mov	r0,#0
	ajmp	$100
$10:					;read new flame flag
	movx	r0,_cis_y_count
	cmp	r0,#119
	ajmp	IF_EQ,$12
;	cmp	r0,#118
;	ajmp	IF_EQ,$12
	cmp	r0,#0			;if cis_frame_count >= 118 then do not read flag
	ajmp	IF_NE,$100		;>=
$12:	bmov	r2.0,r1.0			
	bclr	r1.0
	jnb	r2.0,$100
	nop				;get flag=1
;	movx	r3,pcod
;	xor	r3,#0x0008		;pc3, test frame flag set
;	movx	pcod,r3
	ajmp	$100
	nop
$20:					;read new line flag
	bmov	r2.1,r1.1
	bclr	r1.1
	ajmp	$100
$30:					;read line count
	movx	r2,_cis_y_count
	ajmp	$100
$40:					;read cis buffer point
	movx	r2,_cis_buffer_point
	ajmp	$100
$4:
	movx	r2,_cis_frame_count
	ajmp	$100
$50:					;read error flag
	bmov	r2.0,r1.15		;if no error then return 0
	bmov	r2.1,r1.14		;-1  write i2c error
	bmov	r2.2,r1.13		;-2  read i2c error
	bmov	r2.3,r1.12		;-4  set cis view area error
	cmp	r2,#0			;-8  initial cis Vsync time out
	ajmp	IF_EQ,$100
	xor	r2,#0xffff
	add	r2,#1
	bclr	r1.12
	bclr	r1.13
	bclr	r1.14
	bclr	r1.15
	ajmp	$100
$100:	
	movx	_cis_flag,r1
	mov	r0,r2
	
;	pop	r3
	pop	r2
	pop	r1
;	pop	r0
	unlink
	rets	
;----------------------------------------------------------------------	
;	uint	write_cis_reg_byte(uint reg_addr, uint reg_data);	//write byte to reg_buffer & CIS
;		if ok return = 0 else -1
;
_write_cis_reg_byte::
	link	#0
;	push	r0
	push	r1
;	push	r2
;	push	r3
	
	mov	r0,[fp+#6]
	mov	r1,[fp+#8]
;	bclr	pcod.5			;SCCB_E = low
$10:	
	push	r1		;data
	push	r0		;addr
	scall	_i2c_write_byte
	add	sp,#4
	cmp	r0,#0xffff		;write fail
	ajmp	IF_EQ,$100
	mov	r0,#0
$100:
;	bset	pcod.5			;SCCB_E = high

;	pop	r3
;	pop	r2
	pop	r1
;	pop	r0
	unlink
	rets
;---------------------------------------------------------------------------
;	uint	read_cis_reg_byte(uint reg_addr);		// read CIS to reg_buffer & return
;		if ok then return data else -1
;
_read_cis_reg_byte::
	link	#0
;	push	r0
;	push	r1
;	push	r2
;	push	r3
	
	mov	r0,[fp+#6]
;	bclr	pcod.5			;SCCB_E = low
$10:	
	push	r0
	scall	_i2c_read_byte
	add	sp,#2
;	cmp	r0,#0xffff		;read fail
;	ajmp	IF_EQ,$100

$100:
;	bset	pcod.5			;SCCB_E = high

;	pop	r3
;	pop	r2
;	pop	r1
;	pop	r0
	unlink
	rets
;---------------------------------------------------------------------------
_i2c_int::
;	push	r0
;	bclr	i2ccr.0
	bset	i2ccr.4		;stop bit
$100:
;	pop	r0
	reti

;-------------------------------------------------------------------------	

_initial_cis_driver::
	;link	#0
	push r0
	push r1
	push r2
	push r3

	bclr  pbdir.0         ;pb0 output
	bset  pbod.0          ;pb0 high
	scall _Delay30ms
	bclr  pbod.0          ;pb0 low
	
	movx  r0,pben		
	or    r0,#0x00FF		  ;disable CMOS pin  
	movx  pben,r0	
		
;	bset  pbdir.13	
;	bclr  pbph.13	    
	bclr	cis_vsync_en
	bset	cis_vsync_dir
	bclr	cis_vsync_ph
	
	scall	_initial_i2c	;i2c_lib.asm
	
	movx   r0,pcdir
	or     r0,#0x0600
	movx   pcdir,r0
	
	movx  r0,pcph	
	and   r0,#0xf9ff
	movx  pcph,r0
	
	scall _Delay30ms
	
;;;; 	ajmp	$90	   ;skip check Vsync time out 
  
$ck:
 mov	r0,#0
 mov	r1,#10			    ;time out 10 = 30ms
$00:
;        movx    r3,pbid
;        jnb	r3.13,$01		;wait Vsync = low
 	movx	r3,cis_vsync_id
	jnb	cis_vsync,$01       
       
	nop
	add	r0,#1
	ajmp	IF_NZ,$00	  ;80M/4=0.005us x 65526= 3.27ms
	sub	r1,#1
	ajmp	IF_NZ,$00	
	ajmp	$90			   ;check Vsync time out 
	
$01:	
 mov	r0,#0
 mov	r1,#10			 ;time out 10 = 30ms
$02:	
;        movx    r3,pbid
;        jb	r3.13,$03	 ;wait Vsync = high
	movx	r3,cis_vsync_id
	jb	cis_vsync,$03

	nop
	add	r0,#1
	ajmp	IF_NZ,$02	  ;80M/4=0.005us x 65526= 3.27ms
	sub	r1,#1
	ajmp	IF_NZ,$02	
	ajmp	$90			   ;check Vsync time out 

$03:		
    mov	r0,#2000			    
$09:	
;        movx    r3,pbid
;        jnb     r3.13,$90 		;check Vsync = high 35us
	movx	r3,cis_vsync_id
	jnb	cis_vsync,$90

	nop
	sub	  r0,#1
	ajmp	IF_NZ,$09	  ;80M/4=0.005us x 3000= 15us  
	
$14:	
   
   bclr csicr.0
   mov	cscon,#0x00
  
  ;bset pbicr.0
  ;nop
  ;bset pbpie.13
  ;bclr pbpif.13      ;disable vsync interrupt
  
	 mov	r0,#0
	 ajmp	$100

$90:
	mov	r0,#0xffff
$100:	
	 pop r3
	 pop r2
         pop r1
         pop r0
	;unlink
	rets




_close_cis_driver::
	;link	#0

	mov	csicr,#0x0
	mov	cscon,#0x0
 	;unlink
	rets
;---------------------------------------------------------------------------
;  check Vsync faliing edge
;---------------------------------------------------------------------------
_vsync_int::
  ;link #0
  push r0
  push r1
  push r3
  
   mov	r0,#0
   mov	r1,#20			 
$01:	
;        movx    r3,pbid
;       jb	r3.13,$03	   ;debounce  Vsync = low
	movx	r3,cis_vsync_id
	jb	cis_vsync,$03
	
	nop
	add	r0,#1
	cmp r0,r1
	ajmp	IF_NZ,$01	  ;80M/4=0.005us x 30= 0.015us
		
$02:  
  mov	  r0,#0xffff
	movx	_cis_y_count,r0		;end of y count
	mov	  r0,#0
	movx  _cis_frame_count,r0
	movx	_cis_flag,r0
;	movx      _cis_frame,r0   ;owen Mask
        mov       r0,#ini_cis_x_byte
  movx  _cis_x_byte,r0
        mov      r0,#ini_cis_y_byte
  movx  _cis_y_byte,r0
 	mov	  r0,#_cis_line_buffer0
	movx	_cis_buffer_point,r0
	movx	cswdba,r0		;CSI write DMA uffer address
;	mov	 r0,#ini_cis_x_byte
	;movx	csnb,r0			;CSI DMA number of byte
	
;	bclr pbpif.13      ;disable vsync interrupt
;	bclr pbicr.0
;       bclr pbpie.13
	bclr	cis_vsync_pif
	bclr	cis_vsync_icr
	bclr	cis_vsync_pie    
	
	mov	csicr,#0x01    ;enable CMOS interrupt
;	bset csicr.0
;	bset csicr.4
	mov	cscon,#0x9400
	
	;mov r1,#0
$021: 	
	;scall _CIS_Read_Flag
	;cmp r0,r1
	;ajmp IF_NE, $021
	;mov r1,#0x11
	;movx _cis_frame,r1
	
$03:
        bclr	cis_vsync_pif
	pop r3
	pop r1
	pop r0
	;unlink
	reti
	
;---------------------------------------------------------------------------
;			CSI = 	CMOS Sensor Interface
;---------------------------------------------------------------------------

_cs_int::
;	link	#0
	push	r0
	push	r1
	push	r2
	push	r3
	
	movx	r0,_cis_y_count	  
	movx	r2,_cis_flag		  
        mov	r1,#0xffff
        cmp 	r0,r1
	ajmp	IF_NE,$1 

	bset	r2.0			;set new frame flag, line 0
	movx	r0,_cis_frame_count
	add  	r0,#1
	movx	_cis_frame_count,r0
	mov   r0,#0
	movx  _cis_y_count,r0
	ajmp  $2
	
$1:		
	add	  r0,#1			      ;
$2:	
	bset	r2.1			      ;set new line flag
	mov   r1,#1
    cmp   r0,r1
    ajmp  IF_NE,$4
    bclr	r2.0           ;clear frame flag
  
$4: 
	movx  _cis_flag,r2
    movx  _cis_y_count,r0		;
  
	jnb	r0.0,$10
	mov	r1,#_cis_line_buffer0
	mov	r3,#_cis_line_buffer1
	ajmp	$12
	
$10:	
    mov	r1,#_cis_line_buffer1
	mov	r3,#_cis_line_buffer0
$12:
	movx	cswdba,r1		;CSI write DMA uffer address
   	movx	_cis_buffer_point,r3
;	    mov	  r0,#ini_cis_x_byte
	    ;movx	csnb,r0			;CSI DMA number of byte
	mov	r1,#ini_cis_y_byte-1
;	    mov   r1,#119      ;disable CMOS interrupt
	movx  r0,_cis_y_count
    cmp   r0,r1
    ajmp  IF_NE ,$13
      ;mov	csicr,#0x00
     
      ;movx  r2,_cis_flag
      ;bset  r2.0
      ;movx  _cis_flag,r2
      
      mov	csicr,#0x00
      mov	cscon,#0x00
;      bset pbicr.0
;      bset pbpie.13
	bset	cis_vsync_icr
	bset	cis_vsync_pie
 $13:	    
	pop	r3
	pop	r2
	pop	r1
	pop	r0
;	unlink

	reti	
;-------------------------------------------------------------------------
; void	clear_line_buffer(void);
;
_clear_cis_line_buffer::
	link	#0
	push	r0
	push	r1
	push	r2

	mov	r0,#_cis_line_buffer0
	mov	r1,#ini_cis_x_byte/2
	mov	r2,#0
$10:
	mov	[r0++],r2	
	sub	r1,#1
	ajmp	IF_NZ,$10

;	mov	r0,#_cis_line_buffer1
;	mov	r1,#ini_cis_x_byte/2
;$12:
;	mov	[r0++],r2	
;	sub	r1,#1
;	ajmp	IF_NZ,$12

	pop	r2
	pop	r1
	pop	r0
	unlink
	rets

_Delay30ms::
   link #0
   push r0
   push r1
   mov	r1,#8			    ;time out 8 = 30 sec
   mov	r0,#0
$10:	
   nop
	 nop
	 add	r0,#1
	 ajmp	IF_NZ,$10	  ;80M/4=0.005us x 65526= 3.27ms
	 sub	r1,#1
	 ajmp	IF_NZ,$10	
	 pop r1
   pop r0
   unlink
   rets
   
   nop