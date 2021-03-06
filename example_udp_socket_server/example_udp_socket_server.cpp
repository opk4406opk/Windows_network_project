// example_udp_socket_server.cpp : Defines the entry point for the console application.
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

	// Init.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return 1;
	}

	// tcp가 아닌 Dgram(=udp)로 소켓을 생성한다.
	listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		return 1;
	}

	//Init server addres struct..
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//biding..
	if (bind(listenSocket, (sockaddr*)&server_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		return 1;
	}
	//listen..
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		return 1;
	}

	while (1) {
		// recvfrom();
		// to do...
	}

    return 0;
}

