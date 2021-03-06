// example_event_select_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include "main.h"
#define SERVER_PORT 8000

int main()
{
	WSANETWORKEVENTS networkEvents;
	int flags = 0;
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

	if (CreateSocketInfo(listenSocket) == -1) {
		// socket info failed...
		return 1;
	}
	if (WSAEventSelect(listenSocket, eventArray[eventTotal - 1], FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
		// event select failed.
		return 1;
	}

	char receivedBuffer[MAX_PACKET_LEN];
	while (true) {
		int eventIndex = WSAWaitForMultipleEvents(eventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		if (eventIndex == WSA_WAIT_FAILED) {
			// event wait failed.
			std::cout << "event wait failed" << std::endl;
		}
		if (WSAEnumNetworkEvents(socketArray[eventIndex - WSA_WAIT_EVENT_0]->fd,
			eventArray[eventIndex - WSA_WAIT_EVENT_0], &networkEvents) == SOCKET_ERROR) {
			// event type error.
			std::cout << "event type error." << std::endl;
		}

		if (networkEvents.lNetworkEvents & FD_ACCEPT) {
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
				break;
			}
			if ((clientSocket = accept(socketArray
				[eventIndex - WSA_WAIT_EVENT_0]->fd, NULL, NULL)) == INVALID_SOCKET) {
				break;
			}
			if (eventTotal > WSA_MAXIMUM_WAIT_EVENTS) {
				std::cout << "Too many connections.." << std::endl;
				closesocket(clientSocket);
			}
			//
			CreateSocketInfo(clientSocket);
			//
			if ((WSAEventSelect(clientSocket, eventArray[eventTotal - 1], FD_READ | FD_CLOSE) == SOCKET_ERROR)){
				return 1;
			}
		}

		if (networkEvents.lNetworkEvents & FD_READ) {
			flags = 0;
			memset(receivedBuffer, 0x00, MAX_PACKET_LEN);
			SOCKETINFO* socketInfo = socketArray[eventIndex - WSA_WAIT_EVENT_0];
			socketInfo->readN = recv(socketInfo->fd, socketInfo->buf, MAX_PACKET_LEN, 0);
			send(socketInfo->fd, socketInfo->buf, socketInfo->readN, 0);
			if (networkEvents.lNetworkEvents & FD_CLOSE) {
				std::cout << " socket close.." << std::endl;
				FreeSockerInfo(eventIndex - WSA_WAIT_EVENT_0);
			}
		}
	}

    return 0;
}

