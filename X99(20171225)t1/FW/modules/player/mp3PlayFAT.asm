;*******************************************************************
;* Copyright (c) 2009
;* King Billion Electronics Co., Ltd.
;* File Name	: mp3PlayFAT.asm
;* Author		: Nick
;* Create Date	: September 1, 2009
;* History		: Please Read the History.txt File.
;* Comment      :  
;*******************************************************************

.include "kbccmacro.asm"
.include "MP3_Information_def.inc"

.define     BS_BUF_SZ   722 ; size of circular buffer for bit stream (unit:word) = ceil(144*320kbps/32kHz+1)/2
.define     CATCH_BUF_SZ (256*4)

.area   _MP3_RAM2_(BITSTREAM_BUF_CIRC_XRAM) ; XRAM
_bitstream_buf_::       .CIRCBUF    BS_BUF_SZ

.area   _XBuffer_(CATCH_BUF_CIRC_XRAM)  ; YRAM
_catch_Buffer_::        .CIRCBUF    CATCH_BUF_SZ
_END_catch_Buffer_::

;**********************************************************************
; int Fill_MP3BitStream(void);
;**********************************************************************
.area _Fill_MP3BitStream_(MP3_COMMON_CODE)
;int Fill_MP3BitStream(void)
;{
_Fill_MP3BitStream::
;   INT16U bs_len;
    LINK #0
    PUSH R1 ; bs_len
    PUSH X0
    PUSH XM0
    PUSH X1
    PUSH XM1
    PUSH DS0
    PUSH DS1

    MOVX R0,_MP3_API_Flag_
    JB R0.2,$NOT_FILL_BS
    BSET R0.2 ;Set FILLBS_FLAG
    MOVX _MP3_API_Flag_,R0

;   if(free_len_ >= (CATCH_BUF_SZ/2))
;       Fill_MP3CatchBuffer();
;   if(free_len_ == (CATCH_BUF_SZ))
;       return -1;
    MOVX X0,_free_len_
    CMP X0,#(CATCH_BUF_SZ/2)
    AJMP IF_UL,$CONTINUE
    SCALL _Fill_MP3CatchBuffer
    MOVX X0,_free_len_
    CMP X0,#CATCH_BUF_SZ
    AJMP IF_NE,$CONTINUE  
	MOVX R0,_MP3_API_Flag_
	JB R0.3,$NFTL_BUSY
    MOV R0,#-1
    AJMP $EMPTY_END

$NFTL_BUSY:
	MOV R0,#-2
	AJMP $EMPTY_END
$CONTINUE:

;   bs_len = mp3QueryBuf();
;   __asm("BCLR SOCR.3\n");
    SCALL _mp3QueryBuf
    MOV R1,R0
    BCLR SOCR.3

;   if(CATCH_BUF_SZ-free_len_  < bs_len)
;       bs_len = CATCH_BUF_SZ-free_len_;
    MOV R0,#CATCH_BUF_SZ
    SUB R0,X0
    CMP R0,R1
    AJMP IF_UGE,$L2
    MOV R1,R0
$L2:

;   Audio_Fill_BS_Buf( (int *)pHead_, bs_len );
    MOV     XM0 , #BS_BUF_SZ*2
    MOV     XM1 , #CATCH_BUF_SZ*2
    
    MOVX    X0 ,  _mp3Buf
    
    ;LOAD4$memory R0,Y1,_pHead_
    MOVX    X1,_pHead_
    MOV     R0,R1
    SUB     R0 , #1
    AJMP    IF_N , $L3
    MOV     DS0,#0
    MOV     DS1,#1
    REPB    R0,$loop
    MOV     [X0++] , [X1++]
    NOP
$loop:
    MOVX    _mp3Buf , X0
    ;MOV     R0 , #0
    ;SAVE4$memory _pHead_,R0,Y1
    MOVX    _pHead_,X1
$L3:

;   free_len_+=bs_len;
    IGNDS #1
    ADD _free_len_,R1


;   return 0;
$END:
    MOV R0,#0
$EMPTY_END:
    MOVX R1,_MP3_API_Flag_
    BCLR R1.2
    MOVX _MP3_API_Flag_,R1
;}
$NOT_FILL_BS:
    POP DS1
    POP DS0
    POP XM1
    POP X1
    POP XM0
    POP X0
    POP R1
    UNLINK_MACRO
    RETS

;**********************************************************************
; int Calculate_Time(unsigned int MP3WordCount);
;**********************************************************************
.area _Calculate_Time_(MP3_COMMON_CODE,REL)
;void Calculate_Time(unsigned int MP3WordCount)
;{
_Calculate_Time::
    LINK #0
    PUSH R0
    PUSH R1
    PUSH R2
    PUSH R3

;       MP3_Information.dwFilePosition += (MP3WordCount<<1);        
    MOV R1,[FP+#(6)]
    SL R1,#1
    MOV R2,#0
    IGNDS #2
    ADD _MP3_Information+MP3_INFO_dwFilePosition+2,R1
    ADDC _MP3_Information+MP3_INFO_dwFilePosition,R2

;       MP3_Information.dwFrameCount = mp3FrameCount;
    MOVX R0,_mp3FrameCount
    MOVX R1,_mp3FrameCount+2
    ;IGNDS #2
    MOVX _MP3_Information+MP3_INFO_dwFrameCount,R0
    MOVX _MP3_Information+MP3_INFO_dwFrameCount+2,R1

;    MP3_Information.dwTimePosition = (INT32U)(mp3FrameCount*MP3_Information.wFrameDuration)/10;
    ;MOVX R0,_mp3FrameCount
    ;MOVX R1,_mp3FrameCount+2
    MOVX R3,_MP3_Information+MP3_INFO_wFrameDuration
    ;MOV R2,#0
    MUL$ulong R0,R1,R2,R3
    MOV R3,#10
    ;MOV R2,#0
    DIV$ulong R0,R1,R2,R3
    MOVX _MP3_Information+MP3_INFO_dwTimePosition,R0
    MOVX _MP3_Information+MP3_INFO_dwTimePosition+2,R1

;    //CurrentBitRate_ = mp3Bitrate;
    MOVX R0,_mp3Bitrate
    ;MOVX _CurrentBitRate_,R0

;    if(MP3_Information.bVBRType == CONSTANT_BITRATE && MP3_Information.wBitRate != mp3Bitrate)
;    {
    MOVX R1,_MP3_Information+MP3_INFO_wBitRate
    CMP R1,R0
    AJMP IF_EQ,$Exit
    MOVBX Rb0,_MP3_Information+MP3_INFO_bVBRType
    CMPB Rb0,#CONSTANT_BITRATE
    AJMP IF_NE,$Exit

;        MP3_Information.wBitRate = (unsigned int)((MP3_Information.dwFilePosition - MP3_Information.dwHeaderOffset)/MP3_Information.dwTimePosition);
    MOVX R0,_MP3_Information+MP3_INFO_dwFilePosition
    MOVX R1,_MP3_Information+MP3_INFO_dwFilePosition+2
    IGNDS #2
    SUB R1,_MP3_Information+MP3_INFO_dwHeaderOffset+2
    SUBC R0,_MP3_Information+MP3_INFO_dwHeaderOffset
    MOVX R2,_MP3_Information+MP3_INFO_dwTimePosition
    MOVX R3,_MP3_Information+MP3_INFO_dwTimePosition+2
    DIV$ulong R0,R1,R2,R3
    MOV R0,R1
    MOVX _MP3_Information+MP3_INFO_wBitRate,R0

;        MP3_Information.dwTimeLength = (MP3_Information.dwAudioFileSize)/MP3_Information.wBitRate;
    MOVX R0,_MP3_Information+MP3_INFO_dwAudioFileSize
    MOVX R1,_MP3_Information+MP3_INFO_dwAudioFileSize+2
    MOVX R3,_MP3_Information+MP3_INFO_wBitRate
    MOV R2,#0
    DIV$ulong R0,R1,R2,R3
    MOVX _MP3_Information+MP3_INFO_dwTimeLength,R0
    MOVX _MP3_Information+MP3_INFO_dwTimeLength+2,R1

;        MP3_Information.wBitRate <<= 3;
    MOVX R1,_MP3_Information+MP3_INFO_wBitRate
    SL R1,#3
    MOVX _MP3_Information+MP3_INFO_wBitRate,R1

;    }    
$Exit:
    POP R3
    POP R2
    POP R1
    POP R0
    UNLINK_MACRO
    RETS
;}

;**********************************************************************
; void MP3_API_Decode(void);
;**********************************************************************
.area _MP3_API_Decode_(MP3_COMMON_CODE)
_MP3_API_Decode::
    PUSH R0
    PUSH R1

$MP3DEC_LOOP:
    SCALL _Fill_MP3BitStream
    MOV R1,R0
    PUSH R1
    SCALL _mp3dec
    BCLR SOCR.3
    POP R1
    CMP R0,#0
    AJMP IF_NE,$MP3DEC_EXIT
	SCALL _DAC_Mute2
$11:	
;    CMP R1,#0
 ;   AJMP IF_EQ,$MP3DEC_LOOP
	CMP R1,#-2
	AJMP IF_EQ,$MP3DEC_EXIT
;    MOVX R0,_Remain_Bitstream_len_+2
;    CMP R0,#0
;    AJMP IF_NE,$MP3DEC_LOOP
;    MOVX R0,_Remain_Bitstream_len_
;    CMP R0,#0
;    AJMP IF_NE,$MP3DEC_LOOP
$MP3DEC_END_PLAY:
    SCALL _MP3API_EndPlay
$CLEAR_COUNT:
    MOV R0,#0
    AJMP $SAVE_COUNT
$MP3DEC_EXIT:
    CMP R1,#0
    AJMP IF_EQ,$CLEAR_COUNT
    MOVX R0,_BitStreamEmptyCount_
    CMP R0,#20
    AJMP IF_UG,$MP3DEC_END_PLAY
    ADD R0,#1
$SAVE_COUNT:
    MOVX _BitStreamEmptyCount_,R0
	
    POP R1
    POP R0
    RETS
;**********************************************************************
; void __interrupt AudioMP3_Left_ISR(void)
;**********************************************************************
;.area _AudioMP3_Left_ISR_(MP3_COMMON_CODE)
;_AudioMP3_Left_ISR::
;    SCALL _SysClkUpgrade
;    SJMP _PCMv2_ISR_L
;    RETI
