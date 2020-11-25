#include "stdafx.h"

// General
#include "PropertyAction.h"

CAction::CAction(std::string Name, std::string Description)
	: CPropertyBase(Name, Description)
{
	SetSyntetic(true);
}

CAction::~CAction()
{}



//
// IProperty
//
void CAction::ResetToDefault()
{
	throw CException("TODO: CAction ResetToDefault.");
}

bool CAction::IsDefault() const
{
	throw CException("TODO: CAction IsDefault.");
}

void CAction::FromString(const std::string& String, bool BlockCallback)
{
	throw CException("TODO: CAction FromString.");
}

std::string CAction::ToString() const
{
	throw CException("TODO: CAction ToString.");
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
