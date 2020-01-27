#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "SceneFunctional/UI/Fonts/Font.h"

CUIFontPass::CUIFontPass(IRenderDevice * RenderDevice, std::shared_ptr<IScene> Scene)
	: BaseUIPass(RenderDevice, Scene)
{
}

CUIFontPass::~CUIFontPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIFontPass::CreatePipeline(IRenderTarget * RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::CreatePipeline(RenderTarget, Viewport);
	
	// CreateShaders
	IShader* g_pVertexShader;
	IShader* g_pPixelShader;

	if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_UI_FONT", IShader::ShaderMacros(), "VS_main", "latest"
		);

		g_pPixelShader = GetRenderDevice()->CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_UI_FONT", IShader::ShaderMacros(), "PS_main", "latest"
		);
	}
	else if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", IShader::ShaderMacros(), "", ""
		);

		g_pPixelShader = GetRenderDevice()->CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", IShader::ShaderMacros(), "", ""
		);
	}

	g_pVertexShader->LoadInputLayoutFromReflector();

	// Create samplers
	ISamplerState* g_LinearClampSampler = GetRenderDevice()->CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	GetPipeline()->SetSampler(0, g_LinearClampSampler);

	// Material
	GetPipeline()->SetShader(EShaderType::VertexShader, g_pVertexShader);
	GetPipeline()->SetShader(EShaderType::PixelShader, g_pPixelShader);

	// Diryy hack!
	GetPipeline()->SetTexture(0, GetBaseManager()->GetManager<IFontsManager>()->GetMainFont()->GetTexture());

	return shared_from_this();
}



//
// IVisitor
//
bool CUIFontPass::Visit(ISceneNodeUI * node)
{
	return BaseUIPass::Visit(node);
}

bool CUIFontPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	if (CFontMesh* fontMesh = dynamic_cast<CFontMesh*>(Mesh))
	{
		return BaseUIPass::Visit(Mesh, GeometryPartParams);
	}

	return false;
}
