#include "stdafx.h"

// General
#include "ChunkedFile.h"

// Consts
const uint32 cActualVersion = 0x01;
// Consts




CChunkedFile::CChunkedFile()

{}

CChunkedFile::~CChunkedFile()
{
}



//
// CChunkedFile
//
void CChunkedFile::Open(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	_ASSERT_EXPR(ByteBuffer != nullptr, L"Source file must not be null.");
	_ASSERT_EXPR(ByteBuffer->getPos() == 0, L"Source file must be untouched for chunk reading.");

	Log::Info("Chunks Open BEGIN.");

	m_Chunks.clear();

	uint32 version = 0;
	ByteBuffer->read(&version);
	_ASSERT_EXPR(version == cActualVersion, L"Chunked file version mismatch.");
	Log::Info("  Chunks file version = '0x%X'.", version);

	uint32 linksCnt = 0;
	ByteBuffer->read(&linksCnt);
	_ASSERT_EXPR(linksCnt != 0, L"Incorrect chunks links count.");
	Log::Info("  Chunks count = '%s'.", linksCnt);

	for (size_t i = 0; i < linksCnt; i++)
	{
		SChunkLink link = { 0 };
		ByteBuffer->read(&link);

		_ASSERT_EXPR(link.CHOF < ByteBuffer->getSize(), L"Incorrect offset of chunk.");
		_ASSERT_EXPR(link.CHSZ < ByteBuffer->getSize(), L"Incorrect size of chunk.");
		_ASSERT_EXPR((link.CHOF + link.CHSZ) < ByteBuffer->getSize(), L"Incorrect offset and size to chunk.");

		char dbgName[5] = { 0 };
		memcpy_s(&dbgName, 4, link.CHID, 4);

		Log::Info("    Chunk '%s': Offset '%d', size '%d'", dbgName, link.CHOF, link.CHSZ);
		AddChunk(dbgName, std::make_shared<CByteBufferOnlyPointer>(ByteBuffer->getData() + link.CHOF, link.CHSZ));
	}

	Log::Info("Chunks Open END.");
}

void CChunkedFile::Save(std::shared_ptr<IFile> File)
{
	_ASSERT(File != nullptr);
	_ASSERT_EXPR(File->getPos() == 0 && File->getSize() == 0, L"Destination file must be untouched for chunk writing.");

	File->write(&cActualVersion);

	uint32 chunksCategotyCnt = static_cast<uint32>(m_Chunks.size());
	File->write(&chunksCategotyCnt);

	std::vector<SLinkAndBuffer> linksAndBuffers;
	uint32 lastOffeset = 0;

	for (const auto& it : m_Chunks)
	{
		for (const auto& it2 : it.second)
		{
			SLinkAndBuffer linkAndBuffer;

			memcpy_s(linkAndBuffer.Link.CHID, 4, it.first.c_str(), 4);
			linkAndBuffer.Link.CHOF = lastOffeset;
			linkAndBuffer.Link.CHSZ = it2->getSize();
			lastOffeset += linkAndBuffer.Link.CHSZ;

			linkAndBuffer.Buffer = it2;

			linksAndBuffers.push_back(linkAndBuffer);
		}
	}

	for (const auto& it : linksAndBuffers)
	{
		File->write(&it.Link);
	}

	for (const auto& it : linksAndBuffers)
	{
		File->writeBytes(it.Buffer->getData(), it.Buffer->getSize());
	}
}

void CChunkedFile::AddChunk(const std::string & ChunkName, const void * DataPtr, size_t DataSize)
{
	AddChunk(ChunkName, std::make_shared<CByteBuffer>(DataPtr, DataSize));
}

void CChunkedFile::AddChunk(const std::string & ChunkName, const std::vector<uint8>& Bytes)
{
	AddChunk(ChunkName, std::make_shared<CByteBuffer>(Bytes.data(), Bytes.size()));
}

void CChunkedFile::AddChunk(const std::string & ChunkName, const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	const auto& it = m_Chunks.find(ChunkName);
	if (it != m_Chunks.end())
	{
		_ASSERT(it->second.empty() == false); // unexpected case
		it->second.push_back(ByteBuffer);
	}
	else
	{
		m_Chunks.insert(std::make_pair(ChunkName, std::vector<std::shared_ptr<IByteBuffer>>({ ByteBuffer })));
	}
}

std::shared_ptr<IByteBuffer> CChunkedFile::GetChunk(const std::string & ChunkName)
{
	const auto& chunksIt = m_Chunks.find(ChunkName);
	if (chunksIt == m_Chunks.end())
	{
		Log::Warn("Chunk '%s' not found.", ChunkName.c_str());
		return nullptr;
	}

	if (chunksIt->second.size() > 1)
		Log::Warn("Chunk '%s' has '%d' instanced in file. First will be returned.", ChunkName.c_str(), chunksIt->second.size());

	// Reset byte buffer
	(*(chunksIt->second.begin()))->seek(0);

	return *(chunksIt->second.begin());
}

std::vector<std::shared_ptr<IByteBuffer>> CChunkedFile::GetChunks(const std::string & ChunkName)
{
	const auto& chunksIt = m_Chunks.find(ChunkName);
	if (chunksIt == m_Chunks.end())
	{
		Log::Warn("Chunk '%s' not found.", ChunkName.c_str());
		return std::vector<std::shared_ptr<IByteBuffer>>();
	}

	// Reset byte buffers
	for (const auto& it : chunksIt->second)
		it->seek(0);

	return chunksIt->second;
}
