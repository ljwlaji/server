#pragma once
#ifdef WIN32
#define FD_SETSIZE 5120
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include<unistd.h> //uni std
#include<arpa/inet.h>
#include<string.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif

//缓冲区最小单元大小

#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 102400
#endif // !RECV_BUFF_SZIE

#define SocketForSingleThread 256

#include <vector>
#include <atomic>
#include <mutex>
#define ThreadLocker std::lock_guard<std::mutex>
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
	unsigned char GetPage() { return m_Page; }
	//将socketArray中的套接字放入fd_list这个结构体中  
	void makefd(fd_set * fd_list);
};
class SocketServer
{
public:
	SocketServer();
	~SocketServer();

	bool Lisiten();
	bool Init(const char* Ip, const unsigned short Port);
	bool Start();
	void Run();
protected:
	int GetThreadCount();
	SOCKET Accept();
	virtual void OnDelete();
	virtual void CleanUpAndDelete();



private:
	SOCKET m_Socket;
	unsigned long m_Ip;
	unsigned short m_Port;
	std::atomic<bool> m_IsBinded;
	std::vector<SocketList*> SocketListVector;
};