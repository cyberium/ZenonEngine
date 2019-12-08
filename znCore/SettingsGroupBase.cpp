#include "stdafx.h"

// General
#include "SettingsGroupBase.h"

CSettingsGroupBase::CSettingsGroupBase()
{
}

CSettingsGroupBase::~CSettingsGroupBase()
{
}



//
// ISettingGroup
//
void CSettingsGroupBase::AddDefaultSettings()
{
	// Do nothing
}

void CSettingsGroupBase::AddSetting(const std::string& SettingName, std::shared_ptr<ISetting> Setting)
{
	_ASSERT(m_Settings.find(SettingName) == m_Settings.end());
	std::dynamic_pointer_cast<ISettingInternal>(Setting)->SetName(SettingName);
	std::dynamic_pointer_cast<ISettingInternal>(Setting)->ResetToDefault();
	m_Settings.insert(std::make_pair(SettingName, Setting));
}

std::shared_ptr<ISetting> CSettingsGroupBase::GetSetting(const std::string & SettingName)
{
	_ASSERT(m_Settings.find(SettingName) != m_Settings.end());
	return m_Settings[SettingName];
}

bool CSettingsGroupBase::IsGroupDefault() const
{
	for (const auto& it : m_Settings)
		if (!it.second->IsDefault())
			return false;

	return true;
}

void CSettingsGroupBase::ResetGroup()
{
	for (const auto& it : m_Settings)
		if (!it.second->IsDefault())
			std::dynamic_pointer_cast<ISettingInternal>(it.second)->ResetToDefault();
}
