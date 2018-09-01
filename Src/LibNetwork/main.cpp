#include <SocketServer.h>
#include <LogRunnable.h>
#include <Config.h>
#include <iostream>
#include <thread>
int main()
{
	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();

	sConfig;

	SocketServer* server = new SocketServer();
	server->Init(nullptr, 9876);
	server->Lisiten();
	server->Start();
	std::thread th(&SocketServer::Start, server);

	std::string arg;
	while (true)
	{
		std::cin >> arg;
	}

	return 0;
}