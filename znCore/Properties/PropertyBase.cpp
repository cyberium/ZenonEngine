#include "stdafx.h"

// General
#include "PropertyBase.h"

CPropertyBase::CPropertyBase(std::string Name, std::string Description)
	: m_Name(Name)
	, m_Description(Description)
	, m_IsSyntetic(false)
	, m_IsNonCopyable(false)
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

void CPropertyBase::SetNonCopyable(bool Value)
{
	m_IsNonCopyable = Value;
}

bool CPropertyBase::IsNonCopyable() const
{
	return m_IsNonCopyable;
}

void CPropertyBase::CopyTo(const std::shared_ptr<IProperty>& Other) const
{
	if (IsNonCopyable())
		return;

	if (Other->IsNonCopyable())
		return;

	Other->SetName(GetName());
	Other->SetDescription(GetDescription());
	Other->SetSyntetic(IsSyntetic());
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
	Writer->SetStrAttribute(GetPropertyTypeName(this), "Type");
}



//
// Protected
//
std::string CPropertyBase::GetPropertyTypeName(const IProperty* Property) const
{
	if (Property == nullptr)
		throw CException("Property is nullptr.");

	if (dynamic_cast<const CProperty<bool>*>(Property))
	{
		return "Bool";
	}
	else if (dynamic_cast<const CProperty<int64>*>(Property))
	{
		return "Int";
	}
	else if (dynamic_cast<const CProperty<uint64>*>(Property))
	{
		return "UInt";
	}
	else if (dynamic_cast<const CProperty<float>*>(Property))
	{
		return "Float";
	}
	else if (dynamic_cast<const CProperty<glm::vec2>*>(Property))
	{
		return "Vec2";
	}
	else if (dynamic_cast<const CProperty<glm::vec3>*>(Property))
	{
		return "Vec3";
	}
	else if (dynamic_cast<const CProperty<glm::vec4>*>(Property))
	{
		return "Vec4";
	}
	else if (dynamic_cast<const CProperty<std::string>*>(Property))
	{
		return "String";
	}
	else if (dynamic_cast<const CPropertiesGroup*>(Property))
	{
		return "Group";
	}

	throw CException("Unknown property type '%s'", GetName().c_str());
}

std::shared_ptr<IProperty> CPropertyBase::CreateNewPropety(std::string PropertyName, std::string TypeName)
{
	if (TypeName == "Bool")
	{
		return MakeShared(CProperty<bool>, PropertyName, "someDescription", false);
	}
	else if (TypeName == "Float")
	{
		return MakeShared(CProperty<float>, PropertyName, "someDescription", 0.0f);
	}
	else if (TypeName == "Vec2")
	{
		return MakeShared(CProperty<glm::vec2>, PropertyName, "someDescription", glm::vec2(0.0f));
	}
	else if (TypeName == "Vec3")
	{
		return MakeShared(CProperty<glm::vec3>, PropertyName, "someDescription", glm::vec3(0.0f));
	}
	else if (TypeName == "Vec4")
	{
		return MakeShared(CProperty<glm::vec4>, PropertyName, "someDescription", glm::vec4(0.0f));
	}
	else if (TypeName == "String")
	{
		return MakeShared(CProperty<std::string>, PropertyName, "someDescription", "");
	}
	else if (TypeName == "Group")
	{
		return MakeShared(CPropertiesGroup, PropertyName, "someDescription");
	}

	throw CException("Unknown property type '%s'", TypeName);
}