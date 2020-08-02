#pragma once

class ZN_API GenerateMipMapPass 
	: public RenderPass
{
public:
	GenerateMipMapPass(IRenderDevice& RenderDevice, IRenderTarget* renderTarget);
	GenerateMipMapPass(IRenderDevice& RenderDevice, ITexture* texture);
	virtual ~GenerateMipMapPass();

	virtual void Render(RenderEventArgs& e);

private:
	IRenderTarget* m_RenderTarget;
	ITexture* m_Texture;
};
