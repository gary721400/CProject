//--------------------------------------------------------------------------------------------------
//#pragma DATA = FILESEARCH_DATA
//#pragma CODE = FILESEARCH_CODE
//--------------------------------------------------------------------------------------------------
#define	FILE_LEVEL		32
#define	FILE_EXT		8
#define	FILE_CACHE_SIZE		0		// ?
#define	FILE_CACHE_INTERVAL	1		// ?
//------------------------------------------------------
#define	FILE_MODE_LOCAL		0
#define	FILE_MODE_ALL		1
//--------------------------------------------------------------------------------------------------
#define	MLFN_SIZE		128
#define	NULL_LFN		((__X unsigned int *)0)
//--------------------------------------------------------------------------------------------------
extern void Find_Initial( int level, char *icExt );
extern int  Find_PathFile( __X char *cFile, int Mode, char *icPathName );
extern int  Find_FileName( __X char *cFile, __X unsigned int *iclFileName );
//--------------------------------------------------------------------------------------------------
// R0[rb0,rb1], R1[rb2,rb3], R2[rb4,rb5], R3[rb6,rb7], R4[rb8,rb9], R5[rb10,rb11], R6[rb12,rb13], R7[rb14,rb15].
// PTR0[ES0:R3], PTR1[ES1:R4].
//--------------------------------------------------------------------------------------------------
#ifdef FILESEARCH_CPP
	//------------------------------------------------------------------------------------------
	#pragma DATA = FILESEARCH_DATA
	//------------------------------------------------------------------------------------------
	unsigned int  iMode, iMaxLevel, iPathIndex[FILE_LEVEL], iIndexCache[(FILE_CACHE_SIZE+1)*2];
	char          cExt3[5*FILE_EXT+1];

	unsigned long lFileSize;
	long          lFileMax, lFileNow;
	long          lNumFile, lNumDire;
	unsigned int  iNamePos;
	char          cFileName1[3+13*FILE_LEVEL+1];
	char          cFileName2[3+13*FILE_LEVEL+1];
	//unsigned int cLongFileName[MLFN_SIZE+1];
	//------------------------------------------------------------------------------------------
#else
	//------------------------------------------------------------------------------------------
	extern __X unsigned long lFileSize;
	extern __X long          lFileMax, lFileNow;
	extern __X long          lNumFile, lNumDire;
	extern __X unsigned int  iNamePos;
	extern __X char          cFileName1[], cFileName2[];
	//extern unsigned int cLongFileName[];
	//------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------