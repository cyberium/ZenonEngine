#include "stdafx.h"

// General
#include "XMLManager.h"

// Additional
#include "tinyxml.h"
#include "XMLReader.h"
#include "XMLWriter.h"

namespace
{
	TiXmlElement * CreateTiXMLWriter(const std::shared_ptr<IXMLWriter>& Writer)
	{
		TiXmlElement * element = DEBUG_NEW TiXmlElement(Writer->GetName());

		// Name
		element->SetValue(Writer->GetName());

		// Value
		if (false == Writer->GetValue().empty())
		{
			TiXmlText * text = DEBUG_NEW TiXmlText(Writer->GetValue());
			element->LinkEndChild(text);
		}

		for (const auto& attr : Writer->GetAttributes())
			element->SetAttribute(attr.first, attr.second);

		for (const auto& child : Writer->GetChilds())
			element->LinkEndChild(CreateTiXMLWriter(child.second));

		return element;
	}



	std::shared_ptr<IXMLReader> CreateXMLReader(const TiXmlElement * Node)
	{
		// Name
		std::string Name = Node->ValueStr();

		// Value
		const char* text = Node->GetText();
		std::string Value = "";
		if (text != nullptr)
			Value = text;


		std::vector<std::pair<std::string, std::string>> attributes;
		for (const TiXmlAttribute* attr = Node->FirstAttribute(); attr != nullptr; attr = attr->Next())
		{
			_ASSERT(std::find_if(attributes.begin(), attributes.end(), [attr](const std::pair<std::string, std::string>& Value) -> bool { return Value.first == attr->Name(); }) == attributes.end());
			attributes.push_back(std::make_pair(attr->Name(), attr->Value()));
		}

		std::vector<std::pair<std::string, std::shared_ptr<IXMLReader>>> childs;
		for (const TiXmlElement* node = Node->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		{
			//_ASSERT(std::find_if(childs.begin(), childs.end(), [node](const std::pair<std::string, std::shared_ptr<IXMLReader>>& Value) -> bool { return Value.first == node->ValueStr(); }) == childs.end());
			childs.push_back(std::make_pair(node->ValueStr(), CreateXMLReader(node)));
		}

		return MakeShared(CXMLReader, Name, Value, attributes, childs);
	}
}


void CheckTinyXMLError(const std::shared_ptr<TiXmlDocument>& TiniXMLDocument)
{
	if (!TiniXMLDocument->Error())
		return;

	throw CException(L"TinyXMLError: %s. ID: '%d', Row: '%d', Col: '%d'.", Resources::utf8_to_utf16(TiniXMLDocument->ErrorDesc()), TiniXMLDocument->ErrorId(), TiniXMLDocument->ErrorRow(), TiniXMLDocument->ErrorCol());
}


// CONST BEGIN
const char* cXMLRootNodeSignature = "xml";
// CONST END

CXMLManager::CXMLManager(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CXMLManager::~CXMLManager()
{
}

std::shared_ptr<IXMLReader> CXMLManager::CreateReader(std::shared_ptr<IFile> File)
{
	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	bool loadOkay = xmlDocument->LoadFile(File);
	CheckTinyXMLError(xmlDocument);

	const TiXmlElement* root = xmlDocument->FirstChildElement();
	_ASSERT_EXPR(root->ValueStr() == cXMLRootNodeSignature, L"All ZenonEngine xml file must contains root node with specified name.");

	return CreateXMLReader(root);
}

std::shared_ptr<IXMLWriter> CXMLManager::CreateWriter()
{
	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	TiXmlDeclaration * decl = DEBUG_NEW TiXmlDeclaration("1.0", "", "");
	xmlDocument->LinkEndChild(decl);

	TiXmlElement * rootElement = DEBUG_NEW TiXmlElement(cXMLRootNodeSignature);
	xmlDocument->LinkEndChild(rootElement);

	return MakeShared(CXMLWriter, rootElement->ValueStr());
}

std::shared_ptr<IXMLWriter> CXMLManager::CreateWriter(const std::string & NodeName)
{
	return MakeShared(CXMLWriter, NodeName);
}


std::shared_ptr<IFile> CXMLManager::SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName)
{
	std::shared_ptr<IFile> file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);

	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	TiXmlDeclaration * decl = DEBUG_NEW TiXmlDeclaration("1.0", "", "");
	xmlDocument->LinkEndChild(decl);
	xmlDocument->LinkEndChild(CreateTiXMLWriter(Writer));

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	xmlDocument->Accept(&printer);
	CheckTinyXMLError(xmlDocument);

	file->writeBytes(printer.CStr(), printer.Size());

	return file;
}
