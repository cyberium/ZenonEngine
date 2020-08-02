#pragma once

ZN_INTERFACE ZN_API IInvokeFunctionPass
{
	virtual ~IInvokeFunctionPass() {}

	virtual void SetFunc(std::function<void(void)> func) = 0;
};


class ZN_API InvokeFunctionPass 
	: public RenderPass
	, public IInvokeFunctionPass
{
public:
	InvokeFunctionPass(IRenderDevice& RenderDevice, std::function<void(void)> func);
	virtual ~InvokeFunctionPass();

	// IInvokeFunctionPass
	void SetFunc(std::function<void(void)> func) override;

	// RenderPass
	virtual void Render(RenderEventArgs& e) override;

private:
	std::function<void(void)> m_Func;
};