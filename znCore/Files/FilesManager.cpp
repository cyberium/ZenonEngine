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
	if (FileName.empty())
		throw CException("Filename is empty.");

	auto filesStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(GetGamedataStorage());
	if (filesStorageEx == nullptr)
		throw CException("For 'Create' operation USERDATA must supports IznFilesStorageExtended. Filename '%s'.", FileName.c_str());

	try
	{
		return filesStorageEx->Create(FileName);
	}
	catch (const CException& e)
	{
		Log::Error("Error while creating file '%s'.", FileName.c_str());
		Log::Error("--->'%s'", e.MessageCStr());
		throw;
	}

	return nullptr;
}

std::shared_ptr<IFile> CFilesManager::Open(std::string FileName, EFileAccessType FileAccessType) const
{
	if (FileName.empty())
		throw CException("Filename is empty.");

	for (const auto& fs : m_Storages)
		if (fs.second->IsExists(FileName))
			return fs.second->Open(FileName);

	Log::Warn("File '%s' not exists.", FileName.c_str());
	return nullptr;
}

void CFilesManager::Delete(std::string FileName) const
{
	if (FileName.empty())
		throw CException("Filename is empty.");

	auto filesStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(GetGamedataStorage());
	if (filesStorageEx == nullptr)
		throw CException("For 'Delete' operation USERDATA must supports IznFilesStorageExtended. Filename '%s'.", FileName.c_str());

	try
	{
		filesStorageEx->Delete(FileName);
	}
	catch (const CException& e)
	{
		Log::Error("Error while deleting file '%s'.", FileName.c_str());
		Log::Error("--->'%s'", e.MessageCStr());
		throw;
	}
}

size_t CFilesManager::GetFileSize(std::string FileName) const
{
	if (FileName.empty())
		throw CException("Filename is empty.");

	for (const auto& fs : m_Storages)
		if (fs.second->IsExists(FileName))
			return fs.second->GetSize(FileName);

	Log::Warn("File '%s' not exists.", FileName.c_str());
	return 0;
}

bool CFilesManager::IsFileExists(std::string FileName) const
{
	if (FileName.empty())
		throw CException("Filename is empty.");

	for (const auto& fs : m_Storages)
		if (fs.second->IsExists(FileName))
			return true;

	return false;
}

void CFilesManager::AddStorage(EFilesStorageType FilesStorageType, std::shared_ptr<IznFilesStorage> Storage)
{
	if (FilesStorageType == EFilesStorageType::USERDATA)
		if (auto gameDataStorage = GetStorage(EFilesStorageType::USERDATA))
			throw CException("Unable to register second USERDATA storage.");

	m_Storages.push_back(std::make_pair(FilesStorageType, Storage));
}

void CFilesManager::RemoveStorage(std::shared_ptr<IznFilesStorage> Storage)
{
	const auto& it = std::find_if(m_Storages.begin(), m_Storages.end(), [Storage](const std::pair<EFilesStorageType, std::shared_ptr<IznFilesStorage>>& StoragePair) -> bool {
		return StoragePair.second == Storage;
	});
	if (it == m_Storages.end())
	{
		Log::Warn("Unable to remove file storage, because not found.");
		return;
	}
	m_Storages.erase(it);
}

std::shared_ptr<IznFilesStorage> CFilesManager::GetStorage(EFilesStorageType FilesStorageType) const
{
	const auto& it = std::find_if(m_Storages.begin(), m_Storages.end(), [FilesStorageType](const std::pair<EFilesStorageType, std::shared_ptr<IznFilesStorage>>& StoragePair) -> bool {
		return StoragePair.first == FilesStorageType;
	});
	if (it == m_Storages.end())
		return nullptr;
	return it->second;
}



//
// Private
//
std::shared_ptr<IznFilesStorage> CFilesManager::GetGamedataStorage() const
{
	auto gamedataStorage = GetStorage(EFilesStorageType::USERDATA);
	if (gamedataStorage == nullptr)
		throw CException("USERDATA storage doen't found.");
	return gamedataStorage;
}
