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
	return (m_Access != EAccess::None) && (m_Access != EAccess::GPUWrite);
}

//
// IObjectLoadSave
//
void CStructuredBufferBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	CBufferBase::Load(ByteBuffer);

	ByteBuffer->read(&m_Access);
}

void CStructuredBufferBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	CBufferBase::Save(ByteBuffer);

	ByteBuffer->write(&m_Access);
}

namespace
{
	std::string AccessToString(EAccess Access)
	{
		std::string result = "";
		if ((Access & EAccess::GPUWrite) != 0)
			result += "GPUWrite";
		if ((Access & EAccess::CPURead) != 0)
			result += "CPURead";
		if ((Access & EAccess::CPUWrite) != 0)
			result += "CPUWrite";
		return result;
	}

	EAccess AccessFromString(const std::string& Access)
	{
		EAccess access = EAccess::None;
		if (Access.find("GPUWrite") != std::string::npos)
			access = static_cast<EAccess>(access | EAccess::GPUWrite);
		if (Access.find("CPURead") != std::string::npos)
			access = static_cast<EAccess>(access | EAccess::CPURead);
		if (Access.find("CPUWrite") != std::string::npos)
			access = static_cast<EAccess>(access | EAccess::CPUWrite);
		return access;
	}
}

void CStructuredBufferBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CBufferBase::Load(Reader);

	m_Access = AccessFromString(Reader->GetStrAttribute("Access"));
}

void CStructuredBufferBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CBufferBase::Save(Writer);

	Writer->SetStrAttribute(AccessToString(m_Access), "Access");
}