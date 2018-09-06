#pragma once
#include <ShareDefine.h>
#include <HttpRequest.h>
#include <list>
#include <mutex>
#include <Runnable.h>
enum SMSSTATUE
{
	SMS_STATUE_INIT,
	SMS_STATUE_WAIT,
	SMS_STATUE_COMPELETED,
	SMS_STATUE_FAILED,
};

struct SMS
{
public:
	SMS(const std::string& PostUrl) : m_Statue(SMS_STATUE_INIT), m_TimeOutTime(0), request(PostUrl)
	{
		std::string abb = PostUrl;
		sLog->OutWarning(___F("%s", abb.c_str()));
	}
	~SMS()
	{

	}

	SMSSTATUE GetStatue() { return m_Statue; }
	void TrySend()
	{
		if (m_Statue != SMS_STATUE_INIT)
			return;

		m_Statue = SMS_STATUE_WAIT;
		respone = request.send("GET");
		if (respone.code == 200)
			m_Statue = SMS_STATUE_COMPELETED;
		else
			m_Statue = SMS_STATUE_FAILED;
	}
	uint16 GetResponeCode() { return respone.code; }
	unsigned char* GetResponeValue() { if (respone.code == 200) return respone.body.data(); return 0; }
private:
	SMSSTATUE m_Statue;
	uint32 m_TimeOutTime;
	http::Request request;
	http::Response respone;
};
class SMSManager
{
public:
	SMSManager();
	~SMSManager();
	static SMSManager* GetInstance();
	void NewSMS(const char* Nubmer);
	bool Init();
	void OnUpdate(uint32 diff);
private:

	std::string m_PostURL;
	uint32 m_MaxTimeOutTime;
	std::list<SMS*> m_SMSQueue;
	std::mutex m_QueueLocker;
};

#define sSMSManager SMSManager::GetInstance()

class SMSRunnable : public Runnable<SMSManager>
{
public:
	SMSRunnable();
	~SMSRunnable();

	virtual void Start() override;
private:
	virtual void OnUpdate(const uint32 diff) override;
};