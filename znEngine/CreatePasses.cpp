#include "stdafx.h"

// Include
#include "Passes/RenderTechnique.h"

// General
#include "CreatePasses.h"

// Additional
#include "Passes/BaseUIPass.h"

void AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<SceneUI> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	std::shared_ptr<PipelineState> UIPipeline = device->CreatePipelineState();
	UIPipeline->GetBlendState().SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState().SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
    UIPipeline->GetRasterizerState().SetAntialiasedLineEnable(true);
    UIPipeline->GetRasterizerState().SetMultisampleEnabled(true);
	UIPipeline->SetRenderTarget(_renderTarget);
	UIPipeline->GetRasterizerState().SetViewport(viewport);

	technique->AddPass(std::make_shared<BaseUIPass>(scene, UIPipeline));
}
