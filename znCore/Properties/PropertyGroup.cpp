#include "stdafx.h"

// General
#include "PropertyGroup.h"

CPropertiesGroup::CPropertiesGroup(std::string Name, std::string Description)
	: CPropertyBase(Name, Description)
{
}

CPropertiesGroup::~CPropertiesGroup()
{
}



//
// IProperty
//
void CPropertiesGroup::ResetToDefault()
{
	for (const auto& prop : m_Properties)
	{
		Log::Info("Property '%s' has current value '%s' and will be reseted to default.", prop.first.c_str(), prop.second->ToString().c_str());
		prop.second->ResetToDefault();
	}
}

bool CPropertiesGroup::IsDefault() const
{
	for (const auto& prop : m_Properties)
		if (false == prop.second->IsDefault())
			return false;
	return true;
}

void CPropertiesGroup::FromString(const std::string & String, bool BlockCallback)
{
	throw CException("Incorrect behaviour");
}

std::string CPropertiesGroup::ToString() const
{
	throw CException("Incorrect behaviour");
}

void CPropertiesGroup::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	__super::Load(Reader);

	for (const auto& childReader : Reader->GetChilds())
	{
		std::string storedPropertyName = childReader->GetName();
		auto prop = GetProperty(storedPropertyName);

		if (prop == nullptr)
			prop = CreateNewPropety(storedPropertyName, childReader->GetStrAttribute("Type"));

		prop->Load(childReader);

		AddProperty(prop);
	}
}

void CPropertiesGroup::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	__super::Save(Writer);

	for (const auto& prop : GetProperties())
	{
		const auto& propObjet = prop.second;
		if (propObjet->IsSyntetic())
			continue;

		if (propObjet->IsDefault())
			continue;

		auto propertyWriter = Writer->CreateChild(propObjet->GetName());
		propertyWriter->SetStrAttribute(GetPropertyTypeName(propObjet.get()), "Type");
		propObjet->Save(propertyWriter);
	}
}



//
// IPropertiesGroup
//
void CPropertiesGroup::AddProperty(std::shared_ptr<IProperty> Property)
{
	_ASSERT(Property != nullptr);
	m_Properties.insert(std::make_pair(Property->GetName(), Property));
}

std::shared_ptr<IProperty> CPropertiesGroup::GetProperty(const std::string& PropertyName)
{
	const auto& it = m_Properties.find(PropertyName);
	if (it == m_Properties.end())
		return nullptr;
	return it->second;
}

const std::unordered_map<std::string, std::shared_ptr<IProperty>>& CPropertiesGroup::GetProperties() const
{
	return m_Properties;
}


//
// Private
//
std::string CPropertiesGroup::GetPropertyTypeName(const IProperty* Property) const
{
	if (Property == nullptr)
		throw CException("Property is nullptr.");

	if (dynamic_cast<const CProperty<float>*>(Property))
		return "Float";
	else if (dynamic_cast<const CProperty<glm::vec2>*>(Property))
		return "Vec2";
	else if (dynamic_cast<const CProperty<glm::vec3>*>(Property) || dynamic_cast<const CPropertyWrapped<glm::vec3>*>(Property))
		return "Vec3";
	else if (dynamic_cast<const CProperty<glm::vec4>*>(Property))
		return "Vec4";
	else if (dynamic_cast<const CProperty<std::string>*>(Property))
		return "String";
	else if (dynamic_cast<const CPropertiesGroup*>(Property))
		return "Group";
	else
		throw CException("Unknown property type '%s'", GetName().c_str());
}

std::shared_ptr<IProperty> CPropertiesGroup::CreateNewPropety(std::string PropertyName, std::string TypeName)
{
	if (TypeName == "Float")
		return MakeShared(CProperty<float>, PropertyName, "someDescription", 0.0f);
	else if (TypeName == "Vec2")
		return MakeShared(CProperty<glm::vec2>, PropertyName, "someDescription", glm::vec2(0.0f));
	else if (TypeName == "Vec3")
		return MakeShared(CProperty<glm::vec3>, PropertyName, "someDescription", glm::vec3(0.0f));
	else if (TypeName == "Vec4")
		return MakeShared(CProperty<glm::vec4>, PropertyName, "someDescription", glm::vec4(0.0f));
	else if (TypeName == "String")
		return MakeShared(CProperty<std::string>, PropertyName, "someDescription", "");
	else if (TypeName == "Group")
		return MakeShared(CPropertiesGroup, PropertyName, "someDescription");
	else
		throw CException("Unknown property type '%s'", TypeName);
}
