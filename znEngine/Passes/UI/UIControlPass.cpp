#include "stdafx.h"

// General
#include "UIControlPass.h"

// Additional
#include "UIControls/UIControlCommon.h"
#include "UIControls/Common/UICommonModel.h"

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
	__super::ConfigurePipeline(RenderTarget);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UIControlCommon.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UIControlCommon.hlsl", "PS_main");

	// Material
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetShader(vertexShader);
	GetPipeline().SetShader(pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIControlPass::Visit(const std::shared_ptr<IUIControl>& node)
{
	if (auto textNode = std::dynamic_pointer_cast<const IUIControlCommon>(node))
	{
		PerObject perObject(node->GetWorldTransfom());
		//perObject.Model = glm::translate(perObject.Model, glm::vec3(subGeom->GetTranslate().x, subGeom->GetTranslate().y, 0.0f));
		//perObject.Model = glm::scale(perObject.Model, glm::vec3(subGeom->GetSize().x, subGeom->GetSize().y, 1.0f));

		BindPerObjectData(perObject);

		/*for (const auto& subGeom : textNode->GetSubgeometries())
		{


			__super::Visit(node);

			subGeom->GetMaterial()->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
			subGeom->GetGeom()->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), SGeometryDrawArgs());
			subGeom->GetMaterial()->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}*/

		return EVisitResult::AllowVisitChilds;
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIControlPass::Visit(const std::shared_ptr<IModel>& Model)
{
	if (auto commonModel = std::dynamic_pointer_cast<IUIControlCommonModel>(Model))
		return __super::Visit(commonModel);

	return EVisitResult::AllowAll;
}