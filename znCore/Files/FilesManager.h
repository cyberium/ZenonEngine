#pragma once

class ZN_API CFilesManager : public IFilesManager
{
public:
	CFilesManager(IBaseManager& BaseManager);
	virtual ~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Open(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) override final;
	size_t GetFileSize(std::string FileName) override final;
	bool IsFileExists(std::string FileName) override final;

	void AddFilesStorage(std::string StorageName, std::shared_ptr<IFilesStorage> Storage) override final;
	void RemoveFilesStorage(std::shared_ptr<IFilesStorage> Storage) override final;
	std::shared_ptr<IFilesStorage> GetFilesStorage(std::string StorageName) const override final;

private:
	IBaseManager& m_BaseManager;
	std::unordered_map<std::string, std::shared_ptr<IFilesStorage>> m_Storages;
};