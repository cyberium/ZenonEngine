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
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;
	void SetAction(std::function<bool(void)> Action) override;
	void SetActionPrecondition(std::function<bool(void)> ActionPrecondition) override;

	bool ExecuteAction() override;
	bool ExecutePrecondition() override;

private:
	std::string m_Name;
	std::string m_Description;
	std::function<bool(void)> m_Action;
	std::function<bool(void)> m_ActionPrecondition;
};
