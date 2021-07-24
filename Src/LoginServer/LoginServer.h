#pragma once
#include <SocketServer.h>
#include <list>

class NetWorkRunnable;
class LoginServer : public SocketServer
{
public:
	LoginServer();
	~LoginServer();

private:
	virtual void OnAcceptSocket(SOCKET s) override;
	virtual int GetThreadCount() override;

private:
	typedef std::list<NetWorkRunnable*> NetWorkPool;
	NetWorkPool m_Threads;
};

