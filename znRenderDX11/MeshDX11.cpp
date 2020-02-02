#include "stdafx.h"

// General
#include "MeshDX11.h"

// Additional
#include "GeometryDX11.h"

MeshDX11::MeshDX11(IRenderDeviceDX11& RenderDeviceDX11)
{
	m_Geometry = std::make_unique<GeometryDX11>(RenderDeviceDX11);
}

MeshDX11::~MeshDX11()
{}

void MeshDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Geometry->SetPrimitiveTopology(_topology);
}

bool MeshDX11::Render(const RenderEventArgs& renderArgs, const std::shared_ptr<IConstantBuffer>& perObject, const SGeometryPartParams& GeometryPartParams)
{
	return true;
}
