#ifndef __Locked_Stl_H__
#define __Locked_Stl_H__

#include <list>
#include <mutex>

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

	void PopFront()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.pop_front();
	}

	void PopBegin(T _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		if (!m_List.size())
			_ASSERT(false);
		_E = *m_List.begin();
		m_List.pop_front();
	}

	T Begin()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		return *m_List.begin();
	}

	void PopBack()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.pop_back();
	}

	void Lock()
	{
		m_Locker.lock();
	}

	void UnLock()
	{
		m_Locker.unlock();
	}

	unsigned int Size()
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		return (unsigned int)m_List.size();
	}
private:
	std::list<T> m_List;
	std::mutex m_Locker;
};

#endif