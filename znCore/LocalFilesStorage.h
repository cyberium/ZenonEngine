#pragma once

class CLocalFilesStorage : public IFilesStorage, public IFilesStorageEx
{
public:
	CLocalFilesStorage(std::string _path, Priority _priority = PRIOR_NORMAL);
	virtual ~CLocalFilesStorage();

	// IFilesStorage
	std::shared_ptr<IFile>  CreateFile(cstring _name) override;
	size_t                  GetFileSize(cstring _name) override;
	bool                    IsFileExists(cstring _name) override;

	// IFilesStorageEx
	Priority GetPriority() const;

private:
	const std::string       m_Path;
	const Priority          m_Priority;
};
