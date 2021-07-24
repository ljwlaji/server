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
	bool Send(const char* Buffer, const uint32 Lenth);
private:
	RouterClient();
	~RouterClient();

	void ResetHeartBeat();
	SOCKET m_Socket;
	char m_RecvBuffer[4096];

	uint32 m_HeartBeatTimer;
};

#define sRouterClient RouterClient::GetInstance()

class CRouterRunnable : public Runnable
{
public:
	CRouterRunnable();
	~CRouterRunnable();

	virtual void Start(const char* Ip, uint16 Port);
private:
	virtual void OnUpdate(const uint32 diff) override;
};