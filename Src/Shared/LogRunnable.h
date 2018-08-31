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
			m_UpdateCount++;
			m_TotalDiffTime += m_DiffTime;
			if (m_UpdateCount >= 1000)
			{
				printf("Update Diff %d \n", (int)(m_TotalDiffTime / m_UpdateCount));
				m_UpdateCount = 0;
				m_TotalDiffTime = 0;
			}
			Begin += Diff;
		}
	}

	virtual void OnStoped()
	{
	
	}
};