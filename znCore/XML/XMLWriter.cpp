#include "stdafx.h"

// General
#include "XMLWriter.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLManager.h"
#include "Utils/Convert.h"


CXMLWriter::CXMLWriter(const std::string& Name)
	: m_Name(Name)
{}

CXMLWriter::~CXMLWriter()
{}



//
// IXMLShared
//
std::string CXMLWriter::GetName() const
{
	return m_Name;
}

std::string CXMLWriter::GetValue() const
{
	return m_Value;
}

bool CXMLWriter::IsChildExists(const std::string& ChildName) const
{
	auto chIt = std::find_if(m_Childs.begin(), m_Childs.end(), [ChildName](const std::pair<std::string, std::shared_ptr<IXMLWriter>>& Value) -> bool {
		return Value.first == ChildName;
	});
	return chIt != m_Childs.end();
}

bool CXMLWriter::IsAttributeExists(const std::string & AttributeName) const
{
	auto attrIt = std::find_if(m_Attributes.begin(), m_Attributes.end(), [AttributeName](const std::pair<std::string, std::string>& Value) -> bool {
		return Value.first == AttributeName;
	});
	return attrIt != m_Attributes.end();
}



//
// IXMLWriter
//
void CXMLWriter::SetName(const std::string & Value)
{
	m_Name = Value;
}



//
// Attributes
//
void CXMLWriter::SetStrAttribute(std::string AttributeValue, const std::string& AttributeName)
{
	if (IsAttributeExists(AttributeName))
		throw CException("CXMLWriter: Attribute '%s' aleady exists!");
	m_Attributes.push_back(std::make_pair(AttributeName, AttributeValue));
}

void CXMLWriter::SetInt8Attribute(int8 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetUInt8Attribute(uint8 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetInt16Attribute(int16 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetUInt16Attribute(uint16 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetInt32Attribute(int32 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetUInt32Attribute(uint32 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetInt64Attribute(int64 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetUInt64Attribute(uint64 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetFloatAttribute(float AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetDoubleAttribute(double AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetVec2Attribute(glm::vec2 AttributeValue, const std::string & AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetVec3Attribute(glm::vec3 AttributeValue, const std::string& AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

void CXMLWriter::SetVec4Attribute(glm::vec4 AttributeValue, const std::string& AttributeName)
{
	SetStrAttribute(ValueToString(AttributeValue), AttributeName);
}

std::vector< std::pair<std::string, std::string>> CXMLWriter::GetAttributes() const
{
	return m_Attributes;
}



//
// Values
//
void CXMLWriter::SetValue(const std::string& Value)
{
	m_Value = Value;
}

void CXMLWriter::SetInt8(int8 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetUInt8(uint8 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetInt16(int16 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetUInt16(uint16 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetInt32(int32 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetUInt32(uint32 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetInt64(int64 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetUInt64(uint64 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetFloat(float Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetDouble(double Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetVec2(glm::vec2 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetVec3(glm::vec3 Value)
{
	SetValue(ValueToString(Value));
}

void CXMLWriter::SetVec4(glm::vec4 Value)
{
	SetValue(ValueToString(Value));
}


//
// Childs
//
std::shared_ptr<IXMLWriter> CXMLWriter::CreateChild(const std::string& ChildName)
{
	auto child = MakeShared(CXMLWriter, ChildName);
	AddChild(child);
	return child;
}

void CXMLWriter::AddChild(const std::shared_ptr<IXMLWriter>& Writer)
{
	//_ASSERT(false == IsChildExists(Writer->GetName()));
	m_Childs.push_back(std::make_pair(Writer->GetName(), Writer));
}

std::vector< std::pair<std::string, std::shared_ptr<IXMLWriter>>> CXMLWriter::GetChilds() const
{
	return m_Childs;
}
