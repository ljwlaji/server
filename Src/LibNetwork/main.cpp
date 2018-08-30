#include <SocketServer.h>
#include <LogRunnable.h>

void main()
{
	LogRunnable* Runnable = new LogRunnable();
	Runnable->Start();
	sLog->OutBug("Test");
	SocketServer* server = new SocketServer();
	server->Init(nullptr, 9876);
	server->Lisiten();
	server->Start();
}
