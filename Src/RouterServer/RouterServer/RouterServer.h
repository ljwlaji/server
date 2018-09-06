#include <SocketServer.h>

class NetWorkService;
class RouterServer : public SocketServer
{
public:
	RouterServer();
	~RouterServer();

	virtual void Start() override;
private:
	virtual void OnAcceptSocket(SOCKET s) override;
	virtual int GetThreadCount() override;
private:
	typedef std::list<NetWorkService*> NetWorkPool;
	NetWorkPool m_Threads;
};