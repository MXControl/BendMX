// ConfiguratorView.cpp : Implementierung der Klasse CConfiguratorView
//

#include "stdafx.h"
#include "Configurator.h"

#include "ConfiguratorDoc.h"
#include "ConfiguratorView.h"
#include "EventEditor.h"
#include "BotSetup.h"
#include "RandCmd.h"
#include <mmsystem.h>
#include "..\Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorView

IMPLEMENT_DYNCREATE(CConfiguratorView, CFormView)

BEGIN_MESSAGE_MAP(CConfiguratorView, CFormView)
	//{{AFX_MSG_MAP(CConfiguratorView)
	ON_WM_SIZE()
	ON_COMMAND(ID_POPUP_DELETE, OnPopupDelete)
	ON_NOTIFY(NM_RCLICK, IDC_EVENTS, OnRclickEvents)
	ON_COMMAND(ID_POPUP_EDITEVENT, OnPopupEditevent)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_EVENTS, OnDblclkEvents)
	ON_COMMAND(ID_POPUP_ADDSOUND, OnPopupAddsound)
	ON_COMMAND(ID_POPUP_PLAYSOUND, OnPopupPlaysound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorView Nachrichten-Handler

void CConfiguratorView::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	//{{AFX_DATA_MAP(CConfiguratorView)
	DDX_Control(pDX, IDC_EVENTS, m_lbEvents);
	//}}AFX_DATA_MAP
	CFormView::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorView Konstruktion/Destruktion

CConfiguratorView::CConfiguratorView()
:CFormView(CConfiguratorView::IDD)
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,
	//{{AFX_DATA_INIT(CConfiguratorView)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;


}

CConfiguratorView::~CConfiguratorView()
{
}

BOOL CConfiguratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorView Zeichnen

void CConfiguratorView::OnDraw(CDC* pDC)
{
	CConfiguratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen
}

/////////////////////////////////////////////////////////////////////////////
// CConfiguratorView Diagnose

#ifdef _DEBUG
void CConfiguratorView::AssertValid() const
{
 	CFormView::AssertValid();
}

void CConfiguratorView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CConfiguratorDoc* CConfiguratorView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CConfiguratorDoc)));
	return (CConfiguratorDoc*)m_pDocument;
}
#endif //_DEBUG


void CConfiguratorView::ParseConfig()
{

	CString strIniFile = m_strIni; //m_strWd + "\\MXSound.ini";

	CStdioFile ini;
	CString strBuffer;
	int nMode = -1;
	
	m_aBadWords.RemoveAll();
	ClearRandom();

	PCOMMAND cmd;
	CString strSection;
	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText);

		while(ini.ReadString(strBuffer)){

			strBuffer.TrimLeft();
			strBuffer.TrimRight();
			//strBuffer.MakeUpper();

			if(strBuffer.Left(2) == "//") continue;
			if(strBuffer.Find("[Sounds]", 0) == 0){
				
				nMode = 0;
				continue;
			}
			if(strBuffer.Left(1) == "#"){
				
				strBuffer = strBuffer.Mid(1);
				if(strBuffer.IsEmpty() || (nMode != 0)){

					continue;
				}

				CString strEvent, strResp;
				
				CTokenizer token(strBuffer, "¨");

				if(!token.Next(strEvent) || !token.Next(strResp)) continue;
					
				switch(nMode){
				
				case 0:
					
					strEvent.MakeLower();
					if(strResp.GetLength() < 8){

						strResp = m_strWd + "\\SoundFX\\" + strResp;
					}
					else if(strResp.GetAt(1) != ':'){

						strResp = m_strWd + "\\SoundFX\\" + strResp;
					}
					cmd = new COMMAND;
					cmd->strTrigger = strEvent;
					cmd->strResponse = strResp;
					m_aSounds.Add(cmd);
					m_lbEvents.InsertItem(m_lbEvents.GetItemCount(), cmd->strTrigger, 0);
					m_lbEvents.SetItemText(m_lbEvents.GetItemCount() -1, 1, cmd->strResponse);
					break;
				default:
					TRACE("HELP\n");

				}

			}
		}
		ini.Close();
	}
	CATCH(CFileException, e){

		AfxMessageBox("Error during file operation!", MB_OK+MB_ICONSTOP);

	}END_CATCH;

}

void CConfiguratorView::OnInitialUpdate() 
{

	CFormView::OnInitialUpdate();
	

	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	m_strWd.Format("%s", szBuffer);
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));

	ListView_SetExtendedListViewStyle(m_lbEvents.m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lbEvents.InsertColumn(0, "Event", LVCFMT_LEFT, 200);
	m_lbEvents.InsertColumn(1, "Reaction", LVCFMT_LEFT, 600);

	// Create imaglist and attach it to the tracklist
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 5, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_EVENTS);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();

	m_lbEvents.SetImageList(&m_cImageList, LVSIL_SMALL);
	
	m_strIni = m_strWd + "\\MXSound.ini";
	ParseConfig();
	//LoadSettings();
	GetDocument()->SetTitle(m_strIni);

	UpdateData(FALSE);
}

void CConfiguratorView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	

	if (!m_lbEvents.m_hWnd){

		return;
	}

	CRect	rcList;
	CRect	rcParent;

	GetClientRect(rcParent);

	// Get rect of eventlist
	m_lbEvents.GetWindowRect(rcList);
	ScreenToClient(rcList);
	// margin...
	rcList.right = rcParent.right - 5;
	rcList.bottom = rcParent.bottom - 5;
	// adjust the window size
	m_lbEvents.MoveWindow(rcList);
}

void CConfiguratorView::LoadSettings()
{

	m_Ini.SetIniFileName(m_strIni);
	
}

void CConfiguratorView::SaveSettings()
{

	if(UpdateData(TRUE)){

		m_Ini.SetIniFileName(m_strIni);
		
	}
}

//DEL void CConfiguratorView::OnPopupAddevent() 
//DEL {
//DEL 
//DEL 	LVITEM iItem = {0};
//DEL 	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
//DEL 	iItem.iItem = m_lbEvents.GetItemCount();
//DEL 
//DEL 	CEventEditor dlg;
//DEL 	dlg.Init(1, "Trigger", "*", "Reaction", FALSE);
//DEL 	if(dlg.DoModal() == IDOK){
//DEL 		
//DEL 		int nEvent;
//DEL 		CString strTrigger, strReaction, strUser;
//DEL 		BOOL bRegEx;
//DEL 		dlg.GetValues(nEvent, strTrigger, strUser, strReaction, bRegEx);
//DEL 		iItem.mask = LVIF_TEXT | LVIF_IMAGE;
//DEL 		iItem.pszText = (LPTSTR)(LPCSTR)strTrigger;
//DEL 		iItem.iImage = nEvent;
//DEL 		m_lbEvents.InsertItem(&iItem);
//DEL 		m_lbEvents.SetItemText(iItem.iItem, 1, strUser);
//DEL 		m_lbEvents.SetItemText(iItem.iItem, 2, bRegEx ? "TRUE" : "FALSE");
//DEL 		m_lbEvents.SetItemText(iItem.iItem, 3, strReaction);
//DEL 		m_bModified = TRUE;
//DEL 	}
//DEL }


void CConfiguratorView::OnPopupAddsound() 
{
	
	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lbEvents.GetItemCount();

	CEventEditor dlg;
	dlg.Init(0, "Trigger", "sound.wav");
	if(dlg.DoModal() == IDOK){
		
		int nEvent;
		CString strTrigger, strReaction, strUser;
		dlg.GetValues(nEvent, strTrigger, strReaction);
		iItem.mask = LVIF_TEXT | LVIF_IMAGE;
		iItem.pszText = (LPTSTR)(LPCSTR)strTrigger;
		iItem.iImage = nEvent;
		m_lbEvents.InsertItem(&iItem);
		m_lbEvents.SetItemText(iItem.iItem, 1, strReaction);
		m_bModified = TRUE;
	}
}


void CConfiguratorView::OnPopupPlaysound() 
{

	
	if(m_lbEvents.GetSelectedCount() == 0){

		AfxMessageBox("Please select Event to play first!", MB_OK+MB_ICONINFORMATION);
		return;
	}

	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lbEvents.GetSelectionMark();
	m_lbEvents.GetItem(&iItem);
	if(!PlaySound(m_lbEvents.GetItemText(iItem.iItem, 1), NULL, SND_FILENAME|SND_ASYNC)){

		AfxMessageBox("Could not play file " + m_lbEvents.GetItemText(iItem.iItem, 1), MB_ICONSTOP);
	}
}


void CConfiguratorView::OnPopupEditevent() 
{

	if(m_lbEvents.GetSelectedCount() == 0){

		AfxMessageBox("Please select Event to edit first!", MB_OK+MB_ICONINFORMATION);
		return;
	}

	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lbEvents.GetSelectionMark();
	m_lbEvents.GetItem(&iItem);

	CEventEditor dlg;
	dlg.Init(
			   iItem.iImage, 
			   m_lbEvents.GetItemText(iItem.iItem, 0), 
			   m_lbEvents.GetItemText(iItem.iItem, 1)
		   );

	if(dlg.DoModal() == IDOK){
		
		int nEvent;
		CString strTrigger, strReaction;
		dlg.GetValues(nEvent, strTrigger, strReaction);
		iItem.mask = LVIF_TEXT | LVIF_IMAGE;
		iItem.pszText = (LPTSTR)(LPCSTR)strTrigger;
		iItem.iImage = nEvent;
		m_lbEvents.SetItem(&iItem);
		m_lbEvents.SetItemText(iItem.iItem, 1, strReaction);
		m_bModified = TRUE;
	}
}

void CConfiguratorView::OnPopupDelete() 
{

	if(m_lbEvents.GetSelectedCount() == 0){

		AfxMessageBox("Please select Event to delete first!", MB_OK+MB_ICONINFORMATION);
		return;
	}
	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lbEvents.GetSelectionMark();
	m_lbEvents.GetItem(&iItem);

	m_lbEvents.DeleteItem(m_lbEvents.GetSelectionMark());

	m_bModified = TRUE;
}


void CConfiguratorView::OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult) 
{

	OnPopupEditevent();
	*pResult = 0;
}

void CConfiguratorView::OnRclickEvents(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu mContextMenu;
	mContextMenu.LoadMenu(IDR_POPUP);

	POINT point;
	GetCursorPos(&point);

	if(m_lbEvents.GetNextItem(-1, LVNI_FOCUSED) >= 0)
	{

		mContextMenu.GetSubMenu(0)->TrackPopupMenu(	TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd() );
	}

	*pResult = 0;
}


void CConfiguratorView::WriteConfig()
{

	m_aBadWords.RemoveAll();
	ClearRandom(FALSE);	
	
	PCOMMAND cmd;
	for(int i = 0; i < m_lbEvents.GetItemCount(); i++){

		LVITEM iItem = {0};
		iItem.mask = LVIF_IMAGE;
		iItem.iItem = i;
		m_lbEvents.GetItem(&iItem);
		switch(iItem.iImage){

		case 0://Sound
			cmd = new COMMAND;
			cmd->strTrigger = m_lbEvents.GetItemText(i, 0);
			cmd->strResponse = m_lbEvents.GetItemText(i, 1);
			m_aSounds.Add(cmd);
			break;
		default:
			ASSERT(FALSE);
		}
	}

	CStdioFile ini;
	CString strLine;
	ASSERT(!m_strIni.IsEmpty());
	TRY{

		ini.Open(m_strIni, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
		ini.SetLength(0);
		
		ini.WriteString("// MXSound.ini. Automatically generated by Configurator,exe\n");
		ini.WriteString("// Do not edit manually if you dont know what you are doing\n");
		
		ini.WriteString("\n\n[Sounds]\n");
		for(i = 0; i < m_aSounds.GetSize(); i++){
			
			cmd = m_aSounds.GetAt(i);
			strLine.Format("#%s¨%s\n", cmd->strTrigger, cmd->strResponse);
			ini.WriteString(strLine);
		}
		ini.WriteString("//Sounds End\n\n");
		ini.Flush();
		ini.Close();
	}
	CATCH(CFileException, e){

		AfxMessageBox("Error during file operation!", MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CConfiguratorView::OnFileOpen() 
{

	if(m_bModified){

		if(AfxMessageBox("Save changes to current configurations?", MB_YESNO+MB_ICONQUESTION) == IDYES){

			OnFileSave();
		}
	}
	static char BASED_CODE szFilter[] = "Configuration Files (*.ini)|*.ini|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, ".ini", NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, szFilter, this);
	
	if(dlg.DoModal() == IDOK){

		m_strIni = "";
		m_aBadWords.RemoveAll();
		ClearRandom();
		m_lbEvents.DeleteAllItems();
		UpdateData(FALSE);
		
		m_strIni = dlg.GetPathName();
		LoadSettings();
		ParseConfig();
		GetDocument()->SetTitle(m_strIni);
		UpdateData(FALSE);
	}
	
}

void CConfiguratorView::OnFileNew() 
{

	if(m_bModified){

		if(AfxMessageBox("Save changes to current configurations?", MB_YESNO+MB_ICONQUESTION) == IDYES){

			OnFileSave();
		}
	}
	m_strIni = "";
	ClearRandom();
	GetDocument()->SetTitle("Untitled.ini");
	m_lbEvents.DeleteAllItems();
	UpdateData(FALSE);
}

void CConfiguratorView::OnFileSave() 
{

	if(m_strIni.IsEmpty()){
	
		OnFileSaveAs();
		return;
	}

	WriteConfig();
	SaveSettings();
	m_bModified = FALSE;
}

void CConfiguratorView::OnFileSaveAs() 
{
	
	static char BASED_CODE szFilter[] = "Configuration Files (*.ini)|*.ini|All Files (*.*)|*.*||";

	CFileDialog dlg(FALSE, ".ini", m_strWd + "\\MXSound.ini", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	
	if(dlg.DoModal() == IDOK){
	
		m_strIni = dlg.GetPathName();
		WriteConfig();
		SaveSettings();
		GetDocument()->SetTitle(m_strIni);
		m_bModified = FALSE;
	}
}

void CConfiguratorView::OnDestroy() 
{

	CFormView::OnDestroy();
	
	if(m_bModified){

		if(AfxMessageBox("Save changes to current configurations?", MB_YESNO+MB_ICONQUESTION) == IDYES){

			OnFileSave();
		}
	}
	m_aBadWords.RemoveAll();
	ClearRandom();
}

void CConfiguratorView::ClearRandom(BOOL bAll)
{

	int j = 0;
	while((j = m_aSounds.GetSize()) > 0){

		PCOMMAND c = m_aSounds.GetAt(j-1);
		m_aSounds.RemoveAt(j-1);
		c->strResponse.Empty();
		c->strTrigger.Empty();
		delete c;
		c = NULL;
	}
}
