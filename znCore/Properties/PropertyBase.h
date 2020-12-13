#pragma once

class ZN_API CPropertyBase
	: virtual public IProperty
{
public:
	CPropertyBase(std::string Name, std::string Description);
	virtual ~CPropertyBase();

	// IProperty
	std::string GetName() const override;
	void SetName(const std::string& Name) override;

	std::string GetDescription() const override;
	void SetDescription(const std::string& Description) override;

	void SetSyntetic(bool Value) override;
	bool IsSyntetic() const override;

	void SetNonCopyable(bool Value) override;
	bool IsNonCopyable() const override;

	void CopyTo(const std::shared_ptr<IProperty>& Other) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	std::string                GetPropertyTypeName(const IProperty* Property) const;
	std::shared_ptr<IProperty> CreateNewPropety(std::string PropertyName, std::string TypeName);

private:
	std::string m_Name;
	std::string m_Description;
	bool        m_IsSyntetic;
	bool        m_IsNonCopyable;
};