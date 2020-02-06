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

bool GeometryDX11::Render(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const
{
	SGeometryDrawArgs args = FixGeometryDrawArgs(GeometryDrawArgs);

	BindVertexBuffersToVertexShader(VertexShader);
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexed(args.IndexCnt, args.IndexStartLocation, args.VertexStartLocation);
			m_pIndexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->Draw(args.VertexCnt, args.VertexStartLocation);
		}
	}
	UnbindVertexBuffersFromVertexShader(VertexShader);

	return true;
}

bool GeometryDX11::RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs) const
{
	SGeometryDrawInstancedArgs args = FixGeometryDrawInstancedArgs(GeometryDrawInstancedArgs);

	BindVertexBuffersToVertexShader(VertexShader);
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexedInstanced(args.IndexCnt, args.InstanceCnt, args.IndexStartLocation, args.VertexStartLocation, args.InstanceCnt);
			m_pIndexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawInstanced(args.VertexCnt, args.InstanceCnt, args.VertexStartLocation, args.InstanceStartIndex);
		}
	}
	UnbindVertexBuffersFromVertexShader(VertexShader);

	return true;
}
