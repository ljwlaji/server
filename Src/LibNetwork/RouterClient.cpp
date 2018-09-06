#include <RouterClient.h>
#include <Execption.h>
#include <Config.h>

RouterClient::RouterClient() : m_Socket(INVALID_SOCKET), m_HeartBeatTimer(0)
{
}

RouterClient::~RouterClient()
{
}

bool RouterClient::Init()
{
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
	ResetHeartBeat();
	return true;
}

void RouterClient::Connect(const char * Ip, const unsigned short Port)
{
	if (INVALID_SOCKET == m_Socket)
	{
		Init();
	}
	// 2 连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(Port);
#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr(Ip);
#else
	_sin.sin_addr.s_addr = inet_addr(Ip);
#endif
	//printf("<socket=%d>正在连接服务器<%s:%d>...\n", _sock, ip, port);
	int ret = connect(m_Socket, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
		throw(CREATE_EXECPTION(___F("Failed To Connect Server %s at Port %d", Ip, Port).c_str()));

	int TimerOut = 1000;  //30s
	setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimerOut, sizeof(int));
}

void RouterClient::OnUpdate(uint32 diff)
{
	if (m_HeartBeatTimer <= diff)
	{
		Send("1", 1);
		sLog->OutLog("Router Client Keepping Alive...");
	}
	else m_HeartBeatTimer -= diff;

	int nLen = recv(m_Socket, m_RecvBuffer, sizeof(m_RecvBuffer), 0);
	if (nLen < -1)
		return;
}

bool RouterClient::Send(const char * Buffer, const uint32 Lenth)
{
	bool ret = send(m_Socket, Buffer, Lenth, 0) >= 0;
	if (!ret)
	{
#ifdef WIN32
		closesocket(m_Socket);
#else
		close(m_Socket);
#endif
	}
	ResetHeartBeat();
	return ret;
}

void RouterClient::ResetHeartBeat()
{
	m_HeartBeatTimer = sConfig->GetIntDefault("RouterServer.HeartBeatTimer", 5000);
}

CRouterRunnable::CRouterRunnable()
{
}

CRouterRunnable::~CRouterRunnable()
{
}

void CRouterRunnable::Start(const char * Ip, uint16 Port)
{
	sRouterClient->Connect(Ip, Port);
	Runnable::Start();
}

void CRouterRunnable::OnUpdate(const uint32 diff)
{
	sRouterClient->OnUpdate(m_DiffTime);
}