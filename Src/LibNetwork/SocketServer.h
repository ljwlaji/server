#pragma once
#ifdef WIN32
#define FD_SETSIZE 5120
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h> //uni std
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif

//缓冲区最小单元大小

#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 102400
#endif // !RECV_BUFF_SZIE

#define SocketForSingleThread 512

#include <vector>
#include <list>
#include <atomic>
#include <mutex>

#define ThreadLocker std::lock_guard<std::mutex>
class SocketServer
{
public:
	SocketServer();
	~SocketServer();

	bool Lisiten();
	bool Init(const char* Ip, const unsigned short Port);
	virtual void Start();
protected:
	virtual int GetThreadCount() = 0;
	virtual void OnDelete();
	virtual void CleanUpAndDelete();
	virtual void OnAcceptSocket(SOCKET s) = 0;


private:
	SOCKET m_Socket;
	unsigned long m_Ip;
	unsigned short m_Port;
	std::atomic<bool> m_IsBinded;
};