#include "stdafx.h"

// General
#include "ObjectClassCreator.h"

CObjectClassCreator::CObjectClassCreator(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CObjectClassCreator::~CObjectClassCreator()
{
}

//
// IObjectClassCreator
//
size_t CObjectClassCreator::GetSupportedClassCount() const
{
	return m_Keys.size();
}

ObjectClass CObjectClassCreator::GetSupportedClassKey(size_t Index) const
{
	_ASSERT(Index < GetSupportedClassCount());
	return m_Keys.at(Index);
}


//
// Protected
//
void CObjectClassCreator::AddKey(ObjectClass ObjectClassKey)
{
	m_Keys.push_back(ObjectClassKey);
}

IBaseManager & CObjectClassCreator::GetBaseManager()
{
	return m_BaseManager;
}
