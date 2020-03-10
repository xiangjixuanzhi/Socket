#pragma once
#include <winsock2.h>    
#include <WS2tcpip.h>
#include <stdio.h>  
#include <iostream>
#pragma comment(lib,"ws2_32.lib")  

using namespace std;

typedef struct _SocketAttribute
{
	WORD versionRequired;
	WSADATA wsaData;
	SOCKET clientSocket;
	sockaddr_in clientsock_in;
}mySocketAttribute;

#define RET_OK "ok"

class SocketClass
{

private:

	mySocketAttribute SocketAttribute;


public:

	bool bAlreadyOepnSocket;

	// ˜‹Ôìº¯”µºÍÎö˜‹º¯”µ
	SocketClass();
	~SocketClass();

	string OpenSocket(const char *ip, unsigned int Host, float timeout);
	string CloseSocket();

	string SendSocket(const char *command);
	string RecvSocket(int len, char *RecvBuffer);
	string SendAndRecv(const char *command, const char *confirm, char *RecvBuffer, int len, float sleepTime, bool isCheck = true);

	int Ascii2Hex(const char *ascii_buf, unsigned char *hex);
	int Hex2Ascii(const char *hex_buf, unsigned char *ascii);

};

