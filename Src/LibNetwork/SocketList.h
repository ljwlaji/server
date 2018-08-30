#pragma once

#include <SocketServer.h>
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
	//�Ƴ�����Socket
	void deleteAllSocket();
	unsigned char GetPage() { return m_Page; }
	//��socketArray�е��׽��ַ���fd_list����ṹ����  
	void makefd(fd_set * fd_list);
};