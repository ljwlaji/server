#include <SocketList.h>



SocketList::SocketList(unsigned char PageCount) : m_Page(PageCount), m_Size(0)
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
		//��Ϊsocket��ֵ��һ���Ǹ�����ֵ�����Կ��Խ�socketArray��ʼ��Ϊ0����������ʾ�����е���һ��Ԫ����û�б�ʹ��  
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
		//���ĳһ��socketArray[i]Ϊ0����ʾ��һ��λ���Է���socket
		if (socketArray[i] == 0)
		{
			socketArray[i] = s;
			m_Size++;
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
			m_Size--;
			//sLog->OutLog(___F("�Ƴ��ͻ��� %d, Ŀǰ���� %d", s, FD_SETSIZE - num));
			return;
		}
	}
}

void SocketList::deleteAllSocket()
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		closesocket(socketArray[i]);
		socketArray[i] = 0;
	}
	m_Size = 0;
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