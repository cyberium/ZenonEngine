#pragma once

class CPropertyGroupProxy
	: virtual public IProperty
	, public IPropertiesGroup
{
public:
	CPropertyGroupProxy(std::shared_ptr<IPropertiesGroup> SourceProperty);
	virtual ~CPropertyGroupProxy();

	// IProperty
	std::string GetName() const override;
	void SetName(const std::string& Name) override;
	std::string GetDescription() const override;
	void SetDescription(const std::string& Description) override;
	void SetSyntetic(bool Value) override;
	bool IsSyntetic() const override;
	void SetNonCopyable(bool Value) override;
	bool IsNonCopyable() const override;
	void ResetToDefault() override;
	bool IsDefault() const override;
	void FromString(const std::string& String, bool BlockCallback = false) override;
	std::string ToString() const override;
	void CopyTo(const std::shared_ptr<IProperty>& Other) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// IPropertiesGroup
	void AddProperty(std::shared_ptr<IProperty> Property) override;
	std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) override;
	const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const override;

private:
	std::shared_ptr<IPropertiesGroup> m_SourceProperty;
};