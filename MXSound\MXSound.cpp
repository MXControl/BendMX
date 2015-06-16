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
#include "..\Ini.h"
#include <mmsystem.h>
#include <afxtempl.h>
#include "..\Tokenizer.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

typedef struct MX_ROOMDATA {

	DWORD dwRoomID;
	TCHAR szRoomName[256];	
	BOOL bIsMuted;

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
BOOL WriteEchoText(DWORD dwID, LPCTSTR lpszText);
void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);
void OnPrepareMenu (DWORD dwMenuID);

void mute(DWORD dwID);
void mute(DWORD dwID, BOOL bMute);
void RemoveRoomData(DWORD dwID);
PROOMDATA AddRoomData(DWORD dwID, LPCTSTR lpszRoomName);
PROOMDATA GetRoomData(DWORD dwID);
CArray<ROOMDATA, ROOMDATA> m_RoomList;

BPLUGIN g_Plugin = {

	VERSION,  // API Version
	0x0001,   // Plugin ID
	"MXSound Addon for BendMX",
	"Thees Schwab",
	NULL,     // Dll handle, filled in by BendMX
	NULL,	  // config menu
	NULL,	  // user menu
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


CString		 m_strWd;
CStringArray m_aEvents;
CStringArray m_aSounds;
CIni         m_ini;
CMenu*		 m_cfgMenu = 0;
CMenu*		 m_sndMenu = 0;

void ParseIniFile()
{

	CString strIniFile = m_strWd + "\\MXSound.ini";

	CStdioFile ini;
	CString strBuffer;

	m_aEvents.RemoveAll();
	m_aSounds.RemoveAll();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText);

		while(ini.ReadString(strBuffer)){

			strBuffer.TrimLeft();
			strBuffer.TrimRight();
			//strBuffer.MakeUpper();

			if(strBuffer.Left(2) == "//") continue;
			if(strBuffer.Find("[Sound]", 0) == 0) continue;
			
			if(strBuffer.Left(1) != "#") continue;
			else{

				strBuffer = strBuffer.Mid(1);
				if(strBuffer.IsEmpty()) continue;

				CString strEvent, strSound;

				CTokenizer token(strBuffer, "¨");
				if(!token.Next(strEvent) || !token.Next(strSound)) continue;
				
				m_aEvents.Add(strEvent);
				if(strSound.GetLength() < 8){

					strSound = m_strWd + "\\SoundFX\\" + strSound;
				}
				else if(strSound.GetAt(1) != ':'){

					strSound = m_strWd + "\\SoundFX\\" + strSound;
				}
				m_aSounds.Add(strSound);
			}
		}
		ini.Close();
	}
	CATCH(CFileException, e){

		AfxMessageBox("Error during file operation!", MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

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

		m_cfgMenu->AppendMenu(MF_STRING, 15022, "Mute (all rooms)");
		m_cfgMenu->AppendMenu(MF_STRING, 15023, "De-Mute (all rooms)");
		m_cfgMenu->AppendMenu(MF_SEPARATOR, 0, "");
		m_cfgMenu->AppendMenu(MF_STRING, 15000, "Configure sounds");
		m_cfgMenu->AppendMenu(MF_STRING, 15001, "Reload Settings");
		g_Plugin.hCfgMenu = m_cfgMenu->m_hMenu;
	}
}

int g_nMenuBase = 200000;

void makesoundmenu()
{

	if(m_sndMenu != NULL){

		m_sndMenu->DestroyMenu();
		delete m_sndMenu;
		m_sndMenu = 0;
	}

	m_sndMenu = new CMenu;
	BOOL bReturn = TRUE;
	
	if(m_sndMenu->CreatePopupMenu()){

		for(int i = 0; i < m_aEvents.GetSize(); i++){

			m_sndMenu->AppendMenu(MF_STRING, g_nMenuBase + i, m_aEvents[i]);
		}
		
		makemenu();
		g_Plugin.hCfgMenu = 0;
		m_sndMenu->AppendMenu(MF_POPUP, (UINT)m_cfgMenu->m_hMenu, "Configuration");
		g_Plugin.hChatMenu = m_sndMenu->m_hMenu;
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
	m_strWd.Format("%s", szBuffer);

	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\')) + "\\Add-Ons";
	ParseIniFile();
	m_ini.SetIniFileName(m_strWd + "\\Plugins.ini");
	makemenu();
	makesoundmenu();
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
	if(m_sndMenu != 0){

		m_sndMenu->DestroyMenu();
		delete m_sndMenu;
		m_sndMenu = 0;
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

	PROOMDATA pRoom = AddRoomData(dwID, lpszRoom);
	
	pRoom->bIsMuted = m_ini.GetValue("MXSound", pRoom->szRoomName, 0);

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

	PROOMDATA pRd = GetRoomData(dwID);

	if(!pRd) return;
	if(pRd->bIsMuted) return;
	CString strMessage;
	strMessage.Format("%s has entered", lpszUsername);

	for(int i = 0; i < m_aEvents.GetSize(); i++){

		if(strMessage.Find(m_aEvents.GetAt(i), 0) >= 0){

			if(!PlaySound(m_aSounds.GetAt(i), NULL, SND_FILENAME|SND_ASYNC)){

				TRACE("error\n");
			}
			else return;
		}
	}
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
	PROOMDATA pRd = GetRoomData(dwID);

	if(!pRd) return;
	if(pRd->bIsMuted) return;
	CString strMsg = strMessage;
	strMsg.MakeLower();

	for(int i = 0; i < m_aEvents.GetSize(); i++){

		if(strMsg.Find(m_aEvents.GetAt(i), 0) >= 0){

			PlaySound(m_aSounds.GetAt(i), NULL, SND_FILENAME|SND_ASYNC);
			break;
		}
	}
}

void OnOpMessage(DWORD dwID, CString strMessage)
{

	OnMessage(dwID, "", strMessage, FALSE);
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

	PROOMDATA pRd = GetRoomData(dwID);

	if(!pRd) return;
	if(pRd->bIsMuted) return;
	CString strMessage;
	strMessage.Format("%s has left", lpszUsername);

	for(int i = 0; i < m_aEvents.GetSize(); i++){

		if(strMessage.Find(m_aEvents.GetAt(i), 0) >= 0){

			if(!PlaySound(m_aSounds.GetAt(i), NULL, SND_FILENAME|SND_ASYNC)){

				TRACE("error\n");
			}
			else return;
		}
	}
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
//  CString strInput - Pointer to a string containing the input
////////////////////////////////////////////////////////////////////////
void OnInputHook(DWORD dwIP, CString *pInput)
{

	if(*pInput == "/sound load"){

		CHARFORMAT cf; 

		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR|CFM_PROTECTED|CFM_BOLD;
		cf.dwEffects = CFE_PROTECTED;
		cf.crTextColor = RGB(22, 155, 204);
		ParseIniFile();
		*pInput = "";
	}
	else if(*pInput == "/sound config"){

		CHARFORMAT cf; 

		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR|CFM_PROTECTED|CFM_BOLD;
		cf.dwEffects = CFE_PROTECTED;
		cf.crTextColor = RGB(22, 155, 204);
		ShellExecute(NULL, "open", m_strWd + "\\Configurator.exe", NULL, m_strWd, SW_SHOW);

		*pInput = "";
	}
	else if(*pInput == "/mute"){
	
		CHARFORMAT cf; 

		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR|CFM_PROTECTED|CFM_BOLD;
		cf.dwEffects = CFE_PROTECTED;
		cf.crTextColor = RGB(22, 155, 204);
		
		mute(dwIP);		
		*pInput = "";
	}
}

void OnPrepareMenu (DWORD dwMenuID)
{

    if(dwMenuID == 1)
    {

        makemenu();
    }
	else if(dwMenuID == 2){


		makesoundmenu();
	}
}



void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName)
{

	if(nCmdID == 15000){ // edit sounds
 
		ShellExecute(NULL, "open", m_strWd + "\\Configurator.exe", NULL, m_strWd, SW_SHOW);
	}
	else if(nCmdID == 15001){  // reload settings

		ParseIniFile();
	}
	else if(nCmdID == 15023){ // activate sound

		for(int i = 0; i < m_RoomList.GetSize(); i++){

			mute(m_RoomList[i].dwRoomID, FALSE);
		}
	}
	else if(nCmdID == 15022){ // mute

		for(int i = 0; i < m_RoomList.GetSize(); i++){

			mute(m_RoomList[i].dwRoomID, TRUE);
		}
	}
	else if(nCmdID >= g_nMenuBase && nCmdID < g_nMenuBase + m_aEvents.GetSize()){ // play sound

		int nIndex = nCmdID - g_nMenuBase;
		if(nIndex < m_aEvents.GetSize()){

			g_Plugin.InputMessage(dwChannelID, m_aEvents[nIndex]);
		}
	}
}


void mute(DWORD dwID)
{

	PROOMDATA pRd = GetRoomData(dwID);
	if(!pRd) return;
	
	CHARFORMAT cf; 

	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR|CFM_PROTECTED|CFM_BOLD;
	cf.dwEffects = CFE_PROTECTED;
	cf.crTextColor = RGB(22, 155, 204);

	pRd->bIsMuted = !pRd->bIsMuted;
	m_ini.SetValue("MXSound", pRd->szRoomName, pRd->bIsMuted);
}

void mute(DWORD dwID, BOOL bMute)
{

	PROOMDATA pRd = GetRoomData(dwID);
	if(!pRd) return;
	
	CHARFORMAT cf; 

	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR|CFM_PROTECTED|CFM_BOLD;
	cf.dwEffects = CFE_PROTECTED;
	cf.crTextColor = RGB(22, 155, 204);

	pRd->bIsMuted = bMute;
	m_ini.SetValue("MXSound", pRd->szRoomName, pRd->bIsMuted);
}


PROOMDATA GetRoomData(DWORD dwID)
{
	int iNum = m_RoomList.GetSize();

	for(int i = 0 ; i < iNum; i++){

		if(m_RoomList[i].dwRoomID == dwID){
			
			return &(m_RoomList[i]);		
		}
	}

	return NULL;
}

PROOMDATA AddRoomData(DWORD dwID, LPCTSTR lpszRoomName)
{

	CString csRoomName = lpszRoomName;
	int iPos = csRoomName.ReverseFind('_');
	if(iPos != -1)csRoomName = csRoomName.Left(iPos);

	csRoomName.Remove('?');
	csRoomName.Remove(':');
	csRoomName.Remove(',');
	csRoomName.Remove('\\');
	csRoomName.Remove('/');
	csRoomName.Remove('<');
	csRoomName.Remove('>');
	csRoomName.Remove('\"');
	csRoomName.Remove('*');
	csRoomName.Remove('|');
	csRoomName.Replace(1, '-');

	PROOMDATA pRoomData = GetRoomData(dwID);
	if(pRoomData){
	
		lstrcpy(pRoomData->szRoomName, csRoomName);
		return pRoomData;
	}
	else{

		ROOMDATA RoomData;
		RoomData.dwRoomID = dwID;
		RoomData.bIsMuted = TRUE;
;		lstrcpy(RoomData.szRoomName, csRoomName);
		int n = m_RoomList.Add(RoomData);
		return &m_RoomList[n];
	}

	return 0;
}

void RemoveRoomData(DWORD dwID)
{
	int iNum = m_RoomList.GetSize();

	for(int i = 0; i < iNum; i++){

		if(m_RoomList[i].dwRoomID == dwID){

			m_RoomList.RemoveAt(i);
			return;
		}
	}
}
__declspec( dllexport ) PBPLUGIN GetPluginAPI(){

	return &g_Plugin;
}

