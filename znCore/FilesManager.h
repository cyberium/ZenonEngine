#pragma once


class OW_ENGINE_API CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	virtual ~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Open(const std::string& _fileName) override final;
	size_t                 GetFileSize(const std::string& _fileName) override final;
	bool                   IsFileExists(const std::string& _fileName) override final;

	void RegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) override final;
	void UnRegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) override final;

private:
	std::vector<std::shared_ptr<IFilesStorage>> m_Storages;
};