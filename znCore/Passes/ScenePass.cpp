#include "stdafx.h"

// General
#include "ScenePass.h"

ScenePass::ScenePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
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
	m_Scene->Accept(this);
}



//
// Protected
//
std::shared_ptr<IScene> ScenePass::GetScene() const
{
	return m_Scene;
}
