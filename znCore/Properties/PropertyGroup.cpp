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
	for (const auto& propertyIt : m_Properties)
	{
		Log::Info("Property '%s' has current value '%s' and will be reseted to default.", propertyIt.first.c_str(), propertyIt.second->ToString().c_str());
		propertyIt.second->ResetToDefault();
	}
}

bool CPropertiesGroup::IsDefault() const
{
	for (const auto& propertyIt : m_Properties)
		if (false == propertyIt.second->IsDefault())
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
		if (prop.second->IsNonCopyable())
			continue;

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

		//AddProperty(prop);
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
	if (Property == nullptr)
		throw CException("Unable to add nullptr property.");

	std::string newPropertyName = Property->GetName();
	const auto& propertyIt = m_Properties.find(newPropertyName);
	if (propertyIt != m_Properties.end())
		throw CException("PropertiesGroup '%s' already constains property '%s'.", GetName().c_str(), newPropertyName.c_str());

	m_Properties.insert(std::make_pair(newPropertyName, Property));
}

void CPropertiesGroup::RemoveProperty(std::shared_ptr<IProperty> Property)
{
	if (Property == nullptr)
		throw CException("Unable to remove nullptr property.");

	std::string propertyName = Property->GetName();
	const auto& propertyIt = m_Properties.find(Property->GetName());
	if (propertyIt == m_Properties.end())
		throw CException("PropertiesGroup '%s' don't constains property '%s'.", GetName().c_str(), propertyName.c_str());

	m_Properties.erase(propertyIt);
	Log::Info("Property '%s' was successfully removed from PropertiesGroup '%s'.", propertyName.c_str(), GetName().c_str());
}

void CPropertiesGroup::RemoveProperty(const std::string& PropertyName)
{
	if (PropertyName.empty())
		throw CException("Unable to remove property with empty name.");

	auto property = GetProperty(PropertyName);
	if (property == nullptr)
		throw CException("PropertiesGroup '%s' don't constains property '%s'.", GetName().c_str(), PropertyName.c_str());

	RemoveProperty(property);
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
