#pragma once

class ZN_API CByteBuffer 
	: public IByteBuffer
	, public IByteBufferEx
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
	bool isEof() const override { return m_CurrentPosition >= m_Data.size(); }

    void seek(size_t _bufferOffsetAbsolute) override;
    void seekRelative(intptr_t _bufferOffsetRelative) override;

    bool readLine(std::string* _string) override;
    bool readBytes(void* _destination, size_t _size) override;
    void readString(std::string* _string) override;

	void writeLine(std::string String) override;
	void writeBytes(const void * Source, size_t BytesCount) override;
	void writeString(std::string String) override;

	// IByteBufferEx
	void Allocate(size_t _size) override;
	void SetFilled() override;
	void CopyData(const uint8* _data, size_t _size) override;
	uint8* getDataEx() override { return &m_Data[0]; }
	uint8* getDataFromCurrentEx() override { return &m_Data[m_CurrentPosition]; }


    //
	// Write
    //
	void Append(const uint8* _data, size_t _size);
    void Insert(size_t Position, const uint8* _data, size_t _size);

	void Write(const std::string& _string, uint64 _expectedSize = UINT32_MAX);
	void WriteDummy(uint64 _size);

private:
	bool                m_IsFilled;

	bool                m_IsAllocated;
	std::vector<uint8>  m_Data;
	size_t              m_CurrentPosition;
};

//
// Read
//
inline CByteBuffer& operator>>(CByteBuffer& bb, int8& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, int16& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, int32& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, int64& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, uint8& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, uint16& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, uint32& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, uint64& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, float& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, double& value)
{
	bb.readBytes(&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator>>(CByteBuffer& bb, std::string * String)
{
	_ASSERT(String != nullptr);
	(*String) = "";

	while (true)
	{
		uint8 byte;
		bb.readBytes(&byte, sizeof(uint8));

		(*String).append(1, static_cast<char>(byte));

		if (byte == '\0')
			break;
	}

	return bb;
}



//
// Write
//
inline CByteBuffer& operator<<(CByteBuffer& bb, int8 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, int16 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, int32 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, int64 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, uint8 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, uint16 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, uint32 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, uint64 value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, float value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, double value)
{
	bb.Append((uint8*)&value, sizeof(value));
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, CByteBuffer& _other)
{
	bb.Append(_other.getData(), _other.getSize());
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, const char* _string)
{
	size_t len = strlen(_string);
	if (len > 0)
		bb.Append((const uint8*)_string, len);
	bb.Append((uint8)0x00, 1);
	return bb;
}
inline CByteBuffer& operator<<(CByteBuffer& bb, const std::string& _string)
{
	bb.Append((uint8*)_string.c_str(), _string.size() + 1);
	return bb;
}
