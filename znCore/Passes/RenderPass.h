#pragma once

class ZN_API RenderPass 
	: virtual public IRenderPass
{
public:
	RenderPass(IRenderDevice& RenderDevice);
	virtual ~RenderPass();

	// IRenderPass
	virtual void SetEnabled(bool Value) override final;
	virtual bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void PostRender(RenderEventArgs& e) override;

protected:
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;
	const RenderEventArgs& GetRenderEventArgs() const;

private:
	bool                                            m_Enabled;

	IBaseManager&                                   m_BaseManager;
	IRenderDevice&									m_RenderDevice;
	const RenderEventArgs*                          m_RenderEventArgs;
};