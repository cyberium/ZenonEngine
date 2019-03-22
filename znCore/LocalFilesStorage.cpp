#include "stdafx.h"

// General
#include "LocalFilesStorage.h"

// Additional
#include "File.h"

CLocalFilesStorage::CLocalFilesStorage(std::string _path, Priority _priority)
	: m_Path(_path)
	, m_Priority(_priority)
{
}

CLocalFilesStorage::~CLocalFilesStorage()
{
}

std::shared_ptr<IFile> CLocalFilesStorage::CreateFile(cstring _name)
{
	std::shared_ptr<CFile> file = std::make_shared<CFile>(m_Path, _name);
	CByteBuffer& byteBuffer = file->GetByteBuffer();

	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_Path + file->Path_Name()), std::ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		stream.clear();
		stream.close();

		stream.open(std::string(file->Path_Name()), std::ios::binary);

		if (!stream.is_open())
		{
			//Log::Error("File[%s]: Can not open file!", Path_Name().c_str());
			return nullptr;
		}
	}

	// Filesize
	stream.seekg(0, stream.end);
	uint32_t fileSize = uint32_t(stream.tellg());
	stream.seekg(0, stream.beg);

	// Check filesize
	if (fileSize == 0)
	{
		char buff[256];
		sprintf_s(buff, "File[%s]: Is empty!", file->Path_Name().c_str());
		fail2(buff);
		return nullptr;
	}

	// Read data
	byteBuffer.Allocate(fileSize + 1);
	stream.read((char*)&byteBuffer.getDataEx()[0], fileSize);
	byteBuffer.SetFilled();
	byteBuffer.getDataEx()[fileSize] = '\0';

	std::streamsize readedBytes = stream.gcount();
	if (readedBytes < fileSize)
	{
		//memset(&data[0] + readedBytes, 0, fileSize - static_cast<uint32_t>(readedBytes));

		char buff[256];
		sprintf_s(buff, "File[%s]: Stream reading error. Readed [%d], filesize [%d]", file->Path_Name().c_str(), static_cast<int64>(readedBytes), fileSize);
		fail2(buff);
		return nullptr;
	}

	// Close stream
	stream.close();
	stream.clear();

	return file;
}

size_t CLocalFilesStorage::GetFileSize(cstring _name)
{
	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_Path + _name), std::ios::binary);

	// Check stream
	if (!stream.is_open())
		return 0;

	// Filesize
	stream.seekg(0, stream.end);
	size_t fileSize = static_cast<size_t>(stream.tellg());
	stream.seekg(0, stream.beg);

	stream.clear();
	stream.close();

	return fileSize;
}

bool CLocalFilesStorage::IsFileExists(cstring _name)
{
	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_Path + _name), std::ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		stream.clear();
		stream.close();

		// 2nd try to open
		stream.open(_name, std::ios::binary);
		if (!stream.is_open())
			return false;
	}

	stream.clear();
	stream.close();

	return true;
}



//
// IFilesStorageEx
//
IFilesStorageEx::Priority CLocalFilesStorage::GetPriority() const
{
	return m_Priority;
}
