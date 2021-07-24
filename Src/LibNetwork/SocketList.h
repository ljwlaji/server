#pragma once
#include <ShareDefine.h>
#include <atomic>

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
#endif


#define SOCKET int
#define INVALID_SOCKET  		(SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define SocketForSingleThread 	512

struct Session
{
	SOCKET m_Socket = INVALID_SOCKET;
	uint32 m_LastPackageTime = 0;
};

struct SocketList
{
private:
	std::atomic<uint32> m_Size;
	Session m_SessionArray[SocketForSingleThread];
	unsigned char m_Page;
	uint32 m_MaxFD;
public:
	bool IsFull();
	Session* getSession(int i) { return &m_SessionArray[i]; }
	SocketList();
	uint32 getMaxFD() { return m_MaxFD; }
	bool insertSession(SOCKET s);
	bool deleteSession(SOCKET s);
	bool deleteSessionByIndex(uint32 index);
	void deleteAllSession();
	void makefd(fd_set * fd_list);

	uint32 GetSize() 
	{
		return m_Size; 
	}
};