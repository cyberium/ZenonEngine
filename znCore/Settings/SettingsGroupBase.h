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
	void AddSetting(std::shared_ptr<IProperty> Setting) override;
	std::shared_ptr<IProperty> GetSetting(const std::string& SettingName) override;
	bool IsGroupDefault() const override;
	void ResetGroup() override;

private:
	std::unordered_map<std::string, std::shared_ptr<IProperty>> m_Settings;
};