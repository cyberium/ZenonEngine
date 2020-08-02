#pragma once

class ZN_API CopyTexturePass 
	: public RenderPass
{
public:
	CopyTexturePass(IRenderDevice& RenderDevice, const std::shared_ptr<ITexture>& destinationTexture, const std::shared_ptr<ITexture>& sourceTexture);
	virtual ~CopyTexturePass();

	virtual void Render(RenderEventArgs& e);

private:
	const std::shared_ptr<ITexture>& m_SourceTexture;
	const std::shared_ptr<ITexture>& m_DestinationTexture;
};
