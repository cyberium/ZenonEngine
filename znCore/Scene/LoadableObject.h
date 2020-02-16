#pragma once

class ZN_API CLoadableObject
	: public ILoadable
{
public:
	CLoadableObject(const ILoadable* Depends = nullptr);
	virtual ~CLoadableObject();

	virtual bool									Load();
	virtual bool									Delete();

	virtual void                                    SetState(ELoadableState State) override;
	virtual ELoadableState                          GetState() const override;

	const ILoadable*                                getDepends() const;
	virtual uint32									getPriority() const;

private:
	std::atomic<ELoadableState>                     m_State;

	const ILoadable*                                m_Depends;
};