//--------------------------------------------------------------------------------------------------
//#include "fs.h"
//--------------------------------------------------------------------------------------------------
//#pragma DATA = FILELIST_DATA
//#pragma CODE = FILELIST_CODE
//--------------------------------------------------------------------------------------------------
#define	IE_LINE_MAX		9
#define	IE_FONT_COUNT		(8*16)
#define	IE_FONTB_WIDTH		8
#define	IE_FONTW_WIDTH		16
#define	IE_ROLL_TIME		500
#define	IE_UN_ASCII		256

#define	IE_POSX_PIC		26
#define	IE_POSX_STR		(26+2)
#define	IE_POSY_PIC		28
#define	IE_POSY_STR		(28+2)
#define	IE_POSX_PEXT		4
#define	IE_POSX_ICON		165
#define	IE_POSY_ICON		27
#define	IE_ICON_LENGTH		166
#define	IE_POSX_ICONM		166
#define	IE_POSY_ICONM		36

#define	IE_PIC_WIDTH		132
#define	IE_PIC_HINGTH		20
#define	IE_PIC_CLR_COLOR	0xFFFF

#define	IE_CLEAR_POSX		(IE_POSX_PIC-22)
#define	IE_CLEAR_WIDTH		(IE_PIC_WIDTH+22)
//------------------------------------------------------
extern int  Find_DirFile( void );
extern int  Find_DirNext( void );
extern int  Find_DirPrev( void );
extern int  Find_GetName( long Number, int MaxLine );
extern int  Check_LongFileName( unsigned int *LongName );
extern int  Roll_LongFileNameIn( unsigned int *LongName, int iNum );
extern int  Roll_LongFileNameOut( unsigned int *LongName, int iOffset );
extern void Show_LineFileName( int line );
extern int  Show_FrameFileName( void );
extern void Show_ChangeFileName( int Num );
extern void Show_RollFileName( void );
extern void Show_FrameIcon( void );
//--------------------------------------------------------------------------------------------------
#ifdef FILELIST_CPP
	//------------------------------------------------------------------------------------------
	#pragma DATA = FILELIST_DATA
	//------------------------------------------------------------------------------------------
	unsigned int eLongName[IE_LINE_MAX][MLFN_SIZE+1];
	char         eShortName[IE_LINE_MAX][SHORT_NAME_SIZE+1], ecFileAttr[FILE_LEVEL];
	long         elrCntNow[FILE_LEVEL];
	int          eirCntShow[FILE_LEVEL];

	int          eiLineMax, eiShowPos, eiCntShow, eiDrive, eiIconBak;
	long         elCntMax, elCntNow;

	int          eiExitCnt, iInRoll;
	//------------------------------------------------------------------------------------------
#else
	//------------------------------------------------------------------------------------------
	extern unsigned int eLongName[IE_LINE_MAX][MLFN_SIZE+1];
	extern char         eShortName[IE_LINE_MAX][SHORT_NAME_SIZE+1], ecFileAttr[FILE_LEVEL];
	extern long         elrCntNow[FILE_LEVEL];
	extern int          eirCntShow[FILE_LEVEL];

	extern int          eiLineMax, eiShowPos, eiCntShow, eiDrive, eiIconBak;
	extern long         elCntMax, elCntNow;
	//------------------------------------------------------------------------------------------
#endif
