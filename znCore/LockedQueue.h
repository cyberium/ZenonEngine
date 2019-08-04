#pragma once

#ifndef _MANAGED

#include <mutex>

template <class T>
class LockedQueue
{
public:
	//! Adds an item to the queue.
	void add(const T& item)
	{
		lock();

		_queue.push_back(item);

		unlock();
	}

	//! Gets the next result in the queue, if any.
	bool next(T& result)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		if (_queue.empty())
		{
			return false;
		}

		result = _queue.front();
		_queue.pop_front();

		return true;
	}

	//! Peeks at the top of the queue. Check if the queue is empty before calling! Remember to unlock after use if autoUnlock == false.
	T& peek(bool autoUnlock = true)
	{
		lock();

		T& result = _queue.front();

		if (autoUnlock)
			unlock();

		return result;
	}

	void lock()
	{
		this->m_Lock.lock();
	}

	void unlock()
	{
		this->m_Lock.unlock();
	}

	void pop()
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		_queue.pop_front();
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_Lock);
		return _queue.empty();
	}

	std::list<T>& getList()
	{
		return _queue;
	}

private:
	std::mutex m_Lock;
	std::list<T> _queue;
};


#else

template <class T>
class LockedQueue
{
public:
	void add(const T& item)
	{
		lock();

		_queue.push_back(item);

		unlock();
	}

	bool next(T& result)
	{
		if (_queue.empty())
		{
			return false;
		}

		result = _queue.front();
		_queue.pop_front();

		return true;
	}

	T& peek(bool autoUnlock = true)
	{
		lock();

		T& result = _queue.front();

		if (autoUnlock)
			unlock();

		return result;
	}

	void lock()
	{

	}

	void unlock()
	{

	}

	void pop()
	{
		_queue.pop_front();
	}

	bool empty()
	{
		return _queue.empty();
	}

	std::list<T>& getList()
	{
		return _queue;
	}

private:
	std::list<T> _queue;
};

#endif