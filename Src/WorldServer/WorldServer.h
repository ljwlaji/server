#pragma once
#include <SocketServer.h>
#include <list>

class WorldServer : public SocketServer
{
public:
	WorldServer();
	~WorldServer();

	virtual void Start() override;
private:
	virtual void OnAcceptSocket(SOCKET s) override;
};

