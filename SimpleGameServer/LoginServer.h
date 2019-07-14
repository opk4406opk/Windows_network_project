#pragma once
#include <WinSock2.h>
#include "IServer.h"
#include "IPacket.h"

#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"
// 0인경우 CPU수에 맞춰서 조절.
#define MAXIMUM_THREAD_NUM 0

struct LoginPacket : public IPacket
{
public:
	BYTE_UNIT ID[20];
	BYTE_UNIT PW[20];
};

struct LoginSocketInfo {
	OVERLAPPED overlapped;
	SOCKET fd;
	BYTE_UNIT buffer[sizeof(LoginPacket)];
	int readN;
	int writeN;
	WSABUF wsabuf;
};

class LoginServer : public IServer
{
public:
	virtual void Init() override;
	virtual void Start() override;
	LoginServer();
	virtual ~LoginServer() override;
private:
	HANDLE Handles[10];
	WSADATA WsaData;
	sockaddr_in Addr;
	LoginSocketInfo* SocketInfo;
	SOCKET ListenSocket, ClientSocket;
};

