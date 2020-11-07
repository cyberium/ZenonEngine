#pragma once

class CPropertyBase
	: virtual public IProperty
{
public:
	CPropertyBase();
	CPropertyBase(std::string Name, std::string Description);
	virtual ~CPropertyBase();

	// IProperty
	std::string GetName() const override;
	void SetName(const std::string& Name) override;
	std::string GetDescription() const override;
	void SetDescription(const std::string& Description) override;
	void SetSyntetic(bool Value) override;
	bool IsSyntetic() const override;

private:
	std::string m_Name;
	std::string m_Description;
	bool        m_IsSyntetic;
};