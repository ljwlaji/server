#pragma once
#include <ShareDefine.h>
#include <chrono>
#include <thread>
#include <Execption.h>
#include <atomic>
template<class T>
class Runnable
{
public:
	Runnable() : m_WorkThread(nullptr), m_IsStoped(true), m_TotalDiffTime(0), m_UpdateCount(0)
	{

	};
	~Runnable()
	{
		Stop();
	};

	virtual void Start()
	{
		if (!m_IsStoped)
		{
			sLog->OutWarning("Log Service Is Already Started !");
			return;
		}
		m_IsStoped = false;
		m_WorkThread = new (std::nothrow)std::thread(&Runnable::_Run, this);
		if (!m_WorkThread)
		{
			throw(CREATE_EXECPTION("Create Thread Failed : Not Enough Memery"));
			m_IsStoped = true;
			return;
		}
	}

	void Wait()
	{
		m_WorkThread->join();
		delete m_WorkThread;
		m_WorkThread = nullptr;
	}
	//Shut Down Runable
	void Stop()
	{
		if (!m_IsStoped)
		{
			m_IsStoped = true;
			Wait();
		}
	}

private:
	virtual void _Run() = 0;
protected:
	std::thread* m_WorkThread;
	uint32 m_DiffTime;
	uint32 m_TotalDiffTime;
	uint32 m_UpdateCount;
	std::atomic<bool> m_IsStoped;
};