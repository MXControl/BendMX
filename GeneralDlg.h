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

#if !defined(AFX_GENERALDLG_H__F1FE92CF_43EA_4CEA_8855_1E287CCA6C22__INCLUDED_)
#define AFX_GENERALDLG_H__F1FE92CF_43EA_4CEA_8855_1E287CCA6C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg dialog
#ifdef __BENDMX__

class CGeneralDlg : public CDialog
{
// Construction
public:
	void UnloadPlugins();
	void LoadPlugins();
	void Apply();
	CGeneralDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneralDlg)
	enum { IDD = IDD_GENERAL };
	CListCtrl	m_lcPlugins;
	CComboBox	m_cbLang;
	CString	m_strLanguage;
	int		m_nHistory;
	BOOL	m_bHistory;
	BOOL	m_bAllChannels;
	BOOL	m_bNickAlert;
	BOOL	m_bSoundFx;
	int		m_nRes;
	BOOL	m_bMoronMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralDlg)
	afx_msg void OnRepair();
	afx_msg void OnFonthelp();
	afx_msg void OnRemove();
	afx_msg void OnReloadAll();
	afx_msg void OnUnloadAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokLang();
	afx_msg void OnMoronmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALDLG_H__F1FE92CF_43EA_4CEA_8855_1E287CCA6C22__INCLUDED_)
