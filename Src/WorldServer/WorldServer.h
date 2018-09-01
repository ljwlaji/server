#pragma once
#include <SocketServer.h>
#include <list>

class NetWorkService;
class WorldServer : public SocketServer
{
public:
	WorldServer();
	~WorldServer();

	virtual void Start();
private:
	virtual void OnAcceptSocket(SOCKET s);


private:
	typedef std::list<NetWorkService*> NetWorkPool;
	NetWorkPool m_Threads;
};
