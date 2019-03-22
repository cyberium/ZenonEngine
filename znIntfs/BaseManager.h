#pragma once

struct
	__declspec(novtable, uuid("BDC2768B-055D-42EA-ABE3-CF17CD21178D"))
	IManager
{
	virtual ~IManager() = 0 {};
};

//--

template<class T>
struct IRefManager : public IManager
{	
	virtual ~IRefManager() {};

	virtual std::shared_ptr<T> Add(cstring name) = 0;
	virtual bool Exists(cstring name) const = 0;
	virtual void Delete(cstring name) = 0;
	virtual void Delete(std::shared_ptr<T> item) = 0;
};



// FORWARD BEGIN
class CFontMesh;
// FORWARD END

struct
	__declspec(novtable, uuid("1427E242-CCB8-4AEC-ABC8-17DE58A96B05"))
	IFontsManager : public IRefManager<CFontMesh>
{
	virtual ~IFontsManager() {};

	virtual std::shared_ptr<CFontMesh> GetMainFont() const = 0;
	virtual std::shared_ptr<CFontMesh> Add(cstring _fontFileName, uint32 _fontSize) = 0;
};

//--

struct 
	__declspec(novtable, uuid("BB9FD479-C7AD-4F57-837B-E299A04AF171"))
	IBaseManager
{
	virtual ~IBaseManager() {};

	virtual void RegisterManager(GUID _type, std::shared_ptr<IManager> _manager) = 0;
	virtual void UnregisterManager(GUID _type) = 0;

	virtual std::shared_ptr<IManager> GetManager(GUID _type) = 0;
};
