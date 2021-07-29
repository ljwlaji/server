#include <DNS.h>
#include <Config.h>
#include <Log.h>
#include <HttpRequest.h>

DNS::DNS() : m_RequestUrl(""), m_UpdateTimer(0)
{

}

DNS::~DNS()
{

}

DNS* DNS::GetInstance()
{
	static DNS _DNS;
	return &_DNS;
}

void DNS::Init()
{
	m_RequestUrl = sConfig->GetStringDefault("DynamicIp.UpdateUrl", "127.0.0.1") + sConfig->GetStringDefault("DynamicIp.BindSubHostName", "login");
	m_UpdateTimer = sConfig->GetIntDefault("DynamicIp.UpdateDiff", 300000);
	sLog->OutLog(___F("DNS Request URL: %s Update Timer %d", m_RequestUrl.c_str(), m_UpdateTimer));
}

void DNS::OnUpdate(uint32 diff)
{
	if (m_UpdateTimer <= diff)
	{
		try
		{
			http::Request request(m_RequestUrl.c_str());
			http::Response response = request.send("GET");
			if (response.code == 200)
				sLog->OutSuccess(___F("DNS Updated : %s", response.body.data()));
			else
				sLog->OutBug(___F("DNS Reuqested Error Code : <%d>", response.code));
		}
		catch (const std::exception& e)
		{
			sLog->OutExecption(e.what());
		}

		m_UpdateTimer = sConfig->GetIntDefault("DynamicIp.UpdateDiff", 300000);
	}
	else
		m_UpdateTimer -= diff;
}



void DNSRunnable::OnUpdate(const uint32 diff)
{
	sDNS->OnUpdate(diff);
}

void DNSRunnable::Start()
{
	sDNS->Init();
	Runnable::Start();
}