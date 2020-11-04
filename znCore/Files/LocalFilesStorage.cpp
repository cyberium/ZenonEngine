#include "stdafx.h"

// General
#include "LocalFilesStorage.h"

// Additional
#include "File.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;

CLocalFilesStorage::CLocalFilesStorage(std::string _path)
	: m_Path(_path)
{}

CLocalFilesStorage::~CLocalFilesStorage()
{}



//
// IznFilesStorage
//
std::shared_ptr<IFile> CLocalFilesStorage::Open(std::string FileName)
{
	std::ifstream stream;
	stream.open(std::string(m_Path + FileName), std::ios::binary);

	if (false == stream.is_open())
		throw CException("Unable to open file '%s'.", FileName.c_str());

	// Filesize
	stream.seekg(0, stream.end);
	uint32_t fileSize = uint32_t(stream.tellg());
	stream.seekg(0, stream.beg);

	// Check filesize
	if (fileSize == 0)
		throw CException("File '%s' is empty.", FileName.c_str());

	// Read data
	std::vector<uint8> buffer;
	buffer.resize(fileSize);
	stream.read(reinterpret_cast<char*>(&buffer[0]), fileSize);

	std::shared_ptr<CFile> file = MakeShared(CFile, FileName, shared_from_this());
	CByteBuffer& byteBuffer = file->GetByteBuffer();
	byteBuffer = std::move(CByteBuffer(std::move(buffer)));
		
	std::streamsize readedBytes = stream.gcount();
	if (readedBytes < fileSize)
		throw CException("File '%s' stream reading error. Readed '%d' of '%d'.", file->Path_Name().c_str(), static_cast<int64>(readedBytes), fileSize);
	
	// Close stream
	stream.close();
	stream.clear();

	return file;
}

size_t CLocalFilesStorage::GetSize(std::string FileName) const
{
	std::ifstream stream;
	stream.open(std::string(m_Path + FileName), std::ios::binary);

	size_t fileSize = 0;
	if (stream.is_open())
	{
		stream.seekg(0, stream.end);
		fileSize = static_cast<size_t>(stream.tellg());
		stream.seekg(0, stream.beg);
	}
	else
		throw CException("Unable to open file '%s'.", FileName.c_str());

	stream.clear();
	stream.close();

	return fileSize;
}

bool CLocalFilesStorage::IsExists(std::string FileName) const
{
	std::ifstream stream;
	stream.open(std::string(m_Path + FileName), std::ios::binary);

	bool isOpen = stream.is_open();

	stream.clear();
	stream.close();

	return isOpen;
}



//
// IznFilesStorageExtended
//
std::shared_ptr<IFile> CLocalFilesStorage::Create(std::string FileName)
{
	if (IsExists(FileName))
		throw CException("File '%s' already exists.", FileName.c_str());
	return MakeShared(CFile, FileName, shared_from_this());
}

void CLocalFilesStorage::Delete(std::string FileName) const
{
	if (false == IsExists(FileName))
		throw CException("File '%s' doen't exists in this FileStorage.", FileName.c_str());

	if (std::remove((m_Path + FileName).c_str()) != 0)
		throw CException("Error while deleting file '%s'.", FileName.c_str());
}

void CLocalFilesStorage::Save(std::shared_ptr<IFile> File)
{
	std::string savedPath = m_Path + File->Path_Name();

	std::ofstream stream;
	stream.open(savedPath, std::ios::binary);
	if (false == stream.is_open())
	{
		auto pos = File->Path_Name().find(m_Path);
		if (pos != std::string::npos)
		{
			savedPath = File->Path_Name();
		}
		else
		{
			savedPath = m_Path + File->Name();
			Log::Warn("Unable to save '%s' to '%s'. File will be saved to '%s'.", File->Path_Name().c_str(), m_Path.c_str(), savedPath.c_str());
		}

		stream.open(std::string(savedPath), std::ios::binary);
	}

	if (stream.is_open())
	{
		stream.write((const char*)File->getData(), File->getSize());
		stream.flush();
	}
	else
		throw CException("Unable to open file '%s'.", File->Path_Name().c_str());

	stream.close();
}

std::vector<std::string> CLocalFilesStorage::GetAllFilesInFolder(std::string Directory, std::string Extension) const
{
	std::string fullDirPath = m_Path + Directory;
	bool isExists = fs::exists(fullDirPath);
	if (false == isExists)
		throw CException("Unable to get all files. '%s' is not exists.", fullDirPath.c_str());

	bool isDir = fs::is_directory(fullDirPath);
	if (false == isDir)
		throw CException("Unable to get all files. '%s' is not a directory.", fullDirPath.c_str());


	std::vector<std::string> listOfFiles;

	try
	{
		fs::recursive_directory_iterator iter(fullDirPath), end;

		// Iterate till end
		while (iter != end)
		{
			bool isExtensionAccepted = true;
			if (iter->path().has_extension() && Extension.size() > 0)
			{
				isExtensionAccepted = Utils::ToLower(iter->path().extension().string()) == Utils::ToLower(Extension);
			}

			if (false == fs::is_directory(iter->path()) && isExtensionAccepted)
			{
				listOfFiles.push_back(iter->path().string().substr(m_Path.size()));
			}
			else
			{
				iter.disable_recursion_pending();
			}

			std::error_code ec;
			iter.increment(ec);
			if (ec)
				throw CException("Error while accessing '%s'. Error: '%s'.", iter->path().string().c_str(), ec.message().c_str());
		}
	}
	catch (const std::system_error& e)
	{
		throw CException("Exception '%s'", e.what());
	}

	return listOfFiles;
}
