#pragma once
#include <DBConnecttion.h>

class LoginDataBase : public DBConnecttion
{
public:
	static LoginDataBase* GetInstance()
	{
		static LoginDataBase _LoginDataBase;
		return &_LoginDataBase;
	}
	virtual bool Connect();
private:
	LoginDataBase();
	~LoginDataBase();
};

#define sLoginDB LoginDataBase::GetInstance()

class LoginDBRunnable : public Runnable<LoginDataBase>
{
public:
	LoginDBRunnable()
	{

	}
	~LoginDBRunnable()
	{

	}
	virtual void Start() override
	{
		sLog->OutLog("Booting DataBase Service...");
		Runnable::Start();
	}

private:
	virtual void _Run() override
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
		while (true)
		{
			if (m_IsStoped) break;

			auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
			m_DiffTime = Diff.count();
			if (m_DiffTime < 16)
				continue;
			sLoginDB->OnUpdate(m_DiffTime);
			m_UpdateCount++;
			m_TotalDiffTime += m_DiffTime;
			Begin += Diff;
		}
	}
};