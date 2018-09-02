#include <RouterClient.h>
#include <Execption.h>

RouterClient::RouterClient() : m_Socket(INVALID_SOCKET)
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
}

void RouterClient::OnUpdate(uint32 diff)
{
	int nLen = (int)recv(m_Socket, m_Buffer, 4096, 0);
	if (nLen <= 0)
		throw(CREATE_EXECPTION(___F("Router Client Disconnected !").c_str()));
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

void CRouterRunnable::_Run()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
	while (true)
	{
		if (m_IsStoped) break;

		auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
		m_DiffTime = Diff.count();
		if (m_DiffTime < 16)
			continue;
		sRouterClient->OnUpdate(m_DiffTime);
		m_TotalDiffTime += m_DiffTime;
		Begin += Diff;
	}
}

void CRouterRunnable::OnStoped()
{
}
