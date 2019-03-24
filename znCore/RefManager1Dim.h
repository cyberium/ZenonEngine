#pragma once

template <class T>
class CRefManager1Dim
{
public:
	CRefManager1Dim();
	virtual ~CRefManager1Dim();

	std::shared_ptr<T> Add(const std::string& name);

	bool Exists(const std::string& name) const;

	void Delete(const std::string& name);
	void Delete(std::shared_ptr<T> item);

	std::shared_ptr<T> GetItemByName(const std::string& name) const;
	std::string GetNameByItem(std::shared_ptr<T> item) const;

	// Console
	void PrintAllInfo();

protected:
	virtual std::shared_ptr<T> CreateAction(const std::string& name);
	virtual bool DeleteAction(const std::string& name);

public:
	std::map<std::string, std::shared_ptr<T>> objects; // name - item
};

#include "RefManager1Dim.inl"