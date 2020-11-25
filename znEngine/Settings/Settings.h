#pragma once

class ZN_API CSettings 
	: public ISettings
{
public:
	CSettings(IBaseManager& BaseManager);
	virtual ~CSettings();

	// ISettings
	void AddGroup(std::shared_ptr<IPropertiesGroup> Group);
	std::shared_ptr<IPropertiesGroup> GetGroup(const std::string& GroupName);
	void Load() override;
	void Save() override;

private:
	std::shared_ptr<IPropertiesGroup> m_RootGroup;

private:
	IBaseManager& m_BaseManager;
};
