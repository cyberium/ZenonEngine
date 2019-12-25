#pragma once

class ZN_API CLocalFilesStorage : public IFilesStorage, public IFilesStorageEx
{
public:
	CLocalFilesStorage(std::string _path, Priority _priority = Priority::PRIOR_NORMAL);
	virtual ~CLocalFilesStorage();

	// IFilesStorage
	std::shared_ptr<IFile>  OpenFile(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) override;
	bool                    SaveFile(std::shared_ptr<IFile> File) override;
	size_t                  GetFileSize(std::string FileName) override;
	bool                    IsFileExists(std::string FileName) override;

	// IFilesStorageEx
	Priority GetPriority() const;

private:
	const std::string       m_Path;
	const Priority          m_Priority;
};
