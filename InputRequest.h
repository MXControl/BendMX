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

#if !defined(AFX_INPUTREQUEST_H__BF37B7B0_7DDD_4B6F_B59A_132B9AFD20A1__INCLUDED_)
#define AFX_INPUTREQUEST_H__BF37B7B0_7DDD_4B6F_B59A_132B9AFD20A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRequest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRequest dialog
#ifdef __BENDMX__

class CInputRequest : public CDialog
{
// Construction
public:
	int m_nMode;
	DWORD m_dwID;
	CInputRequest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputRequest)
	enum { IDD = IDD_UTIL };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRequest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	afx_msg LRESULT OnGetOut(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetID(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsg(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(CInputRequest)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREQUEST_H__BF37B7B0_7DDD_4B6F_B59A_132B9AFD20A1__INCLUDED_)
