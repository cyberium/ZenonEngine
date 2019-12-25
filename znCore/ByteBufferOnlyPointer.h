#pragma once

class ZN_API CByteBufferOnlyPointer 
	: public IByteBuffer
{
public:
	CByteBufferOnlyPointer(const CByteBufferOnlyPointer& _other);
	CByteBufferOnlyPointer(const uint8_t* DataPtr, size_t DataSize);
	~CByteBufferOnlyPointer();

	CByteBufferOnlyPointer& operator=(const CByteBufferOnlyPointer& _other);

	// IByteBuffer
	size_t getSize() const override { return m_DataSize; }
	size_t getPos() const override { return m_CurrentPosition; }
	const uint8* getData() const override { return m_Data; }
	const uint8* getDataFromCurrent() const override { return &m_Data[m_CurrentPosition]; }
	bool isEof() const override { return m_IsEOF; }

	void seek(size_t _bufferOffsetAbsolute) override;
	void seekRelative(intptr_t _bufferOffsetRelative) override;

	bool readLine(std::string* _string) override;
	bool readBytes(void* _destination, size_t _size) override;
	void readString(std::string* _string) override;

	void writeLine(std::string String) override;
	void writeBytes(const void * Source, size_t BytesCount) override;
	void writeString(std::string String) override;

private:
	bool                m_IsEOF;
	const uint8*		m_Data;
	size_t              m_DataSize;
	size_t              m_CurrentPosition;
};
