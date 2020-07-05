#pragma once

#include "../RenderPass.h"

class ZN_API CSetShaderParameterPass
	: public RenderPass
{
public:
	CSetShaderParameterPass(IRenderDevice& RenderDevice, IShaderParameter* Destination, std::shared_ptr<IShaderParameterSource> Source);
	virtual ~CSetShaderParameterPass();

	void Render(RenderEventArgs& e) override;

private:
	IShaderParameter* m_Destination;
	std::shared_ptr<IShaderParameterSource> m_Source;
};