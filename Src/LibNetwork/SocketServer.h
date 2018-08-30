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

//��������С��Ԫ��С

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
	int num;//��¼socket����ʵ��Ŀ  
	SOCKET socketArray[SocketForSingleThread];//���socket������  
	std::mutex ListLock;
	unsigned char m_Page;
public:
	bool IsFull();
	SOCKET getSocket(int i) { return socketArray[i]; }
	//���캯���ж�������Ա�������г�ʼ��  
	SocketList(unsigned char PageCount);
	//��socketArray�����һ��socket  
	void insertSocket(SOCKET s);
	//��socketArray��ɾ��һ��socket  
	void deleteSocket(SOCKET s);
	unsigned char GetPage() { return m_Page; }
	//��socketArray�е��׽��ַ���fd_list����ṹ����  
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