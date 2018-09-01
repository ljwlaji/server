#include <SocketServer.h>
#include <Execption.h>
#include <Runnable.h>
#include <NetworkRunnable.h>
#ifndef WIN32
#include <errno.h>
#endif

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
	int err = -1;
#ifdef WIN32
	WSADATA data;
	err = WSAStartup(MAKEWORD(2, 2), &data);
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

void SocketServer::Start()
{
	while (true)
	{
		sockaddr_in addr;
		int lenth = sizeof(sockaddr_in);
		SOCKET socket_connection = INVALID_SOCKET;
#ifdef WIN32
		socket_connection = accept(m_Socket, (sockaddr*)&addr, &lenth);
#else
		socket_connection = accept(m_Socket, (sockaddr*)&addr, (socklen_t *)&lenth);
#endif
		if (INVALID_SOCKET != socket_connection)
			OnAcceptSocket(socket_connection);
		else
			sLog->OutWarning(___F("Socket Accept Error : %d", socket_connection));
		/*
		for (std::list<NetWorkRunnable<>*>::iterator i = m_Threads.begin(); i != m_Threads.end(); i++)
		{

		if (!(*i)->IsFull())
		{
		(*i)->InsertSocket(socket_connection);
		#ifdef WIN32
		struct sockaddr_in sa;
		int len = sizeof(sa);
		if (!getpeername(socket_connection, (sockaddr*)&sa, &len))
		sLog->OutLog(___F("新连接接入 %d", socket_connection));
		#endif
		return socket_connection;

		}
		}
		*/
	}
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