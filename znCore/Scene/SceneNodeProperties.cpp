#include "stdafx.h"

// General
#include "SceneNodeProperties.h"

CPropertiesGroup::CPropertiesGroup()
	: m_Name("CPropertiesGroupName")
	, m_Description("CPropertiesGroupDescription")
{
}

CPropertiesGroup::~CPropertiesGroup()
{
}



//
// IProperty
//
std::string CPropertiesGroup::GetName() const
{
	return m_Name;
}

void CPropertiesGroup::SetName(const std::string & Name)
{
	m_Name = Name;
}

std::string CPropertiesGroup::GetDescription() const
{
	return m_Description;
}

void CPropertiesGroup::SetDescription(const std::string& Description)
{
	m_Description = Description;
}



//
// IPropertiesGroup
//
void CPropertiesGroup::AddProperty(std::shared_ptr<IProperty> Property)
{
	m_Properties.insert(std::make_pair(Property->GetName(), Property));
}

std::shared_ptr<IProperty> CPropertiesGroup::GetProperty(const std::string& PropertyName)
{
	return m_Properties[PropertyName];
}

const std::unordered_map<std::string, std::shared_ptr<IProperty>>& CPropertiesGroup::GetProperties() const
{
	return m_Properties;
}
