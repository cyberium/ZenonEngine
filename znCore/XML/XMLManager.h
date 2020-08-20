#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLManager
{
public:
	CXMLManager();
	virtual ~CXMLManager();

	// IXML
	static std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File);
	static std::shared_ptr<IXMLWriter> CreateWriter();
	static std::shared_ptr<IXMLWriter> CreateWriter(const std::string& NodeName);

	static std::shared_ptr<IFile> SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName);
private:

};

void CheckTinyXMLError(const std::shared_ptr<TiXmlDocument>& TiniXMLDocument);
