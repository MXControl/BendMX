// ConfiguratorDoc.h : Schnittstelle der Klasse CConfiguratorDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGURATORDOC_H__8A3ABB05_8337_4765_B2EA_B51B275AB8D0__INCLUDED_)
#define AFX_CONFIGURATORDOC_H__8A3ABB05_8337_4765_B2EA_B51B275AB8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CConfiguratorDoc : public CDocument
{
protected: // Nur aus Serialisierung erzeugen
	CConfiguratorDoc();
	DECLARE_DYNCREATE(CConfiguratorDoc)

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CConfiguratorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CConfiguratorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CConfiguratorDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CONFIGURATORDOC_H__8A3ABB05_8337_4765_B2EA_B51B275AB8D0__INCLUDED_)
