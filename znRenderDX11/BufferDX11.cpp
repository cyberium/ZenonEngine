#include "stdafx.h"

#include "BufferDX11.h"


namespace
{
	UINT BufferTypeToBindFlags(IBuffer::BufferType BufferType)
	{
		switch (BufferType)
		{
			case IBuffer::BufferType::VertexBuffer:
				return D3D11_BIND_VERTEX_BUFFER;
			case IBuffer::BufferType::IndexBuffer:
				return D3D11_BIND_INDEX_BUFFER;
			default:
				throw CznRenderException("Buffer: Incorrect buffer type.");
		}
	}
}


BufferDX11::BufferDX11(IRenderDeviceDX11& RenderDeviceDX11, IBuffer::BufferType BufferType)
	: CBufferBase(RenderDeviceDX11, BufferType)
	, m_RenderDeviceDX11(RenderDeviceDX11)
{}

BufferDX11::~BufferDX11()
{
    
}



//
// IBuffer
//
bool BufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* buffers[] = { m_pBuffer };
	UINT offsets[] = { GetElementOffset() };
	UINT strides[] = { GetElementStride() };

	switch (GetBufferType())
	{
		case IBuffer::BufferType::VertexBuffer:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
			break;
		case IBuffer::BufferType::IndexBuffer:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetIndexBuffer(m_pBuffer, GetElementStride() == sizeof(uint32) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
			break;
		default:
			_ASSERT(false);
	}

	return true;
}

void BufferDX11::UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* buffers[] = { nullptr };
	UINT offsets[] = { 0 };
	UINT strides[] = { 0 };

	switch (GetBufferType())
	{
		case IBuffer::BufferType::VertexBuffer:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
			break;
		case IBuffer::BufferType::IndexBuffer:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			break;
		default:
			_ASSERT(false);
	}
}

void BufferDX11::Copy(const IBuffer* other) const
{
	const BufferDX11* srcBuffer = dynamic_cast<const BufferDX11*>(other);
	if (srcBuffer && srcBuffer != this && GetElementCount() * GetElementStride() == srcBuffer->GetElementCount() * srcBuffer->GetElementStride())
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
		throw CznRenderException("Source buffer is not compatible with this buffer.");
}



//
// protected
//
void BufferDX11::DoInitializeBuffer()
{
	_ASSERT(m_pBuffer == NULL);

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = BufferTypeToBindFlags(GetBufferType());
	bufferDesc.ByteWidth = GetElementCount() * GetElementStride();
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA resourceData = { 0 };
	resourceData.pSysMem = GetData().data();
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer), L"Failed to create buffer.");

	//std::shared_ptr<CByteBuffer> bb = std::make_shared<CByteBuffer>();
	//Save(bb);
	//bb->seek(0);
	//Load(bb);
}
