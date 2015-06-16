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

#if !defined(AFX_PLUGIN_H__B421BA2D_264A_4108_A025_FF9FE6BFA4CC__INCLUDED_)
#define AFX_PLUGIN_H__B421BA2D_264A_4108_A025_FF9FE6BFA4CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __BENDMX__

#include "MXPlugin.h"

class CPlugin  
{
public:
	CPlugin(CString strPluginName);
	virtual ~CPlugin();

	BOOL Init();
	void DeInit();
	void OnEnterChannel(DWORD dwID, LPCTSTR lpszRoom);
	void OnEnterUser(DWORD dwID, LPCTSTR lpszUsername);
	void OnMessage(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction);
	void OnLeaveUser(DWORD dwID, LPCTSTR lpszUsername);
	void OnCloseChannel(DWORD dwID);
	void OnInputHook(DWORD dwIP, CString *pInput);
	void OnOpMessage(DWORD dwIP, CString strMsg);
	void OnMenuCommand(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);
	static BOOL InputMessage(DWORD dwID, LPCTSTR lpszMessage);

	HINSTANCE m_hDll;
	BPLUGIN*  m_pPlugin;
	GETPLUGINAPI m_pGetPluginAPI;
	CString   m_strPluginName;
	CString   m_strDescription;
	CString   m_strAuthor;
	CString   m_strShortName;

};

#endif //#ifdef __BENDMX__

#endif // !defined(AFX_PLUGIN_H__B421BA2D_264A_4108_A025_FF9FE6BFA4CC__INCLUDED_)
