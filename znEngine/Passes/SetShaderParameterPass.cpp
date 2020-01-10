#include "stdafx.h"

// General
#include "SetShaderParameterPass.h"

CSetShaderParameterPass::CSetShaderParameterPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IShaderParameter> Destination, std::function<std::shared_ptr<IShaderParameterSource>(void)> FuncGetSource)
	: AbstractPass(RenderDevice)
	, m_Destination(Destination)
	, m_FuncGetSource(FuncGetSource)
{
}

CSetShaderParameterPass::CSetShaderParameterPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IShaderParameter> Destination, std::shared_ptr<IShaderParameterSource> Source)
	: AbstractPass(RenderDevice)
	, m_Destination(Destination)
	, m_Source(Source)
{
}

CSetShaderParameterPass::~CSetShaderParameterPass()
{
}

void CSetShaderParameterPass::Render(RenderEventArgs& e)
{
	_ASSERT(m_Destination != nullptr);

	if (m_Source != nullptr)
		m_Destination->SetSource(m_Source.get());

	if (m_FuncGetSource != nullptr)
	{
		std::shared_ptr<IShaderParameterSource> sps = m_FuncGetSource();
		_ASSERT(sps != nullptr);
		m_Destination->SetSource(sps.get());
	}
}
