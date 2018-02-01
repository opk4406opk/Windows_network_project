// example_iocp_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>

#define MAX_PACKET_LINE 1024
#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"
struct SocketInfo {
	OVERLAPPED overlapped;
	SOCKET fd;
	char buffer[MAX_PACKET_LINE];
	int readN;
	int writeN;
	WSABUF wsabuf;
};

HANDLE g_hlocp;
unsigned long WINAPI ThreadProc(LPVOID parm) {
	unsigned long readN, coKey, flags;
	SocketInfo* socketInfo;
	while (true) {
		GetQueuedCompletionStatus(g_hlocp, &readN, &coKey, (LPOVERLAPPED*)&socketInfo, INFINITE);
		if (readN == 0) {
			closesocket(socketInfo->fd);
			free(socketInfo);
			continue;
		}
		else {
			WSASend(socketInfo->fd, &(socketInfo->wsabuf), 1, &readN, 0, NULL, NULL);
		}
		flags = 0;
		memset(socketInfo->buffer, 0x00, MAX_PACKET_LINE);
		socketInfo->readN = 0;
		socketInfo->writeN = 0;
		WSARecv(socketInfo->fd, &(socketInfo->wsabuf), 1, &readN, &flags, &socketInfo->overlapped, NULL);
	}
}
// thread handles;
HANDLE g_Handle[10];
int main()
{
	WSADATA wsaData;
	sockaddr_in addr;
	SocketInfo* socketInfo;
	SOCKET listenSocket, clientSocket;
	unsigned long readN, flags;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) return 1;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		return 1;
	}
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		return 1;
	}
	// Init iocp.
	g_hlocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 4);
	// create threads.
	DWORD threadId;
	for (int i = 0; i < 10; i++) {
		g_Handle[i] = CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
	}
	// main loop.
	int addrlen = sizeof(addr);
	while (true) {
		clientSocket = accept(listenSocket, (sockaddr*)&addr, &addrlen);
		if (clientSocket == INVALID_SOCKET) return 1;
		socketInfo = (SocketInfo*)malloc(sizeof(SocketInfo));
		socketInfo->fd = clientSocket;
		socketInfo->readN = 0;
		socketInfo->writeN = 0;
		socketInfo->wsabuf.buf = socketInfo->buffer;
		socketInfo->wsabuf.len = MAX_PACKET_LINE;

		CreateIoCompletionPort((HANDLE)clientSocket, g_hlocp, (unsigned long)socketInfo, 4);
		//WSARecv(socketInfo->fd, &(socketInfo->wsabuf), 1, &readN, &flags, &(socketInfo->overlapped), NULL);
	}
    return 0;
}

