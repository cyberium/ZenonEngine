#include "stdafx.h"

// General
#include "MaterialParticlePass.h"

// Additional
#include "Materials/MaterialParticle.h"

CMaterialParticlePass::CMaterialParticlePass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{
	m_Geometry = GetRenderDevice().GetObjectsFactory().CreateGeometry();
	m_Geometry->SetPrimitiveTopology(PrimitiveTopology::PointList);

	m_GeomParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 1000, sizeof(SParticle), EAccess::CPUWrite);
}

CMaterialParticlePass::~CMaterialParticlePass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterialParticlePass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> geomShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Particle.hlsl", "VS_main");
		geomShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::GeometryShader, "3D/Particle.hlsl", "GS_Billboard");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Particle.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	//std::vector<SCustomInputElement> elements;
	//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending
		/*IBlendState::BlendMode(true, false,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One,
		IBlendState::BlendOperation::Add,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One)*/
	);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::GeometryShader, geomShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	// 'PerObject' in geom shader
	m_GeomShaderPerObjectParameter = &geomShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_GeomShaderPerObjectParameter->IsValid());

	// 'Particles' in geom shader
	m_GeomShaderParticlesBufferParameter = &geomShader->GetShaderParameterByName("Particles");
	_ASSERT(m_GeomShaderParticlesBufferParameter->IsValid());

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(0, sampler);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CMaterialParticlePass::Visit(const ISceneNode3D * SceneNode)
{
	EVisitResult visitResult = Base3DPass::Visit(SceneNode);

	if (m_GeomShaderPerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		m_GeomShaderPerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
		m_GeomShaderPerObjectParameter->Bind();
	}

	return visitResult;
}

EVisitResult CMaterialParticlePass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CMaterialParticlePass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialParticle* objMaterial = dynamic_cast<const MaterialParticle*>(Material);
	if (objMaterial != nullptr)
		return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
	
	return EVisitResult::Block;
}

EVisitResult CMaterialParticlePass::Visit(const IParticleSystem * ParticlesSystem)
{
	const std::vector<SParticle>& partilces = ParticlesSystem->GetParticles();
	if (partilces.empty())
		return EVisitResult::Block;

	if (partilces.size() > m_GeomParticlesBuffer->GetElementCount())
		m_GeomParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(partilces, EAccess::CPUWrite);
	else
		m_GeomParticlesBuffer->Set(partilces);

	m_GeomShaderParticlesBufferParameter->SetStructuredBuffer(m_GeomParticlesBuffer);
	m_GeomShaderParticlesBufferParameter->Bind();

	// Blend state
	if (ParticlesSystem->GetBlendState())
		ParticlesSystem->GetBlendState()->Bind();

	// Bind material
	if (ParticlesSystem->GetMaterial())
		ParticlesSystem->GetMaterial()->Bind(GetRenderEventArgs().PipelineState->GetShaders());

	// Draw geom
	SGeometryDrawArgs args;
	args.VertexCnt = partilces.size();
	m_Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), args);

	return EVisitResult::AllowAll;
}
