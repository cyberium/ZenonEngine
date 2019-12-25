#pragma once

class ZN_API CBaseManager : public IBaseManager
{
public:
	CBaseManager();
	virtual ~CBaseManager();

	// IBaseManager
	void AddManager(GUID Type, std::shared_ptr<IManager> Manager) override;
	void RemoveManager(GUID Type) override;
	std::shared_ptr<IManager> GetManager(GUID Type) const override;

private:
	std::unordered_map<GUID, std::shared_ptr<IManager>> m_Managers;
};
