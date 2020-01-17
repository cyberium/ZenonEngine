#pragma once

#include "../RenderPass.h"

class CSetShaderParameterPass
	: public RenderPass
{
public:
	CSetShaderParameterPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IShaderParameter> Destination, std::function<std::shared_ptr<IShaderParameterSource>(void)> FuncGetSource);
	CSetShaderParameterPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IShaderParameter> Destination, std::shared_ptr<IShaderParameterSource> Source);
	virtual ~CSetShaderParameterPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<IShaderParameter> m_Destination;
	std::function<std::shared_ptr<IShaderParameterSource>(void)> m_FuncGetSource;
	std::shared_ptr<IShaderParameterSource> m_Source;
};