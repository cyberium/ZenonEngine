#pragma once

class ZN_API CLocalFilesStorage 
	: public IFilesStorage
{
public:
	CLocalFilesStorage(std::string _path);
	virtual ~CLocalFilesStorage();

	// IFilesStorage
	std::shared_ptr<IFile>                          Create(std::string FileName) override;
	std::shared_ptr<IFile>                          OpenFile(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) override;
	bool                                            SaveFile(std::shared_ptr<IFile> File) override;
	size_t                                          GetFileSize(std::string FileName) const override;
	bool                                            IsFileExists(std::string FileName) const override;
	std::vector<std::string>                        GetAllFilesInFolder(std::string Directory, std::string Extension = "") const override;
private:
	const std::string       m_Path;
};
