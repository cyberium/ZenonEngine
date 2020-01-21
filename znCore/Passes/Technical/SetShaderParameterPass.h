#pragma once

#include "../RenderPass.h"

class CSetShaderParameterPass
	: public RenderPass
{
public:
	CSetShaderParameterPass(IRenderDevice* RenderDevice, IShaderParameter* Destination, std::function<IShaderParameterSource*(void)> FuncGetSource);
	CSetShaderParameterPass(IRenderDevice* RenderDevice, IShaderParameter* Destination, IShaderParameterSource* Source);
	virtual ~CSetShaderParameterPass();

	virtual void Render(RenderEventArgs& e);

private:
	IShaderParameter* m_Destination;
	std::function<IShaderParameterSource*(void)> m_FuncGetSource;
	IShaderParameterSource* m_Source;
};