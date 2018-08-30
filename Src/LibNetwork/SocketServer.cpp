#include <SocketServer.h>
#include <Execption.h>
#include <Runnable.h>
#include <ThreadPool.h>
#include <NetWorkRunnable.h>

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
		throw(CREATE_EXECPTION("WSAStartUp Error"));
		return false;
	}
#else
#endif
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		throw(CREATE_EXECPTION("Socket Init Error"));
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
		throw(CREATE_EXECPTION("Bind Error"));
		return false;
	}
	m_IsBinded = true;
	return true;
}

bool SocketServer::Start()
{
	std::list<NetWorkRunnable*> threads;
	for (int i = 0; i < GetThreadCount(); i++)
	{
		NetWorkRunnable* th = new NetWorkRunnable(i);
		th->Start();
		threads.push_back(th);
	}
	while (true)
	{
		//5.接收请求，当收到请求后，会将客户端的信息存入clientAdd这个结构体中，并返回描述这个TCP连接的Socket  
		SOCKET sockConn = Accept();
		if (sockConn == INVALID_SOCKET)
		{
			sLog->OutBug("套接字服务端监听失败,错误代码 :%d", GetLastError());
			continue;
		}
		bool AllFull = true;
		for (std::list<NetWorkRunnable*>::iterator i = threads.begin(); i != threads.end(); i++)
		{

			if (!(*i)->IsFull())
			{
				(*i)->InsertSocket(sockConn);
				struct sockaddr_in sa;
				int len = sizeof(sa);
				if (!getpeername(sockConn, (sockaddr*)&sa, &len))
				sLog->OutLog("新连接接入 %d", sockConn);
				//OnNewSocketComming(sockConn, i);
				AllFull = false;
				break;
			}
		}

		if (AllFull)
		{
			printf("ALL FULL");
		}
	}
	return true;
}

bool SocketServer::Lisiten()
{
	if (!m_IsBinded)
	{
		throw(CREATE_EXECPTION("Socket Not Binded Yet !"));
		return false;
	}
	int ret = listen(m_Socket, 20);
	if (SOCKET_ERROR == ret)
	{
		throw(CREATE_EXECPTION("Fail To Lisiten !"));
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
		/*
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
		*/

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