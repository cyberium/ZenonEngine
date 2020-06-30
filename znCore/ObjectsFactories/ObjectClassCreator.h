#pragma once

class ZN_API CObjectClassCreator
	: public IObjectClassCreator
{
public:
	CObjectClassCreator(IBaseManager& BaseManager);
	virtual ~CObjectClassCreator();

	// IObjectClassCreator
	virtual size_t GetSupportedClassCount() const override final;
	virtual ObjectClassType GetSupportedClassKey(size_t Index) const override final;
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) = 0;

protected:
	void AddKey(ObjectClassType ObjectClassKey);
	IBaseManager& GetBaseManager();

private:
	std::vector<ObjectClassType> m_Keys;
	IBaseManager& m_BaseManager;
};