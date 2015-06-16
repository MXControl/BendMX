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

#ifndef LOGING_INCLUDED
#define LOGGING_INCLUDED

#define _NO_DEBUGTRACE

#ifdef _NO_DEBUGTRACE
#define INITTRACE(a)
#define LTRACE
#define ENTRY_TRACE
#define EXIT_TRACE
#else 

#define LOG_FILE_LINE_STAMP _T( "#FILE_LINE#" )

extern void LOG_SetLogFileName(const char* lpszLogFileName );

extern void DebugPrintf( const TCHAR* pzFormat, ...);

extern CString GetLastErrorString( );

#define CBendMXError(c) {ErrorCallBackFunc( __FILE__,__LINE__,c);}

#define INITTRACE( a ) static int gs_nDebug = QueryModuleDebugLevel( a );

#define LTRACE  if (gs_nDebug&0x04) DebugPrintf( _T( "%s%s(%d): "), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf
#define LTRACE1 if (gs_nDebug&0x08) DebugPrintf( _T( "%s%s(%d): "), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf
#define LTRACE2 if (gs_nDebug&0x10) DebugPrintf( _T( "%s%s(%d): "), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf
#define LTRACE3 if (gs_nDebug&0x20) DebugPrintf( _T( "%s%s(%d): "), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf

#define ENTRY_TRACE if (gs_nDebug&0x01) DebugPrintf( _T( "%s%s(%d): ENTRY:"), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf
#define EXIT_TRACE  if (gs_nDebug&0x02) DebugPrintf( _T( "%s%s(%d): EXIT :"), LOG_FILE_LINE_STAMP, _T( __FILE__ ), __LINE__), DebugPrintf

#endif

#ifdef _DEBUG
#define DTRACE DebugPrintf
#else
#define DTRACE
#endif



#endif /* LOGING_INCLUDED */
