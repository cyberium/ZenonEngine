#pragma once

#include "CRefItem.h"

template <class T, class ID_TYPE>
class RefManager2Dim
{
public:
	RefManager2Dim() {}
	~RefManager2Dim()
	{
		for (auto it = items.begin(); it != items.end(); ++it)
		{
			auto obj = *it;
			pre_delete(it->first, it->second);
			it = objects.erase(it);
			delete obj.second;
		}
	}

	inline T* Add(cstring name);

	// Delete

	inline void Delete(cstring name);
	inline void Delete(ID_TYPE id);
	inline void Delete(T* item);


	// Exists

	inline bool Exists(cstring name)
	{
		return (names.find(name) != names.end());
	}
	inline bool ExistsID(ID_TYPE id)
	{
		return (items.find(id) != items.end());
	}

	// Getters

	inline ID_TYPE GetIDByName(cstring name);
	inline T* GetItemByID(ID_TYPE id);
	inline T* GetItemByName(cstring name);

	// Console
	inline void PrintAllInfo();

protected:
	virtual ID_TYPE GenerateID() = 0;
	virtual T* CreateAction(cstring name, ID_TYPE id) = 0;
	virtual bool DeleteAction(cstring name, ID_TYPE id) = 0;

	inline void do_add(cstring name, ID_TYPE id, T* item)
	{
		item->AddRef();

		names[name] = id;
		items[id] = item;
	}

private:
	inline void pre_delete(ID_TYPE id, T* item)
	{
		std::string itemName;

		for (auto it = names.begin(); it != names.end(); ++it)
		{
			if (it->second == id)
			{
				itemName = it->first;
				break;
			}
		}

		// Delete action
		DeleteAction(itemName, id);

		// Delete from arrays
		do_delete(itemName, id);

		delete item;
	}

	inline void do_delete(cstring name, ID_TYPE id)
	{
		names.erase(names.find(name));
		items.erase(items.find(id));
	}

public:
	map<std::string, ID_TYPE> names; // names - id
	map<ID_TYPE, T*> items; //id - item
};

#include "RefManager2Dim.inl"