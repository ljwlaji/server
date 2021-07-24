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

class DNSRunnable : public Runnable
{
public:
	DNSRunnable() {}
	~DNSRunnable() {}

	virtual void OnUpdate(const uint32 diff) override;
	virtual void Start() override;
private:

};