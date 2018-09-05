#pragma once
#include <Runnable.h>
class DNS
{
public:
	DNS();
	~DNS();
	static DNS* GetInstance();
	void OnUpdate(uint32 diff);
	void Init();
private:
	uint32 m_UpdateTimer;
	std::string m_RequestUrl;
};

#define sDNS DNS::GetInstance()

class DNSRunnable : public Runnable<DNS>
{
public:
	DNSRunnable() {}
	~DNSRunnable() {}

	virtual void _Run();
	virtual void Start();
private:

};