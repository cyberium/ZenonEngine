#include "stdafx.h"

// General
#include "Actions.h"

CAction::CAction()
	: m_Name("CActionName")
	, m_Description("CActionDesctiption")
{
}

CAction::CAction(std::string Name, std::string Description)
	: m_Name(Name)
	, m_Description(Description)
{
}

CAction::~CAction()
{
}



//
// IAction
//
std::string CAction::GetName() const
{
	return m_Name;
}

void CAction::SetName(const std::string& Name)
{
	m_Name = Name;
}

std::string CAction::GetDescription() const
{
	return m_Description;
}

void CAction::SetDescription(const std::string & Description)
{
	m_Description = Description;
}

void CAction::SetSyntetic(bool Value)
{
	Log::Warn("Actions in always syntetic properties.");
}

bool CAction::IsSyntetic() const
{
	return true;
}

void CAction::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	SetName(Reader->GetName());
	//SetDescription(Reader->GetStrAttribute("Description"));
}

void CAction::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetName(GetName());
	//Writer->SetStrAttribute(GetDescription(), "Description");
}

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
		throw CException("Action functor for '%s' action in nullptr.", GetName().c_str());

	if (false == ExecutePrecondition())
		throw CException("Action precondition for '%s' action is false.", GetName().c_str());

	return m_Action();
}

bool CAction::ExecutePrecondition()
{
	if (m_ActionPrecondition)
		return m_ActionPrecondition();
	return true;
}
