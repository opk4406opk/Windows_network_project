#pragma once
#include <WinSock2.h>
#include "resource.h"
//network
#define MAX_PACKET_LINE 1024
#define SERVER_PORT 8000
#define WM_SOCKET WM_USER + 1
//
SOCKET listenSocket;

void ErrMsg(LPCWSTR msg) {
	MessageBox(NULL, msg, L"Error", MB_ICONEXCLAMATION | MB_OK);
}

void MakeSocket(HWND hwnd) {
	WSADATA wsaData;
	sockaddr_in addr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//
	memset((void*)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(listenSocket, (sockaddr*)&addr, sizeof(addr));
	listen(listenSocket, 5);
	WSAAsyncSelect(listenSocket, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
}