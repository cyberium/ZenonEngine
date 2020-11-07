#pragma once

#include "PropertyBase.h"

class ZN_API CAction
	: public CPropertyBase
	, public IPropertyAction
{
public:
	CAction();
	CAction(std::string Name, std::string Description = "");
	virtual ~CAction();

	// IProperty
	void FromString(const std::string& String) override;
	std::string ToString() const override;
	
	// IPropertyAction
	void SetAction(std::function<bool(void)> Action) override;
	void SetActionPrecondition(std::function<bool(void)> ActionPrecondition) override;
	bool ExecuteAction() override;
	bool ExecutePrecondition() override;

private:
	std::function<bool(void)> m_Action;
	std::function<bool(void)> m_ActionPrecondition;
};
