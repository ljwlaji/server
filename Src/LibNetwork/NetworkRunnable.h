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
		if (ErrorCode < 0)//select返回0表示超时
		{
			//sLog->OutLog(___F("线程 %d 暂无可用数据包接收", pList->GetPage()));
			return;
		}
		else
		{
			//遍历socketList中的每一个socket，查看那些socket是可读的，处理可读的socket  
			//从中读取数据到缓冲区，并发送数据给客户端  
			for (int i = 0; i < m_SocketList.GetSize(); i++)
			{
				//读取有效的socket
				socket = m_SocketList.getSocket(i);
				if (socket == INVALID_SOCKET)
					break;
				//判断哪些socket是可读的，如果这个socket是可读的，从它里面读取数据  
				if (FD_ISSET(socket, &fdread))
				{
					ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
					//如果返回值表示要关闭这个连接，那么关闭它，并将它从sockeList中去掉  
					if (ErrorCode == 0 || ErrorCode == SOCKET_ERROR)
					{
						CloseSocket(socket);
						continue;
					}
					OnRecvMessage(receBuff, socket);;
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

private:
	SocketList m_SocketList;
	int ErrorCode = 0;
	fd_set fdread;
	struct timeval timeout;
	SOCKET socket;
};