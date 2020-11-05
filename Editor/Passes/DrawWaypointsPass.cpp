#include "stdafx.h"

// General
#include "DrawWaypointsPass.h"

const float cBBoxSizeIncrement = 0.05f;

CDrawWaypointsPass::CDrawWaypointsPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(RenderDevice, Scene)
	, m_IsDirty(false)
{
}

CDrawWaypointsPass::~CDrawWaypointsPass()
{}

void CDrawWaypointsPass::SetNeedRefresh()
{
	m_IsDirty = true;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawWaypointsPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_PointBox = GetRenderDevice().GetPrimitivesFactory().CreateCube();

	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	
	//m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	m_MaterialDebug = MakeShared(MaterialDebug, GetRenderDevice());

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	return shared_from_this();
}

EVisitResult CDrawWaypointsPass::Visit(const ISceneNode * node)
{
	auto waypointSceneNode = dynamic_cast<const ISceneNodePath*>(node);
	if (waypointSceneNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	const auto& points = waypointSceneNode->GetPoints();
	std::vector<glm::vec3> pointsXYZ;
	std::for_each(points.begin(), points.end(), [&pointsXYZ](const std::shared_ptr<ISceneNodePoint>& Point) {
		pointsXYZ.push_back(Point->GetTranslation());
	});

	if (points.size() < 2)
		return EVisitResult::AllowVisitChilds;

	BindPerObjectParameter(glm::mat4(1.0f));

	m_WaypointGeometry = GetRenderDevice().GetPrimitivesFactory().CreateLines(pointsXYZ);

	m_MaterialDebug->Bind(GetPipeline().GetShaders());
	{
		const IShader* vertexShader = GetPipeline().GetShader(EShaderType::VertexShader).get();
		m_WaypointGeometry->Render(GetRenderEventArgs(), vertexShader);
	}
	m_MaterialDebug->Unbind(GetPipeline().GetShaders());

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CDrawWaypointsPass::Visit(const IModel * Model)
{
	_ASSERT(false);
	return EVisitResult::Block;
}
