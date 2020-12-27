#include "stdafx.h"

// General
#include "ParticlesPass.h"

CParticlesPass::CParticlesPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	m_Geometry = GetRenderDevice().GetObjectsFactory().CreateGeometry();
	m_Geometry->SetPrimitiveTopology(PrimitiveTopology::PointList);
}

CParticlesPass::~CParticlesPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CParticlesPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);
	
	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Particle.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(vertexShader);
	}

	{
		std::shared_ptr<IShader> geomShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::GeometryShader, "3D/Particle.hlsl", "GS_Billboard");
		
		m_GeomShaderParticlesBufferParameter = geomShader->GetShaderParameterByName("Particles");
		_ASSERT(m_GeomShaderParticlesBufferParameter);

		GetPipeline().SetShader(geomShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Particle.hlsl", "PS_main");
		GetPipeline().SetShader(pixelShader);
	}

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(//alphaBlending
		IBlendState::BlendMode(true, false,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One,
		IBlendState::BlendOperation::Add,
		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::One)
	);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	
	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CParticlesPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	if (false == SceneNode->IsComponentExistsT<IParticleComponent3D>())
		return EVisitResult::AllowVisitChilds;
	BindPerObjectData(PerObject());
	return EVisitResult::AllowAll;
}

EVisitResult CParticlesPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return EVisitResult::Block;
}

EVisitResult CParticlesPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult CParticlesPass::Visit(const std::shared_ptr<IParticleSystem>& ParticlesSystem)
{
	const auto& partilces = ParticlesSystem->GetGPUParticles();
	if (partilces.empty())
		return EVisitResult::Block;

	if (m_GeomParticlesBuffer == nullptr || partilces.size() != m_GeomParticlesBuffer->GetElementCount())
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
		ParticlesSystem->GetTexture()->Bind(0, pixelShader, IShaderParameter::EType::Texture);

	// Draw geom
	SGeometryDrawArgs args;
	args.VertexCnt = partilces.size();
	m_Geometry->Render(vertexShader, args);

	m_GeomShaderParticlesBufferParameter->Unbind();

	return EVisitResult::AllowAll;
}
