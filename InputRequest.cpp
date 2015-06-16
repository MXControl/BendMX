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

#include "InputRequest.h"
#include "Settings.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRequest dialog

extern CSettings g_sSettings;
extern CI18n g_i18n;
extern CBendMXApp theApp;

UINT UWM_SPECIAL	= ::RegisterWindowMessage("UWM_SPECIAL_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_SETID		= ::RegisterWindowMessage("UWM_SETID_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
UINT UWM_GETOUT		= ::RegisterWindowMessage("UWM_GETOUT_{1B060DB9-E971-472C-92CF-7EDFDB9BBF64}");
extern UINT UWM_AWAY;

CInputRequest::CInputRequest(CWnd* pParent /*=NULL*/)
	: CDialog(CInputRequest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRequest)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CInputRequest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRequest)
	DDX_Text(pDX, IDC_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRequest, CDialog)
	//{{AFX_MSG_MAP(CInputRequest)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_SPECIAL, OnMsg)
	ON_REGISTERED_MESSAGE(UWM_SETID, OnSetID)
	ON_REGISTERED_MESSAGE(UWM_GETOUT, OnGetOut)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRequest message handlers

void CInputRequest::OnOK() 
{

	UpdateData(TRUE);

	CString strTmp;
	int i = 0;
	HWND hTmp;

	switch(m_nMode){

	case 0: // l33t
	
		m_strText.Replace("a", "4");
		m_strText.Replace("A", "4");
		m_strText.Replace("c", "(");
		m_strText.Replace("C", "(");
		m_strText.Replace("d", "Ð");
		m_strText.Replace("D", "Ð");
		m_strText.Replace("e", "3");
		m_strText.Replace("E", "3");
		m_strText.Replace("f", "ƒ");
		m_strText.Replace("F", "ƒ");
		m_strText.Replace("g", "9");
		m_strText.Replace("G", "9");
		m_strText.Replace("i", "1");
		m_strText.Replace("I", "1");
		m_strText.Replace("k", "|{");
		m_strText.Replace("K", "|{");
		m_strText.Replace("l", "£");
		m_strText.Replace("L", "£");
		m_strText.Replace("o", "0");
		m_strText.Replace("O", "0");
		m_strText.Replace("s", "$");
		m_strText.Replace("S", "$");
		m_strText.Replace("t", "7");
		m_strText.Replace("T", "7");
		m_strText.Replace("u", "µ");
		m_strText.Replace("U", "µ");
		m_strText.Replace("y", "¥");
		m_strText.Replace("Y", "¥");
		m_strText.Replace("z", "2");
		m_strText.Replace("Z", "2");
		break;
	case 1: // bubbles;
		m_strText.MakeUpper();
		for(i = 0; i < m_strText.GetLength(); i++){

			if(m_strText[i] != ' ')
				strTmp += "(" + m_strText.Mid(i, 1) + ")";
			else
				strTmp += " ";
		}
		m_strText = strTmp;
		break;
	case 2: // box;
		m_strText.MakeUpper();
		for(i = 0; i < m_strText.GetLength(); i++){

			if(m_strText[i] != ' ')
				strTmp += "[" + m_strText.Mid(i, 1) + "]";
			else
				strTmp += " ";
		}
		m_strText = strTmp;
		break;
	case 3: // 3D buttons msg;
		m_strText.MakeLower();
		strTmp = "-=[";
		for(i = 0; i < m_strText.GetLength(); i++){

			if(m_strText[i] != ' ')
				strTmp += "(" + m_strText.Mid(i, 1) + ")";
			else
				strTmp += " ";
		}
		m_strText = strTmp + "]=-";
		break;
	case 4: // 3D buttons action;
		m_strText.MakeLower();
		strTmp = "/me -=[";
		for(i = 0; i < m_strText.GetLength(); i++){

			if(m_strText[i] != ' ')
				strTmp += "(" + m_strText.Mid(i, 1) + ")";
			else
				strTmp += " ";
		}
		m_strText = strTmp + "]=-";
		break;
	case 5: // away control
		hTmp = ::FindWindow(NULL, g_i18n.GetString(IDD_BENDMX));
		::SendMessage(hTmp, UWM_AWAY, 2, 11);
		ShowWindow(SW_HIDE);
		return;
	case 6: // colored bubble 
		m_strText.MakeUpper();
		for(i = 0; i < m_strText.GetLength(); i++){

			if(m_strText[i] != ' ')
				strTmp += "#c2#(#c1#" + m_strText.Mid(i, 1) + "#c2#)";
			else
				strTmp += " ";
		}
		m_strText = strTmp;
		break;
	case 7: // wild color
		{
			int color = rand() % 11;
			CString strColor;
			for(i = 0; i < m_strText.GetLength(); i++){

				strColor.Format("#c%d#", color++);
				strTmp += strColor + m_strText.Mid(i, 1);
				if(color >= 11) color = 1;
			}
			m_strText = strTmp;
		}
		break;
	default: // #private
		break;
	}

	theApp.InputMessage(FALSE, m_strText, m_dwID);

	m_strText = "";
	m_dwID = 0;
	m_nMode = -1;
	ShowWindow(SW_HIDE);
}

BOOL CInputRequest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	g_i18n.TranslateDialog(this, IDD_UTIL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CInputRequest::OnSetID(WPARAM wParam, LPARAM lParam)
{

	m_dwID = (DWORD)wParam;

	return 1;
}

LRESULT CInputRequest::OnMsg(WPARAM wParam, LPARAM lParam)
{

	m_nMode = (int)wParam;
	m_strText = (LPCTSTR)lParam;
	UpdateData(FALSE);
	GetDlgItem(IDC_TEXT)->SetFocus();
	ShowWindow(SW_SHOW);

	return 1;
}

void CInputRequest::OnCancel() 
{

	m_strText = "";
	m_dwID = 0;
	m_nMode = -1;
	ShowWindow(SW_HIDE);
}

LRESULT CInputRequest::OnGetOut(WPARAM wParam, LPARAM lParam)
{

	if(m_strText.IsEmpty()){

		return 0;
	}
	if(m_strText.GetLength() > 300){

		m_strText = m_strText.Mid(300);
	}
	strcpy((char*)lParam, m_strText);
	m_strText = "";
	m_dwID = 0;
	m_nMode = -1;
	return 1;
}

void CInputRequest::OnDestroy() 
{

	m_dwID = -99;
	CDialog::OnDestroy();
	
}

#endif
