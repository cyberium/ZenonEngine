#include "stdafx.h"

// General
#include "PropertyBase.h"

CPropertyBase::CPropertyBase()
	: m_Name("PropertyName")
	, m_Description("PropertyDescription")
	, m_IsSyntetic(false)
{}

CPropertyBase::CPropertyBase(std::string Name, std::string Description)
	: m_Name(Name)
	, m_Description(Description)
	, m_IsSyntetic(false)
{}

CPropertyBase::~CPropertyBase()
{}



//
// IProperty
//
std::string CPropertyBase::GetName() const
{
	return m_Name;
}

void CPropertyBase::SetName(const std::string& Name)
{
	bool isCorrent = std::all_of(Name.begin(), Name.end(), [](char c) { 
		return std::isalnum(c, std::locale()); 
	});
	if (false == isCorrent)
		throw CException("Property name '%s' is invalid.", Name.c_str());
	m_Name = Name;
}

std::string CPropertyBase::GetDescription() const
{
	return m_Description;
}

void CPropertyBase::SetDescription(const std::string& Description)
{
	m_Description = Description;
}

void CPropertyBase::SetSyntetic(bool Value)
{
	m_IsSyntetic = Value;
}

bool CPropertyBase::IsSyntetic() const
{
	return m_IsSyntetic;
}

void CPropertyBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	std::string propertyName = Reader->GetName();
	SetName(Reader->GetName());
}

void CPropertyBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	std::string propertyName = GetName();
	Writer->SetName(propertyName);
}
