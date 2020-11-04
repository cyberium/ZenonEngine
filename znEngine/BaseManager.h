#pragma once

class ZN_API CBaseManager 
	: public IBaseManager
	, public IBaseManagerInternal
{
public:
	CBaseManager();
	virtual ~CBaseManager();

	// IBaseManager
	IManager* AddManager(ObjectClass Type, const std::shared_ptr<IManager>& Manager) override;
	void RemoveManager(ObjectClass Type) override;
	IManager* GetManager(ObjectClass Type) const override;
	
	const IApplication& GetApplication() const override;

	// IBaseManagerInternal
	void SetApplicationInternal(const IApplication* Application) override;


private:
	struct SManagerInfo
	{
		SManagerInfo(const ObjectClass& Class, const std::shared_ptr<IManager>& Manager)
			: Class(Class)
			, Manager(Manager)
		{}

		ObjectClass Class;
		std::shared_ptr<IManager> Manager;
	};

private:
	std::vector<SManagerInfo> m_Managers;
	const IApplication* m_Application;
};
