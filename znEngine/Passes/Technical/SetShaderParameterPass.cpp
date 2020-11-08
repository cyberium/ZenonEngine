#include "stdafx.h"

// General
#include "SetShaderParameterPass.h"

CSetShaderParameterPass::CSetShaderParameterPass(IRenderDevice& RenderDevice, IShaderParameter* Destination, std::shared_ptr<IShaderParameterSource> Source)
	: RenderPass(RenderDevice)
	, m_Destination(Destination)
	, m_Source(Source)
{
	_ASSERT(Destination != nullptr);
	_ASSERT(Source != nullptr);
}

CSetShaderParameterPass::~CSetShaderParameterPass()
{}



//
// IRenderPass
//
void CSetShaderParameterPass::Render(RenderEventArgs& e)
{
	if (m_Source != nullptr)
		m_Destination->SetSource(m_Source);
}
