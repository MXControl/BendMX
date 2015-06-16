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

#if !defined(AFX_I18N_H__25863D2D_D519_4E06_B6A5_F3041BEAD7E4__INCLUDED_)
#define AFX_I18N_H__25863D2D_D519_4E06_B6A5_F3041BEAD7E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include <vector>
using namespace std;

class CI18nItem
{

public:
	UINT nID;
	CString strText;
};

/*
class CI18nRplcItem
{

public:
	CString strOriginal;
	CString strReplacement;
};
*/
class CI18n  
{
public:
	void UnLoad();

	CI18n();
	virtual ~CI18n();

	CString GetLoadedLanguage(){ return m_strLoadedLang;}
	CString GetLanguageName(){ return m_strLanguageName;}
	CString GetLanguageAuthor(){ return m_strAuthor;}
	CString GetCodepage(){ return m_strCodepage;}
	
	CString GetString(UINT nID);
	BOOL LoadLanguage(CString strLanguage);
	BOOL TranslateDialog(CDialog *pDlg, UINT nID);

protected:
	CI18nItem GetI18nItem(CString strText);
	CString GetItemString(UINT nID);
	CString ConvertString(CString strText, BOOL bBack = TRUE);
	void LogMissingElement(UINT nID, CString strText);
	CString Ascii2Unicode(CString&);

	//DATAMEMBERS
	CString	 m_strAuthor;
	CString	 m_strLanguageName;
	CString  m_strCodepage;
	DWORD	 m_dwCodePageID;
	BOOL     m_bLoaded;
	BOOL     m_bLog;
	CString m_strLoadedLang;
	vector<CI18nItem> m_viItems;
};

#endif // !defined(AFX_I18N_H__25863D2D_D519_4E06_B6A5_F3041BEAD7E4__INCLUDED_)
