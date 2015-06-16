#if !defined(AFX_EVENTEDITOR_H__6C4E83BE_F49C_4976_B5FE_EA42603E6242__INCLUDED_)
#define AFX_EVENTEDITOR_H__6C4E83BE_F49C_4976_B5FE_EA42603E6242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// eventeditor.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CEventEditor 
#include "PPToolTip.h"

class CEventEditor : public CDialog
{
// Konstruktion
public:
	void GetValues(int& nEvent, CString& strTrigger, CString& strReaction);
	void Init(int nEventType, CString strTrigger, CString strReaction);
	CEventEditor(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CEventEditor)
	enum { IDD = IDD_EVENT_EDITOR };
	CEdit	m_wndTrigger;
	CEdit	m_wndRaction;
	CComboBoxEx	m_cbEvent;
	CButton	m_wndBrowseBtn;
	int		m_nEvent;
	CString	m_strReaction;
	CString	m_strTrigger;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CEventEditor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CPPToolTip m_tooltip;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CEventEditor)
	afx_msg void OnBrowseSound();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EVENTEDITOR_H__6C4E83BE_F49C_4976_B5FE_EA42603E6242__INCLUDED_
