#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "UIControls/UIControlText.h"
#include "Formats/Fonts/FontModel.h"

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


namespace
{
	glm::vec2 CalculateOffsetByAlign(const glm::vec2 TextSize, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical)
	{
		glm::vec2 offsetByAlign = glm::vec2(0.0f);

		if (TextAlignHorizontal == ETextAlignHorizontal::Left)
			offsetByAlign.x = 0.0f;
		else if (TextAlignHorizontal == ETextAlignHorizontal::Center)
			offsetByAlign.x = -(TextSize.x / 2.0f);
		else if (TextAlignHorizontal == ETextAlignHorizontal::Right)
			offsetByAlign.x = -TextSize.x;

		if (TextAlignVertical == ETextAlignVertical::Top)
			offsetByAlign.y = 0.0f;
		else if (TextAlignVertical == ETextAlignVertical::Center)
			offsetByAlign.y = -(TextSize.y / 2.0f);
		else if (TextAlignVertical == ETextAlignVertical::Bottom)
			offsetByAlign.y = -TextSize.y;

		return offsetByAlign;
	}
}


//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const std::shared_ptr<IUIControl>& node)
{
	auto textNode = std::dynamic_pointer_cast<CUIControlText>(node);
	if (textNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	return __super::Visit(node);
}

EVisitResult CUIFontPass::Visit(const std::shared_ptr<IModel>& Model)
{
	if (auto fontModel = std::dynamic_pointer_cast<CFontModel>(Model))
	{
		fontModel->Render(GetPipeline().GetShaders());
	}

	return EVisitResult::AllowAll;
}
