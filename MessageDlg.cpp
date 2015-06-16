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

#include "MessageDlg.h"
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
// CMessageDlg dialog
extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CBendMXApp theApp;
extern CStringArray g_strAuto;

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageDlg)
	m_strAdd = _T("");
	m_strWatch = _T("");
	m_strListen = _T("");
	//}}AFX_DATA_INIT
}


void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageDlg)
	DDX_Control(pDX, IDC_QUICKCMDS, m_lbQuick);
	DDX_Text(pDX, IDC_MSG_ADD, m_strAdd);
	DDX_Text(pDX, IDC_NOW_WATCHING, m_strWatch);
	DDX_Text(pDX, IDC_NOWLISTENING, m_strListen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageDlg)
	ON_BN_CLICKED(IDC_MSG_ADD_BTN, OnMsgAddBtn)
	ON_BN_CLICKED(IDC_QUICKREM, OnQuickrem)
	ON_BN_CLICKED(IDC_QUICKUP, OnQuickup)
	ON_BN_CLICKED(IDC_QUICKDOWN, OnQuickdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg message handlers

BOOL CMessageDlg::OnInitDialog() 
{

	ENTRY_TRACE("CMessageDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	
	g_i18n.TranslateDialog(this, IDD_MESSAGES);
	LoadQuickCmds();

	m_strListen = g_sSettings.GetWinampMsg();
	m_strWatch = g_sSettings.GetVideoMsg();
	UpdateData(FALSE);

	EXIT_TRACE("CMessageDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageDlg::Apply()
{
	UpdateData(TRUE);
	
	SaveQuickCmds();
	g_sSettings.SetWinampMsg(m_strListen);
	g_sSettings.SetVideoMsg(m_strWatch);
	LoadQuickCmds();
}


void CMessageDlg::LoadQuickCmds()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\quick.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	g_strAuto.RemoveAll();
	m_lbQuick.ResetContent();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				g_strAuto.Add(strBuffer);
				m_lbQuick.InsertString(m_lbQuick.GetCount(), strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

	}END_CATCH;
}

void CMessageDlg::SaveQuickCmds()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\quick.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbQuick.GetCount(); i++){
			
			m_lbQuick.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CMessageDlg::OnMsgAddBtn() 
{

	UpdateData(TRUE);
	if(m_strAdd.IsEmpty()) return;

	m_lbQuick.AddString(m_strAdd);
	g_strAuto.Add(m_strAdd);
	m_strAdd.Empty();

	UpdateData(FALSE);
}

void CMessageDlg::OnQuickrem() 
{

	int nSel = m_lbQuick.GetCurSel();
	if(nSel != -1){

		m_lbQuick.DeleteString(nSel);
		g_strAuto.RemoveAt(nSel);
	}
}

void CMessageDlg::OnQuickup() 
{
	
	int nSel = m_lbQuick.GetCurSel();
	if(nSel != LB_ERR){

		CString strTmp;
		m_lbQuick.GetText(nSel, strTmp);
		m_lbQuick.DeleteString(nSel);
		g_strAuto.RemoveAt(nSel--);
		m_lbQuick.InsertString(nSel < 0 ? m_lbQuick.GetCount() : nSel, strTmp);
		g_strAuto.InsertAt(nSel < 0 ? g_strAuto.GetSize() : nSel, strTmp);
		m_lbQuick.SetCurSel(nSel < 0 ? m_lbQuick.GetCount()-1 : nSel);
	}	
}

void CMessageDlg::OnQuickdown() 
{
	
	int nSel = m_lbQuick.GetCurSel();
	if(nSel != LB_ERR){

		CString strTmp;
		m_lbQuick.GetText(nSel, strTmp);
		m_lbQuick.DeleteString(nSel);
		g_strAuto.RemoveAt(nSel++);
		m_lbQuick.InsertString(nSel > m_lbQuick.GetCount() ? 0 : nSel,strTmp);
		g_strAuto.InsertAt(nSel > g_strAuto.GetSize() ? 0 : nSel, strTmp);
		m_lbQuick.SetCurSel(nSel > m_lbQuick.GetCount()-1 ? 0 : nSel);
	}	
}

#endif //#ifdef __BENDMX__

