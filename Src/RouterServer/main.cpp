#include <Log.h>
#include <Config.h>
#include <LogRunnable.h>
#include <DNS.h>
#include <RouterServer.h>

#define CONFIG_FILE "RouterServer.conf"
int main()
{
	sLog->OutSuccess(___F("%s", "*******************************************************************************"));
	sLog->OutSuccess(___F("%s", "***                                                                         ***"));
	sLog->OutSuccess(___F("%s", "***                                                                         ***"));
	sLog->OutSuccess(___F("%s", "***       **********                   ******                               ***"));
	sLog->OutSuccess(___F("%s", "***      /**///////                   **////**                              ***"));
	sLog->OutSuccess(___F("%s", "***      /**       ** ******  *****  **    //   ******  ******  *****       ***"));
	sLog->OutSuccess(___F("%s", "***      /******* /** **//*  **///**/**        **////**//**//* **///**      ***"));
	sLog->OutSuccess(___F("%s", "***      /**////  /** /** / /*******/**       /**   /** /** / /*******      ***"));
	sLog->OutSuccess(___F("%s", "***      /**      /** /**   /**//// //**    **/**   /** /**   /**////       ***"));
	sLog->OutSuccess(___F("%s", "***      /**      /** ***   //****** //****** //****** /***   //******      ***"));
	sLog->OutSuccess(___F("%s", "***      //       //  //     //////   //////   //////  ///     //////       ***"));
	sLog->OutSuccess(___F("%s", "***                                                                         ***"));
	sLog->OutSuccess(___F("%s", "***                                                                         ***"));
	sLog->OutSuccess(___F("%s", "*******************************************************************************"));

	sLog->OutSuccess(___F(""));

	if (!sConfig->SetSource(CONFIG_FILE))
		return false;

	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();

	std::shared_ptr<DNSRunnable> DNS = nullptr;
	if (sConfig->GetBoolDefault("DynamicIp.Enabled", false))
	{
		DNS = std::make_shared<DNSRunnable>();
		DNS->Start();
	}

	std::shared_ptr<RouterServer> server = std::make_shared<RouterServer>();
	std::string IpConfig = sConfig->GetStringDefault("Server.Ip", "127.0.0.1");
	server->Init(IpConfig == "null" ? nullptr : IpConfig.c_str(), sConfig->GetIntDefault("Server.Port", 9000));
	server->Lisiten();
	server->Start();
	while (true);

	return 0;
}