#pragma once

class ZN_API CLoadableObject
	: public ILoadable
{
public:
	CLoadableObject();
	virtual ~CLoadableObject();

	virtual bool									PreLoad();

	virtual bool									Load();
	virtual bool									Delete();

	virtual void									setLoadingBegin();
	virtual bool									isLoadingBegin() const;
	virtual void									setLoaded();
	virtual bool									isLoaded() const;

	virtual uint32									getPriority() const;

private:
	std::atomic<bool>                               m_IsLoadingBegin;
	std::atomic<bool>                               m_IsLoaded;
};