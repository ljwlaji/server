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
	sLog->OutLog(___F("DNS Request URL: %s", m_RequestUrl.c_str()));
}

void DNS::OnUpdate(uint32 diff)
{
	if (m_UpdateTimer <= diff)
	{
		http::Request request(m_RequestUrl.c_str());
		http::Response response = request.send("GET");
		if (response.code == 200)
			sLog->OutSuccess(___F("DNS Updated : %s", response.body.data()));
		else
			sLog->OutBug(___F("DNS Reuqested Error Code : <%d>", response.code));

		m_UpdateTimer = sConfig->GetIntDefault("DynamicIp.UpdateDiff", 300000);
	}
	else
		m_UpdateTimer -= diff;
}



void DNSRunnable::_Run()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
	while (true)
	{
		if (m_IsStoped) break;

		auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
		m_DiffTime = Diff.count();
		if (m_DiffTime < 16)
			continue;
		sDNS->OnUpdate(m_DiffTime);
		m_TotalDiffTime += m_DiffTime;
		Begin += Diff;
	}
}

void DNSRunnable::Start()
{
	sDNS->Init();
	Runnable::Start();
}