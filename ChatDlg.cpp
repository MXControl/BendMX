/*
 * BendMX - Chat extension library for WinMX.
 * Copyright (C) 2003-2005 by Thees Schwab
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact: tcwinkler@users.sourceforge.net
 *
*/

#include "stdafx.h"
#include "BendMX.h"
#ifdef __BENDMX__

#include "ChatDlg.h"
#include "Settings.h"
#include "winamp.h"
#include "i18n.h"
#include "Clipboard.h"
#include "Tokenizer.h"
#include "SystemInfo.h"
#include "Plugin.h"
#include "ini.h"
#include "logging\logging.h"

INITTRACE("ChatDlg")

const char* line_types[] = {"Unknown", "14.4K", "28.8K", "33.3K", "56K", 
                            "64K ISDN", "128K ISDN", "Cable", "DSL", "T1", "T3"};


#define NUM_LINES 11

extern CArray<CPlugin*, CPlugin*> g_Plugins;
extern CStringArray g_strTrusted;
extern CStringArray g_strFilter;
extern CString      g_strIdent;
extern CStringArray g_strRooms;
extern CMap<SOCKET, SOCKET, PSOCKETMONITOR, PSOCKETMONITOR> g_mSocketMap;
extern CString		g_strMyName;

#define MENU_BASE		 10000
#define MENU_BASE_CTRL   20000
#define MENU_RCMS        15000

#define ID_CSTM_MENUBASE  22100
#define ID_CSTM_MENUBASE2 23100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL g_bAway;

// Registered  Window messages used by this class

UINT UWM_INPUTMESSAGE	= ::RegisterWindowMessage("UWM_INPUTMESSAGE_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_UPTIME			= ::RegisterWindowMessage("UWM_UPTIME_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_COPYTOPIC		= ::RegisterWindowMessage("UWM_COPYTOPIC_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_MESSAGEFORWARD = ::RegisterWindowMessage("UWM_MESSAGEFORWARD_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_GETID			= ::RegisterWindowMessage("UWM_GETID_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_GETRCMS		= ::RegisterWindowMessage("UWM_GETRCMS_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_CHARFORMAT		= ::RegisterWindowMessage("UWM_CHARFORMAT_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_ONRCMSMENU		= ::RegisterWindowMessage("UWM_ONRCMSMENU_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_ONRCMSEDIT		= ::RegisterWindowMessage("UWM_ONRCMSEDIT_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_COPYIP		    = ::RegisterWindowMessage("UWM_COPYIP_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
extern UINT UWM_MESSAGE;
extern UINT UWM_ACTION;
extern UINT UWM_TOPIC;
extern UINT UWM_MOTD;
extern UINT UWM_JOIN;
extern UINT UWM_PART;
extern UINT UWM_RENAME;
extern UINT UWM_REDIRECT;
extern UINT UWM_ROOMRENAME;
extern UINT UWM_SERVERTYPE;
extern UINT UWM_OPMESSAGE;
extern UINT UWM_IPCLIENT;
extern UINT UWM_ADDUSER;

extern UINT UWM_SETID;
extern UINT UWM_SPECIAL;
extern UINT UWM_AWAY;

// global variables and settings
extern CStringArray g_strAuto;
extern CStringArray g_strHistory;
extern CBendMXApp	theApp;
extern CI18n		g_i18n;
extern DWORD        g_nMXStartTic;
extern CSettings	g_sSettings;
extern CStringArray g_strOps;
extern CStringArray g_strHosts;
extern HWND			g_hMXWindow;
extern BOOL			g_bRCMS;
extern CStringArray g_aRCMSCommands;
extern CStringArray g_aRoseCommands;
extern CStringArray g_aWinMXCommands;
extern CStringArray g_aMXToolsCommands;


CMap<HWND, HWND, CStringArray*, CStringArray*> g_mCmds;
CMap<HWND, HWND, int, int> g_mServers;

#endif

CString FormatIP(DWORD dwIP)
{
	
	if(dwIP == 0) return "";

	CString strReturn;
	int a = ((dwIP >> 24) & 0xff);
	int b = ((dwIP >> 16) & 0xff);
	int c = ((dwIP >> 8) & 0xff);
	int d = (dwIP & 0xff);
	strReturn.Format("%d.%d.%d.%d", d, c, b, a);
	return strReturn;
}

#ifdef __BENDMX__

CString FormatLine(WORD wLine)
{
	
	CString strLine;
	if((wLine >= 0) && (wLine < NUM_LINES)){
		
		strLine = line_types[wLine];
	}
	else{

		strLine = "N/A";
	}

	return strLine;
}

int CChatDlg::g_aRCMSCommandsNUM(HWND hWnd)
{

	CStringArray *pCmds = 0;
 	int n = -1;
 	if(g_mCmds.Lookup(hWnd, pCmds)){
 
 		if(pCmds != 0){
 
			n = pCmds->GetSize();
 		}
 	}
	if(n == -1){

		n = g_aRCMSCommands.GetSize();
	}
	return n;
}

void CChatDlg::OnCancel(){}
void CChatDlg::OnOK(){}


CSystemInfo g_siInfo;

///////////////////////////////////////
// Helper functions
BOOL CALLBACK CChatDlg::FindWhois(HWND hWnd, LPARAM lParam){

	BOOL bReturn = TRUE;
	char *buffer = new char[500];

	::GetWindowText(hWnd, buffer, 500);
	if(strstr(buffer, "on WinMX") && (strstr(buffer, "Peer Network") == NULL)){

		strcpy((char*)lParam, buffer);
		bReturn = FALSE;
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	delete buffer;
	buffer = NULL;
	return bReturn;
}

BOOL CALLBACK CChatDlg::FindWhoisNotClose(HWND hWnd, LPARAM lParam){

	BOOL bReturn = TRUE;
	char *buffer = new char[500];

	::GetWindowText(hWnd, buffer, 500);
	if(strstr(buffer, "on WinMX") && (strstr(buffer, "Peer Network") == NULL)){

		strcpy((char*)lParam, buffer);
		bReturn = FALSE;
	}
	delete buffer;
	buffer = NULL;
	return bReturn;
}

/////////////////////////////////
// Add string to history
/////////////////////////////////
void CChatDlg::AddHistory(CString strText){

	if(g_strHistory.GetSize() > g_sSettings.GetHistoryDepth()){

		g_strHistory.RemoveAt(0);
	}
	g_strHistory.Add(strText);
}

int CChatDlg::SearchRCMSItem(HWND hwnd, CString strString)
{

	CString strTmp;

	CStringArray* pCmds = 0;
	if(!g_mCmds.Lookup(hwnd, pCmds)){

		pCmds = &g_aRCMSCommands;
	}

	for(int nIndex = 0; nIndex < pCmds->GetSize(); nIndex++){

		strTmp = pCmds->GetAt(nIndex);
		if(strTmp == strString) break;
	}

	if((nIndex >= pCmds->GetSize())  || pCmds->GetSize() == 0) return 0;

	return nIndex + 1;
}
/////////////////////////////////
// Search in history
/////////////////////////////////
int CChatDlg::SearchHistory(CString strString)
{
	
	for(int nIndex = 0; nIndex < g_strHistory.GetSize(); nIndex++){

		if(g_strHistory[nIndex] == strString) break;
	}

	if((nIndex >= g_strHistory.GetSize()) || g_strHistory.GetSize() == 0) return 0;

	return nIndex + 1;
}

/////////////////////////////////
// Search string in autocommand
/////////////////////////////////
int CChatDlg::SearchItem(CString strString)
{

	CString strTmp;

	for(int nIndex = 0; nIndex < g_strAuto.GetSize(); nIndex++){

		strTmp = g_strAuto[nIndex];
		if(strTmp == strString) break;
	}

	if((nIndex >= g_strAuto.GetSize())  || g_strAuto.GetSize() == 0) return 0;

	return nIndex + 1;
}


////////////////////////////////////
// Check for Winamp control command
////////////////////////////////////
void CChatDlg::ControlWinamp(CString strString)
{

	if(strString.IsEmpty()) return;
	if(strString[0] != '/') return;

	HWND hwnd_winamp = ::FindWindow("Winamp v1.x",NULL);
	if(hwnd_winamp == NULL) return;

	if(strString == "/play"){
		
		//::SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_STARTPLAY);
		::SendMessage(hwnd_winamp, WM_COMMAND, WINAMP_BUTTON2, 0);	
	}
	else if(strString == "/pause"){

		::SendMessage(hwnd_winamp, WM_COMMAND, WINAMP_BUTTON3, 0);	
	}
	else if(strString == "/next"){

		::SendMessage(hwnd_winamp, WM_COMMAND, WINAMP_BUTTON5, 0);	
	}
	else if(strString == "/prev"){

		::SendMessage(hwnd_winamp, WM_COMMAND, WINAMP_BUTTON1, 0);	
	}
	else if(strString == "/stop"){

		::SendMessage(hwnd_winamp, WM_COMMAND, WINAMP_BUTTON4, 0);	
	}
}



/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog


CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDlg)
	m_bTop = FALSE;
	m_nTrans = 0;
	m_strTrans = _T("");
	//}}AFX_DATA_INIT
	m_hEdit = NULL;
	m_hMX   = NULL;
	m_hList = NULL;
	m_hRich = NULL;
	m_hChat = NULL;
	m_bIsHooked = FALSE;
	m_nStartTic = 0;
	m_bRepair = FALSE;
	m_bRCMS   = FALSE;
	m_bListBans = FALSE;
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_STATIC_100, m_wndTrans);
	DDX_Control(pDX, IDC_TRANSPARENCY, m_slTrans);
	DDX_Control(pDX, IDC_USERLIST, m_lcUsers);
	DDX_Check(pDX, IDC_TOP, m_bTop);
	DDX_Slider(pDX, IDC_TRANSPARENCY, m_nTrans);
	DDX_Text(pDX, IDC_STATIC_100, m_strTrans);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_TOP, OnTop)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RCLICK, IDC_USERLIST, OnRclickUserlist)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_UPTIME, OnGetUpTime)
	ON_REGISTERED_MESSAGE(UWM_GETID, OnGetID)
	ON_REGISTERED_MESSAGE(UWM_GETRCMS, OnGetRCMS)
	ON_REGISTERED_MESSAGE(UWM_INPUTMESSAGE, OnInputMessage)
	ON_REGISTERED_MESSAGE(UWM_ONRCMSMENU, OnRCMSMenu)
	ON_REGISTERED_MESSAGE(UWM_ONRCMSEDIT, CustomizeRCMS)
	ON_REGISTERED_MESSAGE(UWM_ACTION, OnAction)
	ON_REGISTERED_MESSAGE(UWM_MESSAGE, OnMessage)
	ON_REGISTERED_MESSAGE(UWM_TOPIC, OnTopic)
	ON_REGISTERED_MESSAGE(UWM_MOTD, OnMotd)
	ON_REGISTERED_MESSAGE(UWM_JOIN, OnJoin)
	ON_REGISTERED_MESSAGE(UWM_PART, OnPart)
	ON_REGISTERED_MESSAGE(UWM_RENAME, OnRenameMsg)
	ON_REGISTERED_MESSAGE(UWM_ROOMRENAME, OnRoomRename)
	ON_REGISTERED_MESSAGE(UWM_SERVERTYPE, OnSetServerType)
	ON_REGISTERED_MESSAGE(UWM_OPMESSAGE, OnOpMessage)
	ON_REGISTERED_MESSAGE(UWM_IPCLIENT, OnIPClient)
	ON_REGISTERED_MESSAGE(UWM_COPYIP, OnCopyIP)
	ON_REGISTERED_MESSAGE(UWM_ADDUSER, OnAddUser)
	ON_COMMAND_RANGE(10200, 10300, OnAdminMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers


BOOL CChatDlg::OnInitDialog() 
{

	ENTRY_TRACE("ChatDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_APPWINDOW);

	m_lcUsers.SetHeadings(IDS_USERLISTHEADER);
	m_lcUsers.SetBkColor(0);
	m_lcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CBitmap		bitmap;
	bitmap.LoadMappedBitmap(IDB_USERMODES);
	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	m_iImageList.Add(&bitmap, RGB(255,0,255));
	m_lcUsers.SetImageList(&m_iImageList, LVSIL_SMALL);

	m_slTrans.SetRange(0, 100, TRUE);
	if(g_siInfo.IsWindowsNT()){

		m_slTrans.EnableWindow(TRUE);
		m_nTrans = 0;
		m_slTrans.SetPos(0);
		m_strTrans = "0%";
		UpdateData(FALSE);
	}

 	if(m_hMX != NULL){

 		EnumChildWindows(m_hMX, FindEditControl, (LPARAM)this);
		EnumChildWindows(m_hMX, FindListControl, (LPARAM)this);
		EnumChildWindows(m_hMX, FindChatControl, (LPARAM)this);
	}

	
	int iPos = m_strTitle.ReverseFind('_');
	if(iPos != -1)	m_strRoomShort = m_strTitle.Left(iPos);

	POSITION pos = g_mSocketMap.GetStartPosition();

	while(pos){

		PSOCKETMONITOR p = 0;
		SOCKET         s = 0;
		
		g_mSocketMap.GetNextAssoc(pos, s, p);
		
		if(p == 0) continue;
		if(p->strRoom.Find(m_strRoomShort, 0) != 0) continue;

		p->dwID = m_dwID;
		OnSetServerType(0, (LPARAM)p->wServerType);

		TRACE("Servertype is %d\n", m_nServerType);
		break;
	}

	if((m_hEdit != NULL) && (m_hList != NULL) && (m_hChat != NULL) && (m_hRich != NULL)){
 	 		//  <-- no more :-(
		if(!SubclassDlg(m_hChat)|| !SubclassList(m_hList) || !SubclassEdit(m_hEdit) || !SubclassChat(m_hRich)){

			LTRACE("Error. Could not install hook.");
		}
		else{

			if(!m_bRepair){

				InputWelcome();
			}
			m_bIsHooked = TRUE;
			m_nStartTic = GetTickCount();
			m_bRepair = FALSE;
		}
	}
 	else{
 
		AfxMessageBox("Error. Could not install hook.\nnBendMX will not be operable for this Chatwindow.");
		LTRACE("Error. Could not install hook.");
	}
	
	LoadRCMS();
	EXIT_TRACE("ChatDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDlg::InputMessage(CString strMessage, BOOL bEnter)
{

	strMessage.Replace("\\rtf", "****");
	TCHAR szTempStr[1024];
	lstrcpy(szTempStr, strMessage);	 
	if(bEnter){

		CallOrigWndProc(m_hEdit, EM_SETSEL, 0, -1);
		CallOrigWndProc(m_hEdit, EM_REPLACESEL, TRUE, (LPARAM)szTempStr);
		::PostMessage(m_hEdit, WM_KEYDOWN, 13, 256);
	}
	else{

		CallOrigWndProc(m_hEdit, EM_REPLACESEL, TRUE, (LPARAM)szTempStr);
	}
}

void CChatDlg::InputWelcome()
{


	if(g_sSettings.GetSoundFX()){

		PlaySound(g_sSettings.GetWorkingDir() + "\\data\\connected.dat", 0, SND_ASYNC|SND_FILENAME);
	}
	if(g_sSettings.GetDoEnterMsg()){
		
		CString strMsg = g_sSettings.GetEnterMsg();
		ReplaceVars(strMsg);
		InputMessage(strMsg);
	}
	else{

		for(int i = 0; i < g_strRooms.GetSize(); i++){

			if(m_strTitle.Find(g_strRooms[i], 0) >= 0){

				CString strMsg = g_sSettings.GetEnterMsg();
				ReplaceVars(strMsg);
				InputMessage(strMsg);
				break;
			}
		}
	}
}


void CChatDlg::OnDestroy() 
{

	ENTRY_TRACE("CChatDlg::OnDestroy()");
	if(m_bIsHooked){

		if(g_sSettings.GetSoundFX()){

			PlaySound(g_sSettings.GetWorkingDir() + "\\data\\left.dat", 0, SND_ASYNC|SND_FILENAME);
		}
		RemoveHook();
	}
	CDialog::OnDestroy();
	EXIT_TRACE("CChatDlg::OnDestroy()");
}

/////////////////////
// Find The edit control
//
BOOL CALLBACK CChatDlg::FindEditControl(HWND hWnd, LPARAM lParam)
{

	if(::GetDlgCtrlID(hWnd) == 1502){

		HWND hParent = ::GetParent(hWnd);
		char *buffer = new char[1000];
		
		::GetWindowText(hParent, buffer, 1000);
		CString strTmp = buffer;
		::GetClassName(hWnd, buffer, 1000);
		CString strClass = buffer;

		delete buffer;
		buffer = NULL;
		
		if((strTmp == ((CChatDlg*)lParam)->m_strTitle) && (strClass == "Edit")){

			((CChatDlg*)lParam)->m_hEdit = hWnd;
			((CChatDlg*)lParam)->m_hChat = hParent;
			LTRACE("Found EditControl");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CALLBACK CChatDlg::FindListControl(HWND hWnd, LPARAM lParam)
{

	if(::GetDlgCtrlID(hWnd) == 1501){

		HWND hParent = ::GetParent(hWnd);
		char *buffer = new char[1000];
		
		::GetWindowText(hParent, buffer, 1000);
		CString strTmp = buffer;
		::GetClassName(hWnd, buffer, 1000);
		CString strClass = buffer;

		delete buffer;
		buffer = NULL;
		
		if((strTmp == ((CChatDlg*)lParam)->m_strTitle) && (strClass == "SysListView32")){

			((CChatDlg*)lParam)->m_hList = hWnd;
			LTRACE("Found ListControl");
			return FALSE;
		}
	}
	return TRUE;
}

//// Subclass it
BOOL CChatDlg::SubclassEdit(HWND hWnd)
{

	ENTRY_TRACE("CChatDlg::SubclassEdit(hwn=%X)", hWnd);
	if(g_bRCMS){
		
		EXIT_TRACE("CChatDlg::SubclassEdit() RCMS is present");
		return TRUE;
	}

	WNDPROC OldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubEditProc);
	BOOL bReturn = FALSE;
	
	if(OldWndProc && (LONG)OldWndProc != (LONG)SubEditProc){

		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)OldWndProc);
		bReturn = TRUE;
	}

	EXIT_TRACE("CChatDlg::SubclassEdit() return=%d", bReturn);

	return bReturn;
}


LRESULT CALLBACK CChatDlg::SubEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);

	if(!OldWndProc)return FALSE;

	///////////////////////////////////////////////////////
	// Change Middle mouse button click to return key down
	///////////////////////////////////////////////////////
	if(uMsg == WM_MBUTTONDOWN){

		LTRACE("SubEditProc() WM_MBUTTONDOWN");
		::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, 13, 256);
		return TRUE;
	}

	///////////////////////////////////////////////////////
	// Check if this is a message from our menus...
	///////////////////////////////////////////////////////
	else if(uMsg == WM_COMMAND){
		
		LTRACE("SubEditProc() WM_COMMAND, wParam=%x, lParam=%x", wParam, lParam);
		///////////////////////////////////////////////////////
		//  command send from command menu
		///////////////////////////////////////////////////////
		if((wParam >= MENU_BASE) && (wParam <= MENU_BASE + g_strAuto.GetSize())){

			int nID = wParam - MENU_BASE;
			if(nID < g_strAuto.GetSize()){

				TCHAR szTempStr[1024];
				CString strTime, strOut;
				if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

					strOut = szTempStr;
				}
				
				strOut += g_strAuto[nID];

				lstrcpy(szTempStr, strOut);
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			}
			return TRUE;
		}
		///////////////////////////////////////////////////////
		// command send from history menu
		///////////////////////////////////////////////////////
		else if((wParam >= MENU_BASE_CTRL) && (wParam <= MENU_BASE_CTRL + 15)){

			int nID = wParam - MENU_BASE_CTRL;
			if(nID < g_strHistory.GetSize()){

				TCHAR szTempStr[1024];
				CString strTime, strOut;
				if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

					strOut = szTempStr;
				}
				
				strOut += g_strHistory[nID];

				lstrcpy(szTempStr, strOut);
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			}
			return TRUE;
		}
		///////////////////////////////////////////////////////
		// command send from rcms menu
		///////////////////////////////////////////////////////
		else if((wParam >= MENU_RCMS) && (wParam <= MENU_RCMS + g_aRCMSCommandsNUM(hwnd))){

			int nID = wParam - MENU_RCMS;
			if(nID < g_aRCMSCommandsNUM(hwnd)){

				TCHAR szTempStr[1024];
				CString strTime, strOut;
				if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

					strOut = szTempStr;
				}
				
				CStringArray *pCmds = 0;
				if(!g_mCmds.Lookup(hwnd, pCmds)){

					pCmds = &g_aRCMSCommands;
				}
				CString strCmd = pCmds->GetAt(nID);
				strOut += strCmd;

				lstrcpy(szTempStr, strOut);
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			}
			return TRUE;
		}
	}
	///////////////////////////////////////////////////////
	// intercept Rightclick -> contextmenu notification
	///////////////////////////////////////////////////////
	else if(uMsg == WM_CONTEXTMENU){

		LTRACE("SubEditProc() WM_CONTEXTMENU, wParam=%x, lParam=%x", wParam, lParam);
		CMenu *popup = new CMenu;
		BOOL bReturn = TRUE;
		if(popup->CreatePopupMenu()){

			// History menu
			if((GetKeyState(VK_CONTROL) < 0) && !(GetKeyState(VK_SHIFT) < 0)){

				popup->AppendMenu(MF_STRING|MF_DISABLED, 0, g_i18n.GetString(IDS_HISTORY_MENU));
				popup->AppendMenu(MF_SEPARATOR, 0, "0");
				for(int i = 0; i < g_strHistory.GetSize() && i < 16; i++){

					popup->AppendMenu(MF_STRING, MENU_BASE_CTRL+i, g_strHistory[i]);
				}
			}
			// RCMS menu
			else if((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0)){

				popup->AppendMenu(MF_STRING|MF_DISABLED, 0, g_i18n.GetString(IDS_RCMSMENU));
				popup->AppendMenu(MF_SEPARATOR, 0, "0");
				for(int i = 0; i < g_aRCMSCommandsNUM(hwnd); i++){

					CStringArray *pCmds = 0;
					if(!g_mCmds.Lookup(hwnd, pCmds)){

						pCmds = &g_aRCMSCommands;
					}
					if(i == g_aRCMSCommandsNUM(hwnd) / 2){

						popup->AppendMenu(MF_MENUBARBREAK|MF_STRING, MENU_RCMS+i, pCmds->GetAt(i));
					}
					else{

						popup->AppendMenu(MF_STRING, MENU_RCMS+i, pCmds->GetAt(i));
					}
				}
			}
			// Command Menu
			else if((GetKeyState(VK_SHIFT) < 0) && !(GetKeyState(VK_CONTROL) < 0)){

				popup->AppendMenu(MF_STRING|MF_DISABLED, 0, g_i18n.GetString(IDS_CMDMENU));
				popup->AppendMenu(MF_SEPARATOR, 0, "0");
				for(int i = 0; i < g_strAuto.GetSize(); i++){

					popup->AppendMenu(MF_STRING, MENU_BASE+i, g_strAuto[i]);
				}
			}
			else{

				bReturn =  ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
			}

			POINT curPos;
			::GetCursorPos(&curPos);
			popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, curPos.x, curPos.y, CWnd::FromHandle(hwnd), NULL);
			popup->DestroyMenu();
		}
		delete popup;
		popup = NULL;

		return bReturn;
	}
	///////////////////////////////////////////////////////
	// command scroller etc.
	///////////////////////////////////////////////////////
	else if(uMsg == WM_MOUSEWHEEL || uMsg == WM_KEYDOWN){

		LTRACE("SubEditProc() WM_MOUSWHEEL || WM_KEYDOWN, wParam=%x, lParam=%x", wParam, lParam);
		///////////////////////////////////////////////////////
		// Scroll through RCMS commands UP (PAGE UP)
		///////////////////////////////////////////////////////
		if(uMsg == WM_KEYDOWN && wParam == VK_PRIOR){
		
			CStringArray *pCmds = 0;
			if(!g_mCmds.Lookup(hwnd, pCmds)){

				pCmds = &g_aRCMSCommands;
			}
			if(pCmds->GetSize() == 0) return TRUE;

			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				DWORD dwIndex = SearchRCMSItem(hwnd, szTempStr);
				if(dwIndex == 0){

					lstrcpy(szTempStr, pCmds->GetAt(pCmds->GetSize() - 1));
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if(dwIndex == 1){

					::SetWindowText(hwnd, "");
				}
				else{

					lstrcpy(szTempStr, pCmds->GetAt(dwIndex - 2));
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				lstrcpy(szTempStr, pCmds->GetAt(pCmds->GetSize() - 1));
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// RCMS Down (PAGE DOWN)
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_NEXT){
		
			CStringArray *pCmds = 0;
			if(!g_mCmds.Lookup(hwnd, pCmds)){

				pCmds = &g_aRCMSCommands;
			}
			if(pCmds->GetSize() == 0) return TRUE;

			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

				DWORD dwIndex = SearchRCMSItem(hwnd, szTempStr);
				if(dwIndex >= pCmds->GetSize()){

					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)"");
					//Beep(1000, 50);
				}
				else{

					lstrcpy(szTempStr, pCmds->GetAt(dwIndex));
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				lstrcpy(szTempStr, pCmds->GetAt(0));
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// HISTORY UP (CTRL+UPARROR/MOUSEWHEEL UP)
		///////////////////////////////////////////////////////
		else if(((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) > 0) || (uMsg == WM_KEYDOWN && wParam == VK_UP)) && (GetKeyState(VK_CONTROL) < 0)){

			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				DWORD dwIndex = SearchHistory(szTempStr);
				if((dwIndex == 0) && (g_strHistory.GetSize() != 0)){ // Item was not found

					lstrcpy(szTempStr, g_strHistory[g_strHistory.GetSize() - 1]);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if(dwIndex == 1){ // Item found but we are at no1

					::SetWindowText(hwnd, "");
				}
				else{ // item found 

					lstrcpy(szTempStr, g_strHistory[dwIndex - 2]);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				if(g_strHistory.GetSize() != 0){
					
					lstrcpy(szTempStr, g_strHistory[g_strHistory.GetSize() - 1]);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// HISTORY DOWN CTRL+DOWNARROR/MOUSWHEEL DOWN
		///////////////////////////////////////////////////////
		else if(((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) <= 0) || (uMsg == WM_KEYDOWN && wParam == VK_DOWN)) && (GetKeyState(VK_LCONTROL) < 0)){
			
			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){
				DWORD dwIndex = SearchHistory(szTempStr);
				if(dwIndex >=  g_strHistory.GetSize()){

					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)"");
					//Beep(1000, 50);
				}
				else{

					if(g_strHistory.GetSize() != 0){

						lstrcpy(szTempStr,  g_strHistory[dwIndex]);
						::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
						::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
					}
				}
				return TRUE;
			}
			else{

				if(g_strHistory.GetSize() != 0){

					lstrcpy(szTempStr,  g_strHistory[0]);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// Commands UP UPARROW or MOUSEHWEL UP
		///////////////////////////////////////////////////////
		else if(((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) > 0) || 
			     (uMsg == WM_KEYDOWN && wParam == VK_UP)) && (g_strAuto.GetSize() != 0)){
		
			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				DWORD dwIndex = SearchItem(szTempStr);
				if(dwIndex == 0){ // Item was not found

					CString strOut = g_strAuto[g_strAuto.GetSize() - 1];
					CChatDlg::ReplaceVars(strOut);
					lstrcpy(szTempStr, strOut);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if(dwIndex == 1){ // Item found but we are at no1

					::SetWindowText(hwnd, "");
					//Beep(1000, 50);
				}
				else{ // item found 

					CString strOut = g_strAuto[dwIndex - 2];
					CChatDlg::ReplaceVars(strOut);
					lstrcpy(szTempStr, strOut);
					::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
					::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				CString strOut = g_strAuto[g_strAuto.GetSize() - 1];
				CChatDlg::ReplaceVars(strOut);
				lstrcpy(szTempStr, strOut);
				::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT, 0, (LPARAM)szTempStr);
				::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// Commands down DOWNARROW or MOUSEWHEEL DOWN
		///////////////////////////////////////////////////////
		else if(((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) <= 0) || 
			(uMsg == WM_KEYDOWN && wParam == VK_DOWN)) && (g_strAuto.GetSize() != 0)){
		
			TCHAR szTempStr[1024];
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){
				DWORD dwIndex = SearchItem(szTempStr);
				if(dwIndex >= g_strAuto.GetSize()){

					::SetWindowText(hwnd, "");
					//Beep(1000, 50);
				}
				else{

					CString strOut = g_strAuto[dwIndex];
					CChatDlg::ReplaceVars(strOut);

					lstrcpy(szTempStr, strOut);

					::SetWindowText(hwnd, szTempStr);
					::SendMessage(hwnd, WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

					CString strOut = g_strAuto[0];
					CChatDlg::ReplaceVars(strOut);

					lstrcpy(szTempStr, strOut);
				::SetWindowText(hwnd, szTempStr);
				::SendMessage(hwnd, WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// F3 Insert Download and upload stats
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F3){

			CString strStats;
			TCHAR szTempStr[1024];

			char *buffer = new char[500];
			::GetWindowText(g_hMXWindow, buffer, 500);
			strStats = buffer;
			delete buffer; buffer = NULL;
			
			//strStats.Replace("WinMX v3.31", "Stats ");
			
			lstrcpy(szTempStr, strStats);
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			return TRUE;

		}
		///////////////////////////////////////////////////////
		// F4 Insert Room Uptime time at cursor
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F4){

			CString strUptime;
			TCHAR szTempStr[1024];

			HWND hParent = ::GetParent(hwnd);
			char *buffer = new char[500];
			::GetWindowText(hParent, buffer, 500);
			strcat(buffer, g_strIdent);
			HWND hHAL = ::FindWindow(NULL, buffer);
			int nTic = ::SendMessage(hHAL, UWM_UPTIME, 0, 0);
			delete buffer; buffer = NULL;

			DWORD nMS = GetTickCount() -  nTic;
			int nSec = nMS / 1000;
			int nMin = nMS / 60000;
			int nHour = nMS / 3600000;

			strUptime.Format("%s %02d:%02d:%02d.%03d", g_i18n.GetString(IDS_ROOMCONNECTED), nHour, nMin - nHour*60, nSec - nMin*60, nMS-nSec*1000);
			
			lstrcpy(szTempStr, strUptime);
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			return TRUE;

		}
		///////////////////////////////////////////////////////
		// F5 Insert WinMX Uptime time at cursor
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F5){

			CString strUptime;
			TCHAR szTempStr[1024];

			DWORD nMS = GetTickCount() - g_nMXStartTic;
			int nSec = nMS / 1000;
			int nMin = nMS / 60000;
			int nHour = nMS / 3600000;

			strUptime.Format("Online: %02d:%02d:%02d.%03d", nHour, nMin - nHour*60, nSec - nMin*60, nMS-nSec*1000);
			
			lstrcpy(szTempStr, strUptime);
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			return TRUE;

		}
		///////////////////////////////////////////////////////
		// F6 Insert Local time
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F6){

			TCHAR szTempStr[1024];
			CString strOut;
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

				strOut = szTempStr;
			}
			strOut += CChatDlg::GetLocalTime();

			lstrcpy(szTempStr, strOut);
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			return TRUE;
		}
		///////////////////////////////////////////////////////
		// F7 Insert Winamp song
		///////////////////////////////////////////////////////
 		else if(uMsg == WM_KEYDOWN && wParam == VK_F7){

			TCHAR szTempStr[1024];
			CString strOut;

			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

				strOut = szTempStr;
			}

			strOut += CChatDlg::GetWinampSong();
			lstrcpy(szTempStr, strOut);
			
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			
			return TRUE;
		}
		///////////////////////////////////////////////////////
		// F8 Insert /me listens to <Winamp>
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F8){

			TCHAR szTempStr[1024];
			CString strOut;

			strOut = (CChatDlg::IsVideoPlaying() ? g_sSettings.GetVideoMsg() : g_sSettings.GetWinampMsg());
			
			CChatDlg::ReplaceVars(strOut);
			
			lstrcpy(szTempStr, strOut);
			::CallWindowProc(OldWndProc, hwnd, WM_SETTEXT,0, (LPARAM)szTempStr);
			::CallWindowProc(OldWndProc, hwnd, WM_KEYDOWN, VK_END, 0);
			return TRUE;
		}
		///////////////////////////////////////////////////////
		// RETURN: Add to history, manage Winamp commands, intercept /all
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_RETURN){
			
			TCHAR szTempStr[1024];
			CString strOut;
			if(::CallWindowProc(OldWndProc, hwnd, WM_GETTEXT, 1024, (LPARAM)szTempStr)){

				strOut = szTempStr;
				
				HWND hParent = ::GetParent(hwnd);
				char *buffer = new char[500];
				::GetWindowText(hParent, buffer, 500);
				strcat(buffer, g_strIdent);
				HWND hHAL = ::FindWindow(NULL, buffer);
				DWORD dwID = ::SendMessage(hHAL, UWM_GETID, 0, 0);

				for(int i = 0; i < g_Plugins.GetSize(); i++){

					g_Plugins[i]->OnInputHook(dwID, &strOut);
				}
				
				if(!strOut.IsEmpty() && g_sSettings.GetSaveHistory()){
					
					AddHistory(strOut);
					ControlWinamp(strOut);
				}
				if((strOut.Left(4) == "/all") && (strOut.GetLength() > 6)){

					strOut = strOut.Mid(5);
					strcpy(buffer, strOut);
					::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)buffer);
					theApp.InputMessage(strOut);

				}
				else if((strOut.Left(5) == "/exit") || (strOut.Left(5) == "/quit")){

					HWND hParent = ::GetParent(hwnd);
					//::SendMessage(hParent, WM_COMMAND, 1504, 1409163);
					//SubChatProc() WM_COMMAND wParam 5E0 lParam F0612
					//SubChatProc() WM_COMMAND wParam 8AC lParam 0
					::SendMessage(hParent, WM_COMMAND, 0x8AC, 0);
					delete buffer; buffer = NULL;
					return TRUE;
				}
				delete buffer; buffer = NULL;
			}
		}
		///////////////////////////////////////////////////////
		// F12: Show Configuration dialog
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_F12){

			HWND hWnd = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
			if(hWnd != NULL){

				::ShowWindow(hWnd, SW_SHOW);
			}
			return TRUE;
			
		}
		///////////////////////////////////////////////////////
		// Esc = clear
		///////////////////////////////////////////////////////
		else if(uMsg == WM_KEYDOWN && wParam == VK_ESCAPE){

			::SetWindowText(hwnd, "");
			return TRUE;
		}

	}
		
	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}


///////////////
BOOL CChatDlg::SubclassList(HWND hWnd)
{
	
	ENTRY_TRACE("CChatDlg::SubclassList(hwnd=%X)", hWnd);

	WNDPROC OldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubListProc);
	
	BOOL bReturn = FALSE;
	if(OldWndProc && (LONG)OldWndProc != (LONG)SubListProc){

		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)OldWndProc);
		bReturn = TRUE;
	}

	EXIT_TRACE("CChatDlg::SubclassList() return=%d", bReturn);
	return bReturn;
}

char buffer[100];

LRESULT CALLBACK CChatDlg::SubListProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc)return FALSE;

	if((uMsg == WM_RBUTTONDOWN)) {

		TRACE("SubListProc() WM_RBUTTONDOWN wParam %X lParam %X\n", wParam, lParam);
		return ::CallWindowProc(OldWndProc, hwnd, WM_RBUTTONDOWN, wParam, lParam);
		/*
		BOOL bReturn = 1; 
		if(ListView_GetSelectedCount(hwnd) == 0){

			return bReturn;
		}
		
		CMenu *popup = new CMenu;
		CMenu *hSub = new CMenu;
		CMenu *hRCMS = new CMenu;

		if(popup->CreatePopupMenu() && hSub->CreateMenu() && hRCMS->CreateMenu()){

			int nServer = SERVER_RCMS;
			if(!g_mServers.Lookup(::GetParent(hwnd), nServer)){

				nServer = SERVER_RCMS;
			}

			POINT curPos;
			::GetCursorPos(&curPos);
			
			hSub->AppendMenu(MF_STRING, 2000, g_i18n.GetString(IDS_KICK));
			hSub->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hSub->AppendMenu(MF_STRING, 2002, g_i18n.GetString(IDS_KICKBAN));
			hSub->AppendMenu(MF_STRING, 2003, g_i18n.GetString(IDS_BAN));
			hSub->AppendMenu(MF_STRING, 2004, g_i18n.GetString(IDS_UNBAN));
			hSub->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);

			hRCMS->AppendMenu(MF_STRING, 10209, g_i18n.GetString(IDS_SENDMSG));
			hRCMS->AppendMenu(MF_STRING, 10220, g_i18n.GetString(IDS_READMSG));
			hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);

			switch(nServer){

			case SERVER_WINMX353:
			case SERVER_MCMA:
				hRCMS->AppendMenu(MF_STRING, 10211, g_i18n.GetString(IDS_RCMSKICKBAN5)); // kickban 5
				break;
			case SERVER_MCS:
				break;
			default:
				hRCMS->AppendMenu(MF_STRING, 10211, g_i18n.GetString(IDS_RCMSREDIRECT));
				break;
			}
			hRCMS->AppendMenu(MF_STRING, 10212, g_i18n.GetString(IDS_RCMSKICK));
			hRCMS->AppendMenu(MF_STRING, 10213, g_i18n.GetString(IDS_RCMSKICKBAN));
			hRCMS->AppendMenu(MF_STRING, 10214, g_i18n.GetString(IDS_RCMSBAN));
			hRCMS->AppendMenu(MF_STRING, 10215, g_i18n.GetString(IDS_RCMSUNBAN));
			hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			
		
			switch(nServer){

			case SERVER_WINMX353:
			case SERVER_ROSE:
			case SERVER_MCMA:
				hRCMS->AppendMenu(MF_STRING, 10216, g_i18n.GetString(IDS_RCMSDEVOICE)); // devoice
				hRCMS->AppendMenu(MF_STRING, 10217, g_i18n.GetString(IDS_RCMSVOICE));   // voice
				break;
			case SERVER_MCS:
				break;
			default:
				hRCMS->AppendMenu(MF_STRING, 10216, g_i18n.GetString(IDS_RCMSPRINTSTAT)); // devoice
				hRCMS->AppendMenu(MF_STRING, 10217, g_i18n.GetString(IDS_RCMSPRINTIP));   // voice
				break;
			}

			hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hRCMS->AppendMenu(MF_STRING, 10218, g_i18n.GetString(IDS_RCMSADDADMIN));  
			hRCMS->AppendMenu(MF_STRING, 10219, g_i18n.GetString(IDS_RCMSREMOVEADMIN));
			hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hRCMS->AppendMenu(MF_STRING, 10221, g_i18n.GetString(IDS_CUSTOMIZE));
			hRCMS->SetDefaultItem(10209);

			popup->AppendMenu(MF_STRING, 2100, g_i18n.GetString(IDS_SENDMSG));
			popup->AppendMenu(MF_STRING, 2101, g_i18n.GetString(IDS_CHAT));
			popup->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			popup->AppendMenu(MF_STRING, 2102, g_i18n.GetString(IDS_IGNORE));
			popup->AppendMenu(MF_STRING, 2103, g_i18n.GetString(IDS_UNIGNORE));
			popup->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			popup->AppendMenu(MF_STRING, 2104, g_i18n.GetString(IDS_WHOIS));
			popup->AppendMenu(MF_STRING, 10210, g_i18n.GetString(IDS_COPYUSERNAME));
			popup->AppendMenu(MF_STRING, 10222, g_i18n.GetString(IDS_COPYIP));
			popup->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			popup->AppendMenu(MF_STRING, 2105, g_i18n.GetString(IDS_BROWSE));
			popup->AppendMenu(MF_STRING, 2106, g_i18n.GetString(IDS_HOTLISTADD));
			popup->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			popup->AppendMenu(MF_POPUP, (UINT)hSub->m_hMenu, g_i18n.GetString(IDS_ADVANCED));
			popup->AppendMenu(MF_POPUP, (UINT)hRCMS->m_hMenu, g_i18n.GetString(IDS_RCMSCMD));

			for(int i = 0; i < g_Plugins.GetSize(); i++){

				g_Plugins[i]->m_pPlugin->OnPrepareMenu(0);
				if(g_Plugins[i]->m_pPlugin->hUserMenu <= 0) continue;
				popup->AppendMenu(MF_POPUP, (UINT)g_Plugins[i]->m_pPlugin->hUserMenu, g_Plugins[i]->m_strShortName);
			}
			popup->SetDefaultItem(2100);
			popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, curPos.x, curPos.y, CWnd::FromHandle(::GetParent(hwnd)), NULL);
			popup->DestroyMenu();
			hSub->DestroyMenu();
			hRCMS->DestroyMenu();
		}
		delete popup;
		delete hSub;
		delete hRCMS;
		hSub  = NULL;
		hRCMS = NULL;
		popup = NULL;
		
		return bReturn; */
		
	}
	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}

BOOL CChatDlg::SubclassChat(HWND hWnd)
{

	ENTRY_TRACE("CChatDlg::SublcassChat(hwnd=%X)", hWnd);
	WNDPROC OldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubChatProc);
	
	BOOL bReturn = FALSE;
	if(OldWndProc && (LONG)OldWndProc != (LONG)SubChatProc){

		LTRACE("Successfully subclassed chat");
		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)OldWndProc);
		
		bReturn = TRUE;
	}

	EXIT_TRACE("CChatDlg::SubclassChat()");
	return bReturn;
}

LRESULT CALLBACK CChatDlg::SubChatProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc) return FALSE;

	/*if(uMsg == WM_KEYDOWN){

		TRACE("Keydown: 0x%X %d %d\n", uMsg, wParam, lParam);
	}
	if(uMsg == WM_KEYUP){

		TRACE("Keyup: 0x%X %d %d\n", uMsg, wParam, lParam);
	}*/
	if((uMsg == WM_COMMAND) && (wParam >= 22002) && (wParam <= 26010)){

		LTRACE("SubChatProc() WM_COMMAND wParam %X lParam %X", wParam, lParam);
		CString strTmp, strTmp2;
		char chat[500];
		HWND hTmp;
		int i = 0;
		switch(wParam){
	
		case 22002:  // copy room name

			::GetWindowText(::GetParent(hwnd), chat, 500);
			strTmp = chat;
			strTmp = strTmp.Left(strTmp.Find(" on WinMX"));
			CClipboard::SetText((char*)(LPCTSTR)strTmp);
			return TRUE;

		case 22003:  // select all
			//::CallWindowProcA(OldWndProc, hwnd, WM_SETFOCUS, 0, 0);
			//return ::CallWindowProc(OldWndProc, hwnd, EM_SETSEL, 0, -1);
			::GetWindowText(::GetParent(hwnd), chat, 500);
			strTmp = chat;
			strTmp = strTmp.Left(strTmp.Find(" on WinMX"));
			theApp.ShowAdminWindow(strTmp);
			return TRUE;

		case 22004: // copy selected text
			//Keydown: 0x100 17 1075642369
			//Keydown: 0x100 67 3014657
			//Keyup: 0x101 67 -1070727167
			//Keyup: 0x101 17 -1071841279
			::CallWindowProcA(OldWndProc, hwnd, WM_KEYDOWN, 17, 1075642369);
			::CallWindowProcA(OldWndProc, hwnd, WM_KEYDOWN, 67, 3014657);
			::CallWindowProcA(OldWndProc, hwnd, WM_KEYUP, 67, -1070727167);
			
			return ::CallWindowProcA(OldWndProc, hwnd, WM_KEYUP, 17, -1071841279);//::CallWindowProc(OldWndProc, hwnd, WM_COPY, 0, 0);

		case 22005: // send selected text to browser
			::CallWindowProc(OldWndProc, hwnd, WM_COPY, 0, 0);
			
			strTmp = CClipboard::GetText();
			strTmp.TrimLeft(); strTmp.TrimRight();
			if(strTmp.IsEmpty()) return TRUE;

			if((strTmp.Find("http://", 0) < 0) && (strTmp.Find("ftp://", 0) < 0) && (strTmp.Find("cast://", 0) < 0)){

				strTmp.Insert(0, "http://");
			}
			ShellExecute(0, "open", strTmp, 0, 0, SW_SHOW);
			return TRUE;
		case 22006: // search selection on google
			::CallWindowProc(OldWndProc, hwnd, WM_COPY, 0, 0);

			strTmp = CClipboard::GetText();
			strTmp.TrimLeft();
			strTmp.TrimRight();
			strTmp.Replace(" ", "%20");
			strTmp.Remove('\r'); strTmp.Remove('\n');
			strTmp2.Format("http://www.google.com/search?hl=en&ie=UTF-8&oe=UTF-8&q=%s&btnG=Google+Search", strTmp);
			ShellExecute(0, "open", strTmp2, 0, 0, SW_SHOW);
			return TRUE;

		case 22007: // quote

			::CallWindowProc(OldWndProc, hwnd, WM_COPY, 0, 0);
			strTmp = CClipboard::GetText();
			strTmp.TrimLeft(); strTmp.TrimRight();
			strTmp.Insert(0, "\"");
			strTmp += "\"";

			 ::GetWindowText(::GetParent(hwnd), chat, 500);
			 strTmp2 = chat;

			 theApp.InputMessage(strTmp, strTmp2, FALSE);
			return TRUE;
		case 22008: // Clear screen
			theApp.InputMessage(FALSE, "/clr", (UINT)::GetParent(hwnd));
			return TRUE;
		case 23000: // Down/UP stats
			::GetWindowText(g_hMXWindow, chat, 500);
			strTmp = chat;
			theApp.InputMessage(FALSE, strTmp, (UINT)::GetParent(hwnd));
			break;
		case 23001: // Localtime
			 theApp.InputMessage(FALSE, g_i18n.GetString(IDS_LOCALTIMECH) + " " + GetLocalTime(), (UINT)::GetParent(hwnd));
			break;
		case 23002: // LAG in channel
			// changed for RCMS compatible mode
			for(i = 0; i < g_Plugins.GetSize(); i++){

				if(g_Plugins[i]->m_pPlugin->nID == 0x0004)
					g_Plugins[i]->OnMenuCommand(23002, (DWORD)::GetParent(hwnd), 0);
			}
			break;
		case 23003: // Winamp Song
			strTmp = (CChatDlg::IsVideoPlaying() ? g_sSettings.GetVideoMsg() : g_sSettings.GetWinampMsg());
			CChatDlg::ReplaceVars(strTmp);
			 theApp.InputMessage(FALSE, strTmp,  (UINT)::GetParent(hwnd));
			break;
		case 23004: // Operating System and Uptime
			 theApp.InputMessage(FALSE, GetMyOSVersion() + " " + GetUpTime(),  (UINT)::GetParent(hwnd));
			break;
		case 23005: // System Info ;)
			theApp.InputMessage(FALSE, GetMySystemInfo(),  (UINT)::GetParent(hwnd));
			break;
		case 23006: // server info
			theApp.InputServerInfo((UINT)::GetParent(hwnd));
			break; 
		case 23007:// system uptime only
			theApp.InputMessage(FALSE, GetUpTime(),  (UINT)::GetParent(hwnd));
			break;
		case 24000: // l33t
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 0, (LPARAM)"");
			break;
		case 24001: // bubbles
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 1,(LPARAM)"");
			break;
		case 24002: // box
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 2, (LPARAM)"");
			break;
		case 24003: // 3d msg
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 3, (LPARAM)"");
			break;
		case 24004: // 3d action
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 4, (LPARAM)"");
			break;
		case 24005: // colored bubbles
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 6, (LPARAM)"");
			break;
		case 24006: // wild colors
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 7, (LPARAM)"");
			break;
		case 25000: // away back
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
			::SendMessage(hTmp, UWM_AWAY, 0, 0);
			break;
		case 25002:
		case 25003:
		case 25004:
		case 25005:
		case 25006:
		case 25007:
		case 25008:
		case 25009:
		case 25010:
		case 25011:
		case 25012:
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
			::SendMessage(hTmp, UWM_AWAY, 2, wParam-25002);
			break;
		case 25013:
			hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
			::SendMessage(hTmp, UWM_SETID, (WPARAM)::GetParent(hwnd), 0);
			::SendMessage(hTmp, UWM_SPECIAL, 5, (LPARAM)"");
		case ID_CSTM_MENUBASE2-1:
			ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\menu.ini", 0, 0, SW_SHOW);
			break;

		} // end switch

		if((wParam >= ID_CSTM_MENUBASE) && (wParam < ID_CSTM_MENUBASE2 + 900)){

			int nCmd = wParam - ID_CSTM_MENUBASE2;

			if(nCmd < 0) return 1;

			CIni ini;
			ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\menu.ini");

			CString strItem, strTmp, strCmd;
			strItem.Format("Chat_%d", nCmd);

			for(int j = 0; TRUE; j++){

				strTmp.Format("Cmd_%d", j);
				strCmd = ini.GetValue(strItem, strTmp, "");

				if(strCmd.IsEmpty()) break;
				
				ReplaceVars(strCmd);
				theApp.InputMessage(TRUE, strCmd, (UINT)::GetParent(hwnd));
				Sleep(10);
			}
		}
	}
	else if((uMsg == WM_COMMAND) && (wParam >= 12000) && (wParam <= 1000000)){

		LTRACE("SubChatProc() WM_COMMAND wParam %X lParam %X", wParam, lParam);
		for(int i = 0; i < g_Plugins.GetSize(); i++){

			g_Plugins[i]->OnMenuCommand(wParam, (DWORD)::GetParent(hwnd), 0);
		}
	}
	else if(uMsg == WM_RBUTTONUP){

		LTRACE("SubChatProc() WM_RBUTTONUP wParam %X lParam %X", wParam, lParam);
		DWORD dwStart = 0, dwEnd = 0;
		::CallWindowProc(OldWndProc, hwnd, EM_GETSEL, (WPARAM)&dwStart, (WPARAM)&dwEnd);

		CString strTmp;
		char chat[500];

		CMenu *hMenu = new CMenu;
		CMenu *hDisplay = new CMenu;
		CMenu *hHacker  = new CMenu;
		CMenu *hAway    = new CMenu;
		CMenu *hCustom  = new CMenu;

		if(hMenu->CreatePopupMenu() && hCustom->CreatePopupMenu() && hDisplay->CreatePopupMenu() && hHacker->CreatePopupMenu() && hAway->CreatePopupMenu()){

			POINT curPos;
			::GetCursorPos(&curPos);


			CIni ini;
			ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\menu.ini");
			int nServerType = SERVER_RCMS;
			if(!g_mServers.Lookup(::GetParent(hwnd), nServerType)){

				nServerType = SERVER_RCMS;
			}
			CString strLabel, strTmp;
			for(int i = 1; i; i++){

				strTmp.Format("Chat_%d", i);

				// check if this item is to be displayed with this server
				strLabel = ini.GetValue(strTmp, "Server", "all");
				strLabel.MakeLower();
				
				if((nServerType == SERVER_RCMS) && (strLabel.Find("rcms") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_UNKNOWN) && (strLabel.Find("winmx331") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_WINMX353) && (strLabel.Find("winmx353") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_MCS) && (strLabel.Find("mxtools") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_WCS) && (strLabel.Find("wcs") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_OUKA) && (strLabel.Find("ouka") < 0) && (strLabel != "all")) continue;
				if((nServerType == SERVER_ROSE) && (strLabel.Find("rose") < 0) && (strLabel != "all")) continue;

				strLabel = ini.GetValue(strTmp, "Label", "");

				if(strLabel.IsEmpty()) break;
				
				if(strLabel.CompareNoCase("separator") == 0){

					hCustom->InsertMenu(i, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
				}
				else{

					hCustom->InsertMenu(i, MF_STRING|MF_BYPOSITION, ID_CSTM_MENUBASE2 + i, (LPCTSTR)strLabel);
				}
			}
			hCustom->InsertMenu(i++, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
			hCustom->InsertMenu(i, MF_STRING|MF_BYPOSITION, ID_CSTM_MENUBASE2-1, g_i18n.GetString(IDS_CUSTOMIZE));

			hDisplay->AppendMenu(MF_STRING, 23000, g_i18n.GetString(IDS_DOWNUP));
			hDisplay->AppendMenu(MF_STRING, 23001, g_i18n.GetString(IDS_LOCALTIME));
			hDisplay->AppendMenu(MF_STRING, 23002, g_i18n.GetString(IDS_LAG));
			hDisplay->AppendMenu(MF_STRING, 23003, g_i18n.GetString(IDS_WINAMP));
			hDisplay->AppendMenu(MF_STRING, 23007, g_i18n.GetString(IDS_UPTIME));
			hDisplay->AppendMenu(MF_STRING, 23004, g_i18n.GetString(IDS_OS));
			hDisplay->AppendMenu(MF_STRING, 23005, g_i18n.GetString(IDS_SYSINFO));
			hDisplay->AppendMenu(MF_STRING, 23006, g_i18n.GetString(IDS_CHATINFO));
			
			// Text stuff
			hHacker->AppendMenu(MF_STRING, 24000, g_i18n.GetString(IDS_HACKER));
			hHacker->AppendMenu(MF_STRING, 24001, g_i18n.GetString(IDS_BUBBLES));
			hHacker->AppendMenu(MF_STRING, 24005, g_i18n.GetString(IDS_BUBBLESCOLOR));
			hHacker->AppendMenu(MF_STRING, 24002, g_i18n.GetString(IDS_BOX));
			hHacker->AppendMenu(MF_STRING, 24003, g_i18n.GetString(IDS_3DMSG));
			hHacker->AppendMenu(MF_STRING, 24004, g_i18n.GetString(IDS_3DACTION));
			hHacker->AppendMenu(MF_STRING, 24006, g_i18n.GetString(IDS_COLOREDTEXT));
			if(g_bAway){

				hAway->AppendMenu(MF_STRING, 25000, g_i18n.GetString(IDS_SETBACK));
			}
			else{
	
				hAway->AppendMenu(MF_STRING, 25002, g_i18n.GetString(IDS_ZZZZZ));
				hAway->AppendMenu(MF_STRING, 25003, g_i18n.GetString(IDS_AWAYWORK));
				hAway->AppendMenu(MF_STRING, 25004, g_i18n.GetString(IDS_AWAYSLEEP));
				hAway->AppendMenu(MF_STRING, 25005, g_i18n.GetString(IDS_AWAYAIR));
				hAway->AppendMenu(MF_STRING, 25006, g_i18n.GetString(IDS_AWAYBATH));
				hAway->AppendMenu(MF_STRING, 25007, g_i18n.GetString(IDS_AWAYSMOKE));
				hAway->AppendMenu(MF_STRING, 25008, g_i18n.GetString(IDS_AWAYFOOD));
				hAway->AppendMenu(MF_STRING, 25009, g_i18n.GetString(IDS_AWAYBEER));
				hAway->AppendMenu(MF_STRING, 25010, g_i18n.GetString(IDS_AWAYTV));
				hAway->AppendMenu(MF_STRING, 25011, g_i18n.GetString(IDS_AWAYSTORE));
				hAway->AppendMenu(MF_STRING, 25012, g_i18n.GetString(IDS_AWAYNONE));
				hAway->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
				hAway->AppendMenu(MF_STRING, 25013, g_i18n.GetString(IDS_AWAYCUSTOM));
			}

			
			::GetWindowText(::GetParent(hwnd), chat, 500);
			strTmp = chat;
			strTmp = strTmp.Left(strTmp.ReverseFind('_'));
			strTmp.Replace("..", "-");

			hMenu->AppendMenu(MF_STRING|MF_DISABLED|MF_GRAYED, 0, strTmp);
			hMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hMenu->AppendMenu(MF_POPUP, (UINT)hDisplay->m_hMenu, g_i18n.GetString(IDS_DISPLAYCHANNEL));
			hMenu->AppendMenu(MF_POPUP, (UINT)hHacker->m_hMenu, g_i18n.GetString(IDS_TEXTTRICKS));
			hMenu->AppendMenu(MF_POPUP, (UINT)hAway->m_hMenu, g_i18n.GetString(IDS_AWAYCONTROL));
			hMenu->AppendMenu(MF_POPUP, (UINT)hCustom->m_hMenu, g_i18n.GetString(IDS_CUSTOM));
			hMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hMenu->AppendMenu(MF_STRING, 22002, g_i18n.GetString(IDS_COPYROOM));

			if(g_Plugins.GetSize()) 
				hMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
	
			for(i = 0; i < g_Plugins.GetSize(); i++){

				g_Plugins[i]->m_pPlugin->OnPrepareMenu(2);
				if(g_Plugins[i]->m_pPlugin->hChatMenu <= 0) continue;
				hMenu->AppendMenu(MF_POPUP, (UINT)g_Plugins[i]->m_pPlugin->hChatMenu, g_Plugins[i]->m_strShortName);
			}
			
			hMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		    hMenu->AppendMenu(MF_STRING, 22003, g_i18n.GetString(IDS_SHOWADMINWND));
			hMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			hMenu->AppendMenu(MF_STRING, 22008, g_i18n.GetString(IDS_CLEARSCREEN));

			hMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, curPos.x, curPos.y, CWnd::FromHandle(hwnd), NULL);
			hMenu->DestroyMenu();
		}
		delete hAway;
		hAway = 0;
		delete hHacker;
		hHacker = 0;
		delete hDisplay;
		hDisplay = 0;
		delete hMenu;
		hMenu = 0;
		delete hCustom;
		hCustom = 0;
		return TRUE;
	}
	else if(uMsg == WM_MOUSEWHEEL){

		//LTRACE("%d %d %d"), uMsg, wParam, lParam);
		//LTRACE("Mousewheel"));
		// Mousewheel down 522 -7864320 29229593
		// Mousewheel up   522 7864320 32637418
		if((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) > 0)){

			
			return ::CallWindowProc(OldWndProc, hwnd, 256, 33, 21561345);
		}
		else if((uMsg == WM_MOUSEWHEEL && ((short)HIWORD(wParam)) < 0)){

			return ::CallWindowProc(OldWndProc, hwnd, 256, 34, 22085633);
		}
	}
	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK CChatDlg::FindChatControl(HWND hWnd, LPARAM lParam)
{

	if(::GetDlgCtrlID(hWnd) == 1500){

		HWND hParent = ::GetParent(hWnd);
		char *buffer = new char[1000];
		
		::GetWindowText(hParent, buffer, 1000);
		CString strTmp = buffer;
		::GetClassName(hWnd, buffer, 1000);
		CString strClass = buffer;

		delete buffer;
		buffer = NULL;
		
		if((strTmp == ((CChatDlg*)lParam)->m_strTitle)){

			LTRACE("ChatControl found");
			((CChatDlg*)lParam)->m_hRich = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

LRESULT CALLBACK CChatDlg::SubDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc)return FALSE;


	if(uMsg == WM_DESTROY){

		LTRACE("SubChatProc() WM_DESTROY wParam %X lParam %X", wParam, lParam);
		// Chat window is about to be closed
		char chat[200];
		::GetWindowText(hwnd, chat, 199);

		HWND hChat = ::FindWindow(NULL, chat);
		//::KillTimer(hChat, TIMER_CHECKTITLE);

		strcat((char*)chat, g_strIdent);

		theApp.RemoveHook(chat);
	}
	else if((uMsg >= WM_USER) && (uMsg < WM_APP)){


		TRACE("SubChatProc() WM_USER 0x%X (%d) wParam 0x%X (%d) lParam 0x%X (%d)\n", uMsg, uMsg, wParam,wParam,lParam,lParam);
	}
	else if(uMsg == WM_COMMAND){

		// Hex/Dec
		// SendMsg wParam = 834 lParam = 0
		// Chat wParam = 835 lParam = 0
		// Ignore wParam = 836 lParam = 0
		// Unignore wParam = 837 lParam = 0
		// Whois wParam = 838 lParam = 0
		// Browse wParam = 839 lParam = 0
		// Hotlist w = 83A
		TRACE("SubChatProc() WM_COMMAND wParam %X (%d) lParam %X (x)\n", wParam,wParam,lParam,lParam);
		if(((wParam >= 2100) && (wParam <= 2106)) || ((wParam >= 2000) && (wParam <= 2004)) 
			|| ((wParam >= 12000) && (wParam < 1000000))){

			//CallOrigWndProc(hwnd, WM_USER+5,  1241024, 0x12EFB4);
			CallOrigWndProc(hwnd, WM_COMMAND, 2104, 0);

			char* buffer= new char[500];
			CString strUser;
			EnumWindows((wParam == 2104 ? FindWhoisNotClose : FindWhois), (LPARAM)buffer);
			if(buffer != NULL){

				strUser = buffer;
				strUser = strUser.Left(strUser.Find(" on WinMX"));				
			}
			
			::GetWindowText(hwnd, buffer, 500);
			strcat(buffer, g_strIdent);
			HWND hHal = ::FindWindow(0, buffer);

			DWORD dwID = ::SendMessage(hHal, UWM_GETID, 0, 0);

			for(int i = 0; i  < g_Plugins.GetSize(); i++){

				g_Plugins[i]->OnMenuCommand(wParam, dwID, strUser);
			}

			delete buffer;
			buffer = NULL;
			if(wParam == 2102 || wParam == 2103){

				if(g_sSettings.GetSoundFX()){

					PlaySound(g_sSettings.GetWorkingDir() + (wParam == 2102 ? "\\data\\ignored.dat" : "\\data\\unignored.dat"), 0, SND_ASYNC|SND_FILENAME);
				}
			}
			
			return (wParam == 2104 ? TRUE : ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam));

		}
		else if((wParam >= 10209) && (wParam <= 10222)){

			CallOrigWndProc(hwnd, WM_COMMAND, 2104, 0);
			char* buffer= new char[500];
			CString strUser, strCmd, strChat;
			EnumWindows(FindWhois, (LPARAM)buffer);
			if(buffer != NULL){

				strUser = buffer;
				strUser = strUser.Left(strUser.Find(" on WinMX"));				
			}
			
			::GetWindowText(hwnd, buffer, 500);
			strChat = buffer;
			delete buffer;
			buffer = NULL;
			HWND hTmp;

			switch(wParam){

			case 10209: // Send Private message
				hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
				char test[200];
				sprintf((char*)test, "#private %s #message ", strUser);
				::SendMessage(hTmp, UWM_SETID, (WPARAM)hwnd, 0);
				::SendMessage(hTmp, UWM_SPECIAL, (WPARAM)-1, (LPARAM)test);
				break;
			case 10210: // copy username
				CClipboard::SetText((char*)(LPCSTR)strUser, NULL);
				break;;
			case 10222: // copy ip of user
				hTmp = ::FindWindow(NULL, strChat + g_strIdent);
				::SendMessage(hTmp, UWM_COPYIP, wParam, (LPARAM)(LPCTSTR)strUser);
				break;
			case 10211:
			case 10212:
			case 10213:
			case 10214:
			case 10215:
			case 10216:
			case 10217:
			case 10218:
			case 10219:
			case 10220:
				hTmp = ::FindWindow(NULL, strChat + g_strIdent);
				::SendMessage(hTmp, UWM_ONRCMSMENU, wParam, (LPARAM)(LPCTSTR)strUser);
				break;
			case 10221:
				hTmp = ::FindWindow(NULL, strChat + g_strIdent);
				::SendMessage(hTmp, UWM_ONRCMSEDIT, 0, 0);
				break;
			}

			return TRUE;
		}
	}
	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}

BOOL CChatDlg::SubclassDlg(HWND hWnd)
{

	ENTRY_TRACE("CChatDlg::SubclassDlg(hwnd=%X) return=%d", hWnd);
	WNDPROC OldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubDlgProc);

	BOOL bReturn = FALSE;
	if(OldWndProc && (LONG)OldWndProc != (LONG)SubDlgProc){

		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)OldWndProc);
		bReturn = TRUE;
	}
	
	EXIT_TRACE("CChatDlg::SubclassDlg() return=%d", bReturn);
	return bReturn;
}

void CChatDlg::RemoveHook()
{

	ENTRY_TRACE("CChatDlg::RemoveHook()");
	if(m_bIsHooked){

		WNDPROC OldWndProc = NULL;
		if(!g_bRCMS){

			OldWndProc = (WNDPROC)::GetWindowLong(m_hEdit, GWL_USERDATA);
			::SetWindowLong(m_hEdit, GWL_WNDPROC, (LONG)OldWndProc);
		}

		OldWndProc = (WNDPROC)::GetWindowLong(m_hList, GWL_USERDATA);
		::SetWindowLong(m_hList, GWL_WNDPROC, (LONG)OldWndProc);

		OldWndProc = (WNDPROC)::GetWindowLong(m_hRich, GWL_USERDATA);
		::SetWindowLong(m_hRich, GWL_WNDPROC, (LONG)OldWndProc);

		OldWndProc = (WNDPROC)::GetWindowLong(m_hChat, GWL_USERDATA);
		::SetWindowLong(m_hChat, GWL_WNDPROC, (LONG)OldWndProc);

		m_bIsHooked = FALSE;
	}
	EXIT_TRACE("CChatDlg::RemoveHook()");
	g_mServers.RemoveKey(m_hChat);
}


LRESULT CChatDlg::OnGetUpTime(WPARAM wParam, LPARAM lParam)
{

	return m_nStartTic;
}

LRESULT CChatDlg::OnGetRCMS(WPARAM wParam, LPARAM lParam)
{

	return m_bRCMS;
}

LRESULT CChatDlg::OnGetID(WPARAM wParam, LPARAM lParam)
{

	return m_dwID;
}

int CChatDlg::CallOrigWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WNDPROC OldWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc) return FALSE;

	return ::CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}



CString CChatDlg::GetWinampSong()
{

	CString strWinamp, strOut;
	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	if(hwndWinamp != NULL){

		::SendMessage(hwndWinamp, WM_WA_IPC,0,IPC_UPDTITLE);
		TCHAR *buff = new TCHAR[250];
		::GetWindowText(hwndWinamp, buff, 250);
		strWinamp = buff;
		strWinamp = strWinamp.Mid(strWinamp.Find(" ", 0)+1, strWinamp.Find(" - Winamp") - strWinamp.Find(" ", 0)-1);
		delete buff;
		buff = NULL;
	}
	else{

		strWinamp = g_i18n.GetString(IDS_WINAMPNOT);
	}

	return strWinamp;
}


BOOL CChatDlg::IsVideoPlaying()
{

	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	BOOL bReturn = FALSE;
	switch(::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_IS_PLAYING_VIDEO)){

	case 0:
	case 1:
		bReturn = FALSE;
		break;
	default:
		bReturn = TRUE;
	}

	return bReturn;

}

void CChatDlg::ReplaceVars(CString &strOut)
{

	if(strOut.Find("%", 0) < 0 ) return;

	CString strArtist, strSong, strVersion, strPlayTime, strTotalTime, strRemTime, strSampleRate, strBitrate, strNumChannels, strStatus = "not running";
	
	strOut.Replace(_T("%TIME%"), GetLocalTime());
	strOut.Replace(_T("%DATE%"), GetDate());
	
	CString strTmp = CChatDlg::GetWinampSong();


	if(strOut.Find("%WA-", 0) >= 0){


		CTokenizer token(strTmp, "-");
		token.Next(strArtist);
		strSong = token.Tail();
		strArtist.TrimRight();
		strSong.TrimLeft();
		
		HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);

		if(hwndWinamp != NULL){


			strVersion.Format("%x", ::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETVERSION));
			strVersion.SetAt(1, '.');

			int nTotal = 0, nRem = 0, nEla = 0;
			nTotal = ::SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
			strTotalTime.Format("%02d:%02d", nTotal/60, nTotal%60);

			nEla= ::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) / 1000;
			strPlayTime.Format("%02d:%02d", nEla/60, nEla%60);
			
			nRem = nTotal - nEla;
			strRemTime.Format("%02d:%02d", nRem/60, nRem%60);

			strSampleRate.Format("%d", ::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETINFO));
			strBitrate.Format("%d", ::SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETINFO));
			strNumChannels = (::SendMessage(hwndWinamp, WM_WA_IPC, 2, IPC_GETINFO) == 1 ? "Mono" : "Stereo");

			switch(::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_ISPLAYING)){

			case 1: strStatus = "playing";
				break;
			case 3: strStatus = "paused";
				break;
			default: strStatus = "stopped";
			}

		}
		strOut.Replace(_T("%WA-ARTIST%"), strArtist);
		strOut.Replace(_T("%WA-SONG%"), strSong);
		strOut.Replace(_T("%WA-VERSION%"), strVersion);
		strOut.Replace(_T("%WA-ELATIME%"), strPlayTime);
		strOut.Replace(_T("%WA-REMTIME%"), strRemTime);
		strOut.Replace(_T("%WA-TOTALTIME%"), strTotalTime);
		strOut.Replace(_T("%WA-SAMPLERATE%"), strSampleRate);
		strOut.Replace(_T("%WA-BITRATE%"), strBitrate);
		strOut.Replace(_T("%WA-CHANNELS%"), strNumChannels);
		strOut.Replace(_T("%WA-STATUS%"), strStatus);
	}	

}

CString CChatDlg::GetLocalTime()
{

	CString strTime;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	strTime.Format("%02d:%02d.%02d", time.wHour, time.wMinute, time.wSecond);

	return strTime;
}

CString CChatDlg::GetMyOSVersion()
{

	CString strVersion;

	strVersion.Format("%s Build %d", g_siInfo.GetOSType(), g_siInfo.GetBuildNumber());

	return strVersion;
}

CString CChatDlg::GetUpTime()
{

	CString strUp;
	DWORD nMS = GetTickCount();
	int nSec = nMS / 1000;
	int nMin = nMS / 60000;
	int nHour = nMS / 3600000;
	strUp.Format(g_i18n.GetString(IDS_SYSUPTIME),
		nHour/24, nHour%24, nMin - nHour*60, nSec - nMin*60, nMS-nSec*1000);

	return strUp;
}

CString CChatDlg::FillStars(int nCount)
{

	CString strReturn;
	while(nCount){

		strReturn += "*";
		nCount--;
	}

	return strReturn;
}

BOOL CChatDlg::FilterMessage(CString &strNick, CString &strMessage)
{

	// if user is on the trusted list dont filter the message
	for(int n = 0; n < g_strTrusted.GetSize(); n++){

		if(strNick.Find(g_strTrusted[n], 0) >= 0){

			return FALSE;
		}
	}

	// filter offensive words
	if(g_sSettings.GetFilterNormal() || g_sSettings.GetFilterXtrem()){

		for(int i = 0; i < g_strFilter.GetSize(); i++){


			if(strMessage.Find(g_strFilter[i], 0) >= 0){

				if(g_sSettings.GetFilterNormal()){

					strMessage.Replace(g_strFilter[i], FillStars(g_strFilter[i].GetLength()));
				}
				else{

					strMessage.Empty();
					strNick.Empty();
					return TRUE;
				}
			}
		}
	}

	// block #adminmessage
	if(g_sSettings.GetBlockAdmin()){

		if(strMessage.Find("[Admin] ", 0) == 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	// block #private
	if(g_sSettings.GetBlockPrivate()){

		if(strMessage.Find("[Private] ", 0) == 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	// block #ascii
	if(g_sSettings.GetBlockAscii()){

		if(strNick == " "){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}
	// block URL
	if(g_sSettings.GetBlockHttp()){

		if(strMessage.Find("http://", 0) >= 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	// block empty
	if(g_sSettings.GetBlockEmpty()){

		if((strMessage == " ") || (strMessage == " ")){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	return FALSE;
}

//DEL void CChatDlg::WriteEnterStuff()
//DEL {
//DEL 
//DEL /*	CHARFORMAT cf = g_sSettings.GetFMJoin();
//DEL 	CString text;
//DEL 	text.Format("-B-e-n-d-M-X---> %s\n\n", g_i18n.GetString(IDS_BENDMX1STLINE));
//DEL 	CHARRANGE cr;
//DEL 
//DEL 	cr.cpMax = 0;
//DEL 	cr.cpMin = 0;
//DEL 
//DEL 	CallOrigWndProc(m_hRich, EM_EXSETSEL, NULL, (LPARAM)&cr);
//DEL 	CallOrigWndProc(m_hRich, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
//DEL 	CallOrigWndProc(m_hRich, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)(LPCTSTR)text);
//DEL 
//DEL 	cf.dwMask = CFM_PROTECTED|CFM_UNDERLINE|CFM_BOLD|CFM_ITALIC|CFM_FACE|CFM_SIZE|CFM_CHARSET;
//DEL 	CallOrigWndProc(m_hRich, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);*/
//DEL }

CString CChatDlg::GetMySystemInfo(BOOL bRCMS)
{

	CString strInfo;
	strInfo.Format(
				g_i18n.GetString(IDS_SYSSTRING),
				g_siInfo.GetOSType(), g_siInfo.GetBuildNumber(),
				g_siInfo.GetNumProcessors(), g_siInfo.GetCPUSpeed(),
				g_siInfo.GetCPUIdentifier(), g_siInfo.GetCPUVendorIdentifier(), g_siInfo.GetCPUNameString()
			);


    return strInfo;
}

LRESULT CChatDlg::OnInputMessage(WPARAM wParam, LPARAM lParam)
{

	CString strOut = (LPCTSTR)lParam;
	InputMessage(strOut);
	return 0;
}

void CChatDlg::LoadRCMS()
{

	ENTRY_TRACE("CChatDlg::LoadRCMS()");
	CString strIniFile, strRoom;

	strRoom = m_strRoomShort;
	strRoom.Remove('?');
	strRoom.Remove(':');
	strRoom.Remove(',');
	strRoom.Remove('\\');
	strRoom.Remove('/');
	strRoom.Remove('<');
	strRoom.Remove('>');
	strRoom.Remove('\"');
	strRoom.Remove('*');
	strRoom.Remove('|');
	strRoom.Replace(1, '-');

	strIniFile.Format("%s\\%s.mnu", g_sSettings.GetWorkingDir(), strRoom);


	CStdioFile ini;
	CString strBuffer;
	m_aRCMSMenu.RemoveAll();

	TRY{

		if(ini.Open(strIniFile, CFile::modeRead|CFile::typeText|CFile::shareExclusive)){


			while(ini.ReadString(strBuffer)){

				if(!strBuffer.IsEmpty()){

					strBuffer.TrimLeft();
					strBuffer.TrimRight();
					m_aRCMSMenu.Add(strBuffer);
				}
			}
			ini.Close();
		}
		
	}
	CATCH(CFileException, e){

	}END_CATCH;

	if(m_aRCMSMenu.GetSize() == 0){

		if(m_nServerType == SERVER_WINMX353){

			m_aRCMSMenu.Add("/kickban %NAME% 5");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME% 255");
			m_aRCMSMenu.Add("/ban %NAME% 255");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/setuserlevel %NAME% 5");	  // remove voice
			m_aRCMSMenu.Add("/setuserlevel %NAME% 65");	  // give voice
			m_aRCMSMenu.Add("/setuserlevel %NAME% 200");  // add admin
			m_aRCMSMenu.Add("/setuserlevel %NAME% 65");	  // remove admin
			m_aRCMSMenu.Add("/level %NAME%");             // display userlevel
		}
		else if((m_nServerType == SERVER_MCS) || (m_nServerType == SERVER_WCS)){

			m_aRCMSMenu.Add("#UserCmd Redirect %NAME%");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME%");
			m_aRCMSMenu.Add("/ban %NAME%");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/stats %NAME%");
			m_aRCMSMenu.Add("#AdminCmd PrintIP %NAME%");
			m_aRCMSMenu.Add("/addadmin %NAME%");
			m_aRCMSMenu.Add("/removeadmin %NAME%");
			m_aRCMSMenu.Add("#UserCmd Readmsg %NAME%");
		}
		else if(m_nServerType == SERVER_ROSE){

			m_aRCMSMenu.Add("/exile %NAME%");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME%");
			m_aRCMSMenu.Add("/ban %NAME%");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/voice %NAME%");
			m_aRCMSMenu.Add("/muzzle %NAME%");
			m_aRCMSMenu.Add("/admin %NAME%");
			m_aRCMSMenu.Add("/remadmin %NAME%");
			m_aRCMSMenu.Add("/usermode %NAME%");
		}
		else{

			// Fill in defaults
			m_aRCMSMenu.Add("#UserCmd Redirect %NAME%");
			m_aRCMSMenu.Add("#UserCmd /kick %NAME%");
			m_aRCMSMenu.Add("#AdminCmd /kickban %NAME%");
			m_aRCMSMenu.Add("#AdminCmd /ban %NAME%");
			m_aRCMSMenu.Add("#UserCmd /unban %NAME%");
			m_aRCMSMenu.Add("#UserCmd PrintUserStat %NAME%");
			m_aRCMSMenu.Add("#AdminCmd PrintIP %NAME%");
			m_aRCMSMenu.Add("#AdminCmd AddAdmin %NAME%");
			m_aRCMSMenu.Add("#AdminCmd RemoveAdmin %NAME%");
			m_aRCMSMenu.Add("#UserCmd Readmsg %NAME%");
		}
	}

	EXIT_TRACE("CChatDlg::LoadRCMS()");
}

LRESULT CChatDlg::OnRCMSMenu(WPARAM wParam, LPARAM lParam)
{

	CString strUser = (LPCTSTR)lParam;
	int nIndex = wParam - 10211;
	if((nIndex >= 0) && (nIndex < m_aRCMSMenu.GetSize())){

		CString strOut = m_aRCMSMenu[nIndex];
		strOut.Replace("%NAME%", strUser);
		InputMessage(strOut);
	}
	return 1;
}


LRESULT CChatDlg::OnCopyIP(WPARAM wParam, LPARAM lParam)
{

	CString strUser = (LPCTSTR)lParam;
	CString strIP;

	CUser user;
	if(m_mUsers.Lookup(strUser, user)){

		strIP = user.strIP;
	}

	if(strIP.IsEmpty()){

		strIP = g_i18n.GetString(IDS_NOIP);
	}

	CClipboard::SetText((LPSTR)(LPCTSTR)strIP);
	return 1;
}

LRESULT CChatDlg::CustomizeRCMS(WPARAM wParam, LPARAM lParam)
{

	CString strIniFile, strRoom;

	strRoom = m_strRoomShort;
	strRoom.Remove('?');
	strRoom.Remove(':');
	strRoom.Remove(',');
	strRoom.Remove('\\');
	strRoom.Remove('/');
	strRoom.Remove('<');
	strRoom.Remove('>');
	strRoom.Remove('\"');
	strRoom.Remove('*');
	strRoom.Remove('|');
	strRoom.Replace(1, '-');

	strIniFile.Format("%s\\%s.mnu", g_sSettings.GetWorkingDir(), strRoom);


	CStdioFile ini;
	CString strBuffer;
	//m_aRCMSMenu.RemoveAll();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText|CFile::shareExclusive);

		if(ini.GetLength() == 0){

			if(m_nServerType == SERVER_WINMX353){

				ini.WriteString("/kickban %NAME% 5\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%\n");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/setuserlevel %NAME% 5\n");
				ini.WriteString("/setuserlevel %NAME% 65\n");
				ini.WriteString("/setuserlevel %NAME% 200\n");
				ini.WriteString("/setuserlevel %NAME% 65\n");
				ini.WriteString("/level %NAME%\n");
			}
			else if(m_nServerType == SERVER_ROSE){

				ini.WriteString("/exile %NAME%\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/muzzle %NAME%\n");
				ini.WriteString("/voice %NAME%\n");
				ini.WriteString("/admin %NAME%\n");
				ini.WriteString("/remadmin %NAME%\n");
				ini.WriteString("/usermodes %NAME%\n");
			}
			else if(m_nServerType == SERVER_MCS){

				ini.WriteString("#UserCmd Redirect %NAME%\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%\n");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/stats %NAME%\n");
				ini.WriteString("#AdminCmd PrintIP %NAME%\n");
				ini.WriteString("/addadmin %NAME%\n");
				ini.WriteString("/removeadmin %NAME%\n");
				ini.WriteString("#UserCmd Readmsg %NAME%\n");
			}
			else{

				ini.WriteString("#UserCmd Redirect %NAME%\n");
				ini.WriteString("#UserCmd /kick %NAME%\n");
				ini.WriteString("#AdminCmd /kickban %NAME%\n");
				ini.WriteString("#AdminCmd /ban %NAME%\n");
				ini.WriteString("#UserCmd /unban %NAME%\n");
				ini.WriteString("#userCmd PrintUserStat %NAME%\n");
				ini.WriteString("#AdminCmd PrintIP %NAME%\n");
				ini.WriteString("#AdminCmd AddAdmin %NAME%\n");
				ini.WriteString("#AdminCmd RemoveAdmin %NAME%\n");
				ini.WriteString("#UserCmd Readmsg %NAME%\n");
			}
		}

		ini.Close();

		
	}
	CATCH(CFileException, e){

	}END_CATCH;

	AfxMessageBox(g_i18n.GetString(IDS_EDITRCMSMENU), MB_ICONINFORMATION);

	ShellExecute(0, "open", "notepad.exe", strIniFile, 0, SW_SHOW);
	return 1;
}

LRESULT CChatDlg::OnAction(WPARAM wParam, LPARAM lParam)
{

	CString strNick = (LPSTR)wParam, strMsg = (LPSTR)lParam;
	
	theApp.OnMessage(m_strTitle, strNick, strMsg);
	for(int i = 0; i < g_Plugins.GetSize(); i++){

		g_Plugins[i]->OnMessage(m_dwID, strNick, strMsg, TRUE);
	}

	return 0;
}

LRESULT CChatDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{

	CString strNick = (LPSTR)wParam, strMsg = (LPSTR)lParam;
	
	theApp.OnMessage(m_strTitle, strNick, strMsg);

	for(int i = 0; i < g_Plugins.GetSize(); i++){

		g_Plugins[i]->OnMessage(m_dwID, strNick, strMsg, FALSE);
	}

	return 0;
}


LRESULT CChatDlg::OnOpMessage(WPARAM wParam, LPARAM lParam)
{

	CString strMsg = (char*)lParam;
	if(strMsg == "..") return 0;

	CString strTime;

	for(int i = 0; i < g_Plugins.GetSize(); i++){

		g_Plugins[i]->OnOpMessage(m_dwID, strMsg);
	}

	return 1;
}

LRESULT CChatDlg::OnTopic(WPARAM wParam, LPARAM lParam)
{

	m_strMotd = (LPSTR)lParam;
	return 1;
}

LRESULT CChatDlg::OnMotd(WPARAM wParam, LPARAM lParam)
{

	m_strMotd = (LPSTR)lParam;
	return 1;
}


LRESULT CChatDlg::OnAddUser(WPARAM wParam, LPARAM lParam)
{

	CUser user;

	ClientJoin *p = (ClientJoin*)lParam;
	ASSERT(p);

	user.strName	= p->strName;
	user.strIP		= p->strSrcIP;
	user.dwFiles	= p->dwFiles;
	user.dwNodeIP	= p->dwNodeIP;
	user.wLine		= p->wLine;
	user.wUserRank	= p->wLevel;
	user.wNodePort	= p->wNodePort;

	m_mUsers.SetAt(user.strName, user);
	AddUser(user.strName, user.wLine, user.dwFiles, user.dwNodeIP, user.wNodePort, user.strIP, user.strIP, user.wUserRank);

	theApp.OnEnter(m_strRoomShort, user.strName, user.dwFiles);
	
	return 1;
}

LRESULT CChatDlg::OnJoin(WPARAM wParam, LPARAM lParam)
{

	CUser user;

	ClientJoin *p = (ClientJoin*)lParam;
	ASSERT(p);

	user.strName	= p->strName;
	user.strIP		= p->strSrcIP;
	user.dwFiles	= p->dwFiles;
	user.dwNodeIP	= p->dwNodeIP;
	user.wLine		= p->wLine;
	user.wUserRank	= p->wLevel;
	user.wNodePort	= p->wNodePort;

	m_mUsers.SetAt(user.strName, user);
	AddUser(user.strName, user.wLine, user.dwFiles, user.dwNodeIP, user.wNodePort, user.strIP, user.strIP, user.wUserRank);

	theApp.OnEnter(m_strRoomShort, user.strName, user.dwFiles);
	
	return 1;
}

LRESULT CChatDlg::OnPart(WPARAM wParam, LPARAM lParam)
{

	ClientJoin *p = (ClientJoin*)lParam;
	ASSERT(p);

	CUser user;
	if(m_mUsers.Lookup(p->strName, user)){

		CString strIP = FormatIP(p->dwNodeIP);;

		RemoveUser(p->strName, strIP, p->wNodePort);
		m_mUsers.RemoveKey(p->strName);
	}

	return 1;
}

LRESULT CChatDlg::OnRedirect(WPARAM wParam, LPARAM lParam)
{


	CString strTitle = (LPSTR)lParam;
	m_strTitle = strTitle + " on WinMX Peer Network";

	if(g_sSettings.GetSoundFX()){

		PlaySound(g_sSettings.GetWorkingDir() + "\\data\\redirected.dat", 0, SND_ASYNC|SND_FILENAME);
	}

	int iPos = m_strTitle.ReverseFind('_');
	if(iPos != -1)	m_strRoomShort = m_strTitle.Left(iPos);
	m_strRoomShort.Remove('.');

	SetWindowText(m_strTitle + g_strIdent);
	return 1;
}

LRESULT CChatDlg::OnRenameMsg(WPARAM wParam, LPARAM lParam)
{


	ClientJoin *p = (ClientJoin*)lParam;
	ASSERT(p);

	CUser user;
	
	if(m_mUsers.Lookup(p->strName, user)){

		m_mUsers.RemoveKey(p->strName);
	}
	user.strName	= p->strName;
	user.strIP		= p->strSrcIP;
	user.dwFiles	= p->dwFiles;
	user.dwNodeIP	= p->dwNodeIP;
	user.wLine		= p->wLine;
	user.wUserRank	= p->wLevel;
	user.wNodePort	= p->wNodePort;

	m_mUsers.SetAt(user.strName, user);
	return 1;
}

LRESULT CChatDlg::OnIPClient(WPARAM wParam, LPARAM lParam)
{

	ClientJoin *p = (ClientJoin*)lParam;
	ASSERT(p);

	int nPos = m_lcUsers.SafeFind(p->strName);

	if(nPos >= 0){

		CUser newUser;
		CUser oldUser;

		if(m_mUsers.Lookup(p->strName, oldUser)){

			m_mUsers.RemoveKey(p->strName);
		}

		newUser = oldUser;
		newUser.strName		= p->strName;
		newUser.strIP		= p->strSrcIP;
		newUser.dwFiles		= p->dwFiles;
		newUser.dwNodeIP	= p->dwNodeIP;
		newUser.wLine		= p->wLine;
		newUser.wUserRank	= p->wLevel;
		newUser.wNodePort	= p->wNodePort;

		m_mUsers.SetAt(newUser.strName, newUser);
		
		m_lcUsers.DeleteItem(nPos);
		m_lcUsers.Sort();
		AddUser(newUser.strName, newUser.wLine, newUser.dwFiles, newUser.dwNodeIP, newUser.wNodePort, newUser.strIP, newUser.strIP, newUser.wUserRank); 
		m_lcUsers.Sort();																																		   

	}
	return 1;
}

LRESULT CChatDlg::OnRoomRename(WPARAM wParam, LPARAM lParam)
{

	CString strTitle = (LPSTR)lParam;
	
	ASSERT(strTitle.GetLength());

	m_strTitle = strTitle + " on WinMX Peer Network";

	int iPos = m_strTitle.ReverseFind('_');
	if(iPos != -1)	m_strRoomShort = m_strTitle.Left(iPos);
	m_strRoomShort.Remove('.');

	SetWindowText(m_strTitle + g_strIdent);

	return 1;
}

LRESULT CChatDlg::OnSetServerType(WPARAM wParam, LPARAM lParam)
{

	m_nServerType = (int)lParam;
	g_mServers.SetAt(m_hChat, m_nServerType);
	switch((int)m_nServerType){

		case SERVER_RCMS:
		case SERVER_OUKA:
		case SERVER_RCMS353:
		case SERVER_UNKNOWN:
			g_mCmds.SetAt(m_hEdit, &g_aRCMSCommands);
			break;
		case SERVER_WINMX353:
		case SERVER_MCMA:
			g_mCmds.SetAt(m_hEdit, &g_aWinMXCommands);
			break;
		case SERVER_MCS:
		case SERVER_WCS:
			g_mCmds.SetAt(m_hEdit, &g_aMXToolsCommands);
			break;
		case SERVER_ROSE:
			g_mCmds.SetAt(m_hEdit, &g_aRoseCommands);
			break;
		default:
			ASSERT(FALSE);
	}
	LoadRCMS();
	return 1;
}


CString CChatDlg::GetDate()
{

	CString strDate;
	SYSTEMTIME time;

	int n = ::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SSHORTDATE, 0, 0);

	if(n != 0){

		char *szFormat = new char[n];
		ZeroMemory(szFormat, n);

		n = ::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SSHORTDATE, szFormat, n);

		::GetLocalTime(&time);
		
		n = ::GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &time, szFormat, 0, 0); 
		
		if(n != 0){

			char *szDate = new char[n];
			ZeroMemory(szDate, n);

			n = ::GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &time, szFormat, szDate, n);
			
			strDate = szDate;
			
			delete szDate;
			szDate   = 0;
		}

		delete szFormat;
		szFormat = 0;
	}
	return strDate;
}

void CChatDlg::OnHide() 
{

	ShowWindow(SW_HIDE);
}

void CChatDlg::OnTop() 
{

	UpdateData(TRUE);
	if(m_bTop){

		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
	else{

		::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CChatDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	if((pScrollBar == (CScrollBar*)&m_slTrans) && (nSBCode == 5)){

		TRACE("%d %d %X %X\n", nSBCode, nPos, pScrollBar, &m_slTrans);
		m_strTrans.Format("%d%%", nPos);
		m_wndTrans.SetWindowText(m_strTrans);
	
		AddLayeredStyle(m_hWnd);
		SetTransparentPercentage(m_hWnd, 100-nPos);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CChatDlg::AddUser(CString strUsername, WORD wLine, DWORD dwFiles, DWORD dwNodeIP, WORD wNodePort, CString strIP, CString strHost, WORD wUserLevel)
{

	CString strFiles, strLine, strPort, strNodeIP;
	strFiles.Format("%d", dwFiles);
	strLine = FormatLine(wLine);
	strPort.Format("%d", wNodePort);
	strNodeIP = FormatIP(dwNodeIP);

	m_lcUsers.AddItem(wUserLevel, strUsername, (LPCTSTR)strFiles, (LPCTSTR)strLine, (LPCTSTR)strIP, (LPCTSTR)strHost, (LPCTSTR)strNodeIP, (LPCTSTR)strPort);
	m_lcUsers.Sort();
}

void CChatDlg::RemoveUser(const CString strUser, const CString strIP, WORD wPort)
{

	int nPos = m_lcUsers.SafeFind(strUser);

	if(nPos >= 0){

		m_lcUsers.DeleteItem(nPos);
		m_lcUsers.Sort();
	}
}

LONG CChatDlg::AddLayeredStyle(HWND hWnd)
{

	return ::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
}

extern HINSTANCE g_hUserDll;

BOOL CChatDlg::SetLayeredWindowAttributes(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{

	BOOL bRetValue = TRUE;

	if (g_hUserDll)
	{
		lpfnSetLayeredWindowAttributes pFn = NULL;
		pFn = (lpfnSetLayeredWindowAttributes)GetProcAddress(g_hUserDll, "SetLayeredWindowAttributes");
		if (pFn)
		{
			bRetValue = pFn(hWnd, crKey, bAlpha, dwFlags);
		} // if
		else bRetValue = FALSE;
	} // if

	return bRetValue;
}

BOOL CChatDlg::SetTransparentPercentage(HWND hWnd, BYTE byPercentage)
{

	// Do not accept values greater than 100%
	if (byPercentage > 100)	byPercentage = 100;

	return SetLayeredWindowAttributes(hWnd, ::GetSysColor(COLOR_WINDOW), 255 * byPercentage/100, LWA_ALPHA);
}

void CChatDlg::OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{


	CMenu *hRCMS = new CMenu;

	if(hRCMS->CreatePopupMenu()){


		POINT curPos;
		::GetCursorPos(&curPos);
		
		hRCMS->AppendMenu(MF_STRING, 10209, g_i18n.GetString(IDS_SENDMSG));
		hRCMS->AppendMenu(MF_STRING, 10220, g_i18n.GetString(IDS_READMSG));
		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);

		switch(m_nServerType){

		case SERVER_WINMX353:
		case SERVER_MCMA:
			hRCMS->AppendMenu(MF_STRING, 10211, g_i18n.GetString(IDS_RCMSKICKBAN5)); // kickban 5
			break;
		case SERVER_MCS:
			break;
		default:
			hRCMS->AppendMenu(MF_STRING, 10211, g_i18n.GetString(IDS_RCMSREDIRECT));
			break;
		}
		hRCMS->AppendMenu(MF_STRING, 10212, g_i18n.GetString(IDS_RCMSKICK));
		hRCMS->AppendMenu(MF_STRING, 10213, g_i18n.GetString(IDS_RCMSKICKBAN));
		hRCMS->AppendMenu(MF_STRING, 10214, g_i18n.GetString(IDS_RCMSBAN));
		hRCMS->AppendMenu(MF_STRING, 10215, g_i18n.GetString(IDS_RCMSUNBAN));
		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		
	
		switch(m_nServerType){

		case SERVER_WINMX353:
		case SERVER_ROSE:
		case SERVER_MCMA:
			hRCMS->AppendMenu(MF_STRING, 10216, g_i18n.GetString(IDS_RCMSDEVOICE)); // devoice
			hRCMS->AppendMenu(MF_STRING, 10217, g_i18n.GetString(IDS_RCMSVOICE));   // voice
			break;
		case SERVER_MCS:
			break;
		default:
			hRCMS->AppendMenu(MF_STRING, 10216, g_i18n.GetString(IDS_RCMSPRINTSTAT)); // devoice
			hRCMS->AppendMenu(MF_STRING, 10217, g_i18n.GetString(IDS_RCMSPRINTIP));   // voice
			break;
		}

		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		hRCMS->AppendMenu(MF_STRING, 10218, g_i18n.GetString(IDS_RCMSADDADMIN));  
		hRCMS->AppendMenu(MF_STRING, 10219, g_i18n.GetString(IDS_RCMSREMOVEADMIN));
		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		hRCMS->AppendMenu(MF_STRING, 10221, g_i18n.GetString(IDS_CUSTOMIZE));
		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		hRCMS->AppendMenu(MF_STRING, 10210, g_i18n.GetString(IDS_COPYUSERNAME));
		hRCMS->AppendMenu(MF_STRING, 10222, g_i18n.GetString(IDS_COPYIP));

		hRCMS->SetDefaultItem(10209);

		hRCMS->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
		
		for(int i = 0; i < g_Plugins.GetSize(); i++){

			g_Plugins[i]->m_pPlugin->OnPrepareMenu(0);
			if(g_Plugins[i]->m_pPlugin->hUserMenu <= 0) continue;
			hRCMS->AppendMenu(MF_POPUP, (UINT)g_Plugins[i]->m_pPlugin->hUserMenu, g_Plugins[i]->m_strShortName);
		}

		hRCMS->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, curPos.x, curPos.y, this, NULL);
		hRCMS->DestroyMenu();
	}
	delete hRCMS;
	hRCMS = NULL;
	*pResult = 0;
}



//DEL BOOL CChatDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//DEL {
//DEL 
//DEL 	Beep(1000, 2000);
//DEL 	return CDialog::Create(IDD, pParentWnd);
//DEL }

void CChatDlg::OnAdminMenu(UINT nCmdID)
{

	int nSel = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);

	if(nSel >= 0){

		CString strUser = m_lcUsers.GetItemText(nSel, 0);

		switch(nCmdID){

		case 10209: // Send Private message
			{

				HWND hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_UTIL));
				CString strTmp;
				strTmp.Format("#private %s #message ", strUser);
				::SendMessage(hTmp, UWM_SETID, (WPARAM)m_hWnd, 0);
				::SendMessage(hTmp, UWM_SPECIAL, (WPARAM)-1, (LPARAM)(LPCTSTR)strTmp);
			}
			break;
		case 10210: // copy username
			CClipboard::SetText((char*)(LPCSTR)strUser, NULL);
			break;;
		case 10222: // copy ip of user
			{

				CString strIP;

				CUser user;
				if(m_mUsers.Lookup(strUser, user)){

					strIP = user.strIP;
				}

				if(strIP.IsEmpty()){

					strIP = g_i18n.GetString(IDS_NOIP);
				}

				CClipboard::SetText((LPSTR)(LPCTSTR)strIP);
			}
			break;
		case 10211:
		case 10212:
		case 10213:
		case 10214:
		case 10215:
		case 10216:
		case 10217:
		case 10218:
		case 10219:
		case 10220:
			//hTmp = ::FindWindow(NULL, strChat + g_strIdent);
			::SendMessage(m_hWnd, UWM_ONRCMSMENU, nCmdID, (LPARAM)(LPCTSTR)strUser);
			break;
		case 10221:
			//hTmp = ::FindWindow(NULL, strChat + g_strIdent);
			::SendMessage(m_hWnd, UWM_ONRCMSEDIT, 0, 0);
			break;
		}
	}
}

#endif // #ifdef __BENDMX__
