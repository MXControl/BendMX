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

#if !defined(AFX_FILTERDLG_H__60522201_ADBD_4659_ABEB_21CFB005A478__INCLUDED_)
#define AFX_FILTERDLG_H__60522201_ADBD_4659_ABEB_21CFB005A478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog
#ifdef __BENDMX__

class CFilterDlg : public CDialog
{
// Construction
public:
	void SaveFilter();
	void LoadFilter();
	void SaveTrusted();
	void LoadTrusted();
	void Apply();
	CFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterDlg)
	enum { IDD = IDD_FILTER };
	CListBox	m_lbTrusted;
	CListBox	m_lbFilter;
	BOOL	m_bBlockHttp;
	BOOL	m_bBlockMotd;
	BOOL	m_bBlockPrivate;
	BOOL	m_bBlockTopic;
	CString	m_strFilterAdd;
	BOOL	m_bBlockEmpty;
	BOOL	m_bBlockAscii;
	BOOL	m_bBlockAdmin;
	BOOL	m_bFilterXtrem;
	BOOL	m_bFilterNormal;
	CString	m_strTrustedAdd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterDlg)
	afx_msg void OnTrustedAddBtn();
	afx_msg void OnTrustedRemBtn();
	afx_msg void OnFilterAddBtn();
	afx_msg void OnFilterRemBtn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //7#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDLG_H__60522201_ADBD_4659_ABEB_21CFB005A478__INCLUDED_)
