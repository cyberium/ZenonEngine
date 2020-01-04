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
	for (const auto& materialForGeometryPart : m_MaterialForGeometryParts)
	{
		const std::shared_ptr<const IMaterial>& material = materialForGeometryPart.Material;

		std::shared_ptr<const IShader> pVS = nullptr;
		std::shared_ptr<const IShader> pPS = nullptr;
		ShaderMap shadersMap;

		if (material)
		{
			pVS = material->GetShader(SShaderType::VertexShader);
			pPS = material->GetShader(SShaderType::PixelShader);
			shadersMap = material->GetShaders();
		}

		if (pVS == nullptr && pPS == nullptr)
		{
			pVS = renderArgs->PipelineState->GetShader(SShaderType::VertexShader);
			pPS = renderArgs->PipelineState->GetShader(SShaderType::PixelShader);
			shadersMap = renderArgs->PipelineState->GetShaders();
		}

		const IRenderPass* constRenderPass = dynamic_cast<const IRenderPass*>(renderArgs->Caller);
		IRenderPass* renderPass = const_cast<IRenderPass*>(constRenderPass);
		m_Geometry->Accept(renderPass, perObject, shadersMap, material.get(), ((GeometryPartParams.IsDefault() == false) ? GeometryPartParams : materialForGeometryPart.GeometryPartParams));
	}

	return true;
}
