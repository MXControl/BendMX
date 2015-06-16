// AddReaction.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Configurator.h"
#include "AddReaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld AddReaction 


AddReaction::AddReaction(CWnd* pParent /*=NULL*/)
	: CDialog(AddReaction::IDD, pParent)
{
	//{{AFX_DATA_INIT(AddReaction)
	m_strReaction = _T("");
	//}}AFX_DATA_INIT
}


void AddReaction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddReaction)
	DDX_Text(pDX, IDC_REACTION_ADD, m_strReaction);
	DDV_MaxChars(pDX, m_strReaction, 400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddReaction, CDialog)
	//{{AFX_MSG_MAP(AddReaction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten AddReaction 

void AddReaction::OnOK() 
{

	if(UpdateData(TRUE)){

		if(!m_strReaction.IsEmpty()){

			CDialog::OnOK();
		}
	}
}
