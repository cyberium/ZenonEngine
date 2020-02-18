#include "stdafx.h"

// General
#include "FilesManager.h"


CFilesManager::CFilesManager(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CFilesManager::~CFilesManager()
{
	m_BaseManager.RemoveManager<IFilesManager>();
}

std::shared_ptr<IFile> CFilesManager::Open(std::string FileName, EFileAccessType FileAccessType)
{
	for (const auto& fs : m_Storages)
	{
		if (fs.second->IsFileExists(FileName))
			return fs.second->OpenFile(FileName);
	}

	Log::Error("[CFilesManager]: File '%s' not found.", FileName.c_str());
	return nullptr;
}

size_t CFilesManager::GetFileSize(std::string FileName)
{
	for (const auto& fs : m_Storages)
	{
		size_t fileSize = fs.second->GetFileSize(FileName);
		if (fileSize != 0)
			return fileSize;
	}

	return 0;
}

bool CFilesManager::IsFileExists(std::string FileName)
{
	for (const auto& fs : m_Storages)
		if (fs.second->IsFileExists(FileName))
			return true;

	return false;
}

void CFilesManager::AddFilesStorage(std::string StorageName, std::shared_ptr<IFilesStorage> Storage)
{
	_ASSERT(std::dynamic_pointer_cast<IFilesStorageEx>(Storage));

	m_Storages.insert(std::make_pair(StorageName, Storage));

	/*std::sort(m_Storages.begin(), m_Storages.end(),
		[](const std::pair<std::string, std::shared_ptr<IFilesStorage>>& a, const std::pair<std::string, std::shared_ptr<IFilesStorage>>& b)
		{
			std::shared_ptr<const IFilesStorageEx> aEx = std::dynamic_pointer_cast<const IFilesStorageEx>(a.second);
			std::shared_ptr<const IFilesStorageEx> bEx = std::dynamic_pointer_cast<const IFilesStorageEx>(b.second);
			return aEx->GetPriority() > bEx->GetPriority();
		}
	);*/
}

void CFilesManager::RemoveFilesStorage(std::shared_ptr<IFilesStorage> Storage)
{
	_ASSERT(false);
}

std::shared_ptr<IFilesStorage> CFilesManager::GetFilesStorage(std::string StorageName) const
{
	return m_Storages.at(StorageName);
}
