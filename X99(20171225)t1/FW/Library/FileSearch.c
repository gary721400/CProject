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
int  Find_PathFile( int Mode, char *icPathName )
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
	if( (iFileNameLen=iPathLen=Mmemschar( (__X char *)cFileName, Mmemcpy( cFileName, icPathName, Mstrlen( icPathName )+1 ), '\\' )) <= 0 )
		return -2;
	if( iMode )
		cFileName[iFileNameLen=iTempLen=Mmemachar( (__X char *)cFileName, iPathLen, '\\' )] = '\0';
	else
		cFileName[iPathLen] = '\0';
	fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
	while( fs_err != FS_DRIVE_ERROR )
	{
		iFinfoLen = Mstrlen( (char *)finfo.name )+1;
		if( fs_err == FS_END_DIRECTORY )
		{
			if( !iLevel || !iMode )
			{
				if( iMode )
					Mmemcpy( cFileName+iTempLen, "\\\0", 2 );
				else
					cFileName[iPathLen] = '\\';
				return 0;
			}
			cFileName[iFileNameLen=Mmemschar( (__X char *)cFileName, iFileNameLen, '\\' )] = '\0';
			fs_err = FSGetEntry((char *)cFileName, iPathIndex[--iLevel], &finfo, &WMAgfsfentry);
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
					cFileName[iFileNameLen++] = '\\';
					Mmemcpy( cFileName+iFileNameLen, (char *)finfo.name, iFinfoLen );
					iFileNameLen += iFinfoLen-1;
					iPathChk = Mmemcmp( cFileName, icPathName, iPathLen ) && (icPathName[iFileNameLen]=='\\');
					fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
					continue;
				}
			}
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	cFileName[iPathLen] = '\\';
	return -1;
}
//--------------------------------------------------------------------------------------------------
int  Find_FileName( unsigned int *iclFileName )
{
	int           iFinfoLen, iFileNameLen, iLevel;
	int           iPathLen, iExtType;
	FILE_INFO     finfo;
	FS_ENTRY_INFO WMAgfsfentry;
	FS_ERR        fs_err;
	long          lCnt;

	lCnt = 0;
	iLevel = 0;
	if( (iFileNameLen=iPathLen=Mmemschar( (__X char *)cFileName, Mstrlen( cFileName )+1, '\\' )) <= 0 )
		return -2;
	if( iMode )
		cFileName[iFileNameLen=Mmemachar( (__X char *)cFileName, iPathLen, '\\' )] = '\0';
	else
		cFileName[iPathLen] = '\0';
	fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
	while( fs_err != FS_DRIVE_ERROR )
	{
		iFinfoLen = Mstrlen( (char *)finfo.name )+1;
		if( fs_err == FS_END_DIRECTORY )
		{
			if( !iLevel || !iMode )
				return -3;
			cFileName[iFileNameLen=Mmemschar( (__X char *)cFileName, iFileNameLen, '\\' )] = '\0';
			fs_err = FSGetEntry((char *)cFileName, iPathIndex[--iLevel], &finfo, &WMAgfsfentry);
		}
		else if( fs_err == FS_NO_ERROR )
		{
			if( finfo.attribute & FS_ATTR_ARCHIVE )
			{
				if( (iExtType=MstrExt3( (__X char *)finfo.name )) != -1 )
				{
					if( lCnt++ >= lFileNow )
					{
						cFileName[iFileNameLen++] = '\\';
						Mmemcpy( cFileName+(iNamePos=iFileNameLen), (char *)finfo.name, iFinfoLen );
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
					cFileName[iFileNameLen++] = '\\';
					Mmemcpy( cFileName+iFileNameLen, (char *)finfo.name, iFinfoLen );
					iFileNameLen += iFinfoLen-1;
					fs_err = FSGetEntry( cFileName, 0, &finfo, &WMAgfsfentry );
					continue;
				}
			}
		}
		fs_err = FSGetNextEntry( &finfo, &WMAgfsfentry );
	}
	return -1;
}