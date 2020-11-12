#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IScene& scene)
    : ScenePassPipelined(scene)
	//, m_PerObjectParameter(nullptr)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

Base3DPass::~Base3DPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> Base3DPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	GetPipeline().SetSampler(0, sampler);

	auto samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(1, samplerClamp);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult Base3DPass::Visit(const ISceneNode* SceneNode)
{
	BindPerObjectData(PerObject(SceneNode->GetWorldTransfom()));
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IModel * Model)
{
	Model->Render();
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const auto& shaders = GetPipeline().GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

	Material->Bind(shaders);
	Geometry->Render(vertexShader, GeometryDrawArgs);
	Material->Unbind(shaders);
	return EVisitResult::AllowAll;
}



//
// Protected
//
void Base3DPass::BindPerObjectData(const PerObject& PerObject)
{
	m_PerObjectConstantBuffer->Set(PerObject);

	for (const auto& shaderIt : GetPipeline().GetShaders())
	{
		auto& perObjectParam = shaderIt.second->GetShaderParameterByName("PerObject");
		if (perObjectParam.IsValid())
		{
			perObjectParam.SetConstantBuffer(m_PerObjectConstantBuffer);
			perObjectParam.Bind();
		}
	}
}
