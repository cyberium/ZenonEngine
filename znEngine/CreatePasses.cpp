#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "Passes/TextureMaterialPass.h"

void OW_ENGINE_API Add3DPasses(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<IScene> scene)
{
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "VS_main", "latest"
		);
		g_pPixelShader = RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "PS_main", "latest"
		);
	}
	else
	{
		g_pVertexShader = RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", IShader::ShaderMacros(), "", ""
		);
		g_pPixelShader = RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", IShader::ShaderMacros(), "", ""
		);
	}
	//g_pVertexShader->LoadInputLayoutFromReflector();

	std::vector<SCustomVertexElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	// PIPELINES
	std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(_renderTarget);
	Pipeline->GetRasterizerState()->SetViewport(viewport);
	Pipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);

	technique->AddPass(std::make_shared<CTexturedMaterialPass>(RenderDevice, scene, Pipeline));
}

void AddUIPasses(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<IScene> scene)
{
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IPipelineState> UIPipeline = RenderDevice->CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
    UIPipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
    UIPipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	UIPipeline->SetRenderTarget(_renderTarget);
	UIPipeline->GetRasterizerState()->SetViewport(viewport);

	technique->AddPass(std::make_shared<BaseUIPass>(RenderDevice, scene, UIPipeline));
}
