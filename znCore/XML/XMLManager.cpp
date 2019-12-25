#include "stdafx.h"

// General
#include "XMLManager.h"

// Additional
#include "tinyxml.h"

CXMLManager::CXMLManager()
{
}

CXMLManager::~CXMLManager()
{
}

std::shared_ptr<IXMLReader> CXMLManager::CreateReader(std::shared_ptr<IFile> File)
{
	TiXmlDocument* doc = new TiXmlDocument();
	bool loadOkay = doc->LoadFile(File);
	const TiXmlElement* root = doc->FirstChildElement();

	return std::make_shared<CXMLReader>(std::shared_ptr<TiXmlDocument>(doc), root);
}



//
// CXMLReader
//



CXMLReader::CXMLReader(std::shared_ptr<TiXmlDocument> Document, const TiXmlElement* Element)
	: m_Document(Document)
	, m_Node(Element)
{
}

CXMLReader::~CXMLReader()
{
}

std::string CXMLReader::GetName() const
{
	return m_Node->ValueStr();
}

std::string CXMLReader::GetAttribute(std::string AttributeName) const
{
	const std::string* string;
	string = m_Node->Attribute(AttributeName);
	if (string == nullptr)
		_ASSERT_EXPR(false, L"Unable to get xml attribute.");

	return *string;
}

std::shared_ptr<IXMLReader> CXMLReader::GetChild(std::string ChildName) const
{
	const TiXmlElement* childNode = m_Node->FirstChildElement(ChildName);
	if (childNode == nullptr)
		_ASSERT_EXPR(false, L"Unable to find child.");

	return std::make_shared<CXMLReader>(m_Document, childNode);
}

std::vector<std::shared_ptr<IXMLReader>> CXMLReader::GetChilds() const
{
	std::vector<std::shared_ptr<IXMLReader>> childs;
	for (const TiXmlElement* node = m_Node->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		childs.push_back(std::make_shared<CXMLReader>(m_Document, node));

	return childs;
}
