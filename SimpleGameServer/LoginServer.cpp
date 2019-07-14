#include "stdafx.h"
#include "LoginServer.h"
#include <iostream>
#include <assert.h>

// globals.
HANDLE g_hIocp;

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{
}

unsigned long WINAPI ThreadProc(LPVOID parm)
{
	unsigned long readBytes, completionKey, flags;
	LoginSocketInfo* socketInfo;
	while (true)
	{
		int ret = GetQueuedCompletionStatus(g_hIocp, &readBytes, &completionKey, (LPOVERLAPPED*)&socketInfo, INFINITE);
		if (ret == FALSE)
		{
			std::cout << "GetQueuedCompletionStatus : FALSE" << std::endl;
			if (socketInfo != NULL)
			{
				closesocket(socketInfo->fd);
				free(socketInfo);
			}
			continue;
		}
		else
		{
			LoginPacket* receivedPacket = (LoginPacket*)(socketInfo->buffer);
			std::cout << "Login user ID : " << receivedPacket->ID << " PW : " << receivedPacket->PW << std::endl;
			//
			int ret = WSASend(socketInfo->fd, &(socketInfo->wsabuf), 1, &readBytes, 0, NULL, NULL);
			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					// error handling.
				}
			}
		}
		flags = 0;
		memset(socketInfo->buffer, 0x00, sizeof(LoginPacket));
		socketInfo->readN = 0;
		socketInfo->writeN = 0;
		WSARecv(socketInfo->fd, &(socketInfo->wsabuf), 1, &readBytes, &flags, &socketInfo->overlapped, NULL);
	}
}

void LoginServer::Init()
{
	std::cout << "LoginServer::Init" << std::endl;

	WSAStartup(MAKEWORD(2, 2), &WsaData);
	
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(SERVER_PORT);
	Addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(ListenSocket, (sockaddr*)&Addr, sizeof(Addr)) == SOCKET_ERROR)
	{
		// print error
		std::cout << "Bind SOCKET_ERROR" << std::endl;
	}
	if (listen(ListenSocket, 5) == SOCKET_ERROR)
	{
		// print error
		std::cout << "Listen SOCKET_ERROR" << std::endl;
	}
	// Init iocp.
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, MAXIMUM_THREAD_NUM);
	// create threads.
	DWORD threadId;
	for (int i = 0; i < 10; i++)
	{
		Handles[i] = CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
	}
}

void LoginServer::Start()
{
	std::cout << "LoginServer::Start" << std::endl;
	// main loop.
	unsigned long readN = 0, flags = 0;
	int addrlen = sizeof(Addr);
	int recvRet, lastError;
	while (true)
	{
		ClientSocket = accept(ListenSocket, (sockaddr*)&Addr, &addrlen);
		if (ClientSocket == INVALID_SOCKET)
		{
			// error
			std::cout << "ClientSocket INVALID_SOCKET" << std::endl;
		}
		SocketInfo = (LoginSocketInfo*)malloc(sizeof(LoginSocketInfo));
		SocketInfo->fd = ClientSocket;
		SocketInfo->readN = 0;
		SocketInfo->writeN = 0;
		SocketInfo->wsabuf.buf = SocketInfo->buffer;
		SocketInfo->wsabuf.len = sizeof(LoginPacket);
		memset(&(SocketInfo->overlapped), 0x00, sizeof(OVERLAPPED));

		CreateIoCompletionPort((HANDLE)ClientSocket, g_hIocp, (ULONG_PTR)SocketInfo, MAXIMUM_THREAD_NUM);
		recvRet = WSARecv(SocketInfo->fd, &(SocketInfo->wsabuf), 1, &readN, &flags, &(SocketInfo->overlapped), NULL);
		if (recvRet == SOCKET_ERROR)
		{
			lastError = WSAGetLastError();
			if (lastError != WSA_IO_PENDING)
			{
				//ERROR_IO_PENDING 의 경우, iocp에서 정상적으로 I/O 처리가 이루어졌음을 의미한다.
				// error.
				std::cout << "error WSARecv();" << std::endl;
			}
		}
	}
	// iocp close.
	CloseHandle(g_hIocp);
}
