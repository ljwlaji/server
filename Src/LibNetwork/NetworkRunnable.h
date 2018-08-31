#pragma once
#include <SocketList.h>
#include <Runnable.h>

class NetWorker
{
public:
	NetWorker(uint32 Page) : m_SocketList(Page), ErrorCode(0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
	}
	~NetWorker() 
	{
		m_SocketList.deleteAllSocket();
	}

	bool IsFull() { return m_SocketList.IsFull(); }
	void InsertSocket(SOCKET s) { m_SocketList.insertSocket(s); }
	uint32 GetPage() { return m_SocketList.GetPage(); }
	uint32 GetConnecttionCount() 
	{
		return m_SocketList.GetSize(); 
	}
	void OnUpdate(uint32 diff)
	{
		int ErrorCode = 0;
		SOCKET socket = INVALID_SOCKET;
		char receBuff[4096];
		m_SocketList.makefd(&fdread);
		ErrorCode = select(0, &fdread, NULL, NULL, &timeout);
		if (ErrorCode < 0)//select����0��ʾ��ʱ
		{
			//sLog->OutLog(___F("�߳� %d ���޿������ݰ�����", pList->GetPage()));
			return;
		}
		else
		{
			//����socketList�е�ÿһ��socket���鿴��Щsocket�ǿɶ��ģ�����ɶ���socket  
			//���ж�ȡ���ݵ������������������ݸ��ͻ���  
			for (int i = 0; i < m_SocketList.GetSize(); i++)
			{
				//��ȡ��Ч��socket
				socket = m_SocketList.getSocket(i);
				if (socket == INVALID_SOCKET)
					break;
				//�ж���Щsocket�ǿɶ��ģ�������socket�ǿɶ��ģ����������ȡ����  
				if (FD_ISSET(socket, &fdread))
				{
					ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
					//�������ֵ��ʾҪ�ر�������ӣ���ô�ر�������������sockeList��ȥ��  
					if (ErrorCode == 0 || ErrorCode == SOCKET_ERROR)
					{
						sLog->OutLog(___F("�ر�Socket : %d", socket));
						closesocket(socket);
						m_SocketList.deleteSocket(socket);
					}
					else
					{
						//sLog->OutLog(receBuff);
						//printf("Thread = %d, Recv %d\n", i, ErrorCode);
						//FOnRecvPacket(socket, receBuff);
					}
				}
			}
		}
	}
private:
	SocketList m_SocketList;

	int ErrorCode = 0;
	fd_set fdread;
	struct timeval timeout;
	SOCKET socket;
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
			if (m_UpdateCount >= 300)
			{
				auto ConnecttionCount = m_NetWorker.GetConnecttionCount();
				auto Page = m_NetWorker.GetPage();
				auto diff = (int)(m_TotalDiffTime / m_UpdateCount);
				static const char* Out = "Thread <%d> Diff <%d> Connecttion <%d>";
				sLog->OutWarning(___F(Out, Page, diff, ConnecttionCount).c_str());
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