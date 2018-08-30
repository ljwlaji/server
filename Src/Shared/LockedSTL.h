#ifndef __Locked_Stl_H__
#define __Locked_Stl_H__

#include <ShareDefine.h>
#include <mutex>
#include <list>

template<class T>
class LockedList
{
public:
	//LockedQueue()
	//{
	//
	//}
	//virtual ~LockedQueue()
	//{
	//
	//}

	void PushFront(T& _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.push_front(_E);
	}

	void PushBack(const T& _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.push_back(_E);
	}

	void PopFront(T& _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.pop_front(_E);
	}

	void PopBack(T& _E)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		m_List.pop_back(_E);
	}

	bool Next(T& _T)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		if (m_List.empty())
			return false;

		_T = *m_List.begin();
		m_List.pop_front();
		m_List.push_back(_T);
		return true;
	}

	bool PopNext(T& _T)
	{
		std::lock_guard<std::mutex> Locker(m_Locker);
		if (m_List.empty())
			return false;

		_T = *m_List.begin();
		m_List.pop_front();
		return true;
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
		return (uint32)m_List.size();
	}
private:
	std::list<T> m_List;
	std::mutex m_Locker;


};

#endif