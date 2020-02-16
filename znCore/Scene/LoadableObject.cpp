#include "stdafx.h"

// General
#include "LoadableObject.h"

CLoadableObject::CLoadableObject(const ILoadable* Depends)
	: m_State(ELoadableState::Created)
	, m_Depends(Depends)
{
}

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


const ILoadable * CLoadableObject::getDepends() const
{
	return m_Depends;
}

uint32 CLoadableObject::getPriority() const
{
	return 0;
}
