#include <master.h>
#include <iostream>


Master::Master()
{
}

Master::~Master()
{
}

MasterRunnable::MasterRunnable()
{
}

MasterRunnable::~MasterRunnable()
{
}

void MasterRunnable::_Run()
{
	char* Command = new char();
	while (true)
	{
		if (m_IsStoped) break;
		std::cin.getline(Command, 10);
	}
}

void MasterRunnable::OnStoped()
{
}
