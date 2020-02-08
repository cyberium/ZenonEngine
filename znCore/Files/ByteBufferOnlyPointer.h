#pragma once

class ZN_API CByteBufferOnlyPointer 
	: public IByteBuffer
{
public:
	CByteBufferOnlyPointer(const CByteBufferOnlyPointer& _other);
	CByteBufferOnlyPointer(const void* DataPtr, size_t DataSize);
	virtual ~CByteBufferOnlyPointer();

	const CByteBufferOnlyPointer& operator=(const CByteBufferOnlyPointer& _other);

	// IByteBuffer
	size_t getSize() const override { return m_DataSize; }
	size_t getPos() const override { return m_CurrentPosition; }
	const uint8* getData() const override { return m_Data; }
	const uint8* getDataFromCurrent() const override { return &m_Data[m_CurrentPosition]; }
	uint8* getDataEx() override { return nullptr; }
	uint8* getDataFromCurrentEx() override { return nullptr; }
	bool isEof() const override { return m_CurrentPosition >= m_DataSize; }

	void seek(size_t AbsoluteOffset) override;
	void seekRelative(intptr_t RelativeOffset) override;

	bool readLine(std::string* _string) override;
	bool readBytes(void* _destination, size_t _size) override;
	void readString(std::string* _string) override;

	void writeLine(std::string String) override;
	void writeBytes(const void * Source, size_t BytesCount) override;
	void writeDummy(size_t BytesCount) override;
	void insert(size_t Position, const void * DataPtr, size_t DataSize) override;
	void writeString(std::string String) override;

private:
	CByteBufferOnlyPointer(CByteBufferOnlyPointer&&) = delete;
	const CByteBufferOnlyPointer& operator=(CByteBufferOnlyPointer&&) = delete;

private:
	const uint8*		m_Data;
	size_t              m_DataSize;
	size_t              m_CurrentPosition;
};
