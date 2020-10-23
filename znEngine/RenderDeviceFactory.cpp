#include "stdafx.h"

// General
#include "RenderDeviceFactory.h"

CznRenderDeviceFactory::CznRenderDeviceFactory(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{}

CznRenderDeviceFactory::~CznRenderDeviceFactory()
{
	OutputDebugStringW(L"Destroyed.");
	//m_BaseManager.GetManager<IznPluginsManager>()->RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener>(this));
}



//
// IznRenderDeviceFactory
//
void CznRenderDeviceFactory::RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	m_Creators.push_back(RenderDeviceCreator.get());
}

void CznRenderDeviceFactory::UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	const auto& it = std::find_if(m_Creators.begin(), m_Creators.end(), [&RenderDeviceCreator](const IznRenderDeviceCreator* RenderDeviceCreator0) -> bool { return RenderDeviceCreator0 == RenderDeviceCreator.get(); });
	if (it == m_Creators.end())
	{
		throw std::exception(("RenderDeviceCreator [" + std::to_string(1377) + "] not found!").c_str());
	}

	m_Creators.erase(it);
}

const IznRenderDeviceCreator& CznRenderDeviceFactory::GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const
{
	for (const auto& it : m_Creators)
		if (it->GetRenderDeviceType() == RenderDeviceType)
			return *it;

	throw std::exception(("RenderDeviceCreator [" + std::to_string(1377) + "] not found!").c_str());
}



//
// IznPluginsEventListener
//
void CznRenderDeviceFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{

}

void CznRenderDeviceFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<IznRenderDeviceCreator> creator = std::dynamic_pointer_cast<IznRenderDeviceCreator>(Plugin))
	{
		RegisterRenderDeviceCreator(creator);
	}
}

void CznRenderDeviceFactory::OnPluginFinalized(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<IznRenderDeviceCreator> creator = std::dynamic_pointer_cast<IznRenderDeviceCreator>(Plugin))
	{
		UnregisterRenderDeviceCreator(creator);
	}
}
