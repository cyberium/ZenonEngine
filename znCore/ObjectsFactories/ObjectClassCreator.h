#pragma once

class ZN_API CObjectClassCreator
	: public IObjectClassCreator
{
public:
	CObjectClassCreator(IBaseManager& BaseManager);
	virtual ~CObjectClassCreator();

	// IObjectClassCreator
	virtual size_t GetSupportedClassCount() const override final;
	virtual ObjectClass GetSupportedClassKey(size_t Index) const override final;
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) = 0;

protected:
	void AddKey(ObjectClass ObjectClassKey);
	IBaseManager& GetBaseManager();

private:
	std::vector<ObjectClass> m_Keys;
	IBaseManager& m_BaseManager;
};