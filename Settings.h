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

#if !defined(AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_)
#define AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();
	void Save();
	void Load();

	static int axtoi(char *hexStg, int nLen = 1);
	void	SetWorkingDir(CString strValue){ m_strWd = strValue; }
	CString GetWorkingDir(BOOL bCached = TRUE);
#ifdef __BENDMX__

	int GetCharSet(BOOL bIndexOnly = FALSE);
	void SetCharSet(int nValue);
	// Set Functions

	void    SetSloppy(BOOL bValue){ m_bSloppy = bValue; }

	void    SetTimerRes(int nValue){ m_nTimerRes = nValue; }
	void    SetHistoryDepth(int nValue){ m_nDepth = nValue; }
	void    SetSaveHistory(int nValue){ m_bHistory = nValue; }
	void    SetSoundFX(BOOL bValue){ m_bSoundFX = bValue; }
	void	SetBlockHttp(BOOL bValue){ m_bBlockHttp = bValue; }
	void	SetBlockMotd(BOOL bValue){ m_bBlockMotd = bValue; }
	void	SetBlockPrivate(BOOL bValue){ m_bBlockPrivate = bValue; }
	void	SetBlockTopic(BOOL bValue){ m_bBlockTopic = bValue; }
	void	SetBlockEmpty(BOOL bValue){ m_bBlockEmpty = bValue; }
	void	SetBlockAscii(BOOL bValue){ m_bBlockAscii = bValue; }
	void	SetBlockAdmin(BOOL bValue){ m_bBlockAdmin = bValue; }
	void	SetFilterXtrem(BOOL bValue){ m_bFilterXtrem = bValue; }
	void	SetFilterIgnore(BOOL bValue){ m_bFilterIgnore = bValue; }
	void	SetFilterNormal(BOOL bValue){ m_bFilterNormal = bValue; }


	void	SetWinampMsg(CString strValue){ m_strWinampMsg = strValue; }
	void	SetVideoMsg(CString strValue){ m_strVideoMsg = strValue; }
	void	SetEnterMsg(CString strValue){ m_strEnterMsg = strValue; }
	void	SetDoEnterMsg(BOOL bValue){ m_bOnEnter = bValue; }
	void	SetLanguage(CString strValue){ m_strLanguage = strValue; }
	
	// Bot stuff
	void	SetAnnounce(CString strValue){ m_strAnnounce = strValue; }
	void	SetTimer(int nValue){ m_nTimer = nValue; }
	void	SetLechKick(BOOL bValue){ m_bLeechKick = bValue; }
	void	SetLimitBot(BOOL bValue){ m_bLimitBot = bValue; }
	void	SetBotWelcome(CString strValue){ m_strBotWelcome = strValue; }

	// Access functions
	CString GetLanguage(){ return m_strLanguage; }
	int     GetTimerRes(){ return m_nTimerRes; }

	BOOL    GetSloppy(){ return m_bSloppy; }

	BOOL	GetBlockHttp(){ return m_bBlockHttp; }
	BOOL	GetBlockMotd(){ return m_bBlockMotd; }
	BOOL	GetBlockPrivate(){ return m_bBlockPrivate; }
	BOOL	GetBlockTopic(){ return m_bBlockTopic; }
	BOOL	GetBlockEmpty(){ return m_bBlockEmpty; }
	BOOL	GetBlockAscii(){ return m_bBlockAscii; }
	BOOL	GetBlockAdmin(){ return m_bBlockAdmin; }
	BOOL	GetFilterXtrem(){ return m_bFilterXtrem; }
	BOOL	GetFilterIgnore(){ return m_bFilterIgnore; }
	BOOL	GetFilterNormal(){ return m_bFilterNormal; }

	int     GetHistoryDepth(){ return m_nDepth; }
	int     GetSaveHistory(){ return m_bHistory; }
	BOOL    GetSoundFX(){ return m_bSoundFX; }


	CString		GetWinampMsg(){ return m_strWinampMsg; }
	CString		GetVideoMsg(){ return m_strVideoMsg; }
	CString		GetEnterMsg(){ return m_strEnterMsg; }
	BOOL		GetDoEnterMsg(){ return m_bOnEnter; }

	// Bot stuff
	CString		GetAnnounce(){ return m_strAnnounce; }
	int			GetTimer(){ return m_nTimer; }
	BOOL		GetLechKick(){ return m_bLeechKick; }
	BOOL		GetLimitBot(){ return m_bLimitBot; }
	CString		GetBotWelcome(){ return m_strBotWelcome; }
#endif

protected:
	CString m_strWd;

#ifdef __BENDMX__
	BOOL	m_bOnEnter;
	BOOL    m_bSoundFX;
	int     m_nDepth;//   = NULL;
	int     m_bHistory;// = TRUE;
	int     m_nTimerRes;

	BOOL	m_bBlockHttp;
	BOOL	m_bBlockMotd;
	BOOL	m_bBlockPrivate;
	BOOL	m_bBlockTopic;
	BOOL	m_bBlockEmpty;
	BOOL	m_bBlockAscii;
	BOOL	m_bBlockAdmin;
	BOOL	m_bFilterXtrem;
	BOOL	m_bFilterIgnore;
	BOOL	m_bFilterNormal;
	BOOL    m_bSloppy;
	BOOL    m_bXtraColor;
	BOOL    m_bLimitXtra;

	CString		m_strEnterMsg;
	CString		m_strWinampMsg;
	CString		m_strVideoMsg;
	CString     m_strLanguage;
	CString		m_strNickname;

	// Bot stuff
	CString	m_strAnnounce;
	int		m_nTimer;
	BOOL	m_bLeechKick;
	BOOL	m_bLimitBot;
	CString m_strBotWelcome;
#endif
};

#endif // !defined(AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_)
