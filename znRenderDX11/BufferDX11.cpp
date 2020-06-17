#include "stdafx.h"

#include "BufferDX11.h"

BufferDX11::BufferDX11(IRenderDeviceDX11& RenderDeviceDX11, UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_Offset(offset)
	, m_Stride(stride)
	, m_BindFlags(bindFlags)
	, m_Count((UINT)count)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = m_BindFlags;
	bufferDesc.ByteWidth = m_Stride * m_Count;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA resourceData = { 0 };
	resourceData.pSysMem = data;
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer), L"Failed to create buffer.");
}

BufferDX11::~BufferDX11()
{
    
}



//
// IBuffer
//
bool BufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* buffers[] = { m_pBuffer };
	UINT offsets[] = { m_Offset };
	UINT strides[] = { m_Stride };

	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetIndexBuffer(m_pBuffer, m_Stride == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
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
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetVertexBuffers(id, 1, buffers, strides, offsets);
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		break;
	default:
		_ASSERT(false);
	}
}

void BufferDX11::Copy(const IBuffer* other) const
{
	const BufferDX11* srcBuffer = dynamic_cast<const BufferDX11*>(other);
	if (srcBuffer && srcBuffer != this && m_Count * m_Stride == srcBuffer->m_Count * srcBuffer->m_Stride)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
		throw CznRenderException("Source buffer is not compatible with this buffer.");
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
	return m_Count;
}

uint32 BufferDX11::GetElementStride() const
{
	return m_Stride;
}

uint32 BufferDX11::GetElementOffset() const
{
    return m_Offset;
}

void BufferDX11::Load(const std::shared_ptr<IFile>& File)
{

}

void BufferDX11::Save(const std::shared_ptr<IFile>& File)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
	CHECK_HR(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_pBuffer, 0, D3D11_MAP_READ, 0, &mappedResource));

	std::vector<uint8> data;
	data.resize(mappedResource.RowPitch * mappedResource.DepthPitch);

	memcpy_s(data.data(), data.size(), mappedResource.pData, data.size());

	m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_pBuffer, 0);
}
