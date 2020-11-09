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



//
// ILoadable
//
void CLoadableObject::SetState(ELoadableState State)
{
	m_State = State;
}

ILoadable::ELoadableState CLoadableObject::GetState() const
{
	return m_State;
}

void CLoadableObject::AddDependense(std::shared_ptr<ILoadable> Loadable)
{
	m_Depends = Loadable;
}

std::shared_ptr<ILoadable> CLoadableObject::GetDependense() const
{
	return m_Depends.lock();
}
