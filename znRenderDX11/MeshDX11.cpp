#include "stdafx.h"

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "MeshDX11.h"

MeshDX11::MeshDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	, m_pDeviceContext(nullptr)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	m_Geometry = std::make_shared<GeometryDX11>(m_pDevice);
}

MeshDX11::~MeshDX11()
{}

void MeshDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Geometry->SetPrimitiveTopology(_topology);
}

bool MeshDX11::Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams)
{
	return true;
}
