#include "stdafx.h"

// General
#include "ObjectClassCreator.h"

CObjectClassCreatorBase::CObjectClassCreatorBase(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CObjectClassCreatorBase::~CObjectClassCreatorBase()
{
}

//
// IObjectClassCreator
//
size_t CObjectClassCreatorBase::GetSupportedClassCount() const
{
	return m_Keys.size();
}

ObjectClass CObjectClassCreatorBase::GetSupportedClassKey(size_t Index) const
{
	_ASSERT(Index < GetSupportedClassCount());
	return m_Keys.at(Index).second;
}

std::string CObjectClassCreatorBase::GetSupportedClassName(size_t Index) const
{
	_ASSERT(Index < GetSupportedClassCount());
	return m_Keys.at(Index).first;
}


//
// Protected
//
void CObjectClassCreatorBase::AddKey(std::string ObjectName, ObjectClass ObjectClassKey)
{
	m_Keys.push_back(std::make_pair(ObjectName, ObjectClassKey));
}

IBaseManager & CObjectClassCreatorBase::GetBaseManager()
{
	return m_BaseManager;
}
