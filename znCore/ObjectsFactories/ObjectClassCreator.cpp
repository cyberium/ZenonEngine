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
bool CObjectClassCreatorBase::IsCanCreate(ObjectClass ObjectClassKey) const
{
	return m_Keys.find(ObjectClassKey) != m_Keys.end();
}

const std::map<ObjectClass, std::pair<std::string, CreationFunction_t>>& CObjectClassCreatorBase::GetSupportedClasses() const
{
	return m_Keys;
}

void CObjectClassCreatorBase::AddClass(ObjectClass ObjectClassKey, std::string ObjectClassName, CreationFunction_t Func)
{
	const auto& it = m_Keys.find(ObjectClassKey);
	if (it != m_Keys.end())
		throw CException("ObjectClass '%d' already exists in ObjectClassCreator.");
	m_Keys.insert(std::make_pair(ObjectClassKey, std::make_pair(ObjectClassName, Func)));
}

void CObjectClassCreatorBase::RemoveClass(ObjectClass ObjectClassKey)
{
	const auto& it = m_Keys.find(ObjectClassKey);
	if (it == m_Keys.end())
		throw CException("ObjectClass '%d' not found in ObjectClassCreator.");
	m_Keys.erase(it);
}



//
// Protected
//
const std::string & CObjectClassCreatorBase::GetObjectClassName(ObjectClass ObjectClass) const
{
	const auto& it = m_Keys.find(ObjectClass);
	if (it == m_Keys.end())
		throw CException("ObjectClass '%d' not found in ObjectClassCreator.");
	return it->second.first;
}

const CreationFunction_t & CObjectClassCreatorBase::GetObjectClassFunctorCretor(ObjectClass ObjectClass) const
{
	const auto& it = m_Keys.find(ObjectClass);
	if (it == m_Keys.end())
		throw CException("ObjectClass '%d' not found in ObjectClassCreator.");
	return it->second.second;
}

IBaseManager & CObjectClassCreatorBase::GetBaseManager()
{
	return m_BaseManager;
}
