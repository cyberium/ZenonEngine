#pragma once

class ZN_API CObjectClassCreatorBase
	: public IObjectClassCreator
{
public:
	CObjectClassCreatorBase(IBaseManager& BaseManager);
	virtual ~CObjectClassCreatorBase();

	// IObjectClassCreator
	bool IsCanCreate(ObjectClass ObjectClassKey) const override;
	const std::map<ObjectClass, std::pair<std::string, CreationFunction_t>>& GetSupportedClasses() const override;
	void AddClass(ObjectClass ObjectClassKey, std::string ObjectClassName, CreationFunction_t Func) override final;
	void RemoveClass(ObjectClass ObjectClassKey) override final;

protected:
	const std::string& GetObjectClassName(ObjectClass ObjectClass) const;
	const CreationFunction_t& GetObjectClassFunctorCretor(ObjectClass ObjectClass) const;
	IBaseManager& GetBaseManager();

private:
	std::map<ObjectClass, std::pair<std::string, CreationFunction_t>> m_Keys;
	IBaseManager& m_BaseManager;
};