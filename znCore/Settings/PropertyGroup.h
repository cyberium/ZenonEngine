#pragma once

#include "PropertyBase.h"

class CPropertiesGroup
	: public CPropertyBase
	, public IPropertiesGroup
{
public:
	CPropertiesGroup();
	CPropertiesGroup(std::string Name, std::string Description);
	virtual ~CPropertiesGroup();

	// IProperty
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// IPropertiesGroup
	void AddProperty(std::shared_ptr<IProperty> Property) override;
	std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) override;
	const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const override;

private:
	std::string                GetPropertyTypeName(const IProperty* Property) const;
	std::shared_ptr<IProperty> CreatePropetyByType(std::string TypeName);

private:
	std::unordered_map<std::string, std::shared_ptr<IProperty>> m_Properties;
};
