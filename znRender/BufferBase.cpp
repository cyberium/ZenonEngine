#include "stdafx.h"

// General
#include "BufferBase.h"

#include <zlib/source/zlib.h>

CBufferBase::CBufferBase(IRenderDevice& RenderDevice, IBuffer::BufferType ByfferType)
	: m_RenderDevice(RenderDevice)
	, m_BufferType(ByfferType)
{}

CBufferBase::~CBufferBase()
{}



//
// IBuffer
//
IBuffer::BufferType CBufferBase::GetBufferType() const
{
	return m_BufferType;
}

uint32 CBufferBase::GetElementCount() const
{
	return m_Count;
}

uint32 CBufferBase::GetElementStride() const
{
	return m_Stride;
}

uint32 CBufferBase::GetElementOffset() const
{
	return m_Offset;
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CBufferBase::Copy() const
{
	return std::shared_ptr<IObject>();
}

void CBufferBase::CopyTo(std::shared_ptr<IObject> Destination) const
{
}

void CBufferBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	IBuffer::BufferType bufferType;
	uint32 count, offset, stride;
	size_t dataSize;
	std::vector<uint8> data;

	ByteBuffer->read(&bufferType);
	ByteBuffer->read(&count);
	ByteBuffer->read(&offset);
	ByteBuffer->read(&stride);

	// Data of buffer
	{
		ByteBuffer->read(&dataSize);
		data.resize(dataSize);
		ByteBuffer->readBytes(data.data(), dataSize);
	}

	m_BufferType = bufferType;
	InitializeBufferBase(data.data(), count, offset, stride);
}

void CBufferBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	ByteBuffer->write(&m_BufferType);
	ByteBuffer->write(&m_Count);
	ByteBuffer->write(&m_Offset);
	ByteBuffer->write(&m_Stride);
	
	// Data of buffer
	{
		size_t dataSize = m_Data.size();
		ByteBuffer->write(&dataSize);
		ByteBuffer->writeBytes(m_Data.data(), dataSize);
	}
}

namespace
{
	std::string BufferTypeToString(IBuffer::BufferType BufferType)
	{
		switch (BufferType)
		{
			case IBuffer::BufferType::Unknown:
				return "Unknown";
			case IBuffer::BufferType::VertexBuffer:
				return "VertexBuffer";
			case IBuffer::BufferType::IndexBuffer:
				return "IndexBuffer";
			case IBuffer::BufferType::StructuredBuffer:
				return "StructuredBuffer";
			case IBuffer::BufferType::ConstantBuffer:
				return "ConstantBuffer";
		}
		throw CException("Buffer type '%d' is invalid.", BufferType);
	}

	IBuffer::BufferType BufferTypeFromString(const std::string& BufferType)
	{
		if (BufferType == "Unknown")
			return IBuffer::BufferType::Unknown;
		else if (BufferType == "VertexBuffer")
			return IBuffer::BufferType::VertexBuffer;
		else if(BufferType == "IndexBuffer")
			return IBuffer::BufferType::IndexBuffer;
		else if(BufferType == "StructuredBuffer")
			return IBuffer::BufferType::StructuredBuffer;
		else if (BufferType == "ConstantBuffer")
			return IBuffer::BufferType::ConstantBuffer;
		throw CException("Buffer type '%s' is invalid.", BufferType.c_str());
	}
}

void CBufferBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	IBuffer::BufferType bufferType = BufferTypeFromString(Reader->GetName());
	uint32 count = Reader->GetUIntAttribute("Size");
	uint32 offset = Reader->GetUIntAttribute("Offset");
	uint32 stride = Reader->GetUIntAttribute("Stride");
	std::vector<uint8> data = Utils::Base64_Decode(Reader->GetValue());

	//uncompress()


	m_BufferType = bufferType;


	InitializeBufferBase(data.data(), count, offset, stride);
}

void CBufferBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetName(BufferTypeToString(m_BufferType));
	Writer->SetUIntAttribute(m_Count, "Size");
	Writer->SetUIntAttribute(m_Offset, "Offset");
	Writer->SetUIntAttribute(m_Stride, "Stride");

	const auto& data = GetData();
	Writer->SetValue(Utils::Base64_Encode(data.data(), data.size()));

	/*{
		unsigned long compressedSize = compressBound(m_Data.size());

		std::vector<uint8> compressed;
		compressed.resize(compressedSize);
		if (compress(compressed.data(), &compressedSize, data.data(), data.size()) != Z_OK)
			throw CException("ZLIB Compress error.");
		compressed.resize(compressedSize);

		Writer->SetValue(Utils::Base64_Encode(compressed.data(), compressed.size()));
	}*/
}


//
// Protected
//
void CBufferBase::InitializeBufferBase(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	m_Offset = offset;
	m_Stride = stride;
	m_Count = count;
	SetData(data, stride * count);

	DoInitializeBuffer();
}

const std::vector<uint8>& CBufferBase::GetData() const
{
	return m_Data;
}

std::vector<uint8>& CBufferBase::GetDataEx()
{
	return m_Data;
}

void CBufferBase::SetData(const void * data, size_t dataSize)
{
	if (false == m_Data.empty())
		m_Data.clear();

	if (data != nullptr)
		m_Data.assign((const uint8*)data, (const uint8*)data + dataSize);
	else
		m_Data.resize(dataSize);
}
