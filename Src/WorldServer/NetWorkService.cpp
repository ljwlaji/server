#include <NetWorkService.h>

NetWorkService::NetWorkService(uint32 Page) : NetWorkRunnable(Page)
{

}

NetWorkService::~NetWorkService()
{

}

void NetWorkService::OnRecvMessage(const char * msg, SOCKET s)
{
}
