#include "stdafx.h"

// General
#include "LibraryResourceFileStotage.h"

// Additional
#include "File.h"

CLibraryResourceFileStotage::CLibraryResourceFileStotage(HMODULE HModule)
    : m_HModule(HModule)
{}

CLibraryResourceFileStotage::~CLibraryResourceFileStotage()
{
}



//
// IFilesStorage
//
std::shared_ptr<IFile> CLibraryResourceFileStotage::Create(std::string FileName)
{
	throw CException("Operation not supported.");
}

std::shared_ptr<IFile> CLibraryResourceFileStotage::OpenFile(std::string Filename, EFileAccessType FileAccessType)
{
    HRSRC hResource = FindResource(m_HModule, Resources::utf8_to_utf16(Filename).c_str(), RT_RCDATA);
    if (hResource == NULL)
		throw CException("Unable to open find file '%s'.", Filename.c_str());

    HGLOBAL hResourceData = LoadResource(m_HModule, hResource);
	if (hResourceData == NULL)
		throw CException("Unable to load file '%s' resource.", Filename.c_str());

    DWORD resourceSize = SizeofResource(m_HModule, hResource);
	if (resourceSize == 0)
		throw CException("File '%s' is empty.", Filename.c_str());

    LPVOID resourceData = LockResource(hResourceData);

    std::shared_ptr<CFile> file = MakeShared(CFile, Filename, shared_from_this());
    CByteBuffer& byteBuffer = file->GetByteBuffer();
	byteBuffer = std::move(CByteBuffer(resourceData, resourceSize));
            
    return file;
}

bool CLibraryResourceFileStotage::SaveFile(std::shared_ptr<IFile> File)
{
	throw CException("Operation not supported.");
}

size_t CLibraryResourceFileStotage::GetFileSize(std::string Filename) const
{
    HRSRC hResource = FindResource(m_HModule, Resources::utf8_to_utf16(Filename).c_str(), RT_RCDATA);
    if (hResource == NULL)
		throw CException("Unable to open file '%s'.", Filename.c_str());

	return static_cast<size_t>(SizeofResource(m_HModule, hResource));
}

bool CLibraryResourceFileStotage::IsFileExists(std::string Filename) const
{
    return FindResource(m_HModule, Resources::utf8_to_utf16(Filename).c_str(), RT_RCDATA) != NULL;
}
