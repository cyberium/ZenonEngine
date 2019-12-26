#pragma once

class ZN_API CLibraryResourceFileStotage : public IFilesStorage, public IFilesStorageEx
{
public:
    CLibraryResourceFileStotage(HMODULE HModule, Priority Priority = Priority::PRIOR_HIGH);
    virtual ~CLibraryResourceFileStotage();

    // IFilesStorage
    std::shared_ptr<IFile>  OpenFile(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) override;
	bool                    SaveFile(std::shared_ptr<IFile> File) override;
    size_t                  GetFileSize(std::string FileName) override;
    bool                    IsFileExists(std::string FileName) override;

    // IFilesStorageEx
    Priority                GetPriority() const;

private:
    const HMODULE       m_HModule;
    const Priority      m_Priority;
};
