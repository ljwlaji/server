#include <WorldServer.h>
#include <NetWorkService.h>
#include <Log.h>

WorldServer::WorldServer()
{
}

WorldServer::~WorldServer()
{
}

void WorldServer::Start()
{
	for (int i = 0; i < GetThreadCount(); i++)
	{
		NetWorkService* th = new NetWorkService(i);
		th->Start();
		m_Threads.push_back(th);
	}
	SocketServer::Start();
}

void WorldServer::OnAcceptSocket(SOCKET s)
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
