#pragma once

class ZN_API CChunkedFile
{
public:
	enum class ZN_API EFileMode
	{

	};

	typedef std::map<std::string, std::vector<std::shared_ptr<IByteBuffer>>> ChunksMap;

private:
	struct ZN_API SChunkLink
	{
		char   CHID[4];
		uint32 CHOF;
		uint32 CHSZ;
	};

	struct ZN_API SLinkAndBuffer
	{
		SChunkLink Link;
		std::shared_ptr<IByteBuffer> Buffer;
	};

public:
	CChunkedFile();
	virtual ~CChunkedFile();

	void Open(const std::shared_ptr<IByteBuffer>& ByteBuffer);
	void Save(std::shared_ptr<IFile> File);

	// CChunkedFile (shared)
	void                                       AddChunk(const std::string& ChunkName, const std::shared_ptr<IByteBuffer>& ByteBuffer);
	std::shared_ptr<IByteBuffer>               GetChunk(const std::string& ChunkName);
	std::vector<std::shared_ptr<IByteBuffer>>  GetChunks(const std::string& ChunkName);
	
private:
	ChunksMap                                  m_Chunks;
};