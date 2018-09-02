#pragma once
#include <SocketServer.h>
#include <Runnable.h>
class RouterClient
{
public:
	bool Init();
	void Connect(const char* Ip, const unsigned short Port);
	void OnUpdate(uint32 diff);
	static RouterClient* GetInstance()
	{
		static RouterClient _RouterClient;
		return &_RouterClient;
	}
private:
	RouterClient();
	~RouterClient();
	SOCKET m_Socket;
	char m_Buffer[4096];
};

#define sRouterClient RouterClient::GetInstance()

class CRouterRunnable : public Runnable<RouterClient>
{
public:
	CRouterRunnable();
	~CRouterRunnable();

	virtual void Start(const char* Ip, uint16 Port);
private:
	virtual void _Run();
	virtual void OnStoped();
};