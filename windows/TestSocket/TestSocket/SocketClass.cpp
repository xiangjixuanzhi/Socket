#include "pch.h"
#include "SocketClass.h"


SocketClass::SocketClass()
{
	bAlreadyOepnSocket = false;
}


SocketClass::~SocketClass()
{
}

string SocketClass::OpenSocket(const char * ip, unsigned int Host, float timeout)
{	
	SocketAttribute.versionRequired = MAKEWORD(2, 2);
	if (WSAStartup(SocketAttribute.versionRequired, &SocketAttribute.wsaData))
	{
		return "[OpenSocket]WSAStartup fail";
	}

	SocketAttribute.clientsock_in.sin_family = AF_INET;
	SocketAttribute.clientsock_in.sin_port = htons(Host);
	inet_pton(AF_INET, ip, &SocketAttribute.clientsock_in.sin_addr);

	//创建套接字
	SocketAttribute.clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketAttribute.clientSocket == INVALID_SOCKET)
	{
		return "[OpenSocket]invalid socket fail";
	}


	//連接
	if (connect(SocketAttribute.clientSocket, (sockaddr *)&SocketAttribute.clientsock_in, sizeof(SocketAttribute.clientsock_in)) == SOCKET_ERROR)
	{
		return "[OpenSocket]connect fail";
	}

	//設置超時時間
	int recvTimeout = int(timeout * 1000);  
	int sendTimeout = 200;  
	setsockopt(SocketAttribute.clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(int));
	setsockopt(SocketAttribute.clientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&sendTimeout, sizeof(int));
	bAlreadyOepnSocket = true;
	return RET_OK;
}

string SocketClass::CloseSocket()
{
	if (bAlreadyOepnSocket == true)
	{
		//关闭套接字  
		closesocket(SocketAttribute.clientSocket);
		//关闭服务  
		WSACleanup();
	}
	return RET_OK;
}

string SocketClass::SendSocket(const char * command)
{
	if (bAlreadyOepnSocket == false || command == NULL)
	{
		return "[SendSocket]don't open socket fail";
	}
	size_t len = 0, data_len = strlen(command);
	len = send(SocketAttribute.clientSocket, command, data_len + 1, 0);
	if (len == data_len + 1)
	{
		return RET_OK;
	}
	else
	{
		return "[SendSocket]send fail";
	}
}

string SocketClass::RecvSocket(int len, char * RecvBuffer)
{
	if (bAlreadyOepnSocket == false)
	{
		return "[RecvSocket]don't open socket fail";
	}
	size_t len_data;
	memset(RecvBuffer, 0, len + 1);
	try {
		len_data = recv(SocketAttribute.clientSocket, RecvBuffer, len, 0);
		if (len == len_data)
		{
			return RET_OK;
		}
		else
		{
			return "[RecvSocket]recv fail";
		}
	}
	catch (...) {
		return "[RecvSocket]exception fail";
	}
}

string SocketClass::SendAndRecv(const char * command, const char * confirm, char * RecvBuffer, int len, float sleepTime, bool isCheck)
{
	if (bAlreadyOepnSocket == false || command == NULL)
	{
		return "[SendAndRecv]don't open socket fail";
	}

	// write data
	size_t send_len = 0, data_len = strlen(command);
	send_len = send(SocketAttribute.clientSocket, command, data_len + 1, 0);
	if (send_len != data_len + 1)
	{
		return "[SendAndRecv]send fail";
	}

	// delay
	Sleep(DWORD(sleepTime * 1000));

	// read data
	size_t recv_len = 0;
	memset(RecvBuffer, 0, len + 1);
	try {
		recv_len = recv(SocketAttribute.clientSocket, RecvBuffer, len, 0);
		if (len != recv_len)
		{
			return "[SendAndRecv]recv fail";
		}
	}
	catch (...) {
		return "[SendAndRecv]exception fail";
	}

	// check confirm and RecvBuffer
	if (isCheck != false)
	{
		if (strstr(RecvBuffer, confirm) != NULL)
		{
			return RET_OK;
		}
		else
		{
			return "[SendAndRecv]confirm fail";
		}
	}

	return RET_OK;
}

int SocketClass::Ascii2Hex(const char * ascii_buf, unsigned char * hex)
{
	unsigned char highByte, lowByte;
	size_t sourceLen = strlen(ascii_buf);

	for (int i = 0; i < sourceLen; i++)
	{
		highByte = ascii_buf[i] >> 4;
		lowByte = ascii_buf[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			hex[i * 2] = highByte + 0x07;
		else
			hex[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			hex[i * 2 + 1] = lowByte + 0x07;
		else
			hex[i * 2 + 1] = lowByte;
	}
	return 0;

}

int SocketClass::Hex2Ascii(const char * hex_buf, unsigned char * ascii)
{
	if (strlen(hex_buf) % 2 != 0)
	{
		return -1;
	}

	unsigned char highByte, lowByte;
	size_t sourceLen = strlen(hex_buf);

	for (int i = 0; i < sourceLen; i += 2)
	{
		highByte = toupper(hex_buf[i]);
		lowByte = toupper(hex_buf[i + 1]);

		if (highByte > 0x39)    //highByte > '9'
			highByte -= 0x37;   //highByte = highByte - 'A' + 10
		else
			highByte -= 0x30;   //highByte = highByte - '0'

		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;

		ascii[i / 2] = (highByte << 4) | lowByte;
	}
	return 0;

}
