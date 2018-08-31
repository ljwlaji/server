#include <SocketServer.h>
#include <LogRunnable.h>
#include <iostream>
void main()
{
	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();
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
}