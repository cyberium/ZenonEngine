#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE IProperty;
template <typename T> ZN_INTERFACE ZN_API IPropertyT;
// Forward END


ZN_INTERFACE ZN_API ISetting
{
	virtual ~ISetting() {}

	virtual bool IsDefault() const = 0;
	virtual void ResetToDefault() = 0;
};


ZN_INTERFACE ZN_API ISettingGroup
{
	virtual ~ISettingGroup() {}

	virtual void AddDefaultSettings() = 0;

	virtual void AddSetting(std::shared_ptr<IProperty> Setting) = 0;
	virtual std::shared_ptr<IProperty> GetSetting(const std::string& SettingName) = 0;

	virtual bool IsGroupDefault() const = 0;
	virtual void ResetGroup() = 0;

	template<typename T>
	inline std::shared_ptr<IPropertyT<T>> GetSettingT(const std::string& SettingName)
	{
		return std::dynamic_pointer_cast<IPropertyT<T>>(GetSetting(SettingName));
	}
};



ZN_INTERFACE ZN_API
	__declspec(uuid("9FB1EEC3-16B7-4983-84C5-DDF47B203C6D"))
	ISettings : public IManager
{
	virtual ~ISettings() {}

	virtual void AddGroup(const std::string& GroupName, std::shared_ptr<ISettingGroup> Group) = 0;
	virtual std::shared_ptr<ISettingGroup> GetGroup(const std::string& GroupName) = 0;
};