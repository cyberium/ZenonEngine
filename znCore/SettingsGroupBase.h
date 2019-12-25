#pragma once

class ZN_API CSettingsGroupBase
	: public ISettingGroup
{
public:
	CSettingsGroupBase();
	virtual ~CSettingsGroupBase();
	

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