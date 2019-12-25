#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLManager
	: public IXML
{
public:
	CXMLManager();
	virtual ~CXMLManager();

	// IXML
	std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File) override;

private:

};

class ZN_API CXMLReader
	: public IXMLReader
{
public:
	CXMLReader(std::shared_ptr<TiXmlDocument> Document, const TiXmlElement* Element);
	virtual ~CXMLReader();

	// IXMLReader
	std::string GetName() const;
	std::string GetAttribute(std::string AttributeName) const;

	std::shared_ptr<IXMLReader> GetChild(std::string ChildName) const;
	std::vector<std::shared_ptr<IXMLReader>> GetChilds() const;

private:
	std::shared_ptr<TiXmlDocument> m_Document;
	const TiXmlElement* m_Node;
};