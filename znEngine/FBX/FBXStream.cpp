#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXStream.h"

CFBXStream::CFBXStream(std::shared_ptr<IFile> File, FbxIOPluginRegistry* PluginRegistry)
	: m_File(File)
	, m_PluginRegistry(PluginRegistry)
{
}

CFBXStream::~CFBXStream()
{
}

FbxStream::EState CFBXStream::GetState()
{
	return EState::eOpen;
}

bool CFBXStream::Open(void * pStreamData)
{
	return true;
}

bool CFBXStream::Close()
{
	return true;
}

bool CFBXStream::Flush()
{
	return true;
}

int CFBXStream::Write(const void * pData, int pSize)
{
	m_File->writeBytes(pData, pSize);
	return 0;
}

int CFBXStream::Read(void * pData, int pSize) const
{
	if (m_File->readBytes(pData, pSize))
		return pSize;

	return 0;
}

int CFBXStream::GetReaderID() const
{
	int fileFormat = -1;
	if (!m_PluginRegistry->DetectReaderFileFormat(m_File->Name().c_str(), fileFormat))
		throw CException("CFBXStream: GetReaderID: Error while determinate file format. Filename '%s'", m_File->Name().c_str());
	return fileFormat;
}

int CFBXStream::GetWriterID() const
{
	int fileFormat = -1;
	if (!m_PluginRegistry->DetectWriterFileFormat(m_File->Name().c_str(), fileFormat))
		throw CException("CFBXStream: GetWriterID: Error while determinate file format. Filename '%s'", m_File->Name().c_str());
	return fileFormat;
}

void CFBXStream::Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos)
{
	switch (pSeekPos)
	{
		case FbxFile::ESeekPos::eBegin:
		{
			m_File->seek(pOffset);
		}
		break;

		case FbxFile::ESeekPos::eCurrent:
		{
			m_File->seekRelative(pOffset);
		}
		break;

		case FbxFile::ESeekPos::eEnd:
		{
			m_File->seek(m_File->getSize() - 1);
		}
		break;

		default:
		{
			_ASSERT_EXPR(false, L"Not impltemented.");
		}
		break;
	}
}

long CFBXStream::GetPosition() const
{
	return m_File->getPos();
}

void CFBXStream::SetPosition(long pPosition)
{
	m_File->seek(pPosition);
}

int CFBXStream::GetError() const
{
	return 0;
}

void CFBXStream::ClearError()
{
}

#endif
