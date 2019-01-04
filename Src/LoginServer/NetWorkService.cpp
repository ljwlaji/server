#include <NetWorkService.h>
static uint64 recvCount = 0;
NetWorkService::NetWorkService(uint32 Page) : NetWorkRunnable(Page)
{

}

NetWorkService::~NetWorkService()
{

}

void NetWorkService::OnRecvMessage(const char * msg, SOCKET s)
{
	recvCount += strlen(msg);
	sLog->OutLog(___F("Recv : %d K,", recvCount / 1024));
}

void NetWorkService::OnCloseSocket(SOCKET s)
{

}
