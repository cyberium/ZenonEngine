#include "stdafx.h"

// General
#include "FilesManager.h"

// Additional
#include "File.h"

CFilesManager::CFilesManager(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CFilesManager::~CFilesManager()
{
	m_BaseManager.RemoveManager<IFilesManager>();
}



//
// IFilesManager
//
std::shared_ptr<IFile> CFilesManager::Create(std::string FileName) const
{
	return GetGamedataStorage()->Create(FileName);
}

std::shared_ptr<IFile> CFilesManager::Open(std::string FileName, EFileAccessType FileAccessType) const
{
	for (const auto& fs : m_Storages)
		if (fs.second->IsFileExists(FileName))
			return fs.second->OpenFile(FileName);

	Log::Warn("FilesManager: File '%s' not found.", FileName.c_str());
	return nullptr;
}

size_t CFilesManager::GetFileSize(std::string FileName) const
{
	for (const auto& fs : m_Storages)
		if (fs.second->IsFileExists(FileName))
			return fs.second->GetFileSize(FileName);

	return 0;
}

bool CFilesManager::IsFileExists(std::string FileName) const
{
	for (const auto& fs : m_Storages)
		if (fs.second->IsFileExists(FileName))
			return true;

	return false;
}

void CFilesManager::AddStorage(EFilesStorageType FilesStorageType, std::shared_ptr<IFilesStorage> Storage)
{
	if (FilesStorageType == EFilesStorageType::GAMEDATA)
		if (auto gameDataStorage = GetStorage(EFilesStorageType::GAMEDATA))
			throw CException("FilesManager: Unable to register second GAMEDATA storage.");

	m_Storages.push_back(std::make_pair(FilesStorageType, Storage));
}

void CFilesManager::RemoveStorage(std::shared_ptr<IFilesStorage> Storage)
{
	const auto& it = std::find_if(m_Storages.begin(), m_Storages.end(), [Storage](const std::pair<EFilesStorageType, std::shared_ptr<IFilesStorage>>& StoragePair) -> bool {
		return StoragePair.second == Storage;
	});
	if (it == m_Storages.end())
	{
		Log::Warn("FilesManager: Unable to remove file storage, because not found.");
		return;
	}
	m_Storages.erase(it);
}

std::shared_ptr<IFilesStorage> CFilesManager::GetStorage(EFilesStorageType FilesStorageType) const
{
	const auto& it = std::find_if(m_Storages.begin(), m_Storages.end(), [FilesStorageType](const std::pair<EFilesStorageType, std::shared_ptr<IFilesStorage>>& StoragePair) -> bool {
		return StoragePair.first == FilesStorageType;
	});
	if (it == m_Storages.end())
		return nullptr;
	return it->second;
}



//
// Private
//
std::shared_ptr<IFilesStorage> CFilesManager::GetGamedataStorage() const
{
	auto gamedataStorage = GetStorage(EFilesStorageType::GAMEDATA);
	if (gamedataStorage == nullptr)
		throw CException("FilesManager: GAMEDATA storage doen't found.");
	return gamedataStorage;
}
