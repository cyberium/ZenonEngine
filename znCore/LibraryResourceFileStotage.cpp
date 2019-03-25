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

std::shared_ptr<IFile> CLibraryResourceFileStotage::CreateFile(const std::string& _name)
{
    HRSRC hResource = FindResource(m_HModule, Resources::ConvertString(_name).c_str(), RT_RCDATA);
    if (hResource != NULL)
    {
        HGLOBAL hResourceData = LoadResource(m_HModule, hResource);
        DWORD resourceSize = SizeofResource(m_HModule, hResource);

        if (hResourceData && resourceSize > 0)
        {
            LPVOID resourceData = LockResource(hResourceData);

            std::shared_ptr<CFile> file = std::make_shared<CFile>("<empty>", _name);
            CByteBuffer& byteBuffer = file->GetByteBuffer();

            byteBuffer.Allocate(resourceSize + 1);
            memcpy_s(&byteBuffer.getDataEx()[0], resourceSize, resourceData, resourceSize);
            byteBuffer.SetFilled();
            byteBuffer.getDataEx()[resourceSize] = '\0';

            return file;
        }
    }

    return nullptr;
}

size_t CLibraryResourceFileStotage::GetFileSize(const std::string& _name)
{
    HRSRC hResource = FindResource(m_HModule, Resources::ConvertString(_name).c_str(), RT_RCDATA);
    if (hResource != NULL)
    {
        return static_cast<size_t>(SizeofResource(m_HModule, hResource));
    }

    return 0;
}

bool CLibraryResourceFileStotage::IsFileExists(const std::string& _name)
{
    return FindResource(m_HModule, Resources::ConvertString(_name).c_str(), RT_RCDATA) != NULL;
}

IFilesStorageEx::Priority CLibraryResourceFileStotage::GetPriority() const
{
    return m_Priority;
}
