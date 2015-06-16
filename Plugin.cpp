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
#ifdef __BENDMX__

#include "BendMX.h"
#include "Plugin.h"
#include "Logging\Logging.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

INITTRACE("Plugin");

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
extern CBendMXApp theApp;


CPlugin::CPlugin(CString strPluginName)
{

	m_pPlugin		= NULL;
	m_hDll			= NULL;
	m_pGetPluginAPI	= NULL;
	m_strPluginName = strPluginName;
}

CPlugin::~CPlugin()
{

}

BOOL CPlugin::Init()
{

	ENTRY_TRACE("CPlugin::Init()");
	if(m_hDll != NULL){
		
		FreeLibrary(m_hDll);
	}

	m_hDll = LoadLibrary(m_strPluginName);

	if(m_hDll == NULL){

		LTRACE(_T("CPlugin::Init(): Error laoding library %s"), m_strPluginName);
		return 0;
	}

	m_pGetPluginAPI = (GETPLUGINAPI)GetProcAddress(m_hDll, "GetPluginAPI");

	if(m_pGetPluginAPI == NULL){

		LTRACE(_T("CPlugin::Init(): Error getting ProcAddress"));
		return -1;
	}

	m_pPlugin = m_pGetPluginAPI();

	if(m_pPlugin == NULL){

		LTRACE(_T("CPlugin::Init(): Error getting interface"));
		return -2;
	}

	if(m_pPlugin->nVersion != VERSION){

		LTRACE(_T("CPlugin::Init(): Wrong API version"));
		return -3;
	}

	m_pPlugin->hDllInstance  = m_hDll;
	m_pPlugin->InputMessage  = InputMessage;
	m_pPlugin->Init();

	m_strDescription = m_pPlugin->cDescription;
	m_strAuthor = m_pPlugin->cAuthor;
	m_strShortName = m_strPluginName.Mid(m_strPluginName.ReverseFind('\\')+1);
	m_strShortName.Replace(".b4x", "");

	EXIT_TRACE("CPlugin::Init()");
	return TRUE;
}

BOOL CPlugin::InputMessage(DWORD dwID, LPCTSTR lpszMessage)
{

	theApp.InputMessage(FALSE, lpszMessage, dwID);
	return TRUE;
}

void CPlugin::DeInit()
{

	if(m_hDll != NULL){

		m_pPlugin->Quit();
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}


void CPlugin::OnEnterChannel(DWORD dwID, LPCTSTR lpszRoom)
{

	m_pPlugin->OnEnterChannel(dwID, lpszRoom);
}

void CPlugin::OnEnterUser(DWORD dwID, LPCTSTR lpszUsername)
{

	m_pPlugin->OnEnterUser(dwID, lpszUsername);
}

void CPlugin::OnMessage(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction)
{

	m_pPlugin->OnMessage(dwID, strName, strMessage, bIsAction);
}

void CPlugin::OnLeaveUser(DWORD dwID, LPCTSTR lpszUsername)
{

	m_pPlugin->OnLeaveUser(dwID, lpszUsername);
}

void CPlugin::OnCloseChannel(DWORD dwID)
{

	m_pPlugin->OnCloseChannel(dwID);
}

void CPlugin::OnInputHook(DWORD dwIP, CString* pInput)
{

	m_pPlugin->OnInputHook(dwIP, pInput);
}


void CPlugin::OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName)
{

	//TRACE("In Call OnMenuCommand of Plugin %s (%d %d %s)\n", m_strPluginName, nCmdID, dwChannelID, lpszName);
	m_pPlugin->OnMenuCommand(nCmdID, dwChannelID, lpszName);
}

void CPlugin::OnOpMessage(DWORD dwID, CString strMsg)
{

	m_pPlugin->OnOpMessage(dwID, strMsg);
}

#endif // #ifdef __BENDMX__
