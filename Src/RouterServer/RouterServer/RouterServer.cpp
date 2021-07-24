#include "RouterServer.h"
#include <Config.h>
#include <Log.h>
#include <math.h>

RouterServer::RouterServer()
{
}

RouterServer::~RouterServer()
{
}

void RouterServer::Start()
{
}

void RouterServer::OnAcceptSocket(SOCKET s)
{
}

int RouterServer::GetThreadCount()
{
	uint16 MaxSessionCount = sConfig->GetIntDefault("Server.AllowMaxSeeionCount", 1024);
	int ThreadCount = ceil((float)MaxSessionCount / (float)SocketForSingleThread);
	sLog->OutLog(___F("Allowed Max Connecttion Count %d", MaxSessionCount));
	sLog->OutLog(___F("Network Thread Count %d", ThreadCount));
	return ThreadCount;
}
