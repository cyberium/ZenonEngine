#include "stdafx.h"

// General
#include "SettingsGroupBase.h"

CSettingsGroup::CSettingsGroup()
{
}

CSettingsGroup::~CSettingsGroup()
{
}



//
// ISettingGroup
//
void CSettingsGroup::AddDefaultSettings()
{
	// Do nothing
}

void CSettingsGroup::AddSetting(const std::string& SettingName, std::shared_ptr<ISetting> Setting)
{
	_ASSERT(m_Settings.find(SettingName) == m_Settings.end());
	std::dynamic_pointer_cast<ISettingInternal>(Setting)->SetName(SettingName);
	std::dynamic_pointer_cast<ISettingInternal>(Setting)->ResetToDefault();
	m_Settings.insert(std::make_pair(SettingName, Setting));
}

std::shared_ptr<ISetting> CSettingsGroup::GetSetting(const std::string & SettingName)
{
	_ASSERT(m_Settings.find(SettingName) != m_Settings.end());
	return m_Settings[SettingName];
}

bool CSettingsGroup::IsGroupDefault() const
{
	for (const auto& it : m_Settings)
		if (!it.second->IsDefault())
			return false;

	return true;
}

void CSettingsGroup::ResetGroup()
{
	for (const auto& it : m_Settings)
		if (!it.second->IsDefault())
			std::dynamic_pointer_cast<ISettingInternal>(it.second)->ResetToDefault();
}
