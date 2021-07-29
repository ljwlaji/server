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
	virtual void OnUpdate(const uint32 diff);
private:
	LoginDataBase();
	~LoginDataBase();
};

#define sLoginDB LoginDataBase::GetInstance()

class LoginDBRunnable : public Runnable
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

	virtual void OnStop() override
	{
		sLoginDB->Close();
	}

private:
	virtual void OnUpdate(const uint32 diff) override
	{
		sLoginDB->OnUpdate(diff);
	}
};