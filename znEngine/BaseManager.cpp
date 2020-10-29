#include "stdafx.h"

// General
#include "BaseManager.h"


CBaseManager::CBaseManager()
	: m_Application(nullptr)
{
	OutputDebugString(L"BaseManager created.\n");
}

CBaseManager::~CBaseManager()
{
	for (auto rit = m_Managers.rbegin(); rit != m_Managers.rend(); rit++)
	{
		//_ASSERT(rit->Manager.use_count() == 1);
		rit->Manager.reset();
	}

	OutputDebugString(L"BaseManager destroyed.\n");
}



//
// IBaseManager
//
IManager* CBaseManager::AddManager(GUID Type, const std::shared_ptr<IManager>& _manager)
{
	const auto& it = std::find_if(m_Managers.begin(), m_Managers.end(), [&Type](const SManagerInfo& ManagerInfo) -> bool {return ManagerInfo.Key == Type; });
	if (it != m_Managers.end())
		throw CException("BaseManager: Unable to add new manager, becasuse already exists.");
	m_Managers.push_back(SManagerInfo(Type, _manager));
	return _manager.get();
}

void CBaseManager::RemoveManager(GUID Type)
{
	/*
	const auto& it = std::find_if(m_Managers.begin(), m_Managers.end(), [&Type](const SManagerInfo& ManagerInfo) -> bool {return ManagerInfo.Key == Type; });
	if (it == m_Managers.end())
		throw CException("BaseManager: Unable to remove existing manager, becasuse not found.");
	//_ASSERT(it->Manager.use_count() == 1);
	m_Managers.erase(it);
	*/
}

IManager* CBaseManager::GetManager(GUID Type) const
{
	const auto& it = std::find_if(m_Managers.begin(), m_Managers.end(), [&Type](const SManagerInfo& ManagerInfo) -> bool {return ManagerInfo.Key == Type; });
	if (it == m_Managers.end())
		return nullptr;

	return it->Manager.get();
}

const IApplication& CBaseManager::GetApplication() const
{
	_ASSERT(m_Application != nullptr);
	return *m_Application;
}



//
// IBaseManagerInternal
//
void CBaseManager::SetApplicationInternal(const IApplication * Application)
{
	m_Application = Application;
}
