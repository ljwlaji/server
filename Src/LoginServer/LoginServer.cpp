#include <LoginServer.h>
#include <NetWorkService.h>
#include <Log.h>
#include <Config.h>
#include <math.h>

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{
}

void LoginServer::Start()
{
	uint8 ThreadCount = GetThreadCount();
	sLog->OutLog("Creatting Network Threads...");
	for (int i = 0; i < ThreadCount; i++)
	{
		NetWorkService* th = new NetWorkService(i);
		th->Start();
		m_Threads.push_back(th);
	}
	sLog->OutLog("Startting Network Service...");
	SocketServer::Start();
}

void LoginServer::OnAcceptSocket(SOCKET s)
{
	#ifdef WIN32
	int len = 0;
	#else
	socklen_t len = 0;
	#endif
	for (NetWorkPool::iterator i = m_Threads.begin(); i != m_Threads.end(); i++)
	{
		if (!(*i)->IsFull())
		{
			(*i)->InsertSocket(s);
			struct sockaddr_in sa;
			int len = sizeof(sa);
			#ifdef WIN32
				if (!getpeername(s, (sockaddr*)&sa, &len));
			#else
				if (!getpeername(s, (sockaddr*)&sa, (socklen_t*)(&len)));
			#endif
				sLog->OutLog(___F("Client Connected %d", s));
			return;
		}
		len = 0;
	}
}

int LoginServer::GetThreadCount()
{
	uint16 MaxSessionCount = sConfig->GetIntDefault("Server.AllowMaxSeeionCount", 1024);
	int ThreadCount = ceil((float)MaxSessionCount / (float)SocketForSingleThread);
	sLog->OutLog(___F("Allowed Max Connecttion Count %d", MaxSessionCount));
	sLog->OutLog(___F("Network Thread Count %d", ThreadCount));
	return ThreadCount;
}
