#include "stdafx.h"

// General
#include "XMLManager.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"


namespace
{
	void CheckTinyXMLError(const TiXmlDocument* TiniXMLDocument)
	{
		if (TiniXMLDocument->Error())
		{
			throw CException(L"TinyXMLError: %s. ID: '%d', Row: '%d', Col: '%d'.", Resources::utf8_to_utf16(TiniXMLDocument->ErrorDesc()), TiniXMLDocument->ErrorId(), TiniXMLDocument->ErrorRow(), TiniXMLDocument->ErrorCol());
		}
	}
}


// CONST BEGIN
const char* cXMLRootNodeSignature = "xml";
// CONST END

CXMLManager::CXMLManager()
{
}

CXMLManager::~CXMLManager()
{
}

std::shared_ptr<IXMLReader> CXMLManager::CreateReader(std::shared_ptr<IFile> File)
{
	TiXmlDocument* xmlDocument = new TiXmlDocument();
	bool loadOkay = xmlDocument->LoadFile(File);
	CheckTinyXMLError(xmlDocument);

	const TiXmlElement* root = xmlDocument->FirstChildElement();
	_ASSERT_EXPR(root->ValueStr() == cXMLRootNodeSignature, L"All ZenonEngine xml file must contains root node with specified name.");

	return std::make_shared<CXMLReader>(std::shared_ptr<TiXmlDocument>(xmlDocument), root);
}

std::shared_ptr<IXMLWriter> CXMLManager::CreateWriter()
{
	TiXmlDocument* xmlDocument = new TiXmlDocument();
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	xmlDocument->LinkEndChild(decl);

	TiXmlElement * rootElement = new TiXmlElement(cXMLRootNodeSignature);
	xmlDocument->LinkEndChild(rootElement);

	return std::make_shared<CXMLWriter>(std::shared_ptr<TiXmlDocument>(xmlDocument), rootElement);
}



//
// CXMLReader
//



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

std::string CXMLReader::GetAttribute(std::string AttributeName) const
{
	const std::string* string;
	string = m_Element->Attribute(AttributeName);
	if (string == nullptr)
	{
		Log::Warn("CXMLReader: Unable to get xml attribute '%s'.", AttributeName.c_str());
		return nullptr;
	}

	return *string;
}

glm::vec3 CXMLReader::GetVec3Attribute(std::string AttributeName) const
{
	auto attributeString = GetAttribute(AttributeName);
	glm::vec3 value(0.0f);
	sscanf_s(attributeString.c_str(), "%f, %f, %f", &value.x, &value.y, &value.z);
	return value;
}

std::shared_ptr<IXMLReader> CXMLReader::GetChild(std::string ChildName) const
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



//
// CXMLWriter
//



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

void CXMLWriter::SetValue(std::string Value)
{
	TiXmlText * text = new TiXmlText(Value);
	m_Element->LinkEndChild(text);
}

void CXMLWriter::AddAttribute(std::string AttributeName, std::string AttributeValue) const
{
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, AttributeValue);
}

void CXMLWriter::AddIntAttribute(std::string AttributeName, int AttributeValue) const
{
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, AttributeValue);
}

void CXMLWriter::AddFloatAttribute(std::string AttributeName, double AttributeValue) const
{
	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetDoubleAttribute(AttributeName, AttributeValue);
}

void CXMLWriter::AddVec3Attribute(std::string AttributeName, glm::vec3 AttributeValue) const
{
	char buff[256];
	sprintf_s(buff, 256, "%f, %f, %f", AttributeValue.x, AttributeValue.y, AttributeValue.z);

	_ASSERT(m_Element->Attribute(AttributeName) != nullptr);
	m_Element->SetAttribute(AttributeName, buff);
}

std::shared_ptr<IXMLWriter> CXMLWriter::CreateChild(std::string ChildName) const
{
	TiXmlElement * element = new TiXmlElement(ChildName);
	m_Element->LinkEndChild(element);

	return std::make_shared<CXMLWriter>(m_Document, element);
}

std::shared_ptr<IFile> CXMLWriter::SaveToFile(std::string Filename)
{
	std::shared_ptr<CFile> file = std::make_shared<CFile>(Filename);

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	m_Document->Accept(&printer);
	CheckTinyXMLError(m_Document.get());

	file->writeBytes(printer.CStr(), printer.Size());

	return file;
}
