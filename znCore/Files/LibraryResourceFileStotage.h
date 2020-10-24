#pragma once

class ZN_API CLibraryResourceFileStotage 
	: public IFilesStorage
{
public:
    CLibraryResourceFileStotage(HMODULE HModule);
    virtual ~CLibraryResourceFileStotage();

    // IFilesStorage
	std::shared_ptr<IFile>  Create(std::string FileName) override;
    std::shared_ptr<IFile>  OpenFile(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) override;
	bool                    SaveFile(std::shared_ptr<IFile> File) override;
    size_t                  GetFileSize(std::string FileName) const override;
    bool                    IsFileExists(std::string FileName) const override;

private:
    const HMODULE       m_HModule;
};
