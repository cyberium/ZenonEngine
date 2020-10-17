#pragma once

namespace Utils
{
	namespace ByteBufferHelper
	{
		bool readLine(IByteBuffer * ByteBuffer, std::string* String);
		bool getText(IByteBuffer * ByteBuffer, std::string * String);
		bool readBytes(IByteBuffer * ByteBuffer, void* Destination, size_t Size);
		bool readString(IByteBuffer * ByteBuffer, std::string* String);
	}
}
