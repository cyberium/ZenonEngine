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


//
// IFilesStorage
//
std::shared_ptr<IFile> CLocalFilesStorage::OpenFile(std::string FileName, EFileAccessType FileAccessType)
{
	std::shared_ptr<CFile> file = std::make_shared<CFile>(FileName);
	CByteBuffer& byteBuffer = file->GetByteBuffer();

	std::ifstream stream;
	stream.open(std::string(m_Path + file->Path_Name()), std::ios::binary);

	if (stream.is_open())
	{
		// Filesize
		stream.seekg(0, stream.end);
		uint32_t fileSize = uint32_t(stream.tellg());
		stream.seekg(0, stream.beg);

		// Check filesize
		if (fileSize == 0)
		{
			char buff[256];
			sprintf_s(buff, "File[%s]: Is empty!", file->Path_Name().c_str());
			_ASSERT_EXPR(false, buff);
			return nullptr;
		}

		// Read data
		byteBuffer.Allocate(fileSize + 1);
		stream.read((char*)&byteBuffer.getDataEx()[0], fileSize);
		byteBuffer.getDataEx()[fileSize] = '\0';
		byteBuffer.SetFilled();
		
		std::streamsize readedBytes = stream.gcount();
		if (readedBytes < fileSize)
		{
			char buff[256];
			sprintf_s(buff, "File[%s]: Stream reading error. Readed [%d], filesize [%d]", file->Path_Name().c_str(), static_cast<int64>(readedBytes), fileSize);
			_ASSERT_EXPR(false, buff);
			return nullptr;
		}

	}
	else
	{
		_ASSERT_EXPR(false, L"Unable to open file.");
	}

	// Close stream
	stream.close();
	stream.clear();

	return file;
}

bool CLocalFilesStorage::SaveFile(std::shared_ptr<IFile> File)
{
	std::ofstream stream;
	stream.open(std::string(m_Path + File->Path_Name()), std::ios::binary);

	if (stream.is_open())
	{
		stream.write((const char*)File->getData(), File->getSize());
		stream.flush();
	}
	else
	{
		_ASSERT_EXPR(false, L"Unable to open file.");
		return false;
	}

	stream.close();

	return true;
}

size_t CLocalFilesStorage::GetFileSize(std::string FileName)
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
	{
		_ASSERT_EXPR(false, L"Unable to open file.");
	}

	stream.clear();
	stream.close();

	return fileSize;
}

bool CLocalFilesStorage::IsFileExists(std::string FileName)
{
	std::ifstream stream;
	stream.open(std::string(m_Path + FileName), std::ios::binary);

	bool isOpen = stream.is_open();

	stream.clear();
	stream.close();

	return isOpen;
}



//
// IFilesStorageEx
//
IFilesStorageEx::Priority CLocalFilesStorage::GetPriority() const
{
	return m_Priority;
}
