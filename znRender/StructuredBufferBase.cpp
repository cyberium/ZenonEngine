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

	ByteBuffer->read(&m_Access);
}

void CStructuredBufferBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	CBufferBase::Save(ByteBuffer);

	ByteBuffer->write(&m_Access);
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
void CStructuredBufferBase::InitializeStructuredBufferBase(EAccess EAccess)
{
	m_Access = EAccess;

	DoInitializeStructuredBuffer();
}

EAccess CStructuredBufferBase::GetAccess() const
{
	return m_Access;
}

bool CStructuredBufferBase::IsDynamic() const
{
	return (uint32)m_Access != (uint32)EAccess::None && (uint32)m_Access != (uint32)EAccess::GPUWrite;
}
