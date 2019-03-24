#pragma once


#define DISABLE_ASSYNC

#ifndef DISABLE_ASSYNC
extern HANDLE m_Thread_Loader;
extern HANDLE m_Event_Add;
DWORD WINAPI ThreadProc(LPVOID lpParam);
#endif


template <class T>
class CRefManager1DimAssync
{
public:
	CRefManager1DimAssync();
	~CRefManager1DimAssync();

	std::shared_ptr<T> Add(const std::string& name);

	bool Exists(const std::string& name) const;

	void Delete(const std::string& name);
	void Delete(std::shared_ptr<T> item);

	// Getters
	std::shared_ptr<T> GetItemByName(const std::string& name) const;
	std::string GetNameByItem(std::shared_ptr<T> item) const;

	// Console
	void PrintAllInfo();

public:
	virtual std::shared_ptr<T> CreateAction(const std::string& name);
	virtual void LoadAction(std::string name, std::shared_ptr<T>& item);
	virtual bool DeleteAction(const std::string& name);
	virtual void MakeContext();

public:
	std::map<std::string, std::shared_ptr<T>> objects;

#ifndef DISABLE_ASSYNC
public:
	//static HANDLE m_Thread_Loader;
	//static HANDLE m_Event_Add;
	MapAssync<std::string, std::shared_ptr<T>> m_ObjectsToLoad;
#endif
};

#include "RefManager1DimAssync.inl"