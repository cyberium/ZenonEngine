#pragma once

class ZN_API CObjectClassCreatorBase
	: public IObjectClassCreator
{
public:
	CObjectClassCreatorBase(IBaseManager& BaseManager);
	virtual ~CObjectClassCreatorBase();

	// IObjectClassCreator
	size_t GetSupportedClassCount() const override final;
	ObjectClass GetSupportedClassKey(size_t Index) const override final;
	std::string GetSupportedClassName(size_t Index) const override final;
	CreationFunction_t GetSupportedClassFunction(size_t Index) const override final;
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) = 0;

protected:
	void AddKey(std::string ObjectName, ObjectClass ObjectClassKey, CreationFunction_t Func);
	IBaseManager& GetBaseManager();

private:
	std::vector<std::tuple<std::string, ObjectClass, CreationFunction_t>> m_Keys;
	IBaseManager& m_BaseManager;
};