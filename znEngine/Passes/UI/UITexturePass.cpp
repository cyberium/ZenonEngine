#include "stdafx.h"

// General
#include "UITexturePass.h"

// Additional
#include "Scene/Nodes/UITexture.h"

CUITexturePass::CUITexturePass(IRenderDevice& RenderDevice, std::weak_ptr<IScene> Scene)
	: BaseUIPass(RenderDevice, Scene)
{}

CUITexturePass::~CUITexturePass()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUITexturePass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Texture.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Texture.hlsl", "PS_main");

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
EVisitResult CUITexturePass::Visit(const ISceneNodeUI * node)
{
	if (const CUITextureNode* textNode = dynamic_cast<const CUITextureNode*>(node))
	{
		return BaseUIPass::Visit(node);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUITexturePass::Visit(const IModel * Model)
{
	return __super::Visit(Model);
}