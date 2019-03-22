#pragma once

template<class T>
CRefManager1Dim<T>::CRefManager1Dim()
{
}

template<class T>
CRefManager1Dim<T>::~CRefManager1Dim()
{
	for (auto it = objects.begin(); it != objects.end(); )
	{
		std::shared_ptr<T> item = it->second;
		std::string name = GetNameByItem(item);

		this->DeleteAction(name);
		//delete item;
		it = objects.erase(it);
		//item = nullptr;
	}
}

template <class T>
std::shared_ptr<T> CRefManager1Dim<T>::Add(cstring name)
{
	std::shared_ptr<T> item = GetItemByName(name);
	if (item != nullptr)
	{
		return item;
	}

	item = CreateAction(name);
	if (item != nullptr)
	{
		objects[name] = item;
	}

	return item;
}

template<class T>
bool CRefManager1Dim<T>::Exists(cstring name) const
{
	return objects.find(name) != objects.end();
}

template <class T>
void CRefManager1Dim<T>::Delete(cstring name)
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
void CRefManager1Dim<T>::Delete(std::shared_ptr<T> item)
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
std::shared_ptr<T> CRefManager1Dim<T>::GetItemByName(cstring name) const
{
	auto name_item = objects.find(name);
	if (name_item != objects.end())
	{
		return name_item->second;
	}

	return nullptr;
}

template <class T>
std::string CRefManager1Dim<T>::GetNameByItem(std::shared_ptr<T> item) const
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

// CLog

template <class T>
void CRefManager1Dim<T>::PrintAllInfo()
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
inline std::shared_ptr<T> CRefManager1Dim<T>::CreateAction(cstring name)
{
	return NULL;
}

template<class T>
inline bool CRefManager1Dim<T>::DeleteAction(cstring name)
{
	return false;
}
