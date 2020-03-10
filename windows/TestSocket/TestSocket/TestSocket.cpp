// TestSocket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "SocketClass.h"

int main()
{
	SocketClass *pInstance = new SocketClass();
	string err = pInstance->OpenSocket("127.0.0.1", 7000, 2);
	printf("打開串口的返回值是: %s\n", err.c_str());
	//char* recv = new char[1024];
	//string recv_ret = pInstance->RecvSocket(28, recv);
	//printf("recv socket is: %s\n", recv_ret.c_str());
	//printf("recv socket return value is: %s\n", recv);
	//delete[] recv;
	////Sleep(1000);
	//string ret = pInstance->SendSocket("Hello socket!");
	//printf("send socket is: %s\n", ret.c_str());
	for (int i = 0; i < 1; i++)
	{
		char command[6] = "hello";
		char confirm[6] = { "hello" };
		char recv[10] = { 0 };
		printf("客戶端發給服務端的數據是: %s\n", command);
		string wr = pInstance->SendAndRecv(command, confirm, recv, int(strlen(confirm)), 0.2);
		printf("客戶端發給服務端返回值是: %s\n", wr.c_str());
		printf("客戶端接收服務端的數據是: %s\n", recv);
	}


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
