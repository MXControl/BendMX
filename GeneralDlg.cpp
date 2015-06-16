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
#include "GeneralDlg.h"
#include "Settings.h"
#include "I18n.h"
#include "Plugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg dialog

extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CArray<CPlugin*, CPlugin*> g_Plugins;
extern CStringArray g_strRooms;
extern CBendMXApp theApp;
extern CString		g_strMyName;

CGeneralDlg::CGeneralDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralDlg)
	m_strLanguage = _T("");
	m_nHistory = 0;
	m_bHistory = FALSE;
	m_bAllChannels = FALSE;
	m_bNickAlert = FALSE;
	m_bSoundFx = FALSE;
	m_nRes = 0;
	m_bMoronMode = FALSE;
	//}}AFX_DATA_INIT
}


void CGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralDlg)
	DDX_Control(pDX, IDC_PLUGINLIST, m_lcPlugins);
	DDX_Control(pDX, IDC_LANG, m_cbLang);
	DDX_CBString(pDX, IDC_LANG, m_strLanguage);
	DDX_Text(pDX, IDC_HISTORDEPTH, m_nHistory);
	DDX_Check(pDX, IDC_ENABLE_HISTORY, m_bHistory);
	DDX_Check(pDX, IDC_SOUNDEFFECT, m_bSoundFx);
	DDX_Text(pDX, IDC_RESOLUTION, m_nRes);
	DDV_MinMaxInt(pDX, m_nRes, 1000, 100000);
	DDX_Check(pDX, IDC_MORONMODE, m_bMoronMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralDlg)
	ON_BN_CLICKED(IDC_REPAIR, OnRepair)
	ON_BN_CLICKED(IDC_FONTHELP, OnFonthelp)
	ON_BN_CLICKED(IDC_RELOAD_ALL, OnReloadAll)
	ON_BN_CLICKED(IDC_UNLOAD_ALL, OnUnloadAll)
	ON_CBN_SELENDOK(IDC_LANG, OnSelendokLang)
	ON_BN_CLICKED(IDC_MORONMODE, OnMoronmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg message handlers

BOOL CGeneralDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	g_i18n.TranslateDialog(this, IDD_GENERAL);

	m_lcPlugins.InsertColumn(0, g_i18n.GetString(IDS_PLUGIN), LVCFMT_LEFT, 100);
	m_lcPlugins.InsertColumn(1, g_i18n.GetString(IDS_DESC), LVCFMT_LEFT, 250);
	m_lcPlugins.InsertColumn(2, g_i18n.GetString(IDS_AUTHOR), LVCFMT_LEFT, 120);
	ListView_SetExtendedListViewStyle(m_lcPlugins.m_hWnd, LVS_EX_FULLROWSELECT);

	m_bHistory     = g_sSettings.GetSaveHistory();
	m_nHistory     = g_sSettings.GetHistoryDepth();
	m_bSoundFx	   = g_sSettings.GetSoundFX();
	m_strLanguage  = g_sSettings.GetLanguage();
	m_nRes		   = g_sSettings.GetTimerRes();
	m_bMoronMode   = g_sSettings.GetSloppy();

	CFileFind finder;
	BOOL bResult = finder.FindFile(g_sSettings.GetWorkingDir() + "\\languages\\*.txt");
		
	while(bResult){

		bResult = finder.FindNextFile();
		m_cbLang.AddString(finder.GetFileTitle());
	}

	if(m_cbLang.GetCount() > 0){

		int nIndex = m_cbLang.FindStringExact(0, m_strLanguage);
		if(nIndex != CB_ERR){

			m_cbLang.SetCurSel(nIndex);
		}
	}
	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralDlg::Apply()
{

	UpdateData(TRUE);

	g_sSettings.SetDoEnterMsg(m_bAllChannels);	
	g_sSettings.SetSaveHistory(m_bHistory);
	g_sSettings.SetHistoryDepth(m_nHistory);
	g_sSettings.SetSoundFX(m_bSoundFx);
	g_sSettings.SetLanguage(m_strLanguage);
	g_sSettings.SetTimerRes(m_nRes);
	g_sSettings.SetSloppy(m_bMoronMode);
}

void CGeneralDlg::OnRepair() 
{

	theApp.m_pBendMX->OnRepair(0,0);
}

void CGeneralDlg::OnFonthelp() 
{

	ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\readme.html", 0, 0, SW_SHOW);
}

void CGeneralDlg::OnRemove() 
{

	int nSel = m_lcPlugins.GetSelectionMark();
	if(nSel >= 0){

		if(AfxMessageBox("Remove plugin " + g_Plugins[nSel]->m_strShortName + "?", MB_ICONQUESTION+MB_YESNO) == IDNO){

			return;
		}

		CString strFile = g_Plugins[nSel]->m_strPluginName;
		
		for(int i = 0; i < theApp.m_pBendMX->m_aDlgs.GetSize(); i++){

			g_Plugins[nSel]->OnCloseChannel(theApp.m_pBendMX->m_aDlgs[i]->m_dwID);
		}
		//g_Plugins[nSel]->DeInit();
		CPlugin* tmp = g_Plugins.GetAt(nSel);
		g_Plugins.RemoveAt(nSel);
		tmp->DeInit();
		delete tmp;
		tmp = 0;
		BOOL bSuccess = FALSE;
		m_lcPlugins.DeleteItem(nSel);
		TRY{

			CFile::Remove(strFile);
			bSuccess = TRUE;
		}
		CATCH( CFileException, e ){

			bSuccess = FALSE;
		}
		END_CATCH
		
		AfxMessageBox((bSuccess ? "Successfully deleted " : "Could not remove ") + strFile, MB_ICONEXCLAMATION);

	}
	else{

		AfxMessageBox("Select plugin to remove first", MB_ICONEXCLAMATION);
	}
}

void CGeneralDlg::OnReloadAll() 
{
	
	UnloadPlugins();
	LoadPlugins();
	for(int i = 0; i < g_Plugins.GetSize(); i++){

		for(int j = 0; j < theApp.m_pBendMX->m_aDlgs.GetSize(); j++){

			g_Plugins[i]->OnEnterChannel(theApp.m_pBendMX->m_aDlgs[j]->m_dwID, theApp.m_pBendMX->m_aDlgs[j]->m_strTitle);
		}
	}
}

void CGeneralDlg::OnUnloadAll() 
{
	
	UnloadPlugins();
}

void CGeneralDlg::LoadPlugins()
{

	int i = 0;
	CFileFind finder;
	BOOL bResult = finder.FindFile(g_sSettings.GetWorkingDir() + _T("\\Add-Ons\\*.b4x"));
	while(bResult){

		bResult = finder.FindNextFile();
		CPlugin* pPlugin = new CPlugin(finder.GetFilePath());
		int nResult = pPlugin->Init();
		if(nResult == TRUE){

			g_Plugins.Add(pPlugin);

			m_lcPlugins.InsertItem(i, finder.GetFileName());
			m_lcPlugins.SetItemText(i, 1, pPlugin->m_strDescription);
			m_lcPlugins.SetItemText(i++, 2, pPlugin->m_strAuthor);
		}
		else{
			
			delete pPlugin;
			pPlugin = NULL;
			CString strTmp;
			switch(nResult){

			case 0:

				strTmp.Format("Error loading Plugin %s (Call LoadLibrary failed, GetLastError() returns %02d)", finder.GetFilePath(), GetLastError());
				break;
			case -1:
				strTmp.Format("Error loading Plugin %s (GetProcAdress returned zero)", finder.GetFileName());
				break;
			case -2:
				strTmp.Format("Error loading Plugin %s (Call GetPluginAPI failed)", finder.GetFileName());
				break;
			case -3:
				strTmp.Format("Error loading Plugin %s (Wrong API version. Check if there is a newer version of this Plugin)", finder.GetFileName());
				break;
			default:
				strTmp.Format("Unknown error while loading the plugin %s", finder.GetFileName());
			}

			AfxMessageBox(strTmp, MB_OK+MB_ICONEXCLAMATION);
		}

	}
}

void CGeneralDlg::UnloadPlugins()
{

	// De-Init all plugins
	while(g_Plugins.GetSize() > 0){

		CPlugin* pTemp = g_Plugins.GetAt(g_Plugins.GetSize() -1);
		g_Plugins.RemoveAt(g_Plugins.GetSize() -1);
		
		for(int i = 0; i < theApp.m_pBendMX->m_aDlgs.GetSize(); i++){
			
			pTemp->OnCloseChannel(theApp.m_pBendMX->m_aDlgs[i]->m_dwID);
		}
		
		pTemp->DeInit();
				
		delete pTemp;
		pTemp = NULL;
	}
	m_lcPlugins.DeleteAllItems();
}

void CGeneralDlg::OnSelendokLang() 
{

	AfxMessageBox("You have to restart WinMX to take changes into effect.", MB_OK+MB_ICONINFORMATION);
}

void CGeneralDlg::OnMoronmode() 
{

	UpdateData(TRUE);
	if(m_bMoronMode){

		AfxMessageBox("Please use this only if you experience problems when shutting down WinMX (like an evil Crash or so ;-)). Please note that if this option is enabled BendMX will produce a approx. 120KB memory leak on shutdown. This is a temporary workaround and will hopefully be fixed soon.");
	}
}

#endif //#ifdef __BENDMX__

