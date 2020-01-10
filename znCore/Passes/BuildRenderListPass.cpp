#include "stdafx.h"

// General
#include "BuildRenderListPass.h"

BuildRenderListPass::BuildRenderListPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{
}

BuildRenderListPass::~BuildRenderListPass()
{
}



//
// IRenderPass
//
void BuildRenderListPass::PreRender(RenderEventArgs & e)
{
	Base3DPass::PreRender(e);

	m_RenderList.clear();
}

void BuildRenderListPass::PostRender(RenderEventArgs & e)
{
	const ICamera* camera = GetRenderEventArgs()->Camera;
	_ASSERT(camera);

	for (const auto& it : m_RenderList)
	{
		m_PerObjectData->Model = it.Node->GetWorldTransfom();
		m_PerObjectData->View = camera->GetViewMatrix();
		m_PerObjectData->Projection = camera->GetProjectionMatrix();

		SetPerObjectConstantBufferData();

		ShaderMap shadersMap = it.ShadersMap;
		if (shadersMap.empty())
			shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

		it.Geometry->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer().get(), shadersMap, it.Material, it.GeometryPartParams);
	}


	Base3DPass::PostRender(e);
}


//
// IVisitor
//
bool BuildRenderListPass::Visit3D(ISceneNode * node)
{
	m_CurrentSceneNode = node;

	return true;
}

bool BuildRenderListPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return true;
}

bool BuildRenderListPass::Visit(IGeometry * Geometry, const IMaterial * Material, SGeometryPartParams GeometryPartParams)
{
	ShaderMap shadersMap;
	if (Material)
		shadersMap = Material->GetShaders();

	m_RenderList.push_back(SRenderListElement(m_CurrentSceneNode, Geometry, Material, shadersMap, GeometryPartParams));
	return false;
}
