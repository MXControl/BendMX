// ConfiguratorDoc.cpp : Implementierung der Klasse CConfiguratorDoc
//

#include "stdafx.h"
#include "Configurator.h"

#include "ConfiguratorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorDoc

IMPLEMENT_DYNCREATE(CConfiguratorDoc, CDocument)

BEGIN_MESSAGE_MAP(CConfiguratorDoc, CDocument)
	//{{AFX_MSG_MAP(CConfiguratorDoc)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorDoc Konstruktion/Destruktion

CConfiguratorDoc::CConfiguratorDoc()
{
	// ZU ERLEDIGEN: Hier Code f�r One-Time-Konstruktion einf�gen

}

CConfiguratorDoc::~CConfiguratorDoc()
{
}

BOOL CConfiguratorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ZU ERLEDIGEN: Hier Code zur Reinitialisierung einf�gen
	// (SDI-Dokumente verwenden dieses Dokument)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CConfiguratorDoc Serialisierung

void CConfiguratorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einf�gen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einf�gen
	}
}

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorDoc Diagnose

#ifdef _DEBUG
void CConfiguratorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConfiguratorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorDoc Befehle
