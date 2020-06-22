#include "stdafx.h"

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "GeometryDX11.h"

namespace
{
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopologyToDX11PrimitiveTopology(PrimitiveTopology PrimitiveTopology)
	{
		switch (PrimitiveTopology)
		{
			case PrimitiveTopology::PointList:
				return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			case PrimitiveTopology::LineList:
				return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; // TODO REMOVE ME!
			case PrimitiveTopology::LineStrip:
				return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case PrimitiveTopology::TriangleList:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveTopology::TriangleStrip:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
		throw CznRenderException("GeometryDX11: Invalid primitive topology.");
	}
}

GeometryDX11::GeometryDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: GeometryBase(RenderDeviceDX11)
	, m_RenderDeviceDX11(RenderDeviceDX11)
{}

GeometryDX11::~GeometryDX11()
{}



//
// IGeometry
//
void GeometryDX11::Render(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const
{
	Render_BindAllBuffers(RenderEventArgs, VertexShader);
	Render_Draw(GeometryDrawArgs);
	Render_UnbindAllBuffers(RenderEventArgs, VertexShader);
}



//
// IGeometryInternal
//
void GeometryDX11::Render_BindAllBuffers(const RenderEventArgs & RenderEventArgs, const IShader * VertexShader) const
{
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const auto& buffer : m_VertexBuffers)
		{
			UINT slotID = VertexShader->GetInputLayout().GetSemanticSlot(buffer.first);
			if (slotID != UINT_MAX)
				buffer.second->Bind(slotID, VertexShader, IShaderParameter::Type::Buffer);
		}
	}

	if (m_pIndexBuffer != NULL)
		m_pIndexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
}

void GeometryDX11::Render_Draw(const SGeometryDrawArgs GeometryDrawArgs) const
{
	SGeometryDrawArgs args = FixGeometryDrawArgs(GeometryDrawArgs);

	m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(PrimitiveTopologyToDX11PrimitiveTopology(GetPrimitiveTopology()));

	if (m_pIndexBuffer != NULL)
	{
		if (args.InstanceCnt != UINT32_MAX)
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexedInstanced(args.IndexCnt, args.InstanceCnt, args.IndexStartLocation, args.VertexStartLocation, args.InstanceCnt);
		}
		else
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexed(args.IndexCnt, args.IndexStartLocation, args.VertexStartLocation);
		}	
	}
	else
	{
		if (args.InstanceCnt != UINT32_MAX)
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawInstanced(args.VertexCnt, args.InstanceCnt, args.VertexStartLocation, args.InstanceStartIndex);
		}
		else
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->Draw(args.VertexCnt, args.VertexStartLocation);
		}		
	}
}

void GeometryDX11::Render_UnbindAllBuffers(const RenderEventArgs & RenderEventArgs, const IShader * VertexShader) const
{
	if (m_pIndexBuffer != NULL)
		m_pIndexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);

	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const auto& buffer : m_VertexBuffers)
		{
			UINT slotID = VertexShader->GetInputLayout().GetSemanticSlot(buffer.first);
			if (slotID != UINT_MAX)
				buffer.second->UnBind(slotID, VertexShader, IShaderParameter::Type::Buffer);
		}
	}
}
