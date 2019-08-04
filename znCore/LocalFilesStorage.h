#pragma once

class OW_ENGINE_API CLocalFilesStorage : public IFilesStorage, public IFilesStorageEx
{
public:
	CLocalFilesStorage(std::string _path, Priority _priority = PRIOR_NORMAL);
	virtual ~CLocalFilesStorage();

	// IFilesStorage
	std::shared_ptr<IFile>  CreateFile(const std::string& _name) override;
	size_t                  GetFileSize(const std::string& _name) override;
	bool                    IsFileExists(const std::string& _name) override;

	// IFilesStorageEx
	Priority GetPriority() const;

private:
	const std::string       m_Path;
	const Priority          m_Priority;
};
