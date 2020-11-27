#include "stdafx.h"

// General
#include "RendererBase.h"

RendererBase::RendererBase(IBaseManager& BaseManager, IScene& Scene)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_Scene(Scene)
{}

RendererBase::~RendererBase()
{}

void RendererBase::Add3DPass(std::shared_ptr<IRenderPass> pass)
{
	m_3DPasses.push_back(pass);
}

void RendererBase::AddUIPass(std::shared_ptr<IRenderPass> pass)
{
	m_UIPasses.push_back(pass);
}

void RendererBase::Render3D(RenderEventArgs & renderEventArgs)
{
	for (auto pass : m_3DPasses)
	{
		if (pass && pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

void RendererBase::RenderUI(RenderEventArgs& renderEventArgs)
{
	for (auto pass : m_UIPasses)
	{
		if (pass && pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

void RendererBase::Resize(glm::ivec2 Size)
{
	for (const auto& pass : m_3DPasses)
		if (auto renderPassPipelined = std::dynamic_pointer_cast<IRenderPassPipelined>(pass))
			renderPassPipelined->GetPipeline().GetRenderTarget()->Resize(Size.x, Size.y);

	for (const auto& pass : m_UIPasses)
		if (auto renderPassPipelined = std::dynamic_pointer_cast<IRenderPassPipelined>(pass))
			renderPassPipelined->GetPipeline().GetRenderTarget()->Resize(Size.x, Size.y);
}
