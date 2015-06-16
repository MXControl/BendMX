// BotSetup.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Configurator.h"
#include "BotSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBotSetup 


CBotSetup::CBotSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CBotSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBotSetup)
	m_bBadLan = FALSE;
	m_bBot = FALSE;
	m_bExtended = FALSE;
	m_strExtended = _T("");
	m_nFlood = 0;
	m_bKick = FALSE;
	m_nWarnings = 0;
	m_bLog = FALSE;
	m_strMessage = _T("");
	m_strMotd = _T("");
	m_strNewLine = _T("");
	m_strNickname = _T("");
	m_strNoPenalty = _T("");
	m_bProtect = FALSE;
	m_bSound = FALSE;
	m_bRCMS = FALSE;
	m_nBlockMessageCount = 0;
	m_nBlockTime = 0;
	m_nBlockRange = 0;
	//}}AFX_DATA_INIT
}


void CBotSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBotSetup)
	DDX_Check(pDX, IDC_BADLAN, m_bBadLan);
	DDX_Check(pDX, IDC_BOT, m_bBot);
	DDX_Check(pDX, IDC_ENABLEEXTENDED, m_bExtended);
	DDX_Text(pDX, IDC_EXTENDED, m_strExtended);
	DDX_Text(pDX, IDC_FLOOD_SECS, m_nFlood);
	DDV_MinMaxInt(pDX, m_nFlood, 0, 20);
	DDX_Check(pDX, IDC_KICK, m_bKick);
	DDX_Text(pDX, IDC_KICKCOUNT, m_nWarnings);
	DDV_MinMaxInt(pDX, m_nWarnings, 0, 15);
	DDX_Check(pDX, IDC_LOG, m_bLog);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDV_MaxChars(pDX, m_strMessage, 8);
	DDX_Text(pDX, IDC_MOTD, m_strMotd);
	DDV_MaxChars(pDX, m_strMotd, 5);
	DDX_Text(pDX, IDC_NEWLINE, m_strNewLine);
	DDV_MaxChars(pDX, m_strNewLine, 6);
	DDX_Text(pDX, IDC_NICKNAME, m_strNickname);
	DDV_MaxChars(pDX, m_strNickname, 9);
	DDX_Text(pDX, IDC_NOPENATLY, m_strNoPenalty);
	DDX_Check(pDX, IDC_PROTECT, m_bProtect);
	DDX_Check(pDX, IDC_SOUND, m_bSound);
	DDX_Check(pDX, IDC_USERCMS, m_bRCMS);
	DDX_Text(pDX, IDC_MESSAGECOUNT, m_nBlockMessageCount);
	DDV_MinMaxInt(pDX, m_nBlockMessageCount, 1, 50);
	DDX_Text(pDX, IDC_BLOCKTIME, m_nBlockTime);
	DDV_MinMaxInt(pDX, m_nBlockTime, 1, 360000);
	DDX_Text(pDX, IDC_BLOCKRANGE, m_nBlockRange);
	DDV_MinMaxInt(pDX, m_nBlockRange, 1, 360000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBotSetup, CDialog)
	//{{AFX_MSG_MAP(CBotSetup)
	ON_BN_CLICKED(IDC_USERCMS, OnUsercms)
	ON_BN_CLICKED(IDC_KICK, OnKick)
	ON_BN_CLICKED(IDC_BADLAN, OnBadlan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CBotSetup 

void CBotSetup::OnOK() 
{

	if(UpdateData(TRUE)){

		CDialog::OnOK();
	}
}

void CBotSetup::OnUsercms() 
{

	UpdateData(TRUE);
	GetDlgItem(IDC_NEWLINE)->EnableWindow(m_bRCMS);
	GetDlgItem(IDC_MOTD)->EnableWindow(m_bRCMS);
	GetDlgItem(IDC_NICKNAME)->EnableWindow(m_bRCMS);
	GetDlgItem(IDC_MESSAGE)->EnableWindow(m_bRCMS);
}

void CBotSetup::OnKick() 
{

	UpdateData(TRUE);
	GetDlgItem(IDC_KICKCOUNT)->EnableWindow(m_bKick);
	GetDlgItem(IDC_PROTECT)->EnableWindow(m_bKick);
}

void CBotSetup::OnBadlan() 
{

	UpdateData(TRUE);
	GetDlgItem(IDC_KICK)->EnableWindow(m_bBadLan);
	GetDlgItem(IDC_KICKCOUNT)->EnableWindow(m_bBadLan && m_bKick);
	GetDlgItem(IDC_PROTECT)->EnableWindow(m_bBadLan && m_bKick);
}

BOOL CBotSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_tooltip.Create(this);
	m_tooltip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW | PPTOOLTIP_TRACKING_MOUSE | PPTOOLTIP_CLOSE_LEAVEWND, -1);
	
	m_tooltip.AddTool(GetDlgItem(IDC_USERCMS), "Channel Uses RCMS <ct=0x00C000><br><hr=100%><br></ct>This defines if the channel MXControl runs in, uses RCMS.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_NEWLINE), "Newline<ct=0x00C000><br><hr=100%><br></ct>Set this to the newline command the RCMS channel uses. Default is #\\r\\n#", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_MOTD), "Motd<ct=0x00C000><br><hr=100%><br></ct>Set his to the motd command the RCMS channel uses. Default is #motd", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_NICKNAME), "Nickname<ct=0x00C000><br><hr=100%><br></ct>Set this to the nickname command the RCMS channel uses. Default is #nickname", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_MESSAGE), "Message<ct=0x00C000><br><hr=100%><br></ct>Set this to the message command the RCMS channel uses. Default is #message", IDI_TIP);

	m_tooltip.AddTool(GetDlgItem(IDC_NOPENATLY), "Users excluded from Autokick<ct=0x00C000><br><hr=100%><br></ct>Put users that you want to exclude<br>from the Badword autokick here.<br><br>Username may contain wildcards *.<br>Applies only if <b>Enable Protected users</b> is set to TRUE", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_EXTENDED), "Extended Users<ct=0x00C000><br><hr=100%><br></ct>Extended users may trigger commands<br>from the extended command Section.<br><br>Username may contain wildcards *.<br>Applies only if <b>Enable Extended users</b> is set to TRUE", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_BOT), "Run Bot<ct=0x00C000><br><hr=100%><br></ct>Specifies if the the Bot should be enabled<br>by default when entering a channel.<br>Within a channel you can activate the<br>Bot with <b>#RUN_BOT</b>", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_LOG), "Enable Logging<ct=0x00C000><br><hr=100%><br></ct>If enabled, MXControl will log all chats<br>to chatlog.txt in the MXControl folder.<br>You can also en-/disbale this with<br><b>#LOG</b> in the channel.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_SOUND), "Enable Soundevents<ct=0x00C000><br><hr=100%><br></ct>En-/disable Soundevents.<br>You can set this also by using<br><b>#MUTE</b> within a channel.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_BADLAN), "Enable Badlanguage Response<ct=0x00C000><br><hr=100%><br></ct>If enabled the bot will warn users when entering a word from the badword list.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_KICK), "Enable Autokick<ct=0x00C000><br><hr=100%><br></ct>Kick user from your channel afer he was warned x-times", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_KICKCOUNT), "Number of warnings<ct=0x00C000><br><hr=100%><br></ct>Number of warnings a user gets, before he is kicked.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_PROTECT), "Enable Protected users<ct=0x00C000><br><hr=100%><br></ct>Protects users in the \"<b>Users excluded from Autokick</b>\"-list from being kicked on bad language", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_ENABLEEXTENDED), "Enable Extended users<ct=0x00C000><br><hr=100%><br></ct>Enables Extended commands.", IDI_TIP);

	m_tooltip.AddTool(GetDlgItem(IDC_FLOOD_SECS), "Flood protection (Seconds)<ct=0x00C000><br><hr=100%><br></ct>This is the time the bot waits<br>between its own postings.<br>This is to prevent the channel of beeing flooded<br>by the bot.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_MESSAGECOUNT), "Flood protection: Message count<ct=0x00C000><br><hr=100%><br></ct>Number of messages before Flood block is activated", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_BLOCKRANGE), "Blockrange (Seconds)<ct=0x00C000><br><hr=100%><br></ct>Time interval the Message count applies to.", IDI_TIP);
	m_tooltip.AddTool(GetDlgItem(IDC_BLOCKTIME), "Blocktime (Seconds)<ct=0x00C000><br><hr=100%><br></ct>If a user gets blocked because of<br>flooding the bot, ignore input from him<br>for this time interval.", IDI_TIP);
	//m_tooltip.AddTool(GetDlgItem(IDC_KICK), " <ct=0x00C000><br><hr=100%><br></ct>", IDI_TIP);
	
	OnBadlan();
	OnKick();
	OnUsercms();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

BOOL CBotSetup::PreTranslateMessage(MSG* pMsg) 
{

	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
