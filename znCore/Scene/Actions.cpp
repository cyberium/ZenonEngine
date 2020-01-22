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

void CAction::SetAction(std::function<bool(const std::shared_ptr<ISceneNode3D>&)> Action)
{
	m_Action = Action;
}

void CAction::ExecuteAction(const std::shared_ptr<ISceneNode3D>& ProcessedNode)
{
	_ASSERT(m_Action != nullptr);
	m_Action(ProcessedNode);
}



// ---------------------------------------------------------------------------------



CActionsGroup::CActionsGroup()
{}

CActionsGroup::CActionsGroup(std::string Name)
	: m_Name(Name)
{}

CActionsGroup::~CActionsGroup()
{}

//
// IActionsGroup
//
std::string CActionsGroup::GetName() const
{
	return m_Name;
}

void CActionsGroup::SetName(const std::string & Name)
{
	m_Name = Name;
}

void CActionsGroup::AddAction(std::shared_ptr<IAction> Action)
{
	_ASSERT(Action != nullptr);
	_ASSERT(m_Actions.find(Action->GetName()) == m_Actions.end());
	m_Actions.insert(std::make_pair(Action->GetName(), Action));
}

std::shared_ptr<IAction> CActionsGroup::GetAction(const std::string & ActionName)
{
	return m_Actions[ActionName];
}

const std::unordered_map<std::string, std::shared_ptr<IAction>>& CActionsGroup::GetActions() const
{
	return m_Actions;
}
