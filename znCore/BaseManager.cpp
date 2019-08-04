#include "stdafx.h"

// General
#include "BaseManager.h"


std::shared_ptr<CBaseManager> _BaseManager;


CBaseManager::CBaseManager()
{
	OutputDebugString(L"CBaseManager created.\n");
}

CBaseManager::~CBaseManager()
{
	OutputDebugString(L"CBaseManager destroyed.\n");
}

void CBaseManager::RegisterManager(GUID _type, std::shared_ptr<IManager> _manager)
{
	if (m_Managers.find(_type) != m_Managers.end())
	{
		throw std::exception("Manager already exists.");
	}

	m_Managers.insert(std::make_pair(_type, _manager));
}

void CBaseManager::UnregisterManager(GUID _type)
{
	//m_Managers.erase(std::remove(m_Managers.begin(), m_Managers.end(), _type), m_Managers.end());
}

std::shared_ptr<IManager> CBaseManager::GetManager(GUID _type)
{
	if (m_Managers.find(_type) == m_Managers.end())
	{
		return nullptr;
	}

	return m_Managers.at(_type);
}
