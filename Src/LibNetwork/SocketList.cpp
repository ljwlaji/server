#include <SocketList.h>



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

void SocketList::deleteAllSocket()
{
	ThreadLocker loc(ListLock);
	for (int i = 0; i < SocketForSingleThread; i++)
	{
		closesocket(socketArray[i]);
		socketArray[i] = 0;
		num--;
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