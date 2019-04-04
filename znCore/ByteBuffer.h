#pragma once

class CByteBuffer : public IByteBuffer, public IByteBufferEx
{
public:
	CByteBuffer();
	CByteBuffer(const CByteBuffer& _other);
	CByteBuffer(CByteBuffer&& _other);
	CByteBuffer(size_t _size);
	~CByteBuffer();

	CByteBuffer& operator=(const CByteBuffer& _other);
	CByteBuffer& operator=(CByteBuffer&&  _other);

	// IByteBuffer
	size_t getSize() const override { return m_Data.size(); }
	size_t getPos() const override { return m_CurrentPosition; }
	const uint8* getData() const override { return m_Data.data(); }
	const uint8* getDataFromCurrent() const override { return &m_Data[m_CurrentPosition]; }
	bool isEof() const override { return m_IsEOF; }

    void seek(size_t _bufferOffsetAbsolute) override;
    void seekRelative(intptr_t _bufferOffsetRelative) override;

    bool readLine(std::string* _string) override;
    void readBytes(void* _destination, size_t _size) override;
    void readString(std::string* _string) override;

	// IByteBufferEx
	void Allocate(size_t _size) override;
	void SetFilled() override;
	void CopyData(const uint8* _data, size_t _size) override;
	uint8* getDataEx() override { return &m_Data[0]; }
	uint8* getDataFromCurrentEx() override { return &m_Data[m_CurrentPosition]; }

    //
    // Read
    //

    CByteBuffer& operator>>(int8& value);
    CByteBuffer& operator>>(int16& value);
    CByteBuffer& operator>>(int32& value);
    CByteBuffer& operator>>(int64& value);
    CByteBuffer& operator>>(uint8& value);
    CByteBuffer& operator>>(uint16& value);
    CByteBuffer& operator>>(uint32& value);
    CByteBuffer& operator>>(uint64& value);
    CByteBuffer& operator>>(float& value);
    CByteBuffer& operator>>(double& value);
    CByteBuffer& operator>>(std::string * String);

    //
	// Write
    //
    void Resize(size_t NewSize);

	void Append(const uint8* _data, size_t _size);
    void Insert(size_t Position, const uint8* _data, size_t _size);

	void Write(const std::string& _string, uint64 _expectedSize = UINT32_MAX);
	void WriteDummy(uint64 _size);

    CByteBuffer& operator<<(int8 value);
    CByteBuffer& operator<<(int16 value);
    CByteBuffer& operator<<(int32 value);
    CByteBuffer& operator<<(int64 value);
    CByteBuffer& operator<<(uint8 value);
    CByteBuffer& operator<<(uint16 value);
    CByteBuffer& operator<<(uint32 value);
    CByteBuffer& operator<<(uint64 value);
    CByteBuffer& operator<<(float value);
    CByteBuffer& operator<<(double value);
    CByteBuffer& operator<<(CByteBuffer& _other);
    CByteBuffer& operator<<(const char* _string);
    CByteBuffer& operator<<(const std::string& _string);

private:
	bool                m_IsFilled;

	bool                m_IsEOF;
	bool                m_IsAllocated;
	std::vector<uint8>  m_Data;
	size_t              m_CurrentPosition;
};