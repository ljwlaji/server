#include <SMSHandler.h>
#include <Log.h>
#include <Config.h>

SMSManager::SMSManager() : m_PostURL(""), m_MaxTimeOutTime(5000)
{
}

SMSManager::~SMSManager()
{
}

SMSManager * SMSManager::GetInstance()
{
	static SMSManager _SMSManager;
	return &_SMSManager;
}

void SMSManager::NewSMS(const char * Nubmer)
{
	m_QueueLocker.lock();
	m_SMSQueue.push_back(new SMS(m_PostURL + Nubmer));
	m_QueueLocker.unlock();
}

bool SMSManager::Init()
{
	m_PostURL = sConfig->GetStringDefault("SMS.URLTemplate", "null");
	if (m_PostURL == "null")
	{
		sLog->OutBug("SMSManager Was Started, But Request Url Was Not Defined!");
		return false;
	}
	sLog->OutSuccess(___F("SMS Service Started With Post URL : %s", m_PostURL.c_str()));
	return true;
}

void SMSManager::OnUpdate(uint32 diff)
{
	m_QueueLocker.lock();
	for (std::list<SMS*>::iterator i = m_SMSQueue.begin(); i != m_SMSQueue.end(); i++)
	{
		switch ((*i)->GetStatue())
		{
		case SMS_STATUE_INIT:
			(*i)->TrySend();
			break;
		case SMS_STATUE_WAIT:
			break;
		case SMS_STATUE_COMPELETED:
			/*
				Remove From Queue
			*/
			break;
		case SMS_STATUE_FAILED:
			/*
				Remove From Queue If Retry Out Of Chance
			 */
			break;
		}
	}
	m_QueueLocker.unlock();
}

SMSRunnable::SMSRunnable()
{
}

SMSRunnable::~SMSRunnable()
{
}

void SMSRunnable::Start()
{
	sLog->OutLog("Booting SMS Service...");
	if (sSMSManager->Init())
		Runnable::Start();
}

void SMSRunnable::OnUpdate(const uint32 diff)
{
	sSMSManager->OnUpdate(m_DiffTime);
}