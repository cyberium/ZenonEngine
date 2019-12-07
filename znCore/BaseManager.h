#pragma once

inline bool operator<(const GUID& lhs, const GUID& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(GUID)) < 0;
}

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
	std::map<GUID, std::shared_ptr<IManager>> m_Managers;
};

// Helpers

template<class T>
static inline void AddManager(IBaseManager* BaseManager, std::shared_ptr<IManager> _manager)
{
	BaseManager->RegisterManager(__uuidof(T), _manager);
}

template<class T>
static inline void DelManager(IBaseManager* BaseManager)
{
	//BaseManager->UnregisterManager(__uuidof(T));
}

template<class T>
static inline std::shared_ptr<T> GetManager(IBaseManager* BaseManager)
{
	std::shared_ptr<IManager> manager = BaseManager->GetManager(__uuidof(T));
	if (manager == nullptr)
	{
		return nullptr;
		//throw std::exception("Manager not found!");
	}

	return std::dynamic_pointer_cast<T, IManager>(manager);
}