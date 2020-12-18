#include "stdafx.h"

// General
#include "ObjectProxy.h"

CObjectProxy::CObjectProxy(std::shared_ptr<IObject> Object)
	: m_Object(Object)
{
}

CObjectProxy::~CObjectProxy()
{
}



//
// IObject
//
Guid CObjectProxy::GetGUID() const
{
	return m_Object->GetGUID();
}

std::string CObjectProxy::GetObjectTypeName() const
{
	return m_Object->GetObjectTypeName();
}

std::string CObjectProxy::GetObjectClassName() const
{
	return m_Object->GetObjectClassName();
}

std::string CObjectProxy::GetName() const
{
	return m_Object->GetName();
}

void CObjectProxy::SetName(const std::string& Name)
{
	m_Object->SetName(Name);
}

const IBaseManager & CObjectProxy::GetBaseManager() const
{
	return m_Object->GetBaseManager();
}

std::shared_ptr<IPropertiesGroup> CObjectProxy::GetProperties() const
{
	return m_Object->GetProperties();
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CObjectProxy::Copy() const
{
	return m_Object->Copy();
}

void CObjectProxy::CopyTo(std::shared_ptr<IObject> Destination) const
{
	m_Object->CopyTo(Destination);
}

void CObjectProxy::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	m_Object->Load(Buffer);
}

void CObjectProxy::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	m_Object->Save(Buffer);
}

void CObjectProxy::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_Object->Load(Reader);
}

void CObjectProxy::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	m_Object->Save(Writer);
}
