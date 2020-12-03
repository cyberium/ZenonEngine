#include "stdafx.h"

// General
#include "DrawLightFrustumPass.h"

const float cBBoxSizeIncrement = 0.05f;

CDrawLightFrustumPass::CDrawLightFrustumPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
	, m_IsDirty(false)
{
}

CDrawLightFrustumPass::~CDrawLightFrustumPass()
{}

void CDrawLightFrustumPass::SetNeedRefresh()
{
	m_IsDirty = true;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawLightFrustumPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_PointBox = GetRenderDevice().GetPrimitivesFactory().CreateCube();

	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	
	//m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	m_MaterialDebug = MakeShared(MaterialDebug, GetRenderDevice());
	m_MaterialDebug->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	return shared_from_this();
}

EVisitResult CDrawLightFrustumPass::Visit(const std::shared_ptr<ISceneNode>& node)
{
	if (auto lightComp = node->GetComponentT<ILightComponent3D>())
		return EVisitResult::AllowVisitContent;
	
	return EVisitResult::AllowVisitChilds;
}

EVisitResult CDrawLightFrustumPass::Visit(const std::shared_ptr<ILight3D>& Light)
{
	{
		BindPerObjectData(PerObject());

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateFrustum(Light->GetFrustum());

		m_MaterialDebug->Bind(GetPipeline().GetPixelShaderPtr());
		geom->Render(GetPipeline().GetVertexShaderPtr());
		m_MaterialDebug->Unbind(GetPipeline().GetPixelShaderPtr());
	}

	return EVisitResult::AllowAll;
}
