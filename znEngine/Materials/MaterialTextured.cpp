#include "stdafx.h"

// General
#include "MaterialTextured.h"

MaterialTextured::MaterialTextured(IRenderDevice& RenderDevice)
	: MaterialDebug(RenderDevice)
{
	// Create samplers
	const auto& g_LinearClampSampler = RenderDevice.GetObjectsFactory().CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	SetSampler(0, g_LinearClampSampler);
}

MaterialTextured::~MaterialTextured()
{

}