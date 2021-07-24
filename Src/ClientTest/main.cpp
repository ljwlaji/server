#include "EasyTcpClient.hpp"
#include<thread>
#include <future>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#ifndef WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

bool g_bRun = true;
void cmdThread()
{
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			g_bRun = false;
			printf("退出cmdThread线程\n");
			break;
		}
		else {
			printf("不支持的命令。\n");
		}
	}
}

void StartTask()
{
	const int cCount = 5;
    int sendCount = 10;
	EasyTcpClient* client[cCount];

	for (int n = 0; n < cCount; n++)
	{
		if (!g_bRun)
		{
			return;
		}
		client[n] = new EasyTcpClient();
	}
	for (int n = 0; n < cCount; n++)
	{
		if (!g_bRun)
		{
			return;
		}
		//"192.168.2.1"
		client[n]->Connect("127.0.0.1", 19999);
		printf("Connect=%d\n", n);
	}

	Login login;
	strcpy(login.userName, "asdfasdf");
	strcpy(login.PassWord, "asdfaf");
	while (--sendCount)
	{
		for (int n = 0; n < cCount; n++)
		{
			SendCount += client[n]->SendData(&login);
			//client[n]->OnRun();
		}
	}

	for (int n = 0; n < cCount; n++)
	{
		client[n]->Close();
	}
}

int main()
{
	// test();
	// std::thread t1(cmdThread);
	// t1.detach();
	StartTask();
	return 0;
}
