#include "stdafx.h"

// General
#include "LoadableObject.h"

CLoadableObject::CLoadableObject(std::weak_ptr<ILoadable> Depends)
	: m_State(ELoadableState::Created)
	, m_Depends(Depends)
{}

CLoadableObject::~CLoadableObject()
{
}

bool CLoadableObject::Load()
{
	return false;
}

bool CLoadableObject::Delete()
{
	return false;
}

void CLoadableObject::SetState(ELoadableState State)
{
	m_State = State;
}

ILoadable::ELoadableState CLoadableObject::GetState() const
{
	return m_State;
}

void CLoadableObject::AddDependense(const std::weak_ptr<ILoadable>& Loadable)
{
	_ASSERT(m_Depends.lock() == nullptr);
	m_Depends = Loadable;
}

std::weak_ptr<ILoadable> CLoadableObject::GetDependense() const
{
	return m_Depends;
}
