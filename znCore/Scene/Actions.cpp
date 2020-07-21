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

void CAction::ExecuteAction()
{
	_ASSERT(m_Action != nullptr);
	m_Action();
}
