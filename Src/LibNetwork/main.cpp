#include <SocketServer.h>
#include <LogRunnable.h>

void main()
{
	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();
	SocketServer* server = new SocketServer();
	server->Init(nullptr, 9876);
	server->Lisiten();
	server->Start();
}
