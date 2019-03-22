#include "stdafx.h"

// General
#include "Settings.h"

// Additional
#include "BaseManager.h"

CSettings::CSettings()
{
}

CSettings::~CSettings()
{
	ERASE_MAP(m_Groups);

	DelManager<ISettings>();
}

void CSettings::AddDefaults()
{
	AddSettingsGroup<CGroupOpenGL>(new CGroupOpenGL);
	AddSettingsGroup<CGroupQuality>(new CGroupQuality);
	AddSettingsGroup<CGroupRenderCaps>(new CGroupRenderCaps);
	AddSettingsGroup<CGroupVideo>(new CGroupVideo);
}

//--

void CSettings::RegisterBool(cstring _name, bool _defaultValue)
{
	assert1(m_BoolSettings.find(_name) == m_BoolSettings.end());
	m_BoolSettings.insert(make_pair(_name, _defaultValue));
}

bool& CSettings::GetBool(cstring _name)
{
	assert1(m_BoolSettings.find(_name) != m_BoolSettings.end());
	return m_BoolSettings[_name];
}

void CSettings::RegisterUint32(cstring _name, uint32 _defaultValue)
{
	assert1(m_Uint32Settings.find(_name) == m_Uint32Settings.end());
	m_Uint32Settings.insert(make_pair(_name, _defaultValue));
}

uint32& CSettings::GetUint32(cstring _name)
{
	assert1(m_Uint32Settings.find(_name) != m_Uint32Settings.end());
	return m_Uint32Settings[_name];
}

void CSettings::RegisterString(cstring _name, std::string _defaultValue)
{
	assert1(m_StringSettings.find(_name) == m_StringSettings.end());
	m_StringSettings.insert(make_pair(_name, _defaultValue));
}

std::string& CSettings::GetString(cstring _name)
{
	assert1(m_StringSettings.find(_name) != m_StringSettings.end());
	return m_StringSettings[_name];
}

void CSettings::RegisterGroup(GUID _guid, ISettingGroup* _group)
{
	assert1(m_Groups.find(_guid) == m_Groups.end());
	m_Groups.insert(std::make_pair(_guid, _group));
	_group->InitDefault();
}

ISettingGroup* CSettings::GetGroup(GUID _guid)
{
	assert1(m_Groups.find(_guid) != m_Groups.end());
	return m_Groups[_guid];
}
