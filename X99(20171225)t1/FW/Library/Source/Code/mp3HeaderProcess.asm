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
_MP3_CalculateFlag_::
    .ds 2
;*************************************************************************
; 
;*************************************************************************
.area  _MP3_READFRAMEHEADER_CODE_(MP3_TEMP_CODE00)
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
;// int MP3_ReadFrameHeader(void);
;//-------------------------------------------------------------------
_MP3_ReadFrameHeader::
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
		MOVX    R4, _MP3_CalculateFlag_
		BMOV	R4.13, PSR.0; bitMP3_JointStereoMode
		MOVX    _MP3_CalculateFlag_, R4
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
		
.area _MP3_GETAUDIOHEADER_CODE_(MP3_TEMP_CODE00)
;//-------------------------------------------------------------------
;// int MP3_GetAudioHeader(void *bitstream, int bs_len);
;//-------------------------------------------------------------------
_MP3_GetAudioHeader::
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
		SCALL	_MP3_ReadFrameHeader
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
		SCALL	_MP3_ReadFrameHeader;
		CMP		R0, #0;
		RJMP	IF_EQ, $get_header;
		SUB		R2, #3;
		MOVB	Rb0, [R2++];
		SUB		R3, #1;
		RJMP	$search_sync;
$get_header:
        MOVX    R0, _MP3_CalculateFlag_
		BSET	R0.15; bitMP3_InformationFlag
		MOVX    _MP3_CalculateFlag_, R0
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

.area _MP3_SEARCHAUDIOHEADER_CODE_(MP3_TEMP_CODE00)
;//-------------------------------------------------------------------
;// int MP3_SearchAudioHeader(void *bitstream, int bs_len, int mode);
;//-------------------------------------------------------------------
_MP3_SearchAudioHeader::
		LINK	#0
		PUSH	DS0
		PUSH	DS1
		PUSH	R1
		PUSH	R2
		PUSH	R3
		PUSH	R4
		PUSH	R5
		PUSH	ARH
		PUSH	ARL
		
		MOV		DS1, #@(_dwMP3_FrameHeader_)
		;// load bitstream pt
		MOV		R0, [FP+#(6)]   ;// high word of bitstream
		MOV		R2, [FP+#(6+2)] ;// low word of bitstream
		SL		R0, #1
		BMOV	R0.0, R2.15
		BCLR	R2.15
		PUSH    R0
		POP		DS0
		
		MOV		ARH, DS1:(_dwMP3_FrameHeader_)
		MOV		ARL, DS1:(_dwMP3_FrameHeader_+2)
		MOVX    R3, _MP3_CalculateFlag_
		MOV		R5, #0xFDFF ;// mask padding_bit
		BMOVN	R5.15, R3.11; bitMP3_Layer3Type
		BMOVN	R5.14, R3.11;
		BMOVN	R5.13, R3.11;
		BMOVN	R5.12, R3.11;
		BMOVN	R5.7, R3.10; bitMP3_Layer2Type
		BMOVN	R5.6, R3.10;
		BMOVN	R5.5, R3.10;
		BMOVN	R5.4, R3.10;
		BMOVN	R5.5, R3.13; bitMP3_JointStereoMode
		BMOVN	R5.4, R3.13;
		AND		ARL, R5;
		
		MOV		R3, [FP+#(10)];// bs_len
		MOV		R0, [FP+#(12)];// mode
		CMP		R0, #0;
		;AJMP	IF_EQ, MP3_SearchForward;
		AJMP	IF_NE, MP3_SearchBackward;

;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// MP3_SearchForward
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MP3_SearchForward:
		MOV		R0, DS1:(_dwMP3_CompareHeader_);
		CMP		R0, ARH;
		RJMP	IF_NE, $previous_2_byte;
		MOVB	Rb2, DS1:(_dwMP3_CompareHeader_+2);
		MOVB	Rb3, [R2];
		AND		R1, R5;
		CMP		R1, ARL;
		RJMP	IF_NE, $previous_2_byte;
		MOV		R0, #-3;// frame_offset;
		RJMP	$end_func;
		
$previous_2_byte:
		SUB		R3, #2;// last 3 byte retain to next search
		MOV		R4, #-2;// frame_offset;
		MOVB	Rb0, Rb1;
		MOVB	Rb1, DS1:(_dwMP3_CompareHeader_+2);
		CMP		R0, ARH;
		RJMP	IF_EQ, $compare_low_word;
		MOVB	Rb0, Rb1;
$search_sync:
		MOVB	Rb1, [R2++];
		ADD		R4, #1;
		CMP		R0, ARH;
		RJMP	IF_NE, $next_byte ;// find header
$compare_low_word:
		MOVB	Rb2, [R2++] ;
		MOVB	Rb3, [R2++] ;
		SUB		R2, #2
		AND		R1, R5
		CMP		R1, ARL;
		RJMP	IF_EQ, $search_end;
$next_byte:
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
		MOV		R0, R4;
		RJMP	$end_func;
$search_fall:
		SUB		R2, #1;
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb2, [R2++];
		MOV		DS1:(_dwMP3_CompareHeader_), R0;
		MOVB	DS1:(_dwMP3_CompareHeader_+2), Rb2;
		MOV		R0, #MP3_FILE_SEARCH_FALL;
$end_func:
        AJMP    MP3_SearchAudioHeader_
		;POP		ARL
		;POP		ARH
		;POP		R5
		;POP		R4
		;POP		R3
		;POP		R2
		;POP		R1
		;POP		DS1
		;POP		DS0
		;UNLINK
		;RETS

;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// MP3_SearchBackward		;[R0:R1]= 32bit REG,[R2]= bs_pt, 
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MP3_SearchBackward:
		MOV		R4, #1;
		SUB		R3, #1;// bs_len - 1
		ADD		R2, R3;// bs_pt seek to end position.
		MOVB	Rb1, DS1:(_dwMP3_CompareHeader_);
		MOVB	Rb0, [R2];// Rb0 = bs[N-1];
		CMP		R0, ARH;
		RJMP	IF_NE, $previous_2_byte;
		MOVB	Rb2, DS1:(_dwMP3_CompareHeader_+1);
		MOVB	Rb3, DS1:(_dwMP3_CompareHeader_+2);
		AND		R1, R5;
		CMP		R1, ARL;
		RJMP	IF_NE, $previous_2_byte;
		MOV		R0, #1;
		RJMP	$end_func;
		
$previous_2_byte:
		SUB		R2, #1;
		SUB		R3, #1;
		MOVB	Rb1, Rb0;
		MOVB	Rb0, [R2];// Rb0 = bs[N-2]
		CMP		R0, ARH;
		RJMP	IF_NE, $previous_1_byte;
		MOVB	Rb2, DS1:(_dwMP3_CompareHeader_);
		MOVB	Rb3, DS1:(_dwMP3_CompareHeader_+1);
		AND		R1, R5;
		CMP		R1, ARL;
		RJMP	IF_NE, $previous_1_byte;
		MOV		R0, #2;
		RJMP	$end_func;
$previous_1_byte:
		MOV		R4, #3;
		SUB		R2, #1;
		SUB		R3, #1;
		MOVB	Rb1, Rb0;
		MOVB	Rb0, [R2];// Rb0 = bs[N-3]
		CMP		R0, ARH;
		RJMP	IF_NE, $search_sync;
		ADD		R2, #2;
		MOVB	Rb2, [R2];// Rb2 = bs[N-1]
		MOVB	Rb3, DS1:(_dwMP3_CompareHeader_);
		SUB		R2, #2;// bs[N-3]
		AND		R1, R5;
		CMP		R1, ARL;
		RJMP	IF_NE, $search_sync;
		MOV		R0, #3;
		RJMP	$end_func;
$search_sync:
		ADD		R4, #1;
		SUB		R2, #1;
		SUB		R3, #1;// search all bitstream no header
		RJMP	IF_N, $search_fall;
		MOVB	Rb1, Rb0;// Rb1 = bs[(N-i)+1];
		MOVB	Rb0, [R2];// Rb0 = bs[N-i];
		CMP		R0, ARH;
		RJMP	IF_NE, $search_sync;// find sync header
$compare_low_word:
		ADD		R2, #2;
		MOVB	Rb2, [R2++];// Rb2 = bs[(N-i)+2];
		MOVB	Rb3, [R2++];// Rb3 = bs[(N-i)+3];
		SUB		R2, #4;
		AND		R1, R5;
		CMP		R1, ARL;
		RJMP	IF_EQ, $search_end;
		RJMP	$search_sync;// find next byte
$search_end:
		ADD		R2, #2;
		MOVB	Rb2, [R2++];
		MOVB	Rb3, [R2++];
		MOV		DS1:(_dwMP3_FrameHeader_), R0;
		MOV		DS1:(_dwMP3_FrameHeader_+2), R1;
		MOV		R0, R4;
		RJMP	$end_func;
$search_fall:
		ADD		R2, #1;
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb2, [R2++];
		MOV		DS1:(_dwMP3_CompareHeader_), R0;
		MOVB	DS1:(_dwMP3_CompareHeader_+2), Rb2;
		MOV		R0, #MP3_FILE_SEARCH_FALL;
$end_func:
MP3_SearchAudioHeader_:
		POP		ARL
		POP		ARH
		POP		R5
		POP		R4
		POP		R3
		POP		R2
		POP		R1
		POP		DS1
		POP		DS0
		UNLINK
		RETS

.area _MP3_GETVBRHEADER_CODE_(MP3_TEMP_CODE00)
;//-------------------------------------------------------------------
;// int MP3_GetVbrHeader(void *bitstream, int bs_len);
;//-------------------------------------------------------------------
_MP3_GetVbrHeader::
		LINK	#0
		PUSH	DS0
		PUSH	DS1
		PUSH	R1
		PUSH	R2
		PUSH	R3
		MOV		DS1, #@(_MP3_Information);
		PUSH	ARH
		PUSH	ARL

		MOVB	Rb0, DS1:_MP3_Information + MP3_INFO_bLayer;
		CMPB	Rb0, #MPEG_LAYER_2;
        MOVX    R2, _MP3_CalculateFlag_
		BMOV	R2.10, PSR.0 ; bitMP3_Layer2Type
		MOVX    _MP3_CalculateFlag_, R2
		CMPB	Rb0, #MPEG_LAYER_3;
		RJMP	IF_NE, $cbr_type;
		BSET	R2.11; bitMP3_Layer3Type // MPEG layer 3.
		MOVX    _MP3_CalculateFlag_, R2
		;// load bitstream pt
		MOV		R0, [FP+#(6)];// high word of bitstream
		MOV		R2, [FP+#(6+2)];// low word of bitstream
		SL		R0, #1;
		BMOV	R0.0, R2.15;
		BCLR	R2.15;
		PUSH	R0
		POP		DS0;
		
		MOVB	Rb0, DS1:_MP3_Information + MP3_INFO_bVersionID;
		MOVB	Rb1, DS1:_MP3_Information + MP3_INFO_bChannel;
		MOV		R3, #0;
		CMPB	Rb0, #MPEG_VERSION_1;
		RJMP	IF_EQ, $mpeg1;
		CMPB	Rb1, #SINGLE_CHANNEL_SOUND;
		RJMP	IF_EQ, $xing_offset_9;
		RJMP	$xing_offset_17;
$mpeg1:
		CMPB	Rb1, #SINGLE_CHANNEL_SOUND;
		RJMP	IF_EQ, $xing_offset_17;
$xing_offset_32:
		ADD		R3, #(32-17);
$xing_offset_17:	
		ADD		R3, #(17-9);
$xing_offset_9:
		ADD		R3, #(9+4);
		
		ADD		R2, #36;
		MOVB	Rb0, [R2++];// High word of TAG
		MOVB	Rb1, [R2++];// Low word of TAG
		MOVB	Rb2, [R2++];// High word of TAG
		MOVB	Rb3, [R2++];// Low word of TAG
		CMP		R0, #0x5642;// "VB"
		RJMP	IF_NE, $cmp_xing_vbr;
		CMP		R1, #0x5249;// "RI"
		RJMP	IF_NE, $cmp_xing_vbr;
		ACALL	MP3_VBRI_Type;
		RJMP	$calculate_time;
$cmp_xing_vbr:
		SUB		R2, #36+4;
		ADD		R2, R3;
		MOVB	Rb0, [R2++];// High word of TAG
		MOVB	Rb1, [R2++];// Low word of TAG
		MOVB	Rb2, [R2++];// High word of TAG
		MOVB	Rb3, [R2++];// Low word of TAG
		
		CMP		R0, #0x5869;// "Xi"
		RJMP	IF_NE, $cbr_type;
		CMP		R1, #0x6E67;// "ng"
		RJMP	IF_EQ, $call_xing_vbr;
		RJMP $cbr_type		
;$cmp_info_vbr:                        // 2008/9/23 modified by YJ. Info is CBR, not VBR
;        CMP     R0, #0x496E;// "In"
;		RJMP	IF_NE, $cbr_type;
;        CMP     R1, #0x666F;// "fo"
;		RJMP	IF_NE, $cbr_type;
$call_xing_vbr:        
		ACALL	MP3_Xing_Type;
		CMP     R0, #MP3_FILE_HEADER_ERROR;
		RJMP    IF_EQ, $cbr_type;
		RJMP	$calculate_time;
$cbr_type:
		ACALL	MP3_CBR_Type;
$calculate_time:
		;// Get MP3_Information.dwTimeLength = FileSize * 8 / BitRate
		MOV		DS1, #@(_MP3_Information);
.if KERNEL1_E
		NOP
.endif
		MOVB   Rb0, DS1:_MP3_Information + MP3_INFO_bVBRType;
		CMPB	 Rb0, #VBRI_VARIABLE_BITRATE;
		RJMP  IF_NE,$other_calculate_time;
		MOV		R1, DS1:_MP3_Information + MP3_INFO_dwFrameTotal;
		MOV		R0, DS1:_MP3_Information + MP3_INFO_dwFrameTotal+2;
		MOV		R3, DS1:_MP3_Information + MP3_INFO_wFrameDuration;
		UMUL  R1, R3;
		MOV		R1, ARL;
		UMUL  R0, R3;
		ADD   ARH,R1;
		MOV   R1,ARL;
		MOV		ARL,ARH;
		MOV		R0,#10;
    DIV		R0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwTimeLength,ARL;
    MOV		ARL,R1;
    DIVL  R0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwTimeLength+2,ARL;
		RJMP  $calculate_time_end; 
$other_calculate_time:
		MOV		R3, DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2;
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwAudioFileSize;
		;SL		R3, #3;
		;SLOSB	ARL, #3;
		MOV		R1, DS1:_MP3_Information + MP3_INFO_wBitRate;
		UDIV	R1;
		MOV		R2, ARL;
		MOV		ARL, R3;
		UDIVL	R1;
		SL		ARL, #3;
		SLOSB	R2, #3;
		MOV		DS1:_MP3_Information + MP3_INFO_dwTimeLength+2, ARL;
		MOV		DS1:_MP3_Information + MP3_INFO_dwTimeLength, R2;
$calculate_time_end:
		POP		ARL
		POP		ARH
		POP		R3
		POP		R2
		POP		R1
		POP		DS1
		POP		DS0
		UNLINK
		RETS
		
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// void MP3_VBRI_Type(void);
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MP3_VBRI_Type:
		MOV		DS1, #@(_MP3_Information);
.if KERNEL1_E
		NOP
.endif
		;// Get VBR_Information.dwTOC_Offset
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset+2;
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset;
		MOV		DS1, #@(_VBR_Information);
		ADD		ARL, #62;
		ADDC	ARH, #0;
		MOV		DS1:_VBR_Information + VBR_INFO_dwTOC_Offset+2, ARL;
		MOV		DS1:_VBR_Information + VBR_INFO_dwTOC_Offset, ARH;		
		;// Get VBR_Information.wVbriDelay
		ADD		R2, #2;// no read wVbriVersion;
		MOV		R0, [R2++]; // read wVbriDelay;
		MOV   DS1:_VBR_Information + VBR_INFO_wVbriDelay, R0;
		ADD   R2, #2;// no read wVbriQuality;

		;// Get MP3_Informatio.dwMP3_AudioDataSize
		MOV		DS1, #@(_MP3_Information);
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb2, [R2++];
		MOVB	Rb3, [R2++];
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize, R0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2, R1;
		
		;// Get MP3_Information.MP3_INFO_dwFrameTotal
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb2, [R2++];
		MOVB	Rb3, [R2++];
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal, R0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal+2, R1;

		;// Get VBR_Information.wVbriTOC_Size
		MOV		DS1, #@(_VBR_Information);
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOV		DS1:_VBR_Information + VBR_INFO_wVbriTOC_Size, R0;
		
		;// Get VBR_Information.wVbriTOC_Scale
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOV		DS1:_VBR_Information + VBR_INFO_wVbriTOC_Scale, R0;
		
		;// Get VBR_Information.wVbriTOC_BytePerEntry
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOV		DS1:_VBR_Information + VBR_INFO_wVbriTOC_EntryBytes, R0;
		
		;// Get VBR_Information.wVbriTOC_FramePerEntry
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOV		DS1:_VBR_Information + VBR_INFO_wVbriTOC_EntryFrames, R0;
		
		ACALL	Get_VBR_Bitrate;
		MOVX    R0, _MP3_CalculateFlag_
		BSET	R0.14; bitVBR_InformationFlag
		MOVX    _MP3_CalculateFlag_, R0
		MOV		DS1, #@(_MP3_Information);
		MOVB	Rb0, #VBRI_VARIABLE_BITRATE;
		MOVB	DS1:_MP3_Information + MP3_INFO_bVBRType, Rb0;
		MOV		R0, #0;
		RET

;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// void MP3_Xing_Type(void);
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MP3_Xing_Type:

		MOV		DS1, #@(_MP3_Information);
		;// Get Flag
		MOVB	Rb0, [R2++];
		MOVB	Rb1, [R2++];
		MOVB	Rb8, [R2++];
		MOVB	Rb9, [R2++];
		CMP		R0, #0x0000;
		RJMP	IF_UG, $error_header;
		CMP		R4, #0x000F;
		RJMP	IF_UG, $error_header;
		ADD		R3, #0x8;
		
		;// Get MP3_Information.dwFrameTotal
		JNB		R4.0, $no_frame_valid;// Frames field is valid
		MOVB	Rb12, [R2++];
		MOVB	Rb13, [R2++];
		MOVB	Rb14, [R2++];
		MOVB	Rb15, [R2++];
		ADD		R3, #0x4;
		CMP     R6, #0x0000;
		RJMP	IF_NE,$store_frame_void;
		CMP     R7, #0x0000;
		RJMP	IF_NE,$store_frame_void;
		RJMP    $error_header;
$no_frame_valid:
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwFileSize+2;
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_dwFileSize;
		SUB		ARL, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset+2;
		SUBC	ARH, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset;
		SUB		ARL, DS1:_MP3_Information + MP3_INFO_wID3v1Size;
		SUBC	ARH, #0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2, ARL;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize, ARH;
		MOV		R1, ARL;
		MOV		R0, DS1:_MP3_Information + MP3_INFO_wFrameSize;
		MOV		ARL, ARH;
		UDIV	R0;
		PUSH	ARL;
		MOV		ARL, R1;
		UDIVL	R0;
		POP		ARH;
$store_frame_void:
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal, ARH;
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal+2, ARL;
		
		;// Get MP3_Information.dwAudioFileSize
		JNB		R4.1, $no_bytes_valid;// Bytes field is valid
		MOVB	Rb12, [R2++];
		MOVB	Rb13, [R2++];
		MOVB	Rb14, [R2++];
		MOVB	Rb15, [R2++];
		ADD		R3, #0x4;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize, ARH;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2, ARL;
		ACALL	Get_VBR_Bitrate;
$no_bytes_valid:
		
		;// Get VBR_Information.dwTOC_Offset
		JNB		R4.2, $no_toc_valid;// TOC field is valid
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset+2;
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset;
		MOV		DS1, #@(_VBR_Information);
		ADD		ARL, R3;
		ADDC	ARH, #0;
		MOV		DS1:_VBR_Information + VBR_INFO_dwTOC_Offset+2, ARL;
		MOV		DS1:_VBR_Information + VBR_INFO_dwTOC_Offset, ARH;
		MOVX    R0, _MP3_CalculateFlag_
		BSET	R0.14; bitVBR_InformationFlag
		MOVX    _MP3_CalculateFlag_, R0
		MOV		DS1, #@(_MP3_Information);
		MOVB	Rb0, #XING_VARIABLE_BITRATE;
		MOVB	DS1:_MP3_Information + MP3_INFO_bVBRType, Rb0;
		RJMP	$store_toc_void;
$no_toc_valid:
		MOVX    R0, _MP3_CalculateFlag_
		BCLR	R0.14; bitVBR_InformationFlag
		MOVX    _MP3_CalculateFlag_, R0
		MOVB	Rb0, #NO_TOC_VARIABLE_BITRATE;
		MOVB	DS1:_MP3_Information + MP3_INFO_bVBRType, Rb0;
$store_toc_void:
		MOV		R0, #0;
		RET
$error_header:
		MOV		R0, #MP3_FILE_HEADER_ERROR;
		RET

;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// void  Get_VBR_Bitrate(void);
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Get_VBR_Bitrate:
		;// Get VBR_Bitrate = (((FileSize * 8) / TotalFrame) * SampleRate) / SamplePerFrame
		MOV		DS1, #@(_MP3_Information);
.if KERNEL1_E
		NOP
.endif
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_dwAudioFileSize
		MOV		R0, DS1:_MP3_Information + MP3_INFO_dwFrameTotal+2
		UDIVL	R0
		SR		R0, #1
		SUB		R0, ARH
		ADDC	ARL, #0
		
		MOV		R1, DS1:_MP3_Information + MP3_INFO_wSampleRate;
		MOV		R2, DS1:_MP3_Information + MP3_INFO_wSamplePerFrame;
		UMUL	R1, ARL;
		;SR      R2,#3
		UDIVL	R2;
		SR		R2, #1;
		SUB		R2, ARH;
		ADDC	ARL, #0;
		PUSH	R3
		MOV		R3, #125;
		UDIV	R3;
		POP   R3
		MOV		DS1:_MP3_Information + MP3_INFO_wBitRate, ARL;
		MOV		R0, #0;
		RET

;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;// void MP3_CBR_Type(void);
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MP3_CBR_Type:
		MOV		DS1, #@(_MP3_Information);
.if KERNEL1_E
		NOP
.endif
		;// Get MP3_Information.dwAudioFileSize
		MOV		ARL, DS1:_MP3_Information + MP3_INFO_dwFileSize+2;
		MOV		ARH, DS1:_MP3_Information + MP3_INFO_dwFileSize;
		SUB		ARL, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset+2;
		SUBC	ARH, DS1:_MP3_Information + MP3_INFO_dwHeaderOffset;
		SUB		ARL, DS1:_MP3_Information + MP3_INFO_wID3v1Size;
		SUBC	ARH, #0;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize+2, ARL;
		MOV		DS1:_MP3_Information + MP3_INFO_dwAudioFileSize, ARH;
		
		;// Get MP3_Information.dwFrameTotal = dwAudioFileSize / wFrameSize;
		MOV		DS1, #@(_MP3_Information);
.if KERNEL1_E
		NOP
.endif
		MOV		R0, DS1:_MP3_Information + MP3_INFO_wFrameSize;
		MOV		R1, ARL;
		MOV		ARL, ARH;
		UDIV	R0;
		PUSH	ARL;
		MOV		ARL, R1;
		UDIVL	R0;
		POP		ARH;
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal, ARH;
		MOV		DS1:_MP3_Information + MP3_INFO_dwFrameTotal+2, ARL;
		
		MOVX    R0, _MP3_CalculateFlag_
		BCLR	R0.14; bitVBR_InformationFlag
		MOVX    _MP3_CalculateFlag_, R0
		MOV		R0, #CONSTANT_BITRATE;
		MOVB	DS1:_MP3_Information + MP3_INFO_bVBRType, Rb0;
		RET