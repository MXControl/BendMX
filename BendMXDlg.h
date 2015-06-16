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

#if !defined(AFX_BENDMXDLG_H__C6602164_5237_45B7_B60B_FE774DEAF098__INCLUDED_)
#define AFX_BENDMXDLG_H__C6602164_5237_45B7_B60B_FE774DEAF098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BendMXDlg.h : header file
//

#ifdef __BENDMX__

#include "ExtListCtrl.h"
#include "GeneralDlg.h"
#include "ColorDlg.h"
#include "MessageDlg.h"
#include "FilterDlg.h"
#include "ChatDlg.h"
#include "AboutDlg.h"
#include "SimpleBot.h"
#include "WelcomeMsg.h"
/////////////////////////////////////////////////////////////////////////////
// CBendMXDlg dialog

class CBendMXDlg : public CDialog
{
// Construction
public:

	CBendMXDlg(CWnd* pParent = NULL);   // standard constructor
	
	
	static LRESULT CALLBACK SubWinMXProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL SubclassWinMX(HWND hWnd);

	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

	static BOOL CALLBACK FindChatWindows(HWND hWnd, LPARAM lParam);

	void LoadRCMS();
	BOOL DoesRoomExist(const CString strRoom);
	void InputMessage(CString strMessage, DWORD dwID);
	void InputMessage(CString strMessage, CString strChat = "", BOOL bEnter = TRUE);
	void AddChat(LPCSTR strChat, HWND hWnd);
	void RemoveHook(CString strChat);
	void Exit();
	LRESULT OnRepair(WPARAM wParam, LPARAM lParam);

// Dialog Data
	//{{AFX_DATA(CBendMXDlg)
	enum { IDD = IDD_BENDMX };
	CExtListCtrl	m_lcPrefs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBendMXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	afx_msg LRESULT SearchChatWnd(WPARAM wParam, LPARAM lParam);
	CString m_strSearch;
	static UINT WorkerThread(LPVOID pParam);
	//static UINT SearchProc(PVOID pParam);
	CSimpleBot  *m_pBot;
	int			m_nSel;
	BOOL		m_bIsHooked;
	BOOL		m_bRepair;
	BOOL		m_dwAwayTick;
	CString		m_strAway;
	UINT		m_nTick;
	//BOOL		m_bRun;
	CArray<CChatDlg*, CChatDlg*> m_aDlgs;
	CMap<CString, LPCTSTR, HWND, HWND> m_aDlgMap;
	CImageList	m_cImageList;
	HINSTANCE	m_hInstance;
	//CEvent		m_eDone;
protected:
	CGeneralDlg *m_pGeneral;
	CFilterDlg	*m_pFilter;
	CMessageDlg *m_pMessages;
	CAboutDlg   *m_pAbout;
	CColorDlg   *m_pColors;
	CWelcomeMsg *m_pWelcome;	
// Implementation
protected:

	afx_msg LRESULT OnShutdown(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAway(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChanged(int oldItem, int NewItem);

	// Generated message map functions
	//{{AFX_MSG(CBendMXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickPrefbar(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef __BENDMX__


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENDMXDLG_H__C6602164_5237_45B7_B60B_FE774DEAF098__INCLUDED_)
