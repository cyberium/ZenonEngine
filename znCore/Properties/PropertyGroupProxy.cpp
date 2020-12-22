#include "stdafx.h"

// General
#include "PropertyGroupProxy.h"

CPropertyGroupProxy::CPropertyGroupProxy(std::shared_ptr<IPropertiesGroup> SourceProperty)
	: m_SourceProperty(SourceProperty)
{
}

CPropertyGroupProxy::~CPropertyGroupProxy()
{
}



//
// IProperty
//
std::string CPropertyGroupProxy::GetName() const
{
	return m_SourceProperty->GetName();
}

void CPropertyGroupProxy::SetName(const std::string& /*Name*/)
{
	throw CException("Proxy properties don't support 'SetName' operation.");
}

std::string CPropertyGroupProxy::GetDescription() const
{
	return m_SourceProperty->GetDescription();
}

void CPropertyGroupProxy::SetDescription(const std::string& /*Description*/)
{
	throw CException("Proxy properties don't support 'SetDescription' operation.");
}

void CPropertyGroupProxy::SetSyntetic(bool /*Value*/)
{
	throw CException("Proxy properties don't support 'SetSyntetic' operation.");
}

bool CPropertyGroupProxy::IsSyntetic() const
{
	return true;
}

void CPropertyGroupProxy::SetNonCopyable(bool /*Value*/)
{
	throw CException("Proxy properties don't support 'SetNonCopyable' operation.");
}

bool CPropertyGroupProxy::IsNonCopyable() const
{
	return true;
}

void CPropertyGroupProxy::ResetToDefault()
{
	throw CException("Proxy properties don't support 'ResetToDefault' operation.");
}

bool CPropertyGroupProxy::IsDefault() const
{
	return m_SourceProperty->IsDefault();
}

void CPropertyGroupProxy::FromString(const std::string& /*String*/, bool /*BlockCallback*/)
{
	throw CException("Proxy properties don't support 'FromString' operation.");
}

std::string CPropertyGroupProxy::ToString() const
{
	return m_SourceProperty->ToString();
}

void CPropertyGroupProxy::CopyTo(const std::shared_ptr<IProperty>& Other) const
{
	m_SourceProperty->CopyTo(Other);
}

void CPropertyGroupProxy::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	throw CException("Proxy properties don't support 'Load' operation.");
}

void CPropertyGroupProxy::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	throw CException("Proxy properties don't support 'Save' operation.");
}



//
// IPropertiesGroup
//
void CPropertyGroupProxy::AddProperty(std::shared_ptr<IProperty> Property)
{
	throw CException("Proxy properties don't support 'AddProperty' operation.");
}

void CPropertyGroupProxy::RemoveProperty(std::shared_ptr<IProperty> Property)
{
	throw CException("Proxy properties don't support 'RemoveProperty' operation.");
}

void CPropertyGroupProxy::RemoveProperty(const std::string& PropertyName)
{
	throw CException("Proxy properties don't support 'RemoveProperty' operation.");
}

std::shared_ptr<IProperty> CPropertyGroupProxy::GetProperty(const std::string & PropertyName)
{
	return m_SourceProperty->GetProperty(PropertyName);
}

const std::unordered_map<std::string, std::shared_ptr<IProperty>>& CPropertyGroupProxy::GetProperties() const
{
	return m_SourceProperty->GetProperties();
}