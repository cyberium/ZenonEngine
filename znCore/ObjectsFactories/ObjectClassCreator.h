#pragma once

class ZN_API CObjectClassCreatorBase
	: public IObjectClassCreator
{
public:
	CObjectClassCreatorBase(IBaseManager& BaseManager);
	virtual ~CObjectClassCreatorBase();

	// IObjectClassCreator
	virtual size_t GetSupportedClassCount() const override final;
	virtual ObjectClass GetSupportedClassKey(size_t Index) const override final;
	virtual std::string GetSupportedClassName(size_t Index) const override final;
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) = 0;

protected:
	void AddKey(std::string ObjectName, ObjectClass ObjectClassKey);
	IBaseManager& GetBaseManager();

private:
	std::vector<std::pair<std::string, ObjectClass>> m_Keys;
	IBaseManager& m_BaseManager;
};