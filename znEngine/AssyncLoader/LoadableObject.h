#pragma once

class ZN_API CLoadableObject
	: public ILoadable
{
public:
	CLoadableObject(std::weak_ptr<ILoadable> Depends = std::weak_ptr<ILoadable>());
	virtual ~CLoadableObject();

	// ILoadable
	virtual bool									Load() = 0;
	virtual bool									Delete() = 0;
	void                                            SetState(ELoadableState State) override final;
	ELoadableState                                  GetState() const override final;
	void                                            AddDependense(std::shared_ptr<ILoadable> Loadable) override final;
	std::shared_ptr<ILoadable>                      GetDependense() const override final;

private:
	std::atomic<ELoadableState>                     m_State;
	std::weak_ptr<ILoadable>                        m_Depends;
};