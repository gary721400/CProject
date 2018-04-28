//--------------------------------------------------------------------------------------------------
//#pragma DATA = FILESEARCH_DATA
//#pragma CODE = FILESEARCH_CODE
#include "compiler.h"

extern char	__X	g_full_path[];
extern FILE_INFO __X	fat_g_info;
extern unsigned int current_find_num;
//--------------------------------------------------------------------------------------------------
//#define	FILE_LEVEL		32
//#define	FILE_EXT		8
//#define	FILE_CACHE_SIZE		0		// ?
//#define	FILE_CACHE_INTERVAL	1		// ?
//------------------------------------------------------
//#define	FILE_MODE_LOCAL		0
//#define	FILE_MODE_ALL		1
//--------------------------------------------------------------------------------------------------
//#define	MLFN_SIZE		128
//#define	NULL_LFN		((unsigned int *)0)
//--------------------------------------------------------------------------------------------------
//extern void Find_Initial( int level, char *icExt );
//extern int  Find_PathFile( int Mode, char *icPathName );
	//  Return 0  : Ok.
	//  Return -1 : FAT DRIVE ERROR.
	//  Return -2 : icPathName String No '\'.
//extern int  Find_FileName( unsigned int *iclFilename );
	//  Return -1 : FAT DRIVE ERROR.
	//  Return -2 : cFilename String No '\'.
	//  Return -3 : It is unable to find.
	//  Return ?  : FileExt Type.
//--------------------------------------------------------------------------------------------------
// R0[rb0,rb1], R1[rb2,rb3], R2[rb4,rb5], R3[rb6,rb7], R4[rb8,rb9], R5[rb10,rb11], R6[rb12,rb13], R7[rb14,rb15].
// PTR0[ES0:R3], PTR1[ES1:R4].
//--------------------------------------------------------------------------------------------------
#if 0//def FILESEARCH_CPP
	//------------------------------------------------------------------------------------------
	#pragma DATA = FILESEARCH_DATA
	//------------------------------------------------------------------------------------------
	unsigned int  iMode, iMaxLevel, iPathIndex[FILE_LEVEL], iIndexCache[(FILE_CACHE_SIZE+1)*2];
	char          cExt3[5*FILE_EXT+1];

	unsigned long lFileSize;
	long          lFileMax, lFileNow;
	long          lNumFile, lNumDire;
	unsigned int  iNamePos;
	char          cFileName[3+13*FILE_LEVEL+1];
	//unsigned int cLongFileName[MLFN_SIZE+1];
	//------------------------------------------------------------------------------------------
//#else
	//------------------------------------------------------------------------------------------
	extern unsigned long lFileSize;
	extern long          lFileMax, lFileNow;
	extern long          lNumFile, lNumDire;
	extern unsigned int  iNamePos;
	extern char          cFileName[];
	//extern unsigned int cLongFileName[];
	//------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
//	Use Lowercase, Format char*4( .xxx )
//	Find_Initial( FILE_LEVEL, "" );			// Search *.*
//	Find_Initial( FILE_LEVEL, ".jpg.bmp" );		// Search *.jpg | *.bmp
//	Find_Initial( FILE_LEVEL, ".mp3.wma.wav" );	// Search *.mp3 | *.wma | *.wav
//	FILE_EXT = 0, Example: Find_Initial( FILE_LEVEL, "" );
//	FILE_EXT = 1, Example: Find_Initial( FILE_LEVEL, ".mp3" );
//	FILE_EXT = 2, Example: Find_Initial( FILE_LEVEL, ".mp3.wma" );
//	FILE_EXT = 3, Example: Find_Initial( FILE_LEVEL, ".mp3.wma.wav" );
//------------------------------------------------------
//	if 0:\		10 File [0-9]
//	     \123	 4 File [10-13]
//	         \1.bmp
//	         \2.bmp
//	         \3.bmp
//	         \4.bmp
//	     \456	 3 File [14-16]
//------
//	Find_PathFile( FILE_MODE_ALL,   "0:\\" );		//	lFileMax = 17, lFileNow = 0;
//	Find_PathFile( FILE_MODE_ALL,   "0:\\123\\" );		//	lFileMax = 17, lFileNow = 10;
//	Find_PathFile( FILE_MODE_ALL,   "0:\\123\\2.bmp" );	//	lFileMax = 17, lFileNow = 11;
//	Find_PathFile( FILE_MODE_LOCAL, "0:\\" );		//	lFileMax = 10, lFileNow = 0;
//	Find_PathFile( FILE_MODE_LOCAL, "0:\\123\\" );		//	lFileMax = 4,  lFileNow = 0;
//	Find_PathFile( FILE_MODE_LOCAL, "0:\\123\\3.bmp" );	//	lFileMax = 4,  lFileNow = 2;
//------
//	Get File      :	                                            Find_FileName();
//	Get Next File :	if( ++lFileNow >= lFileMax ) lFileNow = 0;  Find_FileName();
//	Get Prev File :	if( --lFileNow < 0 ) lFileNow = lFileMax-1; Find_FileName();
//------
//	Find_Initial( FILE_LEVEL, ".jpg.bmp,.mp3.wma.wav,.txt" );
//	Find_PathFile( FILE_MODE_ALL,   "0:\\" );
//	ExtType = Find_FileName();
//	if( ExtType == 0x00 ) >> ".jpg"
//	if( ExtType == 0x01 ) >> ".bmp"
//	if( ExtType == 0x10 ) >> ".mp3"
//	if( ExtType == 0x11 ) >> ".wma"
//	if( ExtType == 0x12 ) >> ".wav"
//------
//	cFileName : Out FSOpen Use PathName. "Please Don't write it!".
//	lFileSize : Out File Size.
//	lFileMax  : Out "cExt3" File Count Number. "Please Don't write it!".
//	lFileNow  : Out Now File Count Number.
//	iNamePos  : Out Position, Long FileName Have or No.
//------
//	Have Long FileName : if( cFileName[iNamePos] == '\0' )
//	     Show Long  FileName : gui_font_showunicodestring( 0, 0, (unsigned short*)lFilename );
//	No   Long FileName : if( cFileName[iNamePos] != '\0' )
//	     Show Short FileName : gui_font_ShowString( 0, 0, cFileName+iNamePos );
char * get_ext(char* str);
U32 file_name_lead( void );
int	file_get_ext_count(char * dir_path,char * ext);
Bool file_get_path(char * dir_path,char * ext,int index);
Bool search_path_file(char * dir_path, char * ext, long need_num);
//Bool getNumFilePath(char * dir_path,char * ext,int index);
//U8	checkExe(char * ext);
//--------------------------------------------------------------------------------------------------