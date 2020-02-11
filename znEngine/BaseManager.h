#pragma once

class ZN_API CBaseManager 
	: public IBaseManager
	, public IBaseManagerInternal
{
public:
	CBaseManager();
	virtual ~CBaseManager();

	// IBaseManager
	IManager* AddManager(GUID Type, const std::shared_ptr<IManager>& Manager) override;
	void RemoveManager(GUID Type) override;
	IManager* GetManager(GUID Type) const override;
	const IApplication& GetApplication() const override;

	// IBaseManagerInternal
	void SetApplicationInternal(const IApplication* Application) override;


private:
	struct SManagerInfo
	{
		SManagerInfo(const GUID& Key, const std::shared_ptr<IManager>& Manager)
			: Key(Key)
			, Manager(Manager)
		{}

		GUID Key;
		std::shared_ptr<IManager> Manager;
	};

private:
	std::vector<SManagerInfo> m_Managers;
	const IApplication* m_Application;
};
