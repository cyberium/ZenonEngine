#pragma once

class ZN_API CObjectsFactory
	: public IObjectsFactory
{
public:
	CObjectsFactory(IBaseManager& BaseManager);
	virtual ~CObjectsFactory();

	// IObjectsFactory
	std::shared_ptr<IObjectClassFactory> GetClassFactory(ObjectType ObjectFactoryKey) const override;
	void AddClassFactory(std::shared_ptr<IObjectClassFactory> Creator) override;
	void RemoveClassFactory(std::shared_ptr<IObjectClassFactory> Creator) override;
	std::shared_ptr<IObject> CreateObject(ObjectType ObjectFactoryKey, ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs);




private:
	std::unordered_map<ObjectType, std::shared_ptr<IObjectClassFactory>> m_ClassFactories;
	IBaseManager& m_BaseManager;
};