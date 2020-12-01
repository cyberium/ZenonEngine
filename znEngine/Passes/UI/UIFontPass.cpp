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



//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const IUIControl * node)
{
	const CUIControlText* textNode = dynamic_cast<const CUIControlText*>(node);
	if (textNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	BaseUIPass::Visit(node);

	const auto& textToRender = textNode->GetText();
	const auto& color = textNode->GetColor();
	const auto& font = textNode->GetFont();

	const auto& fontGeometry = font->GetGeometry();
	const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

	const auto& shaders = GetPipeline().GetShaders();
	auto vertexShader = shaders.at(EShaderType::VertexShader).get();
	auto pixelShader = shaders.at(EShaderType::PixelShader).get();

	font->GetTexture()->Bind(0, pixelShader, IShaderParameter::EType::Texture);

	fontGeometryInternal->Render_BindAllBuffers(vertexShader);
	{
		auto currentCharOffset = textNode->GetOffset();

		for (size_t i = 0; i < textToRender.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = textNode->GetOffset().x;
				currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
				continue;
			}

			{
				SFontPerCharacterData fontProperties;
				fontProperties.Color = color;
				fontProperties.Offset = currentCharOffset;
			
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
