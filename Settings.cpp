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
#include "Settings.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CSettings::axtoi(char *hexStg, int nLen)
{

 int n = 0;         // position in string
  int m = 0;         // position in digit[] to shift
  int count;         // loop index
  int intValue = 0;  // integer value of hex string
  int *digit = new int[nLen];      // hold values to convert
  while (n < nLen) {
	if (hexStg[n]=='\0'){
     
		break;
	}
    if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ){
		//if 0 to 9
        digit[n] = hexStg[n] & 0x0f;            //convert to int
	}
    else if (hexStg[n] >='a' && hexStg[n] <= 'f'){ //if a to f

        digit[n] = (hexStg[n] & 0x0f) + 9; 
	}//convert to int
	else if (hexStg[n] >='A' && hexStg[n] <= 'F'){ //if A to F

        digit[n] = (hexStg[n] & 0x0f) + 9;  
	}//convert to int
    else break;
    n++;
  }
  count = n;
  m = n - 1;
  n = 0;
  while(n < count) {
     // digit[n] is value of hex digit at position n
     // (m << 2) is the number of positions to shift
     // OR the bits into return value
     intValue = intValue | (digit[n] << (m << 2));
     m--;   // adjust the position to set
     n++;   // next digit to process
  }

  delete digit;
  digit = 0;
  return (intValue);
}

CSettings::CSettings()
{

#ifdef __BENDMX__

	m_nDepth		= 50;
	m_bHistory		= TRUE;
	m_bBlockHttp	= FALSE;
	m_bBlockMotd	= FALSE;
	m_bBlockPrivate = FALSE;
	m_bBlockTopic	= FALSE;
	m_bBlockEmpty	= FALSE;
	m_bBlockAscii	= FALSE;
	m_bBlockAdmin	= FALSE;
	m_bFilterXtrem  = FALSE;
	m_bFilterIgnore = FALSE;
	m_bFilterNormal = FALSE;
	m_bSloppy       = FALSE;
	m_nTimerRes     = 2000;

	// Bot
	m_bLimitBot = TRUE;
	m_nTimer = 4;
	m_bLeechKick = FALSE;
	m_strAnnounce.Format("%s :-)", STR_BENDMXVERSION);

	m_strWinampMsg = "/me plays '%WA-ARTIST% - %WA-SONG%'";
	m_strBotWelcome = "";
#endif

}

CSettings::~CSettings()
{

}

void CSettings::Load()
{

	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	m_strWd.Format("%s", szBuffer);
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));


#ifdef __BENDMX__
	CIni ini;
	ini.SetIniFileName(m_strWd + "\\BendMX.dat");

	m_strLanguage = ini.GetValue("General", "Language", "English");
	m_strNickname = ini.GetValue("General", "Nickname", "Unknown");
	
	m_bSloppy       = ini.GetValue("TempFix", "TmpSloppyShutdown", 0);
    m_bSoundFX = ini.GetValue("Look & Feel", "SoundFX", TRUE);
	m_nTimerRes = ini.GetValue("General", "SearchTimerRes", 2000);

	m_nDepth    = ini.GetValue("History", "Depth", 50);
	m_bHistory  = ini.GetValue("History", "Enable", TRUE);

	m_strVideoMsg = ini.GetValue("Messages", "WinampVideo", "/me watches '%WA-ARTIST% - %WA-SONG%'");
	m_strWinampMsg = ini.GetValue("Messages", "Winamp", "/me listens to '%WA-ARTIST% - %WA-SONG%'");
	m_bOnEnter     = ini.GetValue("Messages", "OnEnterEnable", FALSE);
	m_strEnterMsg  = ini.GetValue("Messages", "OnEnterMsg", "Hello.");
	m_strEnterMsg.Replace("\\rtf", "****");

	m_bBlockHttp = ini.GetValue("Filter", "BlockURL", FALSE);
	m_bBlockMotd = ini.GetValue("Filter", "BlockMotd", FALSE);
	m_bBlockPrivate = ini.GetValue("Filter", "BlockPrivate", FALSE);
	m_bBlockTopic = ini.GetValue("Filter", "BlockTopic", FALSE);
	m_bBlockEmpty = ini.GetValue("Filter", "BlockEmpty", FALSE);
	m_bBlockAscii = ini.GetValue("Filter", "BlockAscii", FALSE);
	m_bBlockAdmin = ini.GetValue("Filter", "BlockAdmin", FALSE);
	m_bFilterXtrem = ini.GetValue("Filter", "FilerXtrem", FALSE);
	m_bFilterIgnore = ini.GetValue("Filter", "FilterIngore", FALSE);
	m_bFilterNormal = ini.GetValue("Filter", "FilterNormal", FALSE);

	// Bot
	m_bLimitBot   = ini.GetValue("Bot", "LimitAccess", TRUE);
	m_nTimer      = ini.GetValue("Bot", "Timer", 4);
	m_bLeechKick  = ini.GetValue("Bot", "LeechKick", FALSE);
	m_strAnnounce = ini.GetValue("Bot", "Announce", m_strAnnounce);
	m_strBotWelcome = ini.GetValue("Bot", "Welcome", "");
	m_strBotWelcome.Replace("\\rtf", "****");
#endif
}

void CSettings::Save()
{

#ifdef __BENDMX__
	ASSERT(!m_strWd.IsEmpty());

	CIni ini;
	ini.SetIniFileName(m_strWd + "\\BendMX.dat");

	ini.SetValue("General", "Language", m_strLanguage);
	ini.SetValue("General", "Nickname", m_strNickname);
	ini.SetValue("General", "SearchTimerRes", m_nTimerRes);

    ini.SetValue("Look & Feel", "SoundFX", m_bSoundFX);

	ini.SetValue("TempFix", "TmpSloppyShutdown", m_bSloppy);

	ini.SetValue("History", "Depth", m_nDepth);
	ini.SetValue("History", "Enable", m_bHistory);


	ini.SetValue("Messages", "WinampVideo", m_strVideoMsg);
	ini.SetValue("Messages", "Winamp", m_strWinampMsg);
	ini.SetValue("Messages", "OnEnterEnable", m_bOnEnter);
	m_strEnterMsg.Replace("\\rtf", "****");
	ini.SetValue("Messages", "OnEnterMsg", m_strEnterMsg);

	ini.SetValue("Filter", "BlockURL", m_bBlockHttp);
	ini.SetValue("Filter", "BlockMotd", m_bBlockMotd);
	ini.SetValue("Filter", "BlockPrivate", m_bBlockPrivate);
	ini.SetValue("Filter", "BlockTopic", m_bBlockTopic);
	ini.SetValue("Filter", "BlockEmpty", m_bBlockEmpty);
	ini.SetValue("Filter", "BlockAscii", m_bBlockAscii);
	ini.SetValue("Filter", "BlockAdmin", m_bBlockAdmin);
	ini.SetValue("Filter", "FilerXtrem", m_bFilterXtrem);
	ini.SetValue("Filter", "FilterIngore", m_bFilterIgnore);
	ini.SetValue("Filter", "FilterNormal", m_bFilterNormal);

	// Bot
	ini.SetValue("Bot", "LimitAccess", m_bLimitBot);
	ini.SetValue("Bot", "Timer", m_nTimer);
	ini.SetValue("Bot", "LeechKick", m_bLeechKick);
	ini.SetValue("Bot", "Announce", m_strAnnounce);

	m_strBotWelcome.Replace("\\rtf", "****");
	ini.SetValue("Bot", "Welcome", m_strBotWelcome);
#endif

}

CString CSettings::GetWorkingDir(BOOL bCached)
{


	if(bCached){

		return m_strWd;
	}

	TCHAR  pszAppPath[ MAX_PATH + 1 ];
	TCHAR* pszSearch = NULL;

	memset( pszAppPath, 0, sizeof( pszAppPath ) );

	GetModuleFileName( NULL, pszAppPath, sizeof( pszAppPath ) );

	// find last \ character
	pszSearch = _tcsrchr( pszAppPath, _T( '\\' ) );

	if ( pszSearch )
	{
		// strip everything after the last \ char, \ char including 
		pszSearch[ 0 ] = '\0';
	}

	return pszAppPath;
}
