// WelcomeMsg.cpp : implementation file
//

#include "stdafx.h"
#ifdef __BENDMX__

#include "BendMX.h"
#include "WelcomeMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcomeMsg dialog


CWelcomeMsg::CWelcomeMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CWelcomeMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcomeMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWelcomeMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomeMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcomeMsg, CDialog)
	//{{AFX_MSG_MAP(CWelcomeMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomeMsg message handlers

void CWelcomeMsg::Apply()
{

}

BOOL CWelcomeMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#endif
