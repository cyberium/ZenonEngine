#include "stdafx.h"

// General
#include "MaterialModelPass.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialModelPass::CMaterialModelPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{
	
}

CMaterialModelPass::~CMaterialModelPass()
{}


IShaderParameter * CMaterialModelPass::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterialModelPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Model_Forward.hlsl", "VS_PTN");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	GetPipeline().SetSampler(0, sampler);

	auto samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(1, samplerClamp);

	m_ShaderBonesBufferParameter = &vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	m_ShaderLightsBufferParameter = &pixelShader->GetShaderParameterByName("Lights");
	//_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	return shared_from_this();
}



EVisitResult CMaterialModelPass::Visit(const ISceneNode3D * SceneNode)
{
	auto skeletonComponent = SceneNode->GetComponent<ISkeletonComponent3D>();
	if (skeletonComponent != nullptr)
		if (m_ShaderBonesBufferParameter->IsValid())
			m_ShaderBonesBufferParameter->Set(skeletonComponent->GetBonesBuffer());

	return Base3DPass::Visit(SceneNode);
}

//
// IVisitor
//
EVisitResult CMaterialModelPass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CMaterialModelPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == true)
	//	return false;
	if (Material)
		Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());


	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Bind();
	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Bind();

	Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);

	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Unbind();
	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Unbind();

	if (Material)
		Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return AllowAll;
}
