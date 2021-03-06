// example_iocp_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>

#define MAX_PACKET_LINE 1024
#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"
#define MAXIMUM_THREAD_NUM 6
struct SocketInfo {
	OVERLAPPED overlapped;
	SOCKET fd;
	char buffer[MAX_PACKET_LINE];
	int readN;
	int writeN;
	WSABUF wsabuf;
};

HANDLE g_hIocp;
unsigned long WINAPI ThreadProc(LPVOID parm) {
	unsigned long readBytes, completionKey, flags;
	SocketInfo* socketInfo;
	while (true) {
		int ret = GetQueuedCompletionStatus(g_hIocp, &readBytes, &completionKey, (LPOVERLAPPED*)&socketInfo, INFINITE);
		if (ret == FALSE) {
			if (socketInfo != NULL) {
				closesocket(socketInfo->fd);
				free(socketInfo);
			}
			continue;
		}
		else {
			int ret = WSASend(socketInfo->fd, &(socketInfo->wsabuf), 1, &readBytes, 0, NULL, NULL);
			if (ret == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					// error handling.
				}
			}
		}
		flags = 0;
		memset(socketInfo->buffer, 0x00, MAX_PACKET_LINE);
		socketInfo->readN = 0;
		socketInfo->writeN = 0;
		WSARecv(socketInfo->fd, &(socketInfo->wsabuf), 1, &readBytes, &flags, &socketInfo->overlapped, NULL);
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
	unsigned long readN = 0, flags = 0;

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
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, MAXIMUM_THREAD_NUM);
	// create threads.
	DWORD threadId;
	for (int i = 0; i < 10; i++) {
		g_Handle[i] = CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
	}
	// main loop.
	int addrlen = sizeof(addr);
	int recvRet, lastError;
	while (true) {
		
		clientSocket = accept(listenSocket, (sockaddr*)&addr, &addrlen);
		if (clientSocket == INVALID_SOCKET) return 1;
		socketInfo = (SocketInfo*)malloc(sizeof(SocketInfo));
		socketInfo->fd = clientSocket;
		socketInfo->readN = 0;
		socketInfo->writeN = 0;
		socketInfo->wsabuf.buf = socketInfo->buffer;
		socketInfo->wsabuf.len = MAX_PACKET_LINE;
		memset(&(socketInfo->overlapped), 0x00, sizeof(OVERLAPPED));

		CreateIoCompletionPort((HANDLE)clientSocket, g_hIocp, (ULONG_PTR)socketInfo, MAXIMUM_THREAD_NUM);
		recvRet = WSARecv(socketInfo->fd, &(socketInfo->wsabuf), 1, &readN, &flags, &(socketInfo->overlapped), NULL);
		if (recvRet == SOCKET_ERROR) {
			lastError = WSAGetLastError();
			if (lastError != WSA_IO_PENDING) {
				//ERROR_IO_PENDING 의 경우, iocp에서 정상적으로 I/O 처리가 이루어졌음을 의미한다.
				// error.
				std::cout << "error WSARecv();" << std::endl;
			}
		}
		
	}
	// iocp close.
	CloseHandle(g_hIocp);
    return 0;
}

