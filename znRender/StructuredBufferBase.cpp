#include "stdafx.h"

// General
#include "StructuredBufferBase.h"

CStructuredBufferBase::CStructuredBufferBase(IRenderDevice& RenderDevice)
	: CBufferBase(RenderDevice, IBuffer::BufferType::StructuredBuffer)
{

}

CStructuredBufferBase::~CStructuredBufferBase()
{
}

void CStructuredBufferBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	CBufferBase::Load(ByteBuffer);

	ByteBuffer->read(&m_CPUAccess);
	ByteBuffer->read(&m_GPUWrite);
}

void CStructuredBufferBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	CBufferBase::Save(ByteBuffer);

	ByteBuffer->write(&m_CPUAccess);
	ByteBuffer->write(&m_GPUWrite);
}


//
// IBufferPrivate
//
void CStructuredBufferBase::DoInitializeBuffer()
{
}


//
// IStructuredBufferPrivate
//
void CStructuredBufferBase::InitializeStructuredBufferBase(CPUAccess CPUAccess, bool GPUWrite)
{
	m_CPUAccess = CPUAccess;
	m_GPUWrite = GPUWrite;

	DoInitializeStructuredBuffer();
}

CPUAccess CStructuredBufferBase::GetCPUAccess() const
{
	return m_CPUAccess;
}

bool CStructuredBufferBase::GetGPUWrite() const
{
	return m_GPUWrite;
}

bool CStructuredBufferBase::IsDynamic() const
{
	return (uint32)m_CPUAccess != 0;
}
