#include "stdafx.h"

// General
#include "BufferBase.h"

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

void CBufferBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(false);
}

void CBufferBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	_ASSERT(false);
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
	if (!m_Data.empty())
		m_Data.clear();

	if (data != nullptr)
		m_Data.assign((const uint8*)data, (const uint8*)data + dataSize);
	else
		m_Data.resize(dataSize);
}
