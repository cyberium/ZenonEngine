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

void GeometryDX11::Render(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const
{
	Render_BindAllBuffers(RenderEventArgs, VertexShader);
	Render_Draw(GeometryDrawArgs);
	Render_UnbindAllBuffers(RenderEventArgs, VertexShader);
}

void GeometryDX11::RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs) const
{
	Render_BindAllBuffers(RenderEventArgs, VertexShader);
	Render_DrawInstanced(GeometryDrawInstancedArgs);
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

	m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexed(args.IndexCnt, args.IndexStartLocation, args.VertexStartLocation);
	else
		m_RenderDeviceDX11.GetDeviceContextD3D11()->Draw(args.VertexCnt, args.VertexStartLocation);
}

void GeometryDX11::Render_DrawInstanced(const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs) const
{
	SGeometryDrawInstancedArgs args = FixGeometryDrawInstancedArgs(GeometryDrawInstancedArgs);

	m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawIndexedInstanced(args.IndexCnt, args.InstanceCnt, args.IndexStartLocation, args.VertexStartLocation, args.InstanceCnt);
	else
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DrawInstanced(args.VertexCnt, args.InstanceCnt, args.VertexStartLocation, args.InstanceStartIndex);
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
