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
#include <direct.h>
#include <afxtempl.h>
#include "..\ini.h"

//#include <vector>

typedef struct MX_ROOMDATA{

	DWORD dwRoomID;
	TCHAR szRoomName[256];
	TCHAR szRealName[256];
	BOOL  bLog;
} ROOMDATA, *PROOMDATA;


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
void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);
void OnPrepareMenu (DWORD dwMenuID);
void OnOpMessage(DWORD dwID, CString strMessage);

void LogMessage(DWORD dwID, LPCTSTR lpszMessage);
void RemoveRoomData(DWORD dwID);
void AddRoomData(DWORD dwID, LPCTSTR lpszRoomName);
PROOMDATA GetRoomData(DWORD dwID);

CArray<ROOMDATA, ROOMDATA> m_RoomList;
CString g_strWd;
CString g_strLogPath;
CMenu*	g_cfgMenu = 0;
CMenu*	g_chatMenu = 0;

BPLUGIN g_Plugin = {

	VERSION,  // API Version
	0x0002,   // Plugin ID
	"Chatlog Plugin",
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
	g_strWd = g_strWd.Left(g_strWd.ReverseFind('\\')) + "\\Add-Ons\\";
	
	CIni ini;
	ini.SetIniFileName(g_strWd + "chatlog.ini");
	g_strLogPath = ini.GetValue("Chatlog", "Path", "");
	
	if(g_strLogPath.IsEmpty()){

		g_strLogPath = g_strWd + "Chatlogs\\";
		ini.SetValue("Chatlog", "Path", g_strLogPath);
	}
	if(g_strLogPath.GetAt(g_strLogPath.GetLength()-1) != '\\'){
	
		g_strLogPath += "\\";
	}
	_mkdir(g_strLogPath);
}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void Quit(void)
{

	if(g_cfgMenu != 0){

		g_cfgMenu->DestroyMenu();
		delete g_cfgMenu;
		g_cfgMenu = 0;
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
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strEnterRoom;
	strEnterRoom.Format("<span class=\"topic\"><b>[%02d/%02d/%04d at %02d:%02d.%02d] Entered room %s</b></span><br>\n", 
		             time.wMonth, time.wDay, time.wYear, time.wHour, time.wMilliseconds, time.wSecond,
					 lpszRoom);
	LogMessage(dwID, strEnterRoom);
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

	PROOMDATA pRoomData = GetRoomData(dwID);
	if(pRoomData){
		CString csSTR;
		csSTR.Format("<span class=\"join\">%s has entered</span>", lpszUsername);
		LogMessage(dwID, csSTR);
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


	PROOMDATA pRoomData = GetRoomData(dwID);
	if(pRoomData){
		CString csSTR;
		strMessage.Replace("<", "&lt;");
		strMessage.Replace(">", "&gt;");
		strMessage.Replace(" ", "&nbsp;");
		strMessage.Replace("\r\n", "");
		strName.Replace("<", "&lt;");
		strName.Replace(">", "&gt;");
		strName.Replace(" ", "&nbsp;");
		strName.Replace("\r\n", "");
		csSTR.Format(bIsAction ? "<span class=\"emote\">%s %s</span>" : "<span class=\"name\">&lt;%s&gt;</span><span class=\"msg\"> %s</span>", strName, strMessage);
		LogMessage(dwID, csSTR);
		}
}

void OnOpMessage(DWORD dwID, CString strMessage)
{

	OnMessage(dwID, "OpMsg", strMessage, TRUE);
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

	PROOMDATA pRoomData = GetRoomData(dwID);
	if(pRoomData){
		CString csSTR;
		csSTR.Format("<span class=\"part\">%s has left</span>", lpszUsername);
		LogMessage(dwID, csSTR);
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
//  CString *pInput - Pointer to a string containing the input
////////////////////////////////////////////////////////////////////////
void OnInputHook(DWORD dwIP, CString *pInput)
{

	if(!pInput)return;

	PROOMDATA pRoomData = GetRoomData(dwIP);
	if(pRoomData){
		CString csCmd = *pInput;
		csCmd.MakeLower();
		
		if(csCmd.Find("/log enable") != -1){

			pRoomData->bLog = TRUE;
			*pInput = "";
			MessageBeep(0);
		}
		if(csCmd.Find("/log open") != -1){

			CString strFile;
			CString strName = pRoomData->szRoomName;
			strName.Remove('?');
			strName.Remove(':');
			strName.Remove(',');
			strName.Remove('\\');
			strName.Remove('/');
			strName.Remove('<');
			strName.Remove('>');
			strName.Remove('\"');
			strName.Remove('*');
			strName.Remove('|');
			strName.Replace(1, '-');
			strFile.Format("%s%s.html", g_strLogPath, strName);
			ShellExecute(NULL, "open", strFile, NULL, g_strLogPath, SW_SHOW);
			MessageBeep(0);
		}
		if(csCmd.Find("/log disable") != -1){

			pRoomData->bLog = FALSE;
			*pInput = "";
			MessageBeep(0);
		}
	}
}

void OnPrepareMenu (DWORD dwMenuID)
{

	if(dwMenuID == 1){

		if(g_cfgMenu != NULL){

			g_cfgMenu->DestroyMenu();
			delete g_cfgMenu;
			g_cfgMenu = 0;
		}
		g_cfgMenu = new CMenu;
		BOOL bReturn = TRUE;
		if(g_cfgMenu->CreatePopupMenu()){

			g_cfgMenu->AppendMenu(MF_STRING, 15102, "Open logfile directory");
			g_cfgMenu->AppendMenu(MF_SEPARATOR, 0, "");
			g_cfgMenu->AppendMenu(MF_STRING, 15100, "Configure Chatlog");
			g_cfgMenu->AppendMenu(MF_STRING, 15101, "Reload Configuration");
			g_Plugin.hCfgMenu = g_cfgMenu->m_hMenu;
		}

	}
	else if(dwMenuID == 2){

		if(g_chatMenu != NULL){

			g_chatMenu->DestroyMenu();
			delete g_chatMenu;
			g_chatMenu = 0;
		}
		g_chatMenu = new CMenu;
		BOOL bReturn = TRUE;
		if(g_chatMenu->CreatePopupMenu()){

			g_chatMenu->AppendMenu(MF_STRING, 15103, "View logfile");
			g_chatMenu->AppendMenu(MF_STRING, 15102, "Open logfile directory");
			g_chatMenu->AppendMenu(MF_SEPARATOR, 0, "");
			g_chatMenu->AppendMenu(MF_STRING, 15100, "Configure Chatlog");
			g_chatMenu->AppendMenu(MF_STRING, 15101, "Reload Configuration");
			g_Plugin.hChatMenu = g_chatMenu->m_hMenu;
		}
	}
}

void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName)
{

	if(nCmdID == 15100){

		ShellExecute(0, "open", g_strWd + "\\chatlog.ini", 0, 0, SW_SHOW);
	}
	else if(nCmdID == 15101){

		CIni ini;
		ini.SetIniFileName(g_strWd + "chatlog.ini");
		g_strLogPath = ini.GetValue("Chatlog", "Path", g_strWd + "Chatlogs");
		if(g_strLogPath.GetAt(g_strLogPath.GetLength()-1) != '\\'){
		
			g_strLogPath += "\\";
		}
	}
	else if(nCmdID == 15102){

		ShellExecute(0, "explore", g_strLogPath, 0, 0, SW_SHOW);
	}
	else if(nCmdID == 15103){

		PROOMDATA p = GetRoomData(dwChannelID);
		if(p){

			CString strFile;
			SYSTEMTIME time;
			GetLocalTime(&time);
			strFile.Format("%s%s_%04d%02d%02d.html", g_strLogPath, p->szRoomName, time.wYear, time.wMonth, time.wDay);
			ShellExecute(0, "open", strFile , 0, 0, SW_SHOW);
		}
	}

}


PROOMDATA GetRoomData(DWORD dwID)
{

	int iNum = m_RoomList.GetSize();

	for(int i = 0; i < iNum; i++){

		if(m_RoomList[i].dwRoomID == dwID){
			
			return &(m_RoomList[i]);		
		}
	}

	return NULL;
}

void AddRoomData(DWORD dwID, LPCTSTR lpszRoomName)
{

	CString csRealName = lpszRoomName;
	CString csRoomName;
	int iPos = csRealName.ReverseFind('_');

	if(iPos != -1){
		
		csRealName = csRealName.Left(iPos);
	}

	csRoomName = csRealName;
	
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

		lstrcpy(pRoomData->szRealName, csRealName);
		lstrcpy(pRoomData->szRoomName, csRoomName);
	}
	else{

		ROOMDATA RoomData;
		RoomData.dwRoomID = dwID;
		RoomData.bLog = TRUE;
		lstrcpy(RoomData.szRoomName, csRoomName);
		lstrcpy(RoomData.szRealName, csRealName);
		m_RoomList.Add(RoomData);
	}
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

void LogMessage(DWORD dwID, LPCTSTR lpszMessage)
{
	int iNum = m_RoomList.GetSize();
	for(int i=0 ; i < iNum ; i++){
		
		if((m_RoomList[i].dwRoomID == dwID) && m_RoomList[i].bLog){

			SYSTEMTIME time;
			GetLocalTime(&time);
			CString strDateString;
			strDateString.Format("%04d%02d%02d", time.wYear, time.wMonth, time.wDay);

			CStdioFile file;
			CString strName, strMessage;
			strName.Format("%s_%s.html", m_RoomList[i].szRoomName, strDateString);
			strName.Insert(0, g_strLogPath);

			CFileStatus fStatus;
			BOOL bWriteHeader = !CFile::GetStatus(strName, fStatus);

			strMessage.Format("<span class=\"time\">[%02d:%02d.%02d]</span> %s<br>\n", time.wHour, time.wMinute, time.wSecond, lpszMessage);
			TRY{

				if(!file.Open( strName , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText)) return;

				file.SeekToEnd();
				if(bWriteHeader){

					CString strHeader;
					strHeader.Format(
						"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
						"<html>\n"
						"<head>\n"
						"<title>Chatlog for room [%s]</title>\n"
						"<link rel=\"StyleSheet\" href=\"main.css\" title=\"Contemporary\">\n"
						"<style type=\"text/css\"></style>\n"
						"</head>\n"
						"<body>\n\n", m_RoomList[i].szRealName);
					file.WriteString(strHeader);
				}
				file.WriteString(strMessage);
				file.Flush();
				file.Close();
			}
			CATCH(CFileException, e){

			}END_CATCH;
		}
	}	
}


__declspec( dllexport ) PBPLUGIN GetPluginAPI(){

	return &g_Plugin;
}
