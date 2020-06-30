#pragma once

class ZN_API CObjectClassFactory
	: public IObjectClassFactory
{
public:
	CObjectClassFactory(IBaseManager& BaseManager, ObjectFactoryType Type);
	virtual ~CObjectClassFactory();
	
	// IObjectClassFactory
	std::shared_ptr<IObjectClassCreator> GetClassCreator(ObjectClassType ObjectClassKey) const override final;
	void AddClassCreator(std::shared_ptr<IObjectClassCreator> Creator) override final;
	void RemoveClassCreator(std::shared_ptr<IObjectClassCreator> Creator) override final;
	virtual ObjectFactoryType GetType() const override;
	Object::Guid GenerateGuid(ObjectClassType ObjectClassKey) override final;

	virtual std::shared_ptr<IObject> CreateObject(ObjectClassType ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs) override;
	virtual std::shared_ptr<IObject> LoadObject(ObjectClassType ObjectClassKey, std::shared_ptr<IByteBuffer> Bytes) override;
	virtual std::shared_ptr<IByteBuffer> SaveObject(std::shared_ptr<IObject> Object) override;

private:
	ObjectFactoryType m_Type;
	ObjectCounterType m_Counter;
	std::unordered_map<ObjectClassType, std::pair<size_t, std::shared_ptr<IObjectClassCreator>>> m_ClassCreators;
	IBaseManager& m_BaseManager;
};