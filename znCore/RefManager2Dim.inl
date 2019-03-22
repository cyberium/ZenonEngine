#pragma once

#include <map>

template <class T, class ID_TYPE>
inline T* RefManager2Dim<T, ID_TYPE>::Add(cstring name)
{
	ID_TYPE id;

	// If exists
	if ((id = GetIDByName(name)) != 0)
	{
		items[id]->AddRef();
		//Log::Warn("Item [%s] already exists", name.c_str());
		return items[id];
	}

	// Generate ID
	id = GenerateID();

	// Create item
	T* item = CreateAction(name, id);
	//if (item == nullptr)
	//{
	//	return nullptr;
	//}

	// Add item
	do_add(name, id, item);

	return item;
}

// Delete

template <class T, class ID_TYPE>
inline void RefManager2Dim<T, ID_TYPE>::Delete(cstring name)
{
	std::map<std::string, ID_TYPE>::iterator name_id = names.find(name);
	if (name_id != names.end())
	{
		Delete(name_id->second);
	}
}

template <class T, class ID_TYPE>
inline void RefManager2Dim<T, ID_TYPE>::Delete(ID_TYPE id)
{
	T* item = GetItemByID(id);
	if (item != nullptr)
	{

		item->Release();

		if (item->NeedDelete())
		{
			pre_delete(id, item);
		}
	}
}

template <class T, class ID_TYPE>
inline void RefManager2Dim<T, ID_TYPE>::Delete(T* item)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it->second == item)
		{
			Delete(it->first);
			return;
		}
	}
}

// Getters

template <class T, class ID_TYPE>
inline ID_TYPE RefManager2Dim<T, ID_TYPE>::GetIDByName(cstring name)
{
	std::map<std::string, ID_TYPE>::iterator name_id = names.find(name);
	if (name_id != names.end())
	{
		return name_id->second;
	}

	return 0;
}

template <class T, class ID_TYPE>
inline T* RefManager2Dim<T, ID_TYPE>::GetItemByID(ID_TYPE id)
{
	std::map<ID_TYPE, T*>::iterator id_item = items.find(id);
	if (id_item != items.end())
	{
		return id_item->second;
	}

	return nullptr;
}

template <class T, class ID_TYPE>
inline T* RefManager2Dim<T, ID_TYPE>::GetItemByName(cstring name)
{
	T* item = nullptr;

	ID_TYPE id;
	if ((id = GetIDByName(name)) == 0)
		return nullptr;

	return GetItemByID(id)
}

template <class T, class ID_TYPE>
inline void RefManager2Dim<T, ID_TYPE>::PrintAllInfo()
{
	uint32 refsCnt = 0;
	for (auto it = names.begin(); it != names.end(); ++it)
	{
		T* item = GetItemByID(it->second);
		refsCnt += item->GetRefsCount();
		Log::Info("Item (%d) [%d]-[%s]", item->GetRefsCount(), it->second, it->first.c_str());
	}

	Log::Info("Item's count [%d], items refs [%d]", names.size(), refsCnt);
}

