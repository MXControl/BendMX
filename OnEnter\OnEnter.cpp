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
#include "OnEnter.h"
#include "..\Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Beachten Sie!
//
//		Wird diese DLL dynamisch an die MFC-DLLs gebunden,
//		muss bei allen von dieser DLL exportierten Funktionen,
//		die MFC-Aufrufe durchführen, das Makro AFX_MANAGE_STATE
//		direkt am Beginn der Funktion eingefügt sein.
//
//		Beispiel:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//		// Hier normaler Funktionsrumpf
//		}
//
//		Es ist sehr wichtig, dass dieses Makro in jeder Funktion
//		vor allen MFC-Aufrufen erscheint. Dies bedeutet, dass es
//		als erste Anweisung innerhalb der Funktion ausgeführt werden
//		muss, sogar vor jeglichen Deklarationen von Objektvariablen,
//		da ihre Konstruktoren Aufrufe in die MFC-DLL generieren
//		könnten.
//
//		Siehe MFC Technical Notes 33 und 58 für weitere
//		Details.
//

// LagTest.cpp : Definiert den Einsprungpunkt für die DLL-Anwendung.
//

#include "stdafx.h"
#include "..\MXPlugin.h"
#include <direct.h>
#include <afxtempl.h>
//#include <vector>

typedef struct MX_ROOMDATA {

	DWORD dwRoomID;
	TCHAR szRoomName[256];	

} ROOMDATA, *PROOMDATA;


void Init(void);
void Quit(void);
void OnEnterChannel(DWORD dwID, LPCTSTR lpszRoom);
void OnEnterUser(DWORD dwID, LPCTSTR lpszUsername);
void OnMessage(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction);
void OnOpMessage(DWORD dwID, CString strMessage);
void OnLeaveUser(DWORD dwID, LPCTSTR lpszUsername);
void OnCloseChannel(DWORD dwID);
void OnInputHook(DWORD dwIP, CString *pInput);
BOOL InputMessage(DWORD dwID, LPCTSTR lpszMessage);
void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);
void OnPrepareMenu (DWORD dwMenuID);

void RemoveRoomData(DWORD dwID);
void AddRoomData(DWORD dwID, LPCTSTR lpszRoomName);
PROOMDATA GetRoomData(DWORD dwID);

void SetRoomAction(DWORD dwID, CString strAction);
void RemoveRoomAction(DWORD dwID);
void DoRoomAction(DWORD dwID);

CArray<ROOMDATA, ROOMDATA> m_RoomList;
CString g_strWd;
CMenu *m_cfgMenu = 0;

BPLUGIN g_Plugin = {

	VERSION,  // API Version
	0x0006,   // Plugin ID
	"OnEnter Plugin",
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


void makemenu()
{

	if(m_cfgMenu != NULL){

		m_cfgMenu->DestroyMenu();
		delete m_cfgMenu;
		m_cfgMenu = 0;
	}
	m_cfgMenu = new CMenu;
	BOOL bReturn = TRUE;
	if(m_cfgMenu->CreatePopupMenu()){

		m_cfgMenu->AppendMenu(MF_STRING, 15004, "Manually edit Messages");
		g_Plugin.hCfgMenu = m_cfgMenu->m_hMenu;
	}
}

////////////////////////////////////////////////////////////////////////
// void Init(...) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void Init(void)
{


	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	g_strWd.Format("%s", szBuffer);
	g_strWd = g_strWd.Left(g_strWd.ReverseFind('\\')) + "\\Add-Ons";
	makemenu();
}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void Quit(void)
{

	if(m_cfgMenu != 0){

		m_cfgMenu->DestroyMenu();
		delete m_cfgMenu;
		m_cfgMenu = 0;
	}
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

	AddRoomData(dwID, lpszRoom);
	DoRoomAction(dwID);
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

	RemoveRoomData(dwID);
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

	if(!pInput)return;

	PROOMDATA pRoomData = GetRoomData(dwIP);
	if(pRoomData){
		CString csCmd = *pInput;
		csCmd.MakeLower();
		
		if(csCmd.Find("/onenter set") != -1){

			CString strAction = pInput->Mid(13);
			if(!strAction.IsEmpty()){

				SetRoomAction(dwIP, strAction);
			}
		}
		
		if(csCmd.Find("/onenter remove") != -1){

			RemoveRoomAction(dwIP);
		}
	}
}

void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName)
{

	if(nCmdID == 15004){

		ShellExecute(0, "open", g_strWd + "\\OnEnter.ini", 0, 0, SW_SHOW);
	}
}

void OnPrepareMenu (DWORD dwMenuID)
{

	if(dwMenuID == 1){

		makemenu();
	}
}

__declspec( dllexport ) PBPLUGIN GetPluginAPI(){

	return &g_Plugin;
}


PROOMDATA GetRoomData(DWORD dwID)
{
	int iNum = m_RoomList.GetSize();

	for(int i=0 ; i < iNum ; i++){

		if(m_RoomList[i].dwRoomID == dwID){
			
			return &(m_RoomList[i]);		
		}
	}

	return NULL;
}

void AddRoomData(DWORD dwID, LPCTSTR lpszRoomName)
{
	CString csRoomName = lpszRoomName;
	int iPos = csRoomName.ReverseFind('_');
	if(iPos != -1)csRoomName = csRoomName.Left(iPos);

	PROOMDATA pRoomData = GetRoomData(dwID);
	if(pRoomData){

		lstrcpy(pRoomData->szRoomName, csRoomName);
	}
	else{

		ROOMDATA RoomData;
		RoomData.dwRoomID = dwID;
		lstrcpy(RoomData.szRoomName, csRoomName);
		m_RoomList.Add(RoomData);
	}
}

void RemoveRoomData(DWORD dwID)
{
	int iNum = m_RoomList.GetSize();

	for(int i=0 ; i < iNum ; i++){

		if(m_RoomList[i].dwRoomID == dwID){

			m_RoomList.RemoveAt(i);
			return;
		}
	}
}

void SetRoomAction(DWORD dwID, CString strAction)
{

	CIni ini;
	ini.SetIniFileName(g_strWd + "\\OnEnter.ini");
	PROOMDATA pRd = GetRoomData(dwID);
	if(!pRd) return;

	CString strName = pRd->szRoomName;
	strName.Replace(1, '-');
	ini.SetValue("RoomList", strName, strAction);
}

void RemoveRoomAction(DWORD dwID)
{

	CIni ini;
	ini.SetIniFileName(g_strWd + "\\OnEnter.ini");
	PROOMDATA pRd = GetRoomData(dwID);
	if(!pRd) return;

	CString strName = pRd->szRoomName;
	strName.Replace(1, '-');

	ini.SetValue("RoomList", strName, "");
}

void DoRoomAction(DWORD dwID)
{

	CIni ini;
	ini.SetIniFileName(g_strWd + "\\OnEnter.ini");
	PROOMDATA pRd = GetRoomData(dwID);
	if(!pRd) return;

	CString strName = pRd->szRoomName;
	strName.Replace(1, '-');

	CString strAction = ini.GetValue("RoomList", strName, "");
	if(!strAction.IsEmpty()){

		g_Plugin.InputMessage(dwID, strAction);
	}
}


