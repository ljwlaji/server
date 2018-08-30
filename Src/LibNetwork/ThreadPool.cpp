#include <ThreadPool.h>
#include <Execption.h>
#include <chrono>
WorkThread::WorkThread(std::function<void()> task, const char* Name) : 
	m_Task(task), 
	m_CurrentThread(nullptr), 
	m_IsStoped(true), 
	m_UpdateCount(0), 
	m_TotalDiffTime(0),
	m_Name(Name ? Name : "Default")
{
}

WorkThread::~WorkThread()
{
	if (!m_IsStoped)
		Wait();
}

void WorkThread::Start()
{
	if (!m_IsStoped)
	{
		throw(Execption("Current WorkThread Already Started !", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return;
	}
	m_CurrentThread = new std::thread(&WorkThread::Run, this);
}

void WorkThread::Stop()
{
	if (!m_IsStoped)
		Wait();
}

void WorkThread::Run()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
	m_IsStoped = false;
	while (true)
	{
		if (m_IsStoped) break;
		auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin);
		m_DiffTime = Diff.count();
		if (m_DiffTime < 16)
			continue;
		m_Task();
		m_UpdateCount++;
		m_TotalDiffTime += m_DiffTime;
		if (m_UpdateCount >= 1000)
		{
			printf("%s Update Diff %d: ", m_Name.c_str(), (int)(m_TotalDiffTime / m_UpdateCount));
			m_UpdateCount = 0;
			m_TotalDiffTime = 0;
		}
		begin += Diff;
	}
}

void WorkThread::Wait()
{
	m_IsStoped = true;
	if (!m_CurrentThread)
	{
		throw(Execption("Can't Wait For A Thrad To Join Witch Is Not Initlized !", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return;
	}
	m_CurrentThread->join();
	delete m_CurrentThread;
	m_CurrentThread = nullptr;
}

void WorkThread::RunTask()
{
	while (true)
	{
		m_Task();
	}
}

ThreadPool::ThreadPool() : m_IsStoped(true)
{
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Start()
{
	if (!m_IsStoped)
		return;
	for (std::list<WorkThread*>::iterator itr = m_ThreadList.begin(); itr != m_ThreadList.end(); itr++)
		(*itr)->Start();
	m_IsStoped = false;
}

void ThreadPool::Stop()
{
	if (m_IsStoped)
		return;
	for (std::list<WorkThread*>::iterator itr = m_ThreadList.begin(); itr != m_ThreadList.end(); itr++)
		(*itr)->Stop();
	m_IsStoped = true;
}

void ThreadPool::SetupThread(std::function<void()> task, const char* ThreadName)
{
	m_ThreadList.push_back(new WorkThread(task, ThreadName));
}