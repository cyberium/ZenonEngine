#include "stdafx.h"

// General
#include "ByteBuffer.h"

CByteBuffer::CByteBuffer() :
	m_CurrentPosition(0)
{}

CByteBuffer::CByteBuffer(const CByteBuffer& _other)
{
	m_Data.clear();
	m_Data.resize(_other.m_Data.size());
	std::copy(_other.m_Data.begin(), _other.m_Data.end(), m_Data.begin());
	m_CurrentPosition = 0;
	// m_CurrentPosition = _other.m_CurrentPosition; It is realy need?
}

CByteBuffer::CByteBuffer(CByteBuffer&& _other)
{
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBuffer::CByteBuffer(std::vector<uint8>&& Vector)
{
	m_Data = std::move(Vector);
	m_CurrentPosition = 0;
}

CByteBuffer::CByteBuffer(const void* DataPtr, size_t DataSize)
{
	_ASSERT(DataPtr != nullptr);
	_ASSERT(DataSize != 0);

	m_Data.resize(DataSize);
	m_Data.assign(static_cast<const uint8*>(DataPtr), static_cast<const uint8*>(DataPtr) + DataSize);
	m_CurrentPosition = 0;
}

CByteBuffer::~CByteBuffer()
{
	m_Data.clear();
}

//--

const CByteBuffer& CByteBuffer::operator=(const CByteBuffer& _other)
{
	m_Data.clear();
	std::copy(_other.m_Data.begin(), _other.m_Data.end(), m_Data.begin());
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

const CByteBuffer& CByteBuffer::operator=(CByteBuffer&& _other)
{
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
	return *this;
}

//--

void CByteBuffer::seek(size_t AbsoluteOffset)
{
	_ASSERT(AbsoluteOffset >= 0 && AbsoluteOffset <= getSize());
	m_CurrentPosition = AbsoluteOffset;
}

void CByteBuffer::seekRelative(intptr_t RelativeOffset)
{
	_ASSERT(m_CurrentPosition + RelativeOffset >= 0 && RelativeOffset <= getSize());
	m_CurrentPosition += RelativeOffset;
}

//-- READ

bool CByteBuffer::readLine(std::string* _string)
{
	_ASSERT(_string != nullptr);

	if (isEof())
		return false;

	// Find first incorrect symbol
	uint64 lineEndPos;
	for (lineEndPos = m_CurrentPosition; (lineEndPos < getSize()) && (m_Data[lineEndPos] != '\n' && m_Data[lineEndPos] != '\r'); lineEndPos++);

	// Find first correct symbol after incorrects
	uint64 nextLineBeginPos;
	for (nextLineBeginPos = lineEndPos; (nextLineBeginPos < getSize()) && (m_Data[nextLineBeginPos] == '\n' || m_Data[nextLineBeginPos] == '\r'); nextLineBeginPos++);

	uint64 charsCount = lineEndPos - m_CurrentPosition;

	char* buff = new char[charsCount + 1];
	buff[charsCount] = '\0';
	readBytes(&buff[0], charsCount);

	// Skip \r and \n
	seekRelative(nextLineBeginPos - lineEndPos);

	std::string line(buff);
	delete[] buff;
	//line = Utils::Trim(line);

	(*_string) = line;
	return true;
}

bool CByteBuffer::readBytes(void* _destination, size_t _size)
{
	_ASSERT(_destination != nullptr);
	_ASSERT(isEof() == false);

	uint64 posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead >= getSize())
	{
		_size = getSize() - m_CurrentPosition;
		posAfterRead = getSize();
	}

	std::memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);
	m_CurrentPosition = posAfterRead;

	return true;
}

void CByteBuffer::readString(std::string* _string)
{
	_ASSERT(_string != nullptr);
	_ASSERT(isEof() == false);

	*_string = "";
	while (true)
	{
		uint8 byte;
		if (readBytes(&byte, sizeof(uint8)) == false)
			break;

		(*_string).append(1, static_cast<char>(byte));

		if (byte == '\0')
			break;
	}
}


void CByteBuffer::writeLine(const std::string& String)
{
}

void CByteBuffer::writeBytes(const void * Source, size_t BytesCount)
{
	for (size_t i = 0; i < BytesCount; i++)
	{
		uint8_t byte = ((const uint8_t*)Source)[i];

		if (isEof())
		{
			m_Data.push_back(byte);
		}
		else
		{
			m_Data[m_CurrentPosition] = byte;
		}

		m_CurrentPosition += 1;
	}
}


void CByteBuffer::writeDummy(size_t BytesCount)
{
	for (size_t i = 0; i < BytesCount; i++)
	{
		if (isEof())
		{
			m_Data.push_back(0x00);
		}
		else
		{
			m_Data[m_CurrentPosition] = 0x00;
		}

		m_CurrentPosition += 1;
	}
}

void CByteBuffer::insert(size_t Position, const void * DataPtr, size_t DataSize)
{
	m_Data.insert(m_Data.begin() + Position, static_cast<const uint8*>(DataPtr), static_cast<const uint8*>(DataPtr) + DataSize);
}

void CByteBuffer::writeString(const std::string& String)
{
	writeBytes(String.c_str(), String.size());
	writeBytes("\0", 1);
}

uint32 CByteBuffer::ReadUInt32()
{
	uint32 value = 0;
	readBytes(&value, sizeof(uint32));
	return value;
}

float CByteBuffer::ReadFloat()
{
	float value = 0.0f;
	readBytes(&value, sizeof(float));
	return value;
}

double CByteBuffer::ReadDouble()
{
	double value = 0.0;
	readBytes(&value, sizeof(double));
	return value;
}





void CByteBuffer::ReadPackedUInt64(uint64& guid)
{
	guid = 0;
	uint8_t mask;

	CByteBuffer::read(&mask);
	ReadPackedUInt64(mask, guid);
}

void CByteBuffer::ReadPackedUInt64(uint8 mask, uint64& value)
{
	for (uint32 i = 0; i < 8; ++i)
	{
		if (mask & (uint8(1) << i))
		{
			uint8 byte;
			CByteBuffer::read(&byte);
			value |= (uint64(byte) << (i * 8));
		}
	}
}

void CByteBuffer::AppendPackedUInt64(uint64 guid)
{
	uint8 mask = 0;
	size_t pos = CByteBuffer::getPos();
	*this << uint8(mask);

	uint8 packed[8];
	if (size_t packedSize = PackUInt64(guid, &mask, packed))
		CByteBuffer::writeBytes(packed, packedSize);

	getDataEx()[pos] = mask;
}

size_t CByteBuffer::PackUInt64(uint64 value, uint8* mask, uint8* result)
{
	size_t resultSize = 0;
	*mask = 0;
	std::memset(result, 0x00, 8);

	for (uint8 i = 0; value != 0; ++i)
	{
		if (value & 0xFF)
		{
			*mask |= uint8(1 << i);
			result[resultSize++] = uint8(value & 0xFF);
		}

		value >>= 8;
	}

	return resultSize;
}