// example_rawSocket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
#define PACKET_LENGTH 65536
#define ICMP_ECHO 8
#define SERVER_IP "127.0.0.1"

struct Icmp_hdr {
	u_char icamp_type;
	u_char icmp_code;
	u_short icmp_chksum;
	u_short icmp_id;
	u_short icmp_seq;
	char icmp_data[1];
};

struct Ip_hdr {
	u_char ip_hl;
	u_char ip_v;
	u_char ip_tos;
	short ip_len;
	u_short ip_id;
	short ip_off;
	u_char ip_ttl;
	u_char ip_p;
	u_short ip_chksum;
	in_addr ip_src;
	in_addr ip_dst;
};

int InChkSum(u_short *p, int n);

int main()
{
	WSADATA wsaData;
	SOCKET rawSocket;
	sockaddr_in addr, from;
	Icmp_hdr* p, *rp;
	Ip_hdr* ip;
	char buffer[1024];
	int ret, sl, hlen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == INVALID_SOCKET){
		return 1;
	}

	memset(buffer, 0x00, 1024);
	p = (Icmp_hdr*)buffer;
	p->icamp_type = ICMP_ECHO;
	p->icmp_code = 0;
	p->icmp_seq = 1;
	p->icmp_id = 1;
	p->icmp_chksum = InChkSum((unsigned short*)p, sizeof(Icmp_hdr));
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addr.sin_family = AF_INET;

	ret = sendto(rawSocket, (char*)p, sizeof(Icmp_hdr), 0, (sockaddr*)&addr, sizeof(addr));
	if (ret < 0) {
		std::cout << "ICMP Request error.." << std::endl;
		return 1;
	}
	sl = sizeof(from);
	ret = recvfrom(rawSocket, buffer, 1024, 0, (sockaddr*)&from, &sl);
	if (ret < 0) {
		std::cout << "ICMP error.." << std::endl;
		return 1;
	}

	ip = (Ip_hdr*)buffer;
	hlen = (ip->ip_hl) * 4;
	rp = (Icmp_hdr*)(buffer + hlen);

	std::cout << "reply from : " << inet_ntoa(from.sin_addr) << std::endl;
	std::cout << "type : " << rp->icamp_type << std::endl;
	std::cout << "code : " << rp->icmp_code << std::endl;
	std::cout << "seq : " << rp->icmp_seq << std::endl;
	std::cout << "iden : " << rp->icmp_id << std::endl;

	closesocket(rawSocket);
	WSACleanup();
    return 0;
}

int InChkSum(u_short *p, int n) {
	register u_short answer;
	register long sum = 0;
	u_short odd_byte = 0;
	while (n > 1) {
		sum += *(p++);
		n -= 2;
	}
	if (n == 1) {
		*(u_char*)(&odd_byte) = *(u_char*)p;
		sum += odd_byte;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

