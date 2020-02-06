#include "stdafx.h"

#include "ConstantBufferDX11.h"

ConstantBufferDX11::ConstantBufferDX11(IRenderDeviceDX11& RenderDeviceDX11, size_t size)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_BufferSize(size)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)m_BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBuffer(&bufferDesc, nullptr, &m_pBuffer)))
	{
		_ASSERT_EXPR(false, "Failed to create constant buffer for shader.");
		return;
	}
}

ConstantBufferDX11::~ConstantBufferDX11()
{}



//
// IBuffer
//
bool ConstantBufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	bool result = true;

	ID3D11Buffer* pBuffers[] = { m_pBuffer };

	switch (shader->GetType())
	{
	case EShaderType::VertexShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::TessellationControlShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::TessellationEvaluationShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::GeometryShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::PixelShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::ComputeShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	default:
		result = false;
		break;
	}

	return result;
}

void ConstantBufferDX11::UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* pBuffers[] = { nullptr };

	switch (shader->GetType())
	{
	case EShaderType::VertexShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::TessellationControlShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::TessellationEvaluationShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::GeometryShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::PixelShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case EShaderType::ComputeShader:
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	}
}

void ConstantBufferDX11::Copy(const IBuffer* other) const
{
	this->Copy(dynamic_cast<const IConstantBuffer*>(other));
}

IBuffer::BufferType ConstantBufferDX11::GetType() const
{
	return BufferType::ConstantBuffer;
}

uint32 ConstantBufferDX11::GetElementCount() const
{
	return 1;
}

uint32 ConstantBufferDX11::GetElementStride() const
{
	return 0;
}

uint32 ConstantBufferDX11::GetElementOffset() const
{
	return 0;
}



//
// IConstantBuffer
//
void ConstantBufferDX11::Copy(const IConstantBuffer* other) const
{
	const ConstantBufferDX11* srcBuffer = dynamic_cast<const ConstantBufferDX11*>(other);

	if (srcBuffer && srcBuffer != this && m_BufferSize == srcBuffer->m_BufferSize)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		Log::Error("Source buffer is not compatible with this buffer.");
	}
}

void ConstantBufferDX11::Set(const void* data, size_t size)
{
	_ASSERT(size == m_BufferSize);

	D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
	if (FAILED(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		_ASSERT_EXPR(false, "Failed to map constant buffer.");
		return;
	}

	memcpy(mappedResource.pData, data, m_BufferSize);

	m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_pBuffer, 0);
}
