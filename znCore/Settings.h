#pragma once

#include "GroupOpenGL.h"
#include "GroupQuality.h"
#include "GroupRenderCaps.h"
#include "GroupVideo.h"

class CSettings : public ISettings
{
public:
	CSettings();
	virtual ~CSettings();

	void AddDefaults();

	// ISettings
	void RegisterBool(cstring _name, bool _defaultValue) override;
	bool& GetBool(cstring _name) override;
	void RegisterUint32(cstring _name, uint32 _defaultValue) override;
	uint32& GetUint32(cstring _name) override;
	void RegisterString(cstring _name, std::string _defaultValue) override;
	std::string& GetString(cstring _name) override;

	void RegisterGroup(GUID _guid, ISettingGroup* _group) override;
	ISettingGroup* GetGroup(GUID _guid) override;

private:
	std::map<std::string, bool> m_BoolSettings;
	std::map<std::string, uint32> m_Uint32Settings;
	std::map<std::string, std::string> m_StringSettings;
	std::map<GUID, ISettingGroup*> m_Groups;
};

template<class T>
static inline void AddSettingsGroup(ISettingGroup* _settingsGroup)
{
	GetManager<ISettings>()->RegisterGroup(__uuidof(T), _settingsGroup);
}

template<class T>
static inline T& GetSettingsGroup()
{
	return *(T*)(GetManager<ISettings>()->GetGroup(__uuidof(T)));
}