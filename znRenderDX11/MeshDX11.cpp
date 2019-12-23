#include <stdafx.h>

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "MeshDX11.h"

MeshDX11::MeshDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	
	, m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)

	, m_pDeviceContext(nullptr)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

MeshDX11::~MeshDX11()
{}

void MeshDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	switch (_topology)
	{
	case PrimitiveTopology::PointList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case PrimitiveTopology::LineList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; // TODO REMOVE ME!
		break;
	case PrimitiveTopology::LineStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case PrimitiveTopology::TriangleList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case PrimitiveTopology::TriangleStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	}
}

bool MeshDX11::Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
    if (indexCnt == 0 && m_pIndexBuffer != nullptr)
        indexCnt = m_pIndexBuffer->GetElementCount();

	if (vertexCnt == 0)
	{
		if (m_VertexBuffer != nullptr)
			vertexCnt = m_VertexBuffer->GetElementCount();
		else
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
	}

	std::shared_ptr<IShader> pVS = nullptr;
	std::shared_ptr<IShader> pPS = nullptr;
	ShaderMap shadersMap;

	if (m_pMaterial)
	{
		pVS = m_pMaterial->GetShader(IShader::ShaderType::VertexShader);
		pPS = m_pMaterial->GetShader(IShader::ShaderType::PixelShader);
		shadersMap = m_pMaterial->GetShaders();
	}

	if (pVS == nullptr && pPS == nullptr)
	{
		pVS = renderArgs->PipelineState->GetShader(IShader::ShaderType::VertexShader);
		pPS = renderArgs->PipelineState->GetShader(IShader::ShaderType::PixelShader);
		shadersMap = renderArgs->PipelineState->GetShaders();
	}

	if (m_pMaterial)
	{
		m_pMaterial->Bind(shadersMap);
	}

	if (pVS)
	{
		const std::shared_ptr<IShaderParameter>& perObjectParameter = pVS->GetShaderParameterByName("PerObject");
		if (perObjectParameter->IsValid() && perObject != nullptr)
		{
			perObjectParameter->SetConstantBuffer(perObject);
			perObjectParameter->Bind();
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Bind(0, pVS.get(), IShaderParameter::Type::Buffer);
		}
		else
		{
			for (const BufferMap::value_type& buffer : m_VertexBuffers)
			{
				const BufferBinding& binding = buffer.first;
				if (pVS->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = pVS->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->Bind(slotID, pVS.get(), IShaderParameter::Type::Buffer);
				}
			}
		}

		m_pDeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, pVS.get(), IShaderParameter::Type::Buffer);
			m_pDeviceContext->DrawIndexed(indexCnt, indexStartLocation, vertexStartLocation);
			m_pIndexBuffer->UnBind(0, pVS.get(), IShaderParameter::Type::Buffer);
		}
		else
		{
			m_pDeviceContext->Draw(vertexCnt, vertexStartLocation);
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->UnBind(0, pVS.get(), IShaderParameter::Type::Buffer);
		}
		else
		{
			for (const BufferMap::value_type& buffer : m_VertexBuffers)
			{
				const BufferBinding& binding = buffer.first;
				if (pVS->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = pVS->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->UnBind(slotID, pVS.get(), IShaderParameter::Type::Buffer);
				}
			}
		}
	}

	if (m_pMaterial)
	{
		m_pMaterial->Unbind(shadersMap);
	}

	return true;
}
