;*******************************************************************
;* Copyright (c) 2007
;* King Billion Electronics Co., Ltd.
;* File Name	: MP3_Information_def.inc
;* Author		: Nick
;* Create Date	: October 22, 2007
;* History		: Please Read the History.txt File.
;* Comment      :  
;*******************************************************************

;*************************************************************************
;// MP3_INFORMATION MP3_Information;// size 128 byte
;*************************************************************************
;MP3_INFO_sFileName			=	0;
MP3_INFO_dwFileSize			=	0 ;//MP3_INFO_sFileName      +72 ;// unit is byte
MP3_INFO_dwHeaderOffset		=	MP3_INFO_dwFileSize		+4	;// unit is byte
MP3_INFO_dwAudioFileSize	=	MP3_INFO_dwHeaderOffset	+4	;// unit is byte
MP3_INFO_dwFilePosition		=	MP3_INFO_dwAudioFileSize+4	;// unit is byte (need update)
MP3_INFO_dwTimeLength		=	MP3_INFO_dwFilePosition	+4	;
MP3_INFO_dwTimePosition		=	MP3_INFO_dwTimeLength	+4	;// unit is 1ms
MP3_INFO_dwFrameTotal		=	MP3_INFO_dwTimePosition	+4	;// unit is 1ms (need update)
MP3_INFO_dwFrameCount		=	MP3_INFO_dwFrameTotal	+4	;
MP3_INFO_bVersionID			=	MP3_INFO_dwFrameCount	+4	;
MP3_INFO_bLayer				=	MP3_INFO_bVersionID		+1	;
MP3_INFO_bVBRType			=	MP3_INFO_bLayer			+1	;
MP3_INFO_bChannel			=	MP3_INFO_bVBRType		+1	;
MP3_INFO_wBitRate			=	MP3_INFO_bChannel		+1	;
MP3_INFO_wSampleRate		=	MP3_INFO_wBitRate		+2	;// unit is 1 kbit/sec (avg.)
MP3_INFO_wFrameSize			=	MP3_INFO_wSampleRate	+2	;// unit is Hz
MP3_INFO_wSamplePerFrame	=	MP3_INFO_wFrameSize		+2	;// unit is byte
MP3_INFO_wFrameDuration     =   MP3_INFO_wSamplePerFrame+2  ;// unit is 0.1 milliseconds
MP3_INFO_bPaddingSize		=	MP3_INFO_wFrameDuration +2	;
MP3_INFO_bCRCs				=	MP3_INFO_bPaddingSize	+1	;
MP3_INFO_bPrivate			=	MP3_INFO_bCRCs			+1	;
MP3_INFO_bCopyright			=	MP3_INFO_bPrivate		+1	;
MP3_INFO_bOriginal			=	MP3_INFO_bCopyright		+1	;
MP3_INFO_bEmphasis			=	MP3_INFO_bOriginal		+1	;
MP3_INFO_wID3v1Size			=	MP3_INFO_bEmphasis		+1	;
MP3_INFO_dwID3v2Size		=	MP3_INFO_wID3v1Size		+2	;

;.define bitMP3_BitstreamFile	_MP3_CalculateFlag_.0;
;.define bitMP3_PauseFile		_MP3_CalculateFlag_.1;
;.define bitMP3_FastRewind		_MP3_CalculateFlag_.2;
;.define bitMP3_FastForward		_MP3_CalculateFlag_.3;
;.define bitMP3_ExtFileHandle	_MP3_CalculateFlag_.4;
;.define bitMP3_MIPsChange1		_MP3_CalculateFlag_.5;
;.define bitMP3_MIPsChange2		_MP3_CalculateFlag_.6;
;.define bitMP3_SoundProcess		_MP3_CalculateFlag_.7;
;.define bitMP3_DecodeError		_MP3_CalculateFlag_.8;
;.define bitMP3_Layer2Type		_MP3_CalculateFlag_.10; 
;.define bitMP3_Layer3Type		_MP3_CalculateFlag_.11; 
;.define bitMP3_NoHeaderVBR		_MP3_CalculateFlag_.12;
;.define bitMP3_JointStereoMode	_MP3_CalculateFlag_.13;
;.define bitVBR_InformationFlag	_MP3_CalculateFlag_.14;
;.define bitMP3_InformationFlag	_MP3_CalculateFlag_.15;

.define MP3_FILE_HEADER_ERROR	-4
.define MP3_FILE_SEARCH_FALL	-6

;// MP3_Information.bVersionID
.define MPEG_VERSION_1			1;// MPEG Version 1 (ISO/IEC 11172-3)
.define MPEG_VERSION_2			2;// MPEG Version 2 (ISO/IEC 13818-3)
.define MPEG_VERSION_RESERVED	3;// reserved
.define MPEG_VERSION_2P5		4;// MPEG Version 2.5 (later extension of MPEG 2)

;// MP3_Information.bLayer
.define MPEG_LAYER_1			1;// Layer I
.define MPEG_LAYER_2			2;// Layer II
.define MPEG_LAYER_3			3;// Layer III
.define MPEG_LAYER_RESERVED		4;// reserved

;// MP3_Information.bChannel
.define STEREO_SOUND 			0;// Stereo
.define JOINT_STEREO_SOUND		1;// Joint Stereo (Stereo)
.define DUAL_CHANNEL_SOUND		2;// Dual channel (2 Mono channels)
.define SINGLE_CHANNEL_SOUND	3;// Single channel (Mono)

;// MP3_Information.bVBRType
.define CONSTANT_BITRATE 		0;// Constant Bitrate
.define XING_VARIABLE_BITRATE	1;// Xing Variable Bitrate
.define VBRI_VARIABLE_BITRATE	2;// VBRI Variable Bitrate
.define NO_TOC_VARIABLE_BITRATE	3;// No Header Variable Bitrate

;// VBR_INFORMATION VBR_Information;// size 16 byte
VBR_INFO_dwTOC_Offset           = 0;
VBR_INFO_wVbriTOC_Size          = VBR_INFO_dwTOC_Offset+4;
VBR_INFO_wVbriDelay	            = VBR_INFO_wVbriTOC_Size+2;
VBR_INFO_wVbriTOC_Scale	        = VBR_INFO_wVbriDelay+2;
VBR_INFO_wVbriTOC_EntryBytes	= VBR_INFO_wVbriTOC_Scale+2;
VBR_INFO_wVbriTOC_EntryFrames	= VBR_INFO_wVbriTOC_EntryBytes+2;
