#pragma once
#include <SocketList.h>
#include <Runnable.h>

class NetWorker
{
public:
	NetWorker(uint32 Page) : m_SocketList(Page)
	{

	}
	~NetWorker() 
	{
		m_SocketList.deleteAllSocket();
	}

	bool IsFull() { return m_SocketList.IsFull(); }
	void InsertSocket(SOCKET s) { m_SocketList.insertSocket(s); }
	void OnUpdate(uint32 diff)
	{
		int ErrorCode = 0;
		fd_set fdread;//存在读文件的set，select会检测这个set中是否可以从某些socket中读入信息  

		struct timeval timeout;//设置select超时的时间  
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		SOCKET socket;

		while (true)
		{
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
				for (int i = 0; i < FD_SETSIZE; i++)
				{
					//读取有效的socket
					socket = m_SocketList.getSocket(i);
					char receBuff[4096];
					if (!socket)
						continue;
					//判断哪些socket是可读的，如果这个socket是可读的，从它里面读取数据  
					if (FD_ISSET(socket, &fdread))
					{
						ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
						//如果返回值表示要关闭这个连接，那么关闭它，并将它从sockeList中去掉  
						if (ErrorCode == 0 || ErrorCode == SOCKET_ERROR)
						{
							closesocket(socket);
							m_SocketList.deleteSocket(socket);
						}
						else
						{
							//printf("Thread = %d, Recv %d\n", i, ErrorCode);
							//FOnRecvPacket(socket, receBuff);
						}
					}
				}
			}
		}
	}
private:
	SocketList m_SocketList;
};


class NetWorkRunnable : public Runnable<NetWorker>
{
public:
	NetWorkRunnable(uint32 page) : m_NetWorker(page)
	{}
	~NetWorkRunnable()
	{

	}

	bool IsFull() { return m_NetWorker.IsFull(); }
	void InsertSocket(SOCKET s) { return m_NetWorker.InsertSocket(s); }
	virtual void Start()
	{
		Runnable::Start();
	}
private:

	virtual void _Run()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
		while (true)
		{
			if (m_IsStoped) break;

			auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
			m_DiffTime = Diff.count();
			if (m_DiffTime < 16)
				continue;
			m_NetWorker.OnUpdate(m_DiffTime);
			m_UpdateCount++;
			m_TotalDiffTime += m_DiffTime;
			if (m_UpdateCount >= 100)
			{
				printf("Update Diff %d \n", (int)(m_TotalDiffTime / m_UpdateCount));
				m_UpdateCount = 0;
				m_TotalDiffTime = 0;
			}
			Begin += Diff;
		}
	}

	virtual void OnStoped()
	{
	}

private:
	NetWorker m_NetWorker;
};