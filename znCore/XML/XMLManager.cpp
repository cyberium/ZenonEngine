#include "stdafx.h"

// General
#include "XMLManager.h"

// Additional
#include "Files/File.h"

// Additional (TiXML)
#include "tinyxml.h"
#include "XMLReader.h"
#include "XMLWriter.h"



namespace
{
	TiXmlElement * CreateTiXMLWriter(const std::shared_ptr<IXMLWriter>& Writer)
	{
		TiXmlElement * element = ZN_NEW TiXmlElement(Writer->GetName());

		// Name
		element->SetValue(Writer->GetName());

		// Value
		if (false == Writer->GetValue().empty())
		{
			TiXmlText * text = ZN_NEW TiXmlText(Writer->GetValue());
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


	void CheckTinyXMLError(const std::shared_ptr<TiXmlDocument>& TiniXMLDocument)
	{
		if (false == TiniXMLDocument->Error())
			return;
		throw CException("TinyXMLError: %s. ID: '%d', Row: '%d', Col: '%d'.", TiniXMLDocument->ErrorDesc(), TiniXMLDocument->ErrorId(), TiniXMLDocument->ErrorRow(), TiniXMLDocument->ErrorCol());
	}
}




// CONST BEGIN
const char* cXMLRootNodeSignature = "xml";
// CONST END

CXMLManager::CXMLManager(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CXMLManager::~CXMLManager()
{
}



//
// IXML
//
std::shared_ptr<IXMLReader> CXMLManager::CreateReaderFromString(const std::string& String)
{
	std::shared_ptr<IByteBuffer> byteBuffer = MakeShared(CByteBuffer);
	byteBuffer->writeString(String);
	byteBuffer->seek(0);

	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	xmlDocument->LoadFile(byteBuffer);
	CheckTinyXMLError(xmlDocument);

	const TiXmlElement* root = xmlDocument->FirstChildElement();
	_ASSERT_EXPR(root->ValueStr() == cXMLRootNodeSignature, L"All ZenonEngine xml file must contains root node with specified name.");

	return CreateXMLReader(root);
}

std::shared_ptr<IXMLReader> CXMLManager::CreateReaderFromFile(const std::string& FileName)
{
	std::shared_ptr<IFile> file = m_BaseManager.GetManager<IFilesManager>()->Open(FileName);
	return CreateReaderFromFile(file);
}

std::shared_ptr<IXMLReader> CXMLManager::CreateReaderFromFile(std::shared_ptr<IFile> File)
{
	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	xmlDocument->LoadFile(File);
	CheckTinyXMLError(xmlDocument);

	const TiXmlElement* root = xmlDocument->FirstChildElement();
	_ASSERT_EXPR(root->ValueStr() == cXMLRootNodeSignature, L"All ZenonEngine xml file must contains root node with specified name.");

	return CreateXMLReader(root);
}

std::shared_ptr<IXMLWriter> CXMLManager::CreateWriter()
{
	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	
	TiXmlDeclaration * decl = ZN_NEW TiXmlDeclaration("1.0", "", "");
	xmlDocument->LinkEndChild(decl);

	TiXmlElement * rootElement = ZN_NEW TiXmlElement(cXMLRootNodeSignature);
	xmlDocument->LinkEndChild(rootElement);

	return MakeShared(CXMLWriter, rootElement->ValueStr());
}

std::shared_ptr<IXMLWriter> CXMLManager::CreateWriter(const std::string & NodeName)
{
	return MakeShared(CXMLWriter, NodeName);
}


std::string CXMLManager::SaveWriterToString(const std::shared_ptr<IXMLWriter>& Writer)
{
	std::shared_ptr<TiXmlDocument> xmlDocument = MakeShared(TiXmlDocument);
	TiXmlDeclaration * decl = ZN_NEW TiXmlDeclaration("1.0", "", "");
	xmlDocument->LinkEndChild(decl);
	xmlDocument->LinkEndChild(CreateTiXMLWriter(Writer));

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	xmlDocument->Accept(&printer);
	CheckTinyXMLError(xmlDocument);

	return std::string(printer.CStr(), printer.Size());
}

std::shared_ptr<IFile> CXMLManager::SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName)
{
	std::shared_ptr<IFile> file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);
	return SaveWriterToFile(Writer, file);
}

std::shared_ptr<IFile> CXMLManager::SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::shared_ptr<IFile>& File)
{
	std::string xmlContent = SaveWriterToString(Writer);
	File->writeString(xmlContent);
	return File;
}
