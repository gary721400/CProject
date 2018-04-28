//--------------------------------------------------------------------------------------------------
#define FILESEARCH_CPP
//--------------------------------------------------------------------------------------------------
#include "fs.h"
#include "string.h"
#include "FileSearch.h"
#include "_FileSearch.h"
//--------------------------------------------------------------------------------------------------
#pragma DATA = FILESEARCH_DATA
#pragma CODE = FILESEARCH_CODE
//--------------------------------------------------------------------------------------------------
void Find_Initial( int level, char *icExt )
{
	iMaxLevel = level;
	Mstrcpy( cExt3, icExt );
}
//--------------------------------------------------------------------------------------------------
int  Find_PathFile( __X char *cFile, int Mode, char *icPathName )
{
	unsigned int  iIndexCnt, iCacheCnt;
	int           iFinfoLen, iFileNameLen, iLevel;
	int           iPathLen, iTempLen, iPathChk, iFlag;
	FILE_INFO     finfo;
	FS_ENTRY_INFO WMAgfsfentry;
	FS_ERR        fs_err;

	iFlag = 0;
	iLevel = 0;
	iPathChk = 0;
	lFileMax = 0;
	lFileNow = 0;
	iIndexCnt = 0;
	iCacheCnt = FILE_CACHE_INTERVAL;
	iMode = Mode;
	if( (iFileNameLen=iPathLen=Mmemschar( (__X char *)cFile, Mmemcpy( cFile, icPathName, Mstrlen( icPathName )+1 ), '\\' )) <= 0 )
		return -2;
	if( iMode )
		cFile[iFileNameLen=iTempLen=Mmemachar( (__X char *)cFile, iPathLen, '\\' )] = '\0';
	else
		cFile[iFileNameLen] = '\0';
	fs_err = FSGetEntry( cFile, 0, &finfo, &WMAgfsfentry );
	while( (fs_err!=FS_DRIVE_ERROR) && (fs_err!=FS_FORMAT_ERROR) )
	{
		iFinfoLen = Mstrlen( (char *)finfo.name )+1;
		if( fs_err == FS_END_DIRECTORY )
		{
			if( !iLevel || !iMode )
			{
				if( iMode )
					Mmemcpy( cFile+iTempLen, "\\\0", 2 );
				else
					cFile[iPathLen] = '\\';
				return 0;
			}
			cFile[iFileNameLen=Mmemschar( (__X char *)cFile, iFileNameLen, '\\' )] = '\0';
			fs_err = FSGetEntry((char *)cFile, iPathIndex[--iLevel], &finfo, &WMAgfsfentry);
		}
		else if( fs_err == FS_NO_ERROR )
		{
			if( finfo.attribute & FS_ATTR_ARCHIVE )
			{
				if( !iFlag )
				{
					if( iPathChk || !iMode )
					{
						if( (iFlag=(Mmemcmp( icPathName+iPathLen+1, (char *)finfo.name, iFinfoLen ) || icPathName[iPathLen+1]=='\0')) )
							lFileNow = lFileMax;
					}
				}
				if( MstrExt3( (__X char *)finfo.name ) != -1 )
				{
					lFileMax++;
					if( ++iCacheCnt >= FILE_CACHE_INTERVAL )
					{
						iCacheCnt = 0;
						if( iIndexCnt < (FILE_CACHE_SIZE+1)*2 )
						{
							iIndexCache[iIndexCnt++] = iLevel?iPathIndex[iLevel-1]:0;
							iIndexCache[iIndexCnt++] = finfo.index;
						}
					}
				}
			}
			else	if( finfo.attribute&FS_ATTR_DIRECTORY && iLevel<iMaxLevel && iMode )
			{
				if( !Mmemcmp( ".", (char *)finfo.name, iFinfoLen ) && !Mmemcmp( "..", (char *)finfo.name, iFinfoLen ) )
				{
					iPathIndex[iLevel++] = finfo.index;
					cFile[iFileNameLen++] = '\\';
					Mmemcpy( cFile+iFileNameLen, (char *)finfo.name, iFinfoLen );
					iFileNameLen += iFinfoLen-1;
					iPathChk = Mmemcmp( cFile, icPathName, iPathLen ) && (icPathName[iFileNameLen]=='\\');
					finfo.attribute = 0;
					fs_err = FSGetEntry( cFile, 0, &finfo, &WMAgfsfentry );
					continue;
				}
			}
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	cFile[iFileNameLen] = '\\';
	return -1;
}
//--------------------------------------------------------------------------------------------------
int  Find_FileName( __X char *cFile, __X unsigned int *iclFileName )
{
	int           iFinfoLen, iFileNameLen, iLevel;
	int           iPathLen, iExtType;
	FILE_INFO     finfo;
	FS_ENTRY_INFO WMAgfsfentry;
	FS_ERR        fs_err;
	long          lCnt;

	lCnt = 0;
	iLevel = 0;
	if( (iFileNameLen=iPathLen=Mmemschar( (__X char *)cFile, Mstrlen( cFile )+1, '\\' )) <= 0 )
		return -2;
	if( iMode )
		cFile[iFileNameLen=Mmemachar( (__X char *)cFile, iPathLen, '\\' )] = '\0';
	else
		cFile[iFileNameLen] = '\0';
	fs_err = FSGetEntry( cFile, 0, &finfo, &WMAgfsfentry );
	while( (fs_err!=FS_DRIVE_ERROR) && (fs_err!=FS_FORMAT_ERROR) )
	{
		iFinfoLen = Mstrlen( (char *)finfo.name )+1;
		if( fs_err == FS_END_DIRECTORY )
		{
			if( !iLevel || !iMode )
			{
				cFile[iFileNameLen] = '\\';
				return -3;
			}
			cFile[iFileNameLen=Mmemschar( (__X char *)cFile, iFileNameLen, '\\' )] = '\0';
			fs_err = FSGetEntry((char *)cFile, iPathIndex[--iLevel], &finfo, &WMAgfsfentry);
		}
		else if( fs_err == FS_NO_ERROR )
		{
			if( finfo.attribute & FS_ATTR_ARCHIVE )
			{
				if( (iExtType=MstrExt3( (__X char *)finfo.name )) != -1 )
				{
					if( lCnt++ >= lFileNow )
					{
						cFile[iFileNameLen++] = '\\';
						Mmemcpy( cFile+(iNamePos=iFileNameLen), (char *)finfo.name, iFinfoLen );
						if( iclFileName )
						{
							if( Mstrwcpy( (char *)iclFileName, (char *)finfo.lfn ) )
								iNamePos += (iFinfoLen-1);
						}
						lFileSize = finfo.size;
						return iExtType;
					}
				}
			}
			else	if( finfo.attribute&FS_ATTR_DIRECTORY && iLevel<iMaxLevel && iMode )
			{
				if( !Mmemcmp( ".", (char *)finfo.name, iFinfoLen ) && !Mmemcmp( "..", (char *)finfo.name, iFinfoLen ) )
				{
					iPathIndex[iLevel++] = finfo.index;
					cFile[iFileNameLen++] = '\\';
					Mmemcpy( cFile+iFileNameLen, (char *)finfo.name, iFinfoLen );
					iFileNameLen += iFinfoLen-1;
					finfo.attribute = 0;
					fs_err = FSGetEntry( cFile, 0, &finfo, &WMAgfsfentry );
					continue;
				}
			}
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	cFile[iFileNameLen] = '\\';
	return -1;
}