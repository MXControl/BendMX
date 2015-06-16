; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=12
Class1=CBendMXApp
LastClass=CChatDlg
NewFileInclude2=#include "BendMX.h"
ResourceCount=20
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_ABOUT
Class2=CBendMXDlg
LastTemplate=CDialog
Class3=CGeneralDlg
Resource2=IDD_COLORS
Class4=CColorDlg
Resource3=IDD_SIMPLEBOT
Resource4=IDD_GENERAL
Class5=CMessageDlg
Class6=CFilterDlg
Resource5=IDD_BENDMX
Class7=CChatDlg
Resource6=IDD_FILTER
Class8=CAboutDlg
Resource7=IDD_MESSAGES
Class9=CInputRequest
Class10=CKickit
Class11=CSimpleBot
Resource8=IDD_CHATDLG
Resource9=IDD_UTIL
Class12=CWelcomeMsg
Resource10=IDD_WELCOMEMSGS
Resource11=IDD_SIMPLEBOT (Englisch (USA))
Resource12=IDD_BENDMX (Englisch (USA))
Resource13=IDD_GENERAL (Englisch (USA))
Resource14=IDD_MESSAGES (Englisch (USA))
Resource15=IDD_COLORS (Englisch (USA))
Resource16=IDD_CHATDLG (Englisch (USA))
Resource17=IDD_ABOUT (Englisch (USA))
Resource18=IDD_UTIL (Englisch (USA))
Resource19=IDD_FILTER (Englisch (USA))
Resource20=IDD_WELCOMEMSGS (Englisch (USA))

[CLS:CBendMXApp]
Type=0
HeaderFile=BendMX.h
ImplementationFile=BendMX.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CBendMXApp

[DLG:IDD_BENDMX]
Type=1
Class=CBendMXDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_APPLY,button,1342242816
Control3=IDC_PREFBAR,SysListView32,1350688772
Control4=IDC_STATIC_CONFIG_RECT,static,1342308352

[DLG:IDD_GENERAL]
Type=1
Class=CGeneralDlg
ControlCount=19
Control1=IDC_STATIC_LANG,button,1342177287
Control2=IDC_LANG,combobox,1344340227
Control3=IDC_STATIC_PLUGINS,button,1342177287
Control4=IDC_PLUGINLIST,SysListView32,1350664193
Control5=IDC_REMOVE,button,1342242816
Control6=IDC_RELOAD_ALL,button,1342242816
Control7=IDC_UNLOAD_ALL,button,1342242816
Control8=IDC_STATIC_OPTIONS,button,1342177287
Control9=IDC_ENABLE_HISTORY,button,1342242819
Control10=IDC_HISTORDEPTH,edit,1350631552
Control11=IDC_STATIC_DEPTH,static,1342308352
Control12=IDC_STATIC_MISC,button,1342177287
Control13=IDC_REPAIR,button,1342242816
Control14=IDC_FONTHELP,button,1342242816
Control15=IDC_SOUNDEFFECT,button,1342242819
Control16=IDC_STATIC_TIMERESOLUTION,static,1342308352
Control17=IDC_RESOLUTION,edit,1350631552
Control18=IDC_STATIC_MS,static,1342308352
Control19=IDC_MORONMODE,button,1342242819

[CLS:CBendMXDlg]
Type=0
HeaderFile=BendMXDlg.h
ImplementationFile=BendMXDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CBendMXDlg

[CLS:CGeneralDlg]
Type=0
HeaderFile=GeneralDlg.h
ImplementationFile=GeneralDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_NICKNAME

[CLS:CColorDlg]
Type=0
HeaderFile=ColorDlg.h
ImplementationFile=ColorDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ENABLEMSGCOLOR

[DLG:IDD_FILTER]
Type=1
Class=CFilterDlg
ControlCount=20
Control1=IDC_STATIC_INCOMING,button,1342177287
Control2=IDC_BLOCK_HTTP,button,1342242819
Control3=IDC_BLOCK_ASCII_ART,button,1342242819
Control4=IDC_BLOCK_PRIVATE_RCMS,button,1342242819
Control5=IDC_BLOCK_TOPIC,button,1342242819
Control6=IDC_BLOCK_MOTD,button,1342242819
Control7=IDC_BLOCK_MSG,button,1476460547
Control8=IDC_BLOCK_ADMIN,button,1342242819
Control9=IDC_STATIC_TRUSTED,button,1342177287
Control10=IDC_TRUSTED_ADD,edit,1350631552
Control11=IDC_TRUSTED_ADD_BTN,button,1342242816
Control12=IDC_TRUSTED_LIST,listbox,1352728835
Control13=IDC_TRUSTED_REM_BTN,button,1342242816
Control14=IDC_STATIC_WORDFILTER,button,1342177287
Control15=IDC_STARS,button,1342242819
Control16=IDC_FILTERXTREM,button,1342242819
Control17=IDC_FILTER_ADD,edit,1350631552
Control18=IDC_FILTER_ADD_BTN,button,1342242816
Control19=IDC_FILTER_LIST,listbox,1352728835
Control20=IDC_FILTER_REM_BTN,button,1342242816

[DLG:IDD_MESSAGES]
Type=1
Class=CMessageDlg
ControlCount=14
Control1=IDC_STATIC_QUICK,button,1342177287
Control2=IDC_MSG_ADD,edit,1350631552
Control3=IDC_MSG_ADD_BTN,button,1342242816
Control4=IDC_QUICKCMDS,listbox,1352728833
Control5=IDC_QUICKREM,button,1342242816
Control6=IDC_QUICKUP,button,1342242816
Control7=IDC_QUICKDOWN,button,1342242816
Control8=IDC_STATIC_WINAMP,button,1342177287
Control9=IDC_STATIC_MUSIC,static,1342308352
Control10=IDC_NOWLISTENING,edit,1350631552
Control11=IDC_STATIC_VIDEO,static,1342308352
Control12=IDC_NOW_WATCHING,edit,1350631552
Control13=IDC_STATIC_VARS,static,1342308352
Control14=IDC_STATIC_VARS2,static,1342308352

[CLS:CMessageDlg]
Type=0
HeaderFile=MessageDlg.h
ImplementationFile=MessageDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CFilterDlg]
Type=0
HeaderFile=FilterDlg.h
ImplementationFile=FilterDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CFilterDlg

[DLG:IDD_CHATDLG]
Type=1
Class=CChatDlg
ControlCount=6
Control1=IDC_USERLIST,SysListView32,1350631557
Control2=IDC_TOP,button,1342242819
Control3=IDC_HIDE,button,1342242816
Control4=IDC_TRANSPARENCY,msctls_trackbar32,1342242840
Control5=IDC_STATIC_TRANS,static,1342308352
Control6=IDC_STATIC_100,static,1342308354

[CLS:CChatDlg]
Type=0
HeaderFile=ChatDlg.h
ImplementationFile=ChatDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STATIC_100

[DLG:IDD_ABOUT]
Type=1
Class=CAboutDlg
ControlCount=12
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC_BENDMX,static,1342308353
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LPGPL,button,1342242816
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342177296
Control11=IDC_STATIC,static,1342177296
Control12=IDC_STATIC,static,1342308353

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAboutDlg
VirtualFilter=dWC

[DLG:IDD_UTIL]
Type=1
Class=CInputRequest
ControlCount=4
Control1=IDC_STATIC_ENTERTEXT,static,1342308352
Control2=IDC_TEXT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CInputRequest]
Type=0
HeaderFile=InputRequest.h
ImplementationFile=InputRequest.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInputRequest

[DLG:IDD_SIMPLEBOT]
Type=1
Class=CSimpleBot
ControlCount=26
Control1=IDC_STATIC_BOT,button,1342177287
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC_WARNINGBOT,static,1342308353
Control4=IDC_STATIC_ROOMNAME,static,1342308352
Control5=IDC_BOTROOM,edit,1350631552
Control6=IDC_STATIC_INTERVAL,static,1342308352
Control7=IDC_ANNOUNCE,edit,1350631552
Control8=IDC_STATIC_EVERY,static,1342308352
Control9=IDC_INTERVAL,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LIMIT,button,1342242819
Control12=IDC_STATIC_TRIGGER,static,1342308352
Control13=IDC_STATIC_RESPONSE,static,1342308352
Control14=IDC_TRIGGER,edit,1350631552
Control15=IDC_RESPONSE,edit,1350631552
Control16=IDC_ADDTRIGGER,button,1342242816
Control17=IDC_COMMANDS,SysListView32,1350664197
Control18=IDC_REMOVECMD,button,1342242816
Control19=IDC_LEECHKICK,button,1342242819
Control20=IDC_ENABLEBOT,button,1342242819
Control21=IDC_STATIC,static,1342308352
Control22=IDC_WELCOME,edit,1350631552
Control23=IDC_STATIC_WELCOME,static,1342308352
Control24=IDC_STATIC_LEAVEEMPTY,static,1342308352
Control25=IDC_STATIC_SETNICKNAME,static,1342308353
Control26=IDC_EDITCMD,button,1342242816

[CLS:CKickit]
Type=0
HeaderFile=Kickit.h
ImplementationFile=Kickit.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BINDADDR
VirtualFilter=dWC

[CLS:CSimpleBot]
Type=0
HeaderFile=SimpleBot.h
ImplementationFile=SimpleBot.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_WELCOME
VirtualFilter=dWC

[DLG:IDD_COLORS]
Type=1
Class=?
ControlCount=11
Control1=65535,button,1342177287
Control2=IDC_STATIC_OPS_PRE,static,1342308352
Control3=IDC_PRE_REG_ADD,edit,1350631552
Control4=IDC_REG_PREFIX,listbox,1352728835
Control5=IDC_OP_ADD,button,1342242816
Control6=IDC_OP_REM,button,1342242816
Control7=IDC_STATIC_HOST_PREF,static,1342308352
Control8=IDC_PRE_HOST_ADD,edit,1350631552
Control9=IDC_HOST_PREFIX,listbox,1352728835
Control10=IDC_OP_ADD2,button,1342242816
Control11=IDC_OP_REM2,button,1342242816

[CLS:CWelcomeMsg]
Type=0
HeaderFile=WelcomeMsg.h
ImplementationFile=WelcomeMsg.cpp
BaseClass=CDialog
Filter=D
LastObject=CWelcomeMsg
VirtualFilter=dWC

[DLG:IDD_WELCOMEMSGS]
Type=1
Class=CWelcomeMsg
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CHANNELNAME,edit,1350631552
Control3=IDC_ADDCHANNEL,button,1342242816
Control4=IDC_CHANNELLIST,listbox,1352728835
Control5=IDC_REMOVECHANNEL,button,1342242816
Control6=IDC_ALLCHANNELS,button,1342242819
Control7=IDC_STATIC_GREETING,static,1342308352
Control8=IDC_GREETING,edit,1350631552
Control9=IDC_ADDCHANNEL2,button,1342242816
Control10=IDC_WELCOMES,listbox,1352728835
Control11=IDC_REMOVECHANNEL2,button,1342242816
Control12=IDC_STATIC,button,1342177287

[DLG:IDD_BENDMX (Englisch (USA))]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_APPLY,button,1342242816
Control3=IDC_PREFBAR,SysListView32,1350688772
Control4=IDC_STATIC_CONFIG_RECT,static,1342308352

[DLG:IDD_GENERAL (Englisch (USA))]
Type=1
Class=?
ControlCount=19
Control1=IDC_STATIC_LANG,button,1342177287
Control2=IDC_LANG,combobox,1344340227
Control3=IDC_STATIC_PLUGINS,button,1342177287
Control4=IDC_PLUGINLIST,SysListView32,1350664193
Control5=IDC_REMOVE,button,1342242816
Control6=IDC_RELOAD_ALL,button,1342242816
Control7=IDC_UNLOAD_ALL,button,1342242816
Control8=IDC_STATIC_OPTIONS,button,1342177287
Control9=IDC_ENABLE_HISTORY,button,1342242819
Control10=IDC_HISTORDEPTH,edit,1350631552
Control11=IDC_STATIC_DEPTH,static,1342308352
Control12=IDC_STATIC_MISC,button,1342177287
Control13=IDC_REPAIR,button,1342242816
Control14=IDC_FONTHELP,button,1342242816
Control15=IDC_SOUNDEFFECT,button,1342242819
Control16=IDC_STATIC_TIMERESOLUTION,static,1342308352
Control17=IDC_RESOLUTION,edit,1350631552
Control18=IDC_STATIC_MS,static,1342308352
Control19=IDC_MORONMODE,button,1342242819

[DLG:IDD_FILTER (Englisch (USA))]
Type=1
Class=?
ControlCount=20
Control1=IDC_STATIC_INCOMING,button,1342177287
Control2=IDC_BLOCK_HTTP,button,1342242819
Control3=IDC_BLOCK_ASCII_ART,button,1342242819
Control4=IDC_BLOCK_PRIVATE_RCMS,button,1342242819
Control5=IDC_BLOCK_TOPIC,button,1342242819
Control6=IDC_BLOCK_MOTD,button,1342242819
Control7=IDC_BLOCK_MSG,button,1476460547
Control8=IDC_BLOCK_ADMIN,button,1342242819
Control9=IDC_STATIC_TRUSTED,button,1342177287
Control10=IDC_TRUSTED_ADD,edit,1350631552
Control11=IDC_TRUSTED_ADD_BTN,button,1342242816
Control12=IDC_TRUSTED_LIST,listbox,1352728835
Control13=IDC_TRUSTED_REM_BTN,button,1342242816
Control14=IDC_STATIC_WORDFILTER,button,1342177287
Control15=IDC_STARS,button,1342242819
Control16=IDC_FILTERXTREM,button,1342242819
Control17=IDC_FILTER_ADD,edit,1350631552
Control18=IDC_FILTER_ADD_BTN,button,1342242816
Control19=IDC_FILTER_LIST,listbox,1352728835
Control20=IDC_FILTER_REM_BTN,button,1342242816

[DLG:IDD_MESSAGES (Englisch (USA))]
Type=1
Class=?
ControlCount=14
Control1=IDC_STATIC_QUICK,button,1342177287
Control2=IDC_MSG_ADD,edit,1350631552
Control3=IDC_MSG_ADD_BTN,button,1342242816
Control4=IDC_QUICKCMDS,listbox,1352728833
Control5=IDC_QUICKREM,button,1342242816
Control6=IDC_QUICKUP,button,1342242816
Control7=IDC_QUICKDOWN,button,1342242816
Control8=IDC_STATIC_WINAMP,button,1342177287
Control9=IDC_STATIC_MUSIC,static,1342308352
Control10=IDC_NOWLISTENING,edit,1350631552
Control11=IDC_STATIC_VIDEO,static,1342308352
Control12=IDC_NOW_WATCHING,edit,1350631552
Control13=IDC_STATIC_VARS,static,1342308352
Control14=IDC_STATIC_VARS2,static,1342308352

[DLG:IDD_CHATDLG (Englisch (USA))]
Type=1
Class=?
ControlCount=6
Control1=IDC_USERLIST,SysListView32,1350631557
Control2=IDC_TOP,button,1342242819
Control3=IDC_HIDE,button,1342242816
Control4=IDC_TRANSPARENCY,msctls_trackbar32,1476460568
Control5=IDC_STATIC_TRANS,static,1342308352
Control6=IDC_STATIC_100,static,1342308354

[DLG:IDD_ABOUT (Englisch (USA))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC_BENDMX,static,1342308353
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LPGPL,button,1342242816
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342177296
Control11=IDC_STATIC,static,1342177296
Control12=IDC_STATIC,static,1342308353

[DLG:IDD_UTIL (Englisch (USA))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC_ENTERTEXT,static,1342308352
Control2=IDC_TEXT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_SIMPLEBOT (Englisch (USA))]
Type=1
Class=?
ControlCount=26
Control1=IDC_STATIC_BOT,button,1342177287
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC_WARNINGBOT,static,1342308353
Control4=IDC_STATIC_ROOMNAME,static,1342308352
Control5=IDC_BOTROOM,edit,1350631552
Control6=IDC_STATIC_INTERVAL,static,1342308352
Control7=IDC_ANNOUNCE,edit,1350631552
Control8=IDC_STATIC_EVERY,static,1342308352
Control9=IDC_INTERVAL,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LIMIT,button,1342242819
Control12=IDC_STATIC_TRIGGER,static,1342308352
Control13=IDC_STATIC_RESPONSE,static,1342308352
Control14=IDC_TRIGGER,edit,1350631552
Control15=IDC_RESPONSE,edit,1350631552
Control16=IDC_ADDTRIGGER,button,1342242816
Control17=IDC_COMMANDS,SysListView32,1350664197
Control18=IDC_REMOVECMD,button,1342242816
Control19=IDC_LEECHKICK,button,1342242819
Control20=IDC_ENABLEBOT,button,1342242819
Control21=IDC_STATIC,static,1342308352
Control22=IDC_WELCOME,edit,1350631552
Control23=IDC_STATIC_WELCOME,static,1342308352
Control24=IDC_STATIC_LEAVEEMPTY,static,1342308352
Control25=IDC_STATIC_SETNICKNAME,static,1342308353
Control26=IDC_EDITCMD,button,1342242816

[DLG:IDD_COLORS (Englisch (USA))]
Type=1
Class=?
ControlCount=11
Control1=65535,button,1342177287
Control2=IDC_STATIC_OPS_PRE,static,1342308352
Control3=IDC_PRE_REG_ADD,edit,1350631552
Control4=IDC_REG_PREFIX,listbox,1352728835
Control5=IDC_OP_ADD,button,1342242816
Control6=IDC_OP_REM,button,1342242816
Control7=IDC_STATIC_HOST_PREF,static,1342308352
Control8=IDC_PRE_HOST_ADD,edit,1350631552
Control9=IDC_HOST_PREFIX,listbox,1352728835
Control10=IDC_OP_ADD2,button,1342242816
Control11=IDC_OP_REM2,button,1342242816

[DLG:IDD_WELCOMEMSGS (Englisch (USA))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CHANNELNAME,edit,1350631552
Control3=IDC_ADDCHANNEL,button,1342242816
Control4=IDC_CHANNELLIST,listbox,1352728835
Control5=IDC_REMOVECHANNEL,button,1342242816
Control6=IDC_ALLCHANNELS,button,1342242819
Control7=IDC_STATIC_GREETING,static,1342308352
Control8=IDC_GREETING,edit,1350631552
Control9=IDC_ADDCHANNEL2,button,1342242816
Control10=IDC_WELCOMES,listbox,1352728835
Control11=IDC_REMOVECHANNEL2,button,1342242816
Control12=IDC_STATIC,button,1342177287

