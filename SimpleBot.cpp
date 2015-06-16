// SimpleBot.cpp : implementation file
//

#include "stdafx.h"
#ifdef __BENDMX__

#include "BendMX.h"
#include "SimpleBot.h"
#include "Tokenizer.h"
#include "Settings.h"
#include "I18n.h"
#include "ChatDlg.h"
#include "logging\logging.h"

INITTRACE("BendMXDlg")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleBot dialog
extern CSettings	g_sSettings;
extern CI18n		g_i18n;
extern CBendMXApp   theApp;
extern CStringArray g_strOps;
extern HWND			g_hMXWindow;
extern CString		g_strIdent;
extern UINT			UWM_INPUTMESSAGE;
extern CString		g_strMyName;

CSimpleBot::CSimpleBot(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleBot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimpleBot)
	m_strAnnounce = _T("");
	m_strRoom = _T("");
	m_bBot = FALSE;
	m_nTimer = 0;
	m_bLeechKick = FALSE;
	m_bLimit = FALSE;
	m_strResponse = _T("");
	m_strTrigger = _T("");
	m_strWelcome = _T("");
	//}}AFX_DATA_INIT
}


void CSimpleBot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleBot)
	DDX_Control(pDX, IDC_COMMANDS, m_lcCommands);
	DDX_Text(pDX, IDC_ANNOUNCE, m_strAnnounce);
	DDX_Text(pDX, IDC_BOTROOM, m_strRoom);
	DDX_Check(pDX, IDC_ENABLEBOT, m_bBot);
	DDX_Text(pDX, IDC_INTERVAL, m_nTimer);
	DDV_MinMaxInt(pDX, m_nTimer, 4, 240);
	DDX_Check(pDX, IDC_LEECHKICK, m_bLeechKick);
	DDX_Check(pDX, IDC_LIMIT, m_bLimit);
	DDX_Text(pDX, IDC_RESPONSE, m_strResponse);
	DDX_Text(pDX, IDC_TRIGGER, m_strTrigger);
	DDX_Text(pDX, IDC_WELCOME, m_strWelcome);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimpleBot, CDialog)
	//{{AFX_MSG_MAP(CSimpleBot)
	ON_BN_CLICKED(IDC_ADDTRIGGER, OnAddtrigger)
	ON_BN_CLICKED(IDC_REMOVECMD, OnRemovecmd)
	ON_BN_CLICKED(IDC_ENABLEBOT, OnEnablebot)
	ON_BN_CLICKED(IDC_EDITCMD, OnEditcmd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleBot message handlers

BOOL CSimpleBot::OnInitDialog() 
{

	ENTRY_TRACE("CSimpleBot::OnInitDialog()");
	CDialog::OnInitDialog();
	
	g_i18n.TranslateDialog(this, IDD_SIMPLEBOT);
	m_lcCommands.InsertColumn(0, "Trigger", LVCFMT_LEFT, 150);
	m_lcCommands.InsertColumn(1, "Response", LVCFMT_LEFT, 150);
	ListView_SetExtendedListViewStyle(m_lcCommands.m_hWnd, LVS_EX_FULLROWSELECT);

	m_bLimit	 = g_sSettings.GetLimitBot();
	m_bLeechKick = g_sSettings.GetLechKick();
	m_nTimer     = g_sSettings.GetTimer();
	m_strAnnounce = g_sSettings.GetAnnounce();
	m_strWelcome = g_sSettings.GetBotWelcome();
	
	if(m_nTimer < 4 || m_nTimer > 240){

		m_nTimer = 4;
	}

	LoadBot();

	UpdateData(FALSE);
	EXIT_TRACE("CSimpleBot::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimpleBot::OnAddtrigger() 
{

	
	UpdateData(TRUE);
	if(m_strTrigger.IsEmpty() || m_strResponse.IsEmpty()){

		AfxMessageBox(g_i18n.GetString(IDS_BOT_EMPTY), MB_OK+MB_ICONINFORMATION);
		return;
	}
	
	if(m_strResponse.Find("\\rtf", 0) >= 0){

		AfxMessageBox("RTF Exploit detected in response. This will be removed you fool :-P", MB_ICONSTOP);
		m_strResponse.Replace("\\rtf", "****");
	}

	BOT_MESSAGE b;
	b.strIn = m_strTrigger;
	b.strOut = m_strResponse;

	m_lcCommands.InsertItem(0, m_strTrigger, -1);
	m_lcCommands.SetItemText(0, 1, m_strResponse);
	m_aBot.Add(b);

	m_strTrigger.Empty();
	m_strResponse.Empty();
	UpdateData(FALSE);
}

void CSimpleBot::OnRemovecmd() 
{

	if(m_lcCommands.GetSelectedCount() == 0) return;

	int nSel = m_lcCommands.GetSelectionMark();

	CString strIn  = m_lcCommands.GetItemText(nSel, 0);
	CString strOut = m_lcCommands.GetItemText(nSel, 1);

	m_lcCommands.DeleteItem(nSel);

	for(int i = 0; i < m_aBot.GetSize(); i++){

		if(m_aBot[i].strIn == strIn && m_aBot[i].strOut == strOut){

			m_aBot.RemoveAt(i);
			break;
		}
	}	
}

void CSimpleBot::Apply()
{

	SaveBot();
	LoadBot();

	g_sSettings.SetLimitBot(m_bLimit);
	g_sSettings.SetLechKick(m_bLeechKick);
	g_sSettings.SetTimer(m_nTimer);
	g_sSettings.SetAnnounce(m_strAnnounce);
	g_sSettings.SetBotWelcome(m_strWelcome);
	
}

void CSimpleBot::LoadBot()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\bot.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	m_aBot.RemoveAll();
	m_lcCommands.DeleteAllItems();
	
	BOT_MESSAGE b;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				
				CTokenizer token(strBuffer, "¨");
				if(token.Next(b.strIn) && token.Next(b.strOut)){

					b.strOut.Replace("\\rtf", "****");
					m_lcCommands.InsertItem(0, b.strIn, -1);
					m_lcCommands.SetItemText(0, 1, b.strOut);
					m_aBot.Add(b);
				}
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

	}END_CATCH;
}

void CSimpleBot::SaveBot()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\bot.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_aBot.GetSize(); i++){
			
			
			ini.WriteString(m_aBot[i].strIn + "¨" + m_aBot[i].strOut + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

UINT CSimpleBot::BotThread(PVOID pParam)
{
	
	CSimpleBot* pBot = (CSimpleBot*)pParam;

	int nLastTimer = GetTickCount();
	
	int nTimer = pBot->m_nTimer * 60 * 1000;

	pBot->m_nLastPost = 0;
	pBot->m_nProtect = 0;

	while(pBot->m_bBot){

		Sleep(2000);

		if(!pBot->m_qInQueue.empty()){

			CString strName = pBot->m_qInQueue.front().strIn;
			CString strMsg = pBot->m_qInQueue.front().strOut;
			pBot->m_qInQueue.pop();
			
			BOOL bAuth = !pBot->m_bLimit;

			if(pBot->m_bLimit){

				for(int i = 0; i < g_strOps.GetSize(); i++){

					if(strName.Find(g_strOps[i], 0) >= 0){

						bAuth = TRUE;
						break;
					}
				}
			}

			if(bAuth){

				for(int i = 0; i < pBot->m_aBot.GetSize(); i++){

					TRACE("Msg: %s | Trigger: %s\n", strMsg, pBot->m_aBot[i].strIn);
					if(strMsg.Find(pBot->m_aBot[i].strIn, 0) >= 0){

					
						if((GetTickCount() - pBot->m_nLastPost) < 5000){

							pBot->m_nProtect++;
							// Prevent the bot from spamming itself out of the channel
							if(pBot->m_nProtect > 3){
								
								Sleep(7000);
								pBot->m_nProtect = 0;
							}

						}
						else{

							pBot->m_nProtect = 0;
						}

						CString strOut;
						strOut = pBot->m_aBot[i].strOut;
						ReplaceVars(strOut, strName);
						pBot->m_strLast = strOut;
						pBot->m_nLastPost = GetTickCount();
						pBot->InputMessage(strOut);
						break;
					}
				}
			}
		}
		if((pBot->m_nTimer > 4) && ((GetTickCount() - nLastTimer) > nTimer)){

			if((GetTickCount() - pBot->m_nLastPost) < 5000){

				pBot->m_nProtect++;
				// Prevent the bot from spamming itself out of the channel
				if(pBot->m_nProtect > 3){
					
					Sleep(7000);
					pBot->m_nProtect = 0;
				}

			}
			else{

				pBot->m_nProtect = 0;
			}
			CString strOut = pBot->m_strAnnounce;
			pBot->ReplaceVars(strOut);
			pBot->m_strLast = strOut;
			nLastTimer = GetTickCount();
			pBot->m_nLastPost  = GetTickCount();
			pBot->InputMessage(strOut);
		}
	}
	LTRACE("Leaving Bot Thread");
	pBot->m_eBotDone.SetEvent();
	return 0;
}

void CSimpleBot::OnMessage(CString strName, CString strMessage)
{

	if((strName.Find(g_strMyName, 0) >= 0) ||
		(m_strLast.Find(strMessage, 0) >= 0))
	{
		// ignore if this is myslelf :-P
		TRACE("Thats me\n");
		return;
	}
	BOT_MESSAGE b;
	b.strIn  = strName;
	b.strOut = strMessage;
	m_qInQueue.push(b);
}

void CSimpleBot::OnEnter(CString strName, int nFiles)
{

	if(m_bLeechKick && (nFiles == 0) && (strName.Find(g_strMyName,0) < 0)){

		InputMessage("/kick " + strName);
		return;
	}

	if(m_strWelcome.IsEmpty()) return;

	BOOL bAuth = !m_bLimit;
	if(m_bLimit){

		for(int i = 0; i < g_strOps.GetSize(); i++){

			if(strName.Find(g_strOps[i], 0) >= 0){

				bAuth = TRUE;
				break;
			}
		}
	}

	if(bAuth){

			
		if((GetTickCount() - m_nLastPost) < 5000){

			m_nProtect++;
			// Prevent the bot from spamming itself out of the channel
			if(m_nProtect > 3){
				
				Sleep(7000);
				m_nProtect = 0;
			}

		}
		else{

			m_nProtect = 0;
		}

		CString strOut;
		strOut = m_strWelcome;
		ReplaceVars(strOut, strName);
		m_strLast = strOut;
		m_nLastPost = GetTickCount();
		InputMessage(strOut);
	}
}

void CSimpleBot::OnEnablebot() 
{

	UpdateData(TRUE);
	if(!m_strRoom.IsEmpty()){

		if(m_strRoom.Find(" on WinMX Peer Network", 0) < 0){

			m_strRoom += " on WinMX Peer Network";
			UpdateData(FALSE);
		}
	}
	if(m_strRoom.IsEmpty() || !theApp.DoesRoomExist(m_strRoom)){

		m_bBot = FALSE;
		UpdateData(FALSE);
		AfxMessageBox(g_i18n.GetString(IDS_NOROOM), MB_ICONINFORMATION);
		return;
	}
	if(m_bBot){

		if(AfxMessageBox(g_i18n.GetString(IDS_BOTWARNING), MB_ICONQUESTION+MB_YESNO) == IDNO){

			m_bBot = FALSE;
			UpdateData(FALSE);
			return;
		}
	}

	GetDlgItem(IDC_BOTROOM)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_ANNOUNCE)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_INTERVAL)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_LIMIT)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_LEECHKICK)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_TRIGGER)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_RESPONSE)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_ADDTRIGGER)->EnableWindow(!m_bBot);
	GetDlgItem(IDC_REMOVECMD)->EnableWindow(!m_bBot);

	if(m_bBot){

		m_eBotDone.ResetEvent();
		AfxBeginThread(BotThread, (LPVOID)this, THREAD_PRIORITY_NORMAL);
	}
	else{

		WaitForSingleObject(m_eBotDone, INFINITE);
	}


}

void CSimpleBot::ReplaceVars(CString &strOut, CString strName)
{

	CChatDlg::ReplaceVars(strOut);
	strOut.Replace("%NAME%", strName);
}

void CSimpleBot::InputMessage(CString strMessage)
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


void CSimpleBot::OnEditcmd() 
{

	if(m_lcCommands.GetSelectedCount()){

		int nSel = m_lcCommands.GetSelectionMark();
		if(nSel != -1){

			m_strTrigger  = m_lcCommands.GetItemText(nSel, 0);
			m_strResponse = m_lcCommands.GetItemText(nSel, 1);
			m_lcCommands.DeleteItem(nSel);
			UpdateData(FALSE);
		}
	}
}

#endif // #ifdef __BENDMX__

