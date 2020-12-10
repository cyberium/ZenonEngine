#include "stdafx.h"

// General
#include "DrawLightFrustumPass.h"

// Additional
#include "Materials/MaterialDebug.h"

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

	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}

	//m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	auto material = MakeShared(MaterialDebug, GetRenderDevice());
	material->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_MaterialDebug = material;

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);	
	return shared_from_this();
}

EVisitResult CDrawLightFrustumPass::Visit(const std::shared_ptr<ISceneNode>& node)
{
	if (auto lightComp = node->GetComponentT<ILightComponent3D>())
		return EVisitResult::AllowVisitContent;
	
	return EVisitResult::AllowVisitChilds;
}

EVisitResult CDrawLightFrustumPass::Visit(const std::shared_ptr<ILight>& Light)
{
	if (false == Light->IsEnabled())
		return EVisitResult::Block;

	if (Light->GetType() == ELightType::Point)
		return EVisitResult::Block;

	{
		BindPerObjectData(PerObject());

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateFrustum(Light->GetFrustum());

		m_MaterialDebug->Bind(GetPipeline().GetPixelShaderPtr());
		geom->Render(GetPipeline().GetVertexShaderPtr());
		m_MaterialDebug->Unbind(GetPipeline().GetPixelShaderPtr());
	}

	return EVisitResult::AllowAll;
}
