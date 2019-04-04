#include "stdafx.h"

// General
#include "ByteBuffer.h"

//
// Read
//
CByteBuffer& CByteBuffer::operator>>(int8& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(int16& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(int32& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(int64& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(uint8& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(uint16& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(uint32& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(uint64& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(float& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(double& value)
{
    readBytes(&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator>>(std::string * String)
{
    _ASSERT(String != nullptr);
    (*String) = "";

    while (true)
    {
        uint8 byte;
        readBytes(&byte, sizeof(uint8));

        (*String).append(1, static_cast<char>(byte));

        if (byte == '\0')
            break;
    }

    return *this;
}



//
// Write
//
CByteBuffer& CByteBuffer::operator<<(int8 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(int16 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(int32 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(int64 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(uint8 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(uint16 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(uint32 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(uint64 value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(float value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(double value)
{
    Append((uint8*)&value, sizeof(value));
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(CByteBuffer& _other)
{
    Append(_other.getData(), _other.getSize());
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(const char* _string)
{
    size_t len = strlen(_string);
    if (len > 0)
        Append((const uint8*)_string, len);
    Append((uint8)0x00, 1);
    return *this;
}
CByteBuffer& CByteBuffer::operator<<(const std::string& _string)
{
    Append((uint8*)_string.c_str(), _string.size() + 1);
    return *this;
}