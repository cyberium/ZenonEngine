#include "stdafx.h"

// General
#include "ScenePassPipelined.h"


ScenePassPipelined::ScenePassPipelined(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
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
	auto scene = m_Scene.lock();
	_ASSERT(scene != nullptr);
	return scene;
}
