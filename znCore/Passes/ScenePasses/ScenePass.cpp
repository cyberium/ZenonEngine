#include "stdafx.h"

// General
#include "ScenePass.h"

ScenePass::ScenePass(IRenderDevice& RenderDevice, IScene& Scene)
	: RenderPass(RenderDevice)
	, m_Scene(Scene)
{}

ScenePass::~ScenePass()
{}

//
// IRenderPass
//
void ScenePass::Render(RenderEventArgs & e)
{
	GetScene().Accept(this);
}

//
// Protected
//
IScene& ScenePass::GetScene() const
{
	return m_Scene;
}
