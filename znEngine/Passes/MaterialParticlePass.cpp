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

	m_ParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 1000, sizeof(SParticle), CPUAccess::Write);
}

CMaterialParticlePass::~CMaterialParticlePass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterialParticlePass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> geomShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_PARTICLE", "VS_main");
		geomShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::GeometryShader, "IDB_SHADER_3D_PARTICLE", "GS_Billboard");
		pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_PARTICLE", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	//std::vector<SCustomVertexElement> elements;
	//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(alphaBlending
		/*IBlendState::BlendMode(true, false,
		IBlendState::BlendFactor::One, IBlendState::BlendFactor::One,
		IBlendState::BlendOperation::Add,
		IBlendState::BlendFactor::One, IBlendState::BlendFactor::One)*/
	);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, vertexShader);
	Pipeline->SetShader(EShaderType::GeometryShader, geomShader);
	Pipeline->SetShader(EShaderType::PixelShader, pixelShader);

	m_ShaderParticlesBufferParameter = &geomShader->GetShaderParameterByName("Particles");
	_ASSERT(m_ShaderParticlesBufferParameter->IsValid());

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	Pipeline->SetSampler(1, sampler);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
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

	if (partilces.size() > m_ParticlesBuffer->GetElementCount())
		m_ParticlesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(partilces, CPUAccess::Write);
	else
		m_ParticlesBuffer->Set(partilces);

	m_ShaderParticlesBufferParameter->SetStructuredBuffer(m_ParticlesBuffer);
	m_ShaderParticlesBufferParameter->Bind();

	SGeometryDrawArgs args;
	args.VertexCnt = ParticlesSystem->GetParticles().size();
	m_Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), args);

	return EVisitResult();
}
