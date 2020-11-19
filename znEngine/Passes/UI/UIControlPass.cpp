#include "stdafx.h"

// General
#include "UIControlPass.h"

// Additional
#include "UIControls/UIControlCommon.h"

CUIControlPass::CUIControlPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{}

CUIControlPass::~CUIControlPass()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIControlPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Texture.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Texture.hlsl", "PS_main");

	// Material
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIControlPass::Visit(const IUIControl * node)
{
	if (const CUIControlCommon* textNode = dynamic_cast<const CUIControlCommon*>(node))
	{
		BaseUIPass::Visit(node);

		const auto& shaders = GetPipeline().GetShaders();
		const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

		for (const auto& subGeom : textNode->GetSubgeometries())
		{
			subGeom.Material->Bind(shaders);
			subGeom.Geom->Render(vertexShader, SGeometryDrawArgs());
			subGeom.Material->Unbind(shaders);
		}

		return EVisitResult::AllowVisitChilds;
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIControlPass::Visit(const IModel * Model)
{
	//return __super::Visit(Model);
	return EVisitResult::Block;
}