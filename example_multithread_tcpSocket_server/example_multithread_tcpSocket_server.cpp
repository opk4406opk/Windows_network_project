// example_multithread_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <thread>
#include <vector>
#include <iostream>

#define MAX_PACKET_LEN 1024
#define SERVER_PORT 8000
CRITICAL_SECTION criticalSection;

void ReplyClientConnect(SOCKET _clientSocket);
int main()
{
	
	if (!InitializeCriticalSectionAndSpinCount(&criticalSection,
		0x00000400))
		return 1;

	WSADATA wsaData;
	SOCKET listenSocket, clientSocket;
	sockaddr_in server_addr, client_addr;
	ZeroMemory(&server_addr, sizeof(sockaddr_in));
	//
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return 1;
	}
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		return 1;
	}

	//Init server addres struct..
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//binding..
	if (bind(listenSocket, (sockaddr*)&server_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		return 1;
	}
	//listen..
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		return 1;
	}
	std::vector<std::thread> threadPool;
	std::vector<int> test;
	while (true) {
		int len = sizeof(sockaddr_in);
		ZeroMemory(&client_addr, len);
		clientSocket = accept(listenSocket, (sockaddr*)&client_addr, &len);
		threadPool.push_back(std::thread(ReplyClientConnect, clientSocket));
		std::cout << "threadPool Count :: " << threadPool.size() << std::endl;
	}
	for (unsigned int idx = 0; idx < threadPool.size(); idx++) {
		threadPool[idx].join();
	}
	closesocket(listenSocket);
	WSACleanup();

	// Release resources used by the critical section object.
	DeleteCriticalSection(&criticalSection);
    return 0;
}

void ReplyClientConnect(SOCKET _clientSocket) {
	// to do
	char receivedBuffer[MAX_PACKET_LEN];
	int writeN = 0;
	// Enter
	EnterCriticalSection(&criticalSection);
	int readN = recv(_clientSocket, receivedBuffer, MAX_PACKET_LEN, 0);
	if (readN > 0) {
		writeN = send(_clientSocket, receivedBuffer, readN, 0);
	}
	// Leave
	LeaveCriticalSection(&criticalSection);
	closesocket(_clientSocket);
}

