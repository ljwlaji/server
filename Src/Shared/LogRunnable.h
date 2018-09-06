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

	virtual void OnUpdate(const uint32 diff)
	{
		sLog->OnUpdate(diff);
	}
};