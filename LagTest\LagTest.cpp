/*
 * BendMX - Chat extension library for WinMX.
 * Copyright (C) 2003-2005 by Thees Schwab
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact: tcwinkler@users.sourceforge.net
 *
*/

#include "stdafx.h"
#include "..\MXPlugin.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

void Init(void);
void Quit(void);
void OnEnterChannel(DWORD dwID, LPCTSTR lpszRoom);
void OnEnterUser(DWORD dwID, LPCTSTR lpszUsername);
void OnMessage(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction);
void OnLeaveUser(DWORD dwID, LPCTSTR lpszUsername);
void OnCloseChannel(DWORD dwID);
void OnInputHook(DWORD dwIP, CString *pInput);
BOOL InputMessage(DWORD dwID, LPCTSTR lpszMessage);
BOOL WriteEchoText(DWORD dwID, LPCTSTR lpszText);
void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);
void OnPrepareMenu (DWORD dwMenuID);
void OnOpMessage(DWORD dwID, CString strMessage);


BPLUGIN g_Plugin = {

	VERSION,  // API Version
	0x0004,   // Plugin ID
	"Lag Test Plugin",
	"Thees Schwab",
	NULL,     // Dll handle, filled in by BendMX
	NULL,
	NULL,
	NULL,
	Init,
	Quit,
	OnEnterChannel,
	OnEnterUser,
	OnMessage,
	OnOpMessage,
	OnLeaveUser,
	OnCloseChannel,
	OnInputHook,
	OnPrepareMenu,
	OnMenuCommand,
	NULL, // filled in by BendMX
};


DWORD g_iTestTic = -1;
DWORD g_dwID = -1;

LARGE_INTEGER g_liTest;
__int64 g_i64Freq = 0;

////////////////////////////////////////////////////////////////////////
// void Init(...) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void Init(void)
{
	
	LARGE_INTEGER liFreq;
	int nSupported =::QueryPerformanceFrequency(&liFreq);
	if(nSupported > 0){

		g_i64Freq = liFreq.QuadPart;
	}
	else{

		g_i64Freq = -1;
	}

}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void Quit(void)
{

}

////////////////////////////////////////////////////////////////////////
// void OnEnterChannel() - will be called when you enter a channel
//
// Parameters:
//  DWORD dwID - identnumber of the channel
//  LPCTSTR lpszRoom - pointer to a string containing the room name
////////////////////////////////////////////////////////////////////////
void OnEnterChannel(DWORD dwID, LPCTSTR lpszRoom)
{

}


////////////////////////////////////////////////////////////////////////
// void OnEnterUser() - will be called when a user enters a channel 
//
// Parameters:
//  DWORD dwID - identnumber of the channel
//  LPCTSTR lpszusernam - pointer to a string containing the username
////////////////////////////////////////////////////////////////////////
void OnEnterUser(DWORD dwID, LPCTSTR lpszUsername)
{

}


////////////////////////////////////////////////////////////////////////
// void OnEnterUser() - will be called when a user enters a channel 
//
// Parameters:
//  DWORD dwID - identnumber of the channel
//  LPCTSTR lpszusernam - pointer to a string containing the username
////////////////////////////////////////////////////////////////////////
void OnMessage(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction)
{

	if(g_iTestTic > 0){
		
		if((strMessage.Find(".;;.",0) != -1) && (dwID == g_dwID) && (g_i64Freq != -1)){

			
			LARGE_INTEGER liStop;
			__int64 i64Lag;

			::QueryPerformanceCounter(&liStop);
			
			CString strLag;
			i64Lag = liStop.QuadPart - g_liTest.QuadPart;
			
			__int64 iLag_us = (i64Lag * 1000000.0)/(double)g_i64Freq;
			
			int nMs = iLag_us/1000;
			
			int nSec = nMs/1000;
			
			nMs = nMs%1000;

			int nUs = iLag_us%1000;

			strLag.Format("Lag in channel is %03ds, %03dms and %03dµs", nSec, nMs, nUs);

			g_Plugin.InputMessage(dwID, strLag);
			

			g_iTestTic = -1;
			g_dwID = -1;
		}
		else if((strMessage.Find(".;.", 0) != -1) && (dwID == g_dwID)){
		
			DWORD nLag = ::GetTickCount() - g_iTestTic;
			CString strLag;
			if(nLag < 1000){

				strLag.Format("Lag in this channel is %03d milliseconds", nLag);
			}
			else{

				strLag.Format("Lag in this channel is %02d seconds and %03d milliseconds", nLag/1000, nLag%1000);
			}
			g_Plugin.InputMessage(g_dwID, strLag);
			g_iTestTic = -1;
			g_dwID = -1;
		}
	}

}

void OnOpMessage(DWORD dwID, CString strMessage)
{

}


////////////////////////////////////////////////////////////////////////
// void OnLeaveUser() - will be called when a user leaves a channel 
//
// Parameters:
//  DWORD dwID - identnumber of the channel
//  LPCTSTR lpszusernam - pointer to a string containing the username
////////////////////////////////////////////////////////////////////////
void OnLeaveUser(DWORD dwID, LPCTSTR lpszUsername)
{

}


////////////////////////////////////////////////////////////////////////
// void OnCloseChannel() - will be called after a channel is closed
//
// Parameters:
//  DWORD dwID - identnumber of the channel that is closed
////////////////////////////////////////////////////////////////////////
void OnCloseChannel(DWORD dwID)
{

}


////////////////////////////////////////////////////////////////////////
// void OnInputHook() - will be called when a user enters a message to
//                      the chat edit control
//
// Parameters:
//  DWORD dwIP  - identnumber of the edit control
//  CString *pInput - Pointer to a string containing the input
////////////////////////////////////////////////////////////////////////
void OnInputHook(DWORD dwIP, CString *pInput)
{

	if(*pInput == "LAG"){

		g_dwID = dwIP;
		g_Plugin.InputMessage(dwIP, ".;.");
		g_iTestTic = ::GetTickCount();
		*pInput = "";
		

	}
	if(*pInput == "LAG+"){

		if(g_i64Freq != -1){

			g_dwID = dwIP;
			g_Plugin.InputMessage(dwIP, ".;;.");
			::QueryPerformanceCounter(&g_liTest);
			g_iTestTic = 1;
		}
		*pInput = "";
		

	}
}

void OnPrepareMenu (DWORD dwMenuID)
{

}


void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName)
{

	if(nCmdID == 23002){

		g_dwID = dwChannelID;
		g_Plugin.InputMessage(dwChannelID, ".;.");
		g_iTestTic = ::GetTickCount();
	}
}
__declspec( dllexport ) PBPLUGIN GetPluginAPI(){

	return &g_Plugin;
}

