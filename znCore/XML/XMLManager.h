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
	std::shared_ptr<IXMLWriter> CreateWriter() override;

private:

};

// ------------------------------------------------------------------------------------------

class ZN_API CXMLReader
	: public IXMLReader
{
public:
	CXMLReader(std::shared_ptr<TiXmlDocument> Document, const TiXmlElement* Element);
	virtual ~CXMLReader();

	// IXMLReader
	std::string GetName() const override;
	std::string GetValue() const override;
	std::string GetAttribute(std::string AttributeName) const override;

	std::shared_ptr<IXMLReader> GetChild(std::string ChildName) const override;
	std::vector<std::shared_ptr<IXMLReader>> GetChilds() const override;

private:
	std::shared_ptr<TiXmlDocument> m_Document;
	const TiXmlElement* m_Element;
};

// ------------------------------------------------------------------------------------------

class ZN_API CXMLWriter
	: public IXMLWriter
{
public:
	CXMLWriter(std::shared_ptr<TiXmlDocument> Document, TiXmlElement* Element);
	virtual ~CXMLWriter();

	// IXMLWriter
	void SetValue(std::string Value) override;
	void AddAttribute(std::string AttributeName, std::string AttributeValue) const override;

	std::shared_ptr<IXMLWriter> CreateChild(std::string ChildName) const override;

	//

	std::shared_ptr<IFile> SaveToFile(std::string Filename) override;

private:
	std::shared_ptr<TiXmlDocument> m_Document;
	TiXmlElement* m_Element;
};