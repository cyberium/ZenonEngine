#include "stdafx.h"

// General
#include "XMLReader.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLManager.h"

namespace
{
#include <inttypes.h>

	template <typename T>
	inline T GetNumericAttribute(const std::string& FullString, const std::string& Pattern, T DefaultValue)
	{
		T value(DefaultValue);
		sscanf_s(FullString.c_str(), (std::string("%") + Pattern).c_str(), &value);
		return value;
	}
}


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


int8 CXMLReader::GetInt8Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<int8>(GetStrAttribute(AttributeName), PRId8, 0);
}

uint8 CXMLReader::GetUInt8Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint8>(GetStrAttribute(AttributeName), PRIu8, 0u);
}

int16 CXMLReader::GetInt16Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<int16>(GetStrAttribute(AttributeName), PRId16, 0);
}

uint16 CXMLReader::GetUInt16Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint16>(GetStrAttribute(AttributeName), PRIu16, 0u);
}

int32 CXMLReader::GetInt32Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<int32>(GetStrAttribute(AttributeName), PRId32, 0);
}

uint32 CXMLReader::GetUInt32Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint32>(GetStrAttribute(AttributeName), PRIu32, 0u);
}

int64 CXMLReader::GetInt64Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<int64>(GetStrAttribute(AttributeName), PRId64, 0l);
}

uint64 CXMLReader::GetUInt64Attribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint64>(GetStrAttribute(AttributeName), PRIu64, 0ul);
}

float CXMLReader::GetFloatAttribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<float>(GetStrAttribute(AttributeName), "%f", 0.0f);
}

double CXMLReader::GetDoubleAttribute(const std::string & AttributeName) const
{
	return GetNumericAttribute<double>(GetStrAttribute(AttributeName), "%lf", 0.0);
}

glm::vec2 CXMLReader::GetVec2Attribute(const std::string & AttributeName) const
{
	auto attributeString = GetStrAttribute(AttributeName);
	glm::vec2 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f", &value.x, &value.y);
	return value;
}

glm::vec3 CXMLReader::GetVec3Attribute(const std::string& AttributeName) const
{
	auto attributeString = GetStrAttribute(AttributeName);
	glm::vec3 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f", &value.x, &value.y, &value.z);
	return value;
}

glm::vec4 CXMLReader::GetVec4Attribute(const std::string & AttributeName) const
{
	auto attributeString = GetStrAttribute(AttributeName);
	glm::vec4 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f, %f", &value.x, &value.y, &value.z, &value.w);
	return value;
}



//
// Values
//
int8 CXMLReader::GetInt8() const
{
	return GetNumericAttribute<int8>(GetValue(), PRId8, 0);
}

uint8 CXMLReader::GetUInt8() const
{
	return GetNumericAttribute<uint8>(GetValue(), PRIu8, 0u);
}

int16 CXMLReader::GetInt16() const
{
	return GetNumericAttribute<int16>(GetValue(), PRId16, 0);
}

uint16 CXMLReader::GetUInt16() const
{
	return GetNumericAttribute<uint16>(GetValue(), PRIu16, 0u);
}

int32 CXMLReader::GetInt32() const
{
	return GetNumericAttribute<int32>(GetValue(), PRId32, 0);
}

uint32 CXMLReader::GetUInt32() const
{
	return GetNumericAttribute<uint32>(GetValue(), PRIu32, 0u);
}

int64 CXMLReader::GetInt64() const
{
	return GetNumericAttribute<int64>(GetValue(), PRId64, 0l);
}

uint64 CXMLReader::GetUInt64() const
{
	return GetNumericAttribute<uint64>(GetValue(), PRIu64, 0ul);
}

float CXMLReader::GetFloat() const
{
	return GetNumericAttribute<float>(GetValue(), "%f", 0.0f);
}

double CXMLReader::GetDouble() const
{
	return GetNumericAttribute<double>(GetValue(), "%lf", 0.0);
}

glm::vec2 CXMLReader::GetVec2() const
{
	auto attributeString = GetValue();
	glm::vec2 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f", &value.x, &value.y);
	return value;
}

glm::vec3 CXMLReader::GetVec3() const
{
	auto attributeString = GetValue();
	glm::vec3 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f", &value.x, &value.y, &value.z);
	return value;
}

glm::vec4 CXMLReader::GetVec4() const
{
	auto attributeString = GetValue();
	glm::vec4 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f, %f", &value.x, &value.y, &value.z, &value.w);
	return value;
}

std::vector<std::string> CXMLReader::GetAttributes() const
{
	std::vector<std::string> attributes;
	for (const auto& it : m_Attributes)
		attributes.push_back(it.second);
	return attributes;
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