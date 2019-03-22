#pragma once

template <class T>
class CRefManager1Dim
{
public:
	CRefManager1Dim();
	virtual ~CRefManager1Dim();

	std::shared_ptr<T> Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(std::shared_ptr<T> item);

	std::shared_ptr<T> GetItemByName(cstring name) const;
	std::string GetNameByItem(std::shared_ptr<T> item) const;

	// Console
	void PrintAllInfo();

protected:
	virtual std::shared_ptr<T> CreateAction(cstring name);
	virtual bool DeleteAction(cstring name);

public:
	std::map<std::string, std::shared_ptr<T>> objects; // name - item
};

#include "RefManager1Dim.inl"