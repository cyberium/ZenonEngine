#pragma once

ZN_INTERFACE OW_ENGINE_API
	__declspec(novtable, uuid("BDC2768B-055D-42EA-ABE3-CF17CD21178D"))
	IManager
{
	virtual ~IManager() = 0 {};
};

//--

template<typename T>
ZN_INTERFACE OW_ENGINE_API IRefManager : public IManager
{	
	virtual ~IRefManager() {};

	virtual std::shared_ptr<T> Add(const std::string& name) = 0;
	virtual bool Exists(const std::string& name) const = 0;
	virtual void Delete(const std::string& name) = 0;
	virtual void Delete(std::shared_ptr<T> item) = 0;
};



// FORWARD BEGIN
class CFontMesh;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API
	__declspec(novtable, uuid("1427E242-CCB8-4AEC-ABC8-17DE58A96B05"))
	IFontsManager : public IRefManager<CFontMesh>
{
	virtual ~IFontsManager() {};

	virtual std::shared_ptr<CFontMesh> GetMainFont() const = 0;
	virtual std::shared_ptr<CFontMesh> Add(const std::string& _fontFileName, uint32 _fontSize) = 0;
};

//--

ZN_INTERFACE OW_ENGINE_API
	__declspec(novtable, uuid("BB9FD479-C7AD-4F57-837B-E299A04AF171"))
	IBaseManager
{
	virtual ~IBaseManager() {};

	virtual void RegisterManager(GUID _type, std::shared_ptr<IManager> _manager) = 0;
	virtual void UnregisterManager(GUID _type) = 0;

	virtual std::shared_ptr<IManager> GetManager(GUID _type) = 0;
};

ZN_INTERFACE OW_ENGINE_API IBaseManagerHolder
{
	virtual ~IBaseManagerHolder() {}

	virtual std::shared_ptr<IBaseManager> GetBaseManager() const = 0;
};