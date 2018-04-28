;*******************************************************************
;* Copyright (c) 2009
;* King Billion Electronics Co., Ltd.
;* File Name	: mp3HeaderProcess.asm
;* Author		: Nick
;* Create Date	: September 1, 2009
;* History		: Please Read the History.txt File.
;* Comment      :  
;*******************************************************************

;*************************************************************************
; define
;*************************************************************************
.include "MP3_Information_def.inc"

.area _MP3_API_XRAM$RD$MP3_CalculateFlag_(MP3_API_XRAM,REL,dat)
.even
_MP3_CalculateFlag1_::
    .ds 2
;*************************************************************************
; 
;*************************************************************************
.area  _MP3_READFRAMEHEADER_1_CODE_(MP3_TEMP_CODE04)
MP3_BitRate_Table:;//		0x0,	0x1,	0x2,	0x3,	0x4,	0x5,	0x6,	0x7,	0x8,	0x9,	0xA,	0xB,	0xC,	0xD,	0xE,	0xF;
MPEG_V1_L1_BitRate_T:	.dw	#0,		#32,	#64, 	#96,	#128,	#160,	#192,	#224,	#256,	#288,	#320,	#352,	#384,	#416,	#448,	#0
MPEG_V1_L2_BitRate_T:	.dw	#0,		#32,	#48, 	#56,	#64,	#80,	#96,	#112,	#128,	#160,	#192,	#224,	#256,	#320,	#384,	#0
MPEG_V1_L3_BitRate_T:	.dw	#0,		#32,	#40, 	#48,	#56,	#64,	#80,	#96,	#112,	#128,	#160,	#192,	#224,	#256,	#320,	#0
MPEG_V2_L1_BitRate_T:	.dw	#0,		#32,	#48, 	#56,	#64,	#80,	#96,	#112,	#128,	#144,	#160,	#176,	#192,	#224,	#256,	#0
MPEG_V2_L23_BitRate_T:	.dw	#0,		#8,		#16, 	#24,	#32,	#40,	#48,	#56,	#64,	#80,	#96,	#112,	#128,	#144,	#160,	#0

MP3_SampleRate_Table:;//	0x0,	0x1,	0x2,	0x3;
MPEG_V2P5_SampleRate_T:	.dw	#11025,	#12000,	#8000,	#0;
MPEG_V1_SampleRate_T:	.dw	#44100,	#48000,	#32000,	#0;
MPEG_V2_SampleRate_T:	.dw	#22050,	#24000,	#16000,	#0;

MP3_SamplePerFrame_Table:;//	reserv,	L1,		L2,		L3,
MPEG_V2P5_SamplePerFrame_T:	.dw	#0,		#384,	#1152,	#576;
MPEG_V1_SamplePerFrame_T:	.dw	#0,		#384,	#1152,	#1152;
MPEG_V2_SamplePerFrame_T:	.dw	#0,		#384,	#1152,	#576;

;//-------------------------------------------------------------------
;// int MP3_ReadFrameHeader_1(void);
;//-------------------------------------------------------------------
_MP3_ReadFrameHeader_1::
		PUSH	DS0
		PUSH	DS1
		PUSH	R1
		PUSH	R2
		PUSH	R3
		PUSH    R4
		PUSH	ARH
		PUSH	ARL
		
		MOV		DS1, #@(_dwMP3_FrameHeader_);
		MOV		R0, DS1:_dwMP3_FrameHeader_;
		MOV		R1, DS1:_dwMP3_FrameHeader_+2;
		
		MOV		DS0, #@(MP3_BitRate_Table);
		MOV		DS1, #@(_MP3_Information);
		;// Get MP3_Information.bVersionID
		SL		R0, #5 ;// AAAA AAAA AAAB BCCD -> AAAA AABB CCD0 0000
		MOV		R2, #4;
		ANDB	Rb0, #0x3;
		SUBB	Rb5, Rb0;
		CMPB	Rb0, #0x1;
		AJMP	IF_EQ, $error_header;
		ANDB	Rb5, #0x3;
		MOVB	DS1:_MP3_Information + MP3_INFO_bVersionID, Rb5;
		MOV		ARH, R2;
		
		;// Get MP3_Information.bLayer
		SL		R0, #2 ;// 0000 00BB CCD0 0000 -> 0000 BBCC D000 0000
		MOV		R2, #4;
		ANDB	Rb0, #0x3;
		SUBB	Rb5, Rb0;
		CMPB	Rb0, #0x0;
		AJMP	IF_EQ, $error_header;
		ANDB	Rb5, #0x3;
		MOVB	DS1:_MP3_Information + MP3_INFO_bLayer, Rb5;
		MOV		ARL, R2;
		
		;// Get MP3_Information.bCRCs
		SL		R0, #1;// 0000 00CC D000 0000 -> 0000 0CCD 0000 0000
		BMOVN	R0.8, R0.7;
		ANDB	Rb0, #0x1;
		MOVB	DS1:_MP3_Information + MP3_INFO_bCRCs, Rb0;
		
		;// Get MP3_Information.wSamplePerFrame
		MOV		R0, ARH ;// ARH = MPEG Version ID
		SL		R0, #2;
		ADD		R0, ARL ;// ARL = MPEG Leayer ID
		SL		R0, #1;
		ADD		R0, #DS0:MP3_SamplePerFrame_Table;
		MOV		R2, [R0];
		CMP		R2, #0;
		AJMP	IF_EQ, $error_header;
		MOV		DS1:_MP3_Information + MP3_INFO_wSamplePerFrame, R2;
		
		;// Get MP3_Information.wBitRate
		MOV		R0, #0;
		JB		ARH.0, $1 ;// if (!MPEG Version 1) {increase 0x30}
		ADD		R0, #0x30;
$1:
		SUB		ARL, #1;
		SL		ARL, #4;
		ADD		R0, ARL;
		CMP		R0, #0x50;
		RJMP	IF_UL, $2;
		SUB		R0, #0x10;
$2:
		SL		R1, #4 ;// EEEE FFGH IIJJ KLMM -> FFGH IIJJ KLMM 0000
		SROSB	R2, #15 ;// R2 = bitrate index
		ADD		R0, R2;
		SL		R0, #1;
		ADD		R0, #DS0:MP3_BitRate_Table;
		MOV		R2, [R0];
		CMP		R2, #0;
		AJMP	IF_EQ, $error_header;
		CMP		R2, #320;
		AJMP	IF_UG, $error_header;
		MOV		DS1:_MP3_Information + MP3_INFO_wBitRate, R2;
		
		;// Get MP3_Information.wSampleRate
		MOV		R0, ARH ;// ARH = MPEG Version ID
		SL		R1, #2 ;// FFGH IIJJ KLMM 0000 -> GHII JJKL MM00 0000
		SLOSB	R0, #2;
		SL		R0, #1;
		ADD		R0, #DS0:MP3_SampleRate_Table;
		MOV		R2, [R0];
		CMP		R2, #0;
		AJMP	IF_EQ, $error_header;
		MOV		DS1:_MP3_Information + MP3_INFO_wSampleRate, R2;
		
		;// Get MP3_Information.bPaddingSize
		SL		R1, #1;// GHII JJKL MM00 0000 -> HIIJ JKLM M000 0000
		SLOSB	R0, #8;
		MOVB	DS1:_MP3_Information + MP3_INFO_bPaddingSize, Rb1;
		
		;// Get MP3_Information.bPrivate
		SL		R1, #1;// HIIJ JKLM M000 0000 -> IIJJ KLMM 0000 0000
		SLOSB	R0, #8;
		MOVB	DS1:_MP3_Information + MP3_INFO_bPrivate, Rb1;
		
		;// Get MP3_Information.bChannel
		SL		R1, #2;// IIJJ KLMM 0000 0000 -> JJKL MM00 0000 0000
		SLOSB	R0, #8;
		MOVB	DS1:_MP3_Information + MP3_INFO_bChannel, Rb1;
		CMPB	Rb1, #JOINT_STEREO_SOUND;// Joint Stereo (Stereo)
		MOVX    R4, _MP3_CalculateFlag1_
		BMOV	R4.13, PSR.0; bitMP3_JointStereoMode
		MOVX    _MP3_CalculateFlag1_, R4
		JB		R4.13, $joint_mode;
		;// Compare channel mode
		BMOV	R0.1, R1.15;
		BMOV	R0.0, R1.14;
		CMPB	Rb1, #0;
		RJMP	IF_NE, $error_header;
$joint_mode:
		
		;// Get MP3_Information.bCopyright
		BMOV	R1.0, R1.13;
		MOVB	DS1:_MP3_Information + MP3_INFO_bCopyright, Rb3;
		
		;// Get MP3_Information.bOriginal
		BMOV	R1.0, R1.12;
		MOVB	DS1:_MP3_Information + MP3_INFO_bOriginal, Rb3;
		
		;// Get MP3_Information.bEmphasis
		SR		R1, #2;// JJKL MM00 0000 0000 -> 00JJ KLMM 0000 0000
		ANDB	Rb2, #0x3;
		MOVB	DS1:_MP3_Information + MP3_INFO_bEmphasis, Rb2;
		
		;// Get MP3_Information.wFrameSize = ((wSamplePerFrame * wBitRate * 1000) / wSampleRate) /8 + Padding;
		MOV		R0, #125;
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_wBitRate;
		UMUL	R0, ARL;
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_wSamplePerFrame;
		UMUL	ARH, ARL;
		SL		ARL, #3;
		SLOSB	ARH, #3;
		MOV		R0, DS1:_MP3_Information + MP3_INFO_wSampleRate;
		MOV		R1, #8;
		UDIVL	R0;
		MOVZE	R2, DS1:_MP3_Information + MP3_INFO_bPaddingSize;
		UDIV	R1;
		ADD		ARL, R2;
		MOV		DS1:_MP3_Information + MP3_INFO_wFrameSize, ARL;
		
		;// compare Leayer I
		MOVB	Rb5, DS1:_MP3_Information + MP3_INFO_bLayer;
		CMPB	Rb5, #MPEG_LAYER_1;
		RJMP	IF_EQ, $error_header;
		MOV		R0, #0;
		RJMP    $exit
$error_header:
		MOV		R0, #MP3_FILE_HEADER_ERROR;
$exit:
		POP		ARL
		POP		ARH
        POP     R4
		POP		R3
		POP		R2
		POP		R1
		POP		DS1
		POP		DS0
		RETS
		
.area _MP3_GETAUDIOHEADER_1_CODE_(MP3_TEMP_CODE04)
;//-------------------------------------------------------------------
;// int MP3_GetAudioHeader_1(void *bitstream, int bs_len);
;//-------------------------------------------------------------------
_MP3_GetAudioHeader_1::
		LINK	#0
		PUSH	DS0
		PUSH	DS1
		PUSH	R1
		PUSH	R2
		PUSH	R3
		PUSH	R4
		PUSH	X0
		PUSH	ARH
		PUSH	ARL
		
		MOV		DS1, #@_dwMP3_FrameHeader_
		;// load bitstream pt
		MOV		R0, [FP+#(6)] ;// high word of bitstream
		MOV		R2, [FP+#(6+2)] ;// low word of bitstream
		SL		R0, #1
		BMOV	R0.0, R2.15
		BCLR	R2.15
		PUSH	R0
		POP		DS0
		
previous_3_byte:
		MOV		R0, DS1:(_dwMP3_FrameHeader_)
		CMP		R0, #0xFFE0
		RJMP	IF_NE, $previous_2_byte
		MOVB	Rb3, [R2]
		MOVB	DS1:(_dwMP3_FrameHeader_+3), Rb3
		SCALL	_MP3_ReadFrameHeader_1
		CMP		R0, #0
		RJMP	IF_NE, $previous_2_byte
		MOV		R0, #-3
		RJMP	$end_func
		
$previous_2_byte:
		MOV		R3, [FP+#(10)];// bs_len
		MOV		R4, #-2;// frame_offset;
		SUB		R3, #2;// last 3 byte retain to next search
		MOVB	Rb0, DS1:(_dwMP3_FrameHeader_+1);
		MOVB	Rb1, DS1:(_dwMP3_FrameHeader_+2);
		CMP		R0, #0xFFE0;
		RJMP	IF_UGE, $search_end;// find header
		MOVB	Rb0, Rb1;
		
$search_sync:
		MOVB	Rb1, [R2++];
		ADD		R4, #1;
		CMP		R0, #0xFFE0;
		RJMP	IF_UGE, $search_end;// find header
		MOVB	Rb0, Rb1;
		SUB		R3, #1;
		RJMP	IF_NZ, $search_sync;// find next byte
		
$search_end:
		CMP		R3, #0;// search all bitstream no header
		RJMP	IF_EQ, $search_fall;
		
		MOVB	Rb2, [R2++];
		MOVB	Rb3, [R2++];
		MOV		DS1:(_dwMP3_FrameHeader_), R0;
		MOV		DS1:(_dwMP3_FrameHeader_+2), R1;
		SCALL	_MP3_ReadFrameHeader_1;
		CMP		R0, #0;
		RJMP	IF_EQ, $get_header;
		SUB		R2, #3;
		MOVB	Rb0, [R2++];
		SUB		R3, #1;
		RJMP	$search_sync;
$get_header:
        MOVX    R0, _MP3_CalculateFlag1_
		BSET	R0.15; bitMP3_InformationFlag
		MOVX    _MP3_CalculateFlag1_, R0
		MOV		R0, R4;
		RJMP	$end_func;
$search_fall:
		SUB		R2, #1;
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb2, [R2++];
		MOV		DS1:(_dwMP3_FrameHeader_), R0;
		MOVB	DS1:(_dwMP3_FrameHeader_+2), Rb2;
		MOV		R0, #MP3_FILE_SEARCH_FALL;
$end_func:
		POP		ARL
		POP		ARH
		POP		X0
		POP		R4
		POP		R3
		POP		R2
		POP		R1
		POP		DS1
		POP		DS0
		UNLINK
		RETS

