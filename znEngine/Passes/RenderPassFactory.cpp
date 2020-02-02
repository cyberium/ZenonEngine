#include "stdafx.h"

// General
#include "RenderPassFactory.h"

// Additional
#include "RenderPassDefaultCreator.h"

CRenderPassFactory::CRenderPassFactory()
{
	AddRenderPassCreator(std::make_shared<CRenderPassDefaultCreator>());
}

CRenderPassFactory::~CRenderPassFactory()
{
}



//
// IRenderPassFactory
//
void CRenderPassFactory::AddRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetRenderPassCount(); i++)
	{
		m_Creators.push_back(Creator);
		Log::Print("CRenderPassFactory: RenderPassName '%s' registered.", Creator->GetRenderPassName(i).c_str());
	}
}

void CRenderPassFactory::RemoveRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator)
{
}

std::shared_ptr<IRenderPass> CRenderPassFactory::CreateRenderPass(std::string RenderPassName, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const
{
	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetRenderPassCount(); i++)
		{
			if (creator->GetRenderPassName(i) == RenderPassName)
			{
				return creator->CreateRenderPass(i, RenderDevice, RenderTarget, Viewport, Scene);
			}
		}
	}

	return nullptr;
}



//
// IznPluginsEventListener
//
void CRenderPassFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<IRenderPassCreator> creator = std::dynamic_pointer_cast<IRenderPassCreator>(Plugin))
	{
		AddRenderPassCreator(creator);
	}
}

void CRenderPassFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{}
