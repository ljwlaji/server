#include <LoginServer.h>
#include <NetWorkRunnable.h>
#include <Log.h>
#include <Config.h>
#include <math.h>

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{
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
		if ((*i)->TryInsertSocket(s))
		{
			struct sockaddr_in sa;
			int len = sizeof(sa);
			#ifdef WIN32
				if (!getpeername(s, (sockaddr*)&sa, &len));
			#else
				if (!getpeername(s, (sockaddr*)&sa, (socklen_t*)(&len)));
			#endif
			return;
		}
	}
	// 所有队列都加满了 断开
	sLog->OutLog("All Full");
#ifdef WIN32
		closesocket(s);
#else
		close(s);
#endif
}

int LoginServer::GetThreadCount()
{
	uint16 MaxSessionCount = sConfig->GetIntDefault("Server.AllowMaxSeeionCount", 1024);
	int ThreadCount = ceil((float)MaxSessionCount / (float)SocketForSingleThread);
	sLog->OutLog(___F("Allowed Max Connecttion Count %d", MaxSessionCount));
	sLog->OutLog(___F("Network Thread Count %d", ThreadCount));
	return ThreadCount;
}
