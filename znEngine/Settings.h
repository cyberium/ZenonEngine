#pragma once

class ZN_API CSettings 
	: public ISettings
{
public:
	CSettings(IBaseManager* BaseManager);
	virtual ~CSettings();

	void AddGroup(const std::string& GroupName, std::shared_ptr<ISettingGroup> Group);
	std::shared_ptr<ISettingGroup> GetGroup(const std::string& GroupName);

private:
	IBaseManager* m_BaseManager;

	std::unordered_map<std::string, std::shared_ptr<ISettingGroup>> m_Groups;
};
