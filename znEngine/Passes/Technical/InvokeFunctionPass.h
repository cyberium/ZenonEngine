#pragma once

class ZN_API InvokeFunctionPass 
	: public RenderPass
{
public:
	InvokeFunctionPass(IRenderDevice& RenderDevice, std::function<void(void)> func);
	virtual ~InvokeFunctionPass();

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

private:
	std::function<void(void)> m_Func;
};