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

#ifndef OUT_PLUGIN_H___INCLUDED
#define OUT_PLUGIN_H___INCLUDED

#define VERSION 0x400

typedef struct 
{

	// data membmers of the API
	int  nVersion;				// which api vertsion?
	int  nID;					// unique ID of the plugin
	_TCHAR *cDescription;		// description of plugin, with version string
	_TCHAR *cAuthor;            // author of plugin

	HINSTANCE hDllInstance;		// DLL instance handle (filled in by BendMX)
	
	HMENU hCfgMenu;			// Configuration submenu of this plugin.
				    		// Set it to NULL if you dont plan to use it

	HMENU hUserMenu;        // userlist submenu of this plugin.
							// Set it to NULL if you dont plan to use it.

	HMENU hChatMenu;        // chatwindow submenu of this plugin.
							// Set it to NULL if you dont plan to use it.
	// initialisation functions

	////////////////////////////////////////////////////////////////////////
	// void Init(...) - will be called after loading the plugin and after
	// filling in the hMainWindow and hDllInstance datamembers
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Init)(void);

	////////////////////////////////////////////////////////////////////////
	// void Quit(void) - will be called before unloading the plugin 
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Quit)(void);

	////////////////////////////////////////////////////////////////////////
	// void OnEnterChannel() - will be called when you enter a channel
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  LPCTSTR lpszRoom - pointer to a string containing the room name
	////////////////////////////////////////////////////////////////////////
	void (*OnEnterChannel)(DWORD dwID, LPCTSTR lpszRoom);

	////////////////////////////////////////////////////////////////////////
	// void OnEnterUser() - will be called when a user enters a channel 
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  LPCTSTR lpszusernam - pointer to a string containing the username
	////////////////////////////////////////////////////////////////////////
	void (*OnEnterUser)(DWORD dwID, LPCTSTR lpszUsername);

	////////////////////////////////////////////////////////////////////////
	// void OnEnterUser() - will be called when a user enters a channel 
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  LPCTSTR lpszusernam - pointer to a string containing the username
	////////////////////////////////////////////////////////////////////////
	void (*OnMessage)(DWORD dwID, CString strName, CString strMessage, BOOL bIsAction);

	////////////////////////////////////////////////////////////////////////
	// void OnOpMessage() - will be called when a a operator message is recieved 
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  CString strMessage - 
	////////////////////////////////////////////////////////////////////////
	void (*OnOpMessage)(DWORD dwID, CString strMessage);

	////////////////////////////////////////////////////////////////////////
	// void OnLeaveUser() - will be called when a user leaves a channel 
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  LPCTSTR lpszusernam - pointer to a string containing the username
	////////////////////////////////////////////////////////////////////////
	void (*OnLeaveUser)(DWORD dwID, LPCTSTR lpszUsername);

	////////////////////////////////////////////////////////////////////////
	// void OnCloseChannel() - will be called after a channel is closed
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel that is closed
	////////////////////////////////////////////////////////////////////////
	void (*OnCloseChannel)(DWORD dwID);

	////////////////////////////////////////////////////////////////////////
	// void OnInputHook() - will be called when a user enters a message to
	//                      the chat edit control
	//
	// Parameters:
	//  DWORD dwIP  - identnumber of the edit control
	//  CString *pInput - Pointer to a string containing the input
	////////////////////////////////////////////////////////////////////////
	void (*OnInputHook)(DWORD dwIP, CString *pInput);
	
	////////////////////////////////////////////////////////////////////////
	// BOOL OnPrepareMenu - BendMX calls this function whenever a menu is about to 
	//                    be displayed. Create you menu in here and set the corresponding
	//                    handle (hCfgMenu or hUserMenu).
	//
	// Parameters:
	//  DWORD dwMenuID    - 2 if chat menu is about to be displayed
	//                    - 1 if config menu is about to be displayed
	//                    - 0 if userlist menu is about to be displayed
	////////////////////////////////////////////////////////////////////////
	void (*OnPrepareMenu)(DWORD dwMenuID);

	////////////////////////////////////////////////////////////////////////
	// BOOL OnMenuCommand - BendMX calls this function whenever the user selects
	//                    an item from the userlist, or the settings menu.
	//
	// Parameters:
	//  UINT  nCmdID      - command id of the menu item selected
	//  DWORD dwChannelID - identnumber of the channel (0 when settins menu)
	//  LPCTSTR lpszName  - name of the user selected in the userlist 
	//                     (0 when not userlist menu)
	////////////////////////////////////////////////////////////////////////
	void (*OnMenuCommand)(UINT nCmdID, DWORD dwChannelID, LPCTSTR lpszName);

	////////////////////////////////////////////////////////////////////////
	// BOOL InputMesage - You must not implement this function
	//                    Call this function to input a message to a channel
	//
	// Parameters:
	//  DWORD dwID - identnumber of the channel
	//  LPCTSTR lpszMessage - pointer to a string containing the message
	////////////////////////////////////////////////////////////////////////
	BOOL (*InputMessage)(DWORD dwID, LPCTSTR lpszMessage);

} BPLUGIN, *PBPLUGIN;


typedef BPLUGIN*	(*GETPLUGINAPI)(void);

#endif