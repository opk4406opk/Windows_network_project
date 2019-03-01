#pragma once
#include <WinSock2.h>
#include "IServer.h"
#include "IPacket.h"

#define MAX_PACKET_LINE 40
#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"
// 0�ΰ�� CPU���� ���缭 ����.
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
	BYTE_UNIT buffer[MAX_PACKET_LINE];
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

