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

#if !defined(AFX_COLORDLG_H__FD250F86_49D0_481E_AC33_BC165405A038__INCLUDED_)
#define AFX_COLORDLG_H__FD250F86_49D0_481E_AC33_BC165405A038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog
#ifdef __BENDMX__

class CColorDlg : public CDialog
{
// Construction
public:
	void WriteHosts();
	void ReadHosts();
	void WriteOps();
	void ReadOps();
	void Apply();
	CColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLORS };
	CListBox	m_lbHostPre;
	CListBox	m_lbRegs;
	CString m_strHostPre;
	CString m_strRegPre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	afx_msg void OnOpAdd();
	afx_msg void OnOpRem();
	afx_msg void OnOpAdd2();
	afx_msg void OnOpRem2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__FD250F86_49D0_481E_AC33_BC165405A038__INCLUDED_)
