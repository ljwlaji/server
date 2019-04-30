#pragma once
#include <SocketList.h>
#include <Runnable.h>

class NetWorkRunnable : public Runnable<SocketList>
{
public:
	NetWorkRunnable(uint32 page) : m_SocketList(page), ErrorCode(0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
	}
	~NetWorkRunnable()
	{
		m_SocketList.deleteAllSocket();
	}

	bool IsFull() { return m_SocketList.IsFull(); }
	void InsertSocket(SOCKET s) { return m_SocketList.insertSocket(s); }
	virtual void Start()
	{
		Runnable::Start();
	}
private:

	virtual void OnUpdate(const uint32 diff)
	{
		_Select(m_DiffTime);
	}

	void _Select(uint32 diff)
	{
		int ErrorCode = 0;
		SOCKET socket = INVALID_SOCKET;
		char receBuff[4096];
		m_SocketList.makefd(&fdread);
		ErrorCode = select(0, &fdread, NULL, NULL, &timeout);
		if (ErrorCode < 0)
			return;
		else
		{
			for (int i = 0; i < m_SocketList.GetSize(); i++)
			{
				socket = m_SocketList.getSocket(i);
				if (socket == INVALID_SOCKET)
					break;
				if (FD_ISSET(socket, &fdread))
				{
					ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
					if (ErrorCode == 0 || ErrorCode == SOCKET_ERROR)
					{
						OnCloseSocket(socket);
						CloseSocket(socket);
						continue;
					}
					OnRecvMessage(receBuff, socket);
				}
			}
		}
	}

	void CloseSocket(SOCKET s)
	{
		m_SocketList.deleteSocket(s);
#ifdef WIN32
		closesocket(s);
#else
		close(s);
#endif
	}

	virtual void OnRecvMessage(const char* msg, SOCKET s) = 0;
	virtual void OnCloseSocket(SOCKET s) = 0;

private:
	SocketList m_SocketList;
	int ErrorCode = 0;
	fd_set fdread;
	struct timeval timeout;
	SOCKET socket;
};