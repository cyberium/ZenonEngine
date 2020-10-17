#include "stdafx.h"

// General
#include "ByteBufferOnlyPointer.h"

#include "ByteBufferHelper.h"


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

bool CByteBufferOnlyPointer::seek(size_t AbsoluteOffset)
{
	_ASSERT(AbsoluteOffset >= 0 && AbsoluteOffset <= getSize());
	if (AbsoluteOffset < 0 || AbsoluteOffset >= getSize())
		return false;

	m_CurrentPosition = AbsoluteOffset;
	return true;
}

bool CByteBufferOnlyPointer::seekRelative(intptr_t RelativeOffset)
{
	size_t newPos = m_CurrentPosition + RelativeOffset;

	_ASSERT(newPos >= 0 && newPos < getSize());
	if (newPos < 0 || newPos >= getSize())
		return false;

	m_CurrentPosition = newPos;
	return true;
}

//-- READ

bool CByteBufferOnlyPointer::readLine(std::string * String)
{
	return Utils::ByteBufferHelper::readLine(this, String);
}

bool CByteBufferOnlyPointer::getText(std::string * String)
{
	return Utils::ByteBufferHelper::getText(this, String);
}

bool CByteBufferOnlyPointer::readBytes(void* Destination, size_t Size)
{
	return Utils::ByteBufferHelper::readBytes(this, Destination, Size);
}

bool CByteBufferOnlyPointer::readString(std::string * String)
{
	return Utils::ByteBufferHelper::readString(this, String);
}

void CByteBufferOnlyPointer::writeLine(const std::string& String)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::writeBytes(const void * Source, size_t BytesCount)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::writeDummy(size_t BytesCount)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::insert(size_t Position, const void * DataPtr, size_t DataSize)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}

void CByteBufferOnlyPointer::writeString(const std::string& String)
{
	_ASSERT_EXPR(false, L"Unsupported write operation.");
}
