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

	bool seek(size_t AbsoluteOffset) override;
	bool seekRelative(intptr_t RelativeOffset) override;

	bool readLine(std::string* String) override;
	bool getText(std::string * String) override;
	bool readBytes(void* Destination, size_t Size) override;
	bool readString(std::string* String) override;

	void writeLine(const std::string& String) override;
	void writeBytes(const void * Source, size_t BytesCount) override;
	void writeDummy(size_t BytesCount) override;
	void insert(size_t Position, const void * DataPtr, size_t DataSize) override;
	void writeString(const std::string& String) override;

private:
	CByteBufferOnlyPointer(CByteBufferOnlyPointer&&) = delete;
	const CByteBufferOnlyPointer& operator=(CByteBufferOnlyPointer&&) = delete;

private:
	const uint8*		m_Data;
	size_t              m_DataSize;
	size_t              m_CurrentPosition;
};
