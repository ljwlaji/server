#include <thread>
#include <list>
#include <functional>
#include <atomic>
class WorkThread
{
public:
	WorkThread(std::function<void()> task, const char* Name);
	~WorkThread();

	void Start();
	void Stop();
private:
	void Run();
	void Wait();
	void RunTask();
private:
	std::thread* m_CurrentThread;
	std::function<void()> m_Task;
	std::atomic<bool> m_IsStoped;
	long long m_DiffTime;
	int m_UpdateCount;
	int m_TotalDiffTime;
	std::string m_Name;
};


class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void Start();
	void Stop();
	void SetupThread(std::function<void()> task, const char* ThreadName);
private:

private:
	std::list<WorkThread*> m_ThreadList;
	std::atomic<bool> m_IsStoped;
};