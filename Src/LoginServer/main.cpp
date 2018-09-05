#include <LoginServer.h>
#include <LogRunnable.h>
#include <Log.h>
#include <Config.h>


#include <RouterClient.h>
#include <Execption.h>

int main()
{
	if (!sConfig->SetSource("LoginServer.conf"))
		return -1;

	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();

	if (sConfig->GetBoolDefault("RouterServer.Enabled", false))
	{
		std::shared_ptr<CRouterRunnable> RouterRunnable = std::make_shared<CRouterRunnable>();
		try
		{
			RouterRunnable->Start(sConfig->GetStringDefault("RouterServer.Host", "127.0.0.1").c_str(), sConfig->GetIntDefault("RouterServer.Port", 9876));
		}
		catch (Execption& e)
		{
			e.Out();
		}
	}

	LoginServer* server = new LoginServer();
	server->Init(nullptr, 9876);
	server->Lisiten();
	return 0;
}