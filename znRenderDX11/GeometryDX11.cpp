#include "stdafx.h"

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "GeometryDX11.h"

GeometryDX11::GeometryDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	, m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_pDeviceContext(nullptr)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

GeometryDX11::~GeometryDX11()
{}

void GeometryDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
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

bool GeometryDX11::Render(const RenderEventArgs* renderArgs, const IShader* VertexShader, const IConstantBuffer* perObject, const SRenderGeometryParams& RenderGeometryParams)
{
	UINT indexCnt = RenderGeometryParams.IndexCnt;
    if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
        indexCnt = m_pIndexBuffer->GetElementCount();

	UINT indexStartLocation = RenderGeometryParams.IndexStartLocation;
	_ASSERT(indexStartLocation < indexCnt);

	UINT vertexCnt = RenderGeometryParams.VertexCnt;
	if (vertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			vertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	UINT vertexStartLocation = RenderGeometryParams.VertexStartLocation;
	_ASSERT(vertexStartLocation < vertexCnt);

	if (VertexShader)
	{
		const std::shared_ptr<IShaderParameter>& perObjectParameter = VertexShader->GetShaderParameterByName("PerObject");
		if (perObjectParameter->IsValid() && perObject != nullptr)
		{
			perObjectParameter->SetConstantBuffer(perObject);
			perObjectParameter->Bind();
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			for (const BufferMap::value_type& buffer : m_VertexBuffers)
			{
				const BufferBinding& binding = buffer.first;
				if (VertexShader->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = VertexShader->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->Bind(slotID, VertexShader, IShaderParameter::Type::Buffer);
				}
			}
		}

		m_pDeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
			m_pDeviceContext->DrawIndexed(indexCnt, indexStartLocation, vertexStartLocation);
			m_pIndexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			m_pDeviceContext->Draw(vertexCnt, vertexStartLocation);
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			for (const BufferMap::value_type& buffer : m_VertexBuffers)
			{
				const BufferBinding& binding = buffer.first;
				if (VertexShader->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = VertexShader->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->UnBind(slotID, VertexShader, IShaderParameter::Type::Buffer);
				}
			}
		}
	}

	return true;
}
