#include "stdafx.h"

// General
#include "ScenePassPipelined.h"

ScenePassPipelined::ScenePassPipelined(IScene& Scene)
	: RenderPassPipelined(Scene.GetRenderDevice())
	, m_Scene(Scene)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

ScenePassPipelined::~ScenePassPipelined()
{}



//
// IRenderPass
//
void ScenePassPipelined::Render(RenderEventArgs& e)
{
	GetScene().Accept(this);
}



//
// IVisitor
//
EVisitResult ScenePassPipelined::Visit(const ISceneNode* node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const IUIControl* node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const IModel* Model)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const ISceneNodeComponent * Component)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const ILight3D* light)
{
	return EVisitResult::Block;
}

EVisitResult ScenePassPipelined::Visit(const IParticleSystem * ParticleSystem)
{
	return EVisitResult::Block;
}



//
// Protected
//
void ScenePassPipelined::BindPerObjectData(const PerObject& PerObject)
{
	m_PerObjectConstantBuffer->Set(PerObject);

	for (const auto& shaderIt : GetPipeline().GetShaders())
	{
		auto* perObjectParam = shaderIt.second->GetShaderParameterByName("PerObject");
		if (perObjectParam)
		{
			perObjectParam->SetConstantBuffer(m_PerObjectConstantBuffer);
			perObjectParam->Bind();
		}
	}
}

IScene& ScenePassPipelined::GetScene() const
{
	return m_Scene;
}
