#include <Runnable.h>


class LogRunnable : public Runnable<Log>
{
public:
	LogRunnable() {}
	~LogRunnable() 
	{
		
	}

	virtual void Start()
	{
		sLog->OutLog("Booting Log Service...");
		Runnable::Start();
	}
private:

	virtual void _Run()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
		while (true)
		{
			if (m_IsStoped) break;

			auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
			m_DiffTime = Diff.count();
			if (m_DiffTime < 16)
				continue;
			sLog->OnUpdate(m_DiffTime);
			Begin += Diff;
		}
	}
};