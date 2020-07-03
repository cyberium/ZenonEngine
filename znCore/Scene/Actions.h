#pragma once

class ZN_API CAction
	: public IPropertyAction
{
public:
	CAction();
	CAction(std::string Name, std::string Description);
	virtual ~CAction();

	// IPropertyAction
	std::string GetName() const override;
	void SetName(const std::string& Name) override;
	std::string GetDescription() const override;
	void SetDescription(const std::string& Description) override;
	void SetAction(std::function<bool(void) > Action) override;
	void ExecuteAction() override;

private:
	std::string m_Name;
	std::string m_Description;
	std::function<bool(void)> m_Action;
};
