// Configurator.h : Haupt-Header-Datei für die Anwendung CONFIGURATOR
//

#if !defined(AFX_CONFIGURATOR_H__07ED682E_4108_43A3_B1CA_B4C9C4FA2BB6__INCLUDED_)
#define AFX_CONFIGURATOR_H__07ED682E_4108_43A3_B1CA_B4C9C4FA2BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorApp:
// Siehe Configurator.cpp für die Implementierung dieser Klasse
//

class CConfiguratorApp : public CWinApp
{
public:
	CConfiguratorApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CConfiguratorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung
	//{{AFX_MSG(CConfiguratorApp)
	afx_msg void OnAppAbout();
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CONFIGURATOR_H__07ED682E_4108_43A3_B1CA_B4C9C4FA2BB6__INCLUDED_)
