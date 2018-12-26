#pragma once
#include <WinSock2.h>
#include "IServer.h"
#define MAX_PACKET_LINE 1024
#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"
#define MAXIMUM_THREAD_NUM 6

struct GameSocketInfo {
	OVERLAPPED overlapped;
	SOCKET fd;
	char buffer[MAX_PACKET_LINE];
	int readN;
	int writeN;
	WSABUF wsabuf;
};

class MainServer : public IServer
{
public:
	virtual void Init() override;
	virtual void Start() override;
	MainServer();
	virtual ~MainServer() override;
private:
	HANDLE handles[10];
	WSADATA wsaData;
	sockaddr_in addr;
	GameSocketInfo* socketInfo;
	SOCKET listenSocket, clientSocket;
};

