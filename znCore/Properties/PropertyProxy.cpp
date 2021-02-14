#include "stdafx.h"

// General
#include "PropertyProxy.h"

CPropertyWrapper::CPropertyWrapper(std::shared_ptr<IProperty> SourceProperty)
	: m_SourceProperty(SourceProperty)
{
}

CPropertyWrapper::~CPropertyWrapper()
{
}

std::string CPropertyWrapper::GetName() const
{
	return m_SourceProperty->GetName();
}

void CPropertyWrapper::SetName(const std::string & Name)
{
	m_SourceProperty->SetName(Name);
}

std::string CPropertyWrapper::GetDescription() const
{
	return m_SourceProperty->GetDescription();
}

void CPropertyWrapper::SetDescription(const std::string & Description)
{
	m_SourceProperty->SetDescription(Description);
}

void CPropertyWrapper::SetSyntetic(bool Value)
{
	m_SourceProperty->SetSyntetic(Value);
}

bool CPropertyWrapper::IsSyntetic() const
{
	return m_SourceProperty->IsSyntetic();
}

void CPropertyWrapper::SetNonCopyable(bool Value)
{
	m_SourceProperty->SetNonCopyable(Value);
}

bool CPropertyWrapper::IsNonCopyable() const
{
	return m_SourceProperty->IsNonCopyable();
}

void CPropertyWrapper::SetReadOnly(bool Value)
{
	m_SourceProperty->SetReadOnly(Value);
}

bool CPropertyWrapper::IsReadOnly() const
{
	return m_SourceProperty->IsReadOnly();
}

void CPropertyWrapper::ResetToDefault()
{
	m_SourceProperty->ResetToDefault();
}

bool CPropertyWrapper::IsDefault() const
{
	return m_SourceProperty->IsDefault();
}

void CPropertyWrapper::FromString(const std::string & String, bool BlockCallback)
{
	m_SourceProperty->FromString(String, BlockCallback);
}

std::string CPropertyWrapper::ToString() const
{
	return m_SourceProperty->ToString();
}

void CPropertyWrapper::CopyTo(const std::shared_ptr<IProperty>& Other) const
{
	m_SourceProperty->CopyTo(Other);
}

void CPropertyWrapper::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_SourceProperty->Load(Reader);
}

void CPropertyWrapper::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	m_SourceProperty->Save(Writer);
}
