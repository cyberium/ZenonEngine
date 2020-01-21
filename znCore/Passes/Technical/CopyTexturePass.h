#pragma once

#include "../RenderPass.h"

class ZN_API CopyTexturePass 
	: public RenderPass
{
public:
	CopyTexturePass(IRenderDevice* RenderDevice, ITexture* destinationTexture, ITexture* sourceTexture);
	virtual ~CopyTexturePass();

	virtual void Render(RenderEventArgs& e);

private:
	ITexture* m_SourceTexture;
	ITexture* m_DestinationTexture;
};
