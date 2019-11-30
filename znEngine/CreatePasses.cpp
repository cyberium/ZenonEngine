#include "stdafx.h"

// Include
#include "Passes/RenderTechnique.h"

// General
#include "CreatePasses.h"

// Additional
#include "Passes/BaseUIPass.h"

void AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<SceneUI> scene)
{
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IPipelineState> UIPipeline = device->CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
    UIPipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
    UIPipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	UIPipeline->SetRenderTarget(_renderTarget);
	UIPipeline->GetRasterizerState()->SetViewport(viewport);

	technique->AddPass(std::make_shared<BaseUIPass>(scene, UIPipeline));
}
