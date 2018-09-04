#pragma once
#include <Runnable.h>


class Master
{
public:
	Master();
	~Master();
	 
private:

};


class MasterRunnable : public Runnable<Master>
{
public:
	MasterRunnable();
	~MasterRunnable();

private:
	virtual void _Run();
	virtual void OnStoped();
};