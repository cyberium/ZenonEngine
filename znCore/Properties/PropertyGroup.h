#pragma once

#include "PropertyBase.h"

class ZN_API CPropertiesGroup
	: public CPropertyBase
	, public IPropertiesGroup
{
public:
	CPropertiesGroup(std::string Name, std::string Description);
	virtual ~CPropertiesGroup();

	// IProperty
	void ResetToDefault() override;
	bool IsDefault() const override;

	void FromString(const std::string& String, bool BlockCallback) override;
	std::string ToString() const override;

	void CopyTo(const std::shared_ptr<IProperty>& Other) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// IPropertiesGroup
	void AddProperty(std::shared_ptr<IProperty> Property) override;
	void RemoveProperty(std::shared_ptr<IProperty> Property) override;
	void RemoveProperty(const std::string& PropertyName) override;
	std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) override;
	const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const override;

private:
	std::unordered_map<std::string, std::shared_ptr<IProperty>> m_Properties;
};
