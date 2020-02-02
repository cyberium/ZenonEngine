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
	OutputDebugString(L"CBaseManager destroyed.\n");
}



//
// IBaseManager
//
void CBaseManager::AddManager(GUID Type, const std::shared_ptr<IManager>& _manager)
{
	if (m_Managers.find(Type) != m_Managers.end())
		_ASSERT_EXPR(false, L"Manager already exists.");

	m_Managers.insert(std::make_pair(Type, _manager));
}

void CBaseManager::RemoveManager(GUID Type)
{
	//m_Managers.erase(std::remove(m_Managers.begin(), m_Managers.end(), Type), m_Managers.end());
}

IManager* CBaseManager::GetManager(GUID Type) const
{
	if (m_Managers.find(Type) == m_Managers.end())
		return nullptr;

	return m_Managers.at(Type).get();
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
