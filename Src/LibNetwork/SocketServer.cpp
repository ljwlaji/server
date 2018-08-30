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
			//传递进来的socketList指针  
			SocketList* socketList = SocketListVector.at(i);
			int ErrorCode = 0;
			fd_set fdread;//存在读文件的set，select会检测这个set中是否可以从某些socket中读入信息  

			struct timeval timeout;//设置select超时的时间  
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			SOCKET socket;

			while (true)
			{
				socketList->makefd(&fdread);
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
						socket = socketList->getSocket(i);
						char receBuff[40960];
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
		//5.接收请求，当收到请求后，会将客户端的信息存入clientAdd这个结构体中，并返回描述这个TCP连接的Socket  
		SOCKET sockConn = Accept();
		if (sockConn == INVALID_SOCKET)
		{
			//sLog->OutBug(___F("套接字服务端监听失败,错误代码 :%d", GetLastError()));
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
				//sLog->OutLog(___F("新连接接入 %d", sockConn));
				//OnNewSocketComming(sockConn, i);
				AllFull = false;
				break;
			}
		}

		if (AllFull)
		{
			printf("ALL FULL");
		}
		//sLog->OutLog("连接数达到上限");
		//将之前的第6步替换成了上面启动workThread这个线程函数和下面这一行代码  
		//将socket放入socketList中  
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
		//因为socket的值是一个非负整数值，所以可以将socketArray初始化为0，让它来表示数组中的这一个元素有没有被使用  
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
		//如果某一个socketArray[i]为0，表示哪一个位可以放入socket
		if (socketArray[i] == 0)
		{
			socketArray[i] = s;
			num++;
			break;//这里一定要加上break，不然一个socket会放在socketArray的多个位置上  
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
			//sLog->OutLog(___F("移除客户端 %d, 目前容量 %d", s, FD_SETSIZE - num));
			return;
		}
	}
}

void SocketList::makefd(fd_set* fd_list)
{
	ThreadLocker loc(ListLock);
	FD_ZERO(fd_list);//首先将fd_list清0  
					 //将每一个socket加入fd_list中  
	for (int i = 0; i < SocketForSingleThread; i++)
		if (socketArray[i]>0)
			FD_SET(socketArray[i], fd_list);
}