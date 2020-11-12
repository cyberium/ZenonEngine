#include "stdafx.h"

// General
#include "MaterialParticlePass.h"

CMaterialParticlePass::CMaterialParticlePass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	m_Geometry = GetRenderDevice().GetObjectsFactory().CreateGeometry();
	m_Geometry->SetPrimitiveTopology(PrimitiveTopology::PointList);

	m_GeomParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 1000, sizeof(SGPUParticle), EAccess::CPUWrite);
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
	GetPipeline().GetBlendState()->SetBlendMode(//alphaBlending
		IBlendState::BlendMode(true, false,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One,
		IBlendState::BlendOperation::Add,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One)
	);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::GeometryShader, geomShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

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
EVisitResult CMaterialParticlePass::Visit(const ISceneNode * SceneNode)
{
	if (false == SceneNode->IsComponentExistsT<IParticleComponent3D>())
		return EVisitResult::AllowVisitChilds;
	BindPerObjectData(PerObject());
	return EVisitResult::AllowAll;
}

EVisitResult CMaterialParticlePass::Visit(const IModel * Model)
{
	return EVisitResult::Block;
}

EVisitResult CMaterialParticlePass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult CMaterialParticlePass::Visit(const IParticleSystem * ParticlesSystem)
{
	const auto& partilces = ParticlesSystem->GetGPUParticles();
	if (partilces.empty())
		return EVisitResult::Block;

	if (partilces.size() > m_GeomParticlesBuffer->GetElementCount())
		m_GeomParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(partilces, EAccess::CPUWrite);
	else
		m_GeomParticlesBuffer->Set(partilces);

	m_GeomShaderParticlesBufferParameter->SetStructuredBuffer(m_GeomParticlesBuffer);
	m_GeomShaderParticlesBufferParameter->Bind();

	const auto& shaders = GetPipeline().GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();
	const auto& pixelShader = shaders.at(EShaderType::PixelShader).get();

	// Bind material
	if (ParticlesSystem->GetTexture())
		ParticlesSystem->GetTexture()->Bind(0, pixelShader, IShaderParameter::Type::Texture);

	// Draw geom
	SGeometryDrawArgs args;
	args.VertexCnt = partilces.size();
	m_Geometry->Render(pixelShader, args);

	m_GeomShaderParticlesBufferParameter->Unbind();

	return EVisitResult::AllowAll;
}
