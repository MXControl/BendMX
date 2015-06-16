#if !defined(AFX_ADDREACTION_H__FFA06337_7CF3_4925_B1B0_86F5DE32A029__INCLUDED_)
#define AFX_ADDREACTION_H__FFA06337_7CF3_4925_B1B0_86F5DE32A029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddReaction.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld AddReaction 

class AddReaction : public CDialog
{
// Konstruktion
public:
	AddReaction(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(AddReaction)
	enum { IDD = IDD_ADD_REACTION };
	CString	m_strReaction;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(AddReaction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(AddReaction)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ADDREACTION_H__FFA06337_7CF3_4925_B1B0_86F5DE32A029__INCLUDED_
