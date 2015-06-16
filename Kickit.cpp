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
#include "Kickit.h"
#include "Tokenizer.h"
#include "Settings.h"
#include "I18n.h"
#include "logging\logging.h"

INITTRACE("BendMXDlg")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKickit dialog

extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CBendMXApp theApp;
extern HWND g_hMXWindow;
extern CString g_strIdent;
extern UINT UWM_INPUTMESSAGE;

CKickit::CKickit(CWnd* pParent /*=NULL*/)
	: CDialog(CKickit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKickit)
	m_bRun = FALSE;
	m_strRoom = _T("");
	m_strAddName = _T("");
	m_strAddPwd = _T("");
	//}}AFX_DATA_INIT
}


void CKickit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKickit)
	DDX_Control(pDX, IDC_KICKITLOG, m_lbLog);
	DDX_Control(pDX, IDC_ADMINLIST, m_lcAdmins);
	DDX_Check(pDX, IDC_ENABLE_SERVER, m_bRun);
	DDX_Text(pDX, IDC_KICKIT_ROOM, m_strRoom);
	DDX_Text(pDX, IDC_ADD_KICKITADMIN, m_strAddName);
	DDX_Text(pDX, IDC_ADD_KICKITPASSWORD, m_strAddPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKickit, CDialog)
	//{{AFX_MSG_MAP(CKickit)
	ON_BN_CLICKED(IDC_ADMIN_ADD, OnAdminAdd)
	ON_BN_CLICKED(IDC_ADMIN_REMOVE, OnAdminRemove)
	ON_BN_CLICKED(IDC_ENABLE_SERVER, OnEnableServer)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKickit message handlers


BOOL CKickit::OnInitDialog() 
{

	ENTRY_TRACE("CKickitDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	g_i18n.TranslateDialog(this, IDD_KICKIT_SERVER);

	m_lcAdmins.InsertColumn(0, "Admin", LVCFMT_LEFT, 150);
	m_lcAdmins.InsertColumn(1, "Password", LVCFMT_LEFT, 130);
	ListView_SetExtendedListViewStyle(m_lcAdmins.m_hWnd, LVS_EX_FULLROWSELECT);

	LoadAdmins();
	
	EXIT_TRACE("CKickitDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKickit::OnAdminAdd() 
{
	
	UpdateData(TRUE);
	if(m_strAddName.IsEmpty() || m_strAddPwd.IsEmpty()){

		AfxMessageBox(g_i18n.GetString(IDS_KICKIT_MISSING), MB_OK+MB_ICONINFORMATION);
		return;
	}
	m_lcAdmins.InsertItem(0, m_strAddName, -1);
	m_lcAdmins.SetItemText(0, 1, m_strAddPwd);
	m_aAdmins.Add(m_strAddName + "-" + m_strAddPwd);

	m_strAddName.Empty();
	m_strAddPwd.Empty();
	UpdateData(FALSE);
}

void CKickit::OnAdminRemove() 
{
	
	if(m_lcAdmins.GetSelectedCount() == 0) return;

	int nSel = m_lcAdmins.GetSelectionMark();
	CString strName;

	strName = m_lcAdmins.GetItemText(nSel, 0);
	strName += "-" + m_lcAdmins.GetItemText(nSel, 1);

	m_lcAdmins.DeleteItem(nSel);

	for(int i = 0; i < m_aAdmins.GetSize(); i++){

		if(m_aAdmins[i] == strName){

			m_aAdmins.RemoveAt(i);
			break;
		}
	}	
}

UINT CKickit::ServerThread(PVOID pParam)
{

	CKickit* pKickit = (CKickit*)pParam;

	WSADATA		wsaData;
	sockaddr_in local;
	
	int wsaret=WSAStartup(0x101,&wsaData);

	if( wsaret!=0){

		pKickit->Log("Startup failed. Could not initialize Winsock.");
		pKickit->m_bRun = FALSE;
		pKickit->m_eServerDown.SetEvent();
		return 0;
	}

	local.sin_family	  = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port        = htons((u_short)25069);
	pKickit->m_sServer     = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(pKickit->m_sServer == INVALID_SOCKET){

		pKickit->Log("Startup failed. Could not create socket");
		pKickit->m_bRun = FALSE;
		pKickit->m_eServerDown.SetEvent();
		return 0;
	}
	if(bind(pKickit->m_sServer, (sockaddr*)&local, sizeof(local)) != 0){

		pKickit->Log("Startup failed. Could not bind to address");
		pKickit->m_bRun = FALSE;
		pKickit->m_eServerDown.SetEvent();
		return 0;
	}
	if(listen(pKickit->m_sServer, 10) != 0){

		pKickit->Log("Startup failed. Set socket to listen mode failed.");
		pKickit->m_bRun = FALSE;
		pKickit->m_eServerDown.SetEvent();
		return 0;
	}

	pKickit->Log("Kickit Server Started. Listening on port 25069 for connections.");

	sockaddr_in from;
	int fromlen = sizeof(from);

	while(pKickit->m_bRun){


		pKickit->m_sClient = accept(pKickit->m_sServer, (struct sockaddr*)&from, &fromlen);

		AfxBeginThread(ClientThread, (LPVOID)pKickit);	
	}	

	pKickit->Log("Server Exited normally");
	pKickit->m_eServerDown.SetEvent();
	return 0;
}

UINT CKickit::ClientThread(PVOID pParam)
{

	CKickit* pKickit = (CKickit*)pParam;
	if(!pKickit->m_bRun) return 0;

	SOCKET client = pKickit->m_sClient;

	char buff[512];
	CString strCmd;
	CString strParams;
	int n;
	BOOL auth= FALSE;


	n = recv(client, buff, 512, 0);
	if(n == SOCKET_ERROR){

		pKickit->Log("Error while reading from socket");
		return 0;
	}
	else{

		buff[n]=0;
		pKickit->HandleCommand(buff);
	}
	closesocket(client);
	return 0;
}

void CKickit::OnEnableServer() 
{

	UpdateData(TRUE);

	char *buffer = new char[200];
	::GetWindowText(g_hMXWindow, buffer, 200);
	
	if(strstr(buffer, "<Online P") == NULL){

		m_bRun = FALSE;
		UpdateData(FALSE);
		AfxMessageBox(g_i18n.GetString(IDS_NOPRIMARY), MB_OK+MB_ICONINFORMATION);
		delete buffer;
		buffer = 0;
		return;
	}
	delete buffer;
	buffer = 0;

	if(!m_strRoom.IsEmpty()){

		if(m_strRoom.Find(" on WinMX Peer Network", 0) < 0){

			m_strRoom += " on WinMX Peer Network";
			UpdateData(FALSE);
		}
	}

	if(m_strRoom.IsEmpty() || !theApp.DoesRoomExist(m_strRoom)){

		m_bRun = FALSE;
		UpdateData(FALSE);
		AfxMessageBox(g_i18n.GetString(IDS_NOROOM), MB_OK+MB_ICONINFORMATION);
		return;
	}

	GetDlgItem(IDC_KICKIT_ROOM)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_ADD_KICKITADMIN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_ADD_KICKITPASSWORD)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_ADMIN_ADD)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_ADMIN_REMOVE)->EnableWindow(!m_bRun);
	if(m_bRun){

		m_eServerDown.ResetEvent();
		AfxBeginThread(ServerThread, (PVOID)this, THREAD_PRIORITY_NORMAL);
	}
	else{

		WaitForSingleObject(&m_eServerDown, INFINITE);
		closesocket(m_sServer);
		WSACleanup();
	}
}

void CKickit::OnDestroy() 
{

	ENTRY_TRACE("CKickitDlg::OnDestroy()");
	CDialog::OnDestroy();

	if(m_bRun){

		m_bRun = FALSE;
		WaitForSingleObject(&m_eServerDown, INFINITE);
		closesocket(m_sServer);
		WSACleanup();
	}	
	EXIT_TRACE("CKickitDlg::OnDestroy()");
}

void CKickit::HandleCommand(CString strCmd)
{

	CTokenizer token(strCmd, "-");

	CString strUser, strPassword, strExec, strParam;
	if(!token.Next(strUser)){

		Log(g_i18n.GetString(IDS_WRONG) + strCmd);
		return;
	}
	if(!token.Next(strPassword)){

		Log(g_i18n.GetString(IDS_WRONG) + strCmd);
		return;
	}
	
	//check if username and password are valid;
	strUser += "-" + strPassword;
	BOOL bAuth = FALSE;

	for(int i = 0; i < m_aAdmins.GetSize(); i++){

		if(m_aAdmins[i] == strUser){
			bAuth = TRUE;
			break;
		}
	}

	if(!bAuth){

		Log("Unauthorized: " + strCmd);
		return;
	}


	// Authorized, get command
	if(!token.Next(strExec)){

		Log(g_i18n.GetString(IDS_WRONG) + strCmd);
		return;
	}


	// get command param if any
	CString strOut;

	if(strExec == "kick"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/%s %s", strExec, strParam);
	}
	else if(strExec == "ban"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/%s %s", strExec, strParam);
	}
	else if(strExec == "unban"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/%s %s", strExec, strParam);
	}
	else if(strExec == "clrban"){

		strOut.Format("/%s", strExec);
	}
	else if(strExec == "topic"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/%s %s", strExec, strParam);
	}
	else if(strExec == "motd"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/setmotd %s", strExec, strParam);
	}
	else if(strExec == "limit"){

		if(!token.Next(strParam)){

			Log(g_i18n.GetString(IDS_WRONG) + strCmd);
			return;
		}
		strOut.Format("/%s %s", strExec, strParam);
	}
	else{

		Log("Unknow command " + strCmd);
		return;
	}
	if(!strOut.IsEmpty()){

		InputMessage(strOut);
		Log("Executed: " + strCmd);
	}
}

void CKickit::Log(CString strLog)
{

	SYSTEMTIME time;

	GetLocalTime(&time);

	CString strOut;
	strOut.Format("[%02d:%02d:%02d] %s", time.wHour, time.wMinute, time.wSecond, strLog);
	int nIndex = m_lbLog.AddString(strOut);
	m_lbLog.SetCaretIndex(nIndex, TRUE);
}

void CKickit::LoadAdmins()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\kickit.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	m_aAdmins.RemoveAll();
	m_lcAdmins.DeleteAllItems();
	
	CString strAdmin, strPassword;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				m_aAdmins.Add(strBuffer);
				CTokenizer token(strBuffer, "-");
				if(token.Next(strAdmin) && token.Next(strPassword)){

					m_lcAdmins.InsertItem(0, strAdmin, -1);
					m_lcAdmins.SetItemText(0, 1, strPassword);
				}
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

	}END_CATCH;
}

void CKickit::SaveAdmins()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\kickit.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_aAdmins.GetSize(); i++){
			
			
			ini.WriteString(m_aAdmins[i] + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CKickit::Apply()
{

	SaveAdmins();
	LoadAdmins();
}

void CKickit::InputMessage(CString strMessage)
{

	HWND hChat = ::FindWindow(NULL, m_strRoom + g_strIdent);
	if(hChat != NULL){

		char buffer[400];
		if(strMessage.GetLength() > 400){

			strMessage = strMessage.Mid(400);
		}

		strcpy((char*)&buffer, (LPSTR)(LPCSTR)strMessage);
		::SendMessage(hChat, UWM_INPUTMESSAGE, 0, (LPARAM)&buffer);
	}
}
