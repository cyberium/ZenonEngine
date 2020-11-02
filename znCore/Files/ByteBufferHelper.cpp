#include "stdafx.h"

// General
#include "ByteBufferHelper.h"

const char * cNewLineSymbol = "\r\n";


bool Utils::ByteBufferHelper::readLine(IByteBuffer * ByteBuffer, std::string* String)
{
	_ASSERT(String != nullptr && String->empty());

	if (ByteBuffer->isEof())
		return false;

	size_t lastGoodCharPos = std::string((const char*)(ByteBuffer->getDataFromCurrent()), ByteBuffer->getSize() - ByteBuffer->getPos()).find_first_of(cNewLineSymbol);
	_ASSERT(ByteBuffer->getPos() + lastGoodCharPos < ByteBuffer->getSize());
	if (lastGoodCharPos != std::string::npos)
	{
		if (lastGoodCharPos != 0) // Non zero value mean that before \r\n exists normal text
		{
			std::string buff;
			buff.resize(lastGoodCharPos);
			if (false == ByteBuffer->readBytes(&buff[0], lastGoodCharPos))
				return false;
			String->append(buff);
		}

		if (false == ByteBuffer->seekRelative(::strlen(cNewLineSymbol))) // Skip \r\n
			return false;
		return true;
	}

	return readString(ByteBuffer, String);
}

bool Utils::ByteBufferHelper::getText(IByteBuffer * ByteBuffer, std::string * String)
{
	_ASSERT(String != nullptr && String->empty());
	if (String == nullptr || false == String->empty())
		return false;

	while (!ByteBuffer->isEof())
	{
		std::string line;
		if (false == ByteBuffer->readLine(&line))
			return false;
		String->append(line);
		String->append(cNewLineSymbol);
	}
	return true;
}

bool Utils::ByteBufferHelper::readBytes(IByteBuffer * ByteBuffer, void* Destination, size_t Size)
{
	_ASSERT(Destination != nullptr);
	_ASSERT(Size > 0);

	if (Size == 0)
	{
		//_ASSERT(false);
		return false;
	}

	if (ByteBuffer->isEof())
	{
		//_ASSERT(false);
		return false;
	}

	if ((ByteBuffer->getPos() + Size) > ByteBuffer->getSize())
	{
		//_ASSERT(false);
		return false;
	}

	std::memcpy(Destination, &(ByteBuffer->getData()[ByteBuffer->getPos()]), Size);
	ByteBuffer->seekRelative(Size);
	return true;
}

bool Utils::ByteBufferHelper::readString(IByteBuffer * ByteBuffer, std::string* String)
{
	_ASSERT(String != nullptr && String->empty());
	while (!ByteBuffer->isEof())
	{
		uint8 byte;
		if (false == ByteBuffer->readBytes(&byte, sizeof(uint8)))
			return false;
		String->append(1, static_cast<char>(byte));
		if (byte == '\0')
			break;
	}
	return true;
}