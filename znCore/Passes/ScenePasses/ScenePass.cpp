#include "stdafx.h"

// General
#include "ScenePass.h"

ScenePass::ScenePass(IRenderDevice& RenderDevice, const std::weak_ptr<IScene>& Scene)
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
	GetScene()->Accept(this);
}

//
// Protected
//
std::shared_ptr<IScene> ScenePass::GetScene() const
{
	return m_Scene.lock();
}
