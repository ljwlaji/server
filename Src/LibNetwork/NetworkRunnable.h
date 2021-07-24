#pragma once
#include <SocketList.h>
#include <Runnable.h>
#include <mutex>

class NetWorkRunnable : public Runnable
{
public:
	NetWorkRunnable(SOCKET m_ServerSocket) : m_SocketList(), ErrorCode(0)
	{
		FD_ZERO(&fdread);
		timeout.tv_sec = 4;
		timeout.tv_usec = 0;
	}
	~NetWorkRunnable()
	{
		m_SocketList.deleteAllSession();
		FD_ZERO(&fdread);
	}

	bool TryInsertSocket(SOCKET s) 
	{
		ThreadLocker loc(m_SessionArrayLock);
		if (m_InsertQueue.size() + m_SocketList.GetSize() == SocketForSingleThread)
			return false;
		m_InsertQueue.push_back(s);
		return true;
	}
	virtual void Start()
	{
		Runnable::Start();
	}
private:
	void InsertSocketToList()
	{
		ThreadLocker loc(m_SessionArrayLock);
		while (m_InsertQueue.size())
		{
			m_SocketList.insertSession(*m_InsertQueue.begin());
			m_InsertQueue.pop_front();
		}
	}

	void CleanZombieSockets(const uint32 diff)
	{
		timer += diff;
		if (timer < 2000)
			return;
		for (int i = 0; i < SocketForSingleThread; i++)
		{
			Session* s = m_SocketList.getSession(i);
			if (s->m_Socket == INVALID_SOCKET)
				continue;
			s->m_LastPackageTime += timer;	
			if (s->m_LastPackageTime > 60000/* for 60 sec */)
				m_SocketList.deleteSessionByIndex(i);
		}
		timer = 0;
	}

	virtual void OnUpdate(const uint32 diff)
	{		
		CleanZombieSockets(diff);
		InsertSocketToList();
		_Select(m_DiffTime);
	}

	void _Select(uint32 diff)
	{
		int ErrorCode = 0;
		char receBuff[4096];
		FD_ZERO(&fdread);
		FD_SET(m_ServerSocket, &fdread);
		ErrorCode = select(0, &fdread, NULL, NULL, &timeout);
		if (ErrorCode == 0)
			return;
		if (ErrorCode < 0)
		{
			sLog->OutLog(___F("Runnable Exit With ErrorCode %d", ErrorCode));		
			return;
		}
		else
		{
			for (int i = 0; i < SocketForSingleThread; i++)
			{
				Session* s = m_SocketList.getSession(i);
				SOCKET socket = s->m_Socket;

				if (socket == INVALID_SOCKET)
					continue;
				if (FD_ISSET(socket, &fdread))
				{
					ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
					if (ErrorCode < 0)
					{
						sLog->OutLog(___F("%d", ErrorCode));
						OnCloseSocket(socket);
						CloseSocket(socket);
						continue;
					}
					sLog->OutLog(___F("%d", ErrorCode));
					OnRecvMessage(receBuff, socket, ErrorCode);
					s->m_LastPackageTime = 0;
				}
			}
		}
	}

	void CloseSocket(SOCKET s)
	{
		m_SocketList.deleteSession(s);
#ifdef WIN32
		closesocket(s);
#else
		close(s);
#endif
	}

	virtual void OnRecvMessage(const char* msg, SOCKET s, uint32 length) {};
	virtual void OnCloseSocket(SOCKET s) {};

private:
	SocketList m_SocketList;
	int ErrorCode = 0;
	fd_set fdread;
	struct timeval timeout;
	std::mutex m_SessionArrayLock; //插入或者删除Session时的锁
	std::list<SOCKET> m_InsertQueue;
	uint32 timer = 0;
	SOCKET m_ServerSocket;
};
