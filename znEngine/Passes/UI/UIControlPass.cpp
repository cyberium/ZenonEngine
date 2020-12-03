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
std::shared_ptr<IRenderPassPipelined> CUIControlPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	BaseUIPass::ConfigurePipeline(RenderTarget);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Texture.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Texture.hlsl", "PS_main");

	// Material
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIControlPass::Visit(const std::shared_ptr<IUIControl>& node)
{
	if (auto textNode = std::dynamic_pointer_cast<const CUIControlCommon>(node))
	{
		for (const auto& subGeom : textNode->GetSubgeometries())
		{
			PerObject perObject(node->GetWorldTransfom());
			perObject.Model = glm::translate(perObject.Model, glm::vec3(subGeom.Translate.x, subGeom.Translate.y, 0.0f));
			perObject.Model = glm::scale(perObject.Model, glm::vec3(subGeom.Size.x, subGeom.Size.y, 1.0f));

			BindPerObjectData(perObject);

			subGeom.Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
			subGeom.Geom->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), SGeometryDrawArgs());
			subGeom.Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}

		return EVisitResult::AllowVisitChilds;
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIControlPass::Visit(const std::shared_ptr<IModel>& Model)
{
	//return __super::Visit(Model);
	return EVisitResult::Block;
}