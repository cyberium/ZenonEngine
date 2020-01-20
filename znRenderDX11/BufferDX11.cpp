#include "stdafx.h"

#include "BufferDX11.h"

BufferDX11::BufferDX11(IRenderDeviceDX11* RenderDeviceD3D11, UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
	, m_pBuffer(NULL)
	, m_uiOffset(offset)
	, m_uiStride(stride)
	, m_BindFlags(bindFlags)
	, m_uiCount((UINT)count)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.ByteWidth = m_uiStride * m_uiCount;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	resourceData.pSysMem = data;
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer)))
	{
		_ASSERT_EXPR(false, "Failed to create buffer.");
		return;
	}
}

BufferDX11::~BufferDX11()
{
    
}

bool BufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* buffers[] = { m_pBuffer };
	UINT offsets[] = { m_uiOffset };
	UINT strides[] = { m_uiStride };

	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetIndexBuffer(m_pBuffer, m_uiStride == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
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

	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		break;
	default:
		_ASSERT(false);
	}
}

void BufferDX11::Copy(std::shared_ptr<IBuffer> other)
{
	std::shared_ptr<BufferDX11> srcBuffer = std::dynamic_pointer_cast<BufferDX11>(other);

	if (srcBuffer && srcBuffer.get() != this && m_uiCount * m_uiStride == srcBuffer->m_uiCount * srcBuffer->m_uiStride)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		Log::Error("Source buffer is not compatible with this buffer.");
	}
}

IBuffer::BufferType BufferDX11::GetType() const
{
	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		return IBuffer::BufferType::VertexBuffer;
		break;
	case D3D11_BIND_INDEX_BUFFER:
		return IBuffer::BufferType::IndexBuffer;
		break;
	case D3D11_BIND_CONSTANT_BUFFER:
		return IBuffer::BufferType::ConstantBuffer;
		break;
	default:
		return IBuffer::BufferType::Unknown;
		break;
	}
}

uint32 BufferDX11::GetElementCount() const
{
	return m_uiCount;
}

uint32 BufferDX11::GetElementStride() const
{
	return m_uiStride;
}

uint32 BufferDX11::GetElementOffset() const
{
    return m_uiOffset;
}
