#include "stdafx.h"

// General
#include "XMLManager.h"

// Additional
#include "tinyxml.h"
#include "Files/File.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "Utils.h"


void CheckTinyXMLError(const TiXmlDocument* TiniXMLDocument)
{
	if (TiniXMLDocument->Error())
	{
		throw CException(L"TinyXMLError: %s. ID: '%d', Row: '%d', Col: '%d'.", Resources::utf8_to_utf16(TiniXMLDocument->ErrorDesc()), TiniXMLDocument->ErrorId(), TiniXMLDocument->ErrorRow(), TiniXMLDocument->ErrorCol());
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
