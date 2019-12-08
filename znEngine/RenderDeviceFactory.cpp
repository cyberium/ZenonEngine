#include "stdafx.h"

// General
#include "RenderDeviceFactory.h"

CznRenderDeviceFactory::CznRenderDeviceFactory(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CznRenderDeviceFactory::~CznRenderDeviceFactory()
{
	GetManager<IznPluginsManager>(m_BaseManager)->RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener>(this));
}

//
// IznRenderDeviceFactory
//
void CznRenderDeviceFactory::RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	m_Creators.push_back(RenderDeviceCreator);
}

void CznRenderDeviceFactory::UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	// TODO!!!
}

std::shared_ptr<IznRenderDeviceCreator> CznRenderDeviceFactory::GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const
{
	for (const auto& it : m_Creators)
		if (it->GetRenderDeviceType() == RenderDeviceType)
			return it;

	throw std::exception(("RenderDeviceCreator [" + std::to_string(1377) + "] not found!").c_str());
}



//
// IznPluginsEventListener
//
void CznRenderDeviceFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<IznRenderDeviceCreator> creator = std::dynamic_pointer_cast<IznRenderDeviceCreator>(Plugin))
	{
		RegisterRenderDeviceCreator(creator);
	}
}

void CznRenderDeviceFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{

}
