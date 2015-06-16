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
#ifdef __BENDMX__

#include "I18n.h"
#include "Settings.h"
#include "BendMX.h"
#include "Logging\Logging.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

INITTRACE("I18n");


const int MENU_SEPARARTOR		= 0;		// identifies the separator id
const int MENU_POPUP			= -1;		// identifies the popup id

enum sections {COMMENT = -1, LANGUAGE_HEADER = 0, DIALOG_SEC, MENU_SEC, STRING_SEC, PLUGIN_SEC, REPLC_SEC};

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

//extern CSettings g_sSettings;
extern CBendMXApp   theApp;
extern CSettings	g_sSettings;

CI18n::CI18n()
{

	m_strAuthor			= "";
	m_strLanguageName	= "";
	m_strCodepage		= "";
	m_bLoaded			= FALSE;
	m_strLoadedLang		= "";
	
#ifdef _DEBUG
	m_bLog				= TRUE;
#else
	m_bLog				= FALSE;
#endif

}

CI18n::~CI18n()
{

}

BOOL CI18n::LoadLanguage(CString strLanguage)
{

	ENTRY_TRACE(_T("CI18n::LoadLanguage(%s)"), strLanguage);

	CString strFilepath;

	strFilepath.Format(_T("%s\\%s.txt"), g_sSettings.GetWorkingDir() + "\\languages", strLanguage);

	CStdioFile fFile;
	CFileException e;
	CString strText	= "";
	sections  sMode = COMMENT;
	
	TRY{

		if(!fFile.Open(strFilepath, CFile::modeRead|CFile::shareDenyWrite|CFile::typeText, &e)){

			LTRACE(_T("Failed to open language file %s"), strFilepath);
			return FALSE;
		}
		fFile.SeekToBegin();
		
		while(fFile.ReadString(strText)){

			strText.TrimLeft();
			strText.TrimRight();
			
			if(strText == _T("START"))      sMode = DIALOG_SEC;
			else if(strText == _T("END"))        sMode = COMMENT;
			else if(strText == _T("INFO_START")) sMode = LANGUAGE_HEADER;
			else if(strText == _T("INFO_END"))   sMode = COMMENT;
			
			if(!strText.IsEmpty() && (strText.Left(1) == _T("#"))){


				switch(sMode){

				case DIALOG_SEC:
					m_viItems.push_back(GetI18nItem(Ascii2Unicode(strText)));
					break;
				case LANGUAGE_HEADER:

					if(strText.Left(10) == _T("#LANGUAGE#")){

						m_strLanguageName = strText.Mid(11, strText.GetLength() - 11);
						m_strLanguageName.TrimLeft();
						m_strLanguageName.TrimRight();
						m_strLanguageName = m_strLanguageName.Mid(1, m_strLanguageName.GetLength() - 2);
					}
					else if(strText.Left(10) == _T("#CODEPAGE#")){

						m_strCodepage = strText.Mid(11, strText.GetLength() - 11);
						m_strCodepage.TrimLeft();
						m_strCodepage.TrimRight();						
						m_strCodepage = m_strCodepage.Mid(1, m_strCodepage.GetLength() - 2);
						m_dwCodePageID = _ttoi(m_strCodepage);
						
					}
					else if(strText.Left(8) == _T("#AUTHOR#")){

						m_strAuthor = strText.Mid(8, strText.GetLength() - 8);
						m_strAuthor.TrimLeft();
						m_strAuthor.TrimRight();						
						m_strAuthor = m_strAuthor.Mid(1, m_strAuthor.GetLength() - 2);
					}
					break;
				}
			}
		}

		fFile.Close();
	}
	CATCH( CFileException, e )
	{

	   #ifdef _DEBUG
		  afxDump << "File could not be opened " << e->m_cause << "\n";
	   #endif

	   m_bLoaded = FALSE;
	   m_strLoadedLang = "";
	}
	END_CATCH
	
	m_strLoadedLang = strLanguage;
	m_bLoaded = TRUE;
	EXIT_TRACE(_T(" CI18n::LoadLanguage()"));
	return TRUE;
}

BOOL CI18n::TranslateDialog(CDialog* pDlg, UINT nID)
{

	ENTRY_TRACE(_T("CI18n::TranslateDialog(%X, %X)"), pDlg, nID);

	CWnd* pDlgItem = NULL;
	short nCtrlID  = nID;

	CString strText = GetItemString(nID);
	
	if(strText.IsEmpty()){

		pDlg->GetWindowText(strText);
		LogMissingElement(nID, strText);
	}
	else{

		pDlg->SetWindowText(strText);
	}

	//Check for elements in dialog
	if(pDlgItem = pDlg->GetWindow(GW_CHILD|GW_HWNDFIRST)){

		do{

			if(!pDlgItem->IsKindOf(RUNTIME_CLASS(CEdit)) &&
				!pDlgItem->IsKindOf(RUNTIME_CLASS(CListCtrl)) &&
				!pDlgItem->IsKindOf(RUNTIME_CLASS(CSpinButtonCtrl)) &&
				!pDlgItem->IsKindOf(RUNTIME_CLASS(CComboBox)) &&
				!pDlgItem->IsKindOf(RUNTIME_CLASS(CProgressCtrl))){

				nCtrlID = pDlgItem->GetDlgCtrlID();
				
				if(nCtrlID > 0){

						
					strText = GetItemString(nCtrlID);

					if(strText.IsEmpty()){


						pDlgItem->GetWindowText(strText);

						LogMissingElement(nCtrlID, strText);
					}
					else{

						pDlgItem->SetWindowText(strText);
					}
				}
			}
			pDlgItem = pDlgItem->GetWindow(GW_HWNDNEXT);


		}while((pDlgItem != NULL) && (pDlgItem != pDlg->GetWindow(GW_CHILD|GW_HWNDFIRST)));
	}

	EXIT_TRACE(_T(" CI18n::TranslateDialog()"));

	return TRUE;
}


void CI18n::LogMissingElement(UINT nID, CString strText)
{

	if(strText.IsEmpty()){

		return;
	}

	LTRACE(_T("Missing Language Element #%X# \"%s\"\n"), m_strLanguageName, nID, ConvertString(strText, FALSE));

	if(!m_bLog) return;

	CString strLog;
	strLog.Format(_T("CI18n[%s]> #%X# \"%s\""), m_strLanguageName, nID, strText);

	CStdioFile fLog;
	CFileException e;
	TRY{

		fLog.Open(g_sSettings.GetWorkingDir() + "\\languages\\missing_controls.txt", CFile::modeWrite|CFile::shareExclusive|CFile::typeText|CFile::modeCreate|CFile::modeNoTruncate , &e);
		fLog.SeekToEnd();
		fLog.WriteString(ConvertString(strLog, FALSE) + "\n");
		fLog.Flush();
		fLog.Close();
		Sleep(50);
	}
	CATCH( CFileException, e )
	{
	   #ifdef _DEBUG
		  afxDump << "File could not be opened " << e->m_cause << "\n";
	   #endif
	}
	END_CATCH

}

CString CI18n::Ascii2Unicode(CString &strToTranslate)
{

#ifdef _UNICODE
	CString strRet;

	USES_CONVERSION;

	int nLen = strToTranslate.GetLength() + 1;

	LPWSTR lpwConvert = new TCHAR[ nLen ];

	LPCSTR pszTmp = W2A( strToTranslate );

	// convert ASCII string to WideCharacter string based on active code page
	MultiByteToWideChar(	m_dwCodePageID , 
							0,
							W2A( strToTranslate ),
							-1,
							lpwConvert,
							nLen  );

	strRet = lpwConvert;
	delete [] lpwConvert;

	return strRet;
#else
	return strToTranslate;
#endif
}

CString CI18n::GetString(UINT nID)
{

	ENTRY_TRACE("CI18n::GetString(%X)", nID);

	CString strReturn = GetItemString(nID);

	if(strReturn.IsEmpty()){

		AfxSetResourceHandle(theApp.m_hInstance);
		strReturn.LoadString(nID);
		LogMissingElement(nID, strReturn);
	}

	EXIT_TRACE("CI18n::GetString()");
	return strReturn;
}

CString CI18n::ConvertString(CString strText, BOOL bBack)
{

	if(bBack){

		strText.Replace(_T("\\n"), _T("\n"));
		strText.Replace(_T("\\t"), _T("\t"));
	}
	else{

		strText.Replace(_T("\n"), _T("\\n"));
		strText.Replace(_T("\t"), _T("\\t"));
	}

	return strText;
}

CString CI18n::GetItemString(UINT nID)
{

	ENTRY_TRACE("CI18n::GetItemString(%X)", nID);
	CString strRet;

	for(DWORD i = 0; i < m_viItems.size(); i++){

		if(m_viItems[i].nID == nID){

			strRet = m_viItems[i].strText;
			break;
		}
	}

	EXIT_TRACE("CI18n::GetItemString()");
	return strRet;
}



CI18nItem CI18n::GetI18nItem(CString strText)
{

	CI18nItem iNewItem;
	
	// First get the ID
	int nEnd = strText.Find(_T("#"), 1);
	
	CString strTmp = strText.Mid(1, nEnd - 1);
	_stscanf(strTmp, _T("%x"), &iNewItem.nID );

	// Get rest of string
	int nStart = strText.Find(_T("\""), nEnd) + 1;
	nEnd = strText.ReverseFind('\"');
	strTmp = strText.Mid(nStart, nEnd - nStart);
	iNewItem.strText = ConvertString(strTmp);

	return iNewItem;
}



void CI18n::UnLoad()
{

	m_viItems.clear();
	m_bLoaded = FALSE;
	m_strAuthor = "";
	m_strCodepage = "";
	m_strLanguageName = "";
	m_strLoadedLang = "";
}

#endif // #ifdef __BENDMX__


