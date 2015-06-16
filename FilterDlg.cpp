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

#include "FilterDlg.h"
#include "Settings.h"
#include "Ini.h"
#include "I18n.h"
#include "logging\logging.h"

INITTRACE("BendMXDlg")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog

extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CStringArray g_strTrusted;
extern CStringArray g_strFilter;

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterDlg)
	m_bBlockHttp = FALSE;
	m_bBlockMotd = FALSE;
	m_bBlockPrivate = FALSE;
	m_bBlockTopic = FALSE;
	m_strFilterAdd = _T("");
	m_bBlockEmpty = FALSE;
	m_bBlockAscii = FALSE;
	m_bBlockAdmin = FALSE;
	m_bFilterXtrem = FALSE;
	m_bFilterNormal = FALSE;
	m_strTrustedAdd = _T("");
	//}}AFX_DATA_INIT
}


void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterDlg)
	DDX_Control(pDX, IDC_TRUSTED_LIST, m_lbTrusted);
	DDX_Control(pDX, IDC_FILTER_LIST, m_lbFilter);
	DDX_Check(pDX, IDC_BLOCK_HTTP, m_bBlockHttp);
	DDX_Check(pDX, IDC_BLOCK_MOTD, m_bBlockMotd);
	DDX_Check(pDX, IDC_BLOCK_PRIVATE_RCMS, m_bBlockPrivate);
	DDX_Check(pDX, IDC_BLOCK_TOPIC, m_bBlockTopic);
	DDX_Text(pDX, IDC_FILTER_ADD, m_strFilterAdd);
	DDX_Check(pDX, IDC_BLOCK_MSG, m_bBlockEmpty);
	DDX_Check(pDX, IDC_BLOCK_ASCII_ART, m_bBlockAscii);
	DDX_Check(pDX, IDC_BLOCK_ADMIN, m_bBlockAdmin);
	DDX_Check(pDX, IDC_FILTERXTREM, m_bFilterXtrem);
	DDX_Check(pDX, IDC_STARS, m_bFilterNormal);
	DDX_Text(pDX, IDC_TRUSTED_ADD, m_strTrustedAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterDlg)
	ON_BN_CLICKED(IDC_TRUSTED_ADD_BTN, OnTrustedAddBtn)
	ON_BN_CLICKED(IDC_TRUSTED_REM_BTN, OnTrustedRemBtn)
	ON_BN_CLICKED(IDC_FILTER_ADD_BTN, OnFilterAddBtn)
	ON_BN_CLICKED(IDC_FILTER_REM_BTN, OnFilterRemBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg message handlers

BOOL CFilterDlg::OnInitDialog() 
{

	ENTRY_TRACE("CFilterDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	
	g_i18n.TranslateDialog(this, IDD_FILTER);
	LoadFilter();
	LoadTrusted();

	m_bBlockHttp = g_sSettings.GetBlockHttp();
	m_bBlockMotd = g_sSettings.GetBlockMotd();
	m_bBlockPrivate = g_sSettings.GetBlockPrivate();
	m_bBlockTopic = g_sSettings.GetBlockTopic();
	m_bBlockEmpty = g_sSettings.GetBlockEmpty();
	m_bBlockAscii = g_sSettings.GetBlockAscii();
	m_bBlockAdmin = g_sSettings.GetBlockAdmin();
	m_bFilterXtrem = g_sSettings.GetFilterXtrem();
	m_bFilterNormal = g_sSettings.GetFilterNormal();

	UpdateData(FALSE);
	
	EXIT_TRACE("CFilterDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDlg::Apply()
{

	UpdateData(TRUE);
	SaveFilter();
	SaveTrusted();

	g_sSettings.SetBlockHttp(m_bBlockHttp);
	g_sSettings.SetBlockMotd(m_bBlockMotd);
	g_sSettings.SetBlockPrivate(m_bBlockPrivate);
	g_sSettings.SetBlockTopic(m_bBlockTopic);
	g_sSettings.SetBlockEmpty(m_bBlockEmpty);
	g_sSettings.SetBlockAscii(m_bBlockAscii);
	g_sSettings.SetBlockAdmin(m_bBlockAdmin);
	g_sSettings.SetFilterXtrem(m_bFilterXtrem);
	g_sSettings.SetFilterNormal(m_bFilterNormal);

	LoadFilter();
	LoadTrusted();
}


void CFilterDlg::OnTrustedAddBtn() 
{

	UpdateData(TRUE);
	if(m_strTrustedAdd.IsEmpty()) return;

	m_lbTrusted.AddString(m_strTrustedAdd);
	m_strTrustedAdd.Empty();
	UpdateData(FALSE);
}

void CFilterDlg::OnTrustedRemBtn() 
{

	if(m_lbTrusted.GetCurSel() != -1){

		m_lbTrusted.DeleteString(m_lbTrusted.GetCurSel());
	}
}

void CFilterDlg::OnFilterAddBtn() 
{
	
	UpdateData(TRUE);
	if(m_strFilterAdd.IsEmpty()) return;

	m_lbFilter.AddString(m_strFilterAdd);
	m_strFilterAdd.Empty();
	UpdateData(FALSE);
}

void CFilterDlg::OnFilterRemBtn() 
{
	
	if(m_lbFilter.GetCurSel() != -1){

		m_lbFilter.DeleteString(m_lbFilter.GetCurSel());
	}
}


void CFilterDlg::LoadTrusted()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\trusted.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	g_strTrusted.RemoveAll();
	m_lbTrusted.ResetContent();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				g_strTrusted.Add(strBuffer);
				m_lbTrusted.InsertString(m_lbTrusted.GetCount(), strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		return;
	}END_CATCH;
}

void CFilterDlg::SaveTrusted()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\trusted.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbTrusted.GetCount(); i++){
			
			m_lbTrusted.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CFilterDlg::LoadFilter()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\filter.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	g_strFilter.RemoveAll();
	m_lbFilter.ResetContent();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				g_strFilter.Add(strBuffer);
				m_lbFilter.InsertString(m_lbFilter.GetCount(), strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		return;
	}END_CATCH;
}

void CFilterDlg::SaveFilter()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\filter.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbFilter.GetCount(); i++){
			
			m_lbFilter.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

#endif //#ifdef __BENDMX__
