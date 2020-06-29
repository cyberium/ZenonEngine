#pragma once

class ZN_API CObjectsFactory
	: public IObjectsFactory
{
public:
	CObjectsFactory(IBaseManager& BaseManager);
	virtual ~CObjectsFactory();

	// IObjectsFactory
	std::shared_ptr<IObjectClassFactory> GetClassFactory(ObjectFactoryType ObjectFactoryKey) const override;
	void AddClassFactory(std::shared_ptr<IObjectClassFactory> Creator) override;
	void RemoveClassFactory(std::shared_ptr<IObjectClassFactory> Creator) override;
	std::shared_ptr<IObject> CreateObject(ObjectFactoryType ObjectFactoryKey, ObjectClassType ObjectClassKey);

private:
	std::unordered_map<ObjectFactoryType, std::shared_ptr<IObjectClassFactory>> m_ClassFactories;
};