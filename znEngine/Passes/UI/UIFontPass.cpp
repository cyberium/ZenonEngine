#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "UIControls/UIControlText.h"
#include "UIControls/Text/UITextModel.h"

CUIFontPass::CUIFontPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{
}

CUIFontPass::~CUIFontPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIFontPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	BaseUIPass::ConfigurePipeline(RenderTarget);

	// CreateShaders
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
	}
	else if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", "");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", "");
	}

	vertexShader->LoadInputLayoutFromReflector();

	// Material
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetShader(vertexShader);
	GetPipeline().SetShader(pixelShader);

	return shared_from_this();
}


//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const std::shared_ptr<IUIControl>& node)
{
	if (auto textNode = std::dynamic_pointer_cast<IUIControlText>(node))
		return __super::Visit(node);

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIFontPass::Visit(const std::shared_ptr<IModel>& Model)
{
	if (auto fontModel = std::dynamic_pointer_cast<CUITextModel>(Model))
		return __super::Visit(fontModel);

	return EVisitResult::AllowAll;
}
