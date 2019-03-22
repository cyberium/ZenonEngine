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
	const uint8* getData() const override { return &m_Data[0]; }
	const uint8* getDataFromCurrent() const override { return &m_Data[m_CurrentPosition]; }
	bool isEof() const override { return m_IsEOF; }

	void seek(size_t _bufferOffsetAbsolute) override;
	void seekRelative(intptr_t _bufferOffsetRelative) override;

	bool readLine(std::string* _string) override;
	void readBytes(void* _destination, size_t _size = 1) override;
	void readString(std::string* _string) override;

	// IByteBufferEx
	void Allocate(size_t _size) override;
	void SetFilled() override;
	void CopyData(const uint8* _data, size_t _size) override;
	uint8* getDataEx() override { return &m_Data[0]; }
	uint8* getDataFromCurrentEx() override { return &m_Data[m_CurrentPosition]; }

	//-- SIGNED
	CByteBuffer& operator>>(int8 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(int16 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(int32 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(int64 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- UNSIGNED
	CByteBuffer& operator>>(uint8 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(uint16 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(uint32 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(uint64 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- FLOAT
	CByteBuffer& operator>>(float &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator>>(double &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- STRING
	CByteBuffer& operator>>(std::string& _string)
	{
		_string = "";

		while (true)
		{
			uint8 byte;
			readBytes(&byte);

			_string.append(1, static_cast<char>(byte));

			if (byte == '\0')
			{
				break;
			}
		}
	}

	// Write
	void Append(const uint8* _data, size_t _size);

	void Write(cstring _string, uint64 _expectedSize = UINT32_MAX);
	void WriteDummy(uint64 _size);

	CByteBuffer& operator<<(int8 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(int16 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(int32 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(int64 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//----
	CByteBuffer& operator<<(uint8 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(uint16 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(uint32 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(uint64 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//--
	CByteBuffer& operator<<(float value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	CByteBuffer& operator<<(double value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//--
	CByteBuffer& operator<<(CByteBuffer& _other)
	{
		Append(_other.getData(), _other.getSize());
		return *this;
	}
	CByteBuffer& operator<<(const char* _string)
	{
		size_t len = strlen(_string);
		if (len > 0)
		{
			Append((const uint8*)_string, len);
		}
		Append((uint8)0x00, 1);
		return *this;
	}
	CByteBuffer& operator<<(cstring _string)
	{
		Append((uint8*)_string.c_str(), _string.size() + 1);
		return *this;
	}

private:
	bool            m_IsFilled;

	bool            m_IsEOF;
	bool		    m_IsAllocated;
	std::vector<uint8>   m_Data;
	size_t	        m_CurrentPosition;
};