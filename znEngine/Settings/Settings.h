#pragma once

#include "GroupQuality.h"
#include "GroupVideo.h"

class OW_ENGINE_API CSettings : public ISettings
{
public:
	CSettings(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~CSettings();

	void AddDefaults();

	// ISettings
	void RegisterBool(const std::string& _name, bool _defaultValue) override;
	bool& GetBool(const std::string& _name) override;
	void RegisterUint32(const std::string& _name, uint32 _defaultValue) override;
	uint32& GetUint32(const std::string& _name) override;
	void RegisterString(const std::string& _name, std::string _defaultValue) override;
	std::string& GetString(const std::string& _name) override;

	void RegisterGroup(GUID _guid, ISettingGroup* _group) override;
	ISettingGroup* GetGroup(GUID _guid) override;

private:
	std::shared_ptr<IBaseManager> m_BaseManager;

	std::map<std::string, bool> m_BoolSettings;
	std::map<std::string, uint32> m_Uint32Settings;
	std::map<std::string, std::string> m_StringSettings;
	std::map<GUID, ISettingGroup*> m_Groups;
};

template<class T>
static inline void AddSettingsGroup(std::shared_ptr<IBaseManager> BaseManager, ISettingGroup* _settingsGroup)
{
	GetManager<ISettings>(BaseManager)->RegisterGroup(__uuidof(T), _settingsGroup);
}

template<class T>
static inline const T* GetSettingsGroup(std::shared_ptr<IBaseManager> BaseManager)
{
	return (const T*)(GetManager<ISettings>(BaseManager)->GetGroup(__uuidof(T)));
}