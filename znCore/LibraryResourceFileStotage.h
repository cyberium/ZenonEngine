#pragma once

class CLibraryResourceFileStotage : public IFilesStorage, public IFilesStorageEx
{
public:
    CLibraryResourceFileStotage(HMODULE HModule, Priority Priority = PRIOR_HIGHEST);
    virtual ~CLibraryResourceFileStotage();

    // IFilesStorage
    std::shared_ptr<IFile>  CreateFile(const std::string& _name) override;
    size_t                  GetFileSize(const std::string& _name) override;
    bool                    IsFileExists(const std::string& _name) override;

    // IFilesStorageEx
    Priority                GetPriority() const;

private:
    const HMODULE       m_HModule;
    const Priority      m_Priority;
};
