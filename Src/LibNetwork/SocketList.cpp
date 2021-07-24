#include <SocketList.h>
#include <Log.h>
SocketList::SocketList() : m_Size(0), m_MaxFD(0)
{

}

bool SocketList::IsFull()
{
	return m_Size == SocketForSingleThread;
}

bool SocketList::insertSession(SOCKET s)
{
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		if (m_SessionArray[i].m_Socket == INVALID_SOCKET)
		{
			m_MaxFD = m_MaxFD > s ? m_MaxFD : s;
			m_SessionArray[i].m_Socket = s;
			m_SessionArray[i].m_LastPackageTime = 0;
			m_Size++;
			sLog->OutLog(___F("makefd %d", m_MaxFD));
			return true; 
		}
	}
	return false;
}

bool SocketList::deleteSession(SOCKET s)
{
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		if (m_SessionArray[i].m_Socket == s)
			return deleteSessionByIndex(i);
	}
	return false;
}

bool SocketList::deleteSessionByIndex(uint32 index)
{
	int socket = m_SessionArray[index].m_Socket;
	if (socket == INVALID_SOCKET)
		return false;
	sLog->OutLog(___F("移除客户端 %d, 目前容量 %d", m_SessionArray[index].m_Socket, SocketForSingleThread - --m_Size));
	m_SessionArray[index].m_Socket = INVALID_SOCKET;
	if (m_MaxFD == socket) 
		m_MaxFD = 0;
	for (int i = 1; i != SocketForSingleThread; ++i)
	{
		int soc = m_SessionArray[index].m_Socket;
		if (soc != INVALID_SOCKET)
			m_MaxFD = m_MaxFD > soc ? m_MaxFD : soc;
	}
	return true;
}

void SocketList::deleteAllSession()
{
	for (int i = 0; i < SocketForSingleThread; i++)
	{
#ifdef WIN32
		closesocket(m_SessionArray[i].m_Socket);
#else
		close(m_SessionArray[i].m_Socket);
#endif
		m_SessionArray[i].m_Socket = INVALID_SOCKET;
	}
	m_Size = 0;
	m_MaxFD = 0;
}

void SocketList::makefd(fd_set* fd_list)
{
	for (int i = 0; i < SocketForSingleThread; i++)
		if (m_SessionArray[i].m_Socket != INVALID_SOCKET)
			FD_SET(m_SessionArray[i].m_Socket, fd_list);
}