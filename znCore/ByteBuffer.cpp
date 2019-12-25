#include "stdafx.h"

// General
#include "ByteBuffer.h"

CByteBuffer::CByteBuffer() :
	m_IsFilled(false),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{}

CByteBuffer::CByteBuffer(const CByteBuffer& _other)
{
	m_IsFilled = false;
	m_IsAllocated = false;
	m_Data.clear();
	m_CurrentPosition = 0;

	CopyData(_other.getData(), _other.getSize());

	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBuffer::CByteBuffer(CByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBuffer::CByteBuffer(size_t _size) :
	m_IsFilled(false),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{
	Allocate(_size);
}

CByteBuffer::~CByteBuffer()
{
	m_Data.clear();
}

//--

CByteBuffer& CByteBuffer::operator=(const CByteBuffer& _other)
{
	m_IsFilled = false;
	m_IsAllocated = false;
	m_Data.clear();
	m_CurrentPosition = 0;

	CopyData(_other.getData(), _other.getSize());

	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

CByteBuffer& CByteBuffer::operator=(CByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
	return *this;
}

//--

void CByteBuffer::seek(size_t _bufferOffsetAbsolute)
{
	_ASSERT(_bufferOffsetAbsolute <= getSize());
	m_CurrentPosition = _bufferOffsetAbsolute;
}

void CByteBuffer::seekRelative(intptr_t _bufferOffsetRelative)
{
	_ASSERT(m_CurrentPosition + _bufferOffsetRelative >= 0);
	_ASSERT(m_CurrentPosition + _bufferOffsetRelative <= getSize());
	m_CurrentPosition += _bufferOffsetRelative;
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
	line = Utils::Trim(line);

	(*_string) = line;
	return true;
}

bool CByteBuffer::readBytes(void* _destination, size_t _size)
{
	if (isEof())
		return false;

	uint64 posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead >= getSize())
	{
		_size = getSize() - m_CurrentPosition;
	}

	_ASSERT(_destination != nullptr);
	std::memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);

	m_CurrentPosition = posAfterRead;

	return true;
}

void CByteBuffer::readString(std::string* _string)
{
	_ASSERT(_string != nullptr);

	std::string str = "";
	while (true)
	{
		uint8 byte;
		if (!readBytes(&byte, sizeof(uint8)))
			break;

		str.append(1, static_cast<char>(byte));

		if (byte == '\0')
		{
			break;
		}
	}

	(*_string) = str;
}

void CByteBuffer::writeLine(std::string String)
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

void CByteBuffer::writeString(std::string String)
{
}

//
// IByteBufferEx
//

void CByteBuffer::Allocate(size_t _size)
{
	if (_size > 0)
	{
		_ASSERT(m_IsAllocated == false, L"ByteBuffer already allocated.");
        m_Data.resize(_size);
		m_IsAllocated = true;
	}

	m_CurrentPosition = 0;
}

void CByteBuffer::SetFilled()
{
	m_IsFilled = true;
}

void CByteBuffer::CopyData(const uint8* _data, size_t _size)
{
	if (!m_IsAllocated)
	{
		Allocate(_size);
	}

	if (_size > getSize())
	{
		Log::Error("ByteBuffer[]: Source m_Data size [%d] bigger than m_IsAllocated memory [%d]!", _size, getSize());
		Log::Error("ByteBuffer[]: Copy part of source m_Data.");
		_size = getSize();
		_ASSERT(false);
	}

	if (_data != nullptr)
	{
		std::memcpy(&m_Data[0], _data, _size);
		SetFilled();
	}
}

//-- WRITE

void CByteBuffer::Append(const uint8* _data, size_t _size)
{
	_ASSERT(_data != nullptr);

	for (size_t i = 0; i < _size; i++)
	{
		m_Data.push_back(_data[i]);
	}
}

void CByteBuffer::Insert(size_t Position, const uint8 * _data, size_t _size)
{
    m_Data.insert(m_Data.begin() + Position, _data, _data + _size);
}

void CByteBuffer::Write(const std::string& _string, uint64 _expectedSize)
{
	Append((uint8*)_string.c_str(), static_cast<uint64>(_string.size()));

	if ((_string.size() < _expectedSize) && (_expectedSize != UINT32_MAX))
	{
		WriteDummy(_expectedSize - _string.size());
	}
}

void CByteBuffer::WriteDummy(uint64 _size)
{
	for (uint64 i = 0; i < _size; i++)
		m_Data.push_back(0x00);
}