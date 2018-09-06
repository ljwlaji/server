#include <SocketServer.h>

class NetWorkService;
class RouterServer : public SocketServer
{
public:
	RouterServer();
	~RouterServer();

	virtual void Start();
private:
	virtual void OnAcceptSocket(SOCKET s);
	virtual int GetThreadCount() override;
private:
	typedef std::list<NetWorkService*> NetWorkPool;
	NetWorkPool m_Threads;
};