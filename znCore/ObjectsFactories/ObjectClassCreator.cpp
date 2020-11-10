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
	return std::get<1>(m_Keys.at(Index));
}

std::string CObjectClassCreatorBase::GetSupportedClassName(size_t Index) const
{
	_ASSERT(Index < GetSupportedClassCount());
	return std::get<0>(m_Keys.at(Index));
}

CreationFunction_t CObjectClassCreatorBase::GetSupportedClassFunction(size_t Index) const
{
	_ASSERT(Index < GetSupportedClassCount());
	return std::get<2>(m_Keys.at(Index));
}


//
// Protected
//
void CObjectClassCreatorBase::AddKey(std::string ObjectName, ObjectClass ObjectClassKey, CreationFunction_t Func)
{
	m_Keys.push_back(std::make_tuple(ObjectName, ObjectClassKey, Func));
}

IBaseManager & CObjectClassCreatorBase::GetBaseManager()
{
	return m_BaseManager;
}
