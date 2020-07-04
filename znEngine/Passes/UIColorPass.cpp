#include "stdafx.h"

// General
#include "UIColorPass.h"

// Additional
#include "SceneFunctional/UI/UIColor.h"

CUIColorPass::CUIColorPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: BaseUIPass(RenderDevice, Scene)
{}

CUIColorPass::~CUIColorPass()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIColorPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::CreatePipeline(RenderTarget, Viewport);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_UI_COLOR", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_UI_COLOR", "PS_main");

	// Create samplers
	std::shared_ptr<ISamplerState> sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	GetPipeline().SetSampler(0, sampler);

	// Material
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIColorPass::Visit(const ISceneNodeUI * node)
{
	if (const CUIColorNode* textNode = dynamic_cast<const CUIColorNode*>(node))
	{
		return BaseUIPass::Visit(node);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIColorPass::Visit(const IModel * Model)
{
	return __super::Visit(Model);
}