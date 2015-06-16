#if !defined(AFX_BOTSETUP_H__746D447B_98E6_4BE9_9AC2_2AE4031F0AE8__INCLUDED_)
#define AFX_BOTSETUP_H__746D447B_98E6_4BE9_9AC2_2AE4031F0AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BotSetup.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBotSetup 
#include "PPToolTip.h"

class CBotSetup : public CDialog
{
// Konstruktion
public:
	CBotSetup(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CBotSetup)
	enum { IDD = IDD_BOTSETTINGS };
	BOOL	m_bBadLan;
	BOOL	m_bBot;
	BOOL	m_bExtended;
	CString	m_strExtended;
	int		m_nFlood;
	BOOL	m_bKick;
	int		m_nWarnings;
	BOOL	m_bLog;
	CString	m_strMessage;
	CString	m_strMotd;
	CString	m_strNewLine;
	CString	m_strNickname;
	CString	m_strNoPenalty;
	BOOL	m_bProtect;
	BOOL	m_bSound;
	BOOL	m_bRCMS;
	int		m_nBlockMessageCount;
	int		m_nBlockTime;
	int		m_nBlockRange;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CBotSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CPPToolTip m_tooltip;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CBotSetup)
	virtual void OnOK();
	afx_msg void OnUsercms();
	afx_msg void OnKick();
	afx_msg void OnBadlan();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_BOTSETUP_H__746D447B_98E6_4BE9_9AC2_2AE4031F0AE8__INCLUDED_
