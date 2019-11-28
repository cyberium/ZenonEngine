#pragma once

class CznRenderDeviceCreatorFactory : public IznRenderDeviceCreatorFactory
{
public:

	// IznRenderDeviceCreatorFactory
	void											RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator);
	void											UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator);
	std::shared_ptr<IznRenderDeviceCreator>			GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const;

private:
	std::vector<std::shared_ptr<IznRenderDeviceCreator>> m_RenderDeviceCreators;
};