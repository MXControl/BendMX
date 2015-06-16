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

#if !defined(AFX_KICKIT_H__C7EF334F_C502_400C_8747_7CC2408CD6FF__INCLUDED_)
#define AFX_KICKIT_H__C7EF334F_C502_400C_8747_7CC2408CD6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Kickit.h : header file
//

#include <Afxmt.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

/////////////////////////////////////////////////////////////////////////////
// CKickit dialog

class CKickit : public CDialog
{
// Construction
public:
	void InputMessage(CString strMsg);
	void Apply();
	void SaveAdmins();
	void LoadAdmins();
	void Log(CString strLog);
	void HandleCommand(CString strCmd);
	static UINT ClientThread(PVOID pParam);
	static UINT ServerThread(PVOID pParam);
	CKickit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKickit)
	enum { IDD = IDD_KICKIT_SERVER };
	CListBox	m_lbLog;
	CListCtrl	m_lcAdmins;
	BOOL	m_bRun;
	CString	m_strRoom;
	CString	m_strAddName;
	CString	m_strAddPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKickit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	SOCKET m_sServer;
	SOCKET m_sClient;
	//CWinThread* m_pServer;
	CEvent m_eServerDown;
	CStringArray m_aAdmins;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKickit)
	afx_msg void OnAdminAdd();
	afx_msg void OnAdminRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnableServer();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KICKIT_H__C7EF334F_C502_400C_8747_7CC2408CD6FF__INCLUDED_)
