#pragma once
#include <ShareDefine.h>
#include <SocketList.h>
#include <Runnable.h>
#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 4096
#endif // !RECV_BUFF_SZIE


#include <list>
#include <atomic>
#include <mutex>

#define ThreadLocker std::lock_guard<std::mutex>
class SocketServer : public Runnable
{
public:
	SocketServer();
	~SocketServer();

	bool Lisiten();
	bool Init(const char* Ip, const unsigned short Port);
	virtual void OnUpdate(const uint32 diff) override;
	virtual bool TryInsertSocket(SOCKET s);
	virtual void Accept();
protected:
	void CloseSocket(SOCKET s);
	virtual void Select();
	virtual void OnDelete();
	virtual void CleanUpAndDelete();
	virtual void OnAcceptSocket(SOCKET s) = 0;
	void InsertSocketToList();
	void CleanZombieSockets(const uint32 diff);

private:
	virtual void OnRecvMessage(const char* msg, SOCKET s, uint32 length) {};
	virtual void OnCloseSocket(SOCKET s) {};
	SOCKET m_Socket;
	unsigned long m_Ip;
	unsigned short m_Port;
	std::atomic<bool> m_IsBinded;

	SocketList m_SocketList;
	int ErrorCode = 0;
	fd_set fdread;
	struct timeval timeout;
	std::mutex m_SessionArrayLock; //插入或者删除Session时的锁
	std::list<SOCKET> m_InsertQueue;
	uint32 timer = 0;
};