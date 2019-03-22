#pragma once

// Forward BEGIN
struct IManager;
// Forward END

struct ILoadable
{
	virtual ~ILoadable() {}

	virtual bool PreLoad() = 0;

	virtual bool Load() = 0;
	virtual bool Delete() = 0;

	virtual void setLoadingBegin() = 0;
	virtual bool isLoadingBegin() const = 0;
	virtual void setLoaded() = 0;
	virtual bool isLoaded() const = 0;

	virtual uint32 getPriority() const = 0;
};

struct
	__declspec(uuid("18A045EF-D291-45E5-8F61-E223C4AFEF83"))
	ILoader : public IManager
{
	virtual void AddToLoadQueue(std::shared_ptr<ILoadable> _item) = 0;
	virtual void LoadAll() = 0;

	virtual void AddToDeleteQueue(std::shared_ptr<ILoadable> _item) = 0;
	virtual void DeleteAll() = 0;
};