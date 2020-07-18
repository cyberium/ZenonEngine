#include "stdafx.h"

// General
#include "XMLReader.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLManager.h"
#include "Utils.h"


CXMLReader::CXMLReader(std::shared_ptr<TiXmlDocument> Document, const TiXmlElement* Element)
	: m_Document(Document)
	, m_Element(Element)
{
}

CXMLReader::~CXMLReader()
{
}

std::string CXMLReader::GetName() const
{
	return m_Element->ValueStr();
}

std::string CXMLReader::GetValue() const
{
	return m_Element->GetText();
}

bool CXMLReader::IsExists(const std::string & AttributeName) const
{
	return m_Element->Attribute(AttributeName) != nullptr;
}

std::string CXMLReader::GetStr(const std::string& AttributeName) const
{
	const std::string* string;
	string = m_Element->Attribute(AttributeName);
	if (string == nullptr)
	{
		Log::Warn("CXMLReader: Unable to get xml attribute '%s'.", AttributeName.c_str());
		return "";
	}

	return *string;
}

#include <inttypes.h>

namespace
{
	template <typename T>
	inline T GetNumericAttribute(const std::string& FullString, const std::string& Pattern, T DefaultValue)
	{
		T value(DefaultValue);
		sscanf_s(FullString.c_str(), Pattern.c_str(), &value);
		return value;
	}
}

int8 CXMLReader::GetInt8(const std::string & AttributeName) const
{
	return GetNumericAttribute<int8>(GetStr(AttributeName), PRId8, 0);
}

uint8 CXMLReader::GetUInt8(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint8>(GetStr(AttributeName), PRIu8, 0u);
}

int16 CXMLReader::GetInt16(const std::string & AttributeName) const
{
	return GetNumericAttribute<int16>(GetStr(AttributeName), PRId16, 0);
}

uint16 CXMLReader::GetUInt16(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint16>(GetStr(AttributeName), PRIu16, 0u);
}

int32 CXMLReader::GetInt32(const std::string & AttributeName) const
{
	return GetNumericAttribute<int32>(GetStr(AttributeName), PRId32, 0);
}

uint32 CXMLReader::GetUInt32(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint32>(GetStr(AttributeName), PRIu32, 0u);
}

int64 CXMLReader::GetInt64(const std::string & AttributeName) const
{
	return GetNumericAttribute<int64>(GetStr(AttributeName), PRId64, 0l);
}

uint64 CXMLReader::GetUInt64(const std::string & AttributeName) const
{
	return GetNumericAttribute<uint64>(GetStr(AttributeName), PRIu64, 0ul);
}

float CXMLReader::GetFloat(const std::string & AttributeName) const
{
	return GetNumericAttribute<float>(GetStr(AttributeName), "%f", 0.0f);
}

double CXMLReader::GetDouble(const std::string & AttributeName) const
{
	return GetNumericAttribute<double>(GetStr(AttributeName), "%lf", 0.0);
}

glm::vec3 CXMLReader::GetVec3(const std::string& AttributeName) const
{
	auto attributeString = GetStr(AttributeName);
	glm::vec3 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f", &value.x, &value.y, &value.z);
	return value;
}

glm::vec4 CXMLReader::GetVec4(const std::string & AttributeName) const
{
	auto attributeString = GetStr(AttributeName);
	glm::vec4 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f, %f", &value.x, &value.y, &value.z, &value.w);
	return value;
}

std::shared_ptr<IXMLReader> CXMLReader::GetChild(const std::string& ChildName) const
{
	const TiXmlElement* childNode = m_Element->FirstChildElement(ChildName);
	if (childNode == nullptr)
	{
		Log::Warn("CXMLReader: Unable to find child '%s'.", ChildName.c_str());
		return nullptr;
	}

	return std::make_shared<CXMLReader>(m_Document, childNode);
}

std::vector<std::shared_ptr<IXMLReader>> CXMLReader::GetChilds() const
{
	std::vector<std::shared_ptr<IXMLReader>> childs;
	for (const TiXmlElement* node = m_Element->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		childs.push_back(std::make_shared<CXMLReader>(m_Document, node));

	return childs;
}