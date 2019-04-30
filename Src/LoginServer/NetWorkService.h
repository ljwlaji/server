#pragma once
#include <NetworkRunnable.h>

class NetWorkService : public NetWorkRunnable
{
public:
	NetWorkService(uint32 Page);
	~NetWorkService();

private:
	virtual void OnRecvMessage(const char* msg, SOCKET s) override;
	virtual void OnCloseSocket(SOCKET s) override;
};
