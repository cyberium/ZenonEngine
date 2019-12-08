#include "stdafx.h"

// General
#include "BaseScenePass.h"

CBaseScenePass::CBaseScenePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: AbstractPass(RenderDevice, Pipeline)
	, m_Scene(Scene)
{
}

CBaseScenePass::~CBaseScenePass()
{
}



//
// IRenderPass
//
void CBaseScenePass::Render(RenderEventArgs & e)
{
	m_Scene->Accept(this);
}



//
// Protected
//
std::shared_ptr<IScene> CBaseScenePass::GetScene() const
{
	return m_Scene;
}
