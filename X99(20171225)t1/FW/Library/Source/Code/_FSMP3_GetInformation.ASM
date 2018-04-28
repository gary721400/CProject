.include "kbccmacro.asm"
.area _$FSMP3_GetInformation(MP3_TEMP_CODE00,REL)
;INT16S FSMP3_GetInformation(void)
;{
_FSMP3_GetInformation::
	LINK 	#12
	PUSH 	R1
	PUSH 	R2
	PUSH 	R3
	PUSH 	R4
	PUSH 	R5
	PUSH 	R6
	PUSH 	R7
	PUSH 	X0
	PUSH 	X1 
	PUSH 	X2
	PUSH 	Y0
	PUSH 	Y1 
	PUSH 	Y2	
;    FS_ERR fserr;
;    INT32U file_remain_size;
;    INT16U read_bytes;
;    INT32U dwMP3_FrameHeader_b;
;    INT32U dwHeaderOffset_b;
;    INT16S frame_offset;
;    INT16S CheckFrameCnt;
;    if (Audio_Handle.FSHandle == 0)
	MOVX 	R0,_Audio_Handle
	CMP 	R0,#0
	MOV 	R0, #-1
	AJMP 	IF_Z,L34
;        return (-1);
;    MP3_CalculateFlag_ = 0;

	MOV 	R2,#0
	MOVX	_MP3_CalculateFlag_, R2
;    MP3_Information.dwFileSize = Audio_Handle.FSHandle->size;
	MOVX 	R4,_Audio_Handle		;R4=Audio_Handle
	IGNDS 	#4
	MOV 	R0, [R4+#42]
	MOV 	R1, [R4+#42+2]
	MOV 	_MP3_Information, R0
	MOV 	_MP3_Information+2,R1
;    MP3API_FSSeek(MP3_Information.dwFileSize - ID3v1SIZE);
	MOV 	R3,#(128 & 0xFFFF)
	SUB 	R1, R3
	SUBC 	R0, R2
	PUSH 	R1
	PUSH 	R0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4
;    read_bytes = MP3API_FSRead((__X void*)&(Audio_Handle.ID3v1_Data),
;                          ID3v1SIZE,
;                          &fserr);
	MOV 	R0,#(_Audio_Handle+68)
	MOV 	R1, R3
	SCALL	FGI_MP3API_FSRead_1
;    if(read_bytes == 0 ||fserr != FS_NO_ERROR)
;        return (-2)	
	CMP 	R0, #0
	MOV 	R0, #-2
	AJMP 	IF_Z, L34
	MOV 	R1,[FP+#(-14)]
	CMP 	R1,#0
	AJMP 	IF_NE,L34
;    if ((Audio_Handle.ID3v1_Data.TAG_id[0] == 'T') &&
;        (Audio_Handle.ID3v1_Data.TAG_id[1] == 'A') &&
;        (Audio_Handle.ID3v1_Data.TAG_id[2] == 'G'))
	MOVBX 	Rb0,_Audio_Handle+68
	MOVBX 	Rb1,_Audio_Handle+68+1
	CMP 	R0,#0x5441
	AJMP 	IF_NE, L40
	MOVBX 	Rb0,_Audio_Handle+68+2
	CMPB 	Rb0, #71
	AJMP 	IF_NE, L40
;//    {
;        MP3_Information.wID3v1Size = ID3v1SIZE;
;//    }
	MOV 	R0,#128
	RJMP 	#0,L41
L40:
    ;else
;//    {
;        MP3_Information.wID3v1Size = 0;
;        Audio_Handle.ID3v1_Data.Song_title[0] = 0;
	MOV 	R0,#0
	MOVBX _Audio_Handle+68+3,Rb1
;//    }
L41:
	MOVX 	_MP3_Information+52,R0
	
;    MP3API_FSSeek(0);
	MOV 	R0,#0
	PUSH 	R0
	PUSH 	R0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4
;    read_bytes = MP3API_FSRead((void*)byte_buf,
;                                10,
;                                &fserr);
	MOV 	R1,#10
	SCALL	FGI_MP3API_FSRead
;    if(read_bytes == 0 ||
;       fserr != FS_NO_ERROR)
;        return (-2);
	CMP 	R0,#0
	MOV 	R0, #-2
	AJMP 	IF_NZ, L34
	MOV 	R1,[FP+#(-14)]
	CMP 	R1,#0
	AJMP 	IF_NZ, L34
L53:
;    if ((byte_buf[0] == (unsigned char)'I') &&
;        (byte_buf[1] == (unsigned char)'D') &&
;        (byte_buf[2] == (unsigned char)'3'))
	MOVBX 	Rb0,_catch_Buffer_
	MOVBX 	Rb1,_catch_Buffer_+1	
	CMP		R0, #0x4944
	AJMP 	IF_NE,L54
	MOVBX 	Rb0,_catch_Buffer_+2
	CMPB 	Rb0,#51
	AJMP 	IF_NE,L54
;//    {   // Tag ID3v2
;        MP3_Information.dwID3v2Size = ((unsigned long)(byte_buf[6] & 0x7F)<<21)
;                                    + ((unsigned long)(byte_buf[7] & 0x7F)<<14)
;                                    + ((unsigned long)(byte_buf[8] & 0x7F)<<7)
;                                    + (unsigned long)(byte_buf[9] & 0x7F) + 10;
;        MP3_Information.dwHeaderOffset = MP3_Information.dwID3v2Size - 10;
	MOV 	R0, #0
	MOV 	R3, #0
	MOV 	R1, #_catch_Buffer_+6
$6:	
	IGNDS 	#1
	MOVB 	Rb1, [R1++]
	ANDB 	Rb0, #0x7F
	OR 		R3, R1
	SL 		R3, #7
	SLOSB 	R2, #7
	CMP 	R1, #_catch_Buffer_+10
	RJMP 	IF_UL, $6
	MOVX 	R0,R2
	MOVX 	R1,R3	
	ADD 	R3,#10
	ADDC 	R2,#0
	RJMP #0,L55
;//    }
;    else
L54:
;//    {
;        MP3_Information.dwID3v2Size = 0;
;        MP3_Information.dwHeaderOffset = 0;
	MOV 	R3,#0
	MOV 	R2,#0
	MOV 	R0, #0
	MOV 	R1, #0

;//    }
L55:
	MOVX _MP3_Information+4,R0
	MOVX _MP3_Information+4+2,R1	
	MOVX _MP3_Information+54,R2
	MOVX _MP3_Information+54+2,R3
;    file_remain_size = 
;        MP3_Information.dwFileSize - MP3_Information.dwHeaderOffset;
	MOVX 	R2,_MP3_Information
	MOVX 	R3,_MP3_Information+2
	SUB 	R3,R1		;[R2R3] = file_remain_size
	SUBC 	R2,R0
;    dwHeaderOffset_b = MP3_Information.dwHeaderOffset;
	MOV 	X0, R0		;[X0X1] = dwHeaderOffset_b
	MOV 	X1, R1
;    MP3API_FSSeek(MP3_Information.dwHeaderOffset);
	PUSH 	R1
	PUSH 	R0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4

;    dwMP3_FrameHeader_b = 0;		
	MOV 	Y0, #0				;[Y0Y1] = dwMP3_FrameHeader_b
	MOV 	Y1, #0
;    CheckFrameCnt = 0;
	MOV 	X2, #0		;X0 = CheckFrameCnt

;    while (file_remain_size > 0)
	AJMP #0,L71
L70:	
;//    {
;        if (file_remain_size > 512)
	MOV 	R1, R3
	MOV 	R0, R1
	AND 	R0, #0xFE00
	OR 		R0, R2
	RJMP 	IF_Z, $L36
	MOV 	R1, #512
$L36:
;//        {
;            read_bytes = MP3API_FSRead(byte_buf, 512, &fserr);
;        }
;        else
;//        {
;            read_bytes = MP3API_FSRead(byte_buf, file_remain_size, &fserr);
;//        }
	SCALL	FGI_MP3API_FSRead
	ADD 	SP,#6
	MOV 	Y2,R0
                                          
;        if (read_bytes == 0)
;            return (-2);
	CMP 	R0,#0
	MOV 	R0, #-2
	AJMP 	IF_Z, L34

;        file_remain_size -= read_bytes;
	SUB 	R3, Y2
	SUBC 	R2, #0

;        dwMP3_FrameHeader_ = 0;
	MOV 	R0,#0
	MOVX 	_dwMP3_FrameHeader_,R0
	MOVX 	_dwMP3_FrameHeader_+2,R0
;        frame_offset = MP3_GetAudioHeader((void*)byte_buf,
;                                          read_bytes);
	PUSH 	R1
	MOV 	R1,#(_catch_Buffer_)
	PUSH 	R1
	PUSH 	R0
	SCALL 	_MP3_GetAudioHeader
	ADD 	SP,#6
;        if (frame_offset > 0)
	CMP 	R0,#0			;R0 = frame_offset
	RJMP 	IF_SLE,L77
;//        {
;          Find_Header:
L79:
;            dwMP3_FrameHeader_b = dwMP3_FrameHeader_ & CHECK_FRAME_DATA_MASK;
	MOVX 	Y0,_dwMP3_FrameHeader_
	MOVX 	Y1,_dwMP3_FrameHeader_+2
	AND 	Y1,#(0xfffe0c0f & 0xFFFF)
	AND 	Y0,#(0xfffe0c0f >> 16)		;[Y0Y1] = dwMP3_FrameHeader_b
;            dwHeaderOffset_b += frame_offset;
	ADD 	X1, R0
	ADDC 	X0, #0
;            MP3_Information.dwHeaderOffset = dwHeaderOffset_b;
	MOVX 	_MP3_Information+4,X0
	MOVX	_MP3_Information+4+2,X1
;            dwHeaderOffset_b += MP3_Information.wFrameSize;
	MOVX 	R1,_MP3_Information+40
	ADD 	X1, R1
	ADDC 	X0, #0
;            MP3API_FSSeek(dwHeaderOffset_b);
	PUSH 	X1
	PUSH 	X0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4
;            CheckFrameCnt = 0;
	MOV 	X2,#0
;            continue;
	RJMP 	#0,L71
;//        }  
L77:
;        else if (frame_offset == 0)
	RJMP IF_NZ,L84
;//        {
;           if(dwMP3_FrameHeader_b == (dwMP3_FrameHeader_ & CHECK_FRAME_DATA_MASK))             
	MOVX 	R0,_dwMP3_FrameHeader_
	MOVX 	R1,_dwMP3_FrameHeader_+2
	AND 	R1,#(0xfffe0c0f & 0xFFFF)
	AND 	R0,#(0xfffe0c0f >> 16)
	CMP$long R0,R1,X0,X1
	RJMP 	IF_NE,L84	
;//            {
;                CheckFrameCnt++;
	ADD 	X2,#1
;                dwHeaderOffset_b += MP3_Information.wFrameSize;
	MOVX 	R1, _MP3_Information+40
	ADD 	X1, R1
	ADDC 	X0, #0
	RJMP 	#0,L85	
;//            }
;            else
L84:
;//            {
;                if(CheckFrameCnt == 0)
	CMP 	X2,#0
;                    goto Find_Header;
	RJMP 	IF_EQ,L79
;                dwHeaderOffset_b = MP3_Information.dwFilePosition+4;
	MOVX 	X0,_MP3_Information+12
	MOVX 	X1,_MP3_Information+12+2
	ADD 	X1,#4
	ADDC 	X0,#0
;                dwMP3_FrameHeader_b = 0;
	MOV 	Y0, #0
	MOV 	Y1, #0
;                CheckFrameCnt = 0;
	MOV 	X2, #0
;//            }
L85:
;            MP3API_FSSeek(dwHeaderOffset_b);
	PUSH 	X1
	PUSH 	X0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4
;            file_remain_size = MP3_Information.dwFileSize - dwHeaderOffset_b;
	IGNDS 	#2
	SUB 	R3, _MP3_Information+2
	SUBC 	R2, _MP3_Information
;            if(CheckFrameCnt > 5)
	CMP 	X2,#5
	RJMP 	IF_SLE,L71
;                break;
	RJMP #0,L72
;            continue;
;//        }

;        dwHeaderOffset_b += read_bytes;
	ADD 	X1,Y2
	ADDC	X0,#0
;        if(dwHeaderOffset_b>0x8000)
	RJMP 	IF_NZ, L217
	CMP 	X1, #0x8000
	RJMP 	IF_ULE, L71
;//        {
L217:	
;            file_remain_size = 0;
	MOV 	R2, #0
	MOV 	R3, #0
;            break;
	RJMP 	#0,L72
;//        }
;//    }
L71:
	MOV 	R1, R2
	OR 		R1, R3
	AJMP 	IF_NZ,L70
L72:

;    if(file_remain_size == 0)
	MOV 	R1, R2
	OR 		R1, R3
	MOV 	R0, #-4
;        return (-4);
	AJMP 	IF_Z,L34        
;    // Get Frame Duration 
;    if(MP3_Information.wSampleRate/1000 > 0 && 
	MOVX 	R7,_MP3_Information+38
	MOV 	R0,#1000
	UDIV 	R0
	RJMP 	IF_Z,L96	
;       MP3_Information.wSamplePerFrame > 0)
	MOVX 	R0,_MP3_Information+42
	CMP 	R0,#0
	AJMP 	IF_EQ,L96	
;//    {
;        MP3_Information.wFrameDuration = 
;            (MP3_Information.wSamplePerFrame*10)/
;            (MP3_Information.wSampleRate/1000);
	MOVX 	R7,_MP3_Information+38
	MOV 	R1,#1000
	UDIV 	R1
	MOV 	R0,R7	
	MOV 	R0,#10
	MOVX 	R1,_MP3_Information+42
	UMUL 	R0,R1
	UDIV 	R1
	MOVX 	_MP3_Information+44,R7
	RJMP 	#0,L97	
;/    }
L96:
;    else
;        return (-4);
	MOV 	R0,#-4
	AJMP 	#0,L34

L97:
;    // Seek position to first frame, read 512 bytes data
;    MP3API_FSSeek(MP3_Information.dwHeaderOffset);
	MOVX 	R0,_MP3_Information+4
	MOVX 	R1,_MP3_Information+4+2
	PUSH 	R1
	PUSH 	R0
	SCALL 	_MP3API_FSSeek
	ADD 	SP,#4
;    file_remain_size = 
;        MP3_Information.dwFileSize - MP3_Information.dwHeaderOffset;
	MOVX 	R2,_MP3_Information
	MOVX 	R3,_MP3_Information+2
	IGNDS 	#2
	SUB 	R3,_MP3_Information+4+2
	SUBC 	R2,_MP3_Information+4
;    if (file_remain_size > 0)
	OR 		R2, R3
	RJMP 	IF_Z, L105
;//    {
;        read_bytes = MP3API_FSRead(byte_buf, 512, &fserr);
	MOV 	R1, #512	
	SCALL 	FGI_MP3API_FSRead
	RJMP 	#0,L106		
;//    }
;    else
;//    {
L105:
;        if (MP3_Information.bLayer == MPEG_LAYER_1)
	MOVBX 	Rb1,_MP3_Information+33
	CMPB 	Rb1,#1
;            return (-3);
	MOV 	R0,#-3	
	RJMP 	IF_EQ,L34
;        else
;            return (-4);
	SUB 	R0,#1
	RJMP 	#0,L34
;//    }

L106:
;    // Get VBR_Infomation
;    if (read_bytes > 62)
	MOV 	R4, R0			;R4=read_bytes
	CMP 	R4, #62
	RJMP 	IF_ULE,L34	
;//    {
;        dwMP3_FrameHeader_ = 0;
	MOV 	R2,#0
	MOVX 	_dwMP3_FrameHeader_,R2
	MOVX 	_dwMP3_FrameHeader_+2,R2
;        frame_offset = MP3_GetAudioHeader((void*)byte_buf,
;                                          read_bytes);
	PUSH 	R4
	MOV 	R3,#(_catch_Buffer_)
	PUSH 	R3
	PUSH 	R2
	SCALL 	_MP3_GetAudioHeader
	ADD 	SP,#6
;        if (frame_offset == -1)
	CMP 	R0,#-1
	MOV 	R0, #-5
;            return (-5);	
	RJMP 	IF_NE,L34
;        frame_offset = MP3_GetVbrHeader((void*)byte_buf,
;                                        read_bytes);
	PUSH 	R4
	PUSH 	R3
	PUSH 	R2
	SCALL 	_MP3_GetVbrHeader
	ADD 	SP,#6	
;//    }
L34:
;    return (0);
	MOV 	R0, #0
	POP 	Y2
	POP 	Y1 
	POP 	Y0
	POP 	X2
	POP 	X1
	POP 	X0
	POP 	R7
	POP 	R6
	POP 	R5
	POP 	R4
	POP 	R3
	POP 	R2
	POP 	R1
	UNLINK
	RETS
	
FGI_MP3API_FSRead:
	MOV 	R0,#(_catch_Buffer_)
FGI_MP3API_FSRead_1:
	MOV 	R5,FP
	ADD 	R5,#(-14)				;R5 = fserr
	PUSH 	R5
	PUSH 	R1
	PUSH 	R0
	SCALL 	_MP3API_FSRead
	ADD 	SP,#6
	RETS
	
	