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

#if !defined(AFX_CHATDLG_H__A6CC810B_77BB_44C3_8C87_132DF55335D6__INCLUDED_)
#define AFX_CHATDLG_H__A6CC810B_77BB_44C3_8C87_132DF55335D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatDlg.h : header file
//
#ifdef __BENDMX__

#include <winsock2.h>
#include "MyListCtrl.h"


#define SERVER_UNKNOWN      0x0000
#define SERVER_RCMS         0x0001
#define SERVER_WINMX353		0x0002
#define SERVER_RCMS353      0x0003
#define SERVER_MCMA			0x0004
#define SERVER_MCS			0x0005
#define SERVER_WCS			0x0006
#define SERVER_OUKA			0x0007
#define SERVER_ROSE			0x0008

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif


/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog
typedef enum TAG_INITSTATE{

	INIT_PENDING		 = 0,
	INIT_DONOTMONITOR    = 1,
	INIT_HANDSHAKE       = 2,
	INIT_ISCHATROOM      = 3,
	INIT_ISPRIMARY		 = 4
} INIT_STATE;


typedef struct TAG_SOCKETMONITOR
{

	SOCKET		socket;
	INIT_STATE  iState;
	DWORD       dwUPKey;
	DWORD       dwDWKey;
	DWORD       dwMXUPKey;
	DWORD       dwMXDWKey;
	WORD        wType;
	WORD        wLen;
	DWORD		dwOffset;
	CString     strUser;
	CString     strRoom;
	WORD        wServerType;
	CString     strServer;
	CString     strVersion;
	DWORD		dwID;
	BOOL		bHooked;
}SOCKETMONITOR, *PSOCKETMONITOR;

class ClientJoin
{
public:
	ClientJoin(){ dwNodeIP = 0; wNodePort = 0; wLine = 0; dwSrcIP = 0; dwFiles = 0; wLevel = 0; dwOldNodeIP = 0; wOldNodePort = 0;}

	CString strName;
	CString strSrcIP;
	DWORD	dwFiles;
	DWORD	dwNodeIP;
	DWORD	dwSrcIP;
	WORD	wLevel;
	WORD	wLine;
	WORD	wNodePort;
	CString strOldName;
	DWORD   dwOldNodeIP;
	WORD    wOldNodePort;
};

class CUser
{

public:
	CUser(){ dwNodeIP = 0; dwFiles = 0; wNodePort = 0; wUserRank = 0; wLine = 0;}

	CString strName;
	CString strIP;
	DWORD   dwFiles;
	DWORD   dwNodeIP;
	WORD    wNodePort;
	WORD    wUserRank;
	WORD    wLine;
};

class CChatDlg : public CDialog
{
// Construction
public:
	CChatDlg(CWnd* pParent = NULL);   // standard constructor

	// Methods
	void InputMessage(CString strMessage, BOOL bEnter = TRUE);
	void InputWelcome();
	void RemoveHook();
	static int SearchRCMSItem(HWND hwnd, CString strString);
	static int g_aRCMSCommandsNUM(HWND hWnd);

	// Subclassing functions
	static LRESULT CALLBACK SubEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL SubclassEdit(HWND hWnd);
	static BOOL CALLBACK FindEditControl(HWND hWnd, LPARAM lParam);
	
	static BOOL CALLBACK FindListControl(HWND hWnd, LPARAM lParam);
	BOOL SubclassList(HWND hWnd);
	static LRESULT CALLBACK SubListProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL SubclassChat(HWND hWnd);
	static LRESULT CALLBACK SubChatProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK FindChatControl(HWND hWnd, LPARAM lParam);

	BOOL SubclassDlg(HWND hWnd);
	static LRESULT CALLBACK SubDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_CHATDLG };
	CStatic	m_wndTrans;
	CSliderCtrl	m_slTrans;
	CMyListCtrl	m_lcUsers;
	BOOL	m_bTop;
	int		m_nTrans;
	CString	m_strTrans;
	//}}AFX_DATA
	CImageList m_iImageList;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	public:

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT OnGetRCMS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetID(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetUpTime(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnHide();
	afx_msg void OnTop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	// Static Help functions
	static BOOL CALLBACK FindWhois(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK FindWhoisNotClose(HWND hWnd, LPARAM lParam);
	static void AddHistory(CString strText);
	static int SearchHistory(CString strString);
	static int SearchItem(CString strString);
	static void ControlWinamp(CString strString);
	static CString GetLocalTime();
	static CString GetDate();
	static void ReplaceVars(CString& strOut);
	static CString GetWinampSong();
	static int CallOrigWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnAdminMenu(UINT nCmdID);
	afx_msg LRESULT OnAddUser(WPARAM wParam, LPARAM lParam);
	void RemoveUser(const CString strUser, const CString strIP, WORD wPort);
	void AddUser(CString strUsername, WORD wLine, DWORD dwFiles, DWORD dwNodeIP, WORD wNodePort, CString strIP, CString strHost, WORD wUserLevel);
	afx_msg LRESULT OnCopyIP(WPARAM wParam, LPARAM lParam);
	CString m_strTopic;
	CString m_strMotd;
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRoomRename(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetServerType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTopic(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJoin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedirect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenameMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIPClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CustomizeRCMS(WPARAM wParam, LPARAM lParam);
	void LoadRCMS();
	afx_msg LRESULT OnRCMSMenu(WPARAM wParam, LPARAM lParam);
	void WriteColoredMessage(CString &rUser, CString& rMsg, BOOL nAction = FALSE);
	afx_msg LRESULT OnInputMessage(WPARAM wParam, LPARAM lParam);
	BOOL FilterMessage(CString &strNick, CString &strMessage);
	static CString GetMySystemInfo(BOOL bRCMS = FALSE);
	static CString GetUpTime();
	static CString GetMyOSVersion();
	static CHARFORMAT& GetNickColor(CString& strNick);
	static CString FillStars(int nCount);
	static BOOL IsVideoPlaying();
	void WriteTime();
	BOOL m_bRCMS;
	// public attributes
	CString m_strRoomShort;
	CString m_strTitle;
	BOOL	m_bRepair;
	HWND	m_hEdit;
	HWND	m_hMX;
	HWND	m_hChat;
	HWND	m_hRich;
	HWND	m_hList;
	DWORD	m_dwID;
	BOOL	m_bIsHooked;
	DWORD	m_nStartTic;
	BOOL	m_bListBans;
	CStringArray m_aRCMSMenu;
	CMap<CString, LPCTSTR, CUser, CUser> m_mUsers;
	int     m_nServerType;
	
	BOOL SetTransparentPercentage(HWND hWnd, BYTE byPercentage);
	BOOL SetLayeredWindowAttributes(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	LONG AddLayeredStyle(HWND hWnd);

	typedef BOOL (WINAPI* lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
protected:
	// protected members
	//CString m_strNick;
	//CString m_strMsg;
};

#endif // #ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__A6CC810B_77BB_44C3_8C87_132DF55335D6__INCLUDED_)
