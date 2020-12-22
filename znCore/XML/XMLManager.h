#pragma once

class ZN_API CXMLManager
{
public:
	CXMLManager(const IBaseManager& BaseManager);
	virtual ~CXMLManager();

	// IXML
	std::shared_ptr<IXMLReader> CreateReaderFromString(const std::string& String);
	std::shared_ptr<IXMLReader> CreateReaderFromFile(const std::string& FileName);
	std::shared_ptr<IXMLReader> CreateReaderFromFile(std::shared_ptr<IFile> File);

	std::shared_ptr<IXMLWriter> CreateDocument();
	std::shared_ptr<IXMLWriter> CreateWriter(const std::string& NodeName);

	std::string                 SaveWriterToString(const std::shared_ptr<IXMLWriter>& Writer);
	std::shared_ptr<IFile>      SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName);
	std::shared_ptr<IFile>      SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::shared_ptr<IFile>& File);

private:
	const IBaseManager& m_BaseManager;
};

