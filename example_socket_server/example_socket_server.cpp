// example_socket_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>

#define MAX_PACKET_LEN 1024
#define SERVER_PORT 8000
int main(){
	WSADATA wsaData;
	SOCKET listenSocket, clientSocket;
	sockaddr_in server_addr, client_addr;
	ZeroMemory(&server_addr, sizeof(sockaddr_in));
	//
	char receivedBuffer[MAX_PACKET_LEN];
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
	if (bind(listenSocket,(sockaddr*)&server_addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		return 1;
	}
	//listen..
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		return 1;
	}

	int readN = 0, writeN = 0, len = 0;
	while (true) {
		ZeroMemory(&client_addr, sizeof(sockaddr_in));
		len = sizeof(sockaddr_in);
		clientSocket = accept(listenSocket, (sockaddr*)&client_addr, &len);
		//
		readN = recv(clientSocket, receivedBuffer, MAX_PACKET_LEN, 0);
		if (readN > 0) {
			writeN = send(clientSocket, receivedBuffer, readN, 0);
		}
		closesocket(clientSocket);
	}
	closesocket(listenSocket);
	WSACleanup();

	return 0;


}

