#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE IProperty;
ZN_INTERFACE IPropertiesGroup;
// Forward END

ZN_INTERFACE ZN_API	ISettings 
	: public IManager
{
	ZN_OBJECTCLASS(cSettingsManager);
	virtual ~ISettings() {}

	virtual void AddGroup(std::shared_ptr<IPropertiesGroup> Group) = 0;
	virtual std::shared_ptr<IPropertiesGroup> GetGroup(const std::string& GroupName) = 0;
	virtual void Load() = 0;
	virtual void Save() = 0;

	template <typename T>
	inline T GetSetting(const std::string& GroupName, const std::string& SettingName)
	{
		return GetBaseManager().GetManager<ISettings>()->GetGroup(GroupName)->GetPropertyT<T>(SettingName)->Get();
	}
};