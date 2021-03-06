// example_overlapped_event_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#define SERVER_PORT 8000
#define MAX_PACKET_LINE 1024

struct SOCKETINFO {
	WSAOVERLAPPED overlapped;
	SOCKET socket_fd;
	WSABUF dataBuffer;
	char buffer[MAX_PACKET_LINE];
	int readN, writeN;
};
//
SOCKETINFO* socketArray[WSA_MAXIMUM_WAIT_EVENTS];
int eventTotal = 0;
WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
//
DWORD WINAPI threadProc(LPVOID argv);
CRITICAL_SECTION criticalSection;

int main() {
	// 크리티컬 섹션 오브젝트 초기화.
	if (!InitializeCriticalSectionAndSpinCount(&criticalSection,
		0x00000400))
		return 1;

	WSADATA wsaData;
	SOCKET listenSocket, clientSocket;
	sockaddr_in server_addr;
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
	int optval = 1;
	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (bind(listenSocket, (sockaddr*)&server_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		return 1;
	}
	//listen..
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		return 1;
	}

	unsigned long threadId;
	unsigned long flags;
	unsigned long readn;
	SOCKETINFO* sInfo;
	if (CreateThread(NULL, 0, threadProc, NULL, 0, &threadId) == NULL) {
		return 1;
	}
	eventTotal = 0;
	while (true) {
		if ((clientSocket = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET) {
			return 1;
		}

		//
		EnterCriticalSection(&criticalSection);
		sInfo = (SOCKETINFO*)malloc(sizeof(SOCKETINFO));
		memset((void*)sInfo, 0x00, sizeof(SOCKETINFO));

		socketArray[eventTotal] = sInfo;
		sInfo->socket_fd = clientSocket;
		sInfo->dataBuffer.len = MAX_PACKET_LINE;
		sInfo->dataBuffer.buf = socketArray[eventTotal]->buffer;

		eventArray[eventTotal] = WSACreateEvent();
		sInfo->overlapped.hEvent = eventArray[eventTotal];

		flags = 0;
		WSARecv(socketArray[eventTotal]->socket_fd, &socketArray[eventTotal]->dataBuffer, 1, &readn, &flags,
			&(socketArray[eventTotal]->overlapped), NULL);

		eventTotal++;
		LeaveCriticalSection(&criticalSection);
	}
	return 0;
}

DWORD WINAPI threadProc(LPVOID argv) {
	unsigned long readN;
	unsigned long index;
	unsigned long flags;
	SOCKETINFO *sInfo;

	while (true) {
		if ((index = WSAWaitForMultipleEvents(eventTotal, eventArray, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED) {
			continue;
		}
		sInfo = socketArray[index - WSA_WAIT_EVENT_0];
		WSAResetEvent(eventArray[index - WSA_WAIT_EVENT_0]);

		if (WSAGetOverlappedResult(sInfo->socket_fd,
			&(sInfo->overlapped),
			&readN, FALSE, &flags) == FALSE || readN == 0) {
			closesocket(sInfo->socket_fd);
			free(sInfo);
			WSACloseEvent(eventArray[index - WSA_WAIT_EVENT_0]);

			EnterCriticalSection(&criticalSection);
			if ((index - WSA_WAIT_EVENT_0 + 1) != eventTotal) {
				//  event, socket data 정리.
				for (int i = index - WSA_WAIT_EVENT_0; i < eventTotal; i++) {
					eventArray[i] = eventArray[i + 1];
					socketArray[i] = socketArray[i + 1];
				}
			}
			eventTotal--;
			LeaveCriticalSection(&criticalSection);
			continue;
		}

		//
		memset((void*)&sInfo->overlapped, 0x00, sizeof(WSAOVERLAPPED));
		sInfo->overlapped.hEvent = eventArray[index - WSA_WAIT_EVENT_0];
		sInfo->dataBuffer.len = MAX_PACKET_LINE;
		sInfo->dataBuffer.buf = sInfo->buffer;

		int bufferLen = strlen(sInfo->buffer);
		send(sInfo->socket_fd, sInfo->buffer, bufferLen, 0);
		if (WSARecv(sInfo->socket_fd,
			&(sInfo->dataBuffer),
			1, &readN, &flags, &(sInfo->overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				// error...
			}
		}
	}
}
