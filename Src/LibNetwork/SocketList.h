#pragma once

#include <SocketServer.h>
struct SocketList
{
private:
	int num;//记录socket的真实数目  
	SOCKET socketArray[SocketForSingleThread];//存放socket的数组  
	std::mutex ListLock;
	unsigned char m_Page;
public:
	bool IsFull();
	SOCKET getSocket(int i) { return socketArray[i]; }
	//构造函数中对两个成员变量进行初始化  
	SocketList(unsigned char PageCount);
	//往socketArray中添加一个socket  
	void insertSocket(SOCKET s);
	//从socketArray中删除一个socket  
	void deleteSocket(SOCKET s);
	//移除所有Socket
	void deleteAllSocket();
	unsigned char GetPage() { return m_Page; }
	//将socketArray中的套接字放入fd_list这个结构体中  
	void makefd(fd_set * fd_list);
};