#include <LoginServer.h>
#include <LogRunnable.h>
#include <Log.h>

int main()
{
	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();


	LoginServer* server = new LoginServer();
	server->Init(nullptr, 9876);
	server->Lisiten();
	server->Start();
	return 0;
}