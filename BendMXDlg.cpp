// BendMXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BendMX.h"
#ifdef __BENDMX__

#include "BendMXDlg.h"
#include "Settings.h"
#include "I18n.h"
#include "Plugin.h"
#include "Logging\Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "winmm.lib.")

INITTRACE("BendMXDlg");

/////////////////////////////////////////////////////////////////////////////
// CBendMXDlg dialog

#define TIMER_ID_STARTUP   1121
#define TIMER_ID_SEARCH    1122

extern CSettings g_sSettings;

HWND         g_hMXWindow;
CStringArray g_strAuto;
CStringArray g_strHistory;
CStringArray g_aRCMSCommands;
CStringArray g_aWinMXCommands;
CStringArray g_aRoseCommands;
CStringArray g_aMXToolsCommands;
CStringArray g_strRooms;
CStringArray g_strOps;
CStringArray g_strHosts;
CStringArray g_strTrusted;
CStringArray g_strFilter;
DWORD 		 g_nMXStartTic = 0;
CString      g_strIdent;
extern CString		g_strMyName;
extern CString		g_strVersion;

CI18n					   g_i18n;
CArray<CPlugin*, CPlugin*> g_Plugins;

BOOL g_bAway = FALSE;

UINT UWM_SHUTDOWN	= ::RegisterWindowMessage("UWM_SHUTDOWN-B5EAC805-7C0E-4C7B-860E-C08465B6ADE5");
UINT UWM_REPAIR		= ::RegisterWindowMessage("UWM_REPAIR-B5EAC805-7C0E-4C7B-860E-C08465B6ADE5");
UINT UWM_AWAY		= ::RegisterWindowMessage("UWM_AWAY_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_ADDCHAT    = ::RegisterWindowMessage("UWM_ADDCHAT_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
extern UINT UWM_GETOUT;

HINSTANCE g_hUserDll = 0;

CBendMXDlg::CBendMXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBendMXDlg::IDD, pParent)
{

	ENTRY_TRACE("CBendMXDlg::CBendMXDlg()");
	//{{AFX_DATA_INIT(CBendMXDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwAwayTick = 0;
	g_hMXWindow  = NULL;
	m_bIsHooked = FALSE;
	m_bRepair   = FALSE;
	m_nTick     = 0;
	EXIT_TRACE("CBendMXDlg::CBendMXDlg()");
	g_hUserDll = ::LoadLibrary("USER32.dll");
}


void CBendMXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBendMXDlg)
	DDX_Control(pDX, IDC_PREFBAR, m_lcPrefs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBendMXDlg, CDialog)
	//{{AFX_MSG_MAP(CBendMXDlg)
	ON_NOTIFY(NM_CLICK, IDC_PREFBAR, OnClickPrefbar)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_SHUTDOWN, OnShutdown)
	ON_REGISTERED_MESSAGE(UWM_REPAIR, OnRepair)
	ON_REGISTERED_MESSAGE(UWM_AWAY, OnAway)
	ON_REGISTERED_MESSAGE(UWM_ADDCHAT, SearchChatWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBendMXDlg message handlers

BOOL CBendMXDlg::OnInitDialog() 
{
	
	ENTRY_TRACE("CBendMXDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	
	g_strIdent.Format(" [%X]", m_hWnd);

	g_i18n.LoadLanguage(g_sSettings.GetLanguage());
	g_i18n.TranslateDialog(this, IDD_BENDMX);

	HIMAGELIST hList = ImageList_Create(32,32, ILC_COLOR8 |ILC_MASK , 5, 1);
	m_cImageList.Attach(hList);

	AfxSetResourceHandle(m_hInstance);
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_PREF);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();
	m_lcPrefs.Init(FALSE);

	m_lcPrefs.SetImageList(&m_cImageList, LVSIL_NORMAL);
	m_lcPrefs.SetIconSpacing(48,32);
	m_lcPrefs.InsertColumn(0, g_i18n.GetString(IDS_SETTINGS), LVCFMT_LEFT, 34);
	m_lcPrefs.InsertItem(0, g_i18n.GetString(IDS_GENERAL),0);
	m_lcPrefs.InsertItem(1, g_i18n.GetString(IDS_WELCOMEMSG), 5);
	m_lcPrefs.InsertItem(2, g_i18n.GetString(IDS_MSG), 3);
	m_lcPrefs.InsertItem(3, g_i18n.GetString(IDS_FILTER), 2);
	m_lcPrefs.InsertItem(4, g_i18n.GetString(IDS_BOT), 6);
	m_lcPrefs.InsertItem(5, g_i18n.GetString(IDS_USERS), 1);
	m_lcPrefs.InsertItem(6, g_i18n.GetString(IDS_ABOUT), 4);
	m_lcPrefs.SetCurSel(0);
	m_nSel = 0;

	// Init Dialogs
	LoadRCMS();

	CRect rcSheet;
	GetDlgItem(IDC_STATIC_CONFIG_RECT)->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);
	
	m_pGeneral = new CGeneralDlg;
	m_pGeneral->Create(IDD_GENERAL, this);
	m_pGeneral->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pGeneral->ShowWindow(SW_SHOW);

	m_pWelcome = new CWelcomeMsg;
	m_pWelcome->Create(IDD_WELCOMEMSGS, this);
	m_pWelcome->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pWelcome->ShowWindow(SW_HIDE);

	m_pColors = new CColorDlg;
	m_pColors->Create(IDD_COLORS, this);
	m_pColors->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pColors->ShowWindow(SW_HIDE);
	
	
	m_pFilter = new CFilterDlg;
	m_pFilter->Create(IDD_FILTER, this);
	m_pFilter->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pFilter->ShowWindow(SW_HIDE);

	m_pMessages = new CMessageDlg;
	m_pMessages->Create(IDD_MESSAGES, this);
	m_pMessages->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pMessages->ShowWindow(SW_HIDE);

	m_pBot = new CSimpleBot;
	m_pBot->Create(IDD_SIMPLEBOT, this);
	m_pBot->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pBot->ShowWindow(SW_HIDE);

	m_pAbout = new CAboutDlg;
	m_pAbout->Create(IDD_ABOUT, this);
	m_pAbout->SetWindowPos(NULL, rcSheet.left+5, rcSheet.top+5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_pAbout->ShowWindow(SW_HIDE);

	SetTimer(TIMER_ID_STARTUP, 1000, 0);
	EXIT_TRACE("CBendMXDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBendMXDlg::OnSelChanged(int oldItem, int newItem)
{
	
	ENTRY_TRACE("CBendMXDlg::OnSelChanged(%d, %d)", oldItem, newItem);
	m_pGeneral->ShowWindow(SW_HIDE);
	m_pFilter->ShowWindow(SW_HIDE);
	m_pMessages->ShowWindow(SW_HIDE);
	m_pAbout->ShowWindow(SW_HIDE);
	m_pBot->ShowWindow(SW_HIDE);
	m_pColors->ShowWindow(SW_HIDE);
	m_pWelcome->ShowWindow(SW_HIDE);

	switch(newItem){

	case 0:
		m_pGeneral->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pWelcome->ShowWindow(SW_SHOW);
		break;
	case 2:
		m_pMessages->ShowWindow(SW_SHOW);
		break;
	case 3:
		m_pFilter->ShowWindow(SW_SHOW);
		break;
	case 4:
		m_pBot->ShowWindow(SW_SHOW);
		break;
	case 5:
		m_pColors->ShowWindow(SW_SHOW);
		break;
	case 6:
		m_pAbout->ShowWindow(SW_SHOW);
		break;
	}
	m_nSel = newItem;
	EXIT_TRACE("CBendMXDlg::OnSelChanged()");
}


void CBendMXDlg::OnClickPrefbar(NMHDR* pNMHDR, LRESULT* pResult) 
{

	ENTRY_TRACE("CBendMXDlg::OnClickPrefbar()");
	int nSel = m_lcPrefs.GetCurSel();
	if((nSel != -1) && (nSel != m_nSel)){

		OnSelChanged(m_nSel, nSel);
	}
	*pResult = 0;
	LTRACE("Leavinging CBendMXDlg::OnInitDialog()");
}

void CBendMXDlg::OnCancel() 
{
	ENTRY_TRACE("CBendMXDlg::OnCancel()");
	OnApply();
	ShowWindow(SW_HIDE);
	EXIT_TRACE("CBendMXDlg::OnCancel()");
}

void CBendMXDlg::OnOK() 
{

	ENTRY_TRACE("CBendMXDlg::OnOK()");
	OnApply();
	ShowWindow(SW_HIDE);
	CDialog::OnOK();
	EXIT_TRACE("CBendMXDlg::OnOK()");
}

void CBendMXDlg::OnApply() 
{

	ENTRY_TRACE("CBendMXDlg::OnApply()");
	m_pGeneral->Apply();
	m_pFilter->Apply();
	m_pMessages->Apply();
	m_pBot->Apply();
	m_pColors->Apply();
	m_pWelcome->Apply();
	g_sSettings.Save();
	g_sSettings.Load();
	EXIT_TRACE("CBendMXDlg::OnApply()");
}

void CBendMXDlg::OnDestroy() 
{

	ENTRY_TRACE("CBendMXDlg::OnDestroy()");
	CDialog::OnDestroy();
	
	if(m_pGeneral != NULL){

		LTRACE("Deleting GeneralDlg");
		m_pGeneral->UnloadPlugins();
		m_pGeneral->ShowWindow(SW_HIDE);
		m_pGeneral->DestroyWindow();
		delete m_pGeneral;
		m_pGeneral = NULL;
	}
	if(m_pWelcome != NULL){

		LTRACE("Deleting WelcomeDlg");
		m_pWelcome->ShowWindow(SW_HIDE);
		m_pWelcome->DestroyWindow();
		delete m_pWelcome;
		m_pWelcome = NULL;
	}
	if(m_pColors != NULL){

		LTRACE("Deleting ColorlDlg");
		m_pColors->ShowWindow(SW_HIDE);
		m_pColors->DestroyWindow();
		delete m_pColors;
		m_pColors = NULL;
	}

	if(m_pFilter != NULL){

		LTRACE("Deleting FilterDlg");
		m_pFilter->ShowWindow(SW_HIDE);
		m_pFilter->DestroyWindow();
		delete m_pFilter;
		m_pFilter = NULL;
	}

	if(m_pMessages != NULL){

		LTRACE("Deleting MessagesDlg");
		m_pMessages->ShowWindow(SW_HIDE);
		m_pMessages->DestroyWindow();
		delete m_pMessages;
		m_pMessages = NULL;
	}

	if(m_pBot != NULL){

		LTRACE("Deleting BotDlg");
		m_pBot->ShowWindow(SW_HIDE);
		m_pBot->DestroyWindow();
		delete m_pBot;
		m_pBot = NULL;
	}

	if(m_pAbout != NULL){

		LTRACE("Deleting AboutDlg");
		m_pAbout->ShowWindow(SW_HIDE);
		m_pAbout->DestroyWindow();
		delete m_pAbout;
		m_pAbout = NULL;
	}
	EXIT_TRACE("CBendMXDlg::OnDestroy()");
}



void CBendMXDlg::OnTimer(UINT nIDEvent) 
{

	ENTRY_TRACE("CBendMXDlg::SearchProc()");

	if(nIDEvent == TIMER_ID_SEARCH){

		//EnumChildWindows(g_hMXWindow, FindChatWindows, (LPARAM)this);
		m_bRepair = FALSE;
		for(int i = 0; i < m_aDlgs.GetSize(); i++){

			if(!m_aDlgs[i]->m_bIsHooked){

				CChatDlg* pDlg = m_aDlgs[i];
				m_aDlgs.RemoveAt(i, 1);
				m_aDlgMap.RemoveKey(pDlg->m_strTitle);
				pDlg->ShowWindow(SW_HIDE);
				pDlg->DestroyWindow();
				delete pDlg;
				pDlg = NULL;
			}
		}
	}
	if(nIDEvent == TIMER_ID_STARTUP){
		
		
		KillTimer(TIMER_ID_STARTUP);
		EnumWindows(EnumWindowsProc, (LPARAM)this);
		if(g_hMXWindow != NULL){

			LTRACE("Found WinMX Window at %X", g_hMXWindow);
			if(!SubclassWinMX(g_hMXWindow)){

				LTRACE("Failed to hook into WinMX window");
				AfxMessageBox(g_i18n.GetString(IDS_SUBCLASS_FAILED), MB_ICONSTOP+MB_OK);
			}
			else{

				LTRACE("Successfully hooked into WinMX window");
				m_bIsHooked = TRUE;
				m_pGeneral->LoadPlugins();
				if(g_sSettings.GetSoundFX()){

					PlaySound(g_sSettings.GetWorkingDir() + "\\data\\startup.dat", 0, SND_ASYNC|SND_FILENAME);
				}
			}
			SetTimer(TIMER_ID_SEARCH, g_sSettings.GetTimerRes(), 0);
		}
	}
	
	EXIT_TRACE("CBendMXDlg::SearchProc()");

	CDialog::OnTimer(nIDEvent);
}

//#define _DEBUG_SPECIAL

BOOL CALLBACK CBendMXDlg::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{

	ENTRY_TRACE("CBendMXDlg::EnumWindowsProc(hwnd=%X, lParam=%X)", hWnd, lParam);
	BOOL bReturn = TRUE;
	char *buffer = new char[400];
	::GetWindowText(hWnd, buffer, 400);

	
#ifndef _DEBUG_SPECIAL
	if((strstr(buffer, "WinMX 3.3") != NULL) || (strstr(buffer, "WinMX v3.31") != NULL)){

		AfxMessageBox("BendMX detected that you are running WinMX 3.3x!\nThis version of BendMX is designed for WinMX 3.53 and later and will not work with this version of WinMX!\n", MB_ICONSTOP);
	}
	else if(strstr(buffer, "WinMX v3.5") != NULL){
#else
	if(strstr(buffer, "WinMX 3.3") != NULL){
#endif
		g_hMXWindow = hWnd;
		bReturn = FALSE;
		g_strVersion = buffer;
		
		int n = g_strVersion.Find(" <", 0);
		if(n > 0){

			g_strVersion = g_strVersion.Left(n);
		}
		else if(g_strVersion.IsEmpty()){

			g_strVersion = "WinMX";
		}
	}
	
	delete buffer;
	buffer = NULL;
	EXIT_TRACE("CBendMXDlg::EnumWindowsProc() return %d", bReturn);
	return bReturn;
}

BOOL CBendMXDlg::SubclassWinMX(HWND hWnd)
{

	ENTRY_TRACE("CBendMXDlg::SubclassWinMX(%X)", hWnd);
	WNDPROC OldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubWinMXProc);

	if(OldWndProc && (LONG)OldWndProc != (LONG)SubWinMXProc){

		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)OldWndProc);

		EXIT_TRACE("CBendMXDlg::SubclassWinMX() return 1");
		return TRUE;
	}
	else{
		
		EXIT_TRACE("CBendMXDlg::SubclassWinMX() return 0");
		return FALSE;
	}
}


LRESULT CALLBACK CBendMXDlg::SubWinMXProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc) return FALSE;
	

	// Intercept command messages sent from 
	if(uMsg == WM_COMMAND){

		ENTRY_TRACE("CBendMXDlg::SubWinMXProc(WM_COMMAND, wParam=%X, lParam=%X)", wParam, lParam);
	
		// Call BendMX Settings
		if(wParam == 10000){

			HWND hWnd = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
			if(hWnd != NULL){

				::ShowWindow(hWnd, SW_SHOW);
			}
		}
		// Call WinMX settings
		else if(wParam == 10001){
			
			return ::CallWindowProc(OldWndProc, hwnd, WM_LBUTTONDOWN, 1, 655950);
		}
		else if((wParam >= 12000) && (wParam <= 90000)){

			for(int i = 0; i < g_Plugins.GetSize(); i++){

				g_Plugins[i]->OnMenuCommand(wParam, 0, 0);
			}
		}
		EXIT_TRACE("CBendMXDlg::SubWinMXProc()");
	}
	else if((uMsg == WM_LBUTTONDOWN)) {
		
		ENTRY_TRACE("CBendMXDlg::SubWinMXProc(WM_LBUTTONDOWN, wParam=%X, lParam=%X)", wParam, lParam);
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if((y >= 3) && (y <= 21) && (x >= 557) && (x <=618)){


			CMenu *popup = new CMenu;
			BOOL bReturn = TRUE;
			if(popup->CreatePopupMenu()){

				popup->AppendMenu(MF_STRING|MF_DISABLED, 0, g_i18n.GetString(IDS_CFG_MENU));
				popup->AppendMenu(MF_SEPARATOR, 0, " ");
				popup->AppendMenu(MF_STRING, 10000, g_i18n.GetString(IDS_BENDMX_CONFIG));
				popup->AppendMenu(MF_STRING, 10001, g_i18n.GetString(IDS_CFG_WINMX));
				popup->AppendMenu(MF_SEPARATOR, 0, " ");

				int nPluginMenuCount = 0;
				for(int i = 0; i < g_Plugins.GetSize(); i++){

					g_Plugins[i]->m_pPlugin->OnPrepareMenu(1);
					if(g_Plugins[i]->m_pPlugin->hCfgMenu <= 0) continue;
					popup->AppendMenu(MF_POPUP, (UINT)g_Plugins[i]->m_pPlugin->hCfgMenu, g_Plugins[i]->m_strShortName);
					nPluginMenuCount ++;
				}

				POINT curPos;
				curPos.x = 618;
				curPos.y = 24;
				::ClientToScreen(hwnd, &curPos);

				popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, curPos.x, curPos.y, CWnd::FromHandle(hwnd), NULL);
				popup->DestroyMenu();
			}
			delete popup;
			popup = NULL;
			EXIT_TRACE("CBendMXDlg::SubWinMXProc()");
			return TRUE;
		}
	}

	else if(uMsg == WM_DESTROY){

		HWND hWnd = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
		::SendMessage(hWnd, UWM_SHUTDOWN, 0, 0);
	}

	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK CBendMXDlg::FindChatWindows(HWND hWnd, LPARAM lParam)
{

	BOOL bReturn = TRUE;
	char *buffer = new char[1000];
	::GetWindowText(hWnd, buffer, 1000);

	//if(strstr(buffer, " on WinMX Peer Network") && (strlen(buffer) <= 75)){
	if(strstr(buffer, (LPCTSTR)((CBendMXDlg*)lParam)->m_strSearch) && (strlen(buffer) <= 75)){

		((CBendMXDlg*)lParam)->AddChat(buffer, hWnd);
	}

	delete buffer;
	buffer = NULL;

	return bReturn;
}

void CBendMXDlg::AddChat(LPCSTR strChat, HWND hWnd)
{

	ENTRY_TRACE("CBendMXDlg::AddChat(strChat=%s, hWnd=%X)", strChat, hWnd);
	CString strTitle = strChat;

	for(int i = 0; i < m_aDlgs.GetSize(); i++){

		if(m_aDlgs[i]->m_strTitle == strTitle){
			
			EXIT_TRACE("CBendMXDlg::AddChat() This window is already hooked");
			return;
		}
	}
	CChatDlg *pDlg    = new CChatDlg();
	pDlg->m_dwID     = (DWORD)hWnd;
	pDlg->m_strTitle = strTitle;
	pDlg->m_hMX      = g_hMXWindow;
	pDlg->m_bRepair  = m_bRepair;

	pDlg->Create(IDD_CHATDLG, this);
	
	pDlg->SetWindowText(strTitle + g_strIdent);

#ifdef _DEBUG_SPECIAL
	pDlg->ShowWindow(SW_SHOW);
#else
	pDlg->ShowWindow(SW_HIDE);
#endif
	m_aDlgs.Add(pDlg);
	CString strRoom = strTitle;
	strRoom.Replace(" on WinMX Peer Network", "");
	m_aDlgMap.SetAt(strRoom, pDlg->m_hWnd);

	for(i = 0; i < g_Plugins.GetSize(); i++){

		g_Plugins[i]->OnEnterChannel(pDlg->m_dwID, strTitle);
	}
	EXIT_TRACE("CBendMXDlg::AddChat()");
}

void CBendMXDlg::RemoveHook(CString strChat)
{

	ENTRY_TRACE("CBendMXDlg::RemoveHook(strChat=%s)", strChat);
	for(int i = 0; i < m_aDlgs.GetSize(); i++){

		if(m_aDlgs[i]->m_strTitle+g_strIdent == strChat){

			for(int j = 0; j < g_Plugins.GetSize(); j++){

				g_Plugins[j]->OnCloseChannel(m_aDlgs[i]->m_dwID);
			}
			m_aDlgs[i]->RemoveHook();
			return;
		}
	}
	EXIT_TRACE("CBendMXDlg::RemoveHook()");
}

LRESULT CBendMXDlg::OnRepair(WPARAM wParam, LPARAM lParam) 
{

	if(AfxMessageBox(g_i18n.GetString(IDS_REPAIRHOOK), MB_ICONQUESTION+MB_YESNO) == IDYES){

		//KillTimer(TIMER_ID_SEARCH);
		Exit();
		m_bRepair = TRUE;
		SetTimer(TIMER_ID_STARTUP, 1000, 0);
	}
	return 1;
}

void CBendMXDlg::Exit()
{

	ENTRY_TRACE("CBendMXDlg::Exit()");
	m_pGeneral->UnloadPlugins();

	while(m_aDlgs.GetSize() > 0){


		CChatDlg* pDlg = m_aDlgs[m_aDlgs.GetSize()-1];
		m_aDlgs.RemoveAt(m_aDlgs.GetSize()-1, 1);
		pDlg->ShowWindow(SW_HIDE);
		pDlg->DestroyWindow();
		m_aDlgMap.RemoveKey(pDlg->m_strTitle);
		delete pDlg;
		pDlg = NULL;
	}

	if(m_bIsHooked){
		
		WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(g_hMXWindow, GWL_USERDATA);
		::SetWindowLong(g_hMXWindow, GWL_WNDPROC, (LONG)OldWndProc);
		m_bIsHooked = FALSE;
	}
	EXIT_TRACE("CBendMXDlg::Exit()");
}

void CBendMXDlg::InputMessage(CString strMessage, CString strChat, BOOL bEnter)
{

	ENTRY_TRACE("CBendMXDlg::InputMessage()");
	if(strChat.IsEmpty()){

		for(int i = 0; i < m_aDlgs.GetSize(); i++){

			m_aDlgs[i]->InputMessage(strMessage, bEnter);
		}
	}
	else{

		for(int i = 0; i < m_aDlgs.GetSize(); i++){

			if(m_aDlgs[i]->m_strTitle == strChat){

				m_aDlgs[i]->InputMessage(strMessage, bEnter);
				EXIT_TRACE("CBendMXDlg::InputMessage()");
				return;
			}
		}
	}
	EXIT_TRACE("CBendMXDlg::InputMessage() Second Exit");
}


void CBendMXDlg::InputMessage(CString strMessage, DWORD dwID)
{

	ENTRY_TRACE("CBendMXDlg::InputMessage(dwID=%X)", dwID);
	for(int i = 0; i < m_aDlgs.GetSize(); i++){

		if(m_aDlgs[i]->m_dwID == dwID){

			m_aDlgs[i]->InputMessage(strMessage);
			return;
		}
	}
	EXIT_TRACE("CBendMXDlg::InputMessage()");
}


LRESULT CBendMXDlg::OnShutdown(WPARAM wParam, LPARAM lParam)
{

	//::KillTimer(m_hWnd, TIMER_ID_SEARCH);
	Exit();
	return 1;
}

LRESULT CBendMXDlg::OnAway(WPARAM wParam, LPARAM lParam)
{

	ENTRY_TRACE("CBendMXDlg::OnAway(wParam=X, lParam=%X)", wParam, lParam);
	int nAction = (int)wParam;
	int nReason = (int)lParam;


	if(nAction != 2){ // Set Back

		DWORD dwAway = GetTickCount() - m_dwAwayTick;
		m_dwAwayTick = 0;
		int nSec = dwAway / 1000;
		int nMin = dwAway / 60000;
		int nHour = dwAway / 3600000;
		CString strMsg, strTmp;
		strTmp.Format("%02d h %02d m %02d s", nHour, nMin - nHour*60, nSec - nMin*60);
		
		if(nAction){
			
			strMsg.Format(g_i18n.GetString(IDS_BACKSILENT), m_strAway, strTmp);
		}
		else{

			strMsg.Format( g_i18n.GetString(IDS_BACK), m_strAway, g_strMyName, strTmp);
		}
					;
		g_bAway = FALSE;
		for(int i = 0; i < m_aDlgs.GetSize(); i++){

			m_aDlgs[i]->InputMessage(strMsg, TRUE);
		}
	}
	else{ // Set away

		m_dwAwayTick = GetTickCount();
		g_bAway = TRUE;
		switch(nReason){

		case 0:
			m_strAway = g_i18n.GetString(IDS_ZZZZZZZZ);
			break;
		case 1:
			m_strAway = g_i18n.GetString(IDS_WORK);
			break;
		case 2:
			m_strAway = g_i18n.GetString(IDS_SLEEP);
			break;
		case 3:
			m_strAway = g_i18n.GetString(IDS_AIR);
			break;
		case 4:
			m_strAway = g_i18n.GetString(IDS_BATHROOM);
			break;
		case 5:
			m_strAway = g_i18n.GetString(IDS_SMOKE);
			break;
		case 6:
			m_strAway = g_i18n.GetString(IDS_FOOD);
			break;
		case 7:
			m_strAway = g_i18n.GetString(IDS_BEER);
			break;
		case 8:
			m_strAway = g_i18n.GetString(IDS_TV);
			break;
		case 9:
			m_strAway = g_i18n.GetString(IDS_STORE);
			break;
		case 10:
			m_strAway = g_i18n.GetString(IDS_NONE);
			break;
		case 11:
			char buffer[300];
			HWND hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_GETOUT, 0, (LPARAM)(char*)&buffer);
			m_strAway = buffer;
			break;			
		}
		
		CString strMsg;
		strMsg.Format(g_i18n.GetString(IDS_GOINGAWAY), m_strAway);
		for(int i = 0; i < m_aDlgs.GetSize(); i++){

			m_aDlgs[i]->InputMessage(strMsg, TRUE);
		}
	}
	EXIT_TRACE("CBendMXDlg::OnAway()");
	return 1;
}

BOOL CBendMXDlg::DoesRoomExist(const CString strRoom)
{

	ENTRY_TRACE("CBendMXDlg::DoesRoomExist()");
	BOOL bReturn = FALSE;

	for(int i = 0; i < m_aDlgs.GetSize(); i++){

		if(m_aDlgs[i]->m_strTitle == strRoom){

			bReturn = TRUE;
			break;
		}
	}
	EXIT_TRACE("CBendMXDlg::OnAway() return %d", bReturn);
	return bReturn;
}

void CBendMXDlg::LoadRCMS()
{

	ENTRY_TRACE("CBendMXDlg::LoadRCMS()");
	CString strIniFile = g_sSettings.GetWorkingDir() + "\\commands.dat";
	CStdioFile ini;
	CString strBuffer;
	g_aRCMSCommands.RemoveAll();
	g_aWinMXCommands.RemoveAll();
	g_aRoseCommands.RemoveAll();
	g_aMXToolsCommands.RemoveAll();


	WORD wMode = -1; // 0 = RCMS, 1 = WinMX, 2 = RoboMX, 3 = MXTools

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(strBuffer.IsEmpty()) continue;

			if(strBuffer.Find("[RCMS]", 0) == 0){

				wMode = 0;
				continue;
			}
			if(strBuffer.Find("[WINMX]", 0) == 0){

				wMode = 1;
				continue;
			}
			if(strBuffer.Find("[MXTOOLS]", 0) == 0){

				wMode = 2;
				continue;
			}
			if(strBuffer.Find("[ROSE]", 0) == 0){

				wMode = 3;
				continue;
			}
			if(strBuffer.Find("[ROBOMX]", 0) == 0){

				wMode = 4;
				continue;
			}
			switch(wMode){

				case 0:
					g_aRCMSCommands.Add(strBuffer);
					break;
				case 1:
					g_aWinMXCommands.Add(strBuffer);
					break;
				case 2:
					g_aMXToolsCommands.Add(strBuffer);
					break;
				case 3:
					g_aRoseCommands.Add(strBuffer);
				default:
					// ignore;
					break;
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox("Error reading commands.dat", MB_OK+MB_ICONSTOP);
	}END_CATCH;

	EXIT_TRACE("CBendMXDlg::LoadRCMS()");
}

UINT CBendMXDlg::WorkerThread(LPVOID pParam)
{

	return 0;
}

LRESULT CBendMXDlg::SearchChatWnd(WPARAM wParam, LPARAM lParam)
{

	CString strChat = (LPCTSTR)lParam;

	m_strSearch = strChat + " on WinMX Peer Network";
	
	EnumChildWindows(g_hMXWindow, FindChatWindows, (LPARAM)this);
	m_strSearch.Empty();
	return 1;
}

#endif // #ifdef __BENDMX__

