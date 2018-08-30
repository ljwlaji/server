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
		fd_set fdread;//���ڶ��ļ���set��select�������set���Ƿ���Դ�ĳЩsocket�ж�����Ϣ  

		struct timeval timeout;//����select��ʱ��ʱ��  
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		SOCKET socket;

		while (true)
		{
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
				for (int i = 0; i < FD_SETSIZE; i++)
				{
					//��ȡ��Ч��socket
					socket = m_SocketList.getSocket(i);
					char receBuff[4096];
					if (!socket)
						continue;
					//�ж���Щsocket�ǿɶ��ģ�������socket�ǿɶ��ģ����������ȡ����  
					if (FD_ISSET(socket, &fdread))
					{
						ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
						//�������ֵ��ʾҪ�ر�������ӣ���ô�ر�������������sockeList��ȥ��  
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