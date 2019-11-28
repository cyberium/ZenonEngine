#include "stdafx.h"

// General
#include "RenderDeviceCreatorFactory.h"

void CznRenderDeviceCreatorFactory::RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	m_RenderDeviceCreators.push_back(RenderDeviceCreator);
}

void CznRenderDeviceCreatorFactory::UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator)
{
	// TODO!!!
}

std::shared_ptr<IznRenderDeviceCreator> CznRenderDeviceCreatorFactory::GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const
{
	for (const auto& it : m_RenderDeviceCreators)
		if (it->GetRenderDeviceType() == RenderDeviceType)
			return it;

	throw std::exception(("RenderDeviceCreator [" + std::to_string(RenderDeviceType) + "] not found!").c_str());
}
