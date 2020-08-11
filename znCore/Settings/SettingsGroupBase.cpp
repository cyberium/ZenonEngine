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

void CSettingsGroupBase::AddSetting(std::shared_ptr<IProperty> Setting)
{
	_ASSERT(m_Settings.find(Setting->GetName()) == m_Settings.end());
	m_Settings.insert(std::make_pair(Setting->GetName(), Setting));
}

std::shared_ptr<IProperty> CSettingsGroupBase::GetSetting(const std::string & SettingName)
{
	_ASSERT(m_Settings.find(SettingName) != m_Settings.end());
	return m_Settings[SettingName];
}

bool CSettingsGroupBase::IsGroupDefault() const
{
	for (const auto& it : m_Settings)
		if (false == std::dynamic_pointer_cast<ISetting>(it.second)->IsDefault())
			return false;

	return true;
}

void CSettingsGroupBase::ResetGroup()
{
	for (const auto& it : m_Settings)
		if (false == std::dynamic_pointer_cast<ISetting>(it.second)->IsDefault())
			std::dynamic_pointer_cast<ISetting>(it.second)->ResetToDefault();
}
