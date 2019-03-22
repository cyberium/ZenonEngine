#include "stdafx.h"

// General
#include "ByteBuffer.h"

CByteBuffer::CByteBuffer() :
	m_IsFilled(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{}

CByteBuffer::CByteBuffer(const CByteBuffer& _other)
{
	m_IsFilled = false;
	m_IsEOF = true;
	m_IsAllocated = false;
	m_Data.clear();
	m_CurrentPosition = 0;

	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBuffer::CByteBuffer(CByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsEOF = _other.m_IsEOF;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBuffer::CByteBuffer(size_t _size) :
	m_IsFilled(false),
	m_IsEOF(true),
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
	m_IsEOF = true;
	m_IsAllocated = false;
	m_Data.clear();
	m_CurrentPosition = 0;

	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

CByteBuffer& CByteBuffer::operator=(CByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsEOF = _other.m_IsEOF;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
	return *this;
}

//--

void CByteBuffer::seek(size_t _bufferOffsetAbsolute)
{
	assert1(_bufferOffsetAbsolute <= getSize());
	m_CurrentPosition = _bufferOffsetAbsolute;
	m_IsEOF = m_CurrentPosition >= getSize();
}

void CByteBuffer::seekRelative(intptr_t _bufferOffsetRelative)
{
	assert1(m_CurrentPosition + _bufferOffsetRelative >= 0);
	assert1(m_CurrentPosition + _bufferOffsetRelative <= getSize());
	m_CurrentPosition += _bufferOffsetRelative;
	m_IsEOF = m_CurrentPosition >= getSize();
}

//-- READ

bool CByteBuffer::readLine(std::string* _string)
{
	assert1(_string != nullptr);

	if (m_IsEOF)
	{
		return false;
	}

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

void CByteBuffer::readBytes(void* _destination, size_t _size)
{
	if (m_IsEOF)
	{
		return;
	}

	uint64 posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead >= getSize())
	{
		_size = getSize() - m_CurrentPosition;
		m_IsEOF = true;
	}

	assert1(_destination != nullptr);
	std::memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);

	m_CurrentPosition = posAfterRead;
}

void CByteBuffer::readString(std::string* _string)
{
	assert1(_string != nullptr);

	std::string str = "";
	while (true)
	{
		uint8 byte;
		readBytes(&byte);

		str.append(1, static_cast<char>(byte));

		if (byte == '\0')
		{
			break;
		}
	}

	(*_string) = str;
}

//
// IByteBufferEx
//

void CByteBuffer::Allocate(size_t _size)
{
	m_IsEOF = true;

	if (_size > 0)
	{
		assert1(!m_IsAllocated);
		m_Data.reserve(_size);
		m_IsAllocated = true;
	}

	if (m_Data.size() < _size)
	{
		m_Data.resize(_size);
	}

	m_CurrentPosition = 0;
}

void CByteBuffer::SetFilled()
{
	m_IsEOF = (getSize() == 0);
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
		fail1();
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
	assert1(_data != nullptr);

	for (size_t i = 0; i < _size; i++)
	{
		uint64 pos = m_CurrentPosition + i;
		m_Data.push_back(_data[i]);
	}

	if (_size != 0)
	{
		m_IsEOF = false;
	}
}

void CByteBuffer::Write(cstring _string, uint64 _expectedSize)
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
	{
		m_Data.push_back(0x00);
	}
}