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
#include "ini.h"

#ifdef __BENDMX__
#include "Logging\Logging.h"
#include "Splash.h"
#include "MXSock.h"
#include "ChatDlg.h"
#include "Plugin.h"
#include "I18n.h"
#else
#define _NO_DEBUGTRACE
#include "Logging\Logging.h"
#include <winsock2.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


INITTRACE("BendMX");

#pragma comment(linker,"/IGNORE:4273")

// HANDLE to original DLL
HINSTANCE o_hDLL;

extern int WINAPI _orig_send(SOCKET s, const char FAR *buf, int len, int flags );
extern int WINAPI _orig_recv(SOCKET s, char FAR *buf, int len, int flags);
extern int WINAPI _orig_closesocket(SOCKET s);
extern int WINAPI _orig_connect(SOCKET s, const struct sockaddr FAR *name, int namelen);
extern SOCKET PASCAL _orig_accept(SOCKET a0, struct sockaddr* a1, int* a2);
extern int PASCAL _orig_recvfrom(SOCKET a0, char* a1, int a2, int a3, struct sockaddr* a4, int* a5);
extern int PASCAL _orig_sendto(SOCKET a0, const char* a1, int a2, int a3, const struct sockaddr* a4, int a5);

int WINAPI send(SOCKET s, const char FAR *buf, int len, int flags );
int WINAPI recv(SOCKET s, char FAR *buf, int len, int flags);
int WINAPI closesocket(SOCKET s);
int WINAPI connect(SOCKET s, const struct sockaddr FAR *name, int namelen);


CMap<CString, LPCTSTR, BOOL, BOOL&> g_aBlockedIPs;
CBendMXApp theApp;
CSettings  g_sSettings;

#ifdef __BENDMX__
CMap<SOCKET, SOCKET, PSOCKETMONITOR, PSOCKETMONITOR> g_mSocketMap;
BOOL g_bRCMS = FALSE;
extern CArray<CPlugin*, CPlugin*> g_Plugins;
extern CStringArray g_strTrusted;
extern CStringArray g_strFilter;
extern CString      g_strIdent;
extern CStringArray g_strRooms;
extern CI18n         g_i18n;


//char g_WPNBuffer[2048];

CString g_strMyName;
CString g_strVersion;

UINT UWM_MESSAGE	= ::RegisterWindowMessage("UWM_MESSAGE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ACTION		= ::RegisterWindowMessage("UWM_ACTION-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_TOPIC		= ::RegisterWindowMessage("UWM_TOPIC-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_MOTD		= ::RegisterWindowMessage("UWM_MOTD-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_JOIN		= ::RegisterWindowMessage("UWM_JOIN-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_PART		= ::RegisterWindowMessage("UWM_PART-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_RENAME		= ::RegisterWindowMessage("UWM_RENAME-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_REDIRECT   = ::RegisterWindowMessage("UWM_REDIRECT-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ROOMRENAME = ::RegisterWindowMessage("UWM_ROOMRENAME-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_OPMESSAGE  = ::RegisterWindowMessage("UWM_OPMESSAGE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_SERVERTYPE = ::RegisterWindowMessage("UWM_SERVERTYPE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_IPCLIENT   = ::RegisterWindowMessage("UWM_IPCLIENT-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ADDUSER    = ::RegisterWindowMessage("UWM_ADDUSER-229F871A-7B27-44C5-8879-AF881DE94891");
extern UINT UWM_ADDCHAT;

WORD FormatMXMessage(WORD wType, char *pBuff, char *lpszFormat, ...)
{

	// Data Format
	va_list Args;
	va_start(Args, lpszFormat);
	WORD wLen = 4;
	char *lpText = NULL;
	
	ZeroMemory(pBuff, sizeof(pBuff));
	
	while(*lpszFormat){

		switch(*lpszFormat){

		case 'S' :	// NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText) + 1);
			break;
		case 's' :	// Not NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText));
			break;
		case 'B' :	// BYTE
			pBuff[wLen] = va_arg(Args, BYTE);
			wLen += 1;
			break;
		case 'W' :	// WORD
			*((WORD *)(pBuff + wLen)) = va_arg(Args, WORD);
			wLen += 2;
			break;
		case 'D' :	// DWORD
			*((DWORD *)(pBuff + wLen)) = va_arg(Args, DWORD);
			wLen += 4;
			break;
		default :
			ASSERT(0);
			break;
	}

	lpszFormat++;
	}

	pBuff[wLen] = 0;

	va_end(Args);

	*((WORD*)pBuff) = wType;
	*((WORD*)(pBuff+2)) = wLen-4;

	return wLen;
}

WORD FormatMXMessageNoID(char *pBuff, char *lpszFormat, ...)
{

	// Data Format
	va_list Args;
	va_start(Args, lpszFormat);
	WORD wLen = 0;
	char *lpText = NULL;
	
	ZeroMemory(pBuff, sizeof(pBuff));
	
	while(*lpszFormat){

		switch(*lpszFormat){

		case 'S' :	// NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText) + 1);
			break;
		case 's' :	// Not NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText));
			break;
		case 'B' :	// BYTE
			pBuff[wLen] = va_arg(Args, BYTE);
			wLen += 1;
			break;
		case 'W' :	// WORD
			*((WORD *)(pBuff + wLen)) = va_arg(Args, WORD);
			wLen += 2;
			break;
		case 'D' :	// DWORD
			*((DWORD *)(pBuff + wLen)) = va_arg(Args, DWORD);
			wLen += 4;
			break;
		default :
			ASSERT(0);
			break;
	}

	lpszFormat++;
	}

	pBuff[wLen] = 0;

	va_end(Args);

	return wLen;
}

int ScanMessage(char *pBuff, WORD wMaxSize, char *lpszFormat, ...)
{

	ASSERT(pBuff && wMaxSize && lpszFormat);

	va_list Args;
	va_start(Args, lpszFormat);

	WORD wPos = 0;
	int iCount = 0;
	char *lpText = NULL;

	while(*lpszFormat && wPos < wMaxSize){

		switch(*lpszFormat){

		case 'S' : // string
			lpText = pBuff + wPos;
			*va_arg(Args, char **) = lpText;
			wPos += (lstrlen(lpText) + 1);
			iCount++;
			break;
		case 'H': // 20 characters long file hash
			memcpy((void*)va_arg(Args, BYTE *), pBuff + wPos, 20);
			wPos += 20;
			iCount++;
			break;			
		case 'B' : // byte
			*va_arg(Args, BYTE *) = *((BYTE *)(pBuff + wPos));
			wPos += 1;
			iCount++;
			break;
		case 'W' : // word
			*va_arg(Args, WORD *) = *((WORD *)(pBuff + wPos));
			wPos += 2;
			iCount++;
			break;
		case 'D' : // dword
			*va_arg(Args, DWORD *) = *((DWORD *)(pBuff + wPos));
			wPos += 4;
			iCount++;
			break;
		default :
			ASSERT(0);
			break;
		}
	
		lpszFormat++;
	}

	va_end(Args);

	return iCount;
}

CString FillStars(int nCount)
{

	CString strReturn;
	while(nCount){

		strReturn += "*";
		nCount--;
	}

	return strReturn;
}

void BlankOut(CString strUser, CString strMsg)
{

	for(int i = 0; i < strUser.GetLength(); i++){

		strUser.SetAt(i, ' ');
	}
	for(i = 0; i < strMsg.GetLength(); i++){

		strMsg.SetAt(i, ' ');
	}
}

BOOL FilterMessage(CString &strNick, CString &strMessage)
{

	// if user is on the trusted list dont filter the message
	for(int n = 0; n < g_strTrusted.GetSize(); n++){

		if(strNick.Find(g_strTrusted[n], 0) >= 0){

			return FALSE;
		}
	}

	// filter offensive words
	if(g_sSettings.GetFilterNormal() || g_sSettings.GetFilterXtrem()){

		for(int i = 0; i < g_strFilter.GetSize(); i++){


			if(strMessage.Find(g_strFilter[i], 0) >= 0){

				if(g_sSettings.GetFilterNormal()){

					strMessage.Replace(g_strFilter[i], FillStars(g_strFilter[i].GetLength()));
				}
				else{

					strMessage.Empty();
					strNick.Empty();
					return TRUE;
				}
			}
		}
	}

	// block #adminmessage
	if(g_sSettings.GetBlockAdmin()){

		if(strMessage.Find("[Admin] ", 0) == 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	// block #private
	if(g_sSettings.GetBlockPrivate()){

		if(strMessage.Find("[Private] ", 0) == 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	// block #ascii
	if(g_sSettings.GetBlockAscii()){

		if(strNick == " "){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}
	// block URL
	if(g_sSettings.GetBlockHttp()){

		if(strMessage.Find("http://", 0) >= 0){

			strMessage.Empty();
			strNick.Empty();
			return TRUE;
		}
	}

	return FALSE;
}

#endif // __BENDMX__

int WINAPI send(SOCKET s, const char FAR *buff, int len, int flags )
{


#ifndef __BENDMX__

	return _orig_send(s, buff, len, flags);
#else

	PSOCKETMONITOR pS = NULL;
	
	if(!g_mSocketMap.Lookup(s, pS)){
		
		LTRACE3("(0x%X) sending %d byte on unmonitored socket", s, len);
		return _orig_send(s, buff, len, flags);
	}
	
	ASSERT(pS);

	if(pS->iState == INIT_DONOTMONITOR){

		return _orig_send(s, buff, len, flags);
	}

	if(pS->iState == INIT_PENDING){

		//int n = _orig_send(s, buff, len, flags);
		LTRACE3("(0x%X) len = %d",s, len);
		if(len == 16){

			WORD wID = GetCryptKeyID((BYTE*)buff);
			LTRACE("(0x%X) wID == 0x%X", s, wID);
			if(wID == 0x57){

				pS->iState = INIT_HANDSHAKE;
				LTRACE3("(0x%X) Changed state to INIT_HANDSHAKE", s);
			}
			/*else{

				pS->iState = INIT_DONOTMONITOR;
				LTRACE3("a Removeing socket 0x%X", s);
				g_mSocketMap.RemoveKey(s);
				delete pS;
				pS = NULL;

			}*/
		}
		//return n;
	}
	else if((pS->iState == INIT_ISCHATROOM) && (len >= 4)){

		pS->dwMXUPKey = DecryptMXTCP((BYTE*)buff, len, pS->dwMXUPKey);
		
        WORD wType = *(WORD *)buff;
        WORD wLen  = *(WORD *)(buff + 2);
		if(wType == 0x0064){

			WORD wLine = 0, wPort = 0;
			DWORD dwIP = 0, dwFiles = 0;
			LPSTR lpRoom = 0, lpUser = 0;
			if(ScanMessage((char*)buff+4, len-4, "SWDWDS", &lpRoom, &wLine, &dwIP, &wPort, &dwFiles, &lpUser) == 6){

				pS->strRoom = lpRoom;
				pS->strUser = lpUser;
				g_strMyName = lpUser;
			}
		}
#ifdef _DEBUG
		else{

			LTRACE3("(0x%X) wType(0x%X) wLen(%d)", s, wType, wLen);
		}
#endif
		pS->dwUPKey = EncryptMXTCP((BYTE*)buff, len, pS->dwUPKey);
	}
	
	return _orig_send(s, buff, len, flags);
#endif // __BENDMX__
}

int WINAPI recv(SOCKET s, char FAR *buff, int len, int flags)
{


#ifndef __BENDMX__
	return _orig_recv(s, buff, len, flags);
#else

	PSOCKETMONITOR pS = NULL;
	
	if(!g_mSocketMap.Lookup(s, pS)){
		
		return _orig_recv(s, buff, len, flags);
	}

	ASSERT(pS);

	if(pS->iState == INIT_DONOTMONITOR){

		return _orig_recv(s, buff, len, flags);
	}


	int n = -1;
	if(pS->iState != INIT_ISPRIMARY){

		// recieve data, then process it :-)
		n = _orig_recv(s, buff, len, flags);
		LTRACE3("(0x%X) Recieved %d byte of data", s, len);
		if(len < 1){
			
			return n;
		}
	}
	if(pS->iState == INIT_PENDING){

		if(len == 1){

			if(buff[0] != 0x31){

				LTRACE3("(0x%X) Pending: Expected 0x31 but recieved 0x%X", s, buff[0]);
				pS->iState = INIT_DONOTMONITOR;
				g_mSocketMap.RemoveKey(s);
				delete pS;
				pS = NULL;
			}
			else{

				LTRACE3("(0x%X) Pending: OK, recieved 0x%X", s, buff[0]);
			}
		}
	}
	else if(pS->iState == INIT_HANDSHAKE){

		if(len == 16){

			WORD wID = GetCryptKeyID((BYTE*)buff);
			LTRACE("(0x%X) Handshake wID= 0x%X",s, wID);
			if(wID == 0x058){
			
				GetCryptKey((BYTE*)buff, &pS->dwUPKey, &pS->dwDWKey);
				pS->dwMXUPKey	= pS->dwUPKey;
				pS->dwMXDWKey	= pS->dwDWKey;
				pS->iState = INIT_ISCHATROOM;
			}
			else{

				LTRACE3("(0x%X) Unknown or invalid keyblock", s);
				pS->iState = INIT_DONOTMONITOR;
				g_mSocketMap.RemoveKey(s);
				delete pS;
				pS = NULL;
			}
		}
	}
	/*else if(pS->iState == INIT_ISPRIMARY){

		// decrypt data for analysis
		LTRACE3("(0x%X) recv len=%d", s, len);
		if((pS->wType == 0) && (len == 4)){ // packet header

			n = _orig_recv(s, buff, len, flags);
			if(len < 1){
				
				LTRACE("(0x%X) Error", s);
				return n;
			}
			pS->dwDWKey = DecryptMXTCP((BYTE*)buff, n, pS->dwDWKey);
			pS->wType = *(WORD *)buff;
			pS->wLen  = *(WORD *)(buff + 2);
			LTRACE3("(0x%X) recv packet 0x%X", s, pS->wType);
			if(pS->wType == 0x238E){
				
				LTRACE3("(0x%X) Chatroompacket. Recieving packet of lenght=%d (0x%X)", (int)pS->socket, pS->wLen, pS->wLen);

				if(pS->wLen < 2048){ // can the buffer hold the lenght?

					// recieve data
					ZeroMemory(g_WPNBuffer, 2048);
					_orig_recv(s, (char*)&g_WPNBuffer, pS->wLen, flags);

					// decrypt
					pS->dwDWKey = DecryptMXTCP((BYTE*)&g_WPNBuffer, pS->wLen, pS->dwDWKey);
					
					LPSTR lpRoom  = 0, lpTopic = 0; 
					DWORD dwUsers = 0, dwLimit = 0;

					// scan for analysis
					if(ScanMessage(g_WPNBuffer, pS->wLen, "DDSS", &dwUsers, &dwLimit, &lpRoom, &lpTopic) == 4){

						LTRACE3("Roomanmelength is %d, Topiclength is %d", strlen(lpRoom), strlen(lpTopic));

						CString strRoom = lpRoom, strTopic = lpTopic;
						if(strlen(lpRoom) > 40){

							// roomname was too long
							strRoom = "x " + strRoom.Mid(strRoom.GetLength()-38);
							LTRACE3("Fixed long roomname! %s --> %s", lpRoom, strRoom);
						}
						else if(strlen(lpTopic) > 125){

							strTopic = "x " + strTopic.Left(123);
							LTRACE3("Fixed long topic! %s --> %s", lpTopic, strTopic);
						}

						ZeroMemory(g_WPNBuffer, 2048);

						// copy back to buffer
						pS->wLen = FormatMXMessageNoID((char*)&g_WPNBuffer, "DDSS", dwUsers, dwLimit, (LPCTSTR)strRoom, (LPCTSTR)strTopic);
			
						// fix packet header:
						*(WORD *)(buff + 2) = pS->wLen;
						
						// encrypt packet header again
						pS->dwMXDWKey = EncryptMXTCP((BYTE*)buff, n, pS->dwMXDWKey);

					} // end scan
					
					// encrypt our WPN buffer
					pS->dwMXDWKey = EncryptMXTCP((BYTE*)&g_WPNBuffer, pS->wLen, pS->dwMXDWKey);
				}
			} // end pS->wType == 0x238E
			else{

				// encrypt packet header again
				pS->dwMXDWKey = EncryptMXTCP((BYTE*)buff, n, pS->dwMXDWKey);
			}
		}
		else{

			LTRACE3("(0x%X) Recv 0x%X", s, pS->wType);
			if(pS->wType == 0x238E){

				memcpy(buff, &g_WPNBuffer, pS->wLen);
				n = pS->wLen;
			}
			pS->wType = 0;
			pS->wLen  = 0;
		}
	}*/
	else if(pS->iState == INIT_ISCHATROOM){

		// decrypt data for analysis
		pS->dwDWKey = DecryptMXTCP((BYTE*)buff, n, pS->dwDWKey);
		if((pS->wType == 0) && (len == 4)){ // packet header

			
			pS->wType = *(WORD *)buff;
			pS->wLen  = *(WORD *)(buff + 2);
			

			if(pS->wType == 0x66){

				pS->strServer  = "WinMX";
				pS->strVersion = "3.3x/compatible";
				
			}
			else if(pS->wType == 0x68){

				if(pS->wServerType < SERVER_WINMX353){

					pS->wServerType	  = SERVER_WINMX353;
					pS->strServer  = "WinMX";
					pS->strVersion = "3.5x/compatible";
				}
			}
			else if(pS->wType == 0x69){

				pS->wServerType = SERVER_MCMA;
				pS->strServer	= "WinMX/MCMA";
				pS->strVersion  = "3.53/compatible";
			}
			else if((pS->wType == 0xFDE8) && (pS->wLen == 0)){

				pS->wType = 0; // ping
				pS->wLen  = 0;
			}

		}
		else if(len == pS->wLen){

			pS->dwOffset = 0;
			if((pS->wType == 0x00C9) || (pS->wType == 0x00CB)){

				LPSTR lpUser = 0, lpMsg = 0;
				if(ScanMessage((char*)buff, pS->wLen, "SS", &lpUser, &lpMsg) == 2){

					theApp.SendMessageTo(pS->strRoom, pS->wType == 0x00C9 ? UWM_MESSAGE : UWM_ACTION, (WPARAM)lpUser, (LPARAM)lpMsg);

					CString strMsg = lpMsg, strUser = lpUser;

					if(FilterMessage(strUser, strMsg)){

						strUser = " "; strMsg = " ";
						ZeroMemory(buff, pS->wLen);
					}

					len = FormatMXMessageNoID(buff, "SS", (LPCTSTR)strUser, (LPCTSTR)strMsg);
				}
			}
			else if((pS->wType == 0x006F) || (pS->wType == 0x0072)){

				if(pS->bHooked == FALSE){

					pS->bHooked = TRUE;
					theApp.SearchChatWnd(pS->strRoom);
					Sleep(10);
				}
				ClientJoin* pUser = new ClientJoin();

				LPSTR lpUser = 0;

				// 0x006F][Username:N][00:1][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4]
				if(pS->wType == 0x072){ // winmx 3.5x

					ScanMessage(buff, pS->wLen, "SDWWDW", &lpUser, &pUser->dwNodeIP, 
											&pUser->wNodePort, &pUser->wLine, &pUser->dwFiles, 
											  &pUser->wLevel);
				}
				else{

					ScanMessage(buff, pS->wLen, "SDWWD", &lpUser, &pUser->dwNodeIP, 
											&pUser->wNodePort, &pUser->wLine, 
											  &pUser->dwFiles);
					pUser->wLevel = 0;
				}

				pUser->strName = lpUser;

				theApp.SendMessageTo(pS->strRoom, UWM_ADDUSER, 0,(LPARAM)pUser);
				delete pUser;
			}
			else if((pS->wType == 0x006E) || (pS->wType == 0x0071) || (pS->wType == 0x0075)){

				ClientJoin* pJoin = new ClientJoin();
				
				LPSTR lpUser  = 0;
				BYTE  btDummy = 0;

				if(pS->wType == 0x071){ // winmx 3.52 and later (no-ip display)

					ScanMessage(buff, pS->wLen, "SDWWDB", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles, &btDummy);
				}
				else if(pS->wType == 0x075){ // winmx 3.52 and later (ip display)

					ScanMessage(buff, pS->wLen, "SDWWDBD", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles, &btDummy, &pJoin->dwSrcIP);
				}
				else{  // old winmx

					ScanMessage(buff, pS->wLen, "SDWWD", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles);
				}

				if(lpUser != 0){

					pJoin->strName = lpUser;
					if((btDummy >= 0) && (btDummy <= 2))
						pJoin->wLevel  = (WORD)btDummy;
					else
						pJoin->wLevel  = 0;

					theApp.SendMessageTo(pS->strRoom, UWM_JOIN, 0, (LPARAM)pJoin);
				}

				delete pJoin; 
			}
			else if(pS->wType == 0x012C){

				if(g_sSettings.GetBlockTopic()){

					ZeroMemory(buff, pS->wLen);
					strcpy(buff, " ");
				}
				else{

					theApp.SendMessageTo(pS->strRoom, UWM_TOPIC, 0, (LPARAM)buff);
					if(g_sSettings.GetSoundFX()){

						PlaySound(g_sSettings.GetWorkingDir() + "\\data\\topic.dat", 0, SND_NOWAIT|SND_ASYNC|SND_FILENAME|SND_NODEFAULT );
					}
					theApp.SendMessageTo(pS->strRoom, UWM_SERVERTYPE, 0, (LPARAM)pS->wServerType);
				}
			}
			else if(pS->wType == 0x0078){

				if(g_sSettings.GetBlockMotd()){

					ZeroMemory(buff, pS->wLen);
					strcpy(buff, " ");
				}
				else{

					LPSTR lpMotd = buff;
					theApp.SendMessageTo(pS->strRoom, UWM_MOTD, 0, (LPARAM)buff);
					if(g_sSettings.GetSoundFX()){

						PlaySound(g_sSettings.GetWorkingDir() + "\\data\\motd.dat", 0, SND_NOWAIT|SND_ASYNC|SND_FILENAME|SND_NODEFAULT );
					}
				}
			}
			else if(pS->wType == 0x0073){

				LPSTR lpUser = 0;
				ClientJoin *pUser = new ClientJoin();
				if(ScanMessage(buff, pS->wLen, "SDW", &lpUser, &pUser->dwNodeIP, &pUser->wNodePort) == 3){

					pUser->strName = lpUser;
					theApp.SendMessageTo(pS->strRoom, UWM_PART, 0, (LPARAM)pUser);
				}
				delete pUser;
			}
			else if((pS->wType == 0x0070) || (pS->wType == 0x0074)){

				LPSTR oldName = 0, newName = 0;
				ClientJoin* pUser = new ClientJoin();

				if(pS->wType == 0x0074){

					// 0x0074	353
					//[Name:N][IP-Address:4][UDP-Port:2]	// old
					//[Name:N][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4][Rank:1] // new
					ScanMessage(buff, pS->wLen, "SDWSDWWDB", &oldName, &pUser->dwOldNodeIP, &pUser->wOldNodePort,
											&newName, &pUser->dwNodeIP, &pUser->wNodePort, 
											&pUser->wLine, &pUser->dwFiles, (BYTE*)&pUser->wLevel);

				}
				else{

					//0x0070]	331
					//[Name:N][00:1][IP-Address:4][UDP-Port:2] // old
					//[Name:N][00:1][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4] // new
					ScanMessage(buff, pS->wLen, "SDWSDWWD", &oldName, &pUser->dwOldNodeIP, &pUser->wOldNodePort,
										 &newName, &pUser->dwNodeIP, &pUser->wNodePort,
										 &pUser->wLine, &pUser->dwFiles);

					
				}

				if((oldName != 0) && (newName != 0)){

					pUser->strOldName = oldName;
					pUser->strName    = newName;
					theApp.SendMessageTo(pS->strRoom, UWM_RENAME, 0, (LPARAM)pUser);
				}
				delete pUser;
			}
			else if((pS->wType == 0x00D2) || (pS->wType == 0x00D3)){

				theApp.SendMessageTo(pS->strRoom, UWM_OPMESSAGE, (WPARAM)pS->wType, (LPARAM)buff);
				
				CString strMsg = buff, strUser = "adminmsg";

				if(FilterMessage(strUser, strMsg)){

					strUser = " "; strMsg = " ";
					ZeroMemory(buff, pS->wLen);
				}

				len = FormatMXMessageNoID(buff, "S", (LPCTSTR)strMsg);
			}
			else if(pS->wType == 0x012D){

				theApp.SendMessageTo(pS->strRoom, UWM_ROOMRENAME, 0, (LPARAM)buff);
			}
			else if(pS->wType == 0x0190){

				theApp.SendMessageTo(pS->strRoom, UWM_REDIRECT, 0, (LPARAM)buff);
			}
			else if(pS->wType == 0x00C8){

				if(pS->wServerType == SERVER_WINMX353){

					pS->wServerType = SERVER_RCMS353;
					pS->strServer  = "RCMS on WinMX > 3.5x";
					pS->strVersion = "N/A";
					theApp.SendMessageTo(pS->strRoom, UWM_SERVERTYPE, SERVER_RCMS353, 0);
				}
				else if((pS->wServerType != SERVER_RCMS) && (pS->wServerType != SERVER_RCMS353)){

					pS->wServerType = SERVER_RCMS;
					pS->strServer	= "RCMS";
					pS->strVersion	= "N/A";
					theApp.SendMessageTo(pS->strRoom, UWM_SERVERTYPE, SERVER_RCMS, 0);
				}
			}
			else if(pS->wType == 0x9900){

				char buffer[16];
				ZeroMemory(buffer, 4);
				
				WORD wLen = FormatMXMessage(0x9901, (char*)&buffer, "B", 0);

				pS->dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, pS->dwUPKey);
				
				_orig_send(s, buffer, wLen, 0);
			}
			else if(pS->wType == 0x9902){

				ClientJoin* pIP = new ClientJoin();

				LPSTR lpName = 0, lpIP = 0, lpHost = 0;
				BYTE  btLevel = 0;
				//[UserName:N][PrimaryIP:4][PrimaryPort:2][Files:4][Connection:2][Rank:1][IP:N]


				if(ScanMessage(buff, pS->wLen, "SDWWWBSS", &lpName, &pIP->dwNodeIP, &pIP->wNodePort,
																   &pIP->dwFiles, &pIP->wLine, &btLevel,
																   &lpIP, &lpHost) < 7){

				}

				pIP->strSrcIP = lpIP;

				pIP->strName = lpName;

				if((btLevel >= 0) && (btLevel <= 2)){

					pIP->wLevel  = (WORD)btLevel;
				}
				else{

					pIP->wLevel  = 0;
				}

				theApp.SendMessageTo(pS->strRoom, UWM_IPCLIENT, 0, (LPARAM)pIP);
				delete pIP; 
			}
			else if(pS->wType == 0x9904){

				// mcs sloppy programing
				LTRACE("mcs shit");
			}
			else if(pS->wType == 0xFDE8){

				LTRACE("ping");
			}
			else if(pS->wType == 0x9905){


				char buffer[1024];
				ZeroMemory(buffer, 1024);
				WORD wLen = FormatMXMessage(0x9905, (char*)&buffer, "SS", (LPCTSTR)g_strVersion, STR_BENDMXVERSION);
				pS->dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, pS->dwUPKey);
			
				_orig_send(s, buffer, wLen, 0);

				LPSTR lpServer = 0, lpVer = 0;
				if(ScanMessage((char*)buff, pS->wLen, "SS", &lpServer, &lpVer) == 2){

					pS->strServer = lpServer;
					pS->strVersion = lpVer;
					if(pS->strServer == "MXTools Chat Server"){

						pS->wServerType = SERVER_MCS;
					}
					else if(pS->strServer.Find("roboServ", 0) >= 0){

						pS->wServerType = SERVER_ROSE;
					}
					else if(pS->strServer.Find("Ouka") >= 0){

						pS->wServerType = SERVER_OUKA;
					}
					else{

						pS->wServerType = SERVER_WINMX353;
					}
					theApp.SendMessageTo(pS->strRoom, UWM_SERVERTYPE, pS->wServerType, 0);

				}
			}
			pS->wType = 0;
			pS->wLen  = 0;

		}
		pS->dwMXDWKey = EncryptMXTCP((BYTE*)buff, n, pS->dwMXDWKey);
	}
	
	return n;
#endif // __BENDMX__
}

int WINAPI closesocket(SOCKET s)
{

#ifdef __BENDMX__
	PSOCKETMONITOR pS = 0;
	
	if(g_mSocketMap.Lookup(s, pS)){

		ASSERT(pS);

		g_mSocketMap.RemoveKey(s);
		LTRACE3("c Removeing socket 0x%X", s);

		delete pS;
		pS = NULL;
	}
	LTRACE3("(0x%X)  close socket", (int)s);
#endif
	return _orig_closesocket(s);
}


extern BOOL WildcardMatch(CString strString, CString strSearch, BOOL bStartWild, BOOL bCase);
extern CString FormatIP(DWORD dwIP);


void LogBlockedIP(const CString strIP, const CString strCall)
{


	CStdioFile file;

	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strMsg;
	
	strMsg.Format("%04d/%02d/%02d [%02d:%02d.%02d] Blocked IP %s in call %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strIP, strCall);

	TRY{

		if(!file.Open(g_sSettings.GetWorkingDir() + "\\bad-ip-log.txt" , 
			CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText)){
			
			return;
		}

		file.SeekToEnd();
		file.WriteString(strMsg);
		file.Flush();
		file.Close();
	}
	CATCH(CFileException, e){

	}END_CATCH;
}

int WINAPI connect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{

#ifdef __BENDMX__

	LTRACE3("(0x%X)  Connect socket", (int)s);

	SOCKADDR_IN* SockAddr = (SOCKADDR_IN*)name;

	BOOL bDummy = FALSE;
	CString strIP = FormatIP((DWORD)SockAddr->sin_addr.S_un.S_addr), strBlock;
	POSITION pos = g_aBlockedIPs.GetStartPosition();
	while(pos){

		g_aBlockedIPs.GetNextAssoc(pos, strBlock, bDummy);
		if(WildcardMatch(strIP, strBlock, FALSE, TRUE)){

			LogBlockedIP(strIP, "connect");
			WSASetLastError(WSAETIMEDOUT);
			return SOCKET_ERROR;
		}
	}

	int n = _orig_connect(s, name, namelen);


	SOCKETMONITOR *sm	= new SOCKETMONITOR;
	
	sm->socket		= s;
	sm->iState		= INIT_PENDING;
	sm->wType		= 0;
	sm->wLen		= 0;
	sm->dwUPKey		= 0;
	sm->dwDWKey		= 0;
	sm->dwMXUPKey	= 0;
	sm->dwMXDWKey	= 0;
	sm->dwOffset    = 0;
	sm->wServerType = SERVER_UNKNOWN;
	sm->dwID		= 0;
	sm->bHooked     = FALSE;
	g_mSocketMap.SetAt(s, sm);
    return n;
	
#else
	SOCKADDR_IN* SockAddr = (SOCKADDR_IN*)name;

	BOOL bDummy = FALSE;
	CString strIP = FormatIP((DWORD)SockAddr->sin_addr.S_un.S_addr), strBlock;
	POSITION pos = g_aBlockedIPs.GetStartPosition();
	while(pos){

		g_aBlockedIPs.GetNextAssoc(pos, strBlock, bDummy);
		if(WildcardMatch(strIP, strBlock, FALSE, TRUE)){

			LogBlockedIP(strIP, "connect");
			WSASetLastError(WSAETIMEDOUT);
			return SOCKET_ERROR;
		}
	}

	return _orig_connect(s, name, namelen);

#endif //__BENDMX__

}

SOCKET PASCAL accept(SOCKET a0, struct sockaddr* a1, int* a2)
{


	SOCKET socket = _orig_accept(a0, a1, a2);

	sockaddr_in* addr = (sockaddr_in*)a1;

	BOOL bDummy = FALSE;
	CString strIP = FormatIP((DWORD)addr->sin_addr.S_un.S_addr), strBlock;
	POSITION pos = g_aBlockedIPs.GetStartPosition();
	while(pos){

		g_aBlockedIPs.GetNextAssoc(pos, strBlock, bDummy);
		if(WildcardMatch(strIP, strBlock, FALSE, TRUE)){

			LogBlockedIP(strIP, "accept");
			WSASetLastError(WSAENETDOWN);
			return SOCKET_ERROR;
		}
	}


    return socket;
}

int PASCAL recvfrom(SOCKET a0, char* a1, int a2, int a3, struct sockaddr* a4, int* a5)
{

	SOCKADDR_IN* SockAddr = (SOCKADDR_IN*)a4;

	BOOL bDummy = FALSE;
	CString strIP = FormatIP((DWORD)SockAddr->sin_addr.S_un.S_addr), strBlock;
	POSITION pos = g_aBlockedIPs.GetStartPosition();
	while(pos){

		g_aBlockedIPs.GetNextAssoc(pos, strBlock, bDummy);
		if(WildcardMatch(strIP, strBlock, FALSE, TRUE)){

			LogBlockedIP(strIP, "recvfrom");
			WSASetLastError(WSAECONNABORTED);
			return SOCKET_ERROR ;
		}
	}

	return _orig_recvfrom(a0, a1, a2, a3, a4, a5);
}

int PASCAL sendto(SOCKET a0, const char* a1, int a2, int a3, const struct sockaddr* a4, int a5)
{

	SOCKADDR_IN* SockAddr = (SOCKADDR_IN*)a4;

	BOOL bDummy = FALSE;
	CString strIP = FormatIP((DWORD)SockAddr->sin_addr.S_un.S_addr), strBlock;
	POSITION pos = g_aBlockedIPs.GetStartPosition();
	while(pos){

		g_aBlockedIPs.GetNextAssoc(pos, strBlock, bDummy);
		if(WildcardMatch(strIP, strBlock, FALSE, TRUE)){

			LogBlockedIP(strIP, "sendto");
			WSASetLastError(WSAECONNABORTED);
			return SOCKET_ERROR ;
		}
	}

	return _orig_sendto(a0, a1, a2, a3, a4, a5);
}


/////////////////////////////////////////////////////////////////////////////
// CBendMXApp

BEGIN_MESSAGE_MAP(CBendMXApp, CWinApp)
	//{{AFX_MSG_MAP(CBendMXApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBendMXApp construction


CBendMXApp::CBendMXApp()
{
	
#ifdef __BENDMX__
	m_pBendMX = NULL;
	m_pInput  = NULL;
#endif

//	HookWinsockProcs();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBendMXApp object

BOOL CBendMXApp::InitInstance() 
{

#ifndef _NO_DEBUGTRACE
	LOG_SetLogFileName( g_sSettings.GetWorkingDir(FALSE) + _T( "\\BendMX-Debug-Log.txt" ) );
#endif

	ENTRY_TRACE("CBendMXApp::InitInstance()");

	g_sSettings.Load();
	
	o_hDLL = LoadLibrary(g_sSettings.GetWorkingDir() + "\\WS2_32r.dll");

	if(!o_hDLL){

		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		
		CString strError;
		strError.Format("Fatal Error: BendMX could not load the library\n\n%s\\WS2_32r.dll:\n%s\nPlease make sure the file is in your WinMX folder!",
			g_sSettings.GetWorkingDir(), (LPCTSTR*)lpMsgBuf);
		AfxMessageBox(strError, MB_ICONSTOP);

		LocalFree(lpMsgBuf);
		return FALSE;
	}


	CIni ini;
	ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\BendMX.dat");
	
	CString strIP; BOOL bDummy = TRUE;
	CString strLabel;

	for(int i = 1; i; i++){


		strLabel.Format("BadIP_%d", i);
		strIP = ini.GetValue("BadIPs", strLabel, "");
		if(strIP.IsEmpty()) break;

		g_aBlockedIPs.SetAt(strIP, bDummy);
	}


#ifdef __BENDMX__
	CFileStatus fStatus;
	if(CFile::GetStatus(g_sSettings.GetWorkingDir() + "\\WS2_32o.DLL", fStatus)){

		LTRACE("Detected RCMS (%s %s). Starting up in safemode", g_sSettings.GetWorkingDir(), fStatus.m_szFullName);
		CIni ini;
		ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\BendMX.dat");
		if(!ini.GetValue("Warnings", "RCMS", FALSE)){

			ini.SetValue("Warnings", "RCMS", TRUE);
			AfxMessageBox(IDS_RCMSWARNING, MB_ICONWARNING);
		}
		g_bRCMS = TRUE;
	}
	
	if(m_pBendMX == NULL){

		m_pBendMX = new CBendMXDlg();
		m_pBendMX->m_hInstance = m_hInstance;
		m_pBendMX->Create(IDD_BENDMX, NULL);
		m_pBendMX->ShowWindow(SW_HIDE);
		m_pMainWnd = m_pBendMX;
	}
	if(m_pInput == NULL){

		m_pInput = new CInputRequest();
		m_pInput->Create(IDD_UTIL, NULL);
		m_pInput->ShowWindow(SW_HIDE);
	}

#ifndef _DEBUG
	CSplash * pSplashWnd = new CSplash(IDB_BMX_LOGO,2500);
	pSplashWnd->Create();
#endif

#endif

	EXIT_TRACE("CBendMXApp::InitInstance()");
	return CWinApp::InitInstance();
}

#ifdef __BENDMX__
int CBendMXApp::ExitInstance() 
{

	ENTRY_TRACE("CBendMXApp::ExitInstance()");

	if(!g_sSettings.GetSloppy()){

		if(m_pInput != NULL){
		
			if(m_pInput->m_dwID != -99){

				::DestroyWindow(m_pInput->m_hWnd);
				//m_pInput->ShowWindow(SW_HIDE);
				//m_pInput->DestroyWindow();
			}
			delete m_pInput;
			m_pInput = NULL;
		}
		if(m_pBendMX != NULL){
		
			if(m_pBendMX->m_pBot != NULL){

				::DestroyWindow(m_pBendMX->m_hWnd);
				//m_pBendMX->ShowWindow(SW_HIDE);
				//m_pBendMX->DestroyWindow();
			}
			delete m_pBendMX;
			m_pBendMX = NULL;
		}
	}

	POSITION pos = g_mSocketMap.GetStartPosition();

	while(pos){

		PSOCKETMONITOR p = NULL;
		SOCKET		   s = NULL;

		g_mSocketMap.GetNextAssoc(pos, s, p);
		if(p == NULL) continue;

		g_mSocketMap.RemoveKey(s);
		delete p;
		p = NULL;
	}

	EXIT_TRACE("CBendMXApp::ExitInstance()");
	return CWinApp::ExitInstance();
}
#endif

#ifdef __BENDMX__

void CBendMXApp::InputMessage(CString strMessage, CString strChat, BOOL bEnter)
{

	ENTRY_TRACE("CBendMXApp::InputMessage() [CString version]");
	if(m_pBendMX != NULL){

		m_pBendMX->InputMessage(strMessage, strChat, bEnter);
	}
	EXIT_TRACE("CBendMXApp::InputMessage() [CString version]");
}

void CBendMXApp::InputMessage(BOOL bDirectSend, CString strMessage, DWORD dwID)
{

	ENTRY_TRACE("CBendMXApp::InputMessage()");

	if(!bDirectSend && (m_pBendMX != NULL)){

		TRACE("Indirect send\n");
		m_pBendMX->InputMessage(strMessage, dwID);
	}
	else{

		POSITION pos = g_mSocketMap.GetStartPosition();
		TRACE("Direct send\n");
		while(pos){

			PSOCKETMONITOR p = NULL;
			SOCKET		   s = NULL;

			g_mSocketMap.GetNextAssoc(pos, s, p);
			if(p == NULL) continue;
			if(p->dwID != dwID) continue;

			SendText(p, strMessage);
			break;
		}
	}
	EXIT_TRACE("CBendMXApp::InputMessage()");
}

void CBendMXApp::InputServerInfo(DWORD dwID)
{

	if(m_pBendMX == NULL) return;


	/*CString strRoom;
	for(int i = 0; i < m_pBendMX->m_aDlgs.GetSize(); i++){

		if(m_pBendMX->m_aDlgs[i]->m_dwID == dwID){

			strRoom = m_pBendMX->m_aDlgs[i]->m_strRoomShort;
			break;
		}
	}

	if(strRoom.IsEmpty()) return;*/

	POSITION pos = g_mSocketMap.GetStartPosition();

	while(pos){

		PSOCKETMONITOR p = NULL;
		SOCKET		   s = NULL;

		g_mSocketMap.GetNextAssoc(pos, s, p);
		if(p == NULL) continue;
		if(p->dwID != dwID) continue;

		//if(p->strRoom.Find(strRoom, 0) != 0) continue;

		CString strOut;
		strOut.Format(g_i18n.GetString(IDS_ROOMVERSION), p->strServer, p->strVersion);
		//InputMessage(strOut, dwID);
		SendText(p, strOut);
		break;
	}

}

void CBendMXApp::SendMessageTo(CString strTitle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if(m_pBendMX == NULL) return;

	HWND hWnd = NULL;
	if(m_pBendMX->m_aDlgMap.Lookup(strTitle, hWnd)){

		ASSERT(hWnd != NULL);

		//TRACE("Sending message\n");
		::SendMessage(hWnd, uMsg, wParam, lParam);
	}
}

void CBendMXApp::ShowAdminWindow(CString strRoom)
{

	if(m_pBendMX == NULL) return;

	HWND hWnd = NULL;
	if(m_pBendMX->m_aDlgMap.Lookup(strRoom, hWnd)){

		ASSERT(hWnd != NULL);

		//TRACE("Sending message\n");
		//::SendMessage(hWnd, uMsg, wParam, lParam);
		::ShowWindow(hWnd, SW_SHOWNORMAL|SW_RESTORE);
	}
}

void CBendMXApp::RemoveHook(CString strChat)
{

	ENTRY_TRACE("CBendMXApp::RemoveHook() [CString version]");
	if(m_pBendMX != NULL){

		m_pBendMX->RemoveHook(strChat);
	}
	EXIT_TRACE("CBendMXApp::RemoveHook() [CString version]");
}


BOOL CBendMXApp::DoesRoomExist(CString &strRoom)
{

	ENTRY_TRACE("CBendMXApp::DoesRoomExist()");
	if(m_pBendMX != NULL){

		EXIT_TRACE("CBendMXApp::DoesRoomExist(). return 1");
		return m_pBendMX->DoesRoomExist(strRoom);
	}
	
	EXIT_TRACE("CBendMXApp::DoesRoomExist(). return 0");
	return FALSE;
}

void CBendMXApp::OnEnter(CString strRoom, CString strUser, int nFiles)
{

	ENTRY_TRACE("CBendMXApp:OnEnter()");
	if(m_pBendMX != NULL){

		if(m_pBendMX->m_pBot->m_bBot){

			if(m_pBendMX->m_pBot->m_strRoom != strRoom) return;
			m_pBendMX->m_pBot->OnEnter(strUser, nFiles);
		}
	}
	EXIT_TRACE("CBendMXApp::OnEnter()");
}

void CBendMXApp::OnMessage(CString strRoom, CString strName, CString strMessage)
{

	ENTRY_TRACE("CBendMXApp::OnMessage()");
	if(m_pBendMX != NULL){

		if(m_pBendMX->m_pBot->m_bBot){

			if(m_pBendMX->m_pBot->m_strRoom != strRoom){
				
				TRACE("Wrong room (%s / %s)\n", m_pBendMX->m_pBot->m_strRoom, strRoom);
				return;
			}
			m_pBendMX->m_pBot->OnMessage(strName, strMessage);
		}
	}
	EXIT_TRACE("CBendMXApp::OnMessage()");
}


BOOL CBendMXApp::SendText(PSOCKETMONITOR pS, CString strText)
{

	char buffer[1024];
	ZeroMemory(buffer, 1024);
	WORD wLen = 0;

	if(pS->wServerType < SERVER_WINMX353){

		BOOL bAction = FALSE;
		if(strText.Find("/me ", 0) == 0){

			strText = strText.Mid(4);
			bAction = TRUE;
		}
		else if(strText.Find("/action ", 0) == 0){

			strText = strText.Mid(8);
			bAction = TRUE;
		}
		else if(strText.Find("/emote ", 0) == 0){

			strText = strText.Mid(7);
			bAction = TRUE;
		}
		if(strText.IsEmpty()) return TRUE;

		wLen = FormatMXMessage((bAction ? 0x00CA : 0x00C8), (char*)&buffer, "S", (LPCSTR)strText);
	}
	else{

		wLen = FormatMXMessage(0x1450, (char*)&buffer, "S", (LPCSTR)strText);
	}

	pS->dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, pS->dwUPKey);
	
	_orig_send(pS->socket, buffer, wLen, 0);

	return TRUE;

	//_orig_send(s, buffer, wLen, 0);
}



void CBendMXApp::SearchChatWnd(CString strChat)
{

	ENTRY_TRACE("CBendMXApp::SearchChatWnd(...)");

	if(m_pBendMX != NULL){

		//m_pBendMX->SearchChatWnd(strChat);
		::SendMessage(m_pBendMX->m_hWnd, UWM_ADDCHAT, 0, (LPARAM)(LPCTSTR)strChat);
	}

	EXIT_TRACE("CBendMXApp::SearchChatWnd(...)");
}

#endif //__BENDMX__

INT QueryModuleDebugLevel( const CString& strModule )
{
	int nReturn = 0;

	TRACE("MaxDebugLevel: %d / %X\n", 0x01|0x02|0x04|0x08|0x10|0x20);
	// Get the debug level for the specified module
	// from the CDex.ini file in the Debug section
	nReturn = ::GetPrivateProfileInt(	_T( "Debug" ),
										strModule,
										0,
										g_sSettings.GetWorkingDir(FALSE) + _T( "\\BendMX.dat" ) );

	return nReturn;

}



