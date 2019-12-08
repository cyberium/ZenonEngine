#pragma once

class OW_ENGINE_API CBaseManager : public IBaseManager
{
public:
	CBaseManager();
	virtual ~CBaseManager();

	// IBaseManager
	void RegisterManager(GUID _type, std::shared_ptr<IManager> _manager) override;
	void UnregisterManager(GUID _type) override;
	std::shared_ptr<IManager> GetManager(GUID _type) override;

private:
	std::unordered_map<GUID, std::shared_ptr<IManager>> m_Managers;
};
