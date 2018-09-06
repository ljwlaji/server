#include "RouterServer.h"
#include <NetWorkService.h>
#include <Config.h>
#include <Log.h>

RouterServer::RouterServer()
{
}

RouterServer::~RouterServer()
{
}

void RouterServer::Start()
{
	uint32 ThreadCount = GetThreadCount();
	for (int i = 0; i < ThreadCount; i++)
	{
		NetWorkService* th = new NetWorkService(i);
		th->Start();
		m_Threads.push_back(th);
	}
	SocketServer::Start();
}

void RouterServer::OnAcceptSocket(SOCKET s)
{
	for (NetWorkPool::iterator i = m_Threads.begin(); i != m_Threads.end(); i++)
	{
		if (!(*i)->IsFull())
		{
			(*i)->InsertSocket(s);
#ifdef WIN32
			struct sockaddr_in sa;
			int len = sizeof(sa);
			if (!getpeername(s, (sockaddr*)&sa, &len))
				sLog->OutLog(___F("新连接接入 %d", s));
#endif
			return;
		}
	}
}

int RouterServer::GetThreadCount()
{
	uint16 MaxSessionCount = sConfig->GetIntDefault("Server.AllowMaxSeeionCount", 1024);
	int ThreadCount = ceil((float)MaxSessionCount / (float)SocketForSingleThread);
	sLog->OutLog(___F("Allowed Max Connecttion Count %d", MaxSessionCount));
	sLog->OutLog(___F("Network Thread Count %d", ThreadCount));
	return ThreadCount;
}
