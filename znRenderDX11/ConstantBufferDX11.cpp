#include "stdafx.h"

#include "ConstantBufferDX11.h"

ConstantBufferDX11::ConstantBufferDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: CBufferBase(RenderDeviceDX11, IBuffer::BufferType::ConstantBuffer)
	, m_RenderDeviceDX11(RenderDeviceDX11)
	, m_bIsDirty(false)
{}

ConstantBufferDX11::~ConstantBufferDX11()
{}



//
// IBuffer
//
bool ConstantBufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	if (m_bIsDirty)
	{
		Commit();
		m_bIsDirty = false;
	}

	ID3D11Buffer* pBuffers[] = { m_pBuffer };

	switch (shader->GetShaderType())
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
			_ASSERT(FALSE);
	}

	return true;
}

void ConstantBufferDX11::UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* pBuffers[] = { nullptr };

	switch (shader->GetShaderType())
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



//
// IBufferPrivate
//
void ConstantBufferDX11::DoInitializeBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = GetElementCount();
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBuffer(&bufferDesc, nullptr, &m_pBuffer), L"Failed to create constant buffer");
}



//
// IConstantBuffer
//
void ConstantBufferDX11::Copy(const IConstantBuffer* other) const
{
	const ConstantBufferDX11* srcBuffer = dynamic_cast<const ConstantBufferDX11*>(other);

	if (srcBuffer && srcBuffer != this && GetElementCount() == srcBuffer->GetElementCount())
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		throw CznRenderException("Source buffer is not compatible with this buffer.");
	}
}

void ConstantBufferDX11::Set(const void* data, size_t size)
{
	if (size != GetElementCount())
		throw CException(L"ConstantBufferDX11: Buffers sizes mistmath. Current: '%d', New: '%d'.", GetElementCount(), size);

	SetData(data, size);
	m_bIsDirty = true;
}



//
// Pivate
//
void ConstantBufferDX11::Commit() const
{
	if (m_bIsDirty && m_pBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = { };
		CHECK_HR(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		memcpy_s(mappedResource.pData, GetElementCount(), GetData().data(), GetData().size());

		m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_pBuffer, 0);
	}
}
