#include "stdafx.h"

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "GeometryDX11.h"

GeometryDX11::GeometryDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{}

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

bool GeometryDX11::Render(const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawArgs& GeometryDrawArgs) const
{
	UINT indexStartLocation = GeometryDrawArgs.IndexStartLocation;
	UINT indexCnt = GeometryDrawArgs.IndexCnt;
    if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
        indexCnt = m_pIndexBuffer->GetElementCount();

	UINT vertexStartLocation = GeometryDrawArgs.VertexStartLocation;
	UINT vertexCnt = GeometryDrawArgs.VertexCnt;
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

	const auto& vertexShader = ShadersMap.at(EShaderType::VertexShader);
	_ASSERT(vertexShader != nullptr);


	BindVertexBuffersToShader(*vertexShader);


	//
	// Draw geometry
	//
	m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Bind(0, vertexShader.get(), IShaderParameter::Type::Buffer);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexed(indexCnt, indexStartLocation, vertexStartLocation);
		m_pIndexBuffer->UnBind(0, vertexShader.get(), IShaderParameter::Type::Buffer);
	}
	else
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->Draw(vertexCnt, vertexStartLocation);
	}


	UnbindVertexBuffersFromShader(*vertexShader);

	return true;
}

bool GeometryDX11::RenderInstanced(const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawInstancedArgs& GeometryDrawInstancedArgs) const
{
	UINT instanceStart = GeometryDrawInstancedArgs.InstanceStartIndex;
	UINT instanceCnt = GeometryDrawInstancedArgs.InstanceCnt;
	if (instanceCnt == UINT_MAX)
		_ASSERT_EXPR(false, L"GeometryDX11: Specify instance count same as instance buffer size.");

	UINT indexStartLocation = GeometryDrawInstancedArgs.IndexStartLocation;
	UINT indexCnt = GeometryDrawInstancedArgs.IndexCnt;
	if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		indexCnt = m_pIndexBuffer->GetElementCount();

	UINT vertexStartLocation = GeometryDrawInstancedArgs.VertexStartLocation;
	UINT vertexCnt = GeometryDrawInstancedArgs.VertexCnt;
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

	const auto& vertexShader = ShadersMap.at(EShaderType::VertexShader);
	_ASSERT(vertexShader != nullptr);


	BindVertexBuffersToShader(*vertexShader);

	m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Bind(0, vertexShader.get(), IShaderParameter::Type::Buffer);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexedInstanced(indexCnt, instanceCnt, indexStartLocation, vertexStartLocation, instanceStart);
		m_pIndexBuffer->UnBind(0, vertexShader.get(), IShaderParameter::Type::Buffer);
	}
	else
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawInstanced(vertexCnt, instanceCnt, vertexStartLocation, instanceStart);
	}

	UnbindVertexBuffersFromShader(*vertexShader);

	return true;
}
