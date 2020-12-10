#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "UIControls/UIControlText.h"


CUIFontPass::CUIFontPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{
	m_FontBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SFontPerCharacterData());
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
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

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
			offsetByAlign.x = - (TextSize.x / 2.0f);
		else if (TextAlignHorizontal == ETextAlignHorizontal::Right)
			offsetByAlign.x = - TextSize.x;

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

	BaseUIPass::Visit(node);

	const auto& textToRender = textNode->GetText();
	const auto& textAlignHorizontal = textNode->GetTextAlignHorizontal();
	const auto& color = textNode->GetColor();
	const auto& font = textNode->GetFont();

	glm::vec2 offsetByAlign = CalculateOffsetByAlign(textNode->GetSize(), textNode->GetTextAlignHorizontal(), textNode->GetTextAlignVertical());

	const auto& fontGeometry = font->GetGeometry();
	const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

	const auto& shaders = GetPipeline().GetShaders();
	auto vertexShader = shaders.at(EShaderType::VertexShader).get();
	auto pixelShader = shaders.at(EShaderType::PixelShader).get();

	font->GetTexture()->Bind(0, pixelShader, IShaderParameter::EType::Texture);

	fontGeometryInternal->Render_BindAllBuffers(vertexShader);
	{
		auto currentCharOffset = glm::vec2(0.0f);

		for (size_t i = 0; i < textToRender.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = 0.0f;
				currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
				continue;
			}

			{
				SFontPerCharacterData fontProperties;
				fontProperties.Color = color;
				fontProperties.Offset = currentCharOffset + offsetByAlign;
			
				BindPerCharacterData(fontProperties);
			}

			{
				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;
				fontGeometryInternal->Render_Draw(GeometryDrawArgs);
			}

			currentCharOffset.x += static_cast<float>(font->GetWidth(ch)) + 0.01f;
		}
	}
	fontGeometryInternal->Render_UnbindAllBuffers(vertexShader);

	font->GetTexture()->UnBind(0, pixelShader, IShaderParameter::EType::Texture);

	return EVisitResult::AllowVisitChilds;
}



//
// Private
//
void CUIFontPass::BindPerCharacterData(const SFontPerCharacterData& PerCharacterData)
{
	m_FontBuffer->Set(PerCharacterData);

	for (const auto& shaderIt : GetPipeline().GetShaders())
	{
		auto* perCharacterParam = shaderIt.second->GetShaderParameterByName("PerCharacterData");
		if (perCharacterParam)
		{
			perCharacterParam->SetConstantBuffer(m_FontBuffer);
			perCharacterParam->Bind();
		}
	}
}
