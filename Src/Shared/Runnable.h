#pragma once
#include <ShareDefine.h>
#include <chrono>
#include <thread>
#include <Execption.h>
#include <atomic>
class Runnable
{
public:
	Runnable() : m_WorkThread(nullptr), m_IsStoped(true), m_TotalDiffTime(0), m_UpdateCount(0)
	{

	};
	
	~Runnable()
	{
		Terminate();
		printf("%s\n", "Runnable Exited");
	};

	virtual void Start()
	{
		if (!m_IsStoped)
		{
			sLog->OutWarning("Already Started !");
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

	virtual void Terminate() final
	{
		if (!m_IsStoped)
			return;
		m_IsStoped = true;
		m_WorkThread->join();
		delete m_WorkThread;
		m_WorkThread = nullptr;
	}

	virtual void OnUpdate(const uint32 diff) = 0;
	virtual void OnPrintDiff(const uint32 /*diff*/) {}
	virtual void OnStop() {};
private:
	virtual void _Run() final
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
		while (true)
		{
			if (m_IsStoped) break;

			auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
			m_DiffTime = Diff.count();
			if (m_DiffTime < 15)
			{
				this_thread::sleep_for(std::chrono::milliseconds(15 - m_DiffTime));
				continue;
			}
			OnUpdate(m_DiffTime);
			m_TotalDiffTime += m_DiffTime;
			if (++m_UpdateCount > 200)
			{
				OnPrintDiff(m_TotalDiffTime / m_UpdateCount);
				m_TotalDiffTime = 0;
				m_UpdateCount = 0;
			}
			Begin += Diff;
		}
	}
protected:
	std::thread* m_WorkThread;
	uint32 m_DiffTime;
	uint32 m_TotalDiffTime;
	uint32 m_UpdateCount;
	std::atomic<bool> m_IsStoped;
};
