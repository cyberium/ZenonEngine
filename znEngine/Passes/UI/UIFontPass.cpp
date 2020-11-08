#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "Scene/Nodes/UIText.h"

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
std::shared_ptr<IRenderPassPipelined> CUIFontPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);

	// CreateShaders
	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
		g_pPixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
	}
	else if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", "");
		g_pPixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", "");
	}

	g_pVertexShader->LoadInputLayoutFromReflector();

	// Create samplers
	std::shared_ptr<ISamplerState> g_LinearClampSampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	GetPipeline().SetSampler(0, g_LinearClampSampler);

	// Material
	GetPipeline().SetShader(EShaderType::VertexShader, g_pVertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, g_pPixelShader);

	// Diryy hack!
	GetPipeline().SetTexture(0, GetBaseManager().GetManager<IznFontsManager>()->GetMainFont()->GetTexture());

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const IUIControl * node)
{
	const CUITextNode* textNode = dynamic_cast<const CUITextNode*>(node);
	if (textNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	BaseUIPass::Visit(node);

	const auto& font = textNode->GetFont();
	const auto& fontMaterial = textNode->GetMaterial();
	const auto& fontGeometry = font->GetGeometry();
	const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

	const auto& shaders = GetPipeline().GetShaders();
	auto vertexShader = shaders.at(EShaderType::VertexShader).get();

	fontGeometryInternal->Render_BindAllBuffers(vertexShader);
	{
		const std::string textToRender = textNode->GetText();
		glm::vec2 currentCharOffset = textNode->GetOffset();

		for (uint32 i = 0; i < textToRender.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = textNode->GetOffset().x;
				currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
				continue;
			}

			fontMaterial->SetOffset(currentCharOffset);
			fontMaterial->Bind(shaders);
			{
				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;
				fontGeometryInternal->Render_Draw(GeometryDrawArgs);
			}
			fontMaterial->Unbind(shaders);

			currentCharOffset.x += static_cast<float>(font->GetWidth(ch)) + 0.01f;
		}
	}
	fontGeometryInternal->Render_UnbindAllBuffers(vertexShader);

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIFontPass::Visit(const IModel * Model)
{
	_ASSERT(false);
	return EVisitResult::Block;
}
