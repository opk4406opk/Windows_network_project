// example_socket_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
int main()
{
	SOCKET connectSocket;
	sockaddr_in addr;
	WSAData wsaData;
	//
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return 1;
	}
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET) {
		return 1;
	}
	//set addr.
	addr.sin_family = AF_INET;
	addr.sin_port = ntohs(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	//connect to server.
	if (connect(connectSocket, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		return 1;
	}

	std::cout << "Print Message to server.." << std::endl;
	char sendBuffer[1024];
	std::cin >> sendBuffer;
	// sending.
	std::cout << "send msg : " << sendBuffer << std::endl;
	// 버퍼 크기만큼 send.
	int sendLen = strlen(sendBuffer);
	send(connectSocket, sendBuffer, sendLen, 0);
	std::cout << "send finish : " << std::endl;
	// receiving.
	std::cout << "receiving dasts.... " << std::endl;
	char receiveBuffer[1];
	std::string totalReceivedText;
	for (int readBytes = 0; readBytes < sendLen;) {
		// 1byte만큼 데이터를 읽는다.
		readBytes += recv(connectSocket, receiveBuffer, 1, 0);
		totalReceivedText.push_back(receiveBuffer[0]);
	}
	// add null terminator.
	totalReceivedText.push_back('\0');
	std::cout << "receiving Done. " << std::endl;
	std::cout << "recv msg : " << totalReceivedText << std::endl;
	closesocket(connectSocket);
	system("pause");
	return 0;
}

