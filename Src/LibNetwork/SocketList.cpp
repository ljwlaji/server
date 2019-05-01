#include <SocketList.h>
#include <Log.h>
SocketList::SocketList(unsigned char PageCount) : m_Page(PageCount), m_Size(0)
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
		socketArray[i] = 0;
}

bool SocketList::IsFull()
{
	return m_Size == SocketForSingleThread;
}

void SocketList::insertSocket(SOCKET s)
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		if (socketArray[i] == 0)
		{
			socketArray[i] = s;
			m_Size++;
			break; 
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
			m_Size--;
			sLog->OutLog(___F("移除客户端 %d, 目前容量 %d", s, SocketForSingleThread - m_Size));
			return;
		}
	}
}

void SocketList::deleteAllSocket()
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
#ifdef WIN32
		closesocket(socketArray[i]);
#else
		close(socketArray[i]);
#endif
		socketArray[i] = 0;
	}
	m_Size = 0;
}

void SocketList::makefd(fd_set* fd_list)
{
	ThreadLocker loc(ListLock);
	FD_ZERO(fd_list);
	for (int i = 0; i < SocketForSingleThread; i++)
		if (socketArray[i]>0)
			FD_SET(socketArray[i], fd_list);
}