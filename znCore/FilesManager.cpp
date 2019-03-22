#include "stdafx.h"

// General
#include "FilesManager.h"

// Additional
#include "BaseManager.h"

CFilesManager::CFilesManager()
{
}

CFilesManager::~CFilesManager()
{
	DelManager<IFilesManager>();
}

std::shared_ptr<IFile> CFilesManager::Open(cstring _fileName)
{
	for (const auto& fs : m_Storages)
	{
		if (fs->IsFileExists(_fileName))
			return fs->CreateFile(_fileName);
	}

	Log::Error("[CFilesManager]: File '%s' not found.", _fileName.c_str());
	return nullptr;
}

size_t CFilesManager::GetFileSize(cstring _fileName)
{
	for (const auto& fs : m_Storages)
	{
		size_t fileSize = fs->GetFileSize(_fileName);
		if (fileSize != 0)
			return fileSize;
	}

	return 0;
}

bool CFilesManager::IsFileExists(cstring _fileName)
{
	for (const auto& fs : m_Storages)
	{
		bool isFileExists = fs->IsFileExists(_fileName);
		if (isFileExists)
			return true;
	}

	return false;
}

void CFilesManager::RegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage)
{
	std::shared_ptr<IFilesStorageEx> storageEx = std::dynamic_pointer_cast<IFilesStorageEx, IFilesStorage>(_storage);
	assert1(storageEx);

	m_Storages.push_back(_storage);

	std::sort(m_Storages.begin(), m_Storages.end(),
		[](const std::shared_ptr<IFilesStorage>& a, const std::shared_ptr<IFilesStorage>& b)
		{
			std::shared_ptr<const IFilesStorageEx> aEx = std::dynamic_pointer_cast<const IFilesStorageEx, const IFilesStorage>(a);
			std::shared_ptr<const IFilesStorageEx> bEx = std::dynamic_pointer_cast<const IFilesStorageEx, const IFilesStorage>(b);
			return aEx->GetPriority() > bEx->GetPriority();
		}
	);
}

void CFilesManager::UnRegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage)
{
	// Do nothing
}
