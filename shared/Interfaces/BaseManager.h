#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IApplication;
// FORWARD END


ZN_INTERFACE ZN_API 
	__declspec(uuid("BDC2768B-055D-42EA-ABE3-CF17CD21178D")) 
	IManager
{
	virtual ~IManager() = 0 {};
};


ZN_INTERFACE ZN_API 
	__declspec(uuid("BB9FD479-C7AD-4F57-837B-E299A04AF171")) 
	IBaseManager
{
	virtual ~IBaseManager() {};

	virtual IManager* AddManager(GUID Type, const std::shared_ptr<IManager>& Manager) = 0;
	virtual void RemoveManager(GUID Type) = 0;
	virtual IManager* GetManager(GUID Type) const = 0;

	virtual const IApplication& GetApplication() const = 0;



	// Templates

	template<typename T>
	inline T* AddManager(const std::shared_ptr<IManager>& Manager)
	{
		return dynamic_cast<T*>(AddManager(__uuidof(T), Manager));
	}

	template<typename T>
	inline void RemoveManager()
	{
		RemoveManager(__uuidof(T));
	}

	template<typename T>
	inline T* GetManager() const
	{
		IManager* manager = GetManager(__uuidof(T));
		if (manager == nullptr)
			throw CException(L"Manager not found.");
		return dynamic_cast<T*>(manager);
	}
};


ZN_INTERFACE ZN_API IBaseManagerInternal
{
	virtual ~IBaseManagerInternal() {}

	virtual void SetApplicationInternal(const IApplication* Application) = 0;
};


ZN_INTERFACE ZN_API IBaseManagerHolder
{
	virtual ~IBaseManagerHolder() {}

	virtual IBaseManager& GetBaseManager() const = 0;
};