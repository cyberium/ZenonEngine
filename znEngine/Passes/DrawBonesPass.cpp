#include "stdafx.h"

// General
#include "DrawBonesPass.h"

// Include 
#include "Materials/MaterialDebug.h"

CDrawBonesPass::CDrawBonesPass(IScene& scene)
	: Base3DPass(scene)
{
}

CDrawBonesPass::~CDrawBonesPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawBonesPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_ConeGeometry = GetRenderDevice().GetPrimitivesFactory().CreateCone();
	m_SphereGeometry = GetRenderDevice().GetPrimitivesFactory().CreateSphere();
	m_Material = MakeShared(MaterialDebug, GetRenderDevice());

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawBonesPass::Visit(const ISceneNode * CSceneNode)
{
	std::shared_ptr<IModelsComponent3D> modelsComponent = CSceneNode->GetComponentT<IModelsComponent3D>();
	if (modelsComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	auto bones = modelsComponent->GetBones();
	if (bones.empty())
		return EVisitResult::AllowVisitChilds;
	
	const auto& shaders = GetPipeline().GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

	m_Material->Bind(shaders);

	for (const auto& b : bones)
	{
		BindPerObjectData(PerObject(CSceneNode->GetWorldTransfom() * modelsComponent->GetModel()->GetFixSkeleton() * b->GetMatrix()));
		m_SphereGeometry->Render(vertexShader);
	}

	m_Material->Unbind(shaders);

	return EVisitResult::AllowAll;
}

EVisitResult CDrawBonesPass::Visit(const IModel * Model)
{
	return EVisitResult::Block;
}

EVisitResult CDrawBonesPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}
