#include "stdafx.h"

// General
#include "ScenePassPipelined.h"

ScenePassPipelined::ScenePassPipelined(IRenderDevice& RenderDevice, const std::weak_ptr<IScene>& Scene)
	: RenderPassPipelined(RenderDevice)
	, m_Scene(Scene)
{}

ScenePassPipelined::~ScenePassPipelined()
{}

//
// IRenderPass
//
void ScenePassPipelined::Render(RenderEventArgs& e)
{
	GetScene()->Accept(this);
}

//
// Protected
//
std::shared_ptr<IScene> ScenePassPipelined::GetScene() const
{
	return m_Scene.lock();
}
