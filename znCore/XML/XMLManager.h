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

void CheckTinyXMLError(const TiXmlDocument* TiniXMLDocument);
