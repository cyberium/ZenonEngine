#include "stdafx.h"

// General
#include "ByteBuffer.h"

#include "ByteBufferHelper.h"


CByteBuffer::CByteBuffer() 
	: m_CurrentPosition(0)
{}

CByteBuffer::CByteBuffer(const CByteBuffer& _other)
{
	m_Data.clear();
	m_Data.resize(_other.m_Data.size());
	std::copy(_other.m_Data.begin(), _other.m_Data.end(), m_Data.begin());
	m_CurrentPosition = 0;
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
	m_Data.resize(_other.m_Data.size());
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

bool CByteBuffer::seek(size_t AbsoluteOffset)
{
	_ASSERT(AbsoluteOffset >= 0 && AbsoluteOffset <= getSize());
	if (AbsoluteOffset < 0 || AbsoluteOffset >= getSize())
		return false;

	m_CurrentPosition = AbsoluteOffset;
	return true;
}

bool CByteBuffer::seekRelative(intptr_t RelativeOffset)
{
	size_t newPos = m_CurrentPosition + RelativeOffset;

	_ASSERT(newPos >= 0 && newPos <= getSize());
	if (newPos < 0 || newPos > getSize())
		return false;

	m_CurrentPosition = newPos;
	return true;
}

//-- READ

bool CByteBuffer::readLine(std::string* String)
{
	return Utils::ByteBufferHelper::readLine(this, String);
}

bool CByteBuffer::getText(std::string * String)
{
	return Utils::ByteBufferHelper::getText(this, String);
}

bool CByteBuffer::readBytes(void* Destination, size_t Size)
{
	return Utils::ByteBufferHelper::readBytes(this, Destination, Size);
}

bool CByteBuffer::readString(std::string* String)
{
	return Utils::ByteBufferHelper::readString(this, String);
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
