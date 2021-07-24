#include <SocketServer.h>
#include <Execption.h>
#include <Runnable.h>
#include <NetworkRunnable.h>
#include <Config.h>
#ifndef WIN32
#include <errno.h>
#endif

SocketServer::SocketServer() : 
	m_Socket(INVALID_SOCKET), 
	m_Ip(0), m_Port(0), 
	m_IsBinded(false), 
	m_SocketList()
{
	FD_ZERO(&fdread);
	timeout.tv_sec = 4;
	timeout.tv_usec = 0;
}

SocketServer::~SocketServer()
{
	FD_ZERO(&fdread);
}

bool SocketServer::Init(const char* Ip, const unsigned short Port)
{
	m_Ip = Ip ? inet_addr(Ip) : INADDR_ANY;
	m_Port = Port;
	int err = -1;
#ifdef WIN32
	WSADATA data;
	err = WSAStartup(MAKEWORD(2, 2), &data);
	if (err != 0)
	{
		throw(CREATE_EXECPTION("WSAStartUp Error"));
		return false;
	}
#endif
	m_Socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	// m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		throw(CREATE_EXECPTION("Socket Init Error"));
		return false;
	}

	sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
#ifdef WIN32
	addr.sin_addr.S_un.S_addr = m_Ip;
#else
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    addr.sin_port = htons(m_Port);
	err = ::bind(m_Socket, (sockaddr*)&addr, sizeof(addr));
	if (err)
	{
		throw(CREATE_EXECPTION("Bind Error"));
		return false;
	}
	m_IsBinded = true;
	sLog->OutLog(___F("Network Serivce Bind On <%s>:%d", Ip ? Ip : "Any Ip", m_Port));
	return true;
}

void SocketServer::Accept()
{
	while (true)
	{
		sockaddr_in addr;
		int lenth = sizeof(sockaddr_in);
		SOCKET socket_connection = INVALID_SOCKET;
#ifdef WIN32
		socket_connection = accept(m_Socket, (sockaddr*)&addr, &lenth);
#else
		// socket_connection = accept4(m_Socket, (sockaddr*)&addr, (socklen_t *)&lenth, SOCK_NONBLOCK);
		socket_connection = accept(m_Socket, (sockaddr*)&addr, (socklen_t *)&lenth);
#endif
		if (INVALID_SOCKET == socket_connection)
		{
			return;
		}
		TryInsertSocket(socket_connection);
		sLog->OutLog("新Socket进入");
	}
}

void SocketServer::CloseSocket(SOCKET s)
{
	m_SocketList.deleteSession(s);
#ifdef WIN32
	closesocket(s);
#else
	close(s);
#endif
}

void SocketServer::InsertSocketToList()
{
	ThreadLocker loc(m_SessionArrayLock);
	if (m_InsertQueue.empty()) return;
	while (m_InsertQueue.size())
	{
		m_SocketList.insertSession(*m_InsertQueue.begin());
		m_InsertQueue.pop_front();
	}
}

bool SocketServer::TryInsertSocket(SOCKET s) 
{
	ThreadLocker loc(m_SessionArrayLock);
	if (m_InsertQueue.size() + m_SocketList.GetSize() == SocketForSingleThread)
		return false;
	m_InsertQueue.push_back(s);
	return true;
}

void SocketServer::CleanZombieSockets(const uint32 diff)
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

// void SocketServer::Select()
// {
// 	FD_ZERO(&fdread);
// 	//把服务器监听的socket添加到监听的文件描述符集合
// 	FD_SET(m_Socket, &fdread);
// 	//设置监听的最大文件描述符
// 	int nMax_fd = nMax_fd > m_Socket ? nMax_fd : m_Socket;
// 	for (int i = 1; i != SocketForSingleThread; ++i)
// 	{
// 		Session* s = m_SocketList.getSession(i);
// 		if (s->m_Socket == INVALID_SOCKET)
// 			continue;
// 		FD_SET(s->m_Socket, &fdread);
// 		nMax_fd = nMax_fd > s->m_Socket ? nMax_fd : s->m_Socket;
// 	}

// 	int res = select(nMax_fd + 1, &fdread, 0, 0, NULL);
// 	if(-1 == res)
// 	{
// 		printf("select error:%m\n");
// 		return;
// 	}
// 	printf("select success\n");
// 	//判断服务器socket是否可读
// 	if(FD_ISSET(m_Socket, &fdread))
// 	{
// 		//接收新的连接
// 		int fd = accept(m_Socket, 0,0);
// 		if(-1 == fd)
// 		{
// 	 		printf("accept error:%m\n");
// 			return;
// 		}
// 		m_SocketList.insertSession(fd);
// 		printf("连接成功\n");
// 	}
// 	for(int i = 1; i != SocketForSingleThread; ++i)
// 	{
// 		Session* se = m_SocketList.getSession(i);
// 		SOCKET s = se->m_Socket;
// 		if (s == INVALID_SOCKET) continue;
// 		//判断客户是否可读
// 		printf("%d\n", s);
// 		if(FD_ISSET(s, &fdread))
// 		{
// 			unsigned char buf[4096] = {0};
// 			int res = recv(s, buf, sizeof(buf), 0);
// 			if(res > 0)
// 			{
// 				printf("%s : %d\n", buf, res);
// 			}
// 			else if(0 == res)
// 			{
// 				printf("客户端退出\n");
// 			}
// 			else
// 			{
// 				printf("recv error\n");
// 			}	
// 		}
// 	}
// }

void SocketServer::Select()
{
	int ErrorCode = 0;
	char receBuff[4096];
	FD_ZERO(&fdread);
	FD_SET(m_Socket, &fdread);
	m_SocketList.makefd(&fdread);
	int fd = m_Socket > m_SocketList.getMaxFD() ? m_Socket : m_SocketList.getMaxFD();
	ErrorCode = select(fd + 1, &fdread, NULL, NULL, &timeout);
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
				sLog->OutLog("有东西可以收取");
				ErrorCode = recv(socket, receBuff, sizeof(receBuff), 0);
				if (ErrorCode == 0)
				{
					sLog->OutLog(___F("%d", ErrorCode));
					OnCloseSocket(socket);
					CloseSocket(socket);
					continue;
				}
				OnRecvMessage(receBuff, socket, ErrorCode);
				sLog->OutLog(___F("%s : %d", receBuff, ErrorCode));
				s->m_LastPackageTime = 0;
			}
		}
	}
}

void SocketServer::OnUpdate(const uint32 diff)
{
	CleanZombieSockets(diff);
	Accept();
	InsertSocketToList();
	Select();
}

bool SocketServer::Lisiten()
{
	if (!m_IsBinded)
	{
		throw(CREATE_EXECPTION("Socket Not Binded Yet !"));
		return false;
	}
	int ret = listen(m_Socket, 10);
	if (ret < 0)
	{
		throw(CREATE_EXECPTION("Fail To Lisiten !"));
		return false;
	}
	return true;
}

void SocketServer::CleanUpAndDelete()
{
#ifdef WIN32
	WSACleanup();
#else
#endif
}

void SocketServer::Start()
{
	Runnable::Start();
}


void SocketServer::OnDelete()
{

}
