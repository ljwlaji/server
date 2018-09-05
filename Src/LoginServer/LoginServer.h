#pragma once
#include <SocketServer.h>
#include <list>

class NetWorkService;
class LoginServer : public SocketServer
{
public:
	LoginServer();
	~LoginServer();

	virtual void Start();
private:
	virtual void OnAcceptSocket(SOCKET s);
	virtual int GetThreadCount() override;

private:
	typedef std::list<NetWorkService*> NetWorkPool;
	NetWorkPool m_Threads;
};

