#include "stdafx.h"

// General
#include "XMLReader.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLManager.h"
#include "Utils/Convert.h"

CXMLReader::CXMLReader(const std::string& Name, const std::string& Value, const std::vector<std::pair<std::string, std::string>>& Attributes, const std::vector<std::pair<std::string, std::shared_ptr<IXMLReader>>>& Childs)
	: m_Name(Name)
	, m_Value(Value)
	, m_Attributes(Attributes)
	, m_Childs(Childs)
{
}

CXMLReader::~CXMLReader()
{
}



//
// IXMLShared
//
std::string CXMLReader::GetName() const
{
	return m_Name;
}

std::string CXMLReader::GetValue() const
{
	return m_Value;
}

bool CXMLReader::IsChildExists(const std::string & ChildName) const
{
	auto chIt = std::find_if(m_Childs.begin(), m_Childs.end(), [ChildName](const std::pair<std::string, std::shared_ptr<IXMLReader>>& Value) -> bool {
		return Value.first == ChildName;
	});
	return chIt != m_Childs.end();
}

bool CXMLReader::IsAttributeExists(const std::string & AttributeName) const
{
	auto attrIt = std::find_if(m_Attributes.begin(), m_Attributes.end(), [AttributeName](const std::pair<std::string, std::string>& Value) -> bool {
		return Value.first == AttributeName;
	});
	return attrIt != m_Attributes.end();
}


//
// IXMLReader
//

//
// Attributes
//
std::string CXMLReader::GetStrAttribute(const std::string & AttributeName) const
{
	auto attrIt = std::find_if(m_Attributes.begin(), m_Attributes.end(), [AttributeName](const std::pair<std::string, std::string>& Value) -> bool {
		return Value.first == AttributeName;
	});
	if (attrIt == m_Attributes.end())
	{
		Log::Warn("CXMLReader: Unable to get xml attribute '%s'.", AttributeName.c_str());
		return "";
	}

	return attrIt->second;
}

bool CXMLReader::GetBoolAttribute(const std::string & AttributeName) const
{
	return StringToValue<bool>(GetStrAttribute(AttributeName));
}

int64 CXMLReader::GetIntAttribute(const std::string & AttributeName) const
{
	return StringToValue<int64>(GetStrAttribute(AttributeName));
}

uint64 CXMLReader::GetUIntAttribute(const std::string & AttributeName) const
{
	return StringToValue<uint64>(GetStrAttribute(AttributeName));
}

float CXMLReader::GetFloatAttribute(const std::string & AttributeName) const
{
	return StringToValue<float>(GetStrAttribute(AttributeName));
}

double CXMLReader::GetDoubleAttribute(const std::string & AttributeName) const
{
	return StringToValue<double>(GetStrAttribute(AttributeName));
}

glm::vec2 CXMLReader::GetVec2Attribute(const std::string & AttributeName) const
{
	return StringToValue<glm::vec2>(GetStrAttribute(AttributeName));
}

glm::vec3 CXMLReader::GetVec3Attribute(const std::string& AttributeName) const
{
	return StringToValue<glm::vec3>(GetStrAttribute(AttributeName));
}

glm::vec4 CXMLReader::GetVec4Attribute(const std::string & AttributeName) const
{
	return StringToValue<glm::vec4>(GetStrAttribute(AttributeName));
}

std::vector<std::string> CXMLReader::GetAttributes() const
{
	std::vector<std::string> attributes;
	for (const auto& it : m_Attributes)
		attributes.push_back(it.second);
	return attributes;
}



//
// Values
//
bool CXMLReader::GetBool() const
{
	return StringToValue<bool>(GetValue());
}

int64 CXMLReader::GetInt() const
{
	return StringToValue<int64>(GetValue());
}

uint64 CXMLReader::GetUInt() const
{
	return StringToValue<uint64>(GetValue());
}

float CXMLReader::GetFloat() const
{
	return StringToValue<float>(GetValue());
}

double CXMLReader::GetDouble() const
{
	return StringToValue<double>(GetValue());
}

glm::vec2 CXMLReader::GetVec2() const
{
	return StringToValue<glm::vec2>(GetValue());
}

glm::vec3 CXMLReader::GetVec3() const
{
	return StringToValue<glm::vec3>(GetValue());
}

glm::vec4 CXMLReader::GetVec4() const
{
	return StringToValue<glm::vec4>(GetValue());
}

ColorRGB CXMLReader::GetColorRGB() const
{
	return StringToValue<ColorRGB>(GetValue());
}

ColorRGBA CXMLReader::GetColorRGBA() const
{
	return StringToValue<ColorRGBA>(GetValue());
}



//
// Childs
//
std::shared_ptr<IXMLReader> CXMLReader::GetChild(const std::string& ChildName) const
{
	auto chIt = std::find_if(m_Childs.begin(), m_Childs.end(), [ChildName](const std::pair<std::string, std::shared_ptr<IXMLReader>>& Value) -> bool {
		return Value.first == ChildName;
	});
	if (chIt == m_Childs.end())
	{
		Log::Warn("CXMLReader: Unable to find child '%s'.", ChildName.c_str());
		return nullptr;
	}

	return chIt->second;
}

std::vector<std::shared_ptr<IXMLReader>> CXMLReader::GetChilds() const
{
	std::vector<std::shared_ptr<IXMLReader>> childs;
	for (const auto& it : m_Childs)
		childs.push_back(it.second);
	return childs;
}