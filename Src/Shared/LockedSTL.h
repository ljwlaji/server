#ifndef __Locked_Stl_H__
#define __Locked_Stl_H__

#include <ShareDefine.h>
#include <mutex>
#include <list>

template<class T>
class LockedList
{
public:

	void PushFront(T _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.push_front(_E);
	}

	void PushBack(T _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.push_back(_E);
	}

	T PopFront()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		T t = *m_List.begin();
		m_List.pop_front();
		return t;
	}

	T PopBack()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		T t = *m_List.rbegin();
		m_List.pop_back();
		return t;
	}

	void Lock()
	{
		m_Locker.lock();
	}

	void UnLock()
	{
		m_Locker.unlock();
	}

	uint32 Size()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		return (uint32)m_List.size();
	}
private:
	std::list<T> m_List;
	std::mutex m_Locker;


};

#endif