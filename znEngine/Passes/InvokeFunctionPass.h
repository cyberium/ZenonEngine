#pragma once

class OW_ENGINE_API InvokeFunctionPass : public AbstractPass
{
public:
	InvokeFunctionPass(std::shared_ptr<IRenderDevice> RenderDevice, std::function<void(void)> func);
	virtual ~InvokeFunctionPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::function<void(void)> m_Func;
};