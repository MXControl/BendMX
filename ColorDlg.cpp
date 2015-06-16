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

#include "ColorDlg.h"
#include "Settings.h"
#include "I18n.h"
#include "logging\logging.h"

INITTRACE("BendMXDlg")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CBendMXApp theApp;

extern CStringArray g_strOps;
extern CStringArray g_strHosts;
/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
	m_strHostPre = _T("");
	m_strRegPre = _T("");
	//}}AFX_DATA_INIT
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_HOST_PREFIX, m_lbHostPre);
	DDX_Control(pDX, IDC_REG_PREFIX, m_lbRegs);
	DDX_Text(pDX, IDC_PRE_HOST_ADD, m_strHostPre);
	DDX_Text(pDX, IDC_PRE_REG_ADD, m_strRegPre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_BN_CLICKED(IDC_OP_ADD, OnOpAdd)
	ON_BN_CLICKED(IDC_OP_REM, OnOpRem)
	ON_BN_CLICKED(IDC_OP_ADD2, OnOpAdd2)
	ON_BN_CLICKED(IDC_OP_REM2, OnOpRem2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{

	ENTRY_TRACE("CColorDlg::OnInitDialog()");
	CDialog::OnInitDialog();
	
	g_i18n.TranslateDialog(this, IDD_COLORS);

	ReadHosts();
	ReadOps();

	UpdateData(FALSE);

	EXIT_TRACE("CColorDlg::OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorDlg::Apply()
{

	UpdateData(TRUE);
	WriteHosts();
	WriteOps();
	
	ReadHosts();
	ReadOps();
}

void CColorDlg::OnOpAdd() 
{
	
	UpdateData(TRUE);

	if(m_strRegPre.IsEmpty()) return;

	m_lbRegs.AddString(m_strRegPre);
	m_strRegPre.Empty();
	UpdateData(FALSE);
}

void CColorDlg::OnOpRem() 
{

	if(m_lbRegs.GetCurSel() != -1){

		m_lbRegs.DeleteString(m_lbRegs.GetCurSel());
	}
}

void CColorDlg::OnOpAdd2() 
{
	
	UpdateData(TRUE);
	if(m_strHostPre.IsEmpty()) return;

	m_lbHostPre.AddString(m_strHostPre);
	m_strHostPre.Empty();
	UpdateData(FALSE);
}

void CColorDlg::OnOpRem2() 
{
	
	if(m_lbHostPre.GetCurSel() != -1){

		m_lbHostPre.DeleteString(m_lbHostPre.GetCurSel());
	}
}



void CColorDlg::ReadOps()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\ops.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	g_strOps.RemoveAll();
	m_lbRegs.ResetContent();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				g_strOps.Add(strBuffer);
				m_lbRegs.InsertString(m_lbRegs.GetCount(), strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		//AfxMessageBox("Error while reading Autocompletion text!", MB_OK+MB_ICONSTOP);
		return;
	}END_CATCH;
}

void CColorDlg::WriteOps()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\ops.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbRegs.GetCount(); i++){
			
			m_lbRegs.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CColorDlg::ReadHosts()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\hosts.dat";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	g_strHosts.RemoveAll();
	m_lbHostPre.ResetContent();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				g_strHosts.Add(strBuffer);
				m_lbHostPre.InsertString(m_lbHostPre.GetCount(), strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		//AfxMessageBox("Error while reading Autocompletion text!", MB_OK+MB_ICONSTOP);
		return;
	}END_CATCH;
}

void CColorDlg::WriteHosts()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\hosts.dat";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbHostPre.GetCount(); i++){
			
			m_lbHostPre.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(g_i18n.GetString(IDS_ERROR_WRITECHANNELS), MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

#endif // #ifdef __BENDMX__
