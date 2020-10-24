#pragma once

class ZN_API CFilesManager 
	: public IFilesManager
{
public:
	CFilesManager(IBaseManager& BaseManager);
	virtual ~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Create(std::string FileName) const override;
	std::shared_ptr<IFile> Open(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) const override;
	size_t GetFileSize(std::string FileName) const override;
	bool IsFileExists(std::string FileName) const override;

	void AddStorage(EFilesStorageType FilesStorageType, std::shared_ptr<IFilesStorage> Storage) override;
	void RemoveStorage(std::shared_ptr<IFilesStorage> Storage) override;
	std::shared_ptr<IFilesStorage> GetStorage(EFilesStorageType FilesStorageType) const override;

private:
	std::shared_ptr<IFilesStorage> GetGamedataStorage() const;

private:
	IBaseManager& m_BaseManager;
	std::vector<std::pair<EFilesStorageType, std::shared_ptr<IFilesStorage>>> m_Storages;
};