#include "stdafx.h"

#include "StructuredBufferDX11.h"

StructuredBufferDX11::StructuredBufferDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: CStructuredBufferBase(RenderDeviceDX11)
	, m_RenderDeviceDX11(RenderDeviceDX11)
	, m_bIsDirty(false)
{}

StructuredBufferDX11::~StructuredBufferDX11()
{}


//
// IBuffer
//
bool StructuredBufferDX11::Bind(uint32 ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	if (m_bIsDirty)
	{
		Commit();
		m_bIsDirty = false;
	}

	if (parameterType == IShaderParameter::Type::Buffer && m_pSRV)
	{
		ID3D11ShaderResourceView* srv[] = { m_pSRV };

		switch (shader->GetType())
		{
		case EShaderType::VertexShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationControlShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationEvaluationShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::GeometryShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::PixelShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWBuffer && m_pUAV)
	{
		ID3D11UnorderedAccessView* uav[] = { m_pUAV };
		switch (shader->GetType())
		{
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}

	return true;
}

void StructuredBufferDX11::UnBind(uint32 ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11UnorderedAccessView* uav[] = { nullptr };
	ID3D11ShaderResourceView* srv[] = { nullptr };

	if (parameterType == IShaderParameter::Type::Buffer)
	{
		switch (shader->GetType())
		{
		case EShaderType::VertexShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationControlShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationEvaluationShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::GeometryShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::PixelShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWBuffer)
	{
		switch (shader->GetType())
		{
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}

}

void StructuredBufferDX11::Copy(const IBuffer* other) const
{
	Copy(dynamic_cast<const IStructuredBuffer*>(other));
}



//
// IStructuredBuffer
//
void StructuredBufferDX11::Copy(const IStructuredBuffer* other)
{
	const StructuredBufferDX11* srcBuffer = dynamic_cast<const StructuredBufferDX11*>(other);

	if (srcBuffer->m_bIsDirty)
	{
		srcBuffer->Commit();
	}

	if (srcBuffer && srcBuffer != this && GetElementCount() * GetElementStride() == srcBuffer->GetElementCount() * srcBuffer->GetElementStride())
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		throw CznRenderException("Source buffer is not compatible with this buffer.");
	}

	if (((uint8_t)GetAccess() & (uint8_t)EAccess::CPURead) != 0)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		CHECK_HR(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_pBuffer, 0, D3D11_MAP_READ, 0, &mappedResource));

		memcpy_s(GetDataEx().data(), GetDataEx().size(), mappedResource.pData, GetData().size());

		m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_pBuffer, 0);
	}
}

void StructuredBufferDX11::Set(void* data, size_t elementSize, size_t numElements)
{
	SetData(data, numElements * elementSize);
	m_bIsDirty = true;
}

void StructuredBufferDX11::Clear()
{
	if (m_pUAV)
	{
		FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_RenderDeviceDX11.GetDeviceContextD3D11()->ClearUnorderedAccessViewFloat(m_pUAV, clearColor);
	}
}



//
// IStructuredBufferDX11
//
ID3D11UnorderedAccessView* StructuredBufferDX11::GetUnorderedAccessView() const
{
	return m_pUAV;
}



//
// Protected
//
void StructuredBufferDX11::Commit() const
{
	if (m_bIsDirty && IsDynamic() && m_pBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = { };
		CHECK_HR(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		memcpy_s(mappedResource.pData, GetData().size(), GetData().data(), GetData().size());

		m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_pBuffer, 0);

		m_bIsDirty = false;
	}
}


//
// IStructuredBufferPrivate
//
void StructuredBufferDX11::DoInitializeStructuredBuffer()
{
	// Create a GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = GetElementCount() * GetElementStride();
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = GetElementStride();

	if (((uint32)GetAccess() & (uint32)EAccess::CPURead) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_STAGING;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	else if (((uint32)GetAccess() & (uint32)EAccess::CPUWrite) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if ((((uint32)GetAccess() & (uint32)EAccess::GPUWrite) != 0) && !IsDynamic()) // UAV can't be CPU writable buffer
		{
			bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
	}

	D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
	subResourceData.pSysMem = (void*)GetData().data();
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBuffer(&bufferDesc, &subResourceData, &m_pBuffer), L"Failed to create read/write buffer.");

	if ((bufferDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = GetElementCount();

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateShaderResourceView(m_pBuffer, &srvDesc, &m_pSRV), L"Failed to create shader resource view.");
	}

	if ((bufferDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = GetElementCount();
		uavDesc.Buffer.Flags = 0;

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateUnorderedAccessView(m_pBuffer, &uavDesc, &m_pUAV), L"Failed to create unordered access view.");
	}
}
