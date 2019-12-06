#pragma once

class OW_ENGINE_API CSettingsGroup
	: public ISettingGroup
{
public:
	CSettingsGroup();
	virtual ~CSettingsGroup();
	

	//
	// ISettingGroup
	//
	virtual void AddDefaultSettings() override;
	void AddSetting(const std::string& SettingName, std::shared_ptr<ISetting> Setting) override;
	std::shared_ptr<ISetting> GetSetting(const std::string& SettingName) override;
	bool IsGroupDefault() const override;
	void ResetGroup() override;

private:
	std::unordered_map<std::string, std::shared_ptr<ISetting>> m_Settings;
};