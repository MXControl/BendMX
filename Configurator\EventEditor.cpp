// eventeditor.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Configurator.h"
#include "eventeditor.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CEventEditor 


CEventEditor::CEventEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CEventEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventEditor)
	m_nEvent = -1;
	m_strReaction = _T("");
	m_strTrigger = _T("");
	//}}AFX_DATA_INIT
}


void CEventEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventEditor)
	DDX_Control(pDX, IDC_TRIGGER, m_wndTrigger);
	DDX_Control(pDX, IDC_REACTION, m_wndRaction);
	DDX_Control(pDX, IDC_BROWSE_SOUND, m_wndBrowseBtn);
	DDX_Text(pDX, IDC_REACTION, m_strReaction);
	DDX_Text(pDX, IDC_TRIGGER, m_strTrigger);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventEditor, CDialog)
	//{{AFX_MSG_MAP(CEventEditor)
	ON_BN_CLICKED(IDC_BROWSE_SOUND, OnBrowseSound)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEventEditor 

void CEventEditor::OnBrowseSound() 
{
	
	UpdateData(TRUE);

	static char BASED_CODE szFilter[] = "Wave Audio Files (*.wav)|*.wav|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, ".wav", NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, szFilter, this);
	
	if(dlg.DoModal() == IDOK){

		m_strReaction = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CEventEditor::Init(int nEventType, CString strTrigger, CString strReaction)
{

	m_nEvent = nEventType;
	m_strTrigger = strTrigger;
	m_strReaction = strReaction;
}

BOOL CEventEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_tooltip.Create(this);
	m_tooltip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW | PPTOOLTIP_TRACKING_MOUSE | PPTOOLTIP_CLOSE_LEAVEWND, -1);

	m_tooltip.AddTool(GetDlgItem(IDC_TRIGGER), "Word(s) that trigger the sound event.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_REACTION), "Filename of sound to be played (must be a valid Wave Audio file).", IDI_TIP);

	UpdateData(FALSE);
	
	m_wndTrigger.SetFocus();
	m_wndTrigger.SetSel(0, -1);
	
	return TRUE;
}

void CEventEditor::GetValues(int &nEvent, CString &strTrigger, CString &strReaction)
{

	nEvent = m_nEvent;
	strTrigger = m_strTrigger;
	strReaction = m_strReaction;
}

void CEventEditor::OnOK() 
{

	if(UpdateData(TRUE)){
		
		CDialog::OnOK();
	}
}

BOOL CEventEditor::PreTranslateMessage(MSG* pMsg) 
{
	
	m_tooltip.RelayEvent(pMsg); 	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEventEditor::OnPlay() 
{

	UpdateData(TRUE);

	PlaySound(m_strReaction, NULL, SND_FILENAME|SND_ASYNC);	
}
