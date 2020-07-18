#pragma once

class ZN_API CObjectClassFactory
	: public IObjectClassFactory
{
public:
	CObjectClassFactory(IBaseManager& BaseManager, ObjectType Type);
	virtual ~CObjectClassFactory();
	
	// IObjectClassFactory
	std::shared_ptr<IObjectClassCreator> GetClassCreator(ObjectClass ObjectClassKey) const override final;
	void AddClassCreator(std::shared_ptr<IObjectClassCreator> Creator) override final;
	void RemoveClassCreator(std::shared_ptr<IObjectClassCreator> Creator) override final;
	virtual ObjectType GetType() const override;
	Guid GenerateGuid(ObjectClass ObjectClassKey) override final;

	virtual std::shared_ptr<IObject> CreateObject(ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs) override;

protected:
	Guid ReadGUIDXML(const std::shared_ptr<IXMLReader>& Reader);
	std::shared_ptr<IXMLWriter> WriteGUIDXML(Guid Guid);
	Guid ReadGUID(const std::shared_ptr<IByteBuffer>& Bytes);
	std::shared_ptr<IByteBuffer> WriteGUID(Guid Guid);
	IBaseManager& GetBaseManager() const;

private:
	ObjectType m_Type;
	ObjectCounterType m_Counter;
	std::unordered_map<ObjectClass, std::pair<size_t, std::shared_ptr<IObjectClassCreator>>> m_ClassCreators;
	IBaseManager& m_BaseManager;
};