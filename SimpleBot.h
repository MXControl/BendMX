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

#if !defined(AFX_SIMPLEBOT_H__75DAD194_406C_4F64_B355_975D5587BA91__INCLUDED_)
#define AFX_SIMPLEBOT_H__75DAD194_406C_4F64_B355_975D5587BA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimpleBot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimpleBot dialog
#ifdef __BENDMX__

#include <queue>
#include <Afxmt.h>

typedef struct{

	CString strIn;
	CString strOut;
}BOT_MESSAGE, *PBOT_MESSAGE;

class CSimpleBot : public CDialog
{
// Construction
public:
	DWORD m_nLastPost;
	int   m_nProtect;
	void InputMessage(CString strMessage);
	static void ReplaceVars(CString &strOut, CString strName = "");
	CString m_strLast;
	CEvent m_eBotDone;
	void OnEnter(CString strName, int nFiles);
	void OnMessage(CString strName, CString strMessage);
	static UINT BotThread(PVOID pParam);
	void SaveBot();
	void LoadBot();
	void Apply();
	CSimpleBot(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSimpleBot)
	enum { IDD = IDD_SIMPLEBOT };
	CListCtrl	m_lcCommands;
	CString	m_strAnnounce;
	CString	m_strRoom;
	BOOL	m_bBot;
	int		m_nTimer;
	BOOL	m_bLeechKick;
	BOOL	m_bLimit;
	CString	m_strResponse;
	CString	m_strTrigger;
	CString	m_strWelcome;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleBot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<BOT_MESSAGE, BOT_MESSAGE> m_aBot;
	std::queue<BOT_MESSAGE> m_qInQueue;
	// Generated message map functions
	//{{AFX_MSG(CSimpleBot)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddtrigger();
	afx_msg void OnRemovecmd();
	afx_msg void OnEnablebot();
	afx_msg void OnEditcmd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEBOT_H__75DAD194_406C_4F64_B355_975D5587BA91__INCLUDED_)
