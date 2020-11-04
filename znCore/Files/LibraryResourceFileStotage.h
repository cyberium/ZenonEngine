#pragma once

class ZN_API CLibraryResourceFileStotage 
	: public IznFilesStorage
{
public:
    CLibraryResourceFileStotage(HMODULE HModule);
    virtual ~CLibraryResourceFileStotage();

    // IznFilesStorage
    std::shared_ptr<IFile>                          Open(std::string FileName) override;
    size_t                                          GetSize(std::string FileName) const override;
    bool                                            IsExists(std::string FileName) const override;

private:
    const HMODULE m_HModule;
};
