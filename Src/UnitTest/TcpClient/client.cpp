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
			printf("�˳�cmdThread�߳�\n");
			break;
		}
		else {
			printf("��֧�ֵ����\n");
		}
	}
}

void StartTask()
{
	const int cCount = 50;
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
		client[n]->Connect("127.0.0.1", 9876);
		printf("Connect=%d\n", n);
	}

	Login login;
	strcpy(login.userName, "lyd");
	strcpy(login.PassWord, "lydmm");
	while (g_bRun)
	{
		for (int n = 0; n < cCount; n++)
		{
			client[n]->SendData(&login);
			//client[n]->OnRun();
		}
	}

	for (int n = 0; n < cCount; n++)
	{
		client[n]->Close();
	}
}
#include <iostream>
#include <fstream>
#include "HttpRequest.h"


int test()
{

	try
	{
		http::Request request("http://www.fire-core.com/sms/update-ip.php");

		// send a get request
		http::Response response = request.send("GET");
		std::cout << response.body.data() << std::endl; // print the result

														// send a post request
		response = request.send("POST", "foo=1&bar=baz", {
			"Content-Type: application/x-www-form-urlencoded"
			});
		std::cout << response.body.data() << std::endl; // print the result

														// pass parameters as a map
		std::map<std::string, std::string> parameters = { { "foo", "1" },{ "bar", "baz" } };
		response = request.send("POST", parameters, {
			"Content-Type: application/x-www-form-urlencoded"
			});
		std::cout << response.body.data() << std::endl; // print the result
	}
	catch (const std::exception& e)
	{
		std::cerr << "Request failed, error: " << e.what() << std::endl;
	}
	return 0;
}

int main()
{
	test();
	std::thread t1(cmdThread);
	t1.detach();
	StartTask();
}