#include "stdafx.h"

// General
#include "UIColorPass.h"

// Additional
#include "Scene/Nodes/UIColor.h"

CUIColorPass::CUIColorPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{}

CUIColorPass::~CUIColorPass()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIColorPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Color.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Color.hlsl", "PS_main");
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	// Create samplers
	std::shared_ptr<ISamplerState> sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(0, sampler);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIColorPass::Visit(const IUIControl * node)
{
	if (const CUIColorNode* textNode = dynamic_cast<const CUIColorNode*>(node))
		return BaseUIPass::Visit(textNode);

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIColorPass::Visit(const IModel * Model)
{
	return __super::Visit(Model);
}