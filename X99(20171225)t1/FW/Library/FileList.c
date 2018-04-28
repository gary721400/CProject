//--------------------------------------------------------------------------------------------------
#define FILELIST_CPP
//--------------------------------------------------------------------------------------------------
#include "fs.h"
#include "string.h"
#include "FileSearch.h"
#include "_FileSearch.h"
#include "FileList.h"
//--------------------------------------------------------------------------------------------------
#include "guiFont.h"
#include "guiBmp.h"
#include "BmpData_16bit.h"
#include "msgTimer.h"
//--------------------------------------------------------------------------------------------------
#pragma DATA = FILELIST_DATA
#pragma CODE = FILELIST_CODE

#if 1
//--------------------------------------------------------------------------------------------------
void Find_GetName_1( unsigned int lArrey, unsigned int iData, __X char *cLfn, __X char *cName );
//--------------------------------------------------------------------------------------------------
void Find_GetName_1( unsigned int lArrey, unsigned int iData, __X char *cLfn, __X char *cName )
{
	__X unsigned int *cLN;

	cLN = (__X unsigned int *)&eLongName[lArrey];
	if( Mstrwcpy( (char *)cLN, (char *)cLfn ) )
		iData |= 0x40;
	if( Check_LongFileName( (unsigned int *)cLN ) )
		iData |= 0x20;
	strcpy( (char*)eShortName[lArrey], (char*)cName );

	ecFileAttr[lArrey] = iData;
	eiExitCnt++;
}
//--------------------------------------------------------------------------------------------------
void Show_LineFileName( int line )
{
	int          iRollEnd, iFontBak, iTemp;
	unsigned int *pLN;

	iTemp = ecFileAttr[line];
	pLN = (unsigned int *)&eLongName[line];
	if( iTemp & 0x40 )
	{
		if( iTemp & 0x20 )
		{
			iFontBak = *(pLN+(iRollEnd=Roll_LongFileNameOut(pLN,0)));
			*(pLN+iRollEnd) = 0;
		}
		gui_font_showunicodestring( IE_POSX_STR, IE_POSY_STR+(IE_PIC_HINGTH*line), (unsigned short*)pLN );
		if( iTemp & 0x20 )
			*(pLN+iRollEnd) = iFontBak;
	}
	else
		gui_font_ShowString( IE_POSX_STR, IE_POSY_STR+(IE_PIC_HINGTH*line), eShortName[line] );
}
//--------------------------------------------------------------------------------------------------
void Show_RollFileName( void )
{
	int          iWidth, iRollEnd, iFontBak;
	unsigned int *pLN;

	gui_bmp_ShowPicture( IE_POSX_PIC, IE_POSY_PIC+(IE_PIC_HINGTH*eiCntShow), BmpId_FILE_03 );
	pLN = (unsigned int *)&eLongName[eiCntShow];
	if( !*(pLN+eiShowPos) )
	{
		iWidth = Roll_LongFileNameIn(pLN,iInRoll);
		iRollEnd = iInRoll++;
		iFontBak = *(pLN+iRollEnd);
		*(pLN+iRollEnd) = 0;
		gui_font_showunicodestring( IE_POSX_STR+IE_FONT_COUNT-iWidth, IE_POSY_STR+(IE_PIC_HINGTH*eiCntShow), (unsigned short*)pLN );
		if( IE_FONT_COUNT-iWidth == IE_FONTB_WIDTH )
			eiShowPos = 0;
		else if( IE_FONT_COUNT-iWidth < IE_FONTB_WIDTH )
			eiShowPos = 1;
	}
	else
	{
		iInRoll = 0;
		iFontBak = *(pLN+(iRollEnd=Roll_LongFileNameOut(pLN,eiShowPos)));
		*(pLN+iRollEnd) = 0;
		gui_font_showunicodestring( IE_POSX_STR, IE_POSY_STR+(IE_PIC_HINGTH*eiCntShow), (unsigned short*)pLN+eiShowPos++ );
	}
	*(pLN+iRollEnd) = iFontBak;
	timer_settimer( 0, IE_ROLL_TIME, smTimerModeOnce );
}
//--------------------------------------------------------------------------------------------------
const int iBmpId[] = { BmpId_FILE_02, BmpId_FILE_07, BmpId_FILE_09, BmpId_FILE_04, BmpId_FILE_08, BmpId_FILE_11 };
int Show_FrameFileName( void )
{
	int iLine;

	Find_GetName( elCntNow, eiLineMax );
	for( iLine=0; iLine<eiLineMax; iLine++ )
	{
		gui_bmp_ShowPicture( IE_POSX_PEXT, IE_POSY_PIC+(IE_PIC_HINGTH*iLine), iBmpId[ecFileAttr[iLine]&0x7] );
		if( iLine == eiCntShow )
			gui_bmp_ShowPicture( IE_POSX_PIC, IE_POSY_PIC+(IE_PIC_HINGTH*iLine), BmpId_FILE_03 );
		else
			LCM_WriteColor( IE_POSX_PIC, IE_POSY_PIC+(IE_PIC_HINGTH*iLine), IE_PIC_WIDTH, IE_PIC_HINGTH, IE_PIC_CLR_COLOR );
		Show_LineFileName( iLine );
	}
	return iLine;
}
//--------------------------------------------------------------------------------------------------
void Show_ChangeFileName( int Num )
{
	LCM_WriteColor( IE_POSX_PIC, IE_POSY_PIC+(IE_PIC_HINGTH*eiCntShow), IE_PIC_WIDTH, IE_PIC_HINGTH, IE_PIC_CLR_COLOR );
	Show_LineFileName( eiCntShow );
	gui_bmp_ShowPicture( IE_POSX_PIC, IE_POSY_PIC+(IE_PIC_HINGTH*(eiCntShow+=Num)), BmpId_FILE_03 );
	Show_LineFileName( eiCntShow );
}
//--------------------------------------------------------------------------------------------------
void Show_FrameIcon( void )
{
	//gui_bmp_ShowPicture( IE_POSX_ICON, IE_POSY_ICON, BmpId_FILE_05 );
	gui_bmp_ShowPicture( IE_POSX_ICONM, IE_POSY_ICONM+eiIconBak, BmpId_FILE_12 );
	eiIconBak = elCntMax>1?(IE_ICON_LENGTH*(elCntNow+eiCntShow))/(elCntMax-1):0;
	gui_bmp_ShowPicture( IE_POSX_ICONM, IE_POSY_ICONM+eiIconBak, BmpId_FILE_06 );
	eiShowPos = 1;
	if( ecFileAttr[eiCntShow] & 0x20 )
		timer_settimer( 0, IE_ROLL_TIME, smTimerModeOnce );
	else
		timer_disabletimer( 0 );
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int Roll_LongFileNameIn( unsigned int *LongName, int iNum )
{
	int iCnt, iWidth;

	for( iWidth=0,iCnt=0; iCnt<iNum; iCnt++ )
	{
		if( LongName[iCnt] < IE_UN_ASCII )
			iWidth += IE_FONTB_WIDTH;
		else
			iWidth += IE_FONTW_WIDTH;
	}
	return iWidth;
}
//--------------------------------------------------------------------------------------------------
int Roll_LongFileNameOut( unsigned int *LongName, int iOffset )
{
	int          iCnt, iWidth;
	unsigned int iFont;

	for( iWidth=0,iCnt=iOffset; iWidth<IE_FONT_COUNT; iCnt++ )
	{
		if( (iFont=LongName[iCnt]) )
		{
			if( iFont < IE_UN_ASCII )
				iWidth += IE_FONTB_WIDTH;
			else
			{
				if( (iWidth+IE_FONTW_WIDTH) > IE_FONT_COUNT )
					break;
				iWidth += IE_FONTW_WIDTH;
			}
		}
		else
			break;
	}
	return iCnt;
}
//--------------------------------------------------------------------------------------------------
int Check_LongFileName( unsigned int *LongName )
{
	unsigned int iCnt, iWidth, iFont;

	for( iWidth=iCnt=0; (iFont=LongName[iCnt]) != 0; iCnt++ )
	{
		iWidth += (IE_FONTB_WIDTH<<(iFont>=IE_UN_ASCII));
		if( iWidth > IE_FONT_COUNT )
			return -1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
int Find_DirPrev( void )
{
	int iTemp;

	if( (iTemp=Mmemschar( (__X char *)cFileName, Mstrlen( cFileName )-1, '\\' )) <= 0 )
		return -2;
	cFileName[iTemp+1] = '\0';
	return 0;
}
//--------------------------------------------------------------------------------------------------
int Find_DirNext( void )
{
	strcpy( cFileName+strlen( cFileName ), eShortName[eiCntShow] );
	strcpy( cFileName+strlen( cFileName ), "\\\0" );
	return 0;
}
//--------------------------------------------------------------------------------------------------
int Find_GetName( long Number, int MaxLine )
{
	long          lPassFile, lPassDire;
	unsigned int  iTemp, iTmpFile, iTmpDire, iCntFile, iCntDire;
	FILE_INFO     finfo;
	FS_ENTRY_INFO WMAgfsfentry;
	FS_ERR        fs_err;

	eiExitCnt = 0;
	iCntFile = iCntDire = iTmpDire = 0;
	lPassFile = lPassDire = 0;
	if( Number > lNumDire )
		lPassFile = Number - lNumDire;
	else
	{
		lPassDire = Number;
		if( (Number+MaxLine) > lNumDire )
		{
			if( Number <= lNumDire )
				iTmpDire = lNumDire - Number;
		}
		else
			iTmpDire = MaxLine;
	}
	iTmpFile = MaxLine-iTmpDire;

	if( (iTemp=Mmemschar( (__X char *)cFileName, Mstrlen( cFileName )+1, '\\' )) <= 0 )
		return -2;
	cFileName[iTemp] = '\0';
	fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
	cFileName[iTemp] = '\\';
	while( fs_err != FS_DRIVE_ERROR && fs_err != FS_END_DIRECTORY )
	{
		if( fs_err == FS_NO_ERROR )
		{
			if( finfo.attribute & FS_ATTR_ARCHIVE && iCntFile<iTmpFile )
			{
				if( (iTemp=MstrExt3( (__X char *)finfo.name )) != -1 )
				{
					if( lPassFile )
						lPassFile--;
					else
						Find_GetName_1( iTmpDire+iCntFile++, (iTemp>>4)+1, (__X char *)finfo.lfn, (__X char *)finfo.name );
				}
			}
			else	if( finfo.attribute&FS_ATTR_DIRECTORY && iCntDire<iTmpDire )
			{
				if( !Mmemcmp( ".", (char *)finfo.name, Mstrlen( (char *)finfo.name )+1 ) )
				{
					if( lPassDire )
						lPassDire--;
					else
						Find_GetName_1( iCntDire++, 0, (__X char *)finfo.lfn, (__X char *)finfo.name );
				}
			}
			if( eiExitCnt >= MaxLine )
				return 0;
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------
int Find_DirFile( void )
{
	int           iTemp;
	FILE_INFO     finfo;
	FS_ENTRY_INFO WMAgfsfentry;
	FS_ERR        fs_err;

	lNumFile = 0;
	lNumDire = 0;
	if( (iTemp=Mmemschar( (__X char *)cFileName, Mstrlen( cFileName )+1, '\\' )) <= 0 )
		return -2;
	cFileName[iTemp] = '\0';
	fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
	cFileName[iTemp] = '\\';
	while( fs_err != FS_DRIVE_ERROR && fs_err != FS_END_DIRECTORY )
	{
		if( fs_err == FS_NO_ERROR )
		{
			if( finfo.attribute & FS_ATTR_ARCHIVE )
			{
				if( MstrExt3( (__X char *)finfo.name ) != -1 )
					lNumFile++;
			}
			else	if( finfo.attribute&FS_ATTR_DIRECTORY )
			{
				if( !Mmemcmp( ".", (char *)finfo.name, Mstrlen( (char *)finfo.name )+1 ) )
					lNumDire++;
			}
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	return -1;
}

#endif //#if 0
