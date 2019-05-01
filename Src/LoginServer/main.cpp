#include <LoginServer.h>
#include <LogRunnable.h>
#include <Log.h>
#include <Config.h>
#include <RouterClient.h>
#include <Execption.h>
#include <LoginDataBase.h>
#include <DNS.h>
#include <SMSHandler.h>
#ifdef WIN32
#include "WheatyExceptionReport.h"
#endif
#define CONFIG_FILE "LoginServer.conf"
#include <iostream>

bool Master()
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

	sLog->OutSuccess("");

	if (!sConfig->SetSource(CONFIG_FILE))
	{
		sLog->OutExecption(___F("Cannot Find Current Config File : <%s>", CONFIG_FILE));
		return false;
	}

	std::shared_ptr<LogRunnable> Log = std::make_shared<LogRunnable>();
	Log->Start();

	if (!sLoginDB->Connect())
	{
		sLog->OutExecption(___F("Failed To Connect LoginDB, Abort."));
		return false;
	}

	std::shared_ptr<LoginDBRunnable> LoginDB = std::make_shared<LoginDBRunnable>();
	LoginDB->Start();

	std::shared_ptr<DNSRunnable> DNS = nullptr;
	if (sConfig->GetBoolDefault("DynamicIp.Enabled", false))
	{
		DNS = std::make_shared<DNSRunnable>();
		DNS->Start();
	}

	std::shared_ptr<SMSRunnable> SMS = std::make_shared<SMSRunnable>();
	SMS->Start();

	std::shared_ptr<CRouterRunnable> RouterRunnable = nullptr;
	if (sConfig->GetBoolDefault("RouterServer.Enabled", false))
	{
		sLog->OutLog("RouterServer Is Enabled By Config File!");
		sLog->OutLog("Booting Router Client...");
		RouterRunnable = std::make_shared<CRouterRunnable>();
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
	server->Start();
}

#include <Zip/Zipper.h>
int main()
{
	std::string Before("Before"), After("");
	uint32 Lenth = strlen(Before.c_str());
	Zipper::ZipFile(Before.c_str(), Lenth);
	Master();
	while (true)
	{

	}
	return 0;
}