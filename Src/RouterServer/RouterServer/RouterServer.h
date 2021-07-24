#include <SocketServer.h>

class RouterServer : public SocketServer
{
public:
	RouterServer();
	~RouterServer();

	virtual void Start() override;
private:
	virtual void OnAcceptSocket(SOCKET s) override;
private:
};