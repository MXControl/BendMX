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

#if !defined(AFX_MESSAGEDLG_H__E9898C1C_FA43_4D50_9EF0_6E5D9581576B__INCLUDED_)
#define AFX_MESSAGEDLG_H__E9898C1C_FA43_4D50_9EF0_6E5D9581576B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog
#ifdef __BENDMX__

class CMessageDlg : public CDialog
{
// Construction
public:
	void SaveQuickCmds();
	void LoadQuickCmds();
	void Apply();
	CMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageDlg)
	enum { IDD = IDD_MESSAGES };
	CListBox	m_lbQuick;
	CString	m_strAdd;
	CString	m_strWatch;
	CString	m_strListen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMsgAddBtn();
	afx_msg void OnQuickrem();
	afx_msg void OnQuickup();
	afx_msg void OnQuickdown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEDLG_H__E9898C1C_FA43_4D50_9EF0_6E5D9581576B__INCLUDED_)
