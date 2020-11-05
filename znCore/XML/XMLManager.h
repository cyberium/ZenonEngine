#pragma once

class ZN_API CXMLManager
{
public:
	CXMLManager(IBaseManager& BaseManager);
	virtual ~CXMLManager();

	// IXML
	std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File);
	std::shared_ptr<IXMLWriter> CreateWriter();
	std::shared_ptr<IXMLWriter> CreateWriter(const std::string& NodeName);

	std::shared_ptr<IFile> SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName);
	std::shared_ptr<IFile> SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::shared_ptr<IFile>& File);

private:
	IBaseManager& m_BaseManager;
};

