#include "stdafx.h"

// General
#include "LocalFilesStorage.h"

// Additional
#include "File.h"

CLocalFilesStorage::CLocalFilesStorage(std::string _path)
	: m_Path(_path)
{}

CLocalFilesStorage::~CLocalFilesStorage()
{}



//
// IFilesStorage
//
std::shared_ptr<IFile> CLocalFilesStorage::Create(std::string FileName)
{
	if (IsFileExists(FileName))
		throw CException("File '%s' already exists.", FileName.c_str());
	return MakeShared(CFile, FileName, shared_from_this());
}

std::shared_ptr<IFile> CLocalFilesStorage::OpenFile(std::string FileName, EFileAccessType FileAccessType)
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
		throw CException("File '%s' stream reading error. Readed [%d]. Filesize [%d]", file->Path_Name().c_str(), static_cast<int64>(readedBytes), fileSize);
	
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
		throw CException("Unable to open file '%s'.", File->Path_Name().c_str());

	stream.close();

	return true;
}

size_t CLocalFilesStorage::GetFileSize(std::string FileName) const
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

bool CLocalFilesStorage::IsFileExists(std::string FileName) const
{
	std::ifstream stream;
	stream.open(std::string(m_Path + FileName), std::ios::binary);

	bool isOpen = stream.is_open();

	stream.clear();
	stream.close();

	return isOpen;
}
