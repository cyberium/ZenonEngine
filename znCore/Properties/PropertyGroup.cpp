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

void CPropertiesGroup::CopyTo(const std::shared_ptr<IProperty>& Other) const
{
	__super::CopyTo(Other);

	if (IsNonCopyable())
		return;

	std::shared_ptr<IPropertiesGroup> otherAsPropertiesGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Other);
	if (otherAsPropertiesGroup == nullptr)
		throw CException("Unable copy '%s' property to '%s' property, because this is IPropertiesGroup, but other is not.", GetName().c_str(), Other->GetName());

	// Copy childs properties
	for (const auto& prop : GetProperties())
	{
		const auto& otherProperties = otherAsPropertiesGroup->GetProperties();
		const auto& otherChildIt = otherProperties.find(prop.first);
		if (otherChildIt == otherProperties.end())
			throw CException("Subproperty '%s' of '%s' not found in '%s'", prop.first.c_str(), GetName().c_str(), Other->GetName().c_str());

		prop.second->CopyTo(otherChildIt->second);
	}
}

void CPropertiesGroup::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	__super::Load(Reader);

	for (const auto& childReader : Reader->GetChilds())
	{
		auto prop = GetProperty(childReader->GetName());

		if (prop == nullptr)
		{
			//prop = CreateNewPropety(storedPropertyName, childReader->GetStrAttribute("Type"));

			Log::Error("Property '%s' exists in XML file, but not found in C++. Skipping.", childReader->GetName().c_str());
			continue;
		}
			
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
