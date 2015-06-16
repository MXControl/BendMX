/*
** Copyright (C) 1999-2002 Albert L. Faber
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <StdAfx.h>
#include "Logging.h"

#ifndef _NO_DEBUGTRACE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TCHAR g_LOG_lpszFileName[ MAX_PATH + 1 ] = {'\0',};


void LOG_SetLogFileName( const char* lpszLogFileName )
{
	strncpy( g_LOG_lpszFileName, lpszLogFileName, sizeof( g_LOG_lpszFileName ) );
	g_LOG_lpszFileName[ sizeof( g_LOG_lpszFileName ) - 1 ] = '\0'; 
}


CMutex g_DebugPrintfMutex( FALSE, _T( "BENDMXLOG" ) );


void DebugPrintf( const TCHAR* pzFormat, ...)
{
	BOOL	bLogFileLine = FALSE;


	USES_CONVERSION;

//#ifdef _DEBUG
    TCHAR szBuffer[ 8192 ];

	g_DebugPrintfMutex.Lock();

    va_list ap;

	// Clear buffer
	memset( szBuffer, 0x00, sizeof( szBuffer ) );

    va_start(ap, pzFormat);
    _vsntprintf(szBuffer, sizeof(szBuffer), pzFormat, ap);

	if ( 0 == _tcsnccmp( szBuffer, LOG_FILE_LINE_STAMP, _tcslen( LOG_FILE_LINE_STAMP ) ) )
	{
		bLogFileLine = TRUE;
	}

    FILE* fp = NULL;
	time_t t = {0,};
	struct tm* psTM;

	time(&t);
	psTM = localtime(&t);

	if ( _tcslen( g_LOG_lpszFileName ) )
	{
		fp = _tfopen( g_LOG_lpszFileName, _T( "a" ) );

		if ( NULL != fp )
		{
			if ( bLogFileLine )
			{
				_ftprintf( fp, _T( "%02d:%02d:%02d (%d): %s" ), psTM->tm_hour, psTM->tm_min, psTM->tm_sec,GetTickCount(), &szBuffer[ _tcslen( LOG_FILE_LINE_STAMP ) ] );
			}
			else
			{
				_ftprintf( fp, _T( "%s\n" ) , szBuffer );
			}

			fclose(fp);
		}
	}

#if defined _DEBUG || defined SPECIALDEBUG
	if ( bLogFileLine )
	{
	    OutputDebugString( &szBuffer[ _tcslen( LOG_FILE_LINE_STAMP ) ] );
	}
	else
	{
		_tcscat( szBuffer, _T( "\n\r" ) );
		OutputDebugString( szBuffer );
	}
#endif

	va_end(ap);

	g_DebugPrintfMutex.Unlock();

}

CString GetLastErrorString( )
{
	CString strRet;

	LPVOID lpMsgBuf = NULL;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,    NULL,
		GetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
		(LPTSTR) &lpMsgBuf,    0,    NULL );// Display the string.

	strRet = CString( (LPCTSTR)lpMsgBuf );

	LocalFree( lpMsgBuf );

	return strRet;
}

#endif