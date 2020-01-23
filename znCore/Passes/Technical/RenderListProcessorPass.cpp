#include "stdafx.h"

// General
#include "RenderListProcessorPass.h"

RenderListProcessorPass::RenderListProcessorPass(IRenderDevice* RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass)
	: RenderPassPipelined(RenderDevice)
	, m_BuildRenderListPass(BuildRenderListPass)
{
}

RenderListProcessorPass::~RenderListProcessorPass()
{
}

void RenderListProcessorPass::Render(RenderEventArgs & e)
{
	for (const auto& it : m_BuildRenderListPass->GetGeometryList())
	{
		Visit(const_cast<ISceneNode3D*>(it.Node)); // TODO!!!
		Visit(const_cast<IGeometry*>(it.Geometry), it.Material, it.GeometryPartParams);
	}

	for (const auto& it : m_BuildRenderListPass->GetLightList())
	{
		Visit(const_cast<ISceneNode3D*>(it.Node)); // TODO!!!
		Visit(const_cast<ILightComponent3D*>(it.Light));
	}
}
