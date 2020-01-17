#pragma once

#include "../RenderPass.h"

class ZN_API CopyTexturePass 
	: public RenderPass
{
public:
	CopyTexturePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<ITexture> destinationTexture, std::shared_ptr<ITexture> sourceTexture);
	virtual ~CopyTexturePass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<ITexture> m_SourceTexture;
	std::shared_ptr<ITexture> m_DestinationTexture;
};
