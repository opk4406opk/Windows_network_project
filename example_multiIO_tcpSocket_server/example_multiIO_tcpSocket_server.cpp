// example_multiIO_tcpSocket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#define MAX_PACKET_LEN 1024
#define SERVER_PORT 8000
int main()
{
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

	fd_set originfds, allfds;
	int selected_fd_num;

	FD_ZERO(&originfds);
	FD_SET(listenSocket, &originfds);

	char receivedBuffer[MAX_PACKET_LEN];

	while (true) {
		allfds = originfds;
		selected_fd_num = select(0, &allfds, NULL, NULL, NULL);
		int client_addr_len = sizeof(client_addr);
		if (FD_ISSET(listenSocket, &allfds)) {
			clientSocket = accept(listenSocket, (sockaddr*)&client_addr, &client_addr_len);
			if (clientSocket == INVALID_SOCKET) {
				// error.
				continue;
			}
			FD_SET(clientSocket, &originfds);
			continue;
		}

		int readN = 0, writeN = 0;
		for (unsigned int idx = 0; idx < allfds.fd_count; idx++) {
			if (allfds.fd_array[idx] == listenSocket) continue;

			SOCKET socket_fd = allfds.fd_array[idx];
			memset(receivedBuffer, 0x00, MAX_PACKET_LEN);
			readN = recv(socket_fd, receivedBuffer, MAX_PACKET_LEN, 0);
			if (readN <= 0) {
				closesocket(clientSocket);
				FD_CLR(socket_fd, &originfds);
			} else {
				writeN = send(socket_fd, receivedBuffer, readN, 0);
			}
		}
	}

	closesocket(listenSocket);
	WSACleanup();
    return 0;
}

