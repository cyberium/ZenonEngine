#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "SceneFunctional/UI/UIText.h"

CUIFontPass::CUIFontPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: BaseUIPass(RenderDevice, Scene)
{
}

CUIFontPass::~CUIFontPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIFontPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::CreatePipeline(RenderTarget, Viewport);
	
	// CreateShaders
	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_UI_FONT", "VS_main"
		);

		g_pPixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_UI_FONT", "PS_main"
		);
	}
	else if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", ""
		);

		g_pPixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", ""
		);
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
	GetPipeline().SetTexture(0, GetBaseManager().GetManager<IFontsManager>()->GetMainFont()->GetTexture());

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const ISceneNodeUI * node)
{
	if (const CUITextNode* textNode = dynamic_cast<const CUITextNode*>(node))
	{
		BaseUIPass::Visit(node);

		const std::shared_ptr<CFont>& font = textNode->GetFont();
		const std::shared_ptr<UI_Font_Material>& mat = textNode->GetMaterial();

		mat->Bind(GetPipeline().GetShaders());
		{
			const std::string _text = textNode->GetText();
			glm::vec2 _offset = textNode->GetOffset();

			for (uint32 i = 0; i < _text.length(); i++)
			{
				uint8 ch = _text.c_str()[i];
				mat->SetOffset(_offset);
				mat->Bind(GetPipeline().GetShaders());
				_offset.x += static_cast<float>(font->GetCharWidth(ch)) + 0.01f;

				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;

				font->GetGeometry()->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);
			}
		}
		mat->Unbind(GetPipeline().GetShaders());
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIFontPass::Visit(const IModel * Model)
{
	_ASSERT(false);
	return EVisitResult::Block;
}
