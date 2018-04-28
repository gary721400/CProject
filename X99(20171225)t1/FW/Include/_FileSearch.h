//--------------------------------------------------------------------------------------------------
//#pragma DATA = FILESEARCH_DATA
//#pragma CODE = FILESEARCH_CODE
//------------------------------------------------------
// ALL Buffer Address In 24Bits(16MBytes).
// R0, R1, ES0, ES1, No PUSH/POP.
extern int  Mstrlen( char *icStr );
	//  Return len, Max 512Bytes.
extern int  Mmemcmp( char *ic1, char *ic2, unsigned int count );
	//  Return 0 : No Ok.
	//  Return 1 : Ok.
extern int  Mmemcpy( char *ic1, char *ic2, unsigned int count );
extern int  Mstrcpy( char *ic1, char *ic2 );
	//  Return copy len, Max 512Bytes.
extern int  Mstrwcpy( char *ic1, char *ic2 );
	//  Return copy len, Max 512Bytes.
//------------------------------------------------------
// ALL Buffer Address In 16Bits(64KBytes).
// R0, R1, DS0, DS1, No PUSH/POP.
extern int  MstrExt3( __X char *icExt );
	//  Return -1 : No Ok.
	//  Return ?  : FileExt Type.
extern int  Mmemachar( __X char *icStr, int count, int ascii );
	//  Return Ascii Address.
extern int  Mmemschar( __X char *icStr, int count, int ascii );
	//  Return Ascii Address.
//--------------------------------------------------------------------------------------------------