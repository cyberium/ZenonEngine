#include "stdafx.h"

// General
#include "ByteBufferOnlyPointer.h"


CByteBufferOnlyPointer::CByteBufferOnlyPointer(const CByteBufferOnlyPointer& _other)
{
	m_Data = _other.m_Data;
	m_DataSize = _other.m_DataSize;
	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBufferOnlyPointer::CByteBufferOnlyPointer(const uint8_t* DataPtr, size_t DataSize)
{
	m_Data = DataPtr;
	m_DataSize = DataSize;
	m_IsEOF = false;
	m_CurrentPosition = 0;
}

CByteBufferOnlyPointer::~CByteBufferOnlyPointer()
{
}

//--

CByteBufferOnlyPointer& CByteBufferOnlyPointer::operator=(const CByteBufferOnlyPointer& _other)
{
	m_Data = _other.m_Data;
	m_DataSize = _other.m_DataSize;
	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

//--

void CByteBufferOnlyPointer::seek(size_t _bufferOffsetAbsolute)
{
	_ASSERT(_bufferOffsetAbsolute <= getSize());
	m_CurrentPosition = _bufferOffsetAbsolute;
	m_IsEOF = m_CurrentPosition >= getSize();
}

void CByteBufferOnlyPointer::seekRelative(intptr_t _bufferOffsetRelative)
{
	_ASSERT(m_CurrentPosition + _bufferOffsetRelative >= 0);
	_ASSERT(m_CurrentPosition + _bufferOffsetRelative <= getSize());
	m_CurrentPosition += _bufferOffsetRelative;
	m_IsEOF = m_CurrentPosition >= getSize();
}

//-- READ

bool CByteBufferOnlyPointer::readLine(std::string* _string)
{
	_ASSERT(_string != nullptr);

	if (m_IsEOF)
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

bool CByteBufferOnlyPointer::readBytes(void* _destination, size_t _size)
{
	if (m_IsEOF)
	{
		return false;
	}

	uint64 posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead >= getSize())
	{
		_size = getSize() - m_CurrentPosition;
		m_IsEOF = true;
	}

	_ASSERT(_destination != nullptr);
	std::memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);

	m_CurrentPosition = posAfterRead;

	return true;
}

void CByteBufferOnlyPointer::readString(std::string* _string)
{
	_ASSERT(_string != nullptr);

	std::string str = "";
	while (true)
	{
		uint8 byte;
		readBytes(&byte, sizeof(uint8));

		str.append(1, static_cast<char>(byte));

		if (byte == '\0')
		{
			break;
		}
	}

	(*_string) = str;
}

void CByteBufferOnlyPointer::writeLine(std::string String)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::writeBytes(const void * Source, size_t BytesCount)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::writeString(std::string String)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}
