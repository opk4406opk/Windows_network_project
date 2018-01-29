#pragma once
#include <WinSock2.h>
#include <iostream>
#define MAX_PACKET_LEN 1024

struct SOCKETINFO {
	SOCKET fd;
	char buf[MAX_PACKET_LEN];
	int readN, writeN;
};

WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
int eventTotal = 0;
SOCKETINFO* socketArray[WSA_MAXIMUM_WAIT_EVENTS];

int CreateSocketInfo(SOCKET s) {
	SOCKETINFO* sInfo;
	if ((eventArray[eventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT) {
		// error.
		return -1;
	}
	sInfo = (SOCKETINFO*)malloc(sizeof(SOCKETINFO));
	memset(sInfo, 0x00, sizeof(SOCKETINFO));
	sInfo->fd = s;
	sInfo->readN = 0;
	sInfo->writeN = 0;
	socketArray[eventTotal] = sInfo;

	eventTotal++;
	//success.
	return 1;
}

void FreeSockerInfo(int eventIndex) {
	SOCKETINFO* si = socketArray[eventIndex];
	closesocket(si->fd);
	free(si);

	if (WSACloseEvent(eventArray[eventIndex]) == TRUE) {
		// event close.
	} else {
		// event close failed.
	}

	for (int i = eventIndex; i < eventTotal; i++) {
		eventArray[i] = eventArray[i + 1];
		socketArray[i] = socketArray[i + 1];
	}
	eventTotal--;
}