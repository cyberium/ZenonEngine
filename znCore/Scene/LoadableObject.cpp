#include "stdafx.h"

// General
#include "LoadableObject.h"

CLoadableObject::CLoadableObject()
	: m_IsLoadingBegin(false)
	, m_IsLoaded(false)
{
}

CLoadableObject::~CLoadableObject()
{
}

bool CLoadableObject::PreLoad()
{
	return false;
}

bool CLoadableObject::Load()
{
	return false;
}

bool CLoadableObject::Delete()
{
	return false;
}

void CLoadableObject::setLoadingBegin()
{
	m_IsLoadingBegin = true;
}

bool CLoadableObject::isLoadingBegin() const
{
	return m_IsLoadingBegin;
}

void CLoadableObject::setLoaded()
{
	m_IsLoaded = true;
}

bool CLoadableObject::isLoaded() const
{
	return m_IsLoaded;
}

uint32 CLoadableObject::getPriority() const
{
	return 0;
}
