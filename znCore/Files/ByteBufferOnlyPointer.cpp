#include "stdafx.h"

// General
#include "ByteBufferOnlyPointer.h"


CByteBufferOnlyPointer::CByteBufferOnlyPointer(const CByteBufferOnlyPointer& _other)
{
	m_Data = _other.m_Data;
	m_DataSize = _other.m_DataSize;
	m_CurrentPosition = _other.m_CurrentPosition;
}

CByteBufferOnlyPointer::CByteBufferOnlyPointer(const void* DataPtr, size_t DataSize)
{
	_ASSERT(DataPtr != nullptr);
	_ASSERT(DataSize != 0);

	m_Data = static_cast<const uint8*>(DataPtr);
	m_DataSize = DataSize;
	m_CurrentPosition = 0;
}

CByteBufferOnlyPointer::~CByteBufferOnlyPointer()
{
}

//--

const CByteBufferOnlyPointer& CByteBufferOnlyPointer::operator=(const CByteBufferOnlyPointer& _other)
{
	m_Data = _other.m_Data;
	m_DataSize = _other.m_DataSize;
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

//--

void CByteBufferOnlyPointer::seek(size_t AbsoluteOffset)
{
	_ASSERT(m_CurrentPosition + AbsoluteOffset >= 0 && AbsoluteOffset <= getSize());
	m_CurrentPosition = AbsoluteOffset;
}

void CByteBufferOnlyPointer::seekRelative(intptr_t RelativeOffset)
{
	_ASSERT(m_CurrentPosition + RelativeOffset >= 0 && RelativeOffset <= getSize());
	m_CurrentPosition += RelativeOffset;
}

//-- READ

bool CByteBufferOnlyPointer::readLine(std::string* _string)
{
	_ASSERT(_string != nullptr);
	_ASSERT(isEof() == false);

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

	(*_string) = line;
	return true;
}

bool CByteBufferOnlyPointer::readBytes(void* _destination, size_t _size)
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

void CByteBufferOnlyPointer::readString(std::string * _string)
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
