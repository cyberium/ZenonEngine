#include "stdafx.h"

// General
#include "Settings.h"





CSettings::CSettings(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CSettings::~CSettings()
{
	m_BaseManager.RemoveManager<ISettings>();
}

void CSettings::AddGroup(const std::string& GroupName, std::shared_ptr<ISettingGroup> Group)
{
	_ASSERT(m_Groups.find(GroupName) == m_Groups.end());
	Group->AddDefaultSettings();
	m_Groups.insert(std::make_pair(GroupName, Group));
}

std::shared_ptr<ISettingGroup> CSettings::GetGroup(const std::string& GroupName)
{
	_ASSERT(m_Groups.find(GroupName) != m_Groups.end());
	return m_Groups[GroupName];
}
