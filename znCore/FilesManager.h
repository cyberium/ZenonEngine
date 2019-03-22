#pragma once


class CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	virtual ~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Open(cstring _fileName) override final;
	size_t                 GetFileSize(cstring _fileName) override final;
	bool                   IsFileExists(cstring _fileName) override final;

	void RegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) override final;
	void UnRegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) override final;

private:
	std::vector<std::shared_ptr<IFilesStorage>> m_Storages;
};