#include "stdafx.h"

// General
#include "BaseManager.h"


CBaseManager::CBaseManager()
	: m_Application(nullptr)
{
	OutputDebugString(L"CBaseManager created.\n");
}

CBaseManager::~CBaseManager()
{
	for (auto rit = m_Managers.rbegin(); rit != m_Managers.rend(); rit++)
		rit->Manager.reset();

	OutputDebugString(L"CBaseManager destroyed.\n");
}



//
// IBaseManager
//
IManager* CBaseManager::AddManager(GUID Type, const std::shared_ptr<IManager>& _manager)
{
	const auto& it = std::find_if(m_Managers.begin(), m_Managers.end(), [&Type](const SManagerInfo& ManagerInfo) -> bool {return ManagerInfo.Key == Type; });
	if (it != m_Managers.end())
		_ASSERT_EXPR(false, L"Manager already exists.");

	m_Managers.push_back(SManagerInfo(Type, _manager));
	return _manager.get();
}

void CBaseManager::RemoveManager(GUID Type)
{
	//m_Managers.erase(std::remove(m_Managers.begin(), m_Managers.end(), Type), m_Managers.end());
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
