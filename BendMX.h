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


#if !defined(AFX_BENDMX_H__7D676FAE_7BAF_42F5_A887_EE5922B2D8DA__INCLUDED_)
#define AFX_BENDMX_H__7D676FAE_7BAF_42F5_A887_EE5922B2D8DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#ifdef __BENDMX__
#include "BendMXDlg.h"
#include "InputRequest.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CBendMXApp
// See BendMX.cpp for the implementation of this class
//
#define WRAPPER_EXPORT	extern __declspec(dllexport)

class CBendMXApp : public CWinApp
{
public:

	CBendMXApp();

#ifdef __BENDMX__
	CBendMXDlg *m_pBendMX;
	CInputRequest *m_pInput;
	void InputMessage(BOOL bDirectSend, CString strMessage, DWORD dwID);
	void RemoveHook(CString strChat);
	void InputMessage(CString strMessage, CString strChat = "", BOOL bEnter = TRUE);
	void ShowAdminWindow(CString strRoom);
	void SearchChatWnd(CString strChat);
	BOOL SendText(PSOCKETMONITOR pS, const CString strText);
	void InputServerInfo(DWORD dwID);
	void SendMessageTo(CString strTitle, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnMessage(CString strRoom, CString strName, CString strMessage);
	void OnEnter(CString strRoom, CString strUser, int nFiles);
	BOOL DoesRoomExist(CString &strRoom);
#endif

#ifdef __BENDMX__
	virtual int ExitInstance();
#endif
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBendMXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBendMXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

INT QueryModuleDebugLevel( const CString& strModule );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENDMX_H__7D676FAE_7BAF_42F5_A887_EE5922B2D8DA__INCLUDED_)
