#include "stdafx.h"

// General
#include "XMLWriter.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLManager.h"
#include "Utils.h"



CXMLWriter::CXMLWriter(std::shared_ptr<TiXmlDocument> Document, TiXmlElement * Element)
	: m_Document(Document)
	, m_Element(Element)
{
}

CXMLWriter::~CXMLWriter()
{
	_ASSERT(m_Element != nullptr);
	delete m_Element;
}

void CXMLWriter::SetName(const std::string & Value)
{
	m_Element->SetValue(Value);
}

void CXMLWriter::SetValue(const std::string& Value)
{
	TiXmlText * text = new TiXmlText(Value);
	m_Element->LinkEndChild(text);
}

void CXMLWriter::AddStr(std::string AttributeValue, const std::string& AttributeName) const
{
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, AttributeValue);
}

#include <inttypes.h>

namespace
{
	template <typename T>
	inline std::string SetNumericAttribute(const std::string& Pattern, T Value)
	{
		T value(Value);

		char buff[256];
		sprintf_s(buff, 256, Pattern.c_str(), value);
		return buff;
	}
}

void CXMLWriter::AddInt8(int8 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<int8>(PRId8, Value));
}

void CXMLWriter::AddUInt8(uint8 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<uint8>(PRIu8, Value));
}

void CXMLWriter::AddInt16(int16 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<int16>(PRId16, Value));
}

void CXMLWriter::AddUInt16(uint16 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<uint16>(PRIu16, Value));
}

void CXMLWriter::AddInt32(int32 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<int32>(PRId32, Value));
}

void CXMLWriter::AddUInt32(uint32 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<uint32>(PRIu32, Value));
}

void CXMLWriter::AddInt64(int64 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<int64>(PRId64, Value));
}

void CXMLWriter::AddUInt64(uint64 Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<uint64>(PRIu64, Value));
}

void CXMLWriter::AddFloat(float Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<float>("%f", Value));
}

void CXMLWriter::AddDouble(double Value, const std::string & AttributeName) const
{
	AddStr(AttributeName, SetNumericAttribute<double>("%lf", Value));
}

void CXMLWriter::AddVec3(glm::vec3 AttributeValue, const std::string& AttributeName) const
{
	char buff[256];
	sprintf_s(buff, 256, "%f, %f, %f", AttributeValue.x, AttributeValue.y, AttributeValue.z);
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, buff);
}

void CXMLWriter::AddVec4(glm::vec4 AttributeValue, const std::string& AttributeName) const
{
	char buff[256];
	sprintf_s(buff, 256, "%f, %f, %f, %f", AttributeValue.x, AttributeValue.y, AttributeValue.z, AttributeValue.w);
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, buff);
}

std::shared_ptr<IXMLWriter> CXMLWriter::CreateChild(const std::string& ChildName) const
{
	TiXmlElement * element = new TiXmlElement(ChildName);
	m_Element->LinkEndChild(element);

	return std::make_shared<CXMLWriter>(m_Document, element);
}

std::shared_ptr<IFile> CXMLWriter::SaveToFile(const std::string& Filename)
{
	std::shared_ptr<CFile> file = std::make_shared<CFile>(Filename);

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	m_Document->Accept(&printer);
	CheckTinyXMLError(m_Document.get());

	file->writeBytes(printer.CStr(), printer.Size());

	return file;
}
