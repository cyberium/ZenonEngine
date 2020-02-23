#pragma once

class ZN_API CLoadableObject
	: public ILoadable
{
public:
	CLoadableObject(std::weak_ptr<ILoadable> Depends = std::weak_ptr<ILoadable>());
	virtual ~CLoadableObject();

	virtual bool									Load();
	virtual bool									Delete();

	virtual void                                    SetState(ELoadableState State) override;
	virtual ELoadableState                          GetState() const override;

	void                                            AddDependense(const std::weak_ptr<ILoadable>& Loadable) override final;
	std::weak_ptr<ILoadable>                        GetDependense() const override final;

private:
	std::atomic<ELoadableState>                     m_State;
	std::weak_ptr<ILoadable>                        m_Depends;
};