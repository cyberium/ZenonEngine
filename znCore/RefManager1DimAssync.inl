#pragma once

#include "RefManager1DimAssync.h"

template <class T>
CRefManager1DimAssync<T>::CRefManager1DimAssync()
{
#ifndef DISABLE_ASSYNC
	if (m_Event_Add == nullptr && m_Thread_Loader == nullptr)
	{
		m_Event_Add = CreateEvent(NULL, TRUE, TRUE, NULL);
		m_Thread_Loader = CreateThread(NULL, 0, &ThreadProc, this, NULL, NULL);
		SetThreadPriority(m_Thread_Loader, THREAD_PRIORITY_BELOW_NORMAL);
	}
#endif
}

template <class T>
CRefManager1DimAssync<T>::~CRefManager1DimAssync()
{
	for (auto it = objects.begin(); it != objects.end(); )
	{
		std::shared_ptr<Texture> item = it->second;
		std::string name = GetNameByItem(item);

		this->DeleteAction(name);
		//delete item;
		it = objects.erase(it);
		//item = nullptr;
	}

#ifndef DISABLE_ASSYNC
	ResetEvent(m_Event_Add);
	CloseHandle(m_Event_Add);

	TerminateThread(m_Thread_Loader, 1);
	CloseHandle(m_Thread_Loader);
#endif
}

template <class T>
inline std::shared_ptr<T> CRefManager1DimAssync<T>::Add(cstring name)
{
	std::shared_ptr<Texture> item = GetItemByName(name);
	if (item != nullptr)
	{
		return item;
	}

	item = CreateAction(name);

#ifndef DISABLE_ASSYNC
	// Add to load list
	m_ObjectsToLoad.add(name, item);

	// Start thread if stopped
	if (m_ObjectsToLoad.size() > 0)
	{
		SetEvent(m_Event_Add);
	}
#else
	LoadAction(name, item);
#endif

	if (item != nullptr)
	{
		objects[name] = item;
	}

	return item;
}

template <class T>
bool CRefManager1DimAssync<T>::Exists(cstring name) const
{
	return objects.find(name) != objects.end();
}

template <class T>
inline void CRefManager1DimAssync<T>::Delete(cstring name)
{
	std::shared_ptr<T> item = GetItemByName(name);
	if (item != nullptr)
	{
		auto it = objects.find(name);

		this->DeleteAction(name);
		//delete item;
		objects.erase(name);
		//item = nullptr;
	}
}

template <class T>
inline void CRefManager1DimAssync<T>::Delete(std::shared_ptr<T> item)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			this->Delete(it->first);
			return;
		}
	}

	// If not found
	//delete item;
	//item = nullptr;
}

template <class T>
inline std::shared_ptr<T> CRefManager1DimAssync<T>::GetItemByName(cstring name) const
{
	auto name_item = objects.find(name);
	if (name_item != objects.end())
	{
		return name_item->second;
	}

	return nullptr;
}

template <class T>
inline std::string CRefManager1DimAssync<T>::GetNameByItem(std::shared_ptr<T> item) const
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			return it->first;
		}
	}

	return "";
}

// Log

template <class T>
inline void CRefManager1DimAssync<T>::PrintAllInfo()
{
	uint32 refsCnt = 0;
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		refsCnt += it->second.use_count();
		Log::Info("Item (%d) [%s]", it->second.use_count(), it->first.c_str());
	}

	Log::Info("Item's count [%d], items refs [%d]", objects.size(), refsCnt);
}

template<class T>
inline std::shared_ptr<T> CRefManager1DimAssync<T>::CreateAction(cstring name)
{
	return NULL;
}

template<class T>
inline void CRefManager1DimAssync<T>::LoadAction(std::string name, std::shared_ptr<T>& item)
{
}

template<class T>
inline bool CRefManager1DimAssync<T>::DeleteAction(cstring name)
{
	return false;
}

template<class T>
inline void CRefManager1DimAssync<T>::MakeContext()
{
}
