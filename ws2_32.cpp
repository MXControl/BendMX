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

   /////
 //
//
// This file contains all exported functions of the original WS2_32.dll and forwards them 
// to the system DLL :-)
// Was quite some typing :rolleyes:
// 
 //
  //////

#include "stdafx.h"
#include "BendMX.h"

#ifndef __BENDMX__
#include <winsock2.h>
#endif

extern HINSTANCE o_hDLL;

#pragma comment(linker,"/IGNORE:4273")

// typedefinitions for GetProcAddress typecasting :-)

typedef SOCKET (PASCAL *o_accept)(SOCKET a0, struct sockaddr* a1, int* a2);
typedef int (PASCAL *o_bind)(SOCKET a0, const struct sockaddr* a1, int a2);
typedef int (PASCAL *o_closesocket)(SOCKET a0);
typedef int (PASCAL *o_connect)(SOCKET a0, const struct sockaddr* a1, int a2);
typedef int (PASCAL *o_getpeername)(SOCKET a0, struct sockaddr* a1, int* a2);
typedef int (PASCAL *o_getsockname)(SOCKET a0, struct sockaddr* a1, int* a2);
typedef int (PASCAL *o_getsockopt)(SOCKET a0, int a1, int a2, char* a3, int* a4);
typedef u_long (PASCAL *o_htonl)(u_long a0);
typedef u_short (PASCAL *o_htons)(u_short a0);
typedef int (PASCAL *o_ioctlsocket)(SOCKET a0, long a1, u_long * a2);
typedef unsigned long (PASCAL *o_inet_addr)(const char* a0);
typedef char* (PASCAL *o_inet_ntoa)(struct in_addr a0);
typedef int (PASCAL *o_listen)(SOCKET a0, int a1);
typedef u_long (PASCAL *o_ntohl)(u_long a0);
typedef u_short (PASCAL *o_ntohs)(u_short a0);
typedef int (PASCAL *o_recv)(SOCKET a0, char* a1, int a2, int a3);
typedef int (PASCAL *o_recvfrom)(SOCKET a0, char* a1, int a2, int a3, struct sockaddr* a4, int* a5);
typedef int (PASCAL *o_select)(int a0, fd_set* a1, fd_set* a2, fd_set* a3, const struct timeval* a4);
typedef int (PASCAL *o_send)(SOCKET a0, const char* a1, int a2, int a3);
typedef int (PASCAL *o_sendto)(SOCKET a0, const char* a1, int a2, int a3, const struct sockaddr* a4, int a5);
typedef int (PASCAL *o_setsockopt)(SOCKET a0, int a1, int a2, const char* a3, int a4);
typedef int (PASCAL *o_shutdown)(SOCKET a0, int a1);
typedef SOCKET (PASCAL *o_socket)(int a0, int a1, int a2);
typedef int  (PASCAL *o_WSApSetPostRoutine)(int a0);
typedef WSAEVENT (WSAAPI *o_WPUCompleteOverlappedRequest)(SOCKET a0, LPWSAOVERLAPPED a1, DWORD a2, DWORD a3, LPINT a4);
typedef SOCKET (WINAPI *o_WSAAccept)(SOCKET a0, struct sockaddr * a1, LPINT a2, LPCONDITIONPROC a3, DWORD a4);
typedef INT  (WINAPI *o_WSAAddressToStringA)(LPSOCKADDR a0, DWORD a1, LPWSAPROTOCOL_INFOA a2, LPSTR a3, LPDWORD a4);
typedef INT  (WINAPI *o_WSAAddressToStringW)(LPSOCKADDR a0, DWORD a1, LPWSAPROTOCOL_INFOW a2, LPWSTR a3, LPDWORD a4);
typedef BOOL  (WINAPI *o_WSACloseEvent)(WSAEVENT a0);
typedef int  (WINAPI *o_WSAConnect)(SOCKET a0, const struct sockaddr * a1, int a2, LPWSABUF a3, LPWSABUF a4, LPQOS a5, LPQOS a6);
typedef WSAEVENT  (WINAPI *o_WSACreateEvent)(void);
typedef int  (WINAPI *o_WSADuplicateSocketA)(SOCKET a0, DWORD a1, LPWSAPROTOCOL_INFOA a2);
typedef int  (WINAPI *o_WSADuplicateSocketW)(SOCKET a0, DWORD a1, LPWSAPROTOCOL_INFOW a2);
typedef INT  (WINAPI *o_WSAEnumNameSpaceProvidersA)(LPDWORD a0, LPWSANAMESPACE_INFOA a1);
typedef INT  (WINAPI *o_WSAEnumNameSpaceProvidersW)(LPDWORD a0, LPWSANAMESPACE_INFOW a1);
typedef int  (WINAPI *o_WSAEnumNetworkEvents)(SOCKET a0, WSAEVENT a1, LPWSANETWORKEVENTS a2);
typedef int  (WINAPI *o_WSAEnumProtocolsA)(LPINT a0, LPWSAPROTOCOL_INFOA a1, LPDWORD a2);
typedef int  (WINAPI *o_WSAEnumProtocolsW)(LPINT a0, LPWSAPROTOCOL_INFOW a1, LPDWORD a2);
typedef int  (WINAPI *o_WSAEventSelect)(SOCKET a0, WSAEVENT a1, long a2);
typedef BOOL  (WINAPI *o_WSAGetOverlappedResult)(SOCKET a0, LPWSAOVERLAPPED a1, LPDWORD a2, BOOL a3, LPDWORD a4);
typedef BOOL  (WINAPI *o_WSAGetQOSByName)(SOCKET a0, LPWSABUF a1, LPQOS a2);
typedef INT  (WINAPI *o_WSAGetServiceClassInfoA)(LPGUID a0, LPGUID a1, LPDWORD a2, LPWSASERVICECLASSINFOA a3);
typedef INT  (WINAPI *o_WSAGetServiceClassInfoW)(LPGUID a0, LPGUID a1, LPDWORD a2, LPWSASERVICECLASSINFOW a3);
typedef INT  (WINAPI *o_WSAGetServiceClassNameByClassIdA)(LPGUID a0, LPSTR a1, LPDWORD a2);
typedef INT  (WINAPI *o_WSAGetServiceClassNameByClassIdW)(LPGUID a0, LPWSTR a1, LPDWORD a2);
typedef int  (WINAPI *o_WSAHtonl)(SOCKET a0, unsigned long a1, unsigned long * a2);
typedef int  (WINAPI *o_WSAHtons)(SOCKET a0, unsigned short a1, unsigned short * a2);
typedef INT  (WINAPI *o_WSAInstallServiceClassA)(LPWSASERVICECLASSINFOA a0);
typedef INT  (WINAPI *o_WSAInstallServiceClassW)(LPWSASERVICECLASSINFOW a0);
typedef int  (WINAPI *o_WSAIoctl)(SOCKET a0, DWORD a1, LPVOID a2, DWORD a3, LPVOID a4, DWORD a5, LPDWORD a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8);
typedef struct hostent * (PASCAL *o_gethostbyaddr)(const char* a0, int a1, int a2);
typedef struct protoent* (PASCAL *o_getprotobyname)(const char* a0);
typedef struct protoent* (PASCAL *o_getprotobynumber)(int a0);
typedef struct servent* (PASCAL *o_getservbyname)(const char* a0, const char* a1);
typedef struct hostent* (PASCAL *o_gethostbyname)(const char* a0);
typedef struct servent* (PASCAL *o_getservbyport)(int a0, const char* a1);
typedef int (PASCAL *o_gethostname)(char* a0, int a1);
typedef SOCKET  (WINAPI *o_WSAJoinLeaf)(SOCKET a0, const struct sockaddr * a1, int a2, LPWSABUF a3, LPWSABUF a4, LPQOS a5, LPQOS a6, DWORD a7);
typedef int  (WINAPI *o_WSANtohl)(SOCKET a0, unsigned long a1, unsigned long * a2);
typedef int  (WINAPI *o_WSANtohs)(SOCKET a0, unsigned short a1, unsigned short * a2);
typedef int (*o_WSAProviderConfigChange)(LPHANDLE a0, LPWSAOVERLAPPED a1, LPWSAOVERLAPPED_COMPLETION_ROUTINE a2);
typedef int  (WINAPI *o_WSARecv)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, LPDWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6);
typedef int  (WINAPI *o_WSARecvDisconnect)(SOCKET a0, LPWSABUF a1);
typedef int  (WINAPI *o_WSARecvFrom)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, LPDWORD a4, struct sockaddr * a5, LPINT a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8);
typedef INT  (WINAPI *o_WSARemoveServiceClass)(LPGUID a0);
typedef BOOL  (WINAPI *o_WSAResetEvent)(WSAEVENT a0);
typedef int  (WINAPI *o_WSASend)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, DWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6);
typedef int  (WINAPI *o_WSASendDisconnect)(SOCKET a0, LPWSABUF a1);
typedef int  (WINAPI *o_WSASendTo)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, DWORD a4, const struct sockaddr * a5, int a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8);
typedef BOOL  (WINAPI *o_WSASetEvent)(WSAEVENT a0);
typedef INT (WSAAPI *o_WSASetServiceA)(LPWSAQUERYSETA a0, WSAESETSERVICEOP a1, DWORD a2);
typedef INT  (WINAPI *o_WSASetServiceW)(LPWSAQUERYSETW a0, WSAESETSERVICEOP a1, DWORD a2);
typedef SOCKET  (WINAPI *o_WSASocketA)(int a0, int a1, int a2, LPWSAPROTOCOL_INFOA a3, GROUP a4, DWORD a5);
typedef SOCKET  (WINAPI *o_WSASocketW)(int a0, int a1, int a2, LPWSAPROTOCOL_INFOW a3, GROUP a4, DWORD a5);
typedef INT  (WINAPI *o_WSAStringToAddressA)(LPSTR a0, INT a1, LPWSAPROTOCOL_INFOA a2, LPSOCKADDR a3, LPINT a4);
typedef INT  (WINAPI *o_WSAStringToAddressW)(LPWSTR a0, INT a1, LPWSAPROTOCOL_INFOW a2, LPSOCKADDR a3, LPINT a4);
typedef DWORD  (WINAPI *o_WSAWaitForMultipleEvents)(DWORD a0, const WSAEVENT * a1, BOOL a2, DWORD a3, BOOL a4);
typedef int  (WINAPI *o_WSCDeinstallProvider)(LPGUID a0, LPINT a1);
typedef int  (WINAPI *o_WSCEnableNSProvider)(LPGUID a0, BOOL a1);
typedef int  (WINAPI *o_WSCEnumProtocols)(LPINT a0, LPWSAPROTOCOL_INFOW a1, LPDWORD a2, LPINT a3);
typedef int  (WINAPI *o_WSCGetProviderPath)(LPGUID a0, LPWSTR a1, LPINT a2, LPINT a3);
typedef int  (WINAPI *o_WSCInstallNameSpace)(LPWSTR a0, LPWSTR a1, DWORD a2, DWORD a3, LPGUID a4);
typedef int  (WINAPI *o_WSCInstallProvider)(const LPGUID a0, const LPWSTR a1, const LPWSAPROTOCOL_INFOW a2, DWORD a3, LPINT a4);
typedef int  (WINAPI *o_WSCUnInstallNameSpace)(LPGUID a0);
typedef int (PASCAL *o_WSAAsyncSelect)(SOCKET a0, HWND a1, u_int a2, long a3);
typedef HANDLE (PASCAL *o_WSAAsyncGetProtoByNumber)(HWND a0, u_int a1, int a2, char* a3, int a4);
typedef HANDLE (PASCAL *o_WSAAsyncGetProtoByName)(HWND a0, u_int a1, const char* a2, char* a3, int a4);
typedef HANDLE (PASCAL *o_WSAAsyncGetServByPort)(HWND a0, u_int a1, int a2, const char* a3, char* a4, int a5);
typedef HANDLE (PASCAL *o_WSAAsyncGetServByName)(HWND a0, u_int a1, const char* a2, const char* a3, char* a4, int a5);
typedef int (PASCAL *o_WSACancelAsyncRequest)(HANDLE a0);
typedef FARPROC (PASCAL *o_WSASetBlockingHook)(FARPROC a0);
typedef int (PASCAL *o_WSAUnhookBlockingHook)(void);
typedef int (PASCAL *o_WSAGetLastError)(void);
typedef void (PASCAL *o_WSASetLastError)(int a0);
typedef int (PASCAL *o_WSACancelBlockingCall)(void);
typedef BOOL (PASCAL *o_WSAIsBlocking)(void);
typedef int  (PASCAL *o_WSAStartup)(WORD a0, LPWSADATA a1);
typedef int (PASCAL *o_WSACleanup)(void);
typedef int (PASCAL *o___WSAFDIsSet)(SOCKET a0, fd_set* a1);
typedef int (PASCAL *o_WEP)(void);
typedef int (PASCAL *o_WSCWriteNameSpaceOrder)(int a0, int a1);
typedef int (PASCAL *o_WSCWriteProviderOrder)(LPDWORD a0, DWORD a1);
typedef int (PASCAL *o_WSANSPIoctl)(HANDLE a0, DWORD a1, LPVOID a2, DWORD a3, LPVOID a4, DWORD a5, LPDWORD a6, LPVOID a7);
typedef int (PASCAL *o_WSCUpdateProvider)(LPGUID a0, const WCHAR FAR* a1, const LPVOID a2, DWORD a3, LPINT a4);
typedef int (PASCAL *o_getaddrinfo)(const char* a0, const char* a1, LPVOID a2, LPVOID a3);
typedef void (PASCAL *o_freeaddrinfo)(LPVOID a0);
typedef int (PASCAL *o_getnameinfo)(LPVOID a0, DWORD a1, char* a2, DWORD a3, char* a4, DWORD a5, int a6);
typedef HANDLE  (WINAPI *o_WSAAsyncGetHostByAddr)(HWND a0, u_int a1, const char* a2, int a3, int a4, char* a5, int a6);
typedef HANDLE (WINAPI *o_WSAAsyncGetHostByName)(HWND a0, u_int a1, const char* a2, char* a3, int a4);
typedef INT (WINAPI *o_WSALookupServiceBeginA)(LPWSAQUERYSETA a0, DWORD a1, LPHANDLE a2);
typedef INT (WINAPI *o_WSALookupServiceBeginW)(LPWSAQUERYSETW a0, DWORD a1, LPHANDLE a2);
typedef INT (WINAPI *o_WSALookupServiceEnd)(HANDLE a0);
typedef INT (WINAPI *o_WSALookupServiceNextA)(HANDLE a0, DWORD a1, LPDWORD a2, LPWSAQUERYSETA a3);
typedef INT (WINAPI *o_WSALookupServiceNextW)(HANDLE a0, DWORD a1, LPDWORD a2, LPWSAQUERYSETW a3);

// actual funtions
// 
// NOTE
// Some of the exports are in BendMX.cpp
// Those are named _orig_xxxx()
// 

SOCKET PASCAL _orig_accept(SOCKET a0, struct sockaddr* a1, int* a2)
{

	o_accept p;
	if ((p = (o_accept)GetProcAddress(o_hDLL, "accept")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int PASCAL bind(SOCKET a0, const struct sockaddr* a1, int a2)
{

	o_bind p;
	if ((p = (o_bind)GetProcAddress(o_hDLL, "bind")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int PASCAL _orig_closesocket(SOCKET a0)
{

	o_closesocket p;
	if ((p = (o_closesocket)GetProcAddress(o_hDLL, "closesocket")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL _orig_connect(SOCKET a0, const struct sockaddr* a1, int a2)
{

	o_connect p;
	if ((p = (o_connect)GetProcAddress(o_hDLL, "connect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int PASCAL getpeername(SOCKET a0, struct sockaddr* a1, int* a2)
{

	o_getpeername p;
	if ((p = (o_getpeername)GetProcAddress(o_hDLL, "getpeername")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int PASCAL getsockname(SOCKET a0, struct sockaddr* a1, int* a2)
{
	o_getsockname p;
	if ((p = (o_getsockname)GetProcAddress(o_hDLL, "getsockname")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int PASCAL getsockopt(SOCKET a0, int a1, int a2, char* a3, int* a4)
{

	o_getsockopt p;
	if ((p = (o_getsockopt)GetProcAddress(o_hDLL, "getsockopt")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2, a3, a4);
}

u_long   PASCAL htonl(u_long a0)
{
	o_htonl p;
	if ((p = (o_htonl)GetProcAddress(o_hDLL, "htonl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

u_short   PASCAL htons(u_short a0)
{
	o_htons p;
	if ((p = (o_htons)GetProcAddress(o_hDLL, "htons")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL ioctlsocket(SOCKET a0, long a1, u_long * a2)
{
	o_ioctlsocket p;
	if ((p = (o_ioctlsocket)GetProcAddress(o_hDLL, "ioctlsocket")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

unsigned long   PASCAL inet_addr(const char* a0)
{
	o_inet_addr p;
	if ((p = (o_inet_addr)GetProcAddress(o_hDLL, "inet_addr")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

 char * PASCAL inet_ntoa(struct in_addr a0)
{
	o_inet_ntoa p;
	if ((p = (o_inet_ntoa)GetProcAddress(o_hDLL, "inet_ntoa")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL listen(SOCKET a0, int a1)
{
	o_listen p;
	if ((p = (o_listen)GetProcAddress(o_hDLL, "listen")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

u_long   PASCAL ntohl(u_long a0)
{
	o_ntohl p;
	if ((p = (o_ntohl)GetProcAddress(o_hDLL, "ntohl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

u_short   PASCAL ntohs(u_short a0)
{
	o_ntohs p;
	if ((p = (o_ntohs)GetProcAddress(o_hDLL, "ntohs")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL _orig_recv(SOCKET a0, char* a1, int a2, int a3)
{
	o_recv p;
	if ((p = (o_recv)GetProcAddress(o_hDLL, "recv")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int PASCAL _orig_recvfrom(SOCKET a0, char* a1, int a2, int a3, struct sockaddr* a4, int* a5)
{
	o_recvfrom p;
	if ((p = (o_recvfrom)GetProcAddress(o_hDLL, "recvfrom")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

int PASCAL select(int a0, fd_set* a1, fd_set* a2, fd_set* a3, const struct timeval* a4)
{
	o_select p;
	if ((p = (o_select)GetProcAddress(o_hDLL, "select")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int PASCAL _orig_send(SOCKET a0, const char* a1, int a2, int a3)
{
	o_send p;
	if ((p = (o_send)GetProcAddress(o_hDLL, "send")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int PASCAL _orig_sendto(SOCKET a0, const char* a1, int a2, int a3, const struct sockaddr* a4, int a5)
{
	o_sendto p;
	if ((p = (o_sendto)GetProcAddress(o_hDLL, "sendto")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

int PASCAL setsockopt(SOCKET a0, int a1, int a2, const char* a3, int a4)
{
	o_setsockopt p;
	if ((p = (o_setsockopt)GetProcAddress(o_hDLL, "setsockopt")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int PASCAL shutdown(SOCKET a0, int a1)
{
	o_shutdown p;
	if ((p = (o_shutdown)GetProcAddress(o_hDLL, "shutdown")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

SOCKET   PASCAL socket(int a0, int a1, int a2)
{
	o_socket p;
	if ((p = (o_socket)GetProcAddress(o_hDLL, "socket")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int  PASCAL WSApSetPostRoutine(int a0)
{
	o_WSApSetPostRoutine p;
	if ((p = (o_WSApSetPostRoutine)GetProcAddress(o_hDLL, "WSApSetPostRoutine")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

WSAEVENT   WSAAPI WPUCompleteOverlappedRequest(SOCKET a0, LPWSAOVERLAPPED a1, DWORD a2, DWORD a3, LPINT a4)
{
	o_WPUCompleteOverlappedRequest p;
	if ((p = (o_WPUCompleteOverlappedRequest)GetProcAddress(o_hDLL, "WPUCompleteOverlappedRequest")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

SOCKET   WINAPI WSAAccept(SOCKET a0, struct sockaddr * a1, LPINT a2, LPCONDITIONPROC a3, DWORD a4)
{
	o_WSAAccept p;
	if ((p = (o_WSAAccept)GetProcAddress(o_hDLL, "WSAAccept")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSAAddressToStringA(LPSOCKADDR a0, DWORD a1, LPWSAPROTOCOL_INFOA a2, LPSTR a3, LPDWORD a4)
{
	o_WSAAddressToStringA p;
	if ((p = (o_WSAAddressToStringA)GetProcAddress(o_hDLL, "WSAAddressToStringA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSAAddressToStringW(LPSOCKADDR a0, DWORD a1, LPWSAPROTOCOL_INFOW a2, LPWSTR a3, LPDWORD a4)
{
	o_WSAAddressToStringW p;
	if ((p = (o_WSAAddressToStringW)GetProcAddress(o_hDLL, "WSAAddressToStringW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

BOOL WINAPI WSACloseEvent(WSAEVENT a0)
{
	o_WSACloseEvent p;
	if ((p = (o_WSACloseEvent)GetProcAddress(o_hDLL, "WSACloseEvent")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int WINAPI WSAConnect(SOCKET a0, const struct sockaddr * a1, int a2, LPWSABUF a3, LPWSABUF a4, LPQOS a5, LPQOS a6)
{
	o_WSAConnect p;
	if ((p = (o_WSAConnect)GetProcAddress(o_hDLL, "WSAConnect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6);
}

WSAEVENT   WINAPI WSACreateEvent(void)
{
	o_WSACreateEvent p;
	if ((p = (o_WSACreateEvent)GetProcAddress(o_hDLL, "WSACreateEvent")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

int WINAPI WSADuplicateSocketA(SOCKET a0, DWORD a1, LPWSAPROTOCOL_INFOA a2)
{
	o_WSADuplicateSocketA p;
	if ((p = (o_WSADuplicateSocketA)GetProcAddress(o_hDLL, "WSADuplicateSocketA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSADuplicateSocketW(SOCKET a0, DWORD a1, LPWSAPROTOCOL_INFOW a2)
{
	o_WSADuplicateSocketW p;
	if ((p = (o_WSADuplicateSocketW)GetProcAddress(o_hDLL, "WSADuplicateSocketW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAEnumNameSpaceProvidersA(LPDWORD a0, LPWSANAMESPACE_INFOA a1)
{
	o_WSAEnumNameSpaceProvidersA p;
	if ((p = (o_WSAEnumNameSpaceProvidersA)GetProcAddress(o_hDLL, "WSAEnumNameSpaceProvidersA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSAEnumNameSpaceProvidersW(LPDWORD a0, LPWSANAMESPACE_INFOW a1)
{
	o_WSAEnumNameSpaceProvidersW p;
	if ((p = (o_WSAEnumNameSpaceProvidersW)GetProcAddress(o_hDLL, "WSAEnumNameSpaceProvidersW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSAEnumNetworkEvents(SOCKET a0, WSAEVENT a1, LPWSANETWORKEVENTS a2)
{
	o_WSAEnumNetworkEvents p;
	if ((p = (o_WSAEnumNetworkEvents)GetProcAddress(o_hDLL, "WSAEnumNetworkEvents")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAEnumProtocolsA(LPINT a0, LPWSAPROTOCOL_INFOA a1, LPDWORD a2)
{

	o_WSAEnumProtocolsA p;
	if ((p = (o_WSAEnumProtocolsA)GetProcAddress(o_hDLL, "WSAEnumProtocolsA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAEnumProtocolsW(LPINT a0, LPWSAPROTOCOL_INFOW a1, LPDWORD a2)
{
	o_WSAEnumProtocolsW p;
	if ((p = (o_WSAEnumProtocolsW)GetProcAddress(o_hDLL, "WSAEnumProtocolsW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAEventSelect(SOCKET a0, WSAEVENT a1, long a2)
{
	o_WSAEventSelect p;
	if ((p = (o_WSAEventSelect)GetProcAddress(o_hDLL, "WSAEventSelect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

BOOL WINAPI WSAGetOverlappedResult(SOCKET a0, LPWSAOVERLAPPED a1, LPDWORD a2, BOOL a3, LPDWORD a4)
{
	o_WSAGetOverlappedResult p;
	if ((p = (o_WSAGetOverlappedResult)GetProcAddress(o_hDLL, "WSAGetOverlappedResult")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}
 
BOOL WINAPI WSAGetQOSByName(SOCKET a0, LPWSABUF a1, LPQOS a2)
{
	o_WSAGetQOSByName p;
	if ((p = (o_WSAGetQOSByName)GetProcAddress(o_hDLL, "WSAGetQOSByName")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAGetServiceClassInfoA(LPGUID a0, LPGUID a1, LPDWORD a2, LPWSASERVICECLASSINFOA a3)
{
	o_WSAGetServiceClassInfoA p;
	if ((p = (o_WSAGetServiceClassInfoA)GetProcAddress(o_hDLL, "WSAGetServiceClassInfoA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int WINAPI WSAGetServiceClassInfoW(LPGUID a0, LPGUID a1, LPDWORD a2, LPWSASERVICECLASSINFOW a3)
{
	o_WSAGetServiceClassInfoW p;
	if ((p = (o_WSAGetServiceClassInfoW)GetProcAddress(o_hDLL, "WSAGetServiceClassInfoW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int WINAPI WSAGetServiceClassNameByClassIdA(LPGUID a0, LPSTR a1, LPDWORD a2)
{
	o_WSAGetServiceClassNameByClassIdA p;
	if ((p = (o_WSAGetServiceClassNameByClassIdA)GetProcAddress(o_hDLL, "WSAGetServiceClassNameByClassIdA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAGetServiceClassNameByClassIdW(LPGUID a0, LPWSTR a1, LPDWORD a2)
{
	o_WSAGetServiceClassNameByClassIdW p;
	if ((p = (o_WSAGetServiceClassNameByClassIdW)GetProcAddress(o_hDLL, "WSAGetServiceClassNameByClassIdW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAHtonl(SOCKET a0, unsigned long a1, unsigned long * a2)
{
	o_WSAHtonl p;
	if ((p = (o_WSAHtonl)GetProcAddress(o_hDLL, "WSAHtonl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAHtons(SOCKET a0, unsigned short a1, unsigned short * a2)
{
	o_WSAHtons p;
	if ((p = (o_WSAHtons)GetProcAddress(o_hDLL, "WSAHtons")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSAInstallServiceClassA(LPWSASERVICECLASSINFOA a0)
{
	o_WSAInstallServiceClassA p;
	if ((p = (o_WSAInstallServiceClassA)GetProcAddress(o_hDLL, "WSAInstallServiceClassA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int WINAPI WSAInstallServiceClassW(LPWSASERVICECLASSINFOW a0)
{
	o_WSAInstallServiceClassW p;
	if ((p = (o_WSAInstallServiceClassW)GetProcAddress(o_hDLL, "WSAInstallServiceClassW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int WINAPI WSAIoctl(SOCKET a0, DWORD a1, LPVOID a2, DWORD a3, LPVOID a4, DWORD a5, LPDWORD a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
{
	o_WSAIoctl p;
	if ((p = (o_WSAIoctl)GetProcAddress(o_hDLL, "WSAIoctl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

struct hostent * PASCAL gethostbyaddr(const char* a0, int a1, int a2)
{
	o_gethostbyaddr p;
	if ((p = (o_gethostbyaddr)GetProcAddress(o_hDLL, "gethostbyaddr")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

 struct hostent * PASCAL gethostbyname(const char* a0)
{
	o_gethostbyname p;
	if ((p = (o_gethostbyname)GetProcAddress(o_hDLL, "gethostbyname")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

 struct protoent * PASCAL getprotobyname(const char* a0)
{
	o_getprotobyname p;
	if ((p = (o_getprotobyname)GetProcAddress(o_hDLL, "getprotobyname")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

 struct protoent * PASCAL getprotobynumber(int a0)
{
	o_getprotobynumber p;
	if ((p = (o_getprotobynumber)GetProcAddress(o_hDLL, "getprotobynumber")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

 struct servent * PASCAL getservbyname(const char* a0, const char* a1)
{
	o_getservbyname p;
	if ((p = (o_getservbyname)GetProcAddress(o_hDLL, "getservbyname")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

 struct servent * PASCAL getservbyport(int a0, const char* a1)
{
	o_getservbyport p;
	if ((p = (o_getservbyport)GetProcAddress(o_hDLL, "getservbyport")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int PASCAL gethostname(char* a0, int a1)
{
	o_gethostname p;
	if ((p = (o_gethostname)GetProcAddress(o_hDLL, "gethostname")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

SOCKET   WINAPI WSAJoinLeaf(SOCKET a0, const struct sockaddr * a1, int a2, LPWSABUF a3, LPWSABUF a4, LPQOS a5, LPQOS a6, DWORD a7)
{
	o_WSAJoinLeaf p;
	if ((p = (o_WSAJoinLeaf)GetProcAddress(o_hDLL, "WSAJoinLeaf")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6, a7);
}

int WINAPI WSANtohl(SOCKET a0, unsigned long a1, unsigned long * a2)
{
	o_WSANtohl p;
	if ((p = (o_WSANtohl)GetProcAddress(o_hDLL, "WSANtohl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSANtohs(SOCKET a0, unsigned short a1, unsigned short * a2)
{
	o_WSANtohs p;
	if ((p = (o_WSANtohs)GetProcAddress(o_hDLL, "WSANtohs")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WSAAPI WSAProviderConfigChange(LPHANDLE a0, LPWSAOVERLAPPED a1, LPWSAOVERLAPPED_COMPLETION_ROUTINE a2)
{
	o_WSAProviderConfigChange p;
	if ((p = (o_WSAProviderConfigChange)GetProcAddress(o_hDLL, "WSAProviderConfigChange")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSARecv(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, LPDWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6)
{

	o_WSARecv p;
	if ((p = (o_WSARecv)GetProcAddress(o_hDLL, "WSARecv")) == NULL) {

	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6);
}

int WINAPI WSARecvDisconnect(SOCKET a0, LPWSABUF a1)
{
	o_WSARecvDisconnect p;
	if ((p = (o_WSARecvDisconnect)GetProcAddress(o_hDLL, "WSARecvDisconnect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSARecvFrom(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, LPDWORD a4, struct sockaddr * a5, LPINT a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
{
	o_WSARecvFrom p;
	if ((p = (o_WSARecvFrom)GetProcAddress(o_hDLL, "WSARecvFrom")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

int WINAPI WSARemoveServiceClass(LPGUID a0)
{
	o_WSARemoveServiceClass p;
	if ((p = (o_WSARemoveServiceClass)GetProcAddress(o_hDLL, "WSARemoveServiceClass")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

BOOL WINAPI WSAResetEvent(WSAEVENT a0)
{
	o_WSAResetEvent p;
	if ((p = (o_WSAResetEvent)GetProcAddress(o_hDLL, "WSAResetEvent")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}
 
int WINAPI WSASend(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, DWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6)
{
	o_WSASend p;
	if ((p = (o_WSASend)GetProcAddress(o_hDLL, "WSASend")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6);
}

int WINAPI WSASendDisconnect(SOCKET a0, LPWSABUF a1)
{
	o_WSASendDisconnect p;
	if ((p = (o_WSASendDisconnect)GetProcAddress(o_hDLL, "WSASendDisconnect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSASendTo(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, DWORD a4, const struct sockaddr * a5, int a6, LPWSAOVERLAPPED a7, LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
{
	o_WSASendTo p;
	if ((p = (o_WSASendTo)GetProcAddress(o_hDLL, "WSASendTo")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

BOOL WINAPI WSASetEvent(WSAEVENT a0)
{
	o_WSASetEvent p;
	if ((p = (o_WSASetEvent)GetProcAddress(o_hDLL, "WSASetEvent")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int WSAAPI WSASetServiceA(LPWSAQUERYSETA a0, WSAESETSERVICEOP a1, DWORD a2)
{
	o_WSASetServiceA p;
	if ((p = (o_WSASetServiceA)GetProcAddress(o_hDLL, "WSASetServiceA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

int WINAPI WSASetServiceW(LPWSAQUERYSETW a0, WSAESETSERVICEOP a1, DWORD a2)
{
	o_WSASetServiceW p;
	if ((p = (o_WSASetServiceW)GetProcAddress(o_hDLL, "WSASetServiceW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2);
}

SOCKET   WINAPI WSASocketA(int a0, int a1, int a2, LPWSAPROTOCOL_INFOA a3, GROUP a4, DWORD a5)
{
	o_WSASocketA p;
	if ((p = (o_WSASocketA)GetProcAddress(o_hDLL, "WSASocketA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

SOCKET   WINAPI WSASocketW(int a0, int a1, int a2, LPWSAPROTOCOL_INFOW a3, GROUP a4, DWORD a5)
{
	o_WSASocketW p;
	if ((p = (o_WSASocketW)GetProcAddress(o_hDLL, "WSASocketW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

int WINAPI WSAStringToAddressA(LPSTR a0, INT a1, LPWSAPROTOCOL_INFOA a2, LPSOCKADDR a3, LPINT a4)
{
	o_WSAStringToAddressA p;
	if ((p = (o_WSAStringToAddressA)GetProcAddress(o_hDLL, "WSAStringToAddressA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSAStringToAddressW(LPWSTR a0, INT a1, LPWSAPROTOCOL_INFOW a2, LPSOCKADDR a3, LPINT a4)
{
	o_WSAStringToAddressW p;
	if ((p = (o_WSAStringToAddressW)GetProcAddress(o_hDLL, "WSAStringToAddressW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

DWORD   WINAPI WSAWaitForMultipleEvents(DWORD a0, const WSAEVENT * a1, BOOL a2, DWORD a3, BOOL a4)
{
	o_WSAWaitForMultipleEvents p;
	if ((p = (o_WSAWaitForMultipleEvents)GetProcAddress(o_hDLL, "WSAWaitForMultipleEvents")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSCDeinstallProvider(LPGUID a0, LPINT a1)
{
	o_WSCDeinstallProvider p;
	if ((p = (o_WSCDeinstallProvider)GetProcAddress(o_hDLL, "WSCDeinstallProvider")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSCEnableNSProvider(LPGUID a0, BOOL a1)
{
	o_WSCEnableNSProvider p;
	if ((p = (o_WSCEnableNSProvider)GetProcAddress(o_hDLL, "WSCEnableNSProvider")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int WINAPI WSCEnumProtocols(LPINT a0, LPWSAPROTOCOL_INFOW a1, LPDWORD a2, LPINT a3)
{
	o_WSCEnumProtocols p;
	if ((p = (o_WSCEnumProtocols)GetProcAddress(o_hDLL, "WSCEnumProtocols")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int WINAPI WSCGetProviderPath(LPGUID a0, LPWSTR a1, LPINT a2, LPINT a3)
{
	o_WSCGetProviderPath p;
	if ((p = (o_WSCGetProviderPath)GetProcAddress(o_hDLL, "WSCGetProviderPath")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

int WINAPI WSCInstallNameSpace(LPWSTR a0, LPWSTR a1, DWORD a2, DWORD a3, LPGUID a4)
{
	o_WSCInstallNameSpace p;
	if ((p = (o_WSCInstallNameSpace)GetProcAddress(o_hDLL, "WSCInstallNameSpace")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSCInstallProvider(const LPGUID a0, const LPWSTR a1, const LPWSAPROTOCOL_INFOW a2, DWORD a3, LPINT a4)
{
	o_WSCInstallProvider p;
	if ((p = (o_WSCInstallProvider)GetProcAddress(o_hDLL, "WSCInstallProvider")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int WINAPI WSCUnInstallNameSpace(LPGUID a0)
{
	o_WSCUnInstallNameSpace p;
	if ((p = (o_WSCUnInstallNameSpace)GetProcAddress(o_hDLL, "WSCUnInstallNameSpace")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL  WSAAsyncSelect(SOCKET a0, HWND a1, u_int a2, long a3)
{
	o_WSAAsyncSelect p;
	if ((p = (o_WSAAsyncSelect)GetProcAddress(o_hDLL, "WSAAsyncSelect")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

HANDLE   PASCAL WSAAsyncGetProtoByNumber(HWND a0, u_int a1, int a2, char* a3, int a4)
{
	o_WSAAsyncGetProtoByNumber p;
	if ((p = (o_WSAAsyncGetProtoByNumber)GetProcAddress(o_hDLL, "WSAAsyncGetProtoByNumber")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

HANDLE   PASCAL WSAAsyncGetProtoByName(HWND a0, u_int a1, const char* a2, char* a3, int a4)
{
	o_WSAAsyncGetProtoByName p;
	if ((p = (o_WSAAsyncGetProtoByName)GetProcAddress(o_hDLL, "WSAAsyncGetProtoByName")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

HANDLE   PASCAL WSAAsyncGetServByPort(HWND a0, u_int a1, int a2, const char* a3, char* a4, int a5)
{
	o_WSAAsyncGetServByPort p;
	if ((p = (o_WSAAsyncGetServByPort)GetProcAddress(o_hDLL, "WSAAsyncGetServByPort")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

HANDLE   PASCAL WSAAsyncGetServByName(HWND a0, u_int a1, const char* a2, const char* a3, char* a4, int a5)
{
	o_WSAAsyncGetServByName p;
	if ((p = (o_WSAAsyncGetServByName)GetProcAddress(o_hDLL, "WSAAsyncGetServByName")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5);
}

int PASCAL WSACancelAsyncRequest(HANDLE a0)
{
	o_WSACancelAsyncRequest p;
	if ((p = (o_WSACancelAsyncRequest)GetProcAddress(o_hDLL, "WSACancelAsyncRequest")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

FARPROC   PASCAL WSASetBlockingHook(FARPROC a0)
{
	o_WSASetBlockingHook p;
	if ((p = (o_WSASetBlockingHook)GetProcAddress(o_hDLL, "WSASetBlockingHook")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0);
}

int PASCAL WSAUnhookBlockingHook(void)
{
	o_WSAUnhookBlockingHook p;
	if ((p = (o_WSAUnhookBlockingHook)GetProcAddress(o_hDLL, "WSAUnhookBlockingHook")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

int PASCAL WSAGetLastError(void)
{
	o_WSAGetLastError p;
	if ((p = (o_WSAGetLastError)GetProcAddress(o_hDLL, "WSAGetLastError")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

void PASCAL WSASetLastError(int a0)
{
	o_WSASetLastError p;
	if ((p = (o_WSASetLastError)GetProcAddress(o_hDLL, "WSASetLastError")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

     (p)(a0);
}

int PASCAL WSACancelBlockingCall(void)
{
	o_WSACancelBlockingCall p;
	if ((p = (o_WSACancelBlockingCall)GetProcAddress(o_hDLL, "WSACancelBlockingCall")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

   return (p)();
}

BOOL PASCAL WSAIsBlocking(void)
{
	o_WSAIsBlocking p;
	if ((p = (o_WSAIsBlocking)GetProcAddress(o_hDLL, "WSAIsBlocking")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

int PASCAL WSAStartup(WORD a0, LPWSADATA a1)
{
	o_WSAStartup p;
	if ((p = (o_WSAStartup)GetProcAddress(o_hDLL, "WSAStartup")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int PASCAL WSACleanup(void)
{
	o_WSACleanup p;
	if ((p = (o_WSACleanup)GetProcAddress(o_hDLL, "WSACleanup")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

int PASCAL __WSAFDIsSet(SOCKET a0, fd_set* a1)
{
	o___WSAFDIsSet p;
	if ((p = (o___WSAFDIsSet)GetProcAddress(o_hDLL, "__WSAFDIsSet")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int  PASCAL WEP(void)
{
	o_WEP p;
	if ((p = (o_WEP)GetProcAddress(o_hDLL, "WEP")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)();
}

int  PASCAL WSCWriteNameSpaceOrder(int a0, int a1)
{
	o_WSCWriteNameSpaceOrder p;
	if ((p = (o_WSCWriteNameSpaceOrder)GetProcAddress(o_hDLL, "WSCWriteNameSpaceOrder")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int PASCAL WSCWriteProviderOrder(LPDWORD a0, DWORD a1)
{
	o_WSCWriteProviderOrder p;
	if ((p = (o_WSCWriteProviderOrder)GetProcAddress(o_hDLL, "WSCWriteProviderOrder")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1);
}

int PASCAL WSANSPIoctl(HANDLE a0, DWORD a1, LPVOID a2, DWORD a3, LPVOID a4, DWORD a5, LPDWORD a6, LPVOID a7)
{
	o_WSANSPIoctl p;
	if ((p = (o_WSANSPIoctl)GetProcAddress(o_hDLL, "WSANSPIoctl")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6, a7);
}

int PASCAL WSCUpdateProvider(LPGUID a0, const WCHAR FAR* a1, const LPVOID a2, DWORD a3, LPINT a4)
{
	o_WSCUpdateProvider p;
	if ((p = (o_WSCUpdateProvider)GetProcAddress(o_hDLL, "WSCUpdateProvider")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4);
}

int PASCAL getaddrinfo(const char* a0, const char* a1, LPVOID a2, LPVOID a3)
{
	o_getaddrinfo p;
	if ((p = (o_getaddrinfo)GetProcAddress(o_hDLL, "getaddrinfo")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3);
}

void PASCAL freeaddrinfo(LPVOID a0)
{
	o_freeaddrinfo p;
	if ((p = (o_freeaddrinfo)GetProcAddress(o_hDLL, "freeaddrinfo")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

     (p)(a0);
}

int PASCAL getnameinfo(LPVOID a0, DWORD a1, char* a2, DWORD a3, char* a4, DWORD a5, int a6)
{
	o_getnameinfo p;
	if ((p = (o_getnameinfo)GetProcAddress(o_hDLL, "getnameinfo")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}

    return (p)(a0, a1, a2, a3, a4, a5, a6);
}

HANDLE   PASCAL WSAAsyncGetHostByAddr(HWND a0, u_int a1, const char* a2, int a3, int a4, char* a5, int a6)
{
	o_WSAAsyncGetHostByAddr p;
	if ((p = (o_WSAAsyncGetHostByAddr)GetProcAddress(o_hDLL, "WSAAsyncGetHostByAddr")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2, a3, a4, a5, a6);
}

HANDLE   PASCAL WSAAsyncGetHostByName(HWND a0, u_int a1, const char* a2, char* a3, int a4)
{
	o_WSAAsyncGetHostByName p;
	if ((p = (o_WSAAsyncGetHostByName)GetProcAddress(o_hDLL, "WSAAsyncGetHostByName")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2, a3, a4);
}

INT   WINAPI WSALookupServiceBeginA(LPWSAQUERYSETA a0, DWORD a1, LPHANDLE a2)
{
	o_WSALookupServiceBeginA p;
	if ((p = (o_WSALookupServiceBeginA)GetProcAddress(o_hDLL, "WSALookupServiceBeginA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2);
}

INT   WINAPI WSALookupServiceBeginW(LPWSAQUERYSETW a0, DWORD a1, LPHANDLE a2)
{
	o_WSALookupServiceBeginW p;
	if ((p = (o_WSALookupServiceBeginW)GetProcAddress(o_hDLL, "WSALookupServiceBeginW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2);
}

INT WINAPI WSALookupServiceEnd(HANDLE a0)
{
	o_WSALookupServiceEnd p;
	if ((p = (o_WSALookupServiceEnd)GetProcAddress(o_hDLL, "WSALookupServiceEnd")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0);
}

INT   WINAPI WSALookupServiceNextA(HANDLE a0, DWORD a1, LPDWORD a2, LPWSAQUERYSETA a3)
{
	o_WSALookupServiceNextA p;
	if ((p = (o_WSALookupServiceNextA)GetProcAddress(o_hDLL, "WSALookupServiceNextA")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2, a3);
}

INT   WINAPI WSALookupServiceNextW(HANDLE a0, DWORD a1, LPDWORD a2, LPWSAQUERYSETW a3)
{
	o_WSALookupServiceNextW p;
	if ((p = (o_WSALookupServiceNextW)GetProcAddress(o_hDLL, "WSALookupServiceNextW")) == NULL) {
	    TRACE("cannot find entry point (%d)\n", GetLastError());
	    abort();
	}
    return (p)(a0, a1, a2, a3);
}
