#include "stdafx.h"

// General
#include "Actions.h"

CAction::CAction()
	: CPropertyBase()
{}

CAction::CAction(std::string Name, std::string Description)
	: CPropertyBase(Name, Description)
{}

CAction::~CAction()
{}



//
// IProperty
//
void CAction::FromString(const std::string & String)
{
	throw CException("Incorrect behaviour");
}

std::string CAction::ToString() const
{
	throw CException("Incorrect behaviour");
}



//
// IPropertyAction
//
void CAction::SetAction(std::function<bool(void)> Action)
{
	m_Action = Action;
}

void CAction::SetActionPrecondition(std::function<bool(void)> ActionPrecondition)
{
	m_ActionPrecondition = ActionPrecondition;
}

bool CAction::ExecuteAction()
{
	if (m_Action == nullptr)
		throw CException("Functor for '%s' action in nullptr.", GetName().c_str());

	if (false == ExecutePrecondition())
		throw CException("Precondition for '%s' action is false.", GetName().c_str());

	return m_Action();
}

bool CAction::ExecutePrecondition()
{
	if (m_ActionPrecondition)
		return m_ActionPrecondition();
	return true;
}
