#pragma once

#include "../RenderPass.h"

class CSetShaderParameterPass
	: public RenderPass
{
public:
	CSetShaderParameterPass(IRenderDevice& RenderDevice, IShaderParameter* Destination, std::function<std::shared_ptr<IShaderParameterSource>(void)> FuncGetSource);
	CSetShaderParameterPass(IRenderDevice& RenderDevice, IShaderParameter* Destination, std::shared_ptr<IShaderParameterSource> Source);
	virtual ~CSetShaderParameterPass();

	virtual void Render(RenderEventArgs& e);

private:
	IShaderParameter* m_Destination;
	std::function<std::shared_ptr<IShaderParameterSource>(void)> m_FuncGetSource;
	std::shared_ptr<IShaderParameterSource> m_Source;
};