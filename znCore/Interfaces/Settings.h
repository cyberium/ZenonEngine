#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
// Forward END


ZN_INTERFACE OW_ENGINE_API ISetting
{
	virtual ~ISetting() {}

	virtual const std::string& GetName() const = 0;
	virtual bool IsDefault() const = 0;

	virtual void OnUpdate() = 0;
};
template<typename T>
ZN_INTERFACE ISettingT : public ISetting
{
	virtual ~ISettingT() {}

	virtual void Set(T Value) = 0;
	virtual T Get() const = 0;
};



ZN_INTERFACE OW_ENGINE_API ISettingInternal
{
	virtual ~ISettingInternal() {}

	virtual void SetName(const std::string& Name) = 0;
	virtual void ResetToDefault() = 0;
};
template<typename T>
ZN_INTERFACE ISettingInternalT : public ISettingInternal
{
	virtual ~ISettingInternalT() {}

	virtual void SetDefaultValue(T Value) = 0;
};



ZN_INTERFACE OW_ENGINE_API ISettingGroup
{
	virtual ~ISettingGroup() {}

	virtual void AddDefaultSettings() = 0;

	virtual void AddSetting(const std::string& SettingName, std::shared_ptr<ISetting> Setting) = 0;
	virtual std::shared_ptr<ISetting> GetSetting(const std::string& SettingName) = 0;

	virtual bool IsGroupDefault() const = 0;
	virtual void ResetGroup() = 0;

	template<typename T>
	inline std::shared_ptr<ISettingT<T>> GetSettingT(const std::string& SettingName)
	{
		return std::dynamic_pointer_cast<ISettingT<T>>(GetSetting(SettingName));
	}
};



ZN_INTERFACE OW_ENGINE_API
	__declspec(uuid("9FB1EEC3-16B7-4983-84C5-DDF47B203C6D"))
	ISettings : public IManager
{
	virtual ~ISettings() {}

	virtual void AddGroup(const std::string& GroupName, std::shared_ptr<ISettingGroup> Group) = 0;
	virtual std::shared_ptr<ISettingGroup> GetGroup(const std::string& GroupName) = 0;
};