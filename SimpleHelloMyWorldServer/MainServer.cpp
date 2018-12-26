#include "stdafx.h"
#include "MainServer.h"
#include <iostream>
#include <assert.h>

// globals.
HANDLE g_hIocp;

MainServer::MainServer()
{
}

MainServer::~MainServer()
{
}

unsigned long WINAPI ThreadProc(LPVOID parm) {
	unsigned long readBytes, completionKey, flags;
	GameSocketInfo* socketInfo;
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

void MainServer::Init()
{
	assert(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
	
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(listenSocket != INVALID_SOCKET);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	assert(bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR);
	assert(listen(listenSocket, 5) != SOCKET_ERROR);
	// Init iocp.
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, MAXIMUM_THREAD_NUM);
	// create threads.
	DWORD threadId;
	for (int i = 0; i < 10; i++) {
		handles[i] = CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
	}
}

void MainServer::Start()
{
	// main loop.
	unsigned long readN = 0, flags = 0;
	int addrlen = sizeof(addr);
	int recvRet, lastError;
	while (true) {
		clientSocket = accept(listenSocket, (sockaddr*)&addr, &addrlen);
		assert(clientSocket == INVALID_SOCKET);

		socketInfo = (GameSocketInfo*)malloc(sizeof(GameSocketInfo));
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
}
