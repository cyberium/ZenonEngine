#include "stdafx.h"

// General
#include "ScenePassPipelined.h"


ScenePassPipelined::ScenePassPipelined(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
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
	m_Scene->Accept(this);
}



//
// Protected
//
std::shared_ptr<IScene> ScenePassPipelined::GetScene() const
{
	return m_Scene;
}
