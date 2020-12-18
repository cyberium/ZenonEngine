#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IApplication;
// FORWARD END


ZN_INTERFACE ZN_API IManager
{
	ZN_OBJECTCLASS(cManager)

	virtual ~IManager() = 0 {};
};


ZN_INTERFACE ZN_API IBaseManager
{
	virtual ~IBaseManager() {};

	virtual IManager* AddManager(ObjectClass Type, const std::shared_ptr<IManager>& Manager) = 0;
	virtual void RemoveManager(ObjectClass Type) = 0;
	virtual IManager* GetManager(ObjectClass Type) const = 0;

	virtual const IApplication& GetApplication() const = 0;



	// Templates

	template<typename T>
	inline T* AddManager(const std::shared_ptr<IManager>& Manager)
	{
		return dynamic_cast<T*>(AddManager(T::GetClassT(), Manager));
	}

	template<typename T>
	inline void RemoveManager()
	{
		RemoveManager(T::GetClassT());
	}

	template<typename T>
	inline T* GetManager() const
	{
		IManager* manager = GetManager(T::GetClassT());
		if (manager == nullptr)
			throw CException("Manager not found.");
		return dynamic_cast<T*>(manager);
	}
};


ZN_INTERFACE ZN_API IBaseManagerInternal
{
	virtual ~IBaseManagerInternal() {}

	virtual void SetApplicationInternal(const IApplication* Application) = 0;
};
