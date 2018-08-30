#include <SocketServer.h>
#include <Execption.h>
#include <Runnable.h>
#include <ThreadPool.h>

SocketServer::SocketServer() : m_Socket(INVALID_SOCKET), m_Ip(0), m_Port(0), m_IsBinded(false)
{

}

SocketServer::~SocketServer()
{

}

bool SocketServer::Init(const char* Ip, const unsigned short Port)
{
	m_Ip = Ip ? inet_addr(Ip) : INADDR_ANY;
	m_Port = Port;
#ifdef WIN32
	WSADATA data;
	int err = WSAStartup(MAKEWORD(2, 2), &data);
	if (err != 0)
	{
		throw(Execption("WSAStartUp Error", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return false;
	}
#else
#endif
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		throw(Execption("Socket Init Error", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
#ifdef WIN32
	addr.sin_port = htons(m_Port);
	addr.sin_addr.S_un.S_addr = m_Ip;
#else
	addr.sin_addr.s_addr = m_Ip;
#endif
	err = ::bind(m_Socket, (sockaddr*)&addr, sizeof(addr));
	if (err != 0)
	{
		throw(Execption("Bind Error", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return false;
	}
	m_IsBinded = true;
	return true;
}

bool SocketServer::Start()
{
	for (int i = 0; i < GetThreadCount(); i++)
		SocketListVector.push_back(new SocketList(i));
	ThreadPool* pool = new ThreadPool();
	for (int i = 0; i < GetThreadCount(); i++)
	{
		pool->SetupThread([=]() 
		{
			//���ݽ�����socketListָ��  
			SocketList* socketList = SocketListVector.at(i);
			int ErrorCode = 0;
			fd_set fdread;//���ڶ��ļ���set��select�������set���Ƿ���Դ�ĳЩsocket�ж�����Ϣ  

			struct timeval timeout;//����select��ʱ��ʱ��  
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			SOCKET socket;

			while (true)
			{
				socketList->makefd(&fdread);
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
						socket = socketList->getSocket(i);
						char receBuff[40960];
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
								socketList->deleteSocket(socket);
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
		}, "ThradName");
	}

	pool->Start();
	while (true)
	{
		//5.�������󣬵��յ�����󣬻Ὣ�ͻ��˵���Ϣ����clientAdd����ṹ���У��������������TCP���ӵ�Socket  
		SOCKET sockConn = Accept();
		if (sockConn == INVALID_SOCKET)
		{
			//sLog->OutBug(___F("�׽��ַ���˼���ʧ��,������� :%d", GetLastError()));
			continue;
		}
		bool AllFull = true;
		for (int i = 0; i < GetThreadCount(); i++)
		{

			if (!SocketListVector.at(i)->IsFull())
			{
				SocketList* TempList = SocketListVector.at(i);
				SocketListVector.at(i)->insertSocket(sockConn);
				//struct sockaddr_in sa;
				//int len = sizeof(sa);
				//if (!getpeername(sockConn, (sockaddr*)&sa, &len))
				//sLog->OutLog(___F("�����ӽ��� %d", sockConn));
				//OnNewSocketComming(sockConn, i);
				AllFull = false;
				break;
			}
		}

		if (AllFull)
		{
			printf("ALL FULL");
		}
		//sLog->OutLog("�������ﵽ����");
		//��֮ǰ�ĵ�6���滻������������workThread����̺߳�����������һ�д���  
		//��socket����socketList��  
	}


	return true;
}

bool SocketServer::Lisiten()
{
	if (!m_IsBinded)
	{
		throw(Execption("Socket Not Binded Yet !", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return false;
	}
	int ret = listen(m_Socket, 20);
	if (SOCKET_ERROR == ret)
	{
		throw(Execption("Fail To Lisiten !", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return false;
	}
	return true;
}

int SocketServer::GetThreadCount()
{
	return (FD_SETSIZE / SocketForSingleThread);
}

SOCKET SocketServer::Accept()
{
	sockaddr_in addr;
	int lenth = sizeof(sockaddr_in);
	SOCKET socket_connection = INVALID_SOCKET;
#ifdef WIN32
	socket_connection = accept(m_Socket, (sockaddr*)&addr, &lenth);
#else
	socket_connection = accept(m_Socket, (sockaddr*)&addr, (socklen_t *)&lenth);
#endif
	if (INVALID_SOCKET == socket_connection)
	{
		//...
	}
	else
	{
		bool AllFull = true;
		for (int i = 0; i < GetThreadCount(); i++)
		{

			if (!SocketListVector.at(i)->IsFull())
			{
				SocketList* TempList = SocketListVector.at(i);
				TempList->insertSocket(socket_connection);
				AllFull = false;
				break;
			}
		}

		if (AllFull)
		{
			return INVALID_SOCKET;
		}
	}
	return socket_connection;
}
void SocketServer::CleanUpAndDelete()
{
#ifdef WIN32
	WSACleanup();
#else
#endif
}


void SocketServer::OnDelete()
{

}

SocketList::SocketList(unsigned char PageCount) : m_Page(PageCount)
{
	ThreadLocker loc(ListLock);
	num = 0;
	for (int i = 0; i < SocketForSingleThread; i++)
		//��Ϊsocket��ֵ��һ���Ǹ�����ֵ�����Կ��Խ�socketArray��ʼ��Ϊ0����������ʾ�����е���һ��Ԫ����û�б�ʹ��  
		socketArray[i] = 0;
}

bool SocketList::IsFull()
{
	bool IsFull = true;
	ThreadLocker loc(ListLock);
	socketArray[SocketForSingleThread - 1] == 0 ? IsFull = false : IsFull = true;
	return IsFull;
}

void SocketList::insertSocket(SOCKET s)
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		//���ĳһ��socketArray[i]Ϊ0����ʾ��һ��λ���Է���socket
		if (socketArray[i] == 0)
		{
			socketArray[i] = s;
			num++;
			break;//����һ��Ҫ����break����Ȼһ��socket�����socketArray�Ķ��λ����  
		}
	}
}

void SocketList::deleteSocket(SOCKET s)
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		if (socketArray[i] == s)
		{
			socketArray[i] = 0;
			num--;
			//sLog->OutLog(___F("�Ƴ��ͻ��� %d, Ŀǰ���� %d", s, FD_SETSIZE - num));
			return;
		}
	}
}

void SocketList::makefd(fd_set* fd_list)
{
	ThreadLocker loc(ListLock);
	FD_ZERO(fd_list);//���Ƚ�fd_list��0  
					 //��ÿһ��socket����fd_list��  
	for (int i = 0; i < SocketForSingleThread; i++)
		if (socketArray[i]>0)
			FD_SET(socketArray[i], fd_list);
}