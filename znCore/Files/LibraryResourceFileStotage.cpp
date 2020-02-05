#include "stdafx.h"

// General
#include "LibraryResourceFileStotage.h"

// Additional
#include "File.h"

CLibraryResourceFileStotage::CLibraryResourceFileStotage(HMODULE HModule, Priority Priority)
    : m_HModule(HModule)
    , m_Priority(Priority)
{
}

CLibraryResourceFileStotage::~CLibraryResourceFileStotage()
{
}

std::shared_ptr<IFile> CLibraryResourceFileStotage::OpenFile(std::string Filename, EFileAccessType FileAccessType)
{
    HRSRC hResource = FindResource(m_HModule, Resources::ConvertString(Filename).c_str(), RT_RCDATA);
    if (hResource != NULL)
    {
        HGLOBAL hResourceData = LoadResource(m_HModule, hResource);
        DWORD resourceSize = SizeofResource(m_HModule, hResource);

        if (hResourceData && resourceSize > 0)
        {
            LPVOID resourceData = LockResource(hResourceData);

            std::shared_ptr<CFile> file = std::make_shared<CFile>(Filename);
            CByteBuffer& byteBuffer = file->GetByteBuffer();

			byteBuffer = std::move(CByteBuffer(resourceData, resourceSize));
            
            return file;
        }
    }

    return nullptr;
}

bool CLibraryResourceFileStotage::SaveFile(std::shared_ptr<IFile> File)
{
	_ASSERT_EXPR(false, L"CLibraryResourceFileStotage: Unable to save file to this file storage.");
	return false;
}

size_t CLibraryResourceFileStotage::GetFileSize(std::string Filename)
{
    HRSRC hResource = FindResource(m_HModule, Resources::ConvertString(Filename).c_str(), RT_RCDATA);
    if (hResource != NULL)
    {
        return static_cast<size_t>(SizeofResource(m_HModule, hResource));
    }

    return 0;
}

bool CLibraryResourceFileStotage::IsFileExists(std::string Filename)
{
    return FindResource(m_HModule, Resources::ConvertString(Filename).c_str(), RT_RCDATA) != NULL;
}

IFilesStorageEx::Priority CLibraryResourceFileStotage::GetPriority() const
{
    return m_Priority;
}
